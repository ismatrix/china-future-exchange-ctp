/*
 * =====================================================================================
 *
 *       Filename:  Attr_API_Var.c
 *
 *    Description:  带变量上报api
 *					1) 支持替换attr名称中的变量($dec/$hex: 分别会替换为10进制或16进制的数字)
 *					2) 变量是由第二个参数传入
 *					3) 原子操作保证较高的准确性
 *					4) 存在更新冲突的概率, 但是概率很低
 *
 *        Version:  1.0
 *        Created:  04/29/2014 14:53:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sunnyhao
 *        Company:  Tencent
 *
 * =====================================================================================
 */
#include "comm_def.h"
#include "optimized_time.h"

// 0x5fee = 24558
#define VAR_SUM_SHM_KEY 0x5e01
#define VAR_SET_SHM_KEY 0x5e02

// 定义hash的长度和阶数
#define VAR_SHM_HASH_LEN 1000
#define VAR_SHM_HASH_TIMES 40

// 定义写冲突重试的最大次数
#define VAR_MAX_ATTEMPT_TIMES 50

// 定义素数表
static const int g_arrMod[VAR_SHM_HASH_TIMES] = {998, 997, 991, 983, 982, 977, 976, 974, 971, 967, 
	964, 958, 956, 953, 947, 944, 941, 937, 934, 932, 929, 926, 922, 919, 916, 914, 911, 908, 907, 904, 
	898, 892, 887, 886, 883, 881, 878, 877, 872, 866};

// 定义新agent共享内存中的节点结构
typedef struct
{
	union
	{
		struct 
		{
			int iAttrId;                                /* 属性id */
			int iVarKey;                                /* 变量key(可以为负数) */
		};
		
		uint64_t key_number;
	};

	union
	{
		struct 
		{
			int iValue;                                 /* 对应的值 */
			int iLastTime;                              /* 最近上报时间 */
		};

		uint64_t val_number;
	};
} SVarNode;

const int VAR_SHM_SIZE = sizeof(SVarNode) * VAR_SHM_HASH_LEN * VAR_SHM_HASH_TIMES;

// 全局变量
static void* g_pSumShm = NULL;
static void* g_pSetShm = NULL;

static inline int CalcKey(int iAttrId, int iVarKey, int iMod)
{
	return ( ((uint64_t)iAttrId << 32u)|(uint64_t)iVarKey ) % iMod;
}

static void DetachShm(void)
{
	if ( g_pSumShm != NULL )
	{
		shmdt(g_pSumShm);
		g_pSumShm = NULL;
	}

	if ( g_pSetShm != NULL )
	{
		shmdt(g_pSetShm);
		g_pSetShm = NULL;
	}
}

static int iInitRet = 0;
static void InitShm(void) 
{
	// 初始挂载时加锁(仅执行一次)
	if ( OI_GetShm2( (void **)&g_pSumShm, VAR_SUM_SHM_KEY, VAR_SHM_SIZE, 0666|IPC_CREAT) < 0 ) 
	{
		g_pSumShm = NULL;
		iInitRet = -1;
	}

	// 初始挂载时加锁(仅执行一次)
	if ( OI_GetShm2( (void **)&g_pSetShm, VAR_SET_SHM_KEY, VAR_SHM_SIZE, 0666|IPC_CREAT) < 0 ) 
	{
		g_pSetShm = NULL;
		iInitRet = -1;
	}

	// 退出的时候卸载共享内存
	atexit(DetachShm);
}

static spinlock_t lock = SPINLOCK_INITIALIZER;

static void InitOnce(void)
{
	spinlock_lock(&lock);                       /* 加锁 */
	InitShm();
	spinlock_unlock(&lock);                     /* 解锁 */
}

typedef int (*UPDATE_METHOD_PTR)(volatile SVarNode*, int, uint32_t, uint32_t*);

static inline int callback_AddValue(volatile SVarNode* pNode, int iCurTime, uint32_t dwNewVal, uint32_t *pdwValue)
{
	volatile SVarNode stNodeCopy = *pNode;

	// 判断 CAS 更新节点操作是否成功
	volatile SVarNode stNewNode;
	stNewNode.iValue = stNodeCopy.iValue + dwNewVal;
	stNewNode.iLastTime = ((uint32_t)iCurTime | 0x80000000u); /* 设置最高位为1, 表示using状态 */

	// 判断 CAS 更新节点操作是否成功
	if ( !CAS_64bit(&pNode->val_number, stNodeCopy.val_number, stNewNode.val_number) )
	{
		return 2;
	}

	if ( NULL != pdwValue )
	{
		*pdwValue = stNewNode.iValue;
	}

	return 0;
}

static inline int callback_SetValue(volatile SVarNode* pNode, int iCurTime, uint32_t dwNewVal, uint32_t *pdwValue)
{
	volatile SVarNode stNodeCopy = *pNode;

	// 判断 CAS 更新节点操作是否成功
	volatile SVarNode stNewNode;
	stNewNode.iValue = dwNewVal;
	stNewNode.iLastTime = ((uint32_t)iCurTime | 0x80000000u); /* 设置最高位为1, 表示using状态 */

	// 判断 CAS 更新节点操作是否成功
	if ( !CAS_64bit(&pNode->val_number, stNodeCopy.val_number, stNewNode.val_number) )
	{
		return 2;
	}

	if ( NULL != pdwValue )
	{
		*pdwValue = stNewNode.iValue;
	}

	return 0;
}

// 更新 attri_id 对应的节点, 如果不存在就创建一个新节点
int CreateOrUpdateNode(volatile void* pShm, int iAttrId, int iVarKey, uint32_t dwNewVal, UPDATE_METHOD_PTR UpdateNode, uint32_t* pdwValue)
{
	// 确保已经初始化过共享内存
	if ( !pShm || iAttrId <= 0 )
	{
		return -2;
	}

	int i = 0, j = 0;
	uint32_t dwHashVal = 0;
	int iCurTime = optimized_time(NULL);        /* 32bit系统直接用time性能较差 */
	volatile SVarNode *pNode = NULL;

	// 记录空节点作为二次备选节点
	int iEmptyNodeSize = 0;
	volatile SVarNode* arrEmptyNodeAddr[VAR_SHM_HASH_TIMES] = {NULL}; /* 初始化全为NULL */

	// 尝试查找节点
	for ( i = 0; i < VAR_SHM_HASH_TIMES; i++ )
	{
		dwHashVal = CalcKey(iAttrId, iVarKey, g_arrMod[i]);
		pNode = (volatile SVarNode*)pShm + VAR_SHM_HASH_LEN*i + dwHashVal;

		if ( 0 == pNode->key_number )				/* 记录(key==0)的节点列表 */
		{
			arrEmptyNodeAddr[iEmptyNodeSize++] = pNode;
		}
		else
		{
			if ( pNode->iAttrId == iAttrId && pNode->iVarKey == iVarKey )  /* 找到对应节点 */
			{
				for ( j = 0; j < VAR_MAX_ATTEMPT_TIMES; j++ )
				{
					if ( UpdateNode(pNode, iCurTime, dwNewVal, pdwValue) == 0 ) /* 更新失败, 循环重试 */
					{
						break;
					}
				}

				if ( j >= VAR_MAX_ATTEMPT_TIMES )
					return -3;

				return 0;
			}
		}
	}

	// 尝试新建节点
	volatile SVarNode stNewNode;
	stNewNode.iAttrId = iAttrId;
	stNewNode.iVarKey = iVarKey;
	stNewNode.iValue = dwNewVal;
	stNewNode.iLastTime = iCurTime;

	volatile SVarNode stEmptyNode;
	stEmptyNode.key_number = 0;
	stEmptyNode.val_number = 0;

	// 1) 尝试在key==0的空闲节点上新建节点
	for ( i = 0; i < iEmptyNodeSize; i++ )
	{
		pNode = arrEmptyNodeAddr[i];

		// 判断 CAS 新增节点操作是否成功
		if ( CAS_64bit(&pNode->key_number, stEmptyNode.key_number, stNewNode.key_number) 
				|| (pNode->iAttrId == iAttrId && pNode->iVarKey == iVarKey) ) /* CAS_64bit若返回false则代表节点被其他线程或进程抢占，再判断被抢后是否key赋值一致，若一致仍可进行更新. */
		{
			// CAS 更新value
			for ( j = 0; j < VAR_MAX_ATTEMPT_TIMES; j++ )
			{
				if ( UpdateNode(pNode, iCurTime, dwNewVal, pdwValue) == 0 ) /* 更新失败, 循环重试 */
				{
					break;
				}
			}

			if ( j >= VAR_MAX_ATTEMPT_TIMES )
				return -3;

			return 0;
		}
		// 如果节点是被其他属性对应的节点占用, 那么就继续循环查找
	}

	// 2) 所有空节点都找不到，则尝试在24小时内的过期节点上新建节点(defence code. only be executed when no empty node avaiable)
	// 有损服务, 一定概率下会导致上报不准确, 不过保证概率尽可能低
	volatile SVarNode stNodeCopy;
	for ( i = VAR_SHM_HASH_TIMES-1; i >= 0; i-- )
	{
		dwHashVal = CalcKey(iAttrId, iVarKey, g_arrMod[i]);
		pNode = (volatile SVarNode*)pShm + VAR_SHM_HASH_LEN*i + dwHashVal;

		stNodeCopy = *pNode;
		stNodeCopy.iLastTime &= (~0x80000000u); /* 不加亦可，落到此处逻辑，必然被agent重新设置过最高位. */
		if ( stNodeCopy.iLastTime > iCurTime - 24*60*60 && stNodeCopy.iLastTime < iCurTime - 5*60 ) /* 上次上报时间距今5分钟到24小时内 */
		{
			// 判断 CAS 新增节点操作是否成功
			if ( CAS_64bit(&pNode->key_number, stNodeCopy.key_number, stNewNode.key_number) 
					|| (pNode->iAttrId == iAttrId && pNode->iVarKey == iVarKey) )
			{
				// 尝试覆盖老数据
				if ( CAS_64bit(&pNode->val_number, stNodeCopy.val_number, stNewNode.val_number) )
				{
					return 0;
				}

				// 如果覆盖值失败, 说明有其他进程或者线程已经覆盖成功, 下面更新值即可
				// CAS 更新value
				for ( j = 0; j < VAR_MAX_ATTEMPT_TIMES; j++ )
				{
					if ( UpdateNode(pNode, iCurTime, dwNewVal, pdwValue) == 0 ) /* 更新失败, 循环重试 */
					{
						break;
					}
				}

				if ( j >= VAR_MAX_ATTEMPT_TIMES )
					return -3;

				return 0;
			}

			// 如果节点是被其他属性对应的节点占用, 那么就继续循环查找
		}
	}

	// 所有尝试都失败
	return -3;
}

int Attr_API_Var(int iAttrId, int iVarKey, int iValue)
{
	// 挂载共享内存
	if ( NULL == g_pSumShm )
	{
		InitOnce();                             /* 线程安全 */
		if ( 0 != iInitRet || NULL == g_pSumShm )
		{
			return -1;
		}
	}

	return CreateOrUpdateNode(g_pSumShm, iAttrId, iVarKey, iValue, callback_AddValue, NULL);
}

int Attr_API_Var_EX(int iAttrId, int iVarKey, int iValue, int *piCurValue)
{
	// 挂载共享内存
	if ( NULL == g_pSumShm )
	{
		InitOnce();                             /* 线程安全 */
		if ( 0 != iInitRet || NULL == g_pSumShm )
		{
			return -1;
		}
	}

	return CreateOrUpdateNode(g_pSumShm, iAttrId, iVarKey, iValue, callback_AddValue, (uint32_t*)piCurValue);
}

int Attr_API_Set_Var(int iAttrId, int iVarKey, int iValue)
{
	// 挂载共享内存
	if ( NULL == g_pSetShm )
	{
		InitOnce();                             /* 线程安全 */
		if ( 0 != iInitRet || NULL == g_pSetShm )
		{
			return -1;
		}
	}

	return CreateOrUpdateNode(g_pSetShm, iAttrId, iVarKey, iValue, callback_SetValue, NULL);
}

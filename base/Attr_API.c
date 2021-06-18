/*
 * =====================================================================================
 *
 *       Filename:  Attr_API.c
 *
 *    Description:  加强版的属性上报api
 *					1) 保证原子操作, 支持多线程/多进程并发调用
 *					2) hash方式插入共享内存, 时间复杂度降低为准O(1)
 *					3) 支持最多1w个属性的上报
 *					4) 相对于monitor而言, 保持对新旧api的兼容, 用户可以无感知的替换成新的api
 *
 *        Version:  1.0
 *        Created:  12/24/2012 04:53:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sunnyhao
 *        Company:  Tencent
 *
 * =====================================================================================
 */
#include "comm_def.h"

// 0x5fee = 24558
#define AGENT_ATTR_SUM_SHM_KEY 0x5fee
#define AGENT_ATTR_SET_SHM_KEY 0x5fef

// 定义hash的长度和阶数
#define AGENT_ATTR_SHM_HASH_LEN 1000
#define AGENT_ATTR_SHM_HASH_TIMES 40

// 定义写冲突重试的最大次数
#define MAX_ATTEMPT_TIMES 50

// 定义素数表
static const int g_arrMod[AGENT_ATTR_SHM_HASH_TIMES] = {998, 997, 991, 983, 982, 977, 976, 974, 971, 967, 
	964, 958, 956, 953, 947, 944, 941, 937, 934, 932, 929, 926, 922, 919, 916, 914, 911, 908, 907, 904, 
	898, 892, 887, 886, 883, 881, 878, 877, 872, 866};

// 定义新agent共享内存中的节点结构
typedef union
{
	struct                                      /* 可以按照一级成员使用 */
	{
		uint32_t dwAttrId;
		uint32_t dwCurVal;
	};

	uint64_t number_value;

} AGENT_ATTR_NODE;

const int AGENT_ATTR_SHM_SIZE = sizeof(AGENT_ATTR_NODE) * AGENT_ATTR_SHM_HASH_LEN * AGENT_ATTR_SHM_HASH_TIMES;

// 全局变量
static void* g_pSumShm = NULL;
static void* g_pSetShm = NULL;

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
	if ( OI_GetShm2( (void **)&g_pSumShm, AGENT_ATTR_SUM_SHM_KEY, AGENT_ATTR_SHM_SIZE, 0666|IPC_CREAT) < 0 ) 
	{
		g_pSumShm = NULL;
		iInitRet = -1;
	}

	// 初始挂载时加锁(仅执行一次)
	if ( OI_GetShm2( (void **)&g_pSetShm, AGENT_ATTR_SET_SHM_KEY, AGENT_ATTR_SHM_SIZE, 0666|IPC_CREAT) < 0 ) 
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

typedef int (*UPDATE_METHOD_PTR)(volatile AGENT_ATTR_NODE*, uint32_t, uint32_t, uint32_t*);

static inline int callback_AddValue(volatile AGENT_ATTR_NODE* pNode, uint32_t dwAttrId, uint32_t dwNewVal, uint32_t *pdwValue)
{
	volatile AGENT_ATTR_NODE stNodeCopy = *pNode;

	// 判断 stNodeCopy 获取值之前, pNode 中的内容被其他线程/进程修改过
	if ( stNodeCopy.dwAttrId != dwAttrId )
	{
		return 1;
	}

	// 判断 CAS 更新节点操作是否成功
	volatile AGENT_ATTR_NODE stNewNode;
	stNewNode.dwAttrId = dwAttrId;
	stNewNode.dwCurVal = stNodeCopy.dwCurVal + dwNewVal;

	// 判断 CAS 更新节点操作是否成功
	if ( !CAS_64bit(&pNode->number_value, stNodeCopy.number_value, stNewNode.number_value) )
	{
		return 2;
	}

	if ( NULL != pdwValue )
	{
		*pdwValue = stNewNode.dwCurVal;
	}

	return 0;
}

static inline int callback_SetValue(volatile AGENT_ATTR_NODE* pNode, uint32_t dwAttrId, uint32_t dwNewVal, uint32_t *pdwValue)
{
	volatile AGENT_ATTR_NODE stNodeCopy = *pNode;

	// 判断 stNodeCopy 获取值之前, pNode 中的内容被其他线程/进程修改过
	if ( stNodeCopy.dwAttrId != dwAttrId )
	{
		return 1;
	}

	// 判断 CAS 更新节点操作是否成功
	volatile AGENT_ATTR_NODE stNewNode;
	stNewNode.dwAttrId = dwAttrId;
	stNewNode.dwCurVal = dwNewVal;

	if ( !CAS_64bit(&pNode->number_value, stNodeCopy.number_value, stNewNode.number_value) ) 
	{
		return 2;
	}

	if ( NULL != pdwValue )
	{
		*pdwValue = stNewNode.dwCurVal;
	}

	return 0;
}

// 更新 attri_id 对应的节点, 如果不存在就创建一个新节点
static int CreateOrUpdateNode(volatile void* pShm, uint32_t dwAttrId, uint32_t dwNewVal, UPDATE_METHOD_PTR UpdateNode, uint32_t dwRecurDepth, uint32_t* pdwValue)
{
	if ( dwRecurDepth > MAX_ATTEMPT_TIMES )     /* 超过最大尝试次数, 就停止递归, 防止出现死循环 */
	{
		return -1;
	}

	// 确保已经初始化过共享内存
	if ( !pShm || dwAttrId == 0 )
	{
		return -2;
	}

	int i = 0;
	uint32_t dwHashVal = 0;
	volatile AGENT_ATTR_NODE *pNode = NULL;

	// 记录空节点作为二次备选节点
	int iEmptyNodeSize = 0;
	volatile AGENT_ATTR_NODE* arrEmptyNodeAddr[AGENT_ATTR_SHM_HASH_TIMES] = {NULL}; /* 初始化全为NULL */

	// 尝试查找节点
	for ( i = 0; i < AGENT_ATTR_SHM_HASH_TIMES; i++ )
	{
		dwHashVal = dwAttrId % g_arrMod[i];
		pNode = (volatile AGENT_ATTR_NODE*)pShm + AGENT_ATTR_SHM_HASH_LEN*i + dwHashVal;

		if ( 0 == pNode->dwAttrId )				/* 记录(attrid==0)的节点列表 */
		{
			arrEmptyNodeAddr[iEmptyNodeSize++] = pNode;
		}
		else
		{
			if ( pNode->dwAttrId == dwAttrId )  /* 找到对应节点 */
			{
				if ( UpdateNode(pNode, dwAttrId, dwNewVal, pdwValue) > 0 ) /* 更新失败, 递归重试 */
				{
					return CreateOrUpdateNode(pShm, dwAttrId, dwNewVal, UpdateNode, dwRecurDepth+1, pdwValue);
				}

				return 0;
			}
		}
	}

	// 尝试新建节点
	volatile AGENT_ATTR_NODE stNewNode;
	stNewNode.dwAttrId = dwAttrId;
	stNewNode.dwCurVal = dwNewVal;

	volatile AGENT_ATTR_NODE stEmptyNode;
	stEmptyNode.number_value = 0;

	// 1) 尝试在attrid==0的空闲节点上新建节点
	if ( iEmptyNodeSize > 0 )                   /* 存在attrid==0的空闲节点 */
	{
		for ( i = 0; i < iEmptyNodeSize; i++ )
		{
			// 判断 CAS 新增节点操作是否成功
			if ( CAS_64bit(&arrEmptyNodeAddr[i]->number_value, stEmptyNode.number_value, stNewNode.number_value) )
			{
				return 0;
			}

			// 如果节点已经被其他进程/线程创建, 那么就递归重试(更新)
			if ( arrEmptyNodeAddr[i]->dwAttrId == dwAttrId )
			{
				return CreateOrUpdateNode(pShm, dwAttrId, dwNewVal, UpdateNode, dwRecurDepth+1, pdwValue);
			}

			// 如果节点是被其他属性对应的节点占用, 那么就继续循环查找
		}
	}

	// 2) 尝试在value==0的空闲节点上新建节点
	if ( iEmptyNodeSize == 0 )					/* 没有attrid==0的空闲节点 */
	{
		volatile AGENT_ATTR_NODE stNodeCopy;
		for ( i = AGENT_ATTR_SHM_HASH_TIMES-1; i >= 0; i-- )
		{
			dwHashVal = dwAttrId % g_arrMod[i];
			pNode = (volatile AGENT_ATTR_NODE*)pShm + AGENT_ATTR_SHM_HASH_LEN*i + dwHashVal;

			stNodeCopy = *pNode;
			if ( 0 == stNodeCopy.dwCurVal )
			{
				// 判断 CAS 新增节点操作是否成功
				if ( CAS_64bit(&pNode->number_value, stNodeCopy.number_value, stNewNode.number_value))
				{
					return 0;
				}

				// 如果节点是被其他属性对应的节点占用, 那么就继续循环查找
			}
		}
	}

	// 所有尝试都失败
	return -3;
}

int Attr_API(int iAttrId, int iValue)
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

	return CreateOrUpdateNode(g_pSumShm, iAttrId, iValue, callback_AddValue, 0, NULL);
}

int Attr_API_EX(int iAttrId, int iValue, int *piCurValue)
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

	return CreateOrUpdateNode(g_pSumShm, iAttrId, iValue, callback_AddValue, 0, (uint32_t*)piCurValue);
}

int Attr_API_Set(int iAttrId, int iValue)
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

	return CreateOrUpdateNode(g_pSetShm, iAttrId, iValue, callback_SetValue, 0, NULL);
}

int Attr_API_Get(int iAttrId, int* piValue)
{
	int i = 0;
	uint32_t dwHashVal = 0;
	AGENT_ATTR_NODE *pNode = NULL;

	for ( i = 0; i < AGENT_ATTR_SHM_HASH_TIMES; i++ )
	{
		dwHashVal = iAttrId % g_arrMod[i];

		// 在sum的内存中查找
		pNode = (AGENT_ATTR_NODE*)g_pSumShm + AGENT_ATTR_SHM_HASH_LEN*i + dwHashVal;
		if ( pNode->dwAttrId == (uint32_t)iAttrId )  /* 找到对应节点 */
		{
			*piValue = pNode->dwCurVal;
			return 0;
		}

		// 在set的内存中查找
		pNode = (AGENT_ATTR_NODE*)g_pSetShm + AGENT_ATTR_SHM_HASH_LEN*i + dwHashVal;
		if ( pNode->dwAttrId == (uint32_t)iAttrId )  /* 找到对应节点 */
		{
			*piValue = pNode->dwCurVal;
			return 0;
		}
	}

	*piValue = 0;
	return -1;
}

void Attr_Time_Span_Usec(int iBaseAttr, uint32_t iTimeSpan)
{
    if (iTimeSpan < 0)
        return;

    int iAttr = 0;

    if (iTimeSpan < 100)
        iAttr = iBaseAttr + 0;
    else if (iTimeSpan < 200)
        iAttr = iBaseAttr + 1;
    else if (iTimeSpan < 500)
        iAttr = iBaseAttr + 2;
    else if (iTimeSpan < 1000)
        iAttr = iBaseAttr + 3;
    else if (iTimeSpan < 2000)
        iAttr = iBaseAttr + 4;
    else if (iTimeSpan < 5000)
        iAttr = iBaseAttr + 5;
    else
        iAttr = iBaseAttr + 6;

    if (iAttr)
        Attr_API(iAttr, 1);
}

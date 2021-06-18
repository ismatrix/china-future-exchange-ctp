/*
 * =====================================================================================
 *
 *       Filename:  Attr_API_Var.c
 *
 *    Description:  �������ϱ�api
 *					1) ֧���滻attr�����еı���($dec/$hex: �ֱ���滻Ϊ10���ƻ�16���Ƶ�����)
 *					2) �������ɵڶ�����������
 *					3) ԭ�Ӳ�����֤�ϸߵ�׼ȷ��
 *					4) ���ڸ��³�ͻ�ĸ���, ���Ǹ��ʺܵ�
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

// ����hash�ĳ��Ⱥͽ���
#define VAR_SHM_HASH_LEN 1000
#define VAR_SHM_HASH_TIMES 40

// ����д��ͻ���Ե�������
#define VAR_MAX_ATTEMPT_TIMES 50

// ����������
static const int g_arrMod[VAR_SHM_HASH_TIMES] = {998, 997, 991, 983, 982, 977, 976, 974, 971, 967, 
	964, 958, 956, 953, 947, 944, 941, 937, 934, 932, 929, 926, 922, 919, 916, 914, 911, 908, 907, 904, 
	898, 892, 887, 886, 883, 881, 878, 877, 872, 866};

// ������agent�����ڴ��еĽڵ�ṹ
typedef struct
{
	union
	{
		struct 
		{
			int iAttrId;                                /* ����id */
			int iVarKey;                                /* ����key(����Ϊ����) */
		};
		
		uint64_t key_number;
	};

	union
	{
		struct 
		{
			int iValue;                                 /* ��Ӧ��ֵ */
			int iLastTime;                              /* ����ϱ�ʱ�� */
		};

		uint64_t val_number;
	};
} SVarNode;

const int VAR_SHM_SIZE = sizeof(SVarNode) * VAR_SHM_HASH_LEN * VAR_SHM_HASH_TIMES;

// ȫ�ֱ���
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
	// ��ʼ����ʱ����(��ִ��һ��)
	if ( OI_GetShm2( (void **)&g_pSumShm, VAR_SUM_SHM_KEY, VAR_SHM_SIZE, 0666|IPC_CREAT) < 0 ) 
	{
		g_pSumShm = NULL;
		iInitRet = -1;
	}

	// ��ʼ����ʱ����(��ִ��һ��)
	if ( OI_GetShm2( (void **)&g_pSetShm, VAR_SET_SHM_KEY, VAR_SHM_SIZE, 0666|IPC_CREAT) < 0 ) 
	{
		g_pSetShm = NULL;
		iInitRet = -1;
	}

	// �˳���ʱ��ж�ع����ڴ�
	atexit(DetachShm);
}

static spinlock_t lock = SPINLOCK_INITIALIZER;

static void InitOnce(void)
{
	spinlock_lock(&lock);                       /* ���� */
	InitShm();
	spinlock_unlock(&lock);                     /* ���� */
}

typedef int (*UPDATE_METHOD_PTR)(volatile SVarNode*, int, uint32_t, uint32_t*);

static inline int callback_AddValue(volatile SVarNode* pNode, int iCurTime, uint32_t dwNewVal, uint32_t *pdwValue)
{
	volatile SVarNode stNodeCopy = *pNode;

	// �ж� CAS ���½ڵ�����Ƿ�ɹ�
	volatile SVarNode stNewNode;
	stNewNode.iValue = stNodeCopy.iValue + dwNewVal;
	stNewNode.iLastTime = ((uint32_t)iCurTime | 0x80000000u); /* �������λΪ1, ��ʾusing״̬ */

	// �ж� CAS ���½ڵ�����Ƿ�ɹ�
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

	// �ж� CAS ���½ڵ�����Ƿ�ɹ�
	volatile SVarNode stNewNode;
	stNewNode.iValue = dwNewVal;
	stNewNode.iLastTime = ((uint32_t)iCurTime | 0x80000000u); /* �������λΪ1, ��ʾusing״̬ */

	// �ж� CAS ���½ڵ�����Ƿ�ɹ�
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

// ���� attri_id ��Ӧ�Ľڵ�, ��������ھʹ���һ���½ڵ�
int CreateOrUpdateNode(volatile void* pShm, int iAttrId, int iVarKey, uint32_t dwNewVal, UPDATE_METHOD_PTR UpdateNode, uint32_t* pdwValue)
{
	// ȷ���Ѿ���ʼ���������ڴ�
	if ( !pShm || iAttrId <= 0 )
	{
		return -2;
	}

	int i = 0, j = 0;
	uint32_t dwHashVal = 0;
	int iCurTime = optimized_time(NULL);        /* 32bitϵͳֱ����time���ܽϲ� */
	volatile SVarNode *pNode = NULL;

	// ��¼�սڵ���Ϊ���α�ѡ�ڵ�
	int iEmptyNodeSize = 0;
	volatile SVarNode* arrEmptyNodeAddr[VAR_SHM_HASH_TIMES] = {NULL}; /* ��ʼ��ȫΪNULL */

	// ���Բ��ҽڵ�
	for ( i = 0; i < VAR_SHM_HASH_TIMES; i++ )
	{
		dwHashVal = CalcKey(iAttrId, iVarKey, g_arrMod[i]);
		pNode = (volatile SVarNode*)pShm + VAR_SHM_HASH_LEN*i + dwHashVal;

		if ( 0 == pNode->key_number )				/* ��¼(key==0)�Ľڵ��б� */
		{
			arrEmptyNodeAddr[iEmptyNodeSize++] = pNode;
		}
		else
		{
			if ( pNode->iAttrId == iAttrId && pNode->iVarKey == iVarKey )  /* �ҵ���Ӧ�ڵ� */
			{
				for ( j = 0; j < VAR_MAX_ATTEMPT_TIMES; j++ )
				{
					if ( UpdateNode(pNode, iCurTime, dwNewVal, pdwValue) == 0 ) /* ����ʧ��, ѭ������ */
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

	// �����½��ڵ�
	volatile SVarNode stNewNode;
	stNewNode.iAttrId = iAttrId;
	stNewNode.iVarKey = iVarKey;
	stNewNode.iValue = dwNewVal;
	stNewNode.iLastTime = iCurTime;

	volatile SVarNode stEmptyNode;
	stEmptyNode.key_number = 0;
	stEmptyNode.val_number = 0;

	// 1) ������key==0�Ŀ��нڵ����½��ڵ�
	for ( i = 0; i < iEmptyNodeSize; i++ )
	{
		pNode = arrEmptyNodeAddr[i];

		// �ж� CAS �����ڵ�����Ƿ�ɹ�
		if ( CAS_64bit(&pNode->key_number, stEmptyNode.key_number, stNewNode.key_number) 
				|| (pNode->iAttrId == iAttrId && pNode->iVarKey == iVarKey) ) /* CAS_64bit������false�����ڵ㱻�����̻߳������ռ�����жϱ������Ƿ�key��ֵһ�£���һ���Կɽ��и���. */
		{
			// CAS ����value
			for ( j = 0; j < VAR_MAX_ATTEMPT_TIMES; j++ )
			{
				if ( UpdateNode(pNode, iCurTime, dwNewVal, pdwValue) == 0 ) /* ����ʧ��, ѭ������ */
				{
					break;
				}
			}

			if ( j >= VAR_MAX_ATTEMPT_TIMES )
				return -3;

			return 0;
		}
		// ����ڵ��Ǳ��������Զ�Ӧ�Ľڵ�ռ��, ��ô�ͼ���ѭ������
	}

	// 2) ���пսڵ㶼�Ҳ�����������24Сʱ�ڵĹ��ڽڵ����½��ڵ�(defence code. only be executed when no empty node avaiable)
	// �������, һ�������»ᵼ���ϱ���׼ȷ, ������֤���ʾ����ܵ�
	volatile SVarNode stNodeCopy;
	for ( i = VAR_SHM_HASH_TIMES-1; i >= 0; i-- )
	{
		dwHashVal = CalcKey(iAttrId, iVarKey, g_arrMod[i]);
		pNode = (volatile SVarNode*)pShm + VAR_SHM_HASH_LEN*i + dwHashVal;

		stNodeCopy = *pNode;
		stNodeCopy.iLastTime &= (~0x80000000u); /* ������ɣ��䵽�˴��߼�����Ȼ��agent�������ù����λ. */
		if ( stNodeCopy.iLastTime > iCurTime - 24*60*60 && stNodeCopy.iLastTime < iCurTime - 5*60 ) /* �ϴ��ϱ�ʱ����5���ӵ�24Сʱ�� */
		{
			// �ж� CAS �����ڵ�����Ƿ�ɹ�
			if ( CAS_64bit(&pNode->key_number, stNodeCopy.key_number, stNewNode.key_number) 
					|| (pNode->iAttrId == iAttrId && pNode->iVarKey == iVarKey) )
			{
				// ���Ը���������
				if ( CAS_64bit(&pNode->val_number, stNodeCopy.val_number, stNewNode.val_number) )
				{
					return 0;
				}

				// �������ֵʧ��, ˵�����������̻����߳��Ѿ����ǳɹ�, �������ֵ����
				// CAS ����value
				for ( j = 0; j < VAR_MAX_ATTEMPT_TIMES; j++ )
				{
					if ( UpdateNode(pNode, iCurTime, dwNewVal, pdwValue) == 0 ) /* ����ʧ��, ѭ������ */
					{
						break;
					}
				}

				if ( j >= VAR_MAX_ATTEMPT_TIMES )
					return -3;

				return 0;
			}

			// ����ڵ��Ǳ��������Զ�Ӧ�Ľڵ�ռ��, ��ô�ͼ���ѭ������
		}
	}

	// ���г��Զ�ʧ��
	return -3;
}

int Attr_API_Var(int iAttrId, int iVarKey, int iValue)
{
	// ���ع����ڴ�
	if ( NULL == g_pSumShm )
	{
		InitOnce();                             /* �̰߳�ȫ */
		if ( 0 != iInitRet || NULL == g_pSumShm )
		{
			return -1;
		}
	}

	return CreateOrUpdateNode(g_pSumShm, iAttrId, iVarKey, iValue, callback_AddValue, NULL);
}

int Attr_API_Var_EX(int iAttrId, int iVarKey, int iValue, int *piCurValue)
{
	// ���ع����ڴ�
	if ( NULL == g_pSumShm )
	{
		InitOnce();                             /* �̰߳�ȫ */
		if ( 0 != iInitRet || NULL == g_pSumShm )
		{
			return -1;
		}
	}

	return CreateOrUpdateNode(g_pSumShm, iAttrId, iVarKey, iValue, callback_AddValue, (uint32_t*)piCurValue);
}

int Attr_API_Set_Var(int iAttrId, int iVarKey, int iValue)
{
	// ���ع����ڴ�
	if ( NULL == g_pSetShm )
	{
		InitOnce();                             /* �̰߳�ȫ */
		if ( 0 != iInitRet || NULL == g_pSetShm )
		{
			return -1;
		}
	}

	return CreateOrUpdateNode(g_pSetShm, iAttrId, iVarKey, iValue, callback_SetValue, NULL);
}

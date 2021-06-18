/*
 * =====================================================================================
 *
 *       Filename:  Attr_API.c
 *
 *    Description:  ��ǿ��������ϱ�api
 *					1) ��֤ԭ�Ӳ���, ֧�ֶ��߳�/����̲�������
 *					2) hash��ʽ���빲���ڴ�, ʱ�临�ӶȽ���Ϊ׼O(1)
 *					3) ֧�����1w�����Ե��ϱ�
 *					4) �����monitor����, ���ֶ��¾�api�ļ���, �û������޸�֪���滻���µ�api
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

// ����hash�ĳ��Ⱥͽ���
#define AGENT_ATTR_SHM_HASH_LEN 1000
#define AGENT_ATTR_SHM_HASH_TIMES 40

// ����д��ͻ���Ե�������
#define MAX_ATTEMPT_TIMES 50

// ����������
static const int g_arrMod[AGENT_ATTR_SHM_HASH_TIMES] = {998, 997, 991, 983, 982, 977, 976, 974, 971, 967, 
	964, 958, 956, 953, 947, 944, 941, 937, 934, 932, 929, 926, 922, 919, 916, 914, 911, 908, 907, 904, 
	898, 892, 887, 886, 883, 881, 878, 877, 872, 866};

// ������agent�����ڴ��еĽڵ�ṹ
typedef union
{
	struct                                      /* ���԰���һ����Աʹ�� */
	{
		uint32_t dwAttrId;
		uint32_t dwCurVal;
	};

	uint64_t number_value;

} AGENT_ATTR_NODE;

const int AGENT_ATTR_SHM_SIZE = sizeof(AGENT_ATTR_NODE) * AGENT_ATTR_SHM_HASH_LEN * AGENT_ATTR_SHM_HASH_TIMES;

// ȫ�ֱ���
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
	// ��ʼ����ʱ����(��ִ��һ��)
	if ( OI_GetShm2( (void **)&g_pSumShm, AGENT_ATTR_SUM_SHM_KEY, AGENT_ATTR_SHM_SIZE, 0666|IPC_CREAT) < 0 ) 
	{
		g_pSumShm = NULL;
		iInitRet = -1;
	}

	// ��ʼ����ʱ����(��ִ��һ��)
	if ( OI_GetShm2( (void **)&g_pSetShm, AGENT_ATTR_SET_SHM_KEY, AGENT_ATTR_SHM_SIZE, 0666|IPC_CREAT) < 0 ) 
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

typedef int (*UPDATE_METHOD_PTR)(volatile AGENT_ATTR_NODE*, uint32_t, uint32_t, uint32_t*);

static inline int callback_AddValue(volatile AGENT_ATTR_NODE* pNode, uint32_t dwAttrId, uint32_t dwNewVal, uint32_t *pdwValue)
{
	volatile AGENT_ATTR_NODE stNodeCopy = *pNode;

	// �ж� stNodeCopy ��ȡֵ֮ǰ, pNode �е����ݱ������߳�/�����޸Ĺ�
	if ( stNodeCopy.dwAttrId != dwAttrId )
	{
		return 1;
	}

	// �ж� CAS ���½ڵ�����Ƿ�ɹ�
	volatile AGENT_ATTR_NODE stNewNode;
	stNewNode.dwAttrId = dwAttrId;
	stNewNode.dwCurVal = stNodeCopy.dwCurVal + dwNewVal;

	// �ж� CAS ���½ڵ�����Ƿ�ɹ�
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

	// �ж� stNodeCopy ��ȡֵ֮ǰ, pNode �е����ݱ������߳�/�����޸Ĺ�
	if ( stNodeCopy.dwAttrId != dwAttrId )
	{
		return 1;
	}

	// �ж� CAS ���½ڵ�����Ƿ�ɹ�
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

// ���� attri_id ��Ӧ�Ľڵ�, ��������ھʹ���һ���½ڵ�
static int CreateOrUpdateNode(volatile void* pShm, uint32_t dwAttrId, uint32_t dwNewVal, UPDATE_METHOD_PTR UpdateNode, uint32_t dwRecurDepth, uint32_t* pdwValue)
{
	if ( dwRecurDepth > MAX_ATTEMPT_TIMES )     /* ��������Դ���, ��ֹͣ�ݹ�, ��ֹ������ѭ�� */
	{
		return -1;
	}

	// ȷ���Ѿ���ʼ���������ڴ�
	if ( !pShm || dwAttrId == 0 )
	{
		return -2;
	}

	int i = 0;
	uint32_t dwHashVal = 0;
	volatile AGENT_ATTR_NODE *pNode = NULL;

	// ��¼�սڵ���Ϊ���α�ѡ�ڵ�
	int iEmptyNodeSize = 0;
	volatile AGENT_ATTR_NODE* arrEmptyNodeAddr[AGENT_ATTR_SHM_HASH_TIMES] = {NULL}; /* ��ʼ��ȫΪNULL */

	// ���Բ��ҽڵ�
	for ( i = 0; i < AGENT_ATTR_SHM_HASH_TIMES; i++ )
	{
		dwHashVal = dwAttrId % g_arrMod[i];
		pNode = (volatile AGENT_ATTR_NODE*)pShm + AGENT_ATTR_SHM_HASH_LEN*i + dwHashVal;

		if ( 0 == pNode->dwAttrId )				/* ��¼(attrid==0)�Ľڵ��б� */
		{
			arrEmptyNodeAddr[iEmptyNodeSize++] = pNode;
		}
		else
		{
			if ( pNode->dwAttrId == dwAttrId )  /* �ҵ���Ӧ�ڵ� */
			{
				if ( UpdateNode(pNode, dwAttrId, dwNewVal, pdwValue) > 0 ) /* ����ʧ��, �ݹ����� */
				{
					return CreateOrUpdateNode(pShm, dwAttrId, dwNewVal, UpdateNode, dwRecurDepth+1, pdwValue);
				}

				return 0;
			}
		}
	}

	// �����½��ڵ�
	volatile AGENT_ATTR_NODE stNewNode;
	stNewNode.dwAttrId = dwAttrId;
	stNewNode.dwCurVal = dwNewVal;

	volatile AGENT_ATTR_NODE stEmptyNode;
	stEmptyNode.number_value = 0;

	// 1) ������attrid==0�Ŀ��нڵ����½��ڵ�
	if ( iEmptyNodeSize > 0 )                   /* ����attrid==0�Ŀ��нڵ� */
	{
		for ( i = 0; i < iEmptyNodeSize; i++ )
		{
			// �ж� CAS �����ڵ�����Ƿ�ɹ�
			if ( CAS_64bit(&arrEmptyNodeAddr[i]->number_value, stEmptyNode.number_value, stNewNode.number_value) )
			{
				return 0;
			}

			// ����ڵ��Ѿ�����������/�̴߳���, ��ô�͵ݹ�����(����)
			if ( arrEmptyNodeAddr[i]->dwAttrId == dwAttrId )
			{
				return CreateOrUpdateNode(pShm, dwAttrId, dwNewVal, UpdateNode, dwRecurDepth+1, pdwValue);
			}

			// ����ڵ��Ǳ��������Զ�Ӧ�Ľڵ�ռ��, ��ô�ͼ���ѭ������
		}
	}

	// 2) ������value==0�Ŀ��нڵ����½��ڵ�
	if ( iEmptyNodeSize == 0 )					/* û��attrid==0�Ŀ��нڵ� */
	{
		volatile AGENT_ATTR_NODE stNodeCopy;
		for ( i = AGENT_ATTR_SHM_HASH_TIMES-1; i >= 0; i-- )
		{
			dwHashVal = dwAttrId % g_arrMod[i];
			pNode = (volatile AGENT_ATTR_NODE*)pShm + AGENT_ATTR_SHM_HASH_LEN*i + dwHashVal;

			stNodeCopy = *pNode;
			if ( 0 == stNodeCopy.dwCurVal )
			{
				// �ж� CAS �����ڵ�����Ƿ�ɹ�
				if ( CAS_64bit(&pNode->number_value, stNodeCopy.number_value, stNewNode.number_value))
				{
					return 0;
				}

				// ����ڵ��Ǳ��������Զ�Ӧ�Ľڵ�ռ��, ��ô�ͼ���ѭ������
			}
		}
	}

	// ���г��Զ�ʧ��
	return -3;
}

int Attr_API(int iAttrId, int iValue)
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

	return CreateOrUpdateNode(g_pSumShm, iAttrId, iValue, callback_AddValue, 0, NULL);
}

int Attr_API_EX(int iAttrId, int iValue, int *piCurValue)
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

	return CreateOrUpdateNode(g_pSumShm, iAttrId, iValue, callback_AddValue, 0, (uint32_t*)piCurValue);
}

int Attr_API_Set(int iAttrId, int iValue)
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

		// ��sum���ڴ��в���
		pNode = (AGENT_ATTR_NODE*)g_pSumShm + AGENT_ATTR_SHM_HASH_LEN*i + dwHashVal;
		if ( pNode->dwAttrId == (uint32_t)iAttrId )  /* �ҵ���Ӧ�ڵ� */
		{
			*piValue = pNode->dwCurVal;
			return 0;
		}

		// ��set���ڴ��в���
		pNode = (AGENT_ATTR_NODE*)g_pSetShm + AGENT_ATTR_SHM_HASH_LEN*i + dwHashVal;
		if ( pNode->dwAttrId == (uint32_t)iAttrId )  /* �ҵ���Ӧ�ڵ� */
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

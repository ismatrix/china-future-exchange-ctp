/*
 * =====================================================================================
 *
 *       Filename:  Attr_API_Str.c
 *
 *    Description:  字符串相关的api实现
 *
 *        Version:  1.0
 *        Created:  04/24/2014 10:27:25 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sunnyhao
 *        Company:  Tencent
 *
 * =====================================================================================
 */
#include "comm_def.h"

#define DEP_SHM_LOCK_ID         34552
#define DEP_SHM_ID				24552
#define DEP_ATTTIBUTE_BASE		600
#define MAX_DEP_SHMEM_LEN		(2*1024*1024)
#define MAX_DEP_VALUE_LEN		(MAX_DEP_SHMEM_LEN - sizeof(int))

// 特征id号, 表示里面是带上报ip的特征值
#define	ATTR_EXE_DATA				12345678
// 特征id号, 表示里面是带上报ip的特征字符串
#define	ATTR_EXE_STR				12345679

//===================================================================================================
//带字符串报警特性的实现
//===================================================================================================
static char *g_pBusiShareMem = NULL;

static int
semlock()
{
    struct sembuf lock[1];
    int ret = 0;
    int lockid = semget(DEP_SHM_LOCK_ID, 1, 0);

    if(lockid == -1)
    {
        perror("get singal error\n");
        return -1;
    }

    lock[0].sem_num = 0;
    lock[0].sem_op = -1;
    lock[0].sem_flg = SEM_UNDO;

    ret = semop(lockid, lock, 1);
    if(ret < 0)
        perror("op signal error\n");

    return ret;
}

static int
semunlock()
{
    int ret = 0;
    struct sembuf lock[1];
    int lockid = semget(DEP_SHM_LOCK_ID, 1, 0);

    if(lockid == -1)
        return -1;

    lock[0].sem_num = 0;
    lock[0].sem_op = 1;
    lock[0].sem_flg = SEM_UNDO;

    ret = semop(lockid, lock, 1);
    return ret;
}

//异常ID上报携带特性字符串
int
Attr_API_Alarm(int attr_id, int len, char *pvalue)
{
    char *p = NULL;
    int *plen = NULL;

    if(attr_id < DEP_ATTTIBUTE_BASE || len == 0 || pvalue == NULL)
        return -1;

    //加锁  
    if(semlock() < 0)
        return -1;

    if(!g_pBusiShareMem && OI_GetShm2((void **) &g_pBusiShareMem, DEP_SHM_ID, MAX_DEP_SHMEM_LEN, 0666) < 0)
    {
        semunlock();
        return -1;  // 修正无法获取共享内存问题,modified by arrowliu, 2006-03-29
    }

    //取可用长度
    plen = (int *) g_pBusiShareMem;
    if(plen == NULL)
    {
        semunlock();
        return -1;
    }

    if(*plen > MAX_DEP_VALUE_LEN || (MAX_DEP_VALUE_LEN - *plen) < len + 8)  // modified by arrowliu, 2006-08-10
    {
        semunlock();
        return -1;
    }

    //copy value
    p = g_pBusiShareMem;
    p += sizeof(int);   //all the data length
    p += *plen;

    *plen += len + 2 * sizeof(int); //increase length,一个整形是长度，一个是id

    //first 4byte is length of data
    *(int *) p = htonl(len);

    //second 4byte is attribute id of data
    p += sizeof(int);
    *(int *) p = htonl(attr_id);

    //other buffer for the data
    p += sizeof(int);
    memcpy(p, pvalue, len);

    semunlock();
    return 0;
}

int AttrProfile(int iId, int iAttrId1, int iAttrId2)
{
	printf("not support!\n");
	return 0;
}

#define ULONG uint32_t
#define setULong_H(_pos, value) {*((ULONG*)(_pos))=htonl(value);}

int setNumAttrWithIP(const char* szIP, int iAttrId, int iValue)
{
	char buf[12];
	char *p = NULL;
	int iIP = -1;
	struct in_addr inaddr;

	if ( !szIP )
	{
		fprintf(stderr, "strIP is NULL, error!\n");
		return -1;
	}

	if ( 0==inet_aton(szIP, &inaddr) )
	{
		fprintf(stderr, "invalid IP: %s\n", szIP);
		return -1;
	}

	iIP = inaddr.s_addr;

	if ( iAttrId <= 0 )
	{
		fprintf(stderr, "invalid iAttrID: %d\n", iAttrId);
		return -1;
	}

	// 封装协议，网络字节序
	// 4个字节的IP, 4个字节id，4个字节value
	bzero(buf, sizeof(buf));
	p = buf;
	setULong_H(p, iIP);
	p+=4;

	setULong_H(p, iAttrId);
	p+=4;

	setULong_H(p, iValue);
	p+=4;

	return Attr_API_Alarm(ATTR_EXE_DATA, sizeof(buf), buf);
}

int setStrAttrWithIP(const char* strIP, int iAttrID, size_t len , char* pval) 
{
	char buf[65536];
	char* p = NULL;
	int iIP = -1;
	struct in_addr inaddr;

	if (len > sizeof(buf)) 
	{
		fprintf(stderr, "len is %zd, longger than 65536!\n", len);
		return -1;
	}

	if (!strIP) 
	{
		fprintf(stderr, "strIP is NULL, error!\n");
		return -1;
	}

	if (0==inet_aton(strIP, &inaddr)) 
	{
		fprintf(stderr, "invalid IP: %s\n", strIP);
		return -1;
	}

	iIP = inaddr.s_addr;

	if (iAttrID <=0) 
	{
		fprintf(stderr, "invalid iAttrID: %d\n", iAttrID);
		return -1;
	}

	// 封装协议，网络字节序
	// 4个字节的IP, 4个字节id，4个字节长度，len个字节内容
	bzero(buf, sizeof(buf));
	p = buf;
	setULong_H(p, iIP);
	p+=4;

	setULong_H(p, iAttrID);
	p+=4;

	setULong_H(p, len);
	p+=4;

	memcpy(p, pval, len);
	p+=len;

	return Attr_API_Alarm(ATTR_EXE_STR, p-buf, buf);
}

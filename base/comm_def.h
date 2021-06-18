/*
 * =====================================================================================
 *
 *       Filename:  comm_def.h
 *
 *    Description:  一些公共定义
 *
 *        Version:  1.0
 *        Created:  04/24/2014 10:30:28 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sunnyhao
 *        Company:  Tencent
 *
 * =====================================================================================
 */
#ifndef __COMM_DEF_H_sunnyhao__
#define __COMM_DEF_H_sunnyhao__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "CAS_64bit.h"
#include "spin_lock.h"


static char* OI_GetShm(int iKey, int iSize, int iFlag) 
{
	int iShmID;
	char *sShm;
	char sErrMsg[50];

	if( 0 == iKey ) 
	{
		snprintf(sErrMsg, sizeof(sErrMsg), "shmget %d %d: iKey don't zero(0)", iKey, iSize);
		perror(sErrMsg);
		return(NULL);
	}

	if((iShmID = shmget(iKey, iSize, iFlag)) < 0) 
	{
		snprintf(sErrMsg, sizeof(sErrMsg), "shmget %d %d", iKey, iSize);
		perror(sErrMsg);
		return NULL;
	}

	if((sShm = (char *) shmat(iShmID, NULL, 0)) == (char *) -1) 
	{
		perror("shmat");
		return NULL;
	}

	return sShm;
}

static int OI_GetShm2(void **pstShm, int iKey, int iSize, int iFlag) 
{
	char *sShm;
	if( 0 == iKey ) 
	{
		return(-1);
	}

	if(!(sShm = OI_GetShm(iKey, iSize, iFlag & (~IPC_CREAT)))) 
	{
		if(!(iFlag & IPC_CREAT)) 
		{
			return -1;
		}

		if(!(sShm = OI_GetShm(iKey, iSize, iFlag))) 
		{
			return -1;
		}

		memset(sShm, 0, iSize);
		*pstShm = sShm;
		return(1);
	}

	*pstShm = sShm;
	return 0;
}

#endif

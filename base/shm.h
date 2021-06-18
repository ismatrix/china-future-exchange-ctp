
#ifndef _OI_SHM_H
#define _OI_SHM_H

#include <sys/ipc.h>
#include <sys/shm.h>

char* GetReadOnlyShm(int iKey, int iSize);
char* GetShm(int iKey, int iSize, int iFlag);
int GetShm2(void **pstShm, int iShmID, int iSize, int iFlag);
int GetShm3(void **pstShm, int iShmID, int iSize, int iFlag);
char* GetShm4(int iKey, int iSize, int iFlag, int *piShmID);

#endif

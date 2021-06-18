#ifndef _OI_DEBUG_H
#define _OI_DEBUG_H

const char *DumpPackage(void *pPkg, int iPkgLen);

const char *DumpMemory(void *pMem, uint32_t uDumpOffset, uint32_t uDumpLen);
const char *DumpHex(void *pMem, uint32_t uDumpOffset, uint32_t uDumpLen);

#endif

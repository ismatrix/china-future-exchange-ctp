#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "debug.h"

const char *DumpHex(void *pMem, uint32_t uDumpOffset, uint32_t uDumpLen)
{
	int inc;
	uint32_t i;
	static char sLogBuf[65536*4];
	char *pDst = sLogBuf, *pSrc = pMem + uDumpOffset;

	sLogBuf[0] = '\0';
	for (i = uDumpOffset; i < uDumpOffset + uDumpLen; i++, pSrc++) {
		inc = snprintf(pDst, sLogBuf + sizeof(sLogBuf) - pDst, "%02X", (unsigned char) *pSrc);
		if (inc < 0) { break; }
		pDst += inc;
		if (pDst >= sLogBuf + sizeof(sLogBuf)) { break; }
	}

	return sLogBuf;
}

const char *DumpMemory(void *pMem, uint32_t uDumpOffset, uint32_t uDumpLen)
{
	int inc;
	uint32_t i;
	static char sLogBuf[65536*4];
	char *pDst = sLogBuf, *pSrc = pMem + uDumpOffset;

	sLogBuf[0] = '\0';
	for (i = uDumpOffset; i < uDumpOffset + uDumpLen; i++, pSrc++) {
		char *sSep, sAddr[20];
		switch (i % 16) {
			case 3:
			case 11: sSep = " - "; break;
			case 7: sSep = " | "; break;
			case 15: sSep = "\n"; break;
			default: sSep = " "; break;
		}
		switch (i % 16) {
			case 0: snprintf(sAddr, sizeof(sAddr), "%04hX: ", i); break;
			default: sAddr[0] = '\0'; break;
		}
		inc = snprintf(pDst, sLogBuf + sizeof(sLogBuf) - pDst, "%s%02X%s", sAddr, (unsigned char) *pSrc, sSep);
		if (inc < 0) { break; }
		pDst += inc;
		if (pDst >= sLogBuf + sizeof(sLogBuf)) { break; }
	}

	return sLogBuf;
}

const char *DumpPackage(void *pPkg, int iPkgLen)
{
	int i, inc;
	static char sLogBuf[65536*4];
	char *pDst = sLogBuf, *pSrc = pPkg;

	sLogBuf[0] = '\0';
	for (i = 0; i< iPkgLen; i++, pSrc++) {
		char *sSep, sAddr[20];
		switch (i % 16) {
			case 3:
			case 11: sSep = " - "; break;
			case 7: sSep = " | "; break;
			case 15: sSep = "\n"; break;
			default: sSep = " "; break;
		}
		switch (i % 16) {
			case 0: snprintf(sAddr, sizeof(sAddr), "%04hX: ", i); break;
			default: sAddr[0] = '\0'; break;
		}
		inc = snprintf(pDst, sLogBuf + sizeof(sLogBuf) - pDst, "%s%02X%s", sAddr, (unsigned char) *pSrc, sSep);
		if (inc < 0) { break; }
		pDst += inc;
		if (pDst >= sLogBuf + sizeof(sLogBuf)) { break; }
	}

	return sLogBuf;
}


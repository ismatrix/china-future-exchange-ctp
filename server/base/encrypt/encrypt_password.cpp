/*************************************************************************
    > File Name: encrypt_password.cpp
    > Created Time: Mon 22 Aug 2016 10:08:35 AM CST
    > Author: 
    > description: 
 ************************************************************************/

#include "encrypt_password.h"


#define __ACCOUNT_KEY__   "smartwin00000000"

string decrypt_account_password(const uint8_t* ibuf, int len)
{
	uint8_t iibuf[1024]={0}; 
	for(int i=0; i<len/2; i++)
	{
		char tmp;
		sscanf(((const char*)ibuf)+2*i, "%02x", &tmp);
		iibuf[i] = tmp;
	}
	len = len/2;

	stringstream ss;
	for(int n=0; n<len; n++)
	{
		int olen=0;
		uint8_t obuf[32]={0};

		int ilen = (16*n+16)>len  ? len%16 : 16;
		if(ilen==0) break;
		
		decrypt_aes_128_ecb((const uint8_t*)__ACCOUNT_KEY__,  iibuf+16*n, ilen, obuf, &olen);

		ss.write((const char*)obuf, olen);
	}
	return ss.str();
}


string encrypt_account_password(const uint8_t* ibuf, const int len)
{
	char out[1024];
	int outlen=0;
	for(int n=0; n<len/16+1; n++)
	{
		int olen=0;
		uint8_t obuf[32]={0};

		int ilen = (16*n+16)>len  ? len%16 : 16;
		if(ilen==0) break;

		encrypt_aes_128_ecb((const uint8_t*)__ACCOUNT_KEY__,  ibuf+16*n, ilen, obuf, &olen);

		for(int i=0; i<olen; i++)
		{
		    outlen += snprintf(out+outlen, sizeof(out)-outlen, "%02x", obuf[i]);
		}
	}
	return out;
}



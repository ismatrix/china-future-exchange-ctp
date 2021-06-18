#include <iconv.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <math.h>
#include <openssl/evp.h>
#include "convert.h"

using namespace std;

int  CodeConvert(const char* from, const char* to, const char* src, size_t inlen, char* dest, size_t outlen)
{
	char * pIn = (char*)src;
    char * pOut = dest;
	memset(dest, 0,  outlen);
	iconv_t conv = iconv_open(to, from);
	if(conv == 0) return -1;
	iconv(conv, &pIn, &inlen, &pOut, &outlen);
	iconv_close(conv);
	return 0;
}

string GBK2UTF8(char* src, int inlen, char* dest, int outlen)
{
	CodeConvert("GB2312", "UTF-8", src, inlen, dest, outlen);
	return string(dest);
}

string UTF82GBK(char* src, int inlen, char* dest, int outlen)
{
	CodeConvert("UTF-8", "GB2312", src, inlen, dest, outlen);
	return string(dest);
}


string GBK2UTF8(string src)
{
	char* dest = new char[4*src.length()+1];
	CodeConvert("GB2312", "UTF-8", src.c_str(), src.length(), dest, sizeof(char)*src.length()*4+1);
	string tmp = dest;
	delete dest;
	return tmp;
}

string UTF82GBK(string src)
{
	char* dest = new char[src.length()+1];
	CodeConvert("UTF-8", "GB2312", src.c_str(), src.length(), dest, sizeof(char)*src.length()+1);
	string tmp = dest;
	delete dest;
	return tmp;
}



int encrypt(const EVP_CIPHER*  _callback, const uint8_t* key, const uint8_t* ibuf, const int ilen, uint8_t* obuf, int* olen)
{
	uint8_t* iv = NULL;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	
	//OpenSSL_add_all_algorithms();
	
	//³É¹¦1 Ê§°Ü0
	if(1 != EVP_EncryptInit_ex(ctx, _callback,NULL, key, iv)) return -1;

	if(1 != EVP_EncryptUpdate(ctx, obuf, olen, ibuf, ilen)) return -2;

	int tmplen;
	if(1 != EVP_EncryptFinal_ex(ctx, obuf + *olen, &tmplen)) return -3;

	*olen += tmplen;
	
	EVP_CIPHER_CTX_free(ctx);
	
	return 0;
}


int decrypt(const EVP_CIPHER* _callback, const uint8_t* key, const uint8_t* ibuf, const int ilen, uint8_t* obuf, int* olen)
{
	uint8_t* iv = NULL;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	
	//OpenSSL_add_all_algorithms();  
	
	if(1 != EVP_DecryptInit_ex(ctx, _callback, NULL, key, iv)) return -1;

	if(1 != EVP_DecryptUpdate(ctx, obuf, olen, ibuf, ilen)) return -2;

	int tmplen;
	if(1 != EVP_DecryptFinal_ex(ctx, obuf + *olen, &tmplen)) return -3;

	*olen += tmplen;
	
	EVP_CIPHER_CTX_free(ctx);
	
	return 0;
}


int encrypt_aes_128_ecb(const uint8_t* key, const uint8_t* ibuf, const int ilen, uint8_t* obuf, int* olen)
{
	return encrypt(EVP_aes_128_ecb(), key, ibuf, ilen, obuf, olen);
}

int decrypt_aes_128_ecb(const uint8_t* key, const uint8_t* ibuf, const int ilen, uint8_t* obuf, int* olen)
{
	return decrypt(EVP_aes_128_ecb(), key, ibuf, ilen, obuf, olen);
}










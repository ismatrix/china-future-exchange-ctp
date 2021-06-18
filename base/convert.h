#ifndef CODE_CONVERT_H
#define CODE_CONVERT_H

#include <string>

using namespace std;



string GBK2UTF8(char* src, int inlen, char* dest, int outlen);
string UTF82GBK(char* src, int inlen, char* dest, int outlen);
string GBK2UTF8(string src);
string UTF82GBK(string src);




//encrypt(EVP_aes_256_ctr(), key, ibuf, 32, obuf, &olen);
//int encrypt(const EVP_CIPHER*  _callback, uint8_t* key, uint8_t* ibuf, int ilen, uint8_t* obuf, int* olen);
//int decrypt(const EVP_CIPHER* _callback, uint8_t* key, uint8_t* ibuf, int ilen, uint8_t* obuf, int* olen);


int encrypt_aes_128_ecb(const uint8_t* key, const uint8_t* ibuf, const int ilen, uint8_t* obuf, int* olen);
int decrypt_aes_128_ecb(const uint8_t* key, const uint8_t* ibuf, const int ilen, uint8_t* obuf, int* olen);


#endif

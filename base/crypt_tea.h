/*
 *  crypt_tea.h
 *
 *  Created on: 2011-5-9
 *      Author: echo
 *      Brief: a very fast cryption from tencent, no copyright claim. I modified it to make it useable on 64bit linux.
 */

#ifndef UTIL_CRYPT_TEA_H__
#define UTIL_CRYPT_TEA_H__
#include <stdint.h>

/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(8byte)*/
void oi_symmetry_encrypt(const uint8_t* pInBuf, int nInBufLen, const uint8_t* pKey, uint8_t* pOutBuf, int *pOutBufLen);

/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数;
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
bool oi_symmetry_decrypt(const uint8_t* pInBuf, int nInBufLen, const uint8_t* pKey, uint8_t* pOutBuf, int *pOutBufLen);

///////////////////////////////////////////////////////////////////////////////////////////////

/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(8byte)*/
void oi_symmetry_encrypt2(const uint8_t* pInBuf, int nInBufLen, const uint8_t* pKey, uint8_t* pOutBuf, int *pOutBufLen);

/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数;
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(8byte)*/
bool oi_symmetry_decrypt2(const uint8_t* pInBuf, int nInBufLen, const uint8_t* pKey, uint8_t* pOutBuf, int *pOutBufLen);


#endif//UTIL_CRYPT_TEA_H__


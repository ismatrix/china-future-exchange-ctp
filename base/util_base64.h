/*
 * util_base64.h
 *
 *  Created on: 2012-11-23
 *      Author: dreamyzhang
 */

#ifndef __UTIL_BASE64__
#define __UTIL_BASE64__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//openssl中的实现 但是编码结尾补了\0 需要特殊处理下
//EVP_EncodeBlock((unsigned char*)decoded, (const unsigned char*)encoded, encodedLength);  
//EVP_DecodeBlock((unsigned char*)decoded, (const unsigned char*)encoded, encodedLength); 

/**
 * encode
 * Note: output buffer length must be greater than input_len/3*4
 */
int32_t util_base64_encode(uint8_t* output, const uint8_t* input, int32_t input_len);

int32_t util_base64_decode(uint8_t* output, const uint8_t* input, int32_t input_len);


#ifdef __cplusplus
}
#endif

#endif /* __UTIL_BASE64__ */

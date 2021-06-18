/*
 * crypto.h
 *
 *  Created on: 2015-1-8
 *      Author: echo
 *       Brief: common used wrappers for libcrypto(from OpenSSL)
 */

#ifndef UT_CRYPTO_H_
#define UT_CRYPTO_H_

#include <stdint.h>

/**
 * PKCS#7 padding
 * return what should be appended when the data length is @len and block size is @block_size.
 * e.g. if 0x5 is returned, you should append 5 bytes to the data, each byte with the value of 0x5.
 */
inline uint8_t pkcs7_pad(uint32_t len, uint8_t block_size) { return block_size - len%block_size; }

/**
 * PKCS#7 de-padding
 * return the real length of @data
 * NOTE: @len should be a multiple of block size(8/16/32). An invalid @len will lead to unknown error.
 */
inline uint32_t pkcs7_depad(const void* data, uint32_t len) { return len - ((const uint8_t*)data)[len-1]; }

/**
 * base64 encode
 * @param output_len	[input]the max size of @output; [output]the decode data length
 * @return false only when output_len is too small.
 * code refer: http://blog.csdn.net/yasi_xi/article/details/9040793
 */
bool base64_encode(void* output, uint32_t& output_len, const void* input, uint32_t input_len);

/**
 * base64 decode
 * @param output_len	[input]the max size of @output; [output]the decode data length
 * @return false only when output_len is too small.
 */
bool base64_decode(void* output, uint32_t& output_len, const void* input, uint32_t input_len);

/**
 * DES encrypt using ECB mode and pkcs#7 padding
 * @param output_len	[input]the max size of @output; [output]the final cipher length
 * return true on success, return false if error occurred(caused by not enough output space)
 * NOTE: length of a DES cipher with pkcs#7 padding will always be a multiply of 8, the output length should better greater than input_len.
 */
bool _des_ecb_encrypt(void* output, uint32_t& output_len, const void* input, uint32_t input_len, const void* key);

/**
 * DES decrypt using ECB mode and pkcs#7 padding
 * @param output_len	[input]the max size of @output; [output]the final output data length
 * return true on success, return false if error occurred(caused by not enough output space or input length not a multiply of 8)
 */
bool _des_ecb_decrypt(void* output, uint32_t& output_len, const void* input, uint32_t input_len, const void* key);

#endif /* UT_CRYPTO_H_ */

/*
 *  base64.h
 *
 *  Created on: 2012-11-23
 *      Author: echo
 */

#ifndef UTIL_BASE64_H_
#define UTIL_BASE64_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * encode
 * Note: output buffer length must be greater than input_len/3*4
 */
int32_t  base64_encode(uint8_t* output, const uint8_t* input, int32_t input_len);

int32_t  base64_decode(uint8_t* output, const uint8_t* input, int32_t input_len);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_BASE64_H_ */

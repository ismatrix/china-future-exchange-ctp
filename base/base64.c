/*
 *  base64.c
 *
 *  Created on: 2012-11-23
 *      Author: echo
 *      Note  : modified from http://base64.sourceforge.net/b64.c
 */

#ifndef UTIL_BASE64_C_
#define UTIL_BASE64_C_

#include "base64.h"
#include <stdio.h>
#include <stdlib.h>

static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*int32_t  base64_encode(uint8_t* output, const uint8_t* input, int32_t input_len)
{
	int32_t output_len = 0;
	while (input_len > 0)
	{
		*output++ = cb64[input[0] >> 2];
		*output++ = cb64[((input[0] & 0x03) << 4) | ((input[1] & 0xF0) >> 4)];
		*output++ = input_len > 1 ? cb64[((input[1] & 0x0F) << 2) | ((input[2] & 0xC0) >> 6)] : '=';
		*output++ = input_len > 2 ? cb64[input[2] & 0x3F] : '=';
		input +=3;
		input_len -=3;
		output_len +=4;
	}
	*output = 0;
	return output_len;
}*/

#include <math.h>
static int mod_table[] = {0, 2, 1};
int32_t  base64_encode(uint8_t* output, const uint8_t* input, int32_t input_len)
{
	int32_t output_len = (int32_t) (4.0 * ceil((double) input_len / 3.0));
	int i, j;
    for (i = 0, j = 0; i < input_len;) {

        uint32_t octet_a = i < input_len ? input[i++] : 0;
        uint32_t octet_b = i < input_len ? input[i++] : 0;
        uint32_t octet_c = i < input_len ? input[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        output[j++] = cb64[(triple >> 3 * 6) & 0x3F];
        output[j++] = cb64[(triple >> 2 * 6) & 0x3F];
        output[j++] = cb64[(triple >> 1 * 6) & 0x3F];
        output[j++] = cb64[(triple >> 0 * 6) & 0x3F];
    }
    for (i = 0; i < mod_table[input_len % 3]; i++)
    	output[output_len - 1 - i] = '=';
    return output_len;
}

static const char cd64[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	62, // '+'
	0, 0, 0,
	63, // '/'
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
	0, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
	13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
	0, 0, 0, 0, 0, 0,
	26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
};
int32_t  base64_decode(uint8_t* output, const uint8_t* input, int32_t input_len)
{
	int32_t output_len = 0;

	int32_t v, i = 0;
	while (i < input_len)
	{
		v = cd64[*input++] << 18;
		v += cd64[*input++] << 12;
		*output++ = (v & 0x00FF0000) >> 16;
		output_len++;
		if (*input != '=')
		{
			v += cd64[*input++] << 6;
			*output++ = (v & 0x0000FF00) >> 8;
			output_len++;
			if (*input != '=')
			{
				v += cd64[*input++];
				*output++ = v & 0x000000FF;
				output_len++;
			}
		}
		i += 4;
	}
	return output_len;
}

#endif /* UTIL_BASE64_C_ */

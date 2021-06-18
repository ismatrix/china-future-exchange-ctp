/*
 *  crypt_md5.h
 *
 *  Created on: 2010-5-1
 *      Author: echoma
 *      Brief: MD5 utilities. All code are from the open-source code from Colin Plumb.
 */

/*
 * This code implements the MD5 message-digest algorithm.
 * The algorithm is due to Ron Rivest.	This code was
 * written by Colin Plumb in 1993, no copyright is claimed.
 * This code is in the public domain; do with it what you wish.
 *
 * Equivalent code is available from RSA Data Security, Inc.
 * This code has been tested against that, and is equivalent,
 * except that you don't need to include two pages of legalese
 * with every copy.
 *
 * To compute the message digest of a chunk of bytes, declare an
 * MD5Context structure, pass it to MD5Init, call MD5Update as
 * needed on buffers full of bytes, and then call MD5Final, which
 * will fill a supplied 16-byte array with the digest.
 */

/* Brutally hacked by John Walker back from ANSI C to K&R (no
   prototypes) to maintain the tradition that Netfone will compile
   with Sun's original "cc". */

#ifndef UTIL_CRYPT_MD5_H_
#define UTIL_CRYPT_MD5_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif


/*#ifdef __alpha
typedef unsigned int uint32;
#else
typedef unsigned long uint32;
#endif*/

struct MD5Context {
        uint32_t buf[4];
        uint32_t bits[2];
        unsigned char in[64];
};

void MD5Init(struct MD5Context *ctx);
void MD5Update(struct MD5Context *ctx, const uint8_t *buf, uint32_t len);
void MD5Final(uint8_t digest[16], struct MD5Context *ctx);

/*return 0 on success,  otherwise -1 is returned.*/
int MD5Hex2Bin(uint8_t bin[16], const uint8_t hex[32]);
void MD5Bin2Hex(uint8_t hex[32], const uint8_t bin[16]);

/*
 * This is needed to make RSAREF happy on some MS-DOS compilers.
 */
typedef struct MD5Context MD5_CTX;

#ifdef __cplusplus
}
#endif

#endif /* UTIL_CRYPT_MD5_H_ */

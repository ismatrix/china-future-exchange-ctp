#ifndef _CRC_32_H_
#define _CRC_32_H_

#include <sys/types.h>
#include <stdint.h> 


uint32_t crc_32(uint32_t sed, unsigned char const * data, uint32_t length);

void store_crc(uint32_t x, unsigned char *buf);

#endif


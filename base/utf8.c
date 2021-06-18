/*
 * utf8.c
 *
 *  Created on: 2014-3-29
 *      Author: echo
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include "utf8.h"

void utf8_vct_init(utf8_vct* vector, const void *utf8, unsigned int string_length)
{
    vector->utf8_str = (const char*)utf8;
    vector->length = string_length;
    vector->index = 0;
}

void utf8_vct_reset(utf8_vct* vector)
{
    vector->index = 0;
}

int utf8_vct_unichar_count(utf8_vct* vector)
{
    int count = 0;
    while (utf8_vct_next(vector) != '\0')
        count ++;
    utf8_vct_reset(vector);
    return count;
}

#define utf8vector_check_eof(num) \
if (vector->length == -1) \
{ int i=0;\
for (;i < num; i ++) \
{ \
if (utf[i] == '\0') return '\0'; \
} \
} \
else \
{ \
if (vector->index + num > vector->length) return '\0'; \
}

wchar_t utf8_vct_next_with_raw(utf8_vct* vector, const char **raw, int *raw_length)
{
    if(vector->index>=vector->length)
    {
    	if(raw)
    	{
    		*raw = NULL;
    		*raw_length = 0;
    	}
    	return 0;
    }

    wchar_t uni;
    const char *utf = vector->utf8_str + vector->index;

    if (*utf >> 7 == 0)
    {
        utf8vector_check_eof(1);
        vector->index += 1;
        if (raw)
        {
            *raw = utf;
            *raw_length = 1;
        }
        return *utf;
    }
    //两个字节
    else if ( (*utf & 0xE0) == 0xC0 )
    {
        utf8vector_check_eof(2);
        vector->index += 2;
        uni = ((utf[0] & 0x1F) << 6)
            | (utf[1] & 0x3F);
        if (raw)
        {
            *raw = utf;
            *raw_length = 2;
        }
        return uni;
    }
    //三个字节
    else if ( (*utf & 0xF0) == 0xE0 )
    {
        utf8vector_check_eof(3);
        vector->index += 3;
        uni = (((utf[0] & 0x0F)) << 12)
            | (((utf[1] & 0x3F)) << 6)
            | (utf[2] & 0x3F);
        if (raw)
        {
            *raw = utf;
            *raw_length = 3;
        }
        return uni;
    }
    //四个字节
    else if ( (*utf & 0xF8) == 0xF0 )
    {
        utf8vector_check_eof(4);
        vector->index += 4;
        uni = ((utf[0] & 0x07) << 18)
             |((utf[1] & 0x3f) << 12)
             |((utf[2] & 0x3f) << 6)
             |(utf[3] & 0x3f);
        if (raw)
        {
            *raw = utf;
            *raw_length = 4;
        }
        return uni;
    }
    //五个字节
    else if ( (*utf & 0xFC) == 0xF8 )
    {
        utf8vector_check_eof(5);
        vector->index += 5;
        uni = ((utf[0] & 0x03) << 24)
             |((utf[1] & 0x3f) << 18)
             |((utf[2] & 0x3f) << 12)
             |((utf[3] & 0x3f) << 6)
             |(utf[4] & 0x3f);
        if (raw)
        {
            *raw = utf;
            *raw_length = 5;
        }
        return uni;
    }
    //六个字节
    else if ( (*utf & 0xFE) == 0xFC )
    {
        utf8vector_check_eof(6);
        vector->index += 6;
        uni = ((utf[0] & 0x01) << 30)
             |((utf[1] & 0x3f) << 24)
             |((utf[2] & 0x3f) << 18)
             |((utf[3] & 0x3f) << 12)
             |((utf[4] & 0x3f) << 6)
             |(utf[5] & 0x3f);
        if (raw)
        {
            *raw = utf;
            *raw_length = 6;
        }
        return uni;
    }
    return 0;
}

wchar_t utf8_vct_next(utf8_vct* vector)
{
	return utf8_vct_next_with_raw(vector, NULL, NULL);
}

unsigned int utf8_validate(void* str, unsigned int len)
{
	utf8_vct vct;
	utf8_vct_init(&vct, str, len);
	wchar_t uni;
	while(vct.index<vct.length)
	{
		while(0!=(uni = utf8_vct_next_with_raw(&vct, NULL, NULL)))
			;
		if(vct.index>=vct.length)
			break;
		memmove(((char*)str)+vct.index, ((char*)str)+vct.index+1, vct.length-vct.index-1);
		--vct.length;
	}
	return vct.length;
}

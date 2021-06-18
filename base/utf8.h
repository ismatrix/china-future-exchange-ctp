/*
 * utf8.h
 *
 *  Created on: 2014-3-29
 *      Author: echo
 *       Brief: parse utf-8 string, get single charaters.
 *
 *  Written by emptyhua@gmail.com
 *	see: https://github.com/emptyhua/bash-pinyin-completion
 */

#ifndef UT_UTF8_H_
#define UT_UTF8_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * utf-8 parse vector
 * it stores the middle data during we parsing a utf-8 string
 */
typedef struct
{
    const char *utf8_str;
    unsigned int length;
    unsigned int index;
}utf8_vct;

///initialize the vector
void utf8_vct_init(utf8_vct* vector, const void *utf8, unsigned int string_length);
///reset the vector
void utf8_vct_reset(utf8_vct* vector);

///get the next character
wchar_t utf8_vct_next_with_raw(utf8_vct* vector, const char **raw, int *raw_length);
wchar_t utf8_vct_next(utf8_vct* vector);

///get total character count
int utf8_vct_unichar_count(utf8_vct* vector);

/**
 * validate the specified string, remove none-utf8-code characters.
 * return the new length of string.
 */
unsigned int utf8_validate(void* str, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif /* UT_UTF8_H_ */

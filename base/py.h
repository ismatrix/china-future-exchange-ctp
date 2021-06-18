/*
 * py.h
 *
 *  Created on: 2014-3-29
 *      Author: echo
 *       Brief: Chinese pin-yin utilities for UTF-8
 *
 *	Written by emptyhua@gmail.com
 *	see: https://github.com/emptyhua/bash-pinyin-completion
 */

#ifndef UT_PY_H_
#define UT_PY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/**
 * get the pinyin list of a specified utf-8 character @uni.
 * @uni can be get via ut_utf8_vct_next() from ut_utf.h
 * On success, the pinyin count of this char is returned, elements in the array @pinyins will be set to global static pinyin talbe.
 * the size of array @pinyins is fixed to 4, because there at most 4 pinyin for a Chinese character.
 */
int py_get(wchar_t uni, const char* pinyins[4]);

/**
 * get pinyin for the specified utf-8 string.
 * a structore of @py_vct will be used for input and output.
 * for multi-pinyin-charater, we will use the first pinyin of its pinyin list.
 */
typedef struct
{
	const char* _string;	//[in]we will parse this string
	uint32_t _string_len;	//[in]length of the @string
	char* _pinyin;			//[out]all pinyin will be stored here. set to NULL if you dont want it.
	uint32_t _pinyin_len;	//[in|out]the max length of @pinyin when input, the result length of @pinyin when output. it must be set if @_pinyin is not NULL.
	char* _pinyin_short;	//[out]acronym, short for @_pinyin. a list of initial letter of pinyin for each Chinese character.
	uint32_t _pinyin_short_len;	//[in|out]for @_pinyin_short, works like @_pinyin_len. it must be set if @_pinyin_short is not NULL.
	uint32_t* _pinyin_pos;	//[out]offset of each charater's pinyin. set to NULL if you dont want it. if @_pinyin is NULL, this property will be ignored.
	uint32_t _pinyin_count;	//[in|out]the max count of elements in array @pinyin_pos when input, the result count when output. it must be set if one of @_pinyin_pos is not NULL.
}py_handle;
void py_get_string(py_handle* hdl);

#ifdef __cplusplus
}
#endif

#endif /* UT_PY_H_ */

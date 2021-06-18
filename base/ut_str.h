/*
 * ut_str.h
 *
 *  Created on: 2013-9-21
 *      Author: echo
 *       Brief: string utilities
 */

#ifndef UT_STR_H_
#define UT_STR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

//memstr() is also defined in old util library, we should check this before re-define.
#ifndef UTIL_PLATFORM_H_

/**
 * find the first occurrence of @needle in @haystack
 * return the pointer to the beginning of the occurrence. return NULL if not found.
 */
void* memstr(const void* haystack, int haystack_size, const char* needle);

#endif//#ifndef UTIL_PLATFORM_H_

///convert string(binary memory block) to lower/upper case
void mem2lower(void* output, const void* input, int len);
void mem2upper(void* output, const void* input, int len);

/**
 * convert memory block to hex string.
 * the output buffer length should at least be mem_len*2.
 */
void bin2hex(void* hex_output, const void* mem, int mem_len);
/**
 * convert a hex string into binary memory buffer.
 * the @hex_len must be an even number (e.g. 2 4 12 34)
 * the output buffer length should at least be @hex_len/2.
 * return the binary result length on success, otherwise return -1.
 */
int hex2bin(void* bin_output, const void* hex, int hex_len);

/// check whether a string is pure integer(/^\d+$/)
bool is_integer(const void* str, int len);

/// check whether a string is pure hex(/^[\da-fA-F]+$/)
bool is_hex(const void* str, int len);

/// check whether a string is a valid email address
bool is_email(const void* str, int32_t len);

/// check whether an ascii charactor is viewable
inline bool is_viewable_ascii(uint32_t ch) { return ch>=32 && ch<=126; }

/**
 * some string hash functions
 * see more details: http://www.cse.yorku.ca/~oz/hash.html or google: string hash.
 * to generate a 32bit hash value, ut_strhash_sdbm() is recommended.
 * ut_strhash_u64() will combine ut_strhash_sdbm() and ut_strhash_djb2() together to generate a 64bit value.
 */
uint32_t ut_strhash_djb2(const void *str, int len);
uint32_t ut_strhash_sdbm(const void *str, int len);
inline static uint64_t ut_strhash_u64(const void *str, int len)
{
	union{struct{uint32_t l; uint32_t h;}s;uint64_t u;}n;
	n.s.l=ut_strhash_sdbm(str, len);
	n.s.h=ut_strhash_djb2(str, len);
	return n.u;
}

/**
 * the KMP algorithm.
 * this can be used for binary memory match.
 */
typedef struct
{
	unsigned char* patn;
	int patn_len;
	unsigned char* next;
} ut_kmp;
/**
 * create the KMP pattern.
 * on success, a structure util_kmp pointer is returned. otherwise, NULL is returned.
 * */
ut_kmp * ut_kmp_create(const void* pat, int len);
/**
 * destroy the KMP pattern.
 * on success, zero is returned and KMP is initialized. otherwise, -1 is returned.
 * */
void ut_kmp_destroy(ut_kmp *kmp);
/**
 * execute a KMP match in specified text.
 * if pattern was found, it returns the index of the occurrence beginning. otherwise, -1 is returned.
 * */
int ut_kmp_find(const void* haystack, int haystack_size, ut_kmp *kmp);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/**
 * dump memory to a file descriptor.
 * the format is just like "tcpdump -X".
 */
void ut_ddump(const void* mem, int len, FILE* file=NULL);

#include <string>
#include <vector>

/// dumping memory to a std::string
std::string ut_dump(const void* mem, int len);

/// dumping memory to a std::string
/// the same format with tcpdump
std::string ut_hex_dump(const void *mem, size_t len);

/// easy json escape
std::string ut_json_escape(const std::string& plain);

/// return string "true" or "false"
std::string ut_bool_str(bool val);

/// replace @from to @to in the @str
void ut_replace(std::string& str, const std::string& from, const std::string& to);

/**
 * split a @str with a specified seperator @sep, and the result is stored(with push_back()) in @ret
 * the parameter @limit specified the max element count in @ret. the last element will contain the rest of @str.
 * return @ret.size()
 * NOTE: remember to clear @ret by yourself, ut_split() will not clear it.
 */
uint32_t ut_split(const std::string& str, std::vector<std::string>& ret_, const std::string& sep = ",", uint32_t limit=4294967295u);

/**
 * difference from @ut_split :
 * 1. if @str is empty, @ret_ will still get one empty string element in it
 * 2. if "a," split with ',' will get two elements in the @ret_
 */
uint32_t ut_split2(const std::string& str, std::vector<std::string>& ret_, const std::string& sep = ",", uint32_t limit=4294967295u);

/// trim the blank char in the @str
void ut_trim(std::string& str);

/// add slashes for \" and \'
inline std::string ut_add_slashes(const std::string& str)
{
	std::string r = str;
	ut_replace(r, "\"", "\\\"");
	ut_replace(r, "\'", "\\\'");
	return r;
}

void ut_utf8str_full2half(std::string& str);

#endif

#endif /* UT_STR_H_ */

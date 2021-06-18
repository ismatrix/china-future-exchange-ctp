/*
 * ut_str.cpp
 *
 *  Created on: 2013-9-21
 *      Author: echo
 */
#include "ut_str.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memstr(const void* haystack, int haystack_size, const char* needle)
{
	char *p = (char*)haystack;
	char needlesize = strlen(needle);
	char *end = (char*)haystack + haystack_size - needlesize;
	for (; p <= end; ++p)
	{
		if (memcmp(p, needle, needlesize) == 0)
			return p;
	}
	return NULL;
}

void mem2lower(void* output, const void* input, int len)
{
	if(output!=input)
	{
		for(int i=0; i<len; ++i)
		{
			unsigned char ch = ((unsigned char*)input)[i];
			if(ch>='A' && ch<='Z')
				((char*)output)[i] = ch + 32;
			else
				((char*)output)[i] = ch;
		}
	}
	else
	{
		for(int i=0; i<len; ++i)
		{
			unsigned char ch = ((unsigned char*)input)[i];
			if(ch>='A' && ch<='Z')
				((char*)output)[i] = ch + 32;
		}
	}
}
void mem2upper(void* output, const void* input, int len)
{
	if(output!=input)
	{
		for(int i=0; i<len; ++i)
		{
			unsigned char ch = ((unsigned char*)input)[i];
			if(ch>='a' && ch<='z')
				((char*)output)[i] = ch - 32;
			else
				((char*)output)[i] = ch;
		}
	}
	else
	{
		for(int i=0; i<len; ++i)
		{
			unsigned char ch = ((unsigned char*)input)[i];
			if(ch>='a' && ch<='z')
				((char*)output)[i] = ch - 32;
		}
	}
}

void bin2hex(void* hex_output, const void* mem, int mem_len)
{
    static char hex[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', 
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', };

	uint8_t *src = (uint8_t *)mem;
    char *des = (char *)hex_output;
    for (int i = 0; i < mem_len; ++i)
    {
        *des++ = hex[((src[i]) & 0xf0) >> 4];
        *des++ = hex[(src[i]) & 0x0f];
    }
    *des = '\0';
}

int hex2bin(void* bin_output, const void* hex, int hex_len)
{
	int len = hex_len/2;
	unsigned char val, val2;
	for(int i=0; i<len; ++i)
	{
		val = ((const char*)hex)[i*2];
		if(val>='0' && val<='9')
			val = val - (unsigned char)('0');
		else if(val>='a' && val<='f')
			val = val - (unsigned char)('a') + 10;
		else if(val>='A' && val<='F')
			val = val - (unsigned char)('A') + 10;
		else
			return -1;
		val2 = ((const char*)hex)[i*2+1];
		if(val2>='0' && val2<='9')
			val2 = val2 - (unsigned char)('0');
		else if(val2>='a' && val2<='f')
			val2 = val2 - (unsigned char)('a') + 10;
		else if(val2>='A' && val2<='F')
			val2 = val2 - (unsigned char)('A') + 10;
		else
			return -1;
		((unsigned char*)bin_output)[i] = (val<<4)+val2;
	}
	return len;
}

bool is_integer(const void* str, int len)
{
	for(int i=0; i<len; ++i)
	{
		if(((char*)str)[i]<'0' || ((char*)str)[i]>'9')
			return false;
	}
	return true;
}
bool is_hex(const void* str, int len)
{
	for(int i=0; i<len; ++i)
	{
		if(!( (((char*)str)[i]>='0'&&((char*)str)[i]<='9')
			|| (((char*)str)[i]>='a'&&((char*)str)[i]<='f')
			|| (((char*)str)[i]>='A'&&((char*)str)[i]<='F')
		))
			return false;
	}
	return true;
}
inline static bool __isalpha(const char c) { return ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }
bool util_is_email(const void* _str, int32_t len)
{
	const char* str = (const char*)_str;
    if(!__isalpha(str[0]))
        return false;
    int at_offset = -1;
    int dot_offset = -1;
    for(int i = 0; i < len; i++)
    {
        if(str[i] == '@')
            at_offset = (int)i;
        else if(str[i] == '.')
            dot_offset = (int)i;
    }
    if(at_offset == -1 || dot_offset == -1)
        return 0;
    if(at_offset > dot_offset)
        return 0;
    return !(dot_offset >= ((int)len-1));
}

uint32_t ut_strhash_djb2(const void *str, int len)
{
	uint32_t hash = 5381;
	int c, i;
	for(i=0;i<len;++i)
	{
		c=((char*)str)[i];
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}
uint32_t ut_strhash_sdbm(const void *str, int len)
{
	uint32_t hash = 0;
	int c, i;
	for(i=0;i<len;++i)
	{
		c=((char*)str)[i];
		hash = c + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}

ut_kmp * ut_kmp_create(const void* pat, int len)
{
	int i=0, j=0;
	ut_kmp* kmp = (ut_kmp*) malloc(sizeof(ut_kmp));
	kmp->patn = (unsigned char*) malloc(len);
	memcpy(kmp->patn, pat, len);
	kmp->patn_len = len;
	kmp->next = (unsigned char*) malloc(sizeof(int)*len);

	kmp->next[0] = 0;
	for (; i < len; ++i)
	{
		while (j > 0 && kmp->patn[j] != kmp->patn[i])
				j = kmp->next[j - 1];
		if (kmp->patn[j] == kmp->patn[i])
			++j;
		kmp->next[i] = j;
	}
	return kmp;
}
void ut_kmp_destroy(ut_kmp *kmp)
{
	if(NULL!=kmp->patn)
		free(kmp->patn);
	if(NULL!=kmp->next)
		free(kmp->next);
	free(kmp);
}
int ut_kmp_find(const void* haystack, int haystack_size, ut_kmp *kmp)
{
	int i=0, j=0;
	for (; i < haystack_size; ++i)
	{
		while (j > 0 && ((unsigned char*)haystack)[i] != kmp->patn[j])
			j = kmp->next[j - 1];
		if (((unsigned char*)haystack)[i] == kmp->patn[j])
			j ++;
		if (j == kmp->patn_len)
			return i + 1 - kmp->patn_len;
	}

	return -1;
}

#ifdef __cplusplus
}
#endif

void ut_ddump(const void* mem, int len, FILE* file)
{
	if(NULL==file)
		file = stdout;
	unsigned char* base = (unsigned char*)(mem);
	unsigned char ch;
	int i=0, m=0;
	char line[67];//3*16 bytes for hex text, 1 byte for \t, 16 bytes for ascii, 1 byte \n, 1 byte for \0 terminator.
	line[65] = '\n'; line[66] = '\0';
	for(i=0; i<len; i+=16)
	{
		//memset((void*)line, 0, sizeof(line));
		for(m=0; m<16; ++m)
		{
			ch = *(base+i+m);
			if(i+m>=len)
			{
				snprintf(line+3*m, 4, "-- ");
				line[3*16+1+m] = ' ';
			}
			else
			{
				snprintf(line+3*m, 4, "%02x ", ch);
				if(ch>=32 && ch<=126)
					line[3*16+1+m] = ch;
				else
					line[3*16+1+m] = '.';
			}
		}
		line[3*16] = '\t';
		if(i+16>=len)//the final line will not include a \n
			line[65] = '\0';
		fwrite(line, sizeof(line)-1, 1, file);
	}
}
std::string ut_dump(const void* mem, int len)
{
	std::string result;
	unsigned char* base = (unsigned char*)(mem);
	unsigned char ch;
	int i=0, m=0;
	char line[67];//3*16 bytes for hex text, 1 byte for \t, 16 bytes for ascii, 1 byte \n, 1 byte for \0 terminator.
	line[65] = '\n'; line[66] = '\0';
	for(i=0; i<len; i+=16)
	{
		//memset((void*)line, 0, sizeof(line));
		for(m=0; m<16; ++m)
		{
			ch = *(base+i+m);
			if(i+m>=len)
			{
				snprintf(line+3*m, 4, "-- ");
				line[3*16+1+m] = ' ';
			}
			else
			{
				snprintf(line+3*m, 4, "%02x ", ch);
				if(ch>32 && ch<=126)
					line[3*16+1+m] = ch;
				else
					line[3*16+1+m] = '.';
			}
		}
		line[3*16] = '\t';
		if(i+16>=len)//the final line will not include a \n
			line[65] = '\0';
		result.append(line,sizeof(line)-1);
	}
	return result;
}

std::string ut_hex_dump(const void *mem, size_t size)
{
	std::string result;
	uint8_t *data = (uint8_t *)mem;
    size_t n = 0;

    while (n < size)
    {
		size_t i;
		size_t len = 0;
		char line[81];

        len += snprintf(line + len, sizeof(line) - len, "0x%04zx:  ", n);

        size_t left   = size - n;
        size_t num    = left >= 16 ? 16 : left;
        size_t group  = num / 2;
        size_t single = num % 2;
        char const *pos = (char *)(data + n);

        for (i = 0; i < group; ++i)
        {
            uint8_t *curr = data + n;
            len += snprintf(line + len, sizeof(line) - len, "%02x%02x ", curr[0], curr[1]);
            n += 2;
        }

        if (group != 8)
        {
            if (single)
            {
                len += snprintf(line + len, sizeof(line) - len, "%02x   ", data[n++]);
            }

            size_t blank = 8 - group - single;
            for (i = 0; i < blank; ++i)
                len += snprintf(line + len, sizeof(line) - len, "     ");
        }

        len += snprintf(line + len, sizeof(line) - len, " ");
        for (i = 0; i < num; ++i)
        {
            char c = pos[i];
            len += snprintf(line + len, sizeof(line) - len, "%c", \
                    (c > (char)0x20 && c < (char)0x7f) ? c : '.');
        }

        if (n < size)
            snprintf(line + len, sizeof(line) - len, "\n");

		result.append(line);
    }

    return result;
}

union json_escape_t
{
	char _n[2];
	uint16_t _v;
};
inline void set(json_escape_t& t, const char* str)
{
	t._n[0] = str[0];
	t._n[1] = str[1];
}
static json_escape_t _json_escape_tbl[256];
static bool _json_escape_tbl_inited = false;
static void init_json_escape_table()
{
	if(_json_escape_tbl_inited)
		return;
	for(uint32_t i=0; i<256; ++i)
		_json_escape_tbl[i]._v = 0;
	set(_json_escape_tbl['\\'], "\\\\");
	set(_json_escape_tbl['"'], "\\\"");
	set(_json_escape_tbl['/'], "\\/");
	set(_json_escape_tbl['\b'], "\\b");
	set(_json_escape_tbl['\f'], "\\f");
	set(_json_escape_tbl['\n'], "\\n");
	set(_json_escape_tbl['\r'], "\\r");
	set(_json_escape_tbl['\t'], "\\t");
}
std::string ut_json_escape(const std::string& plain)
{
	if(!_json_escape_tbl_inited)
		init_json_escape_table();
	std::string ret;
	uint32_t i=0, size = plain.size(), last_plain = 0;
	uint8_t c;
	for(; i<size; ++i)
	{
		c = plain[i];
		if(_json_escape_tbl[c]._v)
		{
			if(i>last_plain)
				ret.append(plain, last_plain, i-last_plain);
			last_plain = i+1;
			ret.append(_json_escape_tbl[c]._n, 2);
		}
	}
	if(size>last_plain)
		ret.append(plain, last_plain, i-last_plain);
	return ret;
}

std::string ut_bool_str(bool val)
{
	if (val)
		return "true";

	return "false";
}

void ut_replace(std::string& str, const std::string& from, const std::string& to)
{
	if(from.empty())
		return;
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

uint32_t ut_split(const std::string& str, std::vector<std::string>& ret_, const std::string& sep, uint32_t limit)
{
	if (str.empty())
		return 0;
	if (limit<=0)
		return 0;
	limit -= 1;
	std::string::size_type comma_pos, find_begin_pos = 0;
	while(ret_.size()<limit)
	{
		comma_pos = str.find(sep, find_begin_pos);
		if (comma_pos != std::string::npos)
		{
			ret_.push_back( str.substr(find_begin_pos, comma_pos-find_begin_pos) );
			find_begin_pos = comma_pos + sep.size();
		}
		else
		{
			break;
		}
	}
	if(str.size() - find_begin_pos>0)
		ret_.push_back( str.substr(find_begin_pos) );
	return ret_.size();
}

uint32_t ut_split2(const std::string& str, std::vector<std::string>& ret_, const std::string& sep, uint32_t limit)
{
	//if (str.empty())
	//	return 0;
	if (limit<=0)
		return 0;
	limit -= 1;
	std::string::size_type comma_pos, find_begin_pos = 0;
	while(ret_.size()<limit)
	{
		comma_pos = str.find(sep, find_begin_pos);
		if (comma_pos != std::string::npos)
		{
			ret_.push_back( str.substr(find_begin_pos, comma_pos-find_begin_pos) );
			find_begin_pos = comma_pos + sep.size();
		}
		else
		{
			break;
		}
	}
	//if(str.size() - find_begin_pos>0)
		ret_.push_back( str.substr(find_begin_pos) );
	return ret_.size();
}

void ut_trim(std::string& str)
{
	std::string::size_type p1 = str.find_first_not_of(" \r\n\t");
	std::string::size_type p2 = str.find_last_not_of(" \r\n\t");
	str.erase(0,p1);
	if(!str.empty() && p2!=std::string::npos)
	{
		p2 -= p1;
		if(str.size() > p2+1)
			str.erase(p2+1);
	}
}

/*
 unicode编码从0xFF01~0xFF5E为全角字符, 对应的半角字符为unicode编码从0x21~0x7E,
 空格比较特殊, 全角为0x3000, 半角为0x20；
 除空格外, 全角/半角按unicode编码排序在顺序上是对应的
 全角字符在utf-8下是三个字节表示,,具体表示为 1110xxxx 10xxxxxx 10xxxxxx
 */
void ut_utf8str_full2half(std::string & str)
{
    std::string temp;
    for (size_t i = 0; i < str.size(); i++) {
        if (((str[i] & 0xF0) ^ 0xE0) == 0) {
            int old_char = (str[i] & 0xF) << 12 | ((str[i + 1] & 0x3F) << 6 | (str[i + 2] & 0x3F));
            if (old_char == 0x3000) { // blank
                char new_char = 0x20;
                temp += new_char;
            } else if (old_char >= 0xFF01 && old_char <= 0xFF5E) { // full char
                char new_char = old_char - 0xFEE0;
                temp += new_char;
            } else { // other 3 bytes char
                temp += str[i];
                temp += str[i + 1];
                temp += str[i + 2];
            }
            i = i + 2;
        } else {
            temp += str[i];
        }
    }
    str = temp;
}


/*
 * wry.h
 *
 *  Created on: 2013-9-22
 *      Author: echo
 *       Brief: the analyser for qqwry.dat(a popular IP dictionary in China, web site: http://www.cz88.net/)
 */

#ifndef UT_WRY_H_
#define UT_WRY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct wry_head
{
	int32_t first_index_offset;
	int32_t last_index_offset;
};

struct wry_loc
{
	uint8_t nation_len;
	uint8_t city_len;
	uint8_t* nation;
	uint8_t* city;
};

struct wry_index
{
	uint32_t ip_start;
	uint32_t ip_end;
	int32_t offset;
	struct wry_loc location;
};

struct wry_dict
{
	struct wry_head head;
	uint8_t* location_mem;
	int32_t location_mem_len;
	int32_t ip_list_size;
	struct wry_index* ip_list;
};

/**
 * initialize the wry_dict structure
 * this will read the ip-location data file into memory, and analysis data, then save result in the dictionary
 * return 0 on success
 */
int wry_dict_init(struct wry_dict* dict, const char* dictionary_file);

/**
 * uninit the wry_dict
 */
void wry_dict_uninit(struct wry_dict* dict);

/**
 * find an ip in the dictionary, get the ip_location of this ip.
 * notice: ip is an integer value in host byte order.
 * return 0 if found.
 */
struct wry_loc * wry_find(struct wry_dict* dict, uint32_t ip);

#ifdef __cplusplus
}
#endif

#endif /* UT_WRY_H_ */

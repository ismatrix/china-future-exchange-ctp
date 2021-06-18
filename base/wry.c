/*
 * wry.c
 *
 *  Created on: 2013-9-22
 *      Author: echo
 */
#include "wry.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SLIM_IP_INDEX_SIZE 7

static inline unsigned int get_three_bytes_int(uint8_t* p)
{
	union
	{
		unsigned int v;
		uint8_t byte[4];
	}data;
	data.byte[0] = p[0];
	data.byte[1] = p[1];
	data.byte[2] = p[2];
	data.byte[3] = 0;
	return data.v;
}

int wry_dict_init(struct wry_dict* dict, const char* dictionary_file)
{
	memset((void*)(dict), 0, sizeof(struct wry_dict));
	FILE* f = NULL;
	f = fopen(dictionary_file, "rb");
	if(NULL==f)
		return -1;

	//read file head, and alloc memry for the data
	if(1!=fread((void*)(&dict->head), sizeof(struct wry_head), 1, f))
	{
		fclose(f);
		return -1;
	}
	dict->location_mem_len = dict->head.first_index_offset - sizeof(struct wry_head);
	dict->location_mem = (uint8_t*)( malloc(dict->location_mem_len) );
	dict->ip_list_size = (dict->head.last_index_offset - dict->head.first_index_offset + SLIM_IP_INDEX_SIZE)/SLIM_IP_INDEX_SIZE;
	dict->ip_list = (struct wry_index*)( calloc(dict->ip_list_size, sizeof(struct wry_index)) );
	memset((void*)(dict->ip_list), 0, dict->ip_list_size * sizeof(struct wry_index));

	//read location record data into mem
	if(1!=fread((void*)(dict->location_mem), dict->location_mem_len, 1, f))
	{
		fclose(f);
		wry_dict_uninit(dict);
		return -1;
	}

	//clear cz88.net string
	dict->location_mem[9] = (uint8_t)('\0');
	dict->location_mem[10] = (uint8_t)('\0');

	//read index data
	int slim_ip_buf_len = dict->head.last_index_offset - dict->head.first_index_offset + SLIM_IP_INDEX_SIZE;
	uint8_t* slim_ip_buf = (uint8_t*)( malloc(slim_ip_buf_len) );
	if(1!=fread((void*)(slim_ip_buf), slim_ip_buf_len, 1, f))
	{
		fclose(f);
		wry_dict_uninit(dict);
		free(slim_ip_buf);
		return -1;
	}
	fclose(f);
	f = NULL;

	int i = 0;
	for(i=0;i<dict->ip_list_size;++i)
	{
		uint8_t* slim = slim_ip_buf + i*SLIM_IP_INDEX_SIZE;
		dict->ip_list[i].ip_start = *( (int*)(slim) );
		dict->ip_list[i].offset = get_three_bytes_int(slim+4);//(int)(slim[4]) + ((int)(slim[5]) << 8) + ((int)(slim[6]) << 16);
		if(i==dict->ip_list_size-1)
			dict->ip_list[i].ip_end = 0xFFFFFFFF;
		else
			dict->ip_list[i].ip_end = *( (int*)(slim+SLIM_IP_INDEX_SIZE) );
	}

	free(slim_ip_buf);
	return 0;
}

void wry_dict_uninit(struct wry_dict* dict)
{
	if(dict)
	{
		if(dict->location_mem)
		{
			free(dict->location_mem);
			dict->location_mem = NULL;
		}
		if(dict->ip_list)
		{
			free(dict->ip_list);
			dict->ip_list = NULL;
		}
		memset((void*)(dict), 0, sizeof(struct wry_dict));
	}
}

struct wry_loc * wry_find(struct wry_dict* dict, uint32_t ip)
{
	struct wry_index* found_idx = NULL;
	int start_pos = 0, end_pos = dict->ip_list_size-1;
	struct wry_index* idx = dict->ip_list + start_pos;
	if(ip > idx->ip_end)
	{
		idx = dict->ip_list + end_pos;
		if(ip < idx->ip_start)
		{
			int mid_pos = 0;
			while( (mid_pos=(end_pos+start_pos)/2) != start_pos )
			{
				//printf("start=%d[%u,%u],end=%d[%u,%u],mid=%d[%u,%u]\n", start_pos, dict->ip_list[start_pos].ip_start, dict->ip_list[start_pos].ip_end, end_pos, dict->ip_list[end_pos].ip_start, dict->ip_list[end_pos].ip_end, mid_pos, dict->ip_list[mid_pos].ip_start, dict->ip_list[mid_pos].ip_end);
				struct wry_index* idx = dict->ip_list + mid_pos;
				if(ip<idx->ip_start)
				{
					end_pos = mid_pos-1;
					//printf("end_pos move to mid_pos-1\n");
					//if(1==end_pos-start_pos)
					//{
						idx = dict->ip_list + end_pos;
						if(ip>=idx->ip_start && ip<=idx->ip_end)
						{
							found_idx = idx;
							break;
						}
					//}
					continue;
				}
				else if(ip>=idx->ip_end)
				{
					start_pos = mid_pos+1;
					//printf("start_pos move to mid_pos+1\n");
					//if(1==end_pos-start_pos)
					//{
						idx = dict->ip_list + start_pos;
						if(ip>=idx->ip_start && ip<=idx->ip_end)
						{
							found_idx = idx;
							break;
						}
					//}
					continue;
				}
				found_idx = idx;
				break;
			}
		}
		else if(ip <= idx->ip_end)
		{
			found_idx = idx;
		}
	}
	else if(ip >= idx->ip_start)
	{
		found_idx = idx;
	}

	if(found_idx)
	{
		//if(found_idx->location.isp != (uint8_t)(UTIL_ISP_NOT_CHECKED))
		//	return &found_idx->location;

		uint8_t* rec = dict->location_mem + found_idx->offset - sizeof(struct wry_head);
		found_idx->ip_end = *((unsigned int*)(rec));

		struct wry_loc* loc = &found_idx->location;

		rec += 4;
		uint8_t redirect_mode = (uint8_t)(rec[0]);
		if(redirect_mode==(uint8_t)(1))//redirect mode 1
		{
			unsigned int offset = get_three_bytes_int(rec+1);
			if(0==offset)
				return NULL;
			rec = dict->location_mem + offset - sizeof(struct wry_head);
			redirect_mode = (uint8_t)(rec[0]);
			if(redirect_mode!=(uint8_t)(2))
			{
				loc->nation = (uint8_t*)(rec);
				loc->nation_len = strlen((char*)loc->nation);
				rec = rec + loc->nation_len + 1;
				redirect_mode = rec[0];
				if(redirect_mode!=(uint8_t)(1) && redirect_mode!=(uint8_t)(2))
				{
					//192.168.1.1 in this case
					loc->city = (uint8_t*)rec;
					loc->city_len = strlen((char*)(loc->city));
				}
				else
				{
					//121.35.244.19 in this case, .35 dev machine
					offset = get_three_bytes_int(rec+1);
					if(0==offset)
						return NULL;
					loc->city = (uint8_t*)(dict->location_mem) + offset - sizeof(struct wry_head);
					loc->city_len = strlen((char*)(loc->city));
				}
			}
			else
			{
				offset = get_three_bytes_int(rec+1);
				loc->nation = (uint8_t*)(dict->location_mem) + offset - sizeof(struct wry_head);
				loc->nation_len = strlen((char*)(loc->nation));
				redirect_mode = (uint8_t)(rec[4]);
				if(redirect_mode==(uint8_t)(1) || redirect_mode==(uint8_t)(2))
				{
					offset = get_three_bytes_int(rec+5);
					if(0==offset)
					{
						loc->city = NULL;
						loc->city_len = 0;
					}
					else
					{
						loc->city = (uint8_t*)(dict->location_mem) + offset - sizeof(struct wry_head);
						loc->city_len = strlen((char*)(loc->city));
					}
				}
				else
				{
					//208.109.4.201 in this case, godaddy.com
					loc->city = (uint8_t*)(rec) + 4;
					loc->city_len = strlen((char*)(loc->city));
				}
			}
		}
		else if(redirect_mode==(uint8_t)(2))//redirect mode 2
		{
			//69.163.34.122 in this case, VPS echoma.com
			unsigned int offset = get_three_bytes_int(rec+1);
			loc->nation = (uint8_t*)(dict->location_mem) + offset - sizeof(struct wry_head);
			loc->nation_len = strlen((char*)(loc->nation));
			loc->city = (uint8_t*)(rec) + 4;
			loc->city_len = strlen((char*)(loc->city));
		}
		else//no redirect
		{
			loc->nation = (uint8_t*)(rec);
			loc->nation_len = strlen((char*)(loc->nation));
			rec = rec + loc->nation_len + 1;
			redirect_mode = rec[0];
			if(redirect_mode!=(uint8_t)(1) && redirect_mode!=(uint8_t)(2))
			{
				loc->city = (uint8_t*)rec;
				loc->city_len = strlen((char*)(loc->city));
			}
			else
			{
				//127.0.0.1 in this case
				unsigned int offset = get_three_bytes_int(rec+1);
				if(0==offset)
					return NULL;
				loc->city = (uint8_t*)(dict->location_mem) + offset - sizeof(struct wry_head);
				loc->city_len = strlen((char*)(loc->city));
			}
		}

		if(ip>found_idx->ip_end)
			return NULL;
		return loc;
	}

	return NULL;
}

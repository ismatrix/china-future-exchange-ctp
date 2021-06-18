/*
 * nw_ses_buf.cpp
 *
 *  Created on: 2013-9-27
 *      Author: echo
 */
#include "nw_ses_buf.h"
#include <algorithm>

bool nw_ses_buf_pool::init(int count, int size)
{
	int mem_size = count * size;
	_buf = new unsigned char[mem_size];
	if(NULL==_buf)
		return false;
	if(!_fact.init(count))
		return false;
	nw_ses_buf* arr = _fact.get_product_array();
	for(int i=0; i<count; ++i)
	{
		arr[i]._buf = _buf + (i*size);
		arr[i]._rpos = arr[i]._wpos = 0;
		arr[i]._size = size;
		arr[i]._next = NULL;
	}
	return true;
}

int nw_ses_buf_list::read(void* buf, int max_size)
{
	int pos = 0;
	nw_ses_buf* cur = _beg;
	while(NULL != cur)
	{
		int ra = cur->ravai();
		if(ra > 0)
		{
			int s = ra;
			if(s > max_size-pos)
				s = max_size - pos;
			memcpy((char*)buf+pos, cur->_buf+cur->_rpos, ra);
			cur->_rpos += s;
			pos += s;
			if(s < ra)
				break;
		}
		cur = cur->_next;
	}
	return pos;
}
int nw_ses_buf_list::pre_read(void* buf, int max_size)
{
	int pos = 0;
	nw_ses_buf* cur = _beg;
	while(NULL != cur)
	{
		int ra = cur->ravai();
		if(ra > 0)
		{
			int s = ra;
			if(s > max_size-pos)
				s = max_size - pos;
			memcpy((char*)buf+pos, cur->_buf+cur->_rpos, ra);
			pos += s;
			if(s < ra)
				break;
		}
		cur = cur->_next;
	}
	return pos;
}
int nw_ses_buf_list::read(int max_size)
{
	int pos = 0;
	nw_ses_buf* cur = _beg;
	while(NULL != cur)
	{
		int ra = cur->ravai();
		if(ra > 0)
		{
			int s = ra;
			if(s > max_size-pos)
				s = max_size - pos;
			cur->_rpos += s;
			pos += s;
			if(s < ra)
				break;
		}
		cur = cur->_next;
	}
	return pos;
}
void nw_ses_buf_list::move_data_to_begin()
{
	nw_ses_buf *rcur = _beg, *wcur = _beg;
	while(NULL!=rcur && NULL!=wcur)
	{
		if(rcur->_rpos>0 && rcur->_rpos<rcur->_wpos)
		{
			if(wcur==rcur)
			{
				wcur->move_data_to_begin();
				rcur = rcur->_next;
				if(0==wcur->wspace())
					wcur = wcur->_next;
			}
			else
			{
				int size = std::min(wcur->wspace(), rcur->ravai());
				memmove(wcur->_buf+wcur->_wpos, rcur->_buf+rcur->_rpos, size);
				wcur->_wpos += size;
				rcur->_rpos += size;
				if(0==rcur->ravai())
					rcur = rcur->_next;
				if(0==wcur->wspace())
					wcur = wcur->_next;
			}
		}
		else
		{
			rcur = rcur->_next;
		}
	}
}

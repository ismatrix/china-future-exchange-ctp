/*
 * nw_ses_buf.h
 *
 *  Created on: 2013-9-27
 *      Author: echo
 *       Brief: basic buffer management for session.
 */

#ifndef NW_SES_BUF_H_
#define NW_SES_BUF_H_

#include "ut_list.h"
#include <string.h>

struct nw_ses_buf
{
	unsigned char* _buf;
	int _rpos;
	int _wpos;
	int _size;
	nw_ses_buf* _next;

	inline void read(void* buf) { memcpy(buf, _buf+_rpos, _wpos-_rpos); _rpos = _wpos = 0; }
	inline void read(void* buf, int max_size)
	{
		int s = max_size>(_wpos-_rpos)? (_wpos-_rpos) : max_size;
		memcpy(buf, _buf+_rpos, s);
		_rpos += s;
	}
	inline void read(int size)
	{
		if(_rpos+size < _wpos)
			_rpos += size;
		else
			_rpos = _wpos;
	}

	inline int write(const void* buf, int size)
	{
		if(_wpos+size<=_size)
		{
			memcpy(_buf+_wpos, buf, size);
			_wpos += size;
			return size;
		}
		else
		{
			int s = _size - _wpos;
			memcpy(_buf+_wpos, buf, s);
			_wpos += s;
			return s;
		}
	}

	inline void move_data_to_begin()
	{
		if(_rpos>0)
		{
			memmove(_buf, _buf+_rpos, _wpos-_rpos);
			_wpos -= _rpos;
			_rpos = 0;
		}
	}

	inline void reset()
	{
		_rpos = _wpos = 0;
	}

	inline int wspace() const { return _size - _wpos; }
	inline int ravai() const { return _wpos - _rpos; }
};


//初始化一块连续空间 然后分成count * size个内存快
//ut_factory其实是一个队列管理的内存池 来管理count个内存块。
//现初始化这个内存池 然后每次获取内存池数据 固定大小包内存 有内存浪费。 但是效率会高
///a simple buffer pool producing fix-sized and unify-sized buffer
struct nw_ses_buf_pool
{
	unsigned char* _buf;
	typedef ut_factory<nw_ses_buf> fact_t;
	fact_t _fact;

	inline nw_ses_buf_pool():_buf(NULL) { }
	~nw_ses_buf_pool()
	{
		if(NULL!=_buf)
		{
			delete[] _buf;
			_buf = NULL;
		}
	}
	bool init(int count, int size);

	inline nw_ses_buf* create()
	{
		nw_ses_buf* b;
		if(_fact.create(b))
		{
			b->_next = NULL;
			b->_rpos = b->_wpos = 0;
			return b;
		}
		return NULL;
	}
	inline bool destroy(nw_ses_buf* buf)
	{
		return _fact.release(buf);
	}
};

//这个通过获取内存池的内存块存储数据 并把内存块指针存到这里的list中。
struct nw_ses_buf_list
{
	unsigned int _count;
	nw_ses_buf* _beg;
	nw_ses_buf* _end;

	inline nw_ses_buf_list():_count(0), _beg(NULL), _end(NULL) { }

	inline void reset()
	{
		_count = 0;
		_beg = _end = NULL;
	}

	/**
	 * read at most @max_size bytes data into @buf, the the read-pointer was moved to the next @max_size byte
	 * return the number of bytes it really read.
	 */
	int read(void* buf, int max_size);
	//same as @read(), but the read-pointer will not moved.
	int pre_read(void* buf, int max_size);
	//move the read-pointer to the next @max_size bytes only
	int read(int max_size);
	//remove the useless space after read(), this will move all data in all buffers to the beginning of the buffer list
	void move_data_to_begin();
	//clear all data in all buffers
	inline void clear_data()
	{
		nw_ses_buf *cur = _beg;
		while(NULL!=cur)
		{
			cur->reset();
			cur = cur->_next;
		}
	}

	inline void append(nw_ses_buf* buf)
	{
		if(NULL==_beg)
		{
			_beg = _end = buf;
		}
		else
		{
			_end->_next = buf;
			_end = buf;
		}
		++_count;
	}
	inline nw_ses_buf* shift()
	{
		if(NULL!=_beg)
		{
			nw_ses_buf* tmp = _beg;
			_beg = _beg->_next;
			--_count;
			return tmp;
		}
		return NULL;
	}
};

#endif /* NW_SES_BUF_H_ */

/*
 * fifo.c
 *
 *  Created on: 2013-9-19
 *      Author: echo
 */
#include "fifo.h"
#include <string.h>

void fifo_init(Fifo* fifo, void* buf, int len)
{
	memset(fifo, 0, sizeof(Fifo));
	fifo->buf = buf;
	fifo->size = len;
}

bool fifo_write(Fifo* fifo, const void* buf, int len)
{
	int old_rpos = fifo->rpos, old_wpos = fifo->wpos;
	if (old_rpos > old_wpos)
	{
		if(old_rpos-old_wpos > len)
		{
			memcpy(fifo->buf+old_wpos, buf, len);
			fifo->wpos += len;
			return true;
		}
	}
	else if(old_wpos+len < fifo->size)
	{
		memcpy(fifo->buf+old_wpos, buf, len);
		fifo->wpos += len;
		return true;
	}
	else if(fifo->size-old_wpos+old_rpos > len)
	{
		int ifirst = fifo->size-old_wpos;
		memcpy(fifo->buf+old_wpos, buf, ifirst);
		memcpy(fifo->buf, buf+ifirst, len-ifirst);
		fifo->wpos = len-ifirst;
		return true;
	}
	return false;
}

bool fifo_read(Fifo* fifo, void* buf, int len)
{
	int old_rpos = fifo->rpos, old_wpos = fifo->wpos;
	if (old_rpos <= old_wpos)
	{
		if(old_wpos-old_rpos >= len)
		{
			memcpy(buf, fifo->buf+old_rpos, len);
			fifo->rpos += len;
			return true;
		}
	}
	else if(old_rpos + len < fifo->size)
	{
		memcpy(buf, fifo->buf+old_rpos, len);
		fifo->rpos += len;
		return true;
	}
	else if(fifo->size-old_rpos+old_wpos >= len)
	{
		int ifirst = fifo->size-old_rpos;
		memcpy(buf, fifo->buf+old_rpos, ifirst);
		memcpy(buf+ifirst, fifo->buf, len-ifirst);
		fifo->rpos = len-ifirst;
		return true;
	}
	return false;
}

bool fifo_pre_write(Fifo* fifo, const void* buf, int len)
{
	int old_rpos = fifo->rpos, old_wpos = fifo->wpos;
	if (old_rpos > old_wpos)
	{
		if(old_rpos-old_wpos > len)
		{
			memcpy(((char*)fifo->buf)+old_wpos, buf, len);
			return true;
		}
	}
	else if(old_wpos+len < fifo->size)
	{
		memcpy(((char*)fifo->buf)+old_wpos, buf, len);
		return true;
	}
	else if(fifo->size-old_wpos+old_rpos > len)
	{
		int ifirst = fifo->size-old_wpos;
		memcpy(((char*)fifo->buf)+old_wpos, buf, ifirst);
		memcpy((char*)fifo->buf, buf+ifirst, len-ifirst);
		return true;
	}
	return false;
}

bool fifo_pre_read(const Fifo* fifo, void* buf, int len)
{
	int old_rpos = fifo->rpos, old_wpos = fifo->wpos;
	if (old_rpos <= old_wpos)
	{
		if(old_wpos-old_rpos >= len)
		{
			memcpy(buf, ((const char*)fifo->buf)+old_rpos, len);
			return true;
		}
	}
	else if(old_rpos + len < fifo->size)
	{
		memcpy(buf, ((const char*)fifo->buf)+old_rpos, len);
		return true;
	}
	else if(fifo->size-old_rpos+old_wpos >= len)
	{
		int ifirst = fifo->size-old_rpos;
		memcpy(buf, ((const char*)fifo->buf)+old_rpos, ifirst);
		memcpy(buf+ifirst, ((const char*)fifo->buf), len-ifirst);
		return true;
	}
	return false;
}

bool fifo_wseek(Fifo* fifo, unsigned int offset)
{
	int old_rpos = fifo->rpos, old_wpos = fifo->wpos;
	if (old_rpos > old_wpos)
	{
		if(old_rpos-old_wpos > offset)
		{
			fifo->wpos += offset;
			return true;
		}
	}
	else if(old_wpos+offset < fifo->size)
	{
		fifo->wpos += offset;
		return true;
	}
	else if(fifo->size-old_wpos+old_rpos > offset)
	{
		int ifirst = fifo->size-old_wpos;
		fifo->wpos = offset-ifirst;
		return true;
	}
	return false;
}

bool fifo_rseek(Fifo* fifo, unsigned int offset)
{
	int old_rpos = fifo->rpos, old_wpos = fifo->wpos;
	if (old_rpos <= old_wpos)
	{
		if(old_wpos-old_rpos >= offset)
		{
			fifo->rpos += offset;
			return true;
		}
	}
	else if(old_rpos + offset < fifo->size)
	{
		fifo->rpos += offset;
		return true;
	}
	else if(fifo->size-old_rpos+old_wpos >= offset)
	{
		int ifirst = fifo->size-old_rpos;
		fifo->rpos = offset-ifirst;
		return true;
	}
	return false;
}

int fifo_wspace(const Fifo* fifo)
{
	int old_rpos = fifo->rpos, old_wpos = fifo->wpos;
	if(old_rpos > old_wpos)
		return old_rpos - old_wpos - 1;
	else
		return fifo->size - old_wpos + old_rpos - 1;
}

int fifo_ravai(const Fifo* fifo)
{
	int old_rpos = fifo->rpos, old_wpos = fifo->wpos;
	if(old_wpos >= old_rpos)
		return old_wpos - old_rpos;
	else
		return fifo->size - old_rpos + old_wpos;
}

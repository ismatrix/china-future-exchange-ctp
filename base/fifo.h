/*
 * fifo.h
 *
 *  Created on: 2013-9-18
 *      Author: echo
 *       Brief: a circle fifo buffer, can be used concurrently by a reader thread and a writer thread.
 */

#ifndef UT_FIFO_H_
#define UT_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

///the fifo structure
typedef struct
{
	void* buf;	/// the buffer
	int size;	/// size of the buffer
	int wpos;	/// the write position
	int rpos;	/// the read position
} Fifo;

///initialize the @fifo
void fifo_init(Fifo* fifo, void* buf, int size);

///get max size of @fifo
inline int fifo_size(const Fifo* fifo) { return fifo->size; }

///reset the @fifo
inline void fifo_reset(Fifo* fifo) { fifo->wpos = fifo->rpos = 0; }

/**
 * write data into a fifo at the write position.
 * on success, return true.
 * if @buf couldn't be written entirely, nothing will be written, and false is returned.
 */
bool fifo_write(Fifo* fifo, const void* buf, int len);

/**
 * read data from a fifo at the read position.
 * on success, return true.
 * if there was not enough data could be read, nothing will be read, and false is returned.
 */
bool fifo_read(Fifo* fifo, void* buf, int len);

/**
 * similar to @fifo_write(), but do not move the write position
 */
bool fifo_pre_write(Fifo* fifo, const void* buf, int len);

/**
 * similar to @fifo_read(), but do not move the read position
 */
bool fifo_pre_read(const Fifo* fifo, void* buf, int len);

/**
 * set the current write position, like fseek(fd,offset,SEEK_SET)
 * return true on success. otherwise return false;
 */
bool fifo_wseek(Fifo* fifo, unsigned int offset);
///get the current write position
inline int fifo_wtell(const Fifo* fifo) { return fifo->wpos; }

/**
 * set the current read position, like lseek(fd,offset,SEEK_SET)
 * return true on success. otherwise return false;
 */
bool fifo_rseek(Fifo* fifo, unsigned int offset);
///get the current read position
inline int fifo_rtell(const Fifo* fifo) { return fifo->rpos; }

///get number of left space can be written
int fifo_wspace(const Fifo* fifo);

///get number of left bytes can be read
int fifo_ravai(const Fifo* fifo);

#ifdef __cplusplus
}
#endif

#endif /* UT_FIFO_H_ */

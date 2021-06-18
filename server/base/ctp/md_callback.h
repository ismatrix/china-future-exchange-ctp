/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 汇海基金
///@file md_callback.h
///@brief 行情回调队列处理函数(多线程用)
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __MD_CALLBACK__
#define __MD_CALLBACK__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fifo.h"
#include "RollLog.h"

/*
typedef struct
{
    void* buf;  /// the buffer
    int size;   /// size of the buffer
    int wpos;   /// the write position
    int rpos;   /// the read position
} Fifo;
*/
extern  Fifo md_fifo;

//初始化队列
int  md_fifo_init(int size);
//入队
int md_fifo_push(const char* pin, uint32_t len);
//出队
int md_fifo_pop(char* pout, uint32_t len);


#endif


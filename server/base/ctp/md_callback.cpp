/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 汇海基金
///@file md_callback.cpp
///@brief 行情回调队列处理函数(多线程用)
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#include "md_callback.h"

Fifo md_fifo;


//无锁队列


int  md_fifo_init(int size)
{
	char* pbuf  = (char*)malloc(size);
	fifo_init(&md_fifo, pbuf, size);
	return 0;
}

int md_fifo_push(const char* pin, uint32_t len)
{
	static uint32_t no = 0;
	if(no++ % 10000 == 0)LOG_DEBUG("left space:" << fifo_wspace(&md_fifo) << " write_len:" << len);
	//LOG_DEBUG("left space:" << fifo_wspace(&md_fifo) << " write_len:" << len);
	if(fifo_write(&md_fifo, pin,  len))
	{
		//写成功了
		return 0;
	}
	//没空间了
	
	LOG_ERROR("no space to write. left space:" << fifo_wspace(&md_fifo) << " write_len:" << len);
	return -1;
} 

int md_fifo_pop(char* pout, uint32_t len)
{
	static uint32_t no = 0;
	if(no++ % 10000 == 0) LOG_DEBUG("can read:" << fifo_ravai(&md_fifo) << " read_len:" << len);
	//LOG_DEBUG("can read:" << fifo_ravai(&md_fifo) << " read_len:" << len);
	if(fifo_read(&md_fifo, pout,  len))
	{
		//读成功了
		return 0;
	}
	//没数据了
	//LOG_DEBUG("no data to  read:" << fifo_ravai(&md_fifo) << " read_len:" << len);
	return -1;
}

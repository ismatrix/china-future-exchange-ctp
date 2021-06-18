/*************************************************************************
    > File Name: snap.h
    > Created Time: Tue 26 Jul 2016 01:34:57 PM CST
    > Author: 
    > description: 定时更新日k数据到db  
 ************************************************************************/

#include<iostream>

#include "conf.h"
#include "helper.h"
#include "ProductIndex.h"

using namespace std;


struct DayBarEx
{
	::CM::DayBar daybar;
	uint32_t	 t;
	string instrumentid;
	string tradingday;
	string productid;
};

class CDayBar : public CProductIndex
{
public:

	void init(Conf& conf);	//初始化线程负责数据库加载初始数据 每隔5s落地一次

	void flush(DayBarEx& daybar);			//数据刷到mongo

	void set(string instrumentid, string tradingday, string productid, ::CM::DayBar& daybar);		//更新内存数据

	void clear();

	void run();				//线程主处理逻辑
	static map<string, DayBarEx > _map_snap_tmp;

private:
	map<string, DayBarEx > _map_snap;
	pthread_mutex_t			 _mutex ;
	CMongodb				db;
	uint32_t				interval;	//处理数据时间间隔
};



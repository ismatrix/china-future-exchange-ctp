/*************************************************************************
    > File Name: holidays.h
    > Created Time: Wed 11 May 2016 05:49:37 PM CST
    > Author: 
    > description: 根据配置和规则推断当前是不是节假日 
 ************************************************************************/

#ifndef __HOLIDAYS_H__
#define __HOLIDAYS_H__


#include <iostream>
#include <string>
#include <list>
#include <time.h>
#include <stdint.h>

#include "RollLog.h"
#include "util.h"
#include "mongoc_json.h"
using namespace std;


//1. 节假日为包含周末的区间  昨天晚上无夜市的都要进节假日表
//2. HOLIDAYS 初始化依赖这个节假日表

struct Holiday
{
	string		market;			//对应的市场
	time_t	timestamp;		//对应的当天的时间戳     2016-06-02 09:30:00的时间戳
	time_t	next_open_time;	//对应的下个开盘时间戳   2016-06-02 21:00:00 或者 2016-06-05 09：30 
};


class Holidays
{
	public:
			
		int Init(CMongodb& db);

		//增加一条记录
		void Add(time_t timestamp, string market="future", time_t next_open_time=0);
		
		//判断是不是节假日
		bool  IsHoladay(time_t timestamp=time(NULL), string market="future");

		//判断今天是不是周末
		bool IsWeekend(time_t timestamp=time(NULL));

		//判断今天要不要开日市
		bool IsOpenDay(time_t timestamp=time(NULL), string market="future");

		//判断今天要不要开夜市
		bool IsOpenNight(time_t timestamp=time(NULL), string market="future");

		//获取今天的初始交易日 以20点划分今天昨天
		time_t GetTradingDay(time_t timestamp=time(NULL), string market="future");
		
		//获取上个交易日
		time_t GetPrevTradingDay(time_t timestamp=time(NULL), string market="future");

		//获取下个交易日
		time_t GetNextTradingDay(time_t timestamp=time(NULL), string market="future");

		//判断当前时间是不是需要交易等
		bool IsTrading(time_t timestamp=time(NULL), string market="future");

		//输出内存数据
		void Print();
	private:
		list<Holiday> m_holidays;
};

extern Holidays g_holidays;

#endif




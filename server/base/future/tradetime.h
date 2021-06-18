/*************************************************************************
    > File Name: tradetime.h
    > Created Time: Fri 18 Nov 2016 04:12:33 PM CST
    > Author: 
    > description: 
		交易时间段的检查控制 配置的区间为需要检查和不需要检查的段
 ************************************************************************/
 
#ifndef  __TRADETIME_H__
#define  __TRADETIME_H__

#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>

#include "mongoc_json.h"
#include "util.h"

using namespace std;


/*
 * 不在区间内就是收盘了 交易提前5s 收盘退后5s
 * //CFFEX 
 * db.TRADETIME.INSERT({"market": "CFFEX", "starttime" : "090100", "endtime" : "091500",  "statusm" : "集合竞价",   "status" : 4});
 * db.TRADETIME.INSERT({"market": "CFFEX", "starttime" : "091500", "endtime" : "113000",  "statusm" : "上午交易",   "status" : 5});
 * db.TRADETIME.INSERT({"market": "CFFEX", "starttime" : "113000", "endtime" : "130000",  "statusm" : "午间休市",   "status" : 7});
 * db.TRADETIME.INSERT({"market": "CFFEX", "starttime" : "130000", "endtime" : "151500",  "statusm" : "下午交易",   "status" : 8});
 * db.TRADETIME.INSERT({"market": "CFFEX", "starttime" : "151500", "endtime" : "153000",  "statusm" : "结算时间",   "status" : 9});
 * 
 * //SHFE DCE   CZCE   涵盖了中金所交易时间段 全文就用这一个    
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "085500" ,   "endtime" : "090000" ,  "statusm" : "上午集合竞价"  ,	"status" : 4 });   
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "090000" ,   "endtime" : "101500" ,  "statusm" : "上午交易"      , 	"status" : 5 });
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "101500" ,   "endtime" : "103000" ,  "statusm" : "上午临时休市"  ,	"status" : 6 });
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "103000" ,   "endtime" : "113000" ,  "statusm" : "上午交易"      ,	"status" : 5 });
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "113000" ,   "endtime" : "133000" ,  "statusm" : "午间休市"      ,	"status" : 7 });
 * //db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "133000" ,   "endtime" : "150000" ,  "statusm" : "下午交易"      ,	"status" : 8 });
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "133000" ,   "endtime" : "151500" ,  "statusm" : "下午交易"      ,	"status" : 8 });
 * //db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "150000" ,   "endtime" : "151500" ,  "statusm" : "下午结算"      ,	"status" : 9 });
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "151500" ,   "endtime" : "153000" ,  "statusm" : "下午结算"      ,	"status" : 9 });
 * //db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "151500" ,   "endtime" : "205500" ,  "statusm" : "收盘时间"      ,	"status" : 10});
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "153000" ,   "endtime" : "205500" ,  "statusm" : "收盘时间"      ,	"status" : 10});
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "205500" ,   "endtime" : "210000" ,  "statusm" : "夜市集合竞价"  ,	"status" : 1 });
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "210000" ,   "endtime" : "240000" ,  "statusm" : "夜盘"          ,	"status" : 2 });
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "000000" ,   "endtime" : "023000" ,  "statusm" : "夜盘"          ,	"status" : 2 });
 * db.TRADETIME.INSERT({"market": "SHFE", "starttime" : "023000" ,   "endtime" : "085500" ,  "statusm" : "夜盘休市时间"  ,	"status" : 3 });
 *
 *  期货交易时间目的：
 *  1. 开盘一些操作   暂时没有
 *  2. 收盘一些操作   刷数据到内存 并清空内存历史数据
 *  3. 屏蔽非开盘时间的数据流 ----对于期货来说没有问题
 *
 *  交易日切换的时候已经做了一些工作。
*/
struct TimeSection
{
	string		market;	//future stock forex option
	string		statusm;//盘前竞价  早盘1 早盘1间歇 早盘2 午休 午盘 收盘 盘后结算 收盘 夜市盘前 夜市 夜市结束
	uint32_t	status;	//市场状态 根据状态确定是开盘还是收盘 以及处理
	uint32_t	starttime;
	uint32_t	endtime;
};

//检查时间对应的状态
class TradeTime
{
	public:

	//初始化配置数据
	int Init(CMongodb& db);	

	uint32_t GetMarketStatus(uint32_t t, string market);
	uint32_t GetMarketStatus(string t, string market, string format="%H:%M:%S");

	//市场状态变化触发事件
	bool OnChangeMarketStatus(string market, uint32_t status);
	bool OnChangeMarketStatus(uint32_t t, string market);
	bool OnChangeMarketStatus(string t, string market, string format="%H:%M:%S");

	//收盘状态判断
	bool  IsCloseMarket(uint32_t t, string market);
	bool  IsCloseMarket(uint32_t status);
	
    //下午结算时间
    bool  IsSettlement(uint32_t status);
	
    bool IsBeforeOpenMarket(uint32_t status);
	
	//停止接收数据
	bool IsStopRcvData(uint32_t t, string market);
	bool IsStopRcvData(uint32_t status);

	//touch flush data
	bool  IsStopTrade(uint32_t t, string market);
	bool  IsStopTrade(uint32_t status);

	//输出内存时间段信息
	inline void Print();

	private:
	
	inline uint32_t GetMarketStatusP(uint32_t second, string market);

	inline void Add(TimeSection ts);
	
	vector<TimeSection> m_tradetimes;
	map<string, uint32_t> m_now_status;	
};

//记录状态 并触发状态change


extern TradeTime g_tradetime;

#endif






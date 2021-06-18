/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file MdSessionI.h
///@description  行情数据存储操作  mongodb操作接口 文件操作接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef  __HQ_H__
#define  __HQ_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include <json/json.h>
#include <float.h>
#include <map>
#include <string>

#include "util.h"
//#include "MdLiveSession.h"
//#include "SessionM.h"

#include "../ctp/ThostFtdcMdSpiI.h"
#include "../ice/Common.h"

using namespace std;
using namespace CM; //public struct define

//#pragma pack(4) 

#define MAKE_TICK_KEY(tm, tk) (((int64_t)tm)*1000 + ((int64_t)tk))


#define  IsDValid(t) (fabs(t-DBL_MAX)>0.0001 && fabs(t)>0.0001)
#define IsEqDouble(t1, t2) (fabs(t1-t2)<0.000001)


enum KlineType{K1MINUTE, K3MINUTE, K5MINUTE, K10MINUTE, K15MINUTE, K30MINUTE, K60MINUTE, KDAY, KMONTH, KNONE, KDAYSNAP};

//单条合约 kline存储一天的(1000*50 + ticker(100 * 3600 *10) )*250
//kline一天20M以内   900M 
struct InstrumentData
{
	void Init()
	{
		memset(&snap, 0, sizeof(snap));
		//memset(&instrucment, 0, sizeof(instrucment));
	}
	//合约静态信息
	CThostFtdcDepthMarketDataField snap;//上游的快照数据
	list<Ticker>	TickerItems;		//一天有20w 必须淘汰制度  保留近20条就可以
	//合约的分k  插入的时候key有序尾部插入   kline保留n条（配置文件读取）
	list<Bar>	kline1_minute;		//一天有600条
	list<Bar>	kline3_minute;		//一天有200条
	list<Bar>	kline5_minute;		//一天有120条
	list<Bar>	kline10_minute;		//一天有60条
	list<Bar>	kline15_minute;		//一天40
	list<Bar>	kline30_minute;		//20
	list<Bar>	kline60_minute;		//10
    list<Bar>	kline_day;			//1
    list<Bar>	kline_month;		//1
	list<DayBar> day_snap;			//日快照
	CThostFtdcInstrumentField      instrucment;	//合约对应信息
};
//全局内存结构 存储kline等相关信息
extern map<string, InstrumentData*>  g_mapInstrmentData;	
typedef map<string, InstrumentData*> mapInstrumentDataType;

//获取一条合约 create=true不存在创建存在返回  create=false查询
InstrumentData* GetInstrumentData(string id, bool create=false);
//删除一条合约空间
void  DelInstrumentData(string id);
//获取一个ticker create=true创建 create=false查询
Ticker* GetTickerItem(InstrumentData* p, int64_t key, bool create=false);
//删除一个ticker
void DelTickerItem(InstrumentData* p, int64_t key);
//获取kline对应类型的列表
template<typename T>
list<T>*  GetKlineList(InstrumentData* p, int64_t key, KlineType type);
//获取一条kline create=true存在返回 不存在创建 create=false查询
template<typename T>
T* GetBar(InstrumentData* p, int64_t key, KlineType type, bool create=false);
//删除一条kline
template<typename T>
void DelBar(InstrumentData* p, int64_t key, KlineType type);
//获取上一根kline
template<typename T>
T* GetPrevBar(InstrumentData* p, int64_t key, KlineType type);


int  Del_Kline(InstrumentData* p,  uint32_t num);
int Del_Ticker(InstrumentData* p, uint32_t num);

//初始化一下内存 且交易日的时候可能需要
void InitInstrumentDatas(map<string, InstrumentData*>&  t);

//预处理数据
int   PreProcessData(InstrumentData* p, CThostFtdcDepthMarketDataField* psrc, bool updateMill=true);

//获取kline的key
uint64_t  GetKlineKey(struct tm* pt, KlineType kline_type);
//获取吓一个kline类型
KlineType  GetNextKlineType(KlineType ktype);
//累积k线
int  calculate_Kline(InstrumentData* p, char* _time, double price, double volum, double turnover, double openinterest);
//根据快照更新内存的k线
int  Snap_Update_Kline(InstrumentData* p, char* _time, double open=0, double close=0, double high=0, double low=0, double openinterest=0);
//更新快照
void Update_Snap(CThostFtdcDepthMarketDataField* psnap, CThostFtdcDepthMarketDataField* pnew);

DayBar*  calculate_DayBar(InstrumentData* p, CThostFtdcDepthMarketDataField* prev, CThostFtdcDepthMarketDataField* pnew, uint64_t Timestamp);

//累积ticker
Ticker*  calculate_Ticker(InstrumentData* p,  CThostFtdcDepthMarketDataField* prev, CThostFtdcDepthMarketDataField* pnew);

//查询ticker情况
string query_ticker(const string& insid);                          
//查询kline情况
string query_kline(const string& insid);                           
//查询snap情况
string query_snap(const string& insid);                                          


template<typename T>
list<T>*  GetKlineList(InstrumentData* p, int64_t Timestamp, KlineType type)
{
	list<T>* k =  NULL;
	switch(type)
	{
		case K1MINUTE:
			k = (list<T>*)&(p->kline1_minute); break;
		case K3MINUTE:
			k = (list<T>*)&(p->kline3_minute); break;
		case K5MINUTE:
			k = (list<T>*)&(p->kline5_minute); break;
		case K10MINUTE:
			k = (list<T>*)&(p->kline10_minute); break;
		case K15MINUTE:
			k = (list<T>*)&(p->kline15_minute); break;
		case K30MINUTE:
			k = (list<T>*)&(p->kline30_minute); break;
		case K60MINUTE:
			k = (list<T>*)&(p->kline60_minute); break;
		case KDAY:
			k = (list<T>*)&(p->kline_day); break;
		case KMONTH:
			k = (list<T>*)&(p->kline_month); break;
		case KDAYSNAP:
			k = (list<T>*)&(p->day_snap); break;
		default:
			break;
	}
	return k;
}

//获取一个kline空间
template<typename T>
T* GetBar(InstrumentData* p, int64_t Timestamp, KlineType type, bool create)
{
	list<T>* k =GetKlineList<T>(p, Timestamp, type);
	if(k == NULL)
	{
		LOG_ERROR("KLINE_TYPE:" << type << " error.");
		return NULL;
	}
	
	//list里面查找Timestamp 存在直接返回
	typeof(k->begin()) itr  = k->begin();
	for(; itr!=k->end(); itr++)
	{
		if(itr->Timestamp == Timestamp)
			return &(*itr);
	}
	
	if(create) //不存在创建
	{
		T ki;
		memset(&ki, 0, sizeof(ki));
		ki.Timestamp = Timestamp;
		k->push_back(ki);
		return &(*(k->rbegin()));
	}
	
	return NULL;

}

//删除一个ticker空间
template<typename T>
void DelBar(InstrumentData* p, int64_t Timestamp, KlineType type)
{
	list<T>* k =GetKlineList<T>(p, Timestamp, type);
	if(k == NULL)
	{
		LOG_INFO("KLINE_TYPE:" << type << " not exist.");
	}
	
	//list里面查找Timestamp
	typeof(k->begin()) itr  = k->begin();
	for(; itr!=k->end(); itr++)
	{
		if(itr->Timestamp == Timestamp)
			k->erase(itr);
	}
	LOG_INFO("DelBar Timestamp:" << Timestamp << " type:" << type);
}

template<typename T>
T* GetPrevBar(InstrumentData* p, int64_t Timestamp, KlineType type)
{
	list<T>* k =GetKlineList<T>(p, Timestamp, type);
	if(k == NULL)
	{
		LOG_ERROR("KLINE_TYPE:" << type << " error.");
		return NULL;
	}
	
	//list里面查找Timestamp 存在直接返回
	typeof(k->rbegin()) ritr  = k->rbegin();
	for(; ritr!=k->rend(); ritr++)
	{
		if(ritr->Timestamp == Timestamp) break;
	}
	if(++ritr != k->rend()) //上一根存在
		return &(*ritr);
	
	return NULL;
}




#endif





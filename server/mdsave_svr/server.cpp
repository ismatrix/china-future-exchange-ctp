// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <Ice/Ice.h>
#include <math.h>

#include "ctp/ThostFtdcMdSpiI.h"
#include "RollLog.h"
#include "conf.h"
#include "ctp/md_callback.h"
#include "mongoc_json.h"

#include "future/tradetime.h"
#include "future/holidays.h"
#include "future/timeseries.h"
#include "future/hq.h"
#include "future/file.h"
#include "util.h"


//加载那些初始化模块
#define		__INIT_CFILE__
#define		__INIT_FIFO__
#include "future/Init.h"

//1. kline有一条写一条 分k收线调用Write_Kline
//2. ticker一次100条。
//3. 在空闲时间遍历所有合约 写ticker和kline
//4. 怎么判断分k收线 上一根存在  当前还没创建 就推送完毕删除这根。 确保一次推送
//5. 空闲flush  只能做ticker的。 

CThostFtdcMdApi* pMd;
CThostFtdcMdSpiI* MdSpi;

using namespace std;

#define __CONFIG_FILE__   "./mdsave_config.ini"

int  Write_Kline(InstrumentData* p)
{
	KlineType ktype  = K1MINUTE;
	list<Bar>* pk = GetKlineList<Bar>(p,  0, ktype);
	if(pk!=NULL && pk->size() > 0)
	{
		//LOG_DEBUG("size:" << pk->size() << " num:" << num);
		g_file->WriteTxt(*pk, p->snap.TradingDay, p->snap.InstrumentID);
	}
	return 0;
}

//删除ticker
int Write_Ticker(InstrumentData* p, uint32_t num)
{
	if(p->TickerItems.size() > num)
	{
		//LOG_DEBUG("size:" << p->TickerItems.size() << " num:" << num);
		g_file->WriteTxt(p->TickerItems, p->snap.TradingDay, p->snap.InstrumentID);
	}
	return 0;
}

int flush_data()
{
	typeof(g_mapInstrmentData.begin()) itr = g_mapInstrmentData.begin();
	for(; itr!=g_mapInstrmentData.end(); itr++)
	{
		InstrumentData* p = itr->second;
		//list<Bar>* pk = GetKlineList<Bar>(p,  0, K1MINUTE);
		//if(pk != NULL &&  pk->size() > 0)
		//	g_file->WriteTxt(*pk, p->snap.TradingDay, p->snap.InstrumentID);
				
		if(p->TickerItems.size() > 0)
			g_file->WriteTxt(p->TickerItems, p->snap.TradingDay, p->snap.InstrumentID);
	}
	return 0;
}

int Write_DayBar(InstrumentData* p, uint32_t clear = false)
{
	//LOG_DEBUG("day_snap.size= " << p->day_snap.size());
	CMongodb& db = CMongodb::Instance();
	db.ChangeCollection("DAYBAR");

	list< ::CM::DayBar>::iterator itr = p->day_snap.begin();     
	for(; itr!=p->day_snap.end(); )
	{
		//json 不支持uint64  转为string
		Json::Value q, s;
		s["instrument"]         =       p->snap.InstrumentID; 
		s["tradingday"]         =       p->snap.TradingDay;
		s["timestamp"]          =       toa(itr->Timestamp);
		s["high"]               =       itr->High;               //最高价
		s["low"]                =       itr->Low;                //最低价
		s["open"]               =       itr->Open;               //开盘价
		s["close"]              =       itr->Close;              //收盘价
		s["average"]            =       itr->Average;			//日均价
		s["volume"]             =       itr->Volume;             //成交量
		s["turnover"]           =       itr->Turnover;           //成交额
		s["settlement"]         =       itr->Settlement;         //结算价
		s["openinterest"]		=       itr->OpenInterest;      //持仓量
		s["presettlement"]      =       itr->PreSettlement;      //上次结算价
		s["preclose"]           =       itr->PreClose;           //昨收盘
		s["preoopeninterest"]   =       itr->PreoOpenInterest;   //昨持仓量
		s["price"]				=		itr->Price;				//当前价
		s["upperlimit"]			=		itr->UpperLimit;		//涨停价
		s["lowerlimit"]			=		itr->LowerLimit;		//跌停价
		s["updatedate"]			=		stamp_to_isodate(time(NULL));

		q["instrument"] = p->snap.InstrumentID;
		q["tradingday"] = p->snap.TradingDay;

		Json::FastWriter writer;  
		std::string ss = writer.write(s);

		int ret = db.Update(ss, q.toStyledString(), true);
		if(ret < 0)
		{
			LOG_ERROR("db fail DayBar:" << db.GetLastErr() << " sql=" << ss);
		}
		LOG_INFO(ss);

		if(clear) p->day_snap.erase(itr++); else itr++;
	}

	return 0;
}


//kline主逻辑
int Run(const char* pin, uint32_t len, int wnum)
{
	CThostFtdcDepthMarketDataField* pnew = (CThostFtdcDepthMarketDataField*)pin;
	if(strlen(pnew->InstrumentID) == 0)
	{
		LOG_ERROR("InstrumentID:" << pnew->InstrumentID);
		return 0;
	}

	//加一个本地时间的限制 不接收行情时间
	uint32_t tExchange = MdSpi->GetExchangeTime(pnew->ExchangeID);
	if(g_tradetime.IsStopRcvData(tExchange, "SHFE"))
	{
		static int count_check = 0;
		if(count_check++ % 1000 == 0) LOG_DEBUG("Exchange time=" << tExchange  << " stop recv data.");
		return 0;
	}	

	map<string, InstrumentData*>& mapInstrmentData = g_mapInstrmentData;
	InstrumentData* prev = NULL;	//上一个快照
	//对比mapInstrmentData snap和pnew
	map<string, InstrumentData*>::iterator itr =  mapInstrmentData.find(pnew->InstrumentID);
	if(itr == mapInstrmentData.end() || strlen(itr->second->snap.InstrumentID) < 1)
	{
		//第一个快照更新
		LOG_INFO("new InstrumentID:" << pnew->InstrumentID);
		prev = GetInstrumentData(pnew->InstrumentID, true);	//创建一个合约位置
		PreProcessData(prev, pnew);
		memcpy(&prev->snap, pnew, sizeof(CThostFtdcDepthMarketDataField));
		return 0;
	}
	else
	{
		prev = (itr->second);
		if(PreProcessData(prev, pnew) < 0) return -1;
	}


	char timebuf[32] = {0};
	snprintf(timebuf, sizeof(timebuf), "%s %s", pnew->ActionDay, pnew->UpdateTime);
	struct tm _tm = strtotm(timebuf, "%Y%m%d %H:%M:%S"); //当前时间
	
	//写kline 写已经收线的那一根
	KlineType ktype  = K1MINUTE;
	while(ktype != KNONE)
	{
		uint64_t Timestamp = GetKlineKey(&_tm, ktype); //当前Timestamp
		Bar* k = GetBar<Bar>(prev, Timestamp,  ktype, false); //获取当前kline NULL 表示新的
		Bar* kprev = GetPrevBar<Bar>(prev, Timestamp, ktype);
		if(k==NULL && kprev!=NULL)
		{
			if(ktype == K1MINUTE) Write_Kline(prev); //写文件 同时删除这根kline
			//LOG_DEBUG("send_kline InstrumentID:" << prev->snap.InstrumentID << " type:" << ktype)
		}
		ktype = GetNextKlineType(ktype);
	}

	//计算kline
	if(pnew->Volume > prev->snap.Volume && IsDValid(pnew->LastPrice) && pnew->Turnover > prev->snap.Turnover)
	{
		calculate_Kline(prev, timebuf, pnew->LastPrice, pnew->Volume - prev->snap.Volume, pnew->Turnover-prev->snap.Turnover, pnew->OpenInterest);
	}

	//计算ticker
	if(IsDValid(pnew->LastPrice)  && 
			((pnew->Volume > prev->snap.Volume) || 
			(pnew->Volume==prev->snap.Volume &&  
			((IsDValid(pnew->BidPrice1) && !IsEqDouble(pnew->BidPrice1, prev->snap.BidPrice1)) || 
			pnew->BidVolume1    != prev->snap.BidVolume1 || 
			(IsDValid(pnew->AskPrice1) && !IsEqDouble(pnew->AskPrice1, prev->snap.AskPrice1))  || 
			pnew->AskVolume1    != prev->snap.AskVolume1 ))))	
	{
		calculate_Ticker(prev,  &prev->snap, pnew);
	}	

	//写日k 这里要实时推送 郑州的全天推送
	if(IsDValid(pnew->ClosePrice) && pnew->Volume >= 0 && IsDValid(pnew->SettlementPrice))
	{
		uint64_t Timestamp = GetKlineKey(&_tm, KDAY);
		DayBar* k = GetBar<DayBar>(prev, Timestamp, KDAYSNAP, false); //查询到他
		calculate_DayBar(prev, &prev->snap, pnew, Timestamp);
		if(!(string(prev->instrucment.ExchangeID) == "CZCE"))
		{
			DayBar* kd = calculate_DayBar(prev, &prev->snap, pnew, Timestamp);
			if(k == NULL && kd != NULL)
			{
				LOG_DEBUG("exchange=" << prev->instrucment.ExchangeID);
				Write_DayBar(prev);
			}
		}
	}

	Update_Snap(&prev->snap, pnew);

	Write_Ticker(prev, wnum); //有一个写一个

	//日内如果kline超过限制 也要删除 以控制内存
	Del_Kline(prev, 500);

	//日k写数据库
	return 0;
}


int main(int argc, char* argv[])
{
	int ret = InitAll(__CONFIG_FILE__);
	if(ret < 0) 
	{
		printf("init error \n");
		exit(-1);
	}
	
	set<string> idlist;
	ret = QueryMongoInstrument(g_conf, idlist);
	if(ret < 0 || idlist.size() < 10)
	{
		    return -1;
	}

	g_tradetime.Init(CMongodb::Instance());
	g_holidays.Init(CMongodb::Instance());
    g_timeserise.Init(CMongodb::Instance());

	string account, pwd, brokerid, ctpurl;
	g_conf.get<string>("CTP", "account", account, "");
	g_conf.get<string>("CTP", "pwd", pwd, "");
	g_conf.get<string>("CTP", "brokerid", brokerid, "");
	g_conf.get<string>("CTP", "ctpurl", ctpurl, "");

	pMd = CThostFtdcMdApi::CreateFtdcMdApi();
	MdSpi = new CThostFtdcMdSpiI(pMd, brokerid, account, pwd, idlist, &md_fifo_push);
	pMd->RegisterSpi(MdSpi);
	pMd->RegisterFront((char*)ctpurl.c_str());
	pMd->Init();
	
	CMongodb& db = CMongodb::Instance();
	db.ChangeCollection("DAYBAR");

	if(g_debug)
	{
		//跑测试数据
		for(int i=1; i<argc; i++)
		{
			LOG_INFO("file " << argv[i] << " is runnning.")
				fstream in;
			in.open(argv[i], ios::binary|ios::in);
			CThostFtdcDepthMarketDataField tmp;
			while(true)
			{
				in.read((char*)&tmp, sizeof(CThostFtdcDepthMarketDataField));
				//printf("insid:%s\n", tmp.InstrumentID);
				Run((const char*)&tmp, sizeof(CThostFtdcDepthMarketDataField), 1000);
				usleep(10);
			}
		}
		return 0;
	}


	LOG_DEBUG("ProcessDataThread start.");
	string tradingday = "";
	while(true)
	{
		CThostFtdcDepthMarketDataField tmp;
		if(!g_holidays.IsTrading(time(NULL)))
		{
			while(md_fifo_pop((char*)&tmp, sizeof(CThostFtdcDepthMarketDataField))==0);
			
			static int count_check = 0;
			if(count_check++ % 1000 == 0) LOG_INFO("holiday stop recv data!");
		
			sleep(1);
			continue;
		}

		while(md_fifo_pop((char*)&tmp, sizeof(CThostFtdcDepthMarketDataField)) == 0)
		{
			//MdSpi->LOG_MD_INFO(&tmp);
			//Run((const char*)&tmp, sizeof(CThostFtdcDepthMarketDataField), 100);
			Run((const char*)&tmp, sizeof(CThostFtdcDepthMarketDataField), 10);
		}

		//根据登录交易所时间和当前系统时间   休市和收盘触发
		uint32_t tExchange = MdSpi->GetExchangeTime("CFFEX");
		uint32_t status = g_tradetime.GetMarketStatus(tExchange, "SHFE");
		bool change = g_tradetime.OnChangeMarketStatus("SHFE", status);
		
		if(g_tradetime.IsCloseMarket(status) && change)	
		{
			typeof(g_mapInstrmentData.begin()) itr = g_mapInstrmentData.begin();
			for(; itr!=g_mapInstrmentData.end(); itr++)
			{
				Write_DayBar(itr->second, true); //清空
				Write_Kline(itr->second);		 //清空
			}
			g_mapInstrmentData.clear();
			LOG_INFO("market change close market flush daybar kline.");
		}

		//kline刷
		if(g_tradetime.IsStopTrade(status) && change)
		{
			typeof(g_mapInstrmentData.begin()) itr = g_mapInstrmentData.begin();
			for(; itr!=g_mapInstrmentData.end(); itr++)
			{
				Write_Kline(itr->second);		 //清空
			}
			LOG_INFO("market status  change flush kline.");
		}

		if(tradingday != MdSpi->GetTradingDay()) 
		{
			set<string> idlist;
			QueryMongoInstrument(g_conf, idlist);
			if(idlist.size() < 100)
			{
				LOG_ERROR("QueryMongoInstrument size=" << idlist.size() << " not enough.");
				continue;
			}
		
			//内存数据清理
			typeof(g_mapInstrmentData.begin()) itr = g_mapInstrmentData.begin();
			for(; itr!=g_mapInstrmentData.end(); itr++)
			{
				itr->second->Init();
			}

			//重新订阅
			MdSpi->ChangeSubcribe(idlist);
			tradingday = MdSpi->GetTradingDay();
		}


		static int num = 0;
		if(num++ % 30000 == 0)
		{
			LOG_DEBUG("No data. free to flush data.");
			flush_data();
			typeof(g_mapInstrmentData.begin()) itr = g_mapInstrmentData.begin();
			for(; itr!=g_mapInstrmentData.end(); itr++)
			{
				InstrumentData* p = itr->second;
				if(p->TickerItems.size() > 0)
					g_file->WriteTxt(p->TickerItems, p->snap.TradingDay, p->snap.InstrumentID);
			}
		}
		
		msleep(10);
	}

	pMd->Release();
	pMd->Join();

	return 0;
}






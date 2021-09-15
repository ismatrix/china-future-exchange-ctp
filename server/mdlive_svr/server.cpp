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
#include <MdLiveSessionI.h>
#include "ctp/ThostFtdcMdSpiI.h"
#include "RollLog.h"
#include "conf.h"
#include "ice/Common.h"
#include "ctp/md_callback.h"
#include "mongoc_json.h"
#include "future/hq.h"
#include "singleton.h"
#include "future/snap.h"
#include "future/tradetime.h"
#include "future/holidays.h"
#include "future/timeseries.h"
//初始化模块
#define  __INIT_FIFO__
#include "future/Init.h"

using namespace std;
using namespace MdLive;

//1. 分k切换就推送, 没分过三秒检查全部分k没推送的进行推送
//2. 除郑州日k结算就推送，郑州的收盘后15:30统一推送
//3. tick生成一个推送一个
//4. 交易日切换清理数据，查询数据库重新订阅合约
//5. 收到行情就更新 写db日k的独立线程缓存
//6. 写db日k的独立缓存间隔批量更新db 现在是50条一批更新4批左右每次。

#define __CONFIG_FILE__   "./mdlive_config.ini"

CThostFtdcMdApi* pMd;
CThostFtdcMdSpiI* MdSpi;

//记录已经推送过的合约对应的分k
map<uint64_t, long> instrument_key_map;	//合约--key映射表   推送过的kline的记录
//记录已经推送过的日k
map<string, long> instrument_daybar_map;

void update_daybar(CThostFtdcDepthMarketDataField* pnew, uint64_t Timestamp, string productid)
{
	DayBar day;
	day.Timestamp           = Timestamp;
	day.High                = pnew->HighestPrice;
	day.Low                 = pnew->LowestPrice;
	day.Open                = pnew->OpenPrice;
	day.Close               = pnew->ClosePrice;
	day.Average             = pnew->AveragePrice;
	day.Volume              = pnew->Volume;
	day.Turnover            = pnew->Turnover;
	day.Settlement          = pnew->SettlementPrice;
	day.OpenInterest        = pnew->OpenInterest;
	day.PreSettlement       = pnew->PreSettlementPrice;
	day.PreClose            = pnew->PreClosePrice;
	day.PreoOpenInterest    = pnew->PreOpenInterest;
	day.Price               = pnew->LastPrice;
	day.UpperLimit          = pnew->UpperLimitPrice;
	day.LowerLimit          = pnew->LowerLimitPrice;

	day.Close = day.Price;

	Singleton<CDayBar>::Instance().set(pnew->InstrumentID, pnew->TradingDay, productid, day);

	//push给mdindex
//	SessionM::Instance().send_day_snap_real(pnew->TradingDay, pnew->InstrumentID, day);
}

//push 深度行情数据到下游
void push_DepthMarketData(CThostFtdcDepthMarketDataField* pnew)
{
	static CM::DepthMarketData mdata;
	mdata.TradingDay			= pnew->TradingDay			;
	mdata.InstrumentID			= pnew->InstrumentID        ;
	mdata.ExchangeID			= pnew->ExchangeID          ;
	mdata.ExchangeInstID		= pnew->ExchangeInstID      ;
	mdata.LastPrice				= pnew->LastPrice           ;
	mdata.PreSettlementPrice	= pnew->PreSettlementPrice  ;
	mdata.PreClosePrice			= pnew->PreClosePrice       ;
	mdata.PreOpenInterest		= pnew->PreOpenInterest     ;
	mdata.OpenPrice				= pnew->OpenPrice           ;
	mdata.HighestPrice			= pnew->HighestPrice        ;
	mdata.LowestPrice           = pnew->LowestPrice         ;
	mdata.Volume                = pnew->Volume              ;
	mdata.Turnover              = pnew->Turnover            ;
	mdata.OpenInterest          = pnew->OpenInterest        ;
	mdata.ClosePrice            = pnew->ClosePrice          ;
	mdata.SettlementPrice       = pnew->SettlementPrice     ;
	mdata.UpperLimitPrice       = pnew->UpperLimitPrice     ;
	mdata.LowerLimitPrice       = pnew->LowerLimitPrice     ;
	mdata.UpdateTime            = pnew->UpdateTime          ;
	mdata.UpdateMillisec        = pnew->UpdateMillisec      ;
	mdata.BidPrice1             = pnew->BidPrice1           ;
	mdata.BidVolume1            = pnew->BidVolume1          ;
	mdata.AskPrice1             = pnew->AskPrice1           ;
	mdata.AskVolume1            = pnew->AskVolume1          ;
	mdata.AveragePrice          = pnew->AveragePrice        ;
	mdata.ActionDay             = pnew->ActionDay           ;
	SessionM::Instance().send_depth_market_data(mdata);
}

string  get_productid(string instrument)
{
    string product;
    uint32_t size = instrument.size();
    for(uint32_t i=0; i<size; i++)
    {
        if(instrument[i] >='0'  && instrument[i] <= '9')
        {
            return instrument.substr(0, i);
        }
    }

    int pos = instrument.find("IDX");
    if(pos != (int)string::npos)
    {
        return instrument.substr(0, pos);
    }
    return instrument;
}

Bar get_1minute_bar(string instrument, InstrumentData* p, int64_t sendtime)
{
    Bar* k = GetBar<Bar>(p, sendtime,  K1MINUTE, false);
    if(k != NULL)
    {
         return *k;
    }
    else
    {

        double close = 0, io = 0;
        Bar* b = GetPrevBar<Bar>(p, sendtime, K1MINUTE);
        if(b == NULL)
        {
            //close = p->snap.PreClosePrice;
            close = p->snap.LastPrice;
            if(close < 0.0001) close = p->snap.PreClosePrice;
            io = p->snap.OpenInterest;
        }
        else
        {
            close = b->Close;
            io    = b->OpenInterest;
        }
        Bar t;
        t.Timestamp     = sendtime;
        t.High          = close;
        t.Low           = close;
        t.Open          = close;
        t.Close         = close;
        t.Volume        = 0;
        t.Turnover      = 0;
        t.OpenInterest  = io;
        return t;
    }
}

//每分钟都来检查 有一个收盘推送最后一笔的逻辑
//要取上个周期到现在
//下午13点来遍历的时候 会推送上午没有推送的 晚上没推送的等到第二天开盘来遍历推送
//kline 改为只在开盘时间推送  这个明显不行
void process_period_kline(string tradingday, string instrument, InstrumentData* p, long Timestamp)
{
    //其他周期kline合成并推送
    set<uint16_t> s = SessionM::Instance().get__msubcribe(instrument);
    for(set<uint16_t>::iterator itr = s.begin(); itr != s.end(); itr++)
    {
        long sendtime = 0;
        uint16_t period = *itr;
        string product = p->instrucment.ProductID; //get_productid(instrument);
        time_t now = g_timeserise.GetSeriseKey((char*)product.c_str(), period, strtostamp(tradingday.c_str(), "%Y%m%d"), Timestamp/1000, sendtime) *1000;
        if(sendtime < 0)
        {
            LOG_ERROR("GetSeriseKey -1. instrument=" << instrument << " tradingday=" << tradingday << " time=" << stamptostr((Timestamp/1000)));
        }
        //LOG_INFO("test4 instrument" << instrument  << " period=" << period << " sendtime=" << sendtime << " t="<< Timestamp << " product=" << product);
        sendtime = sendtime * 1000;

        int64_t key = stoint64(instrument.c_str())  + period;
		map<uint64_t, long>::iterator it = instrument_key_map.find(key);
        if(it == instrument_key_map.end())
        {
            instrument_key_map[key] =  sendtime; //第一次进来就赋值了上个需要推送的数据21:03
            continue;
        }

        if(sendtime <= 0 || now == 0 || sendtime <= it->second) continue;

        long bt = sendtime;
        now = sendtime + period*60*1000;
        //非周期的倍数进不来？？？
        Bar k = get_1minute_bar(instrument, p, sendtime);
        sendtime += 60*1000;
        while(sendtime < now)
        {
            Bar* pk = GetBar<Bar>(p, sendtime,  K1MINUTE, false);
            if(pk == NULL)
            {
                sendtime += 60*1000;
                continue;
            }
            if(pk->High > k.High) k.High = pk->High;
            if(pk->Low > 0.1 && (pk->Low < k.Low || k.Low < 0.1)) k.Low = pk->Low;
            if(pk->Close > 0.1) k.Close = pk->Close;
            k.Volume        += pk->Volume;
            k.Turnover      += pk->Turnover;
            k.OpenInterest  += pk->OpenInterest;
            //LOG_DEBUG("test instrument=" << instrument << "  time=" << t-period*60*1000 << " addkline=" << sendtime << " period=" << period);
            sendtime += 60*1000;
        }
        LOG_INFO("test3 instrument=" << instrument << " push time=" << stamptostr(bt/1000) << " period=" << period << " subsize=" << s.size()  << " it->second=" << stamptostr(it->second/1000) << " key=" << key << " time=" << stamptostr(bt/1000) << "--->" << stamptostr(now/1000));

		SessionM::Instance().send_kline(tradingday,   instrument,  k,  period);
		instrument_key_map[key] =  bt;
    }
}


//检查上根推送了没有 然后推送那些美腿送的 行情里面有昨收 如果没有第一根就拿昨收生成
//这里进行1mk的补全和其他周期k的推送 这里是个麻烦事情 其他周期的计算
//订阅行情的下游都会被补全
void check_and_push_kline(long Timestamp, string tradingday)
{
    //补充1mk的推送
	map<string, InstrumentData*>::iterator itr =  g_mapInstrmentData.begin();
	for(; itr!=g_mapInstrmentData.end(); itr++)
	{
        //sendtime就是当前要推送的
        //没开盘不退送 目的把收盘的这根线放到下次开盘推送   收盘了必须推送。
        string product = itr->second->instrucment.ProductID; //get_productid(itr->first);
        if(!g_timeserise.isTrading((char*)product.c_str(), Timestamp/1000, 120))
            continue;

        process_period_kline(tradingday, itr->first, itr->second, Timestamp);


        //需要计算的是上一个，   上一个可能不存在就不推送  可能已经推送过了。
        time_t sendtime = 0;
        long now = g_timeserise.GetSeriseKey((char*)product.c_str(), 1, strtostamp(tradingday.c_str(), "%Y%m%d"), Timestamp/1000, sendtime);
        sendtime = sendtime * 1000;

        //不存在就设置
        int64_t key = stoint64(itr->first.c_str())  + 1;
		map<uint64_t, long>::iterator it = instrument_key_map.find(key);
		if(it == instrument_key_map.end())
        {
            instrument_key_map[key] =  Timestamp-60;
            continue;
        }

        if(sendtime <= 0 || now == 0 || sendtime <= it->second) continue;

        Bar k = get_1minute_bar(itr->first, itr->second, sendtime);
	    SessionM::Instance().send_kline(tradingday,  itr->first, k, 1);
	    instrument_key_map[key] =  sendtime;

        //LOG_INFO("test2 instrument=" << itr->first << " push time=" << stamptostr(sendtime/1000) << " period=1"  << " key=" << key << " time=" << stamptostr(Timestamp/1000));
    }
}

void check_and_push_daybar( string tradingday)
{
	map<string, InstrumentData*>::iterator itr =  g_mapInstrmentData.begin();
	for(; itr!=g_mapInstrmentData.end(); itr++)
	{
		map<string, long>::iterator it = instrument_daybar_map.find(itr->first);
		if(it == instrument_daybar_map.end())
		{
			struct tm tmp;
			time_t t = time(NULL);
			localtime_r(&t, &tmp);
			uint64_t Timestamp = GetKlineKey(&tmp, KDAY);
			DayBar* k = GetBar<DayBar>(itr->second, Timestamp, KDAYSNAP, false); //查询到他
			if(k != NULL)
			{
				SessionM::Instance().send_day_snap(tradingday, itr->first, *k);
				instrument_daybar_map[itr->first] =  Timestamp;
				LOG_DEBUG("DayBar InstrumentID="<<itr->first
						<< " TradingDay=" << tradingday
						<< " Timestamp=" << k->Timestamp
						<< " Close=" <<k->Close
						<< " Settlement=" << k->Settlement
						<< " voulume=" << k->Volume);
			}
		}
	}
}

//kline主逻辑
int Run(const char* pin, uint32_t len)
{
	CThostFtdcDepthMarketDataField* pnew = (CThostFtdcDepthMarketDataField*)pin;
	// LOG_INFO("-------------->" << pnew->InstrumentID)
    if(strlen(pnew->InstrumentID) == 0)
	{
		LOG_ERROR("InstrumentID:" << pnew->InstrumentID);
		return 0;
	}


    //加一个本地时间的限制 不接收行情时间
	uint32_t tExchange = MdSpi->GetExchangeTime(pnew->ExchangeID);
	if(g_tradetime.IsStopRcvData(tExchange, "SHFE"))
	{
		//LOG_DEBUG("Exchange time=" << tExchange  << " closed.");
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
		PreProcessData(prev, pnew, true);
		memcpy(&prev->snap, pnew, sizeof(CThostFtdcDepthMarketDataField));
		return 0;
	}
	else
	{
		prev = (itr->second);
		PreProcessData(prev, pnew) ;
	}

    LOG_DEBUG(pnew->TradingDay << " " << pnew->ActionDay << " " << pnew->InstrumentID << " " << prev->instrucment.ExchangeID << " " << pnew->UpdateMillisec);

    //推送原始数据
	push_DepthMarketData(pnew);

	char timebuf[32] = {0};
	snprintf(timebuf, sizeof(timebuf), "%s %s", pnew->ActionDay, pnew->UpdateTime);
	struct tm _tm = strtotm(timebuf, "%Y%m%d %H:%M:%S"); //当前时间

	//换k的时候推送单一合约
	int64_t Timestamp = GetKlineKey(&_tm, K1MINUTE); //当前Timestamp
	//int64_t tt = Timestamp;
    Timestamp = g_timeserise.modify_open_close_timestamp(prev->instrucment.ProductID, Timestamp/1000) * 1000l;
    //LOG_INFO("Timestamp=" << Timestamp << " t=" << tt);

    Bar* k = GetBar<Bar>(prev, Timestamp,  K1MINUTE, false); //获取当前kline NULL 表示新的
	Bar* kprev = GetPrevBar<Bar>(prev, Timestamp,  K1MINUTE);
	if(k==NULL && kprev!=NULL)
	{
        int64_t key = stoint64(pnew->InstrumentID)  + 1;
		map<uint64_t, long>::iterator itr = instrument_key_map.find(key);
		if((itr != instrument_key_map.end() && itr->second < kprev->Timestamp) || itr==instrument_key_map.end())
		{
			SessionM::Instance().send_kline(pnew->TradingDay, pnew->InstrumentID, *kprev, 1);
			instrument_key_map[key] =  kprev->Timestamp;
            //LOG_INFO("test1 instrument=" << pnew->InstrumentID << " push time=" << stamptostr(kprev->Timestamp/1000) << " period=1"  << " key=" << key << " time=" << stamptostr(Timestamp/1000));
		}
	}

	//检查上一分钟所有的kline推送 每个第三秒检查
	if(pnew->UpdateTime[7] == '3' && pnew->UpdateTime[6] == '0')
	{
		check_and_push_kline(Timestamp, pnew->TradingDay);
	}

	//计算kline
	if(pnew->Volume > prev->snap.Volume && IsDValid(pnew->LastPrice) && pnew->Turnover > prev->snap.Turnover)
	{
		calculate_Kline(prev, timebuf, pnew->LastPrice, pnew->Volume - prev->snap.Volume, pnew->Turnover-prev->snap.Turnover, pnew->OpenInterest);

		if(string(pnew->InstrumentID).find("0001") != string::npos)
		{
			LOG_DEBUG("prev->snap.Turnover=" << prev->snap.Turnover << " pnew->Turnover=" << pnew->Turnover);
		}
	}

	//计算ticker
	if(IsDValid(pnew->LastPrice) && (pnew->Volume > prev->snap.Volume ||
			(pnew->Volume == prev->snap.Volume && ((IsDValid(pnew->BidPrice1) && !IsEqDouble(pnew->BidPrice1, prev->snap.BidPrice1)) ||
			pnew->BidVolume1	!= prev->snap.BidVolume1 ||
			(IsDValid(pnew->AskPrice1) && !IsEqDouble(pnew->AskPrice1, prev->snap.AskPrice1))  ||
			pnew->AskVolume1	!= prev->snap.AskVolume1))))
	{
		Ticker* k = calculate_Ticker(prev,  &prev->snap, pnew);
		SessionM::Instance().send_ticker(pnew->TradingDay, pnew->InstrumentID, *k);
	}


	//这里是实时日k的处理 更新db线程会定时更新
	if((pnew->Volume >= prev->snap.Volume) && IsDValid(pnew->LastPrice))
	{
		uint64_t Timestampday = GetKlineKey(&_tm, KDAY);
		update_daybar(pnew, Timestampday, prev->instrucment.ProductID);
	}

	//推送郑州的有问题 最后15.15进行推送
	if(IsDValid(pnew->ClosePrice) && pnew->Volume >= 0 && IsDValid(pnew->SettlementPrice))
	{
		uint64_t Timestamp = GetKlineKey(&_tm, KDAY);
		DayBar* k = GetBar<DayBar>(prev, Timestamp, KDAYSNAP, false); //查询到他
		DayBar* kd = calculate_DayBar(prev, &prev->snap, pnew, Timestamp);
		if(!(string(prev->instrucment.ExchangeID)=="CZCE") && k==NULL && kd!=NULL)
		{
			SessionM::Instance().send_day_snap(pnew->TradingDay, pnew->InstrumentID, *kd);
			LOG_DEBUG("DayBar InstrumentID="<<pnew->InstrumentID<<" TradingDay="<<pnew->TradingDay<<" Timestamp="<<kd->Timestamp<<" Close="<<kd->Close<<" Settlement="<<kd->Settlement << " voulume=" << kd->Volume);
			instrument_daybar_map[pnew->InstrumentID] = Timestamp;
		}
	}

	Update_Snap(&prev->snap, pnew);

	Del_Ticker(prev, 200);
	//一天已清理 方便补齐合成多周期的
    //Del_Kline(prev, 500);

	return 0;
}



class ProcessDataThread : public IceUtil::Thread
{
	public:
		virtual void run()
		{
			if(g_debug)
			{
				cout << "debug open\n";
                SessionM::Instance().add__msubcribe("ag1712", 5);
                SessionM::Instance().add__msubcribe("rb1709", 5);
                SessionM::Instance().add__msubcribe("T1709", 5);

                SessionM::Instance().add__msubcribe("ag1712", 15);
                SessionM::Instance().add__msubcribe("rb1709", 15);
                SessionM::Instance().add__msubcribe("T1709",  15);


                fstream in;
				in.open("./md.dat", ios::binary|ios::in);
				CThostFtdcDepthMarketDataField tmp;
				while(!in.eof())
				{
					in.read((char*)&tmp, sizeof(CThostFtdcDepthMarketDataField));
					//printf("insid:%s\n", tmp.InstrumentID);
					//MdSpi->LOG_MD_INFO(&tmp);
                    Run((const char*)&tmp, sizeof(CThostFtdcDepthMarketDataField));
				}
				exit(-1);
			}


			string tradingday = "0";

			LOG_DEBUG("ProcessDataThread start.");
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
					Run((const char*)&tmp, sizeof(CThostFtdcDepthMarketDataField));
				}

				//日k推送 收盘15.30进行推送
				uint32_t tExchange = MdSpi->GetExchangeTime("CFFEX");
				uint32_t status = g_tradetime.GetMarketStatus(tExchange, "SHFE");
				bool change = g_tradetime.OnChangeMarketStatus( "SHFE", status);

                //下午结算时间15:16---15:30  推送最后的15：15的行情
				if(g_tradetime.IsSettlement(status) && change)
				{
                    struct tm tmp = {0};
                    time_t tt = time(NULL);
                    localtime_r(&tt, &tmp);
	                int64_t Timestamp = GetKlineKey(&tmp, K1MINUTE); //当前Timestamp
	                check_and_push_kline(Timestamp, tradingday); //15:16:00
                    LOG_INFO("settlement push Timestamp=" << Timestamp << " tradingday=" << tradingday);
                }

                //15:30
				if(g_tradetime.IsCloseMarket(status) && change)
				{
					check_and_push_daybar(tradingday);
					g_mapInstrmentData.clear();
					LOG_INFO("market status change to 10 close market..");
				}

				if(tradingday < MdSpi->GetTradingDay())
				{
					set<string> idlist;
					QueryMongoInstrument(g_conf, idlist); //这个里面会更新合约信息的

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
						LOG_DEBUG("instrument=" << itr->first << " volumemultipl=" << (itr->second->instrucment.VolumeMultiple) << " exchangeid="<<(itr->second->instrucment.ExchangeID));
					}

					instrument_key_map.clear();
					instrument_daybar_map.clear();
                    g_timeserise.clear();

					Singleton<CDayBar>::Instance().clear();

					LOG_INFO("old tradingday=" << tradingday << " new tradingday="<< MdSpi->GetTradingDay() << " instrument size=" << g_mapInstrmentData.size());
					//重新订阅
					MdSpi->ChangeSubcribe(idlist);
					tradingday = MdSpi->GetTradingDay();
				}

				static int num = 0;
				if(num++ % 30000 == 0)  LOG_DEBUG("No data.");
				msleep(10);
			}
		}
};


class MdServer : public Ice::Application
{
	public:
		virtual int run(int, char*[]);
};



int main(int argc, char* argv[])
{

	int ret = ::InitAll(__CONFIG_FILE__);
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

	//写mongo模块
	Singleton<CDayBar>::Instance().init(g_conf);

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

#ifdef ICE_STATIC_LIBS
	Ice::registerIceSSL();
#endif

	string ip, port;
	g_conf.get<string>("mdserver", "ip", ip, "127.0.0.1");
	g_conf.get<string>("mdserver", "port", port, "10001");

	Ice::InitializationData initData;
	stringstream ss;
	ss << "tcp -h " << ip << " -p " << port;
	initData.properties = Ice::createProperties(argc, argv);
	initData.properties->setProperty("MDLIVE.service.Endpoints", ss.str().c_str());
	initData.properties->setProperty("Ice.Warn.Connections", "1");
	initData.properties->setProperty("Ice.Trace.Network", "0");
	initData.properties->setProperty("Ice.Trace.Protocol", "0");

	initData.properties->setProperty("Ice.ThreadPool.Server.Size", "5");
	initData.properties->setProperty("Ice.ThreadPool.Server.SizeMax", "100");

	MdServer app;
	return app.main(argc, argv, initData);

	return 0;
}

int MdServer::run(int argc, char* argv[])
{
	try
	{
		//创建对象适配器
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("MDLIVE.service");

		MdSessionIPtr sender = new MdSessionI();

		adapter->add(sender, communicator()->stringToIdentity("sender"));

		IceUtil::ThreadPtr t = new ProcessDataThread;
		t->start();

		adapter->activate();

		communicator()->waitForShutdown();
		t->getThreadControl().join();
	}
	catch(::Ice::Exception& e)
	{
		stringstream ss;
		ss << e;
		LOG_ERROR(ss.str());
	}
	catch(...)
	{
		LOG_ERROR("Exception .");
	}

	communicator()->shutdown();
	pMd->Release();
	//pMd->Join();


	return EXIT_SUCCESS;

}

// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include "MdSessionCallBackI.h"
#include "index_calculate.h"
#include "singleton.h"

::Ice::Int MdSessionCallBackI::onTick(const ::std::string& TradingDay, const ::std::string& insid, const ::CM::Ticker& ticker, const ::Ice::Current& current)
{
	
	//LOG_DEBUG(insid << " " << ticker.Timestamp <<" " << ticker.Price <<" " << ticker.Volume <<" " << ticker.Turnover <<" " << ticker.OpenInterest <<" " << ticker.TotalVolume <<" " << ticker.TotalTurnover <<" " << ticker.BidPrice1 <<" " << ticker.AskPrice1 <<" " << ticker.BidVolume1 <<" " << ticker.AskVolume1);
	
	Singleton<BaseCalculate>::Instance().set_hqtime(TradingDay, insid, ticker);	
	
	return 0;  
}


::Ice::Int MdSessionCallBackI::onBar(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::Bar& bar,  const int period, const ::Ice::Current& current)
{
    if(period == 1)
	    Singleton<BaseCalculate>::Instance().set_key_bar(TradingDay, insid, bar);	
    else
        LOG_ERROR("period=" << period);
	//LOG_DEBUG(insid << " " << bar.Timestamp << " " << bar.High << " " << bar.Low << " " << bar.Open << " " << bar.Close << " " << bar.Volume << " " << bar.Turnover);
	
	return 0;  
}


::Ice::Int MdSessionCallBackI::onDay(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::DayBar& day, const ::Ice::Current& current)
{
	LOG_INFO("recv day_kline, TradingDay:" << TradingDay << " InstrumentID:" << insid);
	
	Singleton<BaseCalculate>::Instance().set_key_daybar(TradingDay, insid, day);	
	
	return 0;  
}

void MdSessionCallBackI::onDepthMarketData(const ::CM::DepthMarketData&, const ::Ice::Current&  current)
{

}

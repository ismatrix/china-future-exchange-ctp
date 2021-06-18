// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <signal.h>

#include <string>
#include "RollLog.h"
#include "MdLiveSession.h"
#include "util.h"
#define __LOG__ __FILE__
using namespace CM;
using namespace MdLive;
using namespace std;

class MdSessionCallBackI : public MdSessionCallBack
{
public:
	//用户订阅是否ok
	virtual ::Ice::Int onTick(const ::std::string& TradingDay, const ::std::string& insid, const ::CM::Ticker& ticker, const ::Ice::Current& = ::Ice::Current());
	virtual ::Ice::Int onBar(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::Bar& bar, const int period, const ::Ice::Current& = ::Ice::Current());
	virtual ::Ice::Int onDay(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::DayBar& day, const ::Ice::Current& = ::Ice::Current());
    virtual void onDepthMarketData(const ::CM::DepthMarketData&, const ::Ice::Current& = ::Ice::Current());
};

::Ice::Int MdSessionCallBackI::onTick(const ::std::string& TradingDay, const ::std::string& insid, const ::CM::Ticker& ticker, const ::Ice::Current& current)
{
	//LOG_DEBUG("recv TickerItem, TradingDay:" << TradingDay << " InstrumentID:" << insid);
	//LOG_DEBUG("KEY:" << current.id.name);

	LOG_DEBUG(insid << " " << ticker.Timestamp <<" " << ticker.Price <<" " << ticker.Volume <<" " << ticker.Turnover <<" " << ticker.OpenInterest <<" " << ticker.TotalVolume <<" " << ticker.TotalTurnover <<" " << ticker.BidPrice1 <<" " << ticker.AskPrice1 <<" " << ticker.BidVolume1 <<" " << ticker.AskVolume1);

	return 0;  
}
::Ice::Int MdSessionCallBackI::onBar(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::Bar& bar, const int period, const ::Ice::Current& current)
{
	//LOG_DEBUG("recv kline, TradingDay:" << TradingDay << " InstrumentID:" << insid);
	LOG_DEBUG(insid << ":" << period << " " << bar.Timestamp << " " << bar.High << " " << bar.Low << " " << bar.Open << " " << bar.Close << " " << bar.Volume << " " << bar.Turnover);
	return 0;  
}
::Ice::Int MdSessionCallBackI::onDay(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::DayBar& day, const ::Ice::Current& current)
{
	
    //LOG_DEBUG("recv day_kline, TradingDay:" << TradingDay << " InstrumentID:" << insid);
	//LOG_DEBUG("KEY:" << current.id.name);
	//cout << "timestamp:" << day.Timestamp << endl;
	
	return 0;  
}

void MdSessionCallBackI::onDepthMarketData(const ::CM::DepthMarketData& mdata, const ::Ice::Current& current)
{
     LOG_DEBUG("pDepthMarketData:" << mdata.InstrumentID << " "
             << mdata.OpenPrice << " " 
             << mdata.HighestPrice << " "
             << mdata.LowestPrice << " "
             << mdata.ClosePrice << " "
             << mdata.LastPrice);
	return;
    LOG_DEBUG("pDepthMarketData\n\t" <<
			"TradingDay        : " << mdata.TradingDay          << "\n\t" <<
			"InstrumentID      : " << mdata.InstrumentID        << "\n\t" <<
			"ExchangeID        : " << mdata.ExchangeID          << "\n\t" <<
			"ExchangeInstID    : " << mdata.ExchangeInstID      << "\n\t" <<
			"LastPrice         : " << mdata.LastPrice           << "\n\t" <<
			"PreSettlementPrice: " << mdata.PreSettlementPrice  << "\n\t" <<
			"PreClosePrice     : " << mdata.PreClosePrice       << "\n\t" <<
			"PreOpenInterest   : " << mdata.PreOpenInterest     << "\n\t" <<
			"OpenPrice         : " << mdata.OpenPrice           << "\n\t" <<
			"HighestPrice      : " << mdata.HighestPrice        << "\n\t" <<
			"LowestPrice       : " << mdata.LowestPrice         << "\n\t" <<
			"Volume            : " << mdata.Volume              << "\n\t" <<
			"Turnover          : " << mdata.Turnover            << "\n\t" <<
			"OpenInterest      : " << mdata.OpenInterest        << "\n\t" <<
			"ClosePrice        : " << mdata.ClosePrice          << "\n\t" <<
			"SettlementPrice   : " << mdata.SettlementPrice     << "\n\t" <<
			"UpperLimitPrice   : " << mdata.UpperLimitPrice     << "\n\t" <<
			"LowerLimitPrice   : " << mdata.LowerLimitPrice     << "\n\t" <<
			"UpdateTime        : " << mdata.UpdateTime          << "\n\t" <<
			"UpdateMillisec    : " << mdata.UpdateMillisec      << "\n\t" <<
			"BidPrice1         : " << mdata.BidPrice1           << "\n\t" <<
			"BidVolume1        : " << mdata.BidVolume1          << "\n\t" <<
			"AskPrice1         : " << mdata.AskPrice1           << "\n\t" <<
			"AskVolume1        : " << mdata.AskVolume1          << "\n\t" <<
			"AveragePrice      : " << mdata.AveragePrice        << "\n\t" <<
			"ActionDay         : " << mdata.ActionDay);
}

class Client : public Ice::Application
{
	public:
		Client()
		{
			 //Ice::Application(Ice::NoSignalHandling);
		}
		virtual int run(int, char*[]);
};

int main(int argc, char* argv[])
{
    //set_daemon();
	LOG_INIT(__LOG__, 4, 1012400000, 5); 

	Client app;
	return app.main(argc, argv, "config.client");
}




int Client::run(int argc, char* argv[])
{
	try
	{

		MdSessionPrx server = MdSessionPrx::checkedCast(communicator()->propertyToProxy("MDLIVE.Proxy"));

	
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("");

		Ice::Identity ident;
		ident.name = IceUtil::generateUUID() ; //server唯一标识
		ident.category = "";                                             

		MdSessionCallBackPtr cb = new MdSessionCallBackI;
		adapter->add(cb, ident);
		adapter->activate();

		server->ice_getConnection()->setAdapter(adapter);
		
		server->setCallBack(ident);

		server->subscribeMd("m1805-C-3100", "T");
		server->subscribeMd("m1805-C-3100", "M1");
		//server->subscribeMd("ag1712", "M3");
		//server->subscribeMd("ag1712", "M5");
		//server->subscribeMd("ag1712", "M15");
		//server->subscribeMd("ag1712", "M30");
        //
        //
        //
        //server->subscribeMd("TF1709", "M1");
        //server->subscribeMd("TF1709", "M5");
        //server->subscribeMd("TF1709", "M30");
        //
        //
        ////server->subscribeMd("TF1709", "M5");
		////server->subscribeMd("IF1706", "M5");
		////server->subscribeMd("jd1709", "M5");
		////server->subscribeMd("jm1709", "M5");
		////server->subscribeMd("cu1707", "M5");
		////server->subscribeMd("rb1710", "M5");
	
		//server->subscribeMd("TF1709", "M30");
		//server->subscribeMd("IF1706", "M30");
		//server->subscribeMd("ag1712", "M30");
		//server->subscribeMd("jd1709", "M30");
		//server->subscribeMd("jm1709", "M30");
		//server->subscribeMd("cu1707", "M30");
		//server->subscribeMd("rb1710", "M30");
		//
		//server->subscribeMd("TF1709", "M15");
		//server->subscribeMd("IF1706", "M15");
		//server->subscribeMd("ag1712", "M15");
		//server->subscribeMd("jd1709", "M15");
		//server->subscribeMd("jm1709", "M15");
		//server->subscribeMd("cu1707", "M15");
		//server->subscribeMd("rb1710", "M15");
		//	
        //string ss[] = {"bu1709", "SR709", "a1709", "wr1706", "JR707", "LR707", "RS707", "al1707", "pb1707", "zn1707", "cu1708", "fu1709", "ZC709", "CF709", "TA709", "y1709", "FG709", "OI709", "PM709", "RM709", "SF709", "c1709", "WH709", "cs1709", "fb1709", "i1709", "j1709", "jd1709", "jm1709", "l1709", "m1709", "p1709", "v1709", "MA709", "SM709", "pp1709", "RI709", "ni1709", "ru1709", "sn1709", "hc1710", "rb1710", "T1709", "IC1706", "IF1706", "IH1706", "au1712", "TF1709", "ag1712", "bb1804", "b1805"};
	    //for (uint32_t i=0; i<sizeof(ss)/sizeof(string); i++)
        //{
        //    //cout << "i = " << i << " c=" << ss[i] << " s=" << sizeof(ss)  << " ss=" << sizeof(string) <<  endl;
        //    server->subscribeMd(ss[i], "M1");
        //    server->subscribeMd(ss[i], "M5");
        //    server->subscribeMd(ss[i], "M15");
        //    server->subscribeMd(ss[i], "M30");
        //}
		
		//cout << server->QuerySubCurrent();
		//cout << server->QuerySubCurrent();
		cout << server->QuerySession();
		sleep(5);
		while(true)
		{
			//cout << server->QueryTicker("ag1706");
			//cout << server->QueryKline("ag1706");
			//cout << server->QuerySnap("ag1706");
			//server->QuerySession();

            server->heartBeat();
			//cout << "heartbeat: " << server->heartBeat() << endl;
			msleep(1000);
		}

	}
	catch(Ice::Exception& e)
	{
		cout << e << endl;
	}
	catch(...)
	{
		cout << "unknow exception" << endl;
	}

	communicator()->waitForShutdown();

	communicator()->destroy();

}





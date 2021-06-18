// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
//#include <Glacier2/Glacier2.h>
#include <signal.h>

#include "RollLog.h"
#include "MdLiveSession.h"

using namespace CM;
using namespace MdLive;
using namespace std;

class MdSessionCallBackI : public MdSessionCallBack
{
public:
	//”√ªß∂©‘ƒ «∑Òok
	virtual ::Ice::Int onTick(const ::std::string& TradingDay, const ::std::string& insid, const ::CM::Ticker& ticker, const ::Ice::Current& = ::Ice::Current());
	virtual ::Ice::Int onBar(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::Bar& bar, const int period, const ::Ice::Current& = ::Ice::Current());
	virtual ::Ice::Int onDay(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::DayBar& day, const ::Ice::Current& = ::Ice::Current());
	virtual void onDepthMarketData(const ::CM::DepthMarketData&, const ::Ice::Current& = ::Ice::Current());

};




// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef __MDLIVE_CLIENT__H__
#define __MDLIVE_CLIENT__H__

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <signal.h>

#include "future/tradetime.h"
#include "singleton.h"
#include "RollLog.h"
#include "MdLiveSession.h"
#include "MdSessionCallBackI.h"
#include "index_calculate.h"

using namespace CM;
using namespace MdLive;
using namespace std;

extern MdSessionPrx g_push__session;



class MdliveClient : public Ice::Application
{
public:
	virtual int run(int argc, char* argv[])
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
		g_push__session = server;
		
		subcribe(server);

		
		while(true)
		{
			if(server->heartBeat() < 0) return -1;
			
			uint32_t status = g_tradetime.GetMarketStatus(time(NULL), "SHFE");
			bool change = g_tradetime.OnChangeMarketStatus( "SHFE", status);
			if(g_tradetime.IsBeforeOpenMarket(status) && change) 
			{
				subcribe(server);
			}
			sleep(1);
		}
		//communicator()->waitForShutdown();
		communicator()->destroy();
		return -1;
	}

private:

	void subcribe(MdSessionPrx& _session)
	{
		//订阅所有的合约
		set<string> instrumentids_set = Singleton<BaseCalculate>::Instance().get_instrument_set();
		typeof(instrumentids_set.begin()) itr = instrumentids_set.begin();
		for(; itr != instrumentids_set.end(); itr++)
		{
			_session->subscribeMd(*itr, "M");
			_session->subscribeMd(*itr, "D"); //实时日k数据
			//_session->subscribeMd(*itr, "A");
		}
		
		//需要订阅主连ticker
		list<string>	ins = Singleton<BaseCalculate>::Instance().get_master_list();
		list<string>::iterator it = ins.begin();
		for(; it!=ins.end(); it++)
		{
			_session->subscribeMd(*it,  "T");
		}
	}


};


int run(int argc, char* argv[]);

#endif



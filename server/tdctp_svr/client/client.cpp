#include <iostream>
#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include "TradeSession.h"

#include "RollLog.h"
#include <json/json.h>
#include "future/helper.h"

using namespace std;
using namespace Trade;
using namespace Json;

#define __LOG__ "client.log"

class TdSessionCallBackI : public   TdSessionCallBack
{
	public:
		virtual ::Ice::Int onDone(const ::std::string& ins, const ::CM::Done& done, const ::CM::Account& account, const ::CM::PositionList& position, const ::Ice::Current& cur)
		{
			Json::Value d;
			get_json_done(&done, d);
			FastWriter  w;
			string ss = w.write(d);
			LOG_INFO("done " << ss);

			Json::Value a;
			get_json_account(&account, a);
			ss = w.write(a);
			LOG_INFO("account " << ss);


			typeof(position.begin()) itr = position.begin();
			for(; itr!=position.end(); itr++)
			{
				Json::Value p;
				get_json_position(&(*itr), p);
				ss = w.write(p);
				LOG_INFO("position " << ss);
			}


		}
		virtual ::Ice::Int onOrder(const ::std::string& ins, const ::CM::Order& order, const ::Ice::Current&  cur)
		{
			LOG_INFO("instrument=" << ins);
			Json::Value o;
			get_json_order(&order, o);
			FastWriter  w;
			string ss = w.write(o);
			LOG_INFO("order  " << ss);
		}
		virtual ::Ice::Int onTradingday(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current())
		{

		}
};


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
	LOG_INIT(__LOG__, 4, 1012400000, 5); 

	Client app;
	return app.main(argc, argv, "config.client");
}

int Client::run(int, char*[])
{
	try
	{

		TdSessionPrx server = TdSessionPrx::checkedCast(communicator()->propertyToProxy("TDCTP.Proxy"));

	
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("");

		Ice::Identity ident;
		ident.name = IceUtil::generateUUID() ; //serverÎ¨Ò»±êÊ¶
		ident.category = "";                                             

		TdSessionCallBackPtr cb = new TdSessionCallBackI;
		adapter->add(cb, ident);
		adapter->activate();

		server->ice_getConnection()->setAdapter(adapter);
		
		//1610,1610,1637  
		server->setCallBack(ident);
		
	
		string fundid = "068074";
		server->subscribe("client", fundid);
		server->subscribe("client", fundid);
	//	server->subscribe("client", "1637");
	
	//	while(1)
		{
			//cout << "start=" << time(NULL) << endl;
			//
			cout << server->QuerySession() << endl; 
		//cout << server->QuerySubCurrent(); 
			//cout << "query=" << time(NULL) << endl;
			server->heartBeat();
			//cout << "herat=" << time(NULL) << endl;
	//		sleep(3);
			//cout << "sleep=" << time(NULL) << endl;


		}	

		cout << "query\n";

		cout << "query exchange account  =" << server->jsonQueryAccountOne(fundid, 1) << endl;
	
		Json::Value ro;
		CM::Account aa = server->queryAccountOne(fundid);
		get_json_account(&aa, ro);
		cout << "Account=" << ro.toStyledString() << endl;

		return 0;

		CM::AccountList a = server->queryAccount();
		LOG_INFO("AccountList size:" << a.size());
		typeof(a.begin()) itr = a.begin();
		for(; itr!=a.end(); itr++)
		{
			Json::Value r;
			get_json_account(&*itr, r);
			Json::FastWriter writer;  
			string ss = writer.write(r);
			LOG_INFO(ss);
			cout << "AccountList:" << ss << endl;
		}

		CM::DoneList d = server->queryDone(fundid);
		LOG_INFO("DoneList size:" << d.size());
		typeof(d.begin()) itrd = d.begin();
		for(; itrd!=d.end(); itrd++)
		{
			Json::Value r;
			get_json_done(&*itrd, r);
			Json::FastWriter writer;  
			string ss = writer.write(r);
			LOG_INFO(ss);
			cout << "DoneList:" << ss << endl;
		}

		CM::PositionList p = server->queryPosition(fundid);
		LOG_INFO("PositionList size:" << p.size());
		typeof(p.begin()) itrp = p.begin();
		for(; itrp!=p.end(); itrp++)
		{
			Json::Value r;
			get_json_position(&*itrp, r);
			Json::FastWriter writer;  
			string ss = writer.write(r);
			LOG_INFO(ss);
			cout << "PositionList:" << ss << endl;
		}

		CM::OrderList o = server->queryOrder(fundid);
		LOG_INFO("OrderList size:" << o.size());
		typeof(o.begin()) itro = o.begin();
		for(; itro!=o.end(); itro++)
		{
			Json::Value r;
			get_json_order(&*itro, r);
			Json::FastWriter writer;  
			string ss = writer.write(r);
			LOG_INFO(ss);
			cout << "OrderList:" << ss << endl;
	
		}

		cout << "query exchange account  =" << server->jsonQueryAccount(1) << endl;;
		sleep(2);
		cout << "query exchange posoition  =" << server->jsonQueryPosition(fundid, 1) << endl;;
		sleep(2);
		cout << "query exchange order =" << server->jsonQueryOrder(fundid, 1) << endl;;
		sleep(2);
		cout << "query exchange done =" << server->jsonQueryDone(fundid, 1) << endl;;
		
		/*
		while(true)
		{	
			try{
				int ret = (int)server->heartBeat();
				if(ret < 0)
				{
					cout << "reconnection: " <<  ret << endl;
					server->setCallBack(ident); 
					server->subscribe("client", fundid);
					continue;
				}

				cout << "server->heartBeat() sleep 30:" << ret << endl;
				sleep(10);
				cout << "server->heartBeat():" << ret << endl;
			}
			catch(Ice::Exception& e)
			{
				cout << "Exception: " <<  e << endl;
					server->setCallBack(ident); 
					server->subscribe("client", fundid);
				
			}
		}
*/
		itr = a.begin();
		for(; itr!=a.end(); itr++)
		{
			::CM::DoOrder doorder;
			doorder.fundid			=	itr->fundid;        
			//doorder.brokerid		=	itr->brokerid;      
			doorder.instrumentid	=	"ag1701";  
			doorder.ordertype		=	"1";     
			doorder.direction		=	"buy";     
			doorder.offsetflag		=	"open";    
			doorder.hedgeflag		=	"speculation";     
			doorder.price			=	4080;         
			doorder.volume			=	1;        
			doorder.donetype		=	"0";
			
			doorder.seq = 0;
			doorder.parentid = "";
			doorder.strategyid = "FF";
			doorder.userid = "test";
			doorder.signalname ="MM";
			cout << "doOrder:" << server->doOrder(doorder) << endl;
			cout << "do order fundid=" << itr->fundid << endl;  
		}

		itro = o.begin();
		for(; itro!=o.end(); itro++)
		{
			string ins = itro->instrumentid;
			string fundid = itro->fundid;
			string privateno = itro->privateno;
			string orderid = itro->orderid;
			string sessionid = itro->sessionid;
			cout << "cancleOrder=" << server->cancleOrder(fundid, sessionid, ins, privateno, orderid) << endl;

			cout << "cancle order fundid=" << fundid << " instrumentid=" << ins << " orderid=" << orderid << endl;
		}
	
		sleep(5);
	}
	catch(Ice::Exception& e)
	{
		cout << "Exception: " << e << endl;
	}
	catch(...)
	{
		cout << "unknow exception" << endl;
	}

	communicator()->waitForShutdown();
}













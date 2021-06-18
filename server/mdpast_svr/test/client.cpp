#include <iostream>
#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include "MdPastSession.h"

#include "RollLog.h"
#include "util.h"

using namespace std;
using namespace MdPast;

#define __LOG__ "client.log"

class MdSessionCallBackI : public   MdPast::MdSessionCallBack
{
	public:
		virtual ::Ice::Int onTick(const ::std::string& TradingDay, const ::std::string& InstrumentID, const ::CM::Ticker& ticker, const ::Ice::Current& current)
		{
			//cout << "onTick TradingDay=" <<  TradingDay << " InstrumentID=" << InstrumentID << endl;

			LOG_DEBUG(InstrumentID << " " << ticker.Timestamp <<" " << ticker.Price <<" " << ticker.Volume <<" " << ticker.Turnover <<" " << ticker.OpenInterest <<" " << ticker.TotalVolume <<" " << ticker.TotalTurnover <<" " << ticker.BidPrice1 <<" " << ticker.AskPrice1 <<" " << ticker.BidVolume1 <<" " << ticker.AskVolume1);
			return 0;
		}
		virtual ::Ice::Int onBar(const ::std::string& TradingDay, const ::std::string& InstrumentID, const ::CM::Bar& bar, const int period, const ::Ice::Current& current)
		{
			//cout << "onBar TradingDay=" <<  TradingDay << " InstrumentID=" << InstrumentID << " period=" << period << endl;
			LOG_DEBUG(InstrumentID << ":" << period << " " << stamptostr(bar.Timestamp/1000, "%Y%m%d %H:%M:%S") << " " << bar.High << " " << bar.Open << " " << bar.Low << " " << bar.Close << " " << bar.Volume << " " << bar.Turnover << " " << bar.OpenInterest);
			return 0;
		}
		virtual ::Ice::Int onDay(const ::std::string& TradingDay, const ::std::string& InstrumentID, const ::CM::DayBar& b, const ::Ice::Current& current)
		{
            LOG_DEBUG("TradingDay=" << TradingDay << " TradingDay=" << TradingDay << " Timestamp=" << b.Timestamp)
			return 0;
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


Ice::Identity ident;
MdSessionPrx server;
pthread_t pid;

string req[]= {
        "ag1705", "M3", "20170111", "20170118",
        "ag1706", "M3", "20170111", "20170118",
        "IF1703", "M3", "20170111", "20170118",
        "cu1708", "M3", "20170111", "20170118",
        "pp1705", "M3", "20170111", "20170118",
        "FG1705", "M3", "20170111", "20170118",
        "j1705",  "M3", "20170111", "20170118",
        "j1701",  "M3", "20170111", "20170118",
        "T1703",  "M3", "20170111", "20170118",
        "bu1705", "M3", "20170111", "20170118",
        "bu1703", "M60", "20170111", "20170118",
        "IF1705", "M60", "20170111", "20170118",
        "cu1705", "M60", "20170111", "20170118",
        "ag1705", "M60", "20170111", "20170118",
        "j1705",  "M60", "20170111", "20170118",
        "FG1705", "M60", "20170111", "20170118",
        "T1705",  "M60", "20170111", "20170118",
        "pp1705", "M60", "20170111", "20170118"
        "pp1705", "M60", "20170111", "20170118"
        "pp1705", "M60", "20170111", "20170118"
};

void* fun(void* arg)
{
    srand(time(NULL));
    int row = rand() % 5;
    string* r = &req[0] + row*4;
	server->queryData(ident, r[1], r[0], r[2], r[3]); 
}

int Client::run(int argc, char* argv[])
{
	if(argc < 5)
	{
		cout << "usage:  " << argv[0] << "  M3 ag1705 20170116 20170116" << endl;
	}

	try
	{

		server = MdSessionPrx::checkedCast(communicator()->propertyToProxy("MDPAST.Proxy"));

	
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("");


		ident.name = IceUtil::generateUUID() ; //serverÎ¨Ò»±êÊ¶
		ident.category = "";                                             

		MdSessionCallBackPtr cb = new MdSessionCallBackI;
		adapter->add(cb, ident);
		adapter->activate();

		server->ice_getConnection()->setAdapter(adapter);

        //for(int i=0; i<100; i++)
        //    pthread_create(&pid, NULL, fun, NULL);
		
        
        if(argc >= 5) server->queryData(ident, argv[1], argv[2], argv[3], argv[4]); 
      
        ::std::vector< ::std::string>  symbols, products, exchanges, productclas;
        ::std::vector<int> ranks, istrading;

        exchanges.push_back("SHFE");
        exchanges.push_back("DCE");
        exchanges.push_back("CZCE");
        cout << server->queryInstruments(symbols, products, exchanges, ranks, productclas, istrading); 
	
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













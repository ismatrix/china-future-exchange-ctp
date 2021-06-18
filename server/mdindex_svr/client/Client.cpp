// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <Glacier2/Glacier2.h>
#include <signal.h>

#include "RollLog.h"
#include "MdLiveSession.h"
#include "stdef.h"

using namespace CM;
using namespace MdLive;
using namespace std;

class MdSessionCallBackI : public MdSessionCallBack
{
public:
	//用户订阅是否ok
	virtual ::Ice::Int onTick(const ::std::string& TradingDay, const ::std::string& insid, const ::CM::Ticker& ticker, const ::Ice::Current& = ::Ice::Current());
	virtual ::Ice::Int onBar(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::Bar& bar, const ::Ice::Current& = ::Ice::Current());
	virtual ::Ice::Int onDay(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::DayBar& day, const ::Ice::Current& = ::Ice::Current());
};

::Ice::Int MdSessionCallBackI::onTick(const ::std::string& TradingDay, const ::std::string& insid, const ::CM::Ticker& ticker, const ::Ice::Current& current)
{
	//LOG_DEBUG("recv TickerItem, TradingDay:" << TradingDay << " InstrumentID:" << insid);
	//LOG_DEBUG("KEY:" << current.id.name);

	LOG_DEBUG(insid << " " << ticker.Timestamp <<" " << ticker.Price <<" " << ticker.Volume <<" " << ticker.Turnover <<" " << ticker.OpenInterest <<" " << ticker.TotalVolume <<" " << ticker.TotalTurnover <<" " << ticker.BidPrice1 <<" " << ticker.AskPrice1 <<" " << ticker.BidVolume1 <<" " << ticker.AskVolume1);

	return 0;  
}
::Ice::Int MdSessionCallBackI::onBar(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::Bar& bar, const ::Ice::Current& current)
{
	//LOG_DEBUG("recv kline, TradingDay:" << TradingDay << " InstrumentID:" << insid);
	LOG_DEBUG(insid << " " << bar.Timestamp << " " << bar.High << " " << bar.Low << " " << bar.Open << " " << bar.Close << " " << bar.Volume << " " << bar.Turnover);
	return 0;  
}
::Ice::Int MdSessionCallBackI::onDay(const ::std::string& TradingDay, const ::std::string& insid,  const ::CM::DayBar& day, const ::Ice::Current& current)
{
	//LOG_DEBUG("recv day_kline, TradingDay:" << TradingDay << " InstrumentID:" << insid);
	//LOG_DEBUG("KEY:" << current.id.name);
	//cout << "timestamp:" << day.Timestamp << endl;
	
	return 0;  
}


//定时refresh
class _Timer : public  IceUtil::TimerTask
{
	public:
		virtual void runTimerTask()
		{

		}
};

void handl(int sig)
{
	cout << "sig:" << sig << endl;
	exit(-1);
}

class SubcribeClient : public Glacier2::Application
{
public:

    SubcribeClient() : Application(Ice::NoSignalHandling){};

    virtual Glacier2::SessionPrx createSession()
    {
        Glacier2::SessionPrx sessionPrx;
        while(!sessionPrx)
        {
            try
            {
				//这里要分配serverid  和 name
				string id = "MdSessionCallBack";
				string pwd = "MdSessionCallBack";

				sessionPrx = router()->createSession(id, pwd);
                break;
            }
            catch(const Glacier2::CannotCreateSessionException& ex)
            {
				LOG_ERROR("Glacier2::CannotCreateSessionException: " << string(ex.reason));
            }
            catch(const Glacier2::PermissionDeniedException& ex)
            {
				LOG_ERROR("Glacier2::PermissionDeniedException: " << string(ex.reason));
            }
            catch(const Ice::LocalException& ex)
            {
				LOG_ERROR("LocalException  Communication with the server failed: "); //<< string(ex));
            }
        }
        return sessionPrx;
    }

	virtual void sessionDestroyed()
	{
		LOG_INFO("Session destroyed");
	}

    virtual int  runWithSession(int argc, char*[])
    {
		Ice::Identity callbackReceiverIdent = createCallbackIdentity("callbackReceiver");
        MdSessionCallBackPtr cb = new MdSessionCallBackI;
	    MdSessionCallBackPrx callback = MdSessionCallBackPrx::uncheckedCast(objectAdapter()->add(cb, callbackReceiverIdent));
		
	    MdSessionPrx _session = MdSessionPrx::uncheckedCast(this->session());
	    _session->setCallBack(callback);
	

		//signal(SIGINT, handl);
		
		
		//MdSessionPrx _session = MdSessionPrx::uncheckedCast(this->session());
        //_session->SetCallBack(MdSessionCallBackPrx::uncheckedCast(addWithUUID(new MdSessionCallBackI())));



		//订阅
        //_session->subscribeMd("RM609", "A");
        //_session->subscribeMd("IF1606", "M");
        //_session->subscribeMd("TF1609", "A");
        //_session->subscribeMd("IF1606", "K");
        //_session->subscribeMd("pb1606", "A");

        //_session->subscribeMd("IH1605", "A");
        //_session->subscribeMd("IH1606", "A");
        //_session->subscribeMd("IH1609", "A");
        //
	
		//_session->subscribeMd("IC1605", "A");
		//_session->subscribeMd("IC1606", "A");
		//_session->subscribeMd("IC1609", "A");
    
        _session->subscribeMd("bu1605", "A");
        _session->subscribeMd("IH1606", "A");
        _session->subscribeMd("IH1609", "A");
    
        _session->subscribeMd("IH1605", "A");
        _session->subscribeMd("IH1606", "A");
        _session->subscribeMd("IH1609", "A");
    
        _session->subscribeMd("IH1605", "A");
        _session->subscribeMd("IH1606", "A");
        _session->subscribeMd("IH1609", "A");
    
        _session->subscribeMd("IH1605", "A");
        _session->subscribeMd("IH1606", "A");
        _session->subscribeMd("IH1609", "A");

        
		_session->subscribeMd("zn1609", "A");

    
		
	//	cout << _session->QueryTicker("");


		//cout << _session->QueryTicker("IF1604");
		//cout << _session->QuerySnap("");
		//cout << _session->QuerySnap("zn1609");
		//cout << _session->QueryKline("");
		//cout << _session->QueryKline("IF1604");

		while(true)
		{
			try
			{
			//	cout << _session->QuerySession() << "\n_callback\n_callback\n";
				//cout << _session->QuerySubCurrent();
				//cout << _session->QueryTicker("IF1607");
				//cout << _session->QueryKline("IF1604");
				//cout << _session->QueryKline("IF1606");

				_session->heartBeat();
			
				sleep(2);
				//LOG_DEBUG("refresh");
				//printf("req refresh\n");

			}
			catch(...)
			{
				printf("exception exit.");
				return 0;
			}
		}

        return EXIT_SUCCESS;
    }

private:

};


int main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif

	LOG_INIT(__FILE__, 4, 1024000000, 10);

    SubcribeClient app;
    return app.main(argc, argv, "config.client");
}





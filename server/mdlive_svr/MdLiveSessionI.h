/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 汇海基金
///@file MdLiveSessionI.h
///@brief 定义了客户端接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef __SESSIONI_H__
#define __SESSIONI_H__

#include <Ice/Ice.h>
#include <map>
#include <string>

//连接管理的
#include "MdLiveSession.h"
#include "SessionM.h"


using namespace MdLive;
using namespace std;


//连接管理类
class MdSessionI : public  MdLive::MdSession, public IceUtil::Mutex
{
public:

    MdSessionI(){};						//初始化的时候
	~MdSessionI(){};
	
	virtual ::Ice::Int setCallBack(const ::Ice::Identity& ident, const ::Ice::Current& = ::Ice::Current());
	::Ice::Int subscribeMd(const ::std::string&   insid, const ::std::string& type,  const ::Ice::Current& = ::Ice::Current());
	::Ice::Int unSubscribeMd(const ::std::string& insid, const ::std::string& type,  const ::Ice::Current& = ::Ice::Current());
	int   heartBeat(const Ice::Current& current = ::Ice::Current());				//客户端调用心跳
	void destroy(const ::Ice::Current& = ::Ice::Current());

	
    ::Ice::Int sendTicker(const ::std::string& tradingday, const ::std::string& instrument, const ::CM::Ticker& ticker, const ::Ice::Current& = ::Ice::Current());
    ::Ice::Int sendBar(const ::std::string& tradingday, const ::std::string& instrument, const ::CM::Bar& bar, const ::Ice::Current& = ::Ice::Current());
    ::Ice::Int sendDayBar(const ::std::string& tradingday, const ::std::string& instrument, const ::CM::DayBar& day, const ::Ice::Current& = ::Ice::Current());

	 ::std::string QuerySession(const ::Ice::Current& = ::Ice::Current());
	 ::std::string QuerySubAll(const ::Ice::Current& = ::Ice::Current());
	 ::std::string QuerySubCurrent(const ::Ice::Current& = ::Ice::Current());
	 ::std::string QueryTicker(const ::std::string& insid, const ::Ice::Current& = ::Ice::Current());
	 ::std::string QueryKline(const ::std::string& insid, const ::Ice::Current& = ::Ice::Current());
	 ::std::string QuerySnap(const ::std::string& insid,  const ::Ice::Current& = ::Ice::Current());

	private:
		SessionM::MemInfo* GetSession(const Ice::Current& current);
		string  GetSessionId(const Ice::Current& current);
	
		::MdLive::MdSessionCallBackPrx _callback;
};

typedef IceUtil::Handle<MdSessionI> MdSessionIPtr;


#endif




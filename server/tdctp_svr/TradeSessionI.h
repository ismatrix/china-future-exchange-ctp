/////////////////////////////////////////////////////////////////////////
///@system ����ϵͳ
///@company ��������
///@file TdSessionI.h
///@brief �����˿ͻ��˽ӿ�
///@history 
///20160326	dreamyzhang		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef __TRADE_SESSIONI_H__
#define __TRADE_SESSIONI_H__

#include <Ice/Ice.h>
#include <map>
#include <string>

//���ӹ����
#include "TradeSession.h"
#include "SessionM.h"


using namespace Trade;
using namespace std;


//���ӹ�����
class TdSessionI : public  Trade::TdSession, public IceUtil::Mutex
{
public:

    TdSessionI(){_client = false;};						//��ʼ����ʱ��
	~TdSessionI(){};
	
	virtual ::Ice::Int setCallBack(const ::Ice::Identity& ident, const ::Ice::Current& = ::Ice::Current());
	virtual int heartBeat(const ::Ice::Current& = ::Ice::Current());

	virtual ::CM::AccountList queryAccount(const ::Ice::Current& = ::Ice::Current());
	virtual ::CM::Account queryAccountOne(const ::std::string& fundid, const ::Ice::Current& = ::Ice::Current());
	virtual ::CM::PositionList queryPosition(const ::std::string& fundid,  const ::Ice::Current& = ::Ice::Current());
	virtual ::CM::OrderList queryOrder(const ::std::string& fundid, const ::Ice::Current& = ::Ice::Current());
	virtual ::CM::DoneList queryDone(const ::std::string& fundid, const ::Ice::Current& = ::Ice::Current());

	virtual string jsonQueryAccount(::Ice::Int from, const ::Ice::Current& = ::Ice::Current());
	virtual string jsonQueryAccountOne(const ::std::string& fundid, ::Ice::Int from, const ::Ice::Current& = ::Ice::Current());
	virtual string jsonQueryPosition(const ::std::string& fundid, ::Ice::Int from, const ::Ice::Current& = ::Ice::Current());
	virtual string jsonQueryOrder(const ::std::string& fundid, ::Ice::Int from, const ::Ice::Current& = ::Ice::Current());
	virtual string jsonQueryDone(const ::std::string& fundid, ::Ice::Int from, const ::Ice::Current& = ::Ice::Current());

    virtual ::Ice::Int doOrder(const ::CM::DoOrder& order, const ::Ice::Current& = ::Ice::Current());
    virtual ::Ice::Int cancleOrder(const ::std::string& fundid, const string& sessionid, const ::std::string& instrmentid, const ::std::string& privateno,  const ::std::string& orderid, const ::Ice::Current& = ::Ice::Current());
    virtual ::Ice::Int updatePassword(const ::std::string& oldpwd, const ::std::string& newpwd, const ::Ice::Current& = ::Ice::Current());
	
	
	virtual ::Ice::Int subscribe(const ::std::string& moduleName, const ::std::string& fundid, const ::Ice::Current& = ::Ice::Current());
	virtual ::Ice::Int unSubscribe(const ::std::string& fundid, const ::Ice::Current& = ::Ice::Current());

    virtual ::std::string getTradingday(const ::Ice::Current& = ::Ice::Current());
	
	virtual ::std::string QuerySession(const ::Ice::Current& = ::Ice::Current());
	virtual ::std::string QuerySubCurrent(const ::Ice::Current& = ::Ice::Current());

	private:
		string  GetSessionId(const Ice::Current& current);
		bool	_client;  
};

typedef IceUtil::Handle<TdSessionI> TdSessionIPtr;


#endif





////////////////////////////////////////////////////////////////////////
///@system ��һ��������ϵͳ
///@company �㺣����
///@file MdLiveSessionI.cpp
///@brief �����˿ͻ��˽ӿ�
///@history 
///20160326	dreamyzhang		�������ļ�
/////////////////////////////////////////////////////////////////////////

#include "MdLiveSessionI.h"
#include "SessionM.h"

using namespace std;

::Ice::Int MdSessionI::setCallBack(const ::Ice::Identity& ident, const ::Ice::Current& current)
{
	//Lock sync(*this);

	LOG_DEBUG("KEY:" << current.id.name);
	SessionM::MemInfo*  info  = new SessionM::MemInfo; //һ��session
	//��ȡ���ӵĿͻ�����Ϣ
	::Ice::IPConnectionInfoPtr connectionPtr = ::Ice::IPConnectionInfoPtr::dynamicCast(current.con->getInfo());
	info->_remoteAddress 	= connectionPtr->remoteAddress;
	info->_remotePort 		= connectionPtr->remotePort;
	info->_conn_id 			= connectionPtr->connectionId;		
	info->_callback         = MdSessionCallBackPrx::uncheckedCast(current.con->createProxy(ident));
	info->_session  		= MdSessionPrx::uncheckedCast(current.adapter->createProxy(current.id));	
	info->_timestamp 		= time(NULL);
	info->_id				= current.id.name; 	
	return SessionM::Instance().add(GetSessionId(current), info);
}

::Ice::Int MdSessionI::unSubscribeMd(const ::std::string& insid, const ::std::string& type, const ::Ice::Current& current)
{
	//Lock sync(*this);
	string id = GetSessionId(current);
    if(type[0] == 'M')
	    return SessionM::Instance().del_sub(id, insid, "M", atoll(type.substr(1).c_str()));
    else
	    return SessionM::Instance().del_sub(id, insid, type, 1);
}
	
::Ice::Int MdSessionI::subscribeMd(const ::std::string&   insid, const ::std::string& type, const ::Ice::Current&  current)
{
	//Lock sync(*this);
    if(type == "M") *(string*)&type = "M1";
    string id = GetSessionId(current);
    if(type[0] == 'M')
	    return SessionM::Instance().add_sub(id, insid, "M", atoll(type.substr(1).c_str()));
    else
	    return SessionM::Instance().add_sub(id, insid, type, 1);
}

int   MdSessionI::heartBeat(const Ice::Current& current)
{
	//Lock sync(*this);
	string id = GetSessionId(current);
    //LOG_ERROR("heartBeat " << id.c_str());
	return SessionM::Instance().refresh(id);
}

::Ice::Int MdSessionI::sendTicker(const ::std::string& tradingday, const ::std::string& instrument, const ::CM::Ticker& ticker, const ::Ice::Current&  current)
{
	LOG_DEBUG("recv master and index tradingday=" << tradingday << " instrument=" << instrument << " ticker=" << ticker.Timestamp);

	//ֱ��push��ȥ
	return SessionM::Instance().send_ticker(tradingday, instrument,  ticker);
}

::Ice::Int MdSessionI::sendBar(const ::std::string& tradingday, const ::std::string& instrument, const ::CM::Bar& bar, const ::Ice::Current&  current)
{
	LOG_DEBUG("recv master and index tradingday=" << tradingday << " instrument=" << instrument << " bar=" << bar.Timestamp);

	//ֱ��push��ȥ
	return SessionM::Instance().send_kline(tradingday, instrument,  bar, K1MINUTE);
}


::Ice::Int MdSessionI::sendDayBar(const ::std::string& tradingday, const ::std::string& instrument, const ::CM::DayBar& day, const ::Ice::Current& current)
{
	//�յ�����k����  ��������ֱ��push ʵʱ����Ҫ���¼��� 
	LOG_DEBUG("recv master and index tradingday=" << tradingday << " instrument=" << instrument << " bar=" <<  day.Timestamp);
	
	if(day.Close > 0.0001)
	{
		return SessionM::Instance().send_day_snap(tradingday, instrument, day);   
	}
	else
	{
		//����db
	}
}

/////////////////////////////////memmory ///////////////////////////////////////
::std::string MdSessionI::QuerySession(const ::Ice::Current& current)
{
	//Lock sync(*this);
	return SessionM::Instance().query_session(GetSessionId(current));
}

::std::string MdSessionI::QuerySubCurrent(const ::Ice::Current& current)
{
	//Lock sync(*this);
	return SessionM::Instance().query_current_sub(GetSessionId(current));
}

::std::string MdSessionI::QueryTicker(const ::std::string& insid, const ::Ice::Current& current)
{
	//Lock sync(*this);
	return SessionM::Instance().query_ticker(insid);
}

::std::string MdSessionI::QueryKline(const ::std::string& insid, const ::Ice::Current& current)
{
	//Lock sync(*this);
	
	return SessionM::Instance().query_kline(insid);
}

::std::string MdSessionI::QuerySnap(const ::std::string& insid, const ::Ice::Current& current)
{
	//Lock sync(*this);
	return SessionM::Instance().query_snap(insid);
}

string  MdSessionI::GetSessionId(const Ice::Current& current)
{
	
	::Ice::IPConnectionInfoPtr connectionPtr = ::Ice::IPConnectionInfoPtr::dynamicCast(current.con->getInfo());
	char key[32] = {0};
	snprintf(key, 32, "%s:%d", connectionPtr->remoteAddress.c_str(), connectionPtr->remotePort);
	
	//return current.id.name; 
	return  key;	
}




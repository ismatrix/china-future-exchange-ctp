/////////////////////////////////////////////////////////////////////////
///@system ����ϵͳ
///@company ��������
///@file TdSessionI.cpp
///@brief �����˿ͻ��˽ӿ�
///@history 
///20160326	dreamyzhang		�������ļ�
/////////////////////////////////////////////////////////////////////////

#include "TradeSessionI.h"
#include "CTPTradeDataProcess.h"

using namespace std;


::Ice::Int TdSessionI::setCallBack(const ::Ice::Identity& ident, const ::Ice::Current& current)
{
	Lock sync(*this);

	_client = true;
	
	SessionM::MemInfo*  info  = new SessionM::MemInfo; //һ��session
	//��ȡ���ӵĿͻ�����Ϣ
	::Ice::IPConnectionInfoPtr connectionPtr = ::Ice::IPConnectionInfoPtr::dynamicCast(current.con->getInfo());
	info->_remoteAddress 	= connectionPtr->remoteAddress;
	info->_remotePort 		= connectionPtr->remotePort;
	info->_conn_id 			= connectionPtr->connectionId;		
	info->_callback 		= TdSessionCallBackPrx::uncheckedCast(current.con->createProxy(ident));		
	//���ﴴ����  ����Ҫcurrent.adapter.remove(ident.name);
	//info->_session  		= TdSessionPrx::uncheckedCast(current.adapter->createProxy(ident));	
	info->_timestamp 		= time(NULL);
	info->_id				= current.id.name; 	
	
	return SessionM::Instance().add(GetSessionId(current), info);
}

::Ice::Int TdSessionI::unSubscribe(const ::std::string& fundid, const ::Ice::Current& current)
{
	Lock sync(*this);
	string id = GetSessionId(current);
	return SessionM::Instance().del_sub(id, fundid);
}
	
::Ice::Int TdSessionI::subscribe(const ::std::string& moduleName, const ::std::string& fundid, const ::Ice::Current& current)
{
	Lock sync(*this);
	string id = GetSessionId(current);
	return SessionM::Instance().add_sub(id, moduleName, fundid);
}

int   TdSessionI::heartBeat(const Ice::Current& current)
{
	Lock sync(*this);
	
	string id = GetSessionId(current);
	//LOG_DEBUG("id= " << id);
	if(0 != SessionM::Instance().refresh(id))
	{
		current.con->close(true);
		return -1;
	}
	return 0;
}

::CM::Account TdSessionI::queryAccountOne(const ::std::string& fundid, const ::Ice::Current&  current)
{
	Lock sync(*this);
	return g_syncsp->local_data.get_account(fundid);
}

::CM::AccountList TdSessionI::queryAccount( const ::Ice::Current&  current)
{
	Lock sync(*this);
	return g_syncsp->local_data.get_account_vector();;
}
::CM::PositionList TdSessionI::queryPosition(const ::std::string& fundid, const ::Ice::Current& current)
{
	Lock sync(*this);
	return g_syncsp->local_data.get_position_vector(fundid); 
}
::CM::OrderList TdSessionI::queryOrder(const ::std::string& fundid, const ::Ice::Current& current)
{
	Lock sync(*this);
	return g_syncsp->local_data.get_order_vector(fundid);     
}
::CM::DoneList TdSessionI::queryDone(const ::std::string& fundid,  const ::Ice::Current& current)
{
	Lock sync(*this);
	return g_syncsp->local_data.get_done_vector(fundid);        
}

::Ice::Int TdSessionI::updatePassword(const ::std::string& oldpwd, const ::std::string& newpwd, const ::Ice::Current&  current)
{
	Lock sync(*this);
	return g_syncsp->updatePassword(oldpwd, newpwd);
}

//��ѯԭʼ����
string TdSessionI::jsonQueryAccount(::Ice::Int from, const ::Ice::Current&  current)
{
	Lock sync(*this);
	return g_syncsp->queryAccount(from);
}

//��ѯԭʼ����
string TdSessionI::jsonQueryAccountOne(const ::std::string& fundid, ::Ice::Int from, const ::Ice::Current&  current)
{
	Lock sync(*this);
	return g_syncsp->queryAccountOne(from, fundid);
}

string TdSessionI::jsonQueryPosition(const ::std::string& fundid, ::Ice::Int from, const ::Ice::Current& current)
{
	Lock sync(*this);
	return g_syncsp->queryPosition(from, fundid);
}
string TdSessionI::jsonQueryOrder(const ::std::string& fundid, ::Ice::Int from, const ::Ice::Current& current)
{
	Lock sync(*this);
	return g_syncsp->queryOrder(from, fundid);
}
string TdSessionI::jsonQueryDone(const ::std::string& fundid, ::Ice::Int from, const ::Ice::Current& current)
{
	Lock sync(*this);
	return g_syncsp->queryDone(from, fundid);
}

::std::string TdSessionI::getTradingday(const ::Ice::Current&  current)
{
	Lock sync(*this);
	//��ȡtradingday ���Ա��̬����
	return toa(g_syncsp->tradingday);
}


::Ice::Int TdSessionI::doOrder(const ::CM::DoOrder& order, const ::Ice::Current& current)
{
	Lock sync(*this);

	string id = GetSessionId(current);
	bool c = SessionM::Instance().check_sub(id, order.fundid);
	if(c) return g_syncsp->DoOrder(order);
	return  -1;

}

Ice::Int TdSessionI::cancleOrder(const ::std::string& fundid, const string& sessionid, const ::std::string& instrmentid,  const ::std::string& privateno, const ::std::string& orderid, const ::Ice::Current&  current)
{
	Lock sync(*this);
	string id = GetSessionId(current);
	bool c = SessionM::Instance().check_sub(id, fundid);
	if(c) return g_syncsp->CancleOrder(fundid, sessionid, instrmentid, privateno, orderid);
	return -1;
}

/////////////////////////////////memmory ///////////////////////////////////////
::std::string TdSessionI::QuerySession(const ::Ice::Current& current)
{
	Lock sync(*this);
	return SessionM::Instance().query_session(GetSessionId(current));
}

::std::string TdSessionI::QuerySubCurrent(const ::Ice::Current& current)
{
	Lock sync(*this);
	return SessionM::Instance().query_current_sub(GetSessionId(current));
}

string  TdSessionI::GetSessionId(const Ice::Current& current)
{
	
	::Ice::IPConnectionInfoPtr connectionPtr = ::Ice::IPConnectionInfoPtr::dynamicCast(current.con->getInfo());
	char key[32] = {0};
	snprintf(key, 32, "%s:%d", connectionPtr->remoteAddress.c_str(), connectionPtr->remotePort);
	
	//return current.id.name; 
	return  key;	
}




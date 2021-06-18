/////////////////////////////////////////////////////////////////////////
///@system 交易系统
///@company 慧网基金
///@file TdSessionM.h
///@description  连接管理的类
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef  __SESSIONM_H__
#define  __SESSIONM_H__

//客户端回调的
#include <iostream>  
#include <sstream>  
#include <string>
#include <stdint.h>

#include <IceUtil/IceUtil.h>
#include <IceUtil/Timer.h>
#include <Ice/Ice.h>

#include <json/forwards.h>
#include <json/value.h>
#include <json/json.h>

#include "TradeSession.h"
#include "ice/Common.h"	//里面包含了所有的内存信息

#include "RollLog.h"  
#include "singleton.h"


using namespace std;

class _Timer_Task;

//这里面进行加锁
//所有的session数据更新  所有的数据推送
class  SessionM :public IceUtil::RecMutex, public IceUtil::Shared 
{
	public:
		SessionM(){Init(10);}
		class MemInfo;
		typedef map<string,  MemInfo*>		MapSessionType;

		struct MemInfo
		{
			::Trade::TdSessionPrx 			_session; 		//记录这个session 
			::Trade::TdSessionCallBackPrx 	_callback;		//记录回调指针
			string 					_moduleId;		//模块id	
			string 				 	_remoteAddress;	//客户端ip
			uint16_t			 	_remotePort;	//客户端port
			string 					_conn_id;		//连接id
			string					_id;			//client id
			string  				_name;			//client name 
			uint32_t				_timestamp; 	// The last time the session was refreshed.		
			//第一位代表ticker=T 第二位代表kline=K
			//T没有代表所有T  K没有代表所有K  (k1 k5 k15 k30)
			set<string>	_sub_bit;		//订阅位
		
			string show_as_json_obj()
			{
				try
				{
					stringstream ss,sss,t;
					ss << _session;
				//	sss << _callback;
					t << _timestamp;	
					
					Json::Value mem;
					mem["_session"]			= ss.str();
					mem["_callback"]		= sss.str();
					mem["_moduleId"]		=  _moduleId;     
					mem["_remoteAddress"]	=  _remoteAddress;
					mem["_remotePort"]		=  _remotePort;   
					mem["_conn_id"]			=  _conn_id;      
					mem["_id"]				=  _id;           
					mem["_name"]			=  _name;         
					mem["_timestamp"]		=  t.str(); 
					Json::Value sub;
					typeof(_sub_bit.begin()) itr = _sub_bit.begin();
					for(; itr!=_sub_bit.end(); itr++)
					{
						sub.append(*itr);
					}
					mem["sub_bit"] = sub;
					return mem.toStyledString();
				}
				catch(...)
				{
					return "";
				}
			}
		};
		
		
		static SessionM&  Instance();  //获取唯一实例
		int Init(int timeout);

		//session
		int add(string key, MemInfo* info);			//增加
		int remove(string key);		//删除
		
		//订阅位的增删 
		int  add_sub(string key, string module, string fundid);	//订阅T K
		int  del_sub(string key, string fundid);	//删除订阅T K
		bool  check_sub(string key, string fundid);	//查询订阅T K
	    int  refresh(string& key);	

		int timeout_check(int  _timeout);

		//客户端查询
		string print_session();
		string query_session(string id);
		string query_current_sub(string id);
		
		//主动push
		//onDone(const ::std::string&, const ::CM::Done&, const ::CM::Account&, const ::CM::PositionList&, const ::Ice::Current& = ::Ice::Current()) = 0;
		int  send_done(const ::std::string& fundid, const ::CM::Done& done, const ::CM::Account& account, const ::CM::PositionList& positions);
		//onOrder(const ::std::string&, const ::CM::Order&, const ::Ice::Current& = ::Ice::Current()) = 0;
		int  send_order(const ::std::string& fundid, const ::CM::Order& order);
	
		//onTradingday(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
		int send_tradingday(string fundid, string tradingday);


		 _Timer_Task*         _timer;

		MapSessionType			_sessions;	//session列表

		static SessionM*	_instance;
		static IceUtil::Mutex* _instanceMutex;

		IceUtil::Mutex       _mutex_mem;	//memory
};

class _Timer_Task : public IceUtil::TimerTask
{
	public:
	_Timer_Task(uint32_t timeout) : _timeout(timeout){};
	virtual void runTimerTask()
	{
		SessionM::Instance().timeout_check(_timeout);
	}

	int             _timeout;
};

#endif


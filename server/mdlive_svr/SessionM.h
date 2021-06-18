/////////////////////////////////////////////////////////////////////////
//@system 新一代交易所系统
///@company 汇海基金
///@file MdSessionM.h
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
#include <IceUtil/IceUtil.h>
#include <IceUtil/Timer.h>
#include <Ice/Ice.h>

#include <json/forwards.h>
#include <json/value.h>
#include <json/json.h>


//#include "MdLiveSessionI.h"
#include "MdLiveSession.h"
#include "future/hq.h"	//里面包含了所有的内存信息

#include "RollLog.h"  


using namespace std;
using namespace CM;

class _Timer_Task;
class SessionM;

//这里面进行加锁
//所有的session数据更新  所有的数据推送
class  SessionM : public IceUtil::Mutex //public IceUtil::RecMutex, public IceUtil::Shared 
{
	public:
		SessionM(){Init(10);}

		class SubBit;
		class MemInfo;

		typedef map<string,  MemInfo*>		MapSessionType;
		typedef map<string, set<SubBit> >  MapSubType;

		struct SubBit
		{
			SubBit(string type, int num) : _type(type), _num(num) {};
			string  _type;	//T K
			int  	_num;	//代表几分k
			
			bool operator < (const SubBit& sub)
			{
				if(sub._type < _type)
				{
					return true;
				}
				else if(sub._type > _type)
				{	
					return false;
				}
				else
				{
					return sub._num < _num;
				}
			}
			string show_as_json_obj()
			{
				stringstream ss;
				ss << "{type:" << _type << " ,num:" << _num << "}";
				return ss.str(); 
			}
		};
			
		struct MemInfo
		{
			::MdLive::MdSessionPrx 			_session; 		//记录这个session 
			::MdLive::MdSessionCallBackPrx 	_callback;		//记录回调指针
			string 					_moduleId;		//模块id	
			string 				 	_remoteAddress;	//客户端ip
			uint16_t			 	_remotePort;	//客户端port
			string 					_conn_id;		//连接id
			string					_id;			//client id
			string  				_name;			//client name 
			uint32_t			 	_timestamp; 	// The last time the session was refreshed.		
			//第一位代表ticker=T 第二位代表kline=K
			//T没有代表所有T  K没有代表所有K  (k1 k5 k15 k30)
			MapSubType	_sub_bit;		//订阅位
		
			string show_as_json_obj()
			{
				stringstream ss;
			
				ss << "_session:" << _session << " _moduleId="<<_moduleId << " _remoteAddress:" << _remoteAddress << " _remotePort:" << _remotePort << " _conn_id:" << _conn_id << " _id:" << " _name:" << _name << " _timestamp:" << _timestamp;
		
				MapSubType::iterator itr = _sub_bit.begin();
				for(; itr!=_sub_bit.end(); itr++)
				{
					set<SubBit>::iterator itl =  itr->second.begin();
					for(; itl!=itr->second.end(); itl++)
					{
						ss << "\n\t{ins:" << itr->first << ", type:" << itl->_type << " ,num:" << itl->_num << "}";
					}
				}
				return ss.str();
			}
		};
	  
        //1mk 的订阅信息保存
        map<string, set<uint16_t> > _msubcribe;

	
        set<uint16_t> get__msubcribe(string instrument);
        void add__msubcribe(string instrument, int num);
        void delete__msubcribe(string instrument, int num);
		
        static SessionM&  Instance();  //获取唯一实例
		int Init(int timeout);

		//session的操作
		int add(string key, MemInfo* info);			//增加
		int remove(string key);		//删除
		
		//订阅位的增删 
		int  add_sub(string key, string instrument, string type, int num=-1);	//订阅T K
		int  del_sub(string key, string instrument, string type, int num=-1);	//删除订阅T K
	    int  refresh(string& key);	
	
		//客户端查询
		string print_session();
		string query_session(string id);
		string query_current_sub(string id);
		
		int timeout_check(int  _timeout);
		
		
		/*************memory need mutex****************/
		//chaxun 
		string query_ticker(string insid);	//_mutex_mem
		string query_kline(string insid);	//_mutex_mem
		string query_snap(string insid);	//_mutex_mem
		
		//push数据到下游的 Run
		int  send_ticker(const string TradingDay, const string InstrumentId, const Ticker& ticker);//_mutex
		int  send_kline(const string TradingDay, const string InstrumentId, const Bar&  kline, const uint16_t period);//_mutex
		int  send_day_snap(const string TradingDay, const string InstrumentID , const CM::DayBar& day);

		int  send_depth_market_data(const CM::DepthMarketData& mdata);
		
		int  Run(const char* pin, uint32_t len); //_mutex_mem
		/*************memory****************/
		
		
		//请求kline转化为枚举类型
		KlineType TransKlineType(string type, int num);
		
		MapSessionType			_sessions;	//session列表
		_Timer_Task*		 _timer;
		int					timeout;

		static SessionM*	_instance;
		static IceUtil::Mutex* _instanceMutex;

		IceUtil::Mutex       _mutex_mem;	//memory
};


//定时器 负责轮询连接并处理 SessionM中的session  空订阅位同时清理
//定时器还是在单独的线程中处理的 
class _Timer_Task : public IceUtil::TimerTask
{
	public:

		_Timer_Task(uint32_t timeout) : _timeout(timeout){ }
		virtual void runTimerTask()
		{
			//这里没办法获取
			SessionM::Instance().timeout_check(_timeout);
		}
	private:
		int			 _timeout;			//超时时间
};



#endif


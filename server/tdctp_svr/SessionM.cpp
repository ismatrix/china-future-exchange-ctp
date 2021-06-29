/////////////////////////////////////////////////////////////////////////
///@system 交易系统
///@company 慧网基金
///@file MdSessionM.cpp
///@description  连接管理的类
///@history
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#include "SessionM.h"
#include "CTPTradeDataProcess.h"

//静态成员初始化
SessionM* SessionM::_instance = NULL;
IceUtil::Mutex* SessionM::_instanceMutex = new IceUtil::Mutex;

SessionM&  SessionM::Instance()
{
	IceUtil::Mutex::Lock sync(*_instanceMutex);
	if(_instance == NULL)
	{
		_instance = new SessionM();
	}

	return (*_instance);
}

int SessionM::Init(int timeout)
{
	//增加定时器  会调用到TdSessionI中的destroy导致死锁 针对每个TdSessionI加定时器
	_timer = new _Timer_Task(timeout); //超时时间
	IceUtil::TimerPtr _t = new IceUtil::Timer();
	_t->scheduleRepeated(_timer, IceUtil::Time::seconds(5));//5s一次检测

	return 0;
}

string  SessionM::print_session()
{
	Lock sync(*this);

	stringstream ss;
	ss << "{";
	typeof(_sessions.begin()) itr = _sessions.begin();
	for(; itr!=_sessions.end(); itr++)
	{
		if(itr == _sessions.begin())
			ss << "[" << itr->second->show_as_json_obj() << "]";
		else
			ss << ",[" << itr->second->show_as_json_obj() << "]";
	}
	ss <<"}";
	return ss.str();
}

//add session
int  SessionM::add(string key, MemInfo* info)
{
	Lock sync(*this);

	_sessions[key] = info;

	// LOG_INFO("add session[key:" << key << "]: " << info->show_as_json_obj());
	return 0;
}

//del session
//remove 只有TdSessionI::destroy调用
int  SessionM::remove(string key)
{
	Lock sync(*this);

	typeof(_sessions.begin()) itr =_sessions.find(key);
	if(itr != _sessions.end())
	{
		LOG_INFO("del session[key:" << key << "]:" << (itr->second)->show_as_json_obj());
		//清理工作
		delete (itr->second);
		_sessions.erase(itr); //删除session
	}
	return 0;
}

//add subcribe
int  SessionM::add_sub(string key, string module, string fundid)
{
	Lock sync(*this);

	//首先找到这个session
	typeof(_sessions.begin()) itr = _sessions.find(key);
	if(itr == _sessions.end())
	{
		LOG_ERROR("add subcribe fail key[" << key <<  "] not exist.");
		return -1;	//添加订阅失败  session不存在
	}

	LOG_INFO("old subcribe:" << (itr->second)->show_as_json_obj() << " add new subcribe:[" << fundid << "]");

	itr->second->_moduleId = module;

	//然后找到这个合约
	typeof(itr->second->_sub_bit.begin()) it = itr->second->_sub_bit.find(fundid);
	if(it == itr->second->_sub_bit.end())
	{
		itr->second->_sub_bit.insert(fundid);
	}
	return 0;
}

bool  SessionM::check_sub(string key,  string fundid)
{
	Lock sync(*this);

	//首先找到这个session
	typeof(_sessions.begin()) itr =  _sessions.find(key);
	if(itr == _sessions.end())
	{
		return false;	//session不存在
	}

	//然后找到这个合约
	typeof(itr->second->_sub_bit.begin()) it = itr->second->_sub_bit.find(fundid);
	if(it != itr->second->_sub_bit.end())
	{
		return true;
	}

	return false;
}


int  SessionM::del_sub(string key,  string fundid)
{
	Lock sync(*this);

	//首先找到这个session
	typeof(_sessions.begin()) itr =  _sessions.find(key);
	if(itr == _sessions.end())
	{
		LOG_ERROR("del subcribe fail:key[" << key <<  "] not exist.");
		return 0;	//删除失败  session不存在
	}

	//然后找到这个合约 删除
	typeof(itr->second->_sub_bit.begin()) it = itr->second->_sub_bit.find(fundid);
	if(it != itr->second->_sub_bit.end())
	{
		itr->second->_sub_bit.erase(it);
	}

	return 0;
}

int  SessionM::refresh(string& key)
{
	Lock sync(*this);
	typeof(_sessions.begin()) itr =  _sessions.find(key);
	if(itr != _sessions.end())
	{
		itr->second->_timestamp = time(NULL);

		//LOG_DEBUG("_timestamp=" << itr->second->_timestamp);
		return 0;
	}
	return -1;
}

int SessionM::timeout_check(int  _timeout)
{
	Lock sync(*this);
	 map<string, SessionM::MemInfo*>::iterator itr = _sessions.begin();
	 for(; itr!=_sessions.end(); )
	 {
		 SessionM::MemInfo* mem = itr->second;
		 try
		 {
			 if((time(NULL) - mem->_timestamp) > _timeout*2)
			 {
				LOG_ERROR("[" << mem->_remoteAddress << ":" << mem->_remotePort << "]conn_id:" << mem->_conn_id << " id:" << mem->_id << " name:" << mem->_name << " has timeout.[" << (uint32_t)time(NULL) << " - " << mem->_timestamp << " > " << _timeout*2 << "]");
				_sessions.erase(itr++);
			 }
			 else
			 {
				 itr++;
			 }
		 }
		 catch(const Ice::Exception& e)
		 {
			LOG_ERROR("[" << mem->_remoteAddress << ":" << mem->_remotePort << "]Exception conn_id:" << mem->_conn_id << " id:" << mem->_id << " name:" << mem->_name << " has timeout.");
			_sessions.erase(itr++);
		 }
	 }
	 return 0;
}

string SessionM::query_session(string id)
{
	string ss = print_session();
	//LOG_DEBUG(ss);
	return ss;
}

string SessionM::query_current_sub(string id)
{
	Lock sync(*this);

	//查询当前session
	typeof(_sessions.begin()) itr = _sessions.find(id);
	if(itr != _sessions.end())
	{
		return itr->second->show_as_json_obj();
	}
	return "{}";
}

//onOrder(const ::std::string&, const ::CM::Order&, const ::Ice::Current& = ::Ice::Current()) = 0;
int  SessionM::send_order(const ::std::string& fundid, const ::CM::Order& order)
{
	Lock sync(*this);
	map<string, MemInfo*>::iterator  itr = _sessions.begin();
	for(; itr!=_sessions.end(); itr++)
	{
		MemInfo* info = itr->second;

		//检查这个session 这里只需要推送ticker
		typeof(info->_sub_bit.begin()) it = info->_sub_bit.find(fundid);
		if(it == info->_sub_bit.end())
		{
			LOG_DEBUG("fundid:" << fundid << " not sub");
			continue;
		}

		try
		{
			//printf("TickerItem before\n");
			info->_callback->begin_onOrder(fundid, order);
			//printf("TickerItem end\n");
			LOG_DEBUG("[" << itr->first << "]push order fundid:" << fundid);
		}
		catch(Ice::Exception& e)
		{
			//这里检查session看看是不是断开连接了 TODO
			//或者由定时器来处理连接

			stringstream ss;
			ss << e;
			LOG_ERROR("[" << itr->first << "]push order	fail." << ss.str())
		}
	}

	return 0;
}


//onOrder(const ::std::string&, const ::CM::Order&, const ::Ice::Current& = ::Ice::Current()) = 0;
int  SessionM::send_done(const ::std::string& fundid, const ::CM::Done& done, const ::CM::Account& account, const ::CM::PositionList& positions)
{
	Lock sync(*this);

	LOG_DEBUG("_sessions.size()=" << _sessions.size());
	map<string, MemInfo*>::iterator  itr = _sessions.begin();
	for(; itr!=_sessions.end(); itr++)
	{
		MemInfo* info = itr->second;

		//检查这个session 这里只需要推送ticker
		typeof(info->_sub_bit.begin()) it = info->_sub_bit.find(fundid);
		if(it == info->_sub_bit.end())
		{
			LOG_DEBUG("fundid:" << fundid << " not sub fundid.");
			continue;
		}

		try
		{
			//printf("TickerItem before\n");
			info->_callback->begin_onDone(fundid, done, account, positions);
			//printf("TickerItem end\n");
			LOG_DEBUG("[" << itr->first << "]push done fundid:" << fundid);
		}
		catch(Ice::Exception& e)
		{
			//这里检查session看看是不是断开连接了 TODO
			//或者由定时器来处理连接
			stringstream ss;
			ss << e;
			LOG_ERROR("[" << itr->first << "]push done	fail."  << ss.str())
		}
	}

	return 0;
}

int SessionM::send_tradingday(string fundid, string tradingday)
{
	Lock sync(*this);

	LOG_DEBUG("_sessions.size()=" << _sessions.size());
	map<string, MemInfo*>::iterator  itr = _sessions.begin();
	for(; itr!=_sessions.end(); itr++)
	{
		MemInfo* info = itr->second;

		//检查这个session 这里只需要推送ticker
		typeof(info->_sub_bit.begin()) it = info->_sub_bit.find(fundid);
		if(it == info->_sub_bit.end())
		{
			LOG_DEBUG("fundid:" << fundid << " not sub fundid.");
			continue;
		}

		try
		{
			info->_callback->begin_onTradingday(fundid, tradingday);
			LOG_DEBUG("[" << itr->first << "]push tradingday fundid:" << fundid);
		}
		catch(Ice::Exception& e)
		{
			//这里检查session看看是不是断开连接了 TODO
			//或者由定时器来处理连接
			stringstream ss;
			ss << e;
			LOG_ERROR("[" << itr->first << "]push tradingday fail."  << ss.str())
		}
	}

	return 0;

}

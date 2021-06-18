/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 汇海基金
///@file MdSessionM.cpp
///@description  连接管理的类
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#include "SessionM.h"


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
	//增加定时器  会调用到MdSessionI中的destroy导致死锁 针对每个MdSessionI加定时器
	_timer = new _Timer_Task(timeout);
	IceUtil::TimerPtr _t = new IceUtil::Timer();
	_t->scheduleRepeated(_timer, IceUtil::Time::seconds(5));
	
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

	LOG_INFO("add session[key:" << key << "]: " << info->show_as_json_obj());
	return 0;
}

//del session
//remove 只有MdSessionI::destroy调用
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

void SessionM::add__msubcribe(string instrument, int num)
{
    if(_msubcribe.find(instrument) == _msubcribe.end())
    {
        set<uint16_t>  s;
        s.insert(num);
        _msubcribe[instrument] = s;
    }
    else
    {
        _msubcribe[instrument].insert(num);
    }
    LOG_INFO("instrument=" << instrument << " num="<<num );
}
void SessionM::delete__msubcribe(string instrument, int num)
{
    if(_msubcribe.find(instrument) == _msubcribe.end())
    {
        return;
    }

    _msubcribe[instrument].erase(num);
    LOG_INFO("instrument=" << instrument << " num="<<num );
}

set<uint16_t> SessionM::get__msubcribe(string instrument)
{
	Lock sync(*this);
    set<uint16_t> s;
    if(_msubcribe.find(instrument) == _msubcribe.end())
    {
        return s;
    }

    return _msubcribe[instrument];
}



//add subcribe 
int  SessionM::add_sub(string key, string instrument, string type, int num)
{
	Lock sync(*this);
	
	//首先找到这个session 
	typeof(_sessions.begin()) itr = _sessions.find(key);
	if(itr == _sessions.end())
	{
		LOG_ERROR("add subcribe fail key[" << key <<  "] not exist.");
		return -1;	//添加订阅失败  session不存在
	}
	
	LOG_INFO("new subcribe:[" << instrument << "," << type << "," << num << "]");
	
	//然后找到这个合约
	typeof(itr->second->_sub_bit.begin()) it = itr->second->_sub_bit.find(instrument);
	if(it == itr->second->_sub_bit.end())
	{
		set<SessionM::SubBit> l;
		l.insert(SessionM::SubBit(type, num));
		itr->second->_sub_bit.insert(std::pair<string, set<SessionM::SubBit> >(instrument, l));
	}
	else
	{
		//直接插入订阅位 server端暂时不做检测
		it->second.insert(SessionM::SubBit(type, num)); 
	}
    
    if(type == "M" && num != 1)
    {
        add__msubcribe(instrument, num);
    }
	return 0;
}

int  SessionM::del_sub(string key,  string instrument, string type, int num)
{
	Lock sync(*this);
	
	//首先找到这个session 
	typeof(_sessions.begin()) itr =  _sessions.find(key);
	if(itr == _sessions.end())
	{
		LOG_ERROR("del subcribe fail:key[" << key <<  "] not exist.");
		return 100;	//删除失败  session不存在
	}
	
	//然后找到这个合约
	typeof(itr->second->_sub_bit.begin()) it = itr->second->_sub_bit.find(instrument);
	if(it == itr->second->_sub_bit.end())
	{
		LOG_ERROR("del subcribe fail: key[" << key <<  "] not exist.");
		return 101;	//删除失败  订阅位不存在
	}
	
	typeof(it->second.begin()) ilist = it->second.begin();
	for(; ilist!=it->second.end(); )
	{
		if(ilist->_type == type && ilist->_num == num)
		{
			LOG_INFO("del subcribe:[" << instrument << "," << type << "," << num << "]");
			it->second.erase(ilist++);
		}
		else
		{
			ilist++;
		}
	}
    if(type == "M" && num != 1)
    {
        delete__msubcribe(instrument, num);
    }
	
	return 0;
}

int  SessionM::refresh(string& key)	
{
	Lock sync(*this);	
	typeof(_sessions.begin()) itr =  _sessions.find(key);
	if(itr != _sessions.end())
	{
		//LOG_ERROR("have heartbeat key = " << key) ;
		itr->second->_timestamp = time(NULL);
		return 0;
	}
		//LOG_ERROR("no heartbeat key = " << key) ;
	return -1;
}

string SessionM::query_session(string id)
{
	return print_session();
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


//5s 定时期session的操作和Mdsession要互斥
int SessionM::timeout_check(int  _timeout)
{
	Lock sync(*this);	
	//定时器需要的是Mdsession的锁
	map<string, SessionM::MemInfo*>::iterator itr = _sessions.begin();
	for(; itr!=_sessions.end(); )
	{
		SessionM::MemInfo* mem = itr->second;
		try
		{
			//检查超时时间处理
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



// push ticker数据  
int  SessionM::send_ticker(const string TradingDay, const string InstrumentId, const Ticker& ticker)
{
	Lock sync(*this);	
	
	map<string, MemInfo*>::iterator  itr = _sessions.begin();
	for(; itr!=_sessions.end(); itr++)
	{
		MemInfo* info = itr->second;
		
		//检查这个session 这里只需要推送ticker 
		map<string, set<SubBit> >::iterator it = info->_sub_bit.find(InstrumentId);
		if(it == info->_sub_bit.end()) 
		{
			//LOG_DEBUG("InstrumentId:" << InstrumentId << " not sub ticker");
			continue;
		}
		//遍历合约订阅位列表
		set<SubBit>::iterator itl =  it->second.begin();
		for(; itl!=it->second.end(); itl++)
		{
			if(itl->_type == "A" || itl->_type == "T")
			{
				try
				{
					//printf("TickerItem before\n");
					info->_callback->begin_onTick(TradingDay, InstrumentId, ticker);
					//printf("TickerItem end\n");
					LOG_DEBUG("[" << itr->first << "]push ticker InstrumentId:" << InstrumentId << " key:" << ticker.Timestamp);
				}
				catch(const Ice::Exception& e)
				{

					//printf("TickerItem exception\n");
					stringstream ss;
					ss << e;
					LOG_ERROR("[" << itr->first << "]push ticker fail." << ss.str());
				}				
			}
			//LOG_DEBUG("_type:" << itl->_type);
		}

	}
	return 0;
}

// push kline数据  
int  SessionM::send_kline(const string TradingDay, const string InstrumentId, const Bar&  kline, const uint16_t period)
{
	Lock sync(*this);	
	
	map<string,  MemInfo*>::iterator  itr = _sessions.begin();
	for(; itr!=_sessions.end(); itr++)
	{
		MemInfo* info = itr->second;
		
		//检查这个session 订阅这只合约没有 
		map<string, set<SubBit> >::iterator it = info->_sub_bit.find(InstrumentId);
		if(it == info->_sub_bit.end())
		{
			//LOG_DEBUG("InstrumentId:" <<  InstrumentId << " not sub kline.")
			continue;
		}

		//遍历合约订阅位列表
		set<SubBit>::iterator itl =  it->second.begin();
		for(; itl!=it->second.end(); itl++)
		{
            //检查是否订阅了这个周期的数据
			if(itl->_type == "A" || (itl->_type=="M" && itl->_num==period))
			{
				try
				{
					info->_callback->begin_onBar(TradingDay, InstrumentId, kline, period);
					LOG_DEBUG("[" << itr->first << "]push kline InstrumentId:" << InstrumentId << " key:" << kline.Timestamp << " period=" << period);
				}
				catch(...)
				{
					LOG_ERROR("[" << itr->first << "]push kline fail." )
				}	
			}
			LOG_DEBUG("_type:" << itl->_type << " num=" << itl->_num << " period=" << period);
		}
	}
	return 0;
}

//push 日k 快照数据
int  SessionM::send_day_snap(const string TradingDay, const string InstrumentID , const CM::DayBar& day)
{
	Lock sync(*this);	
	
	map<string,  MemInfo*>::iterator  itr = _sessions.begin();
	for(; itr!=_sessions.end(); itr++)
	{
		MemInfo* info = itr->second;
			
		//检查这个session 订阅这只合约没有 
		map<string, set<SubBit> >::iterator it = info->_sub_bit.find(InstrumentID);
		if(it == info->_sub_bit.end())
		{
			//LOG_DEBUG("InstrumentId:" <<  InstrumentID << " not sub kline.")
			continue;
		}

		//遍历合约订阅位列表
		set<SubBit>::iterator itl =  it->second.begin();
		for(; itl!=it->second.end(); itl++)
		{
			if(itl->_type == "A" || itl->_type=="D")
			{
				try
				{
					info->_callback->begin_onDay(TradingDay, InstrumentID, day);
					LOG_DEBUG("[" << itr->first << "]push daybar InstrumentId:" << InstrumentID << " key:" << day.Timestamp);
				}
				catch(...)
				{
					LOG_ERROR("[" << itr->first << "]push daybar fail." )
				}	
			}
			//LOG_DEBUG("_type:" << itl->_type);
		}
	}
	return 0;
}


//push 深度市场数据
int  SessionM::send_depth_market_data(const CM::DepthMarketData& mdata)
{
	Lock sync(*this);	
	
	map<string,  MemInfo*>::iterator  itr = _sessions.begin();
	for(; itr!=_sessions.end(); itr++)
	{
		MemInfo* info = itr->second;
			
		//检查这个session 订阅这只合约没有 
		map<string, set<SubBit> >::iterator it = info->_sub_bit.find(mdata.InstrumentID);
		if(it == info->_sub_bit.end())
		{
			//LOG_DEBUG("InstrumentId:" <<  InstrumentID << " not sub kline.")
			continue;
		}

		//遍历合约订阅位列表
		set<SubBit>::iterator itl =  it->second.begin();
		for(; itl!=it->second.end(); itl++)
		{
			if(itl->_type == "DMD")
			{
				try
				{
					info->_callback->begin_onDepthMarketData(mdata);
					LOG_DEBUG("[" << itr->first << "]push DepthMarketData InstrumentId:" << mdata.InstrumentID);
				}
				catch(...)
				{
					LOG_ERROR("[" << itr->first << "]push DepthMarketData fail." )
				}	
			}
			//LOG_DEBUG("_type:" << itl->_type);
		}
	}
	return 0;
}




int  SessionM::Run(const char* pin, uint32_t len)
{
	IceUtil::Mutex::Lock lock(_mutex_mem);
	
	//return ::Run(pin, len);
	return 0;
}
string SessionM::query_ticker(string insid)
{
	IceUtil::Mutex::Lock lock(_mutex_mem);
	return ::query_ticker(insid);
}

string SessionM::query_kline(string insid)
{
	IceUtil::Mutex::Lock lock(_mutex_mem);
	return ::query_kline(insid);
}

string SessionM::query_snap(string insid)
{
	IceUtil::Mutex::Lock lock(_mutex_mem);
	return ::query_snap(insid);
}
		
		
		

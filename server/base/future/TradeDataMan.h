/////////////////////////////////////////////////////////////////////////
///@system 交易系统
///@company 慧网基金
///@file TradeDataM.h
///@brief 内存数据的处理 和 入库操作
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef	 __TRADE_DATA_MAN_H__
#define	 __TRADE_DATA_MAN_H__

#include "future/helper.h"
#include "ut_str.h"

//虚拟帐号的计算 根据done来计算account position
struct CMAccountData
{
	::CM::Account               account;      
	//key = exchange + instrument + direction
	map<string, ::CM::Position> position_map; 
	//key = tradid
	map<string, ::CM::Done>     done_map;     
	//tradid
	map<string, ::CM::Order>    order_map;   

	string get_position_key(::CM::Position& p)	{return p.instrumentid + p.direction + p.positiondate;}
	string get_position_key(string instrumentid, string direction, string positiondate="1"/*默认1今仓*/)	{return instrumentid + direction + positiondate;}
	string get_done_key(::CM::Done& d)	{return  d.instrumentid + d.tradeid;};
	//get for insert
    string get_order_key(::CM::Order& o) 
    {
        string k =  o.sessionid + o.instrumentid + o.privateno;
        if(o.orderid != "")
        {
            order_map.erase(k);
            k =  o.sessionid + o.instrumentid + o.orderid;
        }
        return k;
    
    };///  + o.orderid;};

    //get for get
    string get_order_key(string sessionid, string instrumentid, string privateno, string orderid) 
    {
        string k =  sessionid + instrumentid + privateno;
        if(orderid != "")
        {
            k =  sessionid + instrumentid + orderid;
        }
        return k;
    
    };///  + o.orderid;};
   
    void update_option_capital(map<string, CThostFtdcInstrumentField>& mapIns)
    {
        double capital = account.capital;
        account.capital += get_positioncosts(mapIns);
        LOG_DEBUG("capital before:" << capital << " after:" <<account.capital);
    }

    //获取所有期权的持仓花费总额 需要合约信息
    double get_positioncosts(map<string, CThostFtdcInstrumentField>& mapIns)
    {
        double positioncost = 0;
		map<string, ::CM::Position>::iterator itr = position_map.begin();
		for(; itr != position_map.end(); itr++)
		{
            map<string, CThostFtdcInstrumentField>::iterator itrs = mapIns.find(itr->second.instrumentid);
            if(mapIns.begin() == itrs) continue;

            if(itrs->second.ProductClass != '2') 
                continue;

            if(itr->second.direction == "long")
            {
			    positioncost += itr->second.positioncost;
            }
            else if(itr->second.direction == "short")
            {
			    positioncost -= itr->second.positioncost;
            }

            LOG_DEBUG("fundid:" << account.fundid << " instrumentid:" << itr->second.instrumentid << " positioncost:" << itr->second.positioncost << " positioncosts:" << positioncost );
		}
		return positioncost;
    }

	int updatedb(string _account,  string tradingday, int reqestid, bool wdoneorder)
	{
		account.requestid = toa(reqestid);
		db_update_account(&account);
		db_update_position(account.fundid, atoll(tradingday.c_str()), reqestid,  position_map);
		if(wdoneorder)
		{
			db_update_order(order_map);
			db_update_done(done_map);
		}
		return 0;
	};
	

};


#define ACCOUNTSET
//QUERY(STPositionInfo, "STPOSITION", position);
#define QUERY(STRUCTTYPE, dbname, key) do{\
	map<string, STRUCTTYPE>::iterator itr = key##_map.begin();\
	for(; itr!=key##_map.end(); itr++)\
	{\
		Json::Value rootp;\
		get_##STRUCTTYPE(rootp, &itr->second);\
		rootp["tradingday"]  = tradingday;\
		rootp["requestid"]   = toa(reqestid);\
		ACCOUNTSET\
		rootp["account"]	 = _account;\
		rootp["updatedate"]  = stamp_to_isodate(time(NULL));\
		_db.ChangeCollection(dbname);\
		_db.Insert(rootp.toStyledString());\
	}\
}while(0);


struct SunAccountData
{
	STAccountInfo account;
	//key = exchange + instrument + direction
	map<string, STPositionInfo> position_map;
	map<string, STDoneInfo>  done_map;
	map<string, STOrderInfo> order_map;

	string get_position_key(STPositionInfo& p){return string(p.sec_code) + ctos(p.PosiDirection);};
	string get_done_key(STDoneInfo& d) {return string(d.sec_code) + toa(d.trade_id);};
	string get_order_key(STOrderInfo& o) {return toa(o.session_id) + string(o.order.sec_code)  + toa(o.order.private_no);}; // + toa(o.order_no);};

	double get_commission()
	{
		double commission = 0;
		map<string, STPositionInfo>::iterator itr = position_map.begin();
		for(; itr != position_map.end(); itr++)
		{
			commission += itr->second.commission;
		}
		return commission;
	}
	
	double get_closeprofit()
	{
		double closeprofit = 0;
		map<string, STPositionInfo>::iterator itr = position_map.begin();
		for(; itr != position_map.end(); itr++)
		{
			closeprofit += itr->second.realized_profit;
		}
		return closeprofit;
	}

	int updatedb(string _account, string tradingday, int reqestid, bool wdoneorder=false)
	{
		CMongodb _db = CMongodb::Instance();

		Json::Value roota;
		get_STAccountInfo(roota, &account);
		roota["requestid"]   = toa(reqestid);   //推送默认0  查询则为查询id          
		roota["tradingday"]  = tradingday;
		roota["account"]	 = _account;
		roota["updatedate"]  = stamp_to_isodate(time(NULL));
		_db.ChangeCollection("STACCOUNT");
		_db.Insert(roota.toStyledString());

#ifdef ACCOUNTSET
	#undef ACCOUNTSET
	#define ACCOUNTSET rootp["cell_id"]     = account.cell_id;
#endif
		QUERY(STPositionInfo, "STPOSITION", position);
		if(wdoneorder)
		{
			QUERY(STDoneInfo,	  "STDONE",		done);
			QUERY(STOrderInfo,	  "STORDER",	order);
		}

		/*
		map<string, STPositionInfo>::iterator itr = position_map.begin();
		for(; itr!=position_map.end(); itr++)
		{
			Json::Value rootp;
			get_STPositionInfo(rootp, &itr->second);
			rootp["tradingday"]  = tradingday;
			rootp["requestid"]   = toa(reqestid); //统一两个请求id
			rootp["cell_id"]     = account.cell_id;
			rootp["account"]	 = _account;
			rootp["updatedate"]  = stamp_to_isodate(time(NULL));
			_db.ChangeCollection("STPOSITION");
			_db.Insert(rootp.toStyledString());
		}
		*/
		return 0;
	};
};

struct CtpAccountData
{
	CThostFtdcTradingAccountField				 account;      
	//key = exchange + instrument + direction
	map<string, CThostFtdcInvestorPositionField> position_map; 
	//key = tradid
	map<string, CThostFtdcTradeField>			 done_map;     
	//tradid
	map<string, CThostFtdcOrderField>			 order_map;   
    
    bool check(char* p, int len)
    {
        for(int i=0; i<len; i++)
        {
            if(p[i] != ' ') return true;
        }
        return  false;
    }
	string get_position_key(CThostFtdcInvestorPositionField& p) {return string(p.InstrumentID) + ctos(p.PosiDirection) + ctos(p.PositionDate);};
	string get_done_key(CThostFtdcTradeField& d) {return string(d.InstrumentID) + toa(d.TradeID);};
	string get_order_key(CThostFtdcOrderField& o) 
    {
        string k = toa(o.SessionID) + string(o.InstrumentID) + toa(o.OrderLocalID);
        if(check(o.OrderSysID, sizeof(o.OrderSysID)))
        {
            order_map.erase(k);
            k = toa(o.SessionID) + string(o.InstrumentID) + o.OrderSysID;
        }
        return k;
    }; 
	//get for get
    string get_order_key(string sessionid, string instrumentid, string privateno, string orderid) 
    {
        string k =  sessionid + instrumentid + privateno;
        if(orderid != "")
        {
            k =  sessionid + instrumentid + orderid;
        }
        return k;
    
    }
	int updatedb(string _account, string tradingday, int reqestid, bool wdoneorder=false)
	{
		CMongodb _db = CMongodb::Instance();
		
		Json::Value roota;                                                    
		get_CThostFtdcTradingAccountField(roota,  &account);
		roota["requestid"]   = toa(reqestid);   //推送默认0  查询则为查询id           
		roota["tradingday"]  = tradingday;
		roota["account"]	 = _account;
		roota["updatedate"]  = stamp_to_isodate(time(NULL));                   
		_db.ChangeCollection("CTPACCOUNT");                  
		_db.Insert(roota.toStyledString());                

#ifdef ACCOUNTSET
	#undef ACCOUNTSET
	#define ACCOUNTSET rootp["investorid"]  = account.AccountID;
#endif
		QUERY(CThostFtdcInvestorPositionField,	"CTPPOSITION",	position);
		if(wdoneorder)
		{
			QUERY(CThostFtdcTradeField,				"CTPDONE",		done);
			QUERY(CThostFtdcOrderField,				"CTPORDER",		order);
		}	

		/*	
		map<string, CThostFtdcInvestorPositionField>::iterator itr = position_map.begin();
		for(; itr!=position_map.end(); itr++)
		{
			Json::Value rootp;
			get_CThostFtdcInvestorPositionField(rootp, &itr->second); 
			rootp["tradingday"]  = tradingday;
			rootp["requestid"]   = toa(reqestid);
			rootp["investorid"]  = account.AccountID;
			rootp["account"]	 = _account;
			rootp["updatedate"]  = stamp_to_isodate(time(NULL));
			_db.ChangeCollection("CTPPOSITION");
			_db.Insert(rootp.toStyledString());
		}
		*/	
		return 0;
	}
};

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
class DataManageM
{
	public:
		DataManageM()
		{
			pthread_mutex_init(&_mutex, NULL);
		}
		int add_account(string fundid, TA&  _account);
		int add_position(string fundid, TP& _position);
		int add_done(string fundid, TD& _done);
		int add_order(string fundid, TO& _order);

		vector<TP> get_position_vector(string fundid);
		vector<TO> get_order_vector(string fundid);
		vector<TD> get_done_vector(string fundid);
		vector<TA> get_account_vector();
		vector<string> get_fundids();

		bool find_account(string fundid);

		TA get_account(string  fundid);
		map<string, TP> get_position_map(string fundid);

		TO get_order(string fundid, string instrument, string sessionid, string privateno, string order);

		void PrintInfo();

		string get_position_key(TP& p);
		string get_done_key(TD& d);
		string get_order_key(TO& o);

		void clear();
		void clear(string fundid);
		void updatedb(string _account, string tradingday, int reqid, bool wdoneorder=false);
		void updatedb(string fundid, string _account, string tradingday, int reqid, bool wdoneorder=false);

		void updatedb_tradingday(string tradingday);

		//这个是local结构用的
		void updateorder(string fundid);
		void updatedone(string fundid);
        void update_option_capital(map<string, CThostFtdcInstrumentField>& mapIns);   //更新期权部分的
        void update_option_capital(string fundid, map<string, CThostFtdcInstrumentField>& mapIns);   //更新期权部分的

		//sungard使用计算账户信息
		double get_commission(string fundid);
		double get_closeprofit(string fundid);
		
		int  update_commission_closeprofit(string fundid, double commission, double closeprofit);

		int update_balance(string fundid, double balance);
		//cell_id
		map<string, TALL> map_account_data;
		pthread_mutex_t _mutex;
};

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
void  DataManageM<TALL, TA, TP, TD, TO>::PrintInfo()
{
	pthread_mutex_lock(&_mutex);

	typeof(map_account_data.begin()) itr = map_account_data.begin();
	for(; itr != map_account_data.end(); itr++)
	{
		LOG_INFO("fundid=" << itr->first << " position=" << itr->second.position_map.size() << " done="<<itr->second.done_map.size() << " order=" << itr->second.order_map.size());
	}

	pthread_mutex_unlock(&_mutex);

}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
void  DataManageM<TALL, TA, TP, TD, TO>::clear()
{
	pthread_mutex_lock(&_mutex);

	typeof(map_account_data.begin()) itr = map_account_data.begin();
	for(; itr != map_account_data.end(); itr++)
	{
		//itr->account
		itr->second.position_map.clear();
		itr->second.done_map.clear();
		itr->second.order_map.clear();
	}

	pthread_mutex_unlock(&_mutex);
	
	LOG_DEBUG("clear all info.");
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
void  DataManageM<TALL, TA, TP, TD, TO>::clear(string fundid)
{
	pthread_mutex_lock(&_mutex);

	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
	{
		//itr->account
		itr->second.position_map.clear();
		itr->second.done_map.clear();
		itr->second.order_map.clear();
	}

	pthread_mutex_unlock(&_mutex);
	
	LOG_DEBUG("clear fundid=" << fundid << " info.");

}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
void  DataManageM<TALL, TA, TP, TD, TO>::updatedb(string _account, string tradingday,  int reqid, bool wdoneorder)
{
	pthread_mutex_lock(&_mutex);

	typeof(map_account_data.begin()) itr = map_account_data.begin();
	for(; itr != map_account_data.end(); itr++)
	{
		itr->second.updatedb(_account, tradingday, reqid, wdoneorder);
	}

	pthread_mutex_unlock(&_mutex);

}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
void DataManageM<TALL, TA, TP, TD, TO>::updatedb(string fundid, string _account, string tradingday, int reqid, bool wdoneorder)
{
	pthread_mutex_lock(&_mutex);

	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
		itr->second.updatedb(_account, tradingday, reqid, wdoneorder);

	pthread_mutex_unlock(&_mutex);

}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
void  DataManageM<TALL, TA, TP, TD, TO>::updatedb_tradingday(string tradingday)
{
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.begin();
	for(; itr != map_account_data.end(); itr++)
	{
		db_set_tradingday(itr->first, tradingday);
	}
	pthread_mutex_unlock(&_mutex);
	

}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
void DataManageM<TALL, TA, TP, TD, TO>::updateorder(string fundid)
{
	pthread_mutex_lock(&_mutex);

	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
		db_update_order(itr->second.order_map);

	pthread_mutex_unlock(&_mutex);

}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
void DataManageM<TALL, TA, TP, TD, TO>::updatedone(string fundid)
{
	pthread_mutex_lock(&_mutex);

	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
		db_update_done(itr->second.done_map);

	pthread_mutex_unlock(&_mutex);

}



template<typename TALL, typename TA, typename TP, typename TD, typename TO>
map<string, TP> DataManageM<TALL, TA, TP, TD, TO>::get_position_map(string fundid)
{
	map<string, TP> tp;
	pthread_mutex_lock(&_mutex);

	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
		tp = itr->second.position_map;

	pthread_mutex_unlock(&_mutex);

	return tp;
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
vector<string> DataManageM<TALL, TA, TP, TD, TO>::get_fundids()
{
	vector<string> fundids;
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.begin();
	for(; itr!=map_account_data.end(); itr++)
	{
		fundids.push_back(itr->first);
	}
	pthread_mutex_unlock(&_mutex);

	return fundids;
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
vector<TA> DataManageM<TALL, TA, TP, TD, TO>::get_account_vector()
{
	pthread_mutex_lock(&_mutex);

	vector<TA> al;
	typeof(map_account_data.begin()) itr = map_account_data.begin();
	for(; itr!=map_account_data.end(); itr++)
	{
		al.push_back(itr->second.account);
	}

	pthread_mutex_unlock(&_mutex);

	return al;
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
bool DataManageM<TALL, TA, TP, TD, TO>::find_account(string fundid)
{
	bool flag = false;
	pthread_mutex_lock(&_mutex);

	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
	{
		flag = true;
	}

	pthread_mutex_unlock(&_mutex);
	
	return flag;
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
TA DataManageM<TALL, TA, TP, TD, TO>::get_account(string fundid)
{
	pthread_mutex_lock(&_mutex);

	TA al;
	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
	{
		al = itr->second.account;
	}

	pthread_mutex_unlock(&_mutex);
	
	return al;
}


template<typename TALL, typename TA, typename TP, typename TD, typename TO>
vector<TP> DataManageM<TALL, TA, TP, TD, TO>::get_position_vector(string fundid)
{
	pthread_mutex_lock(&_mutex);

	vector<TP> pl;

	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr == map_account_data.end())
	{
		pthread_mutex_unlock(&_mutex);
		return pl;
	}

	map<string, TP>& position_map = itr->second.position_map;

	typeof(position_map.begin()) itrp = position_map.begin();
	for(; itrp!=position_map.end(); itrp++)
	{
		pl.push_back(itrp->second);
	}

	pthread_mutex_unlock(&_mutex);

	return pl;
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
vector<TO> DataManageM<TALL, TA, TP, TD, TO>::get_order_vector(string fundid)
{
	pthread_mutex_lock(&_mutex);
	vector<TO> ol;

	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr == map_account_data.end())
	{
		pthread_mutex_unlock(&_mutex);
		return ol; 
	}

	map<string, TO>& order_map = itr->second.order_map;

	typeof(order_map.begin()) itro = order_map.begin();
	for(; itro!=order_map.end(); itro++)
	{
		ol.push_back(itro->second);
	}
	pthread_mutex_unlock(&_mutex);
	return ol;
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
vector<TD> DataManageM<TALL, TA, TP, TD, TO>::get_done_vector(string fundid)
{
	pthread_mutex_lock(&_mutex);
	vector<TD> dl;

	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr == map_account_data.end())
	{
		pthread_mutex_unlock(&_mutex);
		return dl; 
	}

	map<string, TD>& done_map = itr->second.done_map;

	typeof(done_map.begin()) itrd = done_map.begin();
	for(; itrd!=done_map.end(); itrd++)
	{
		dl.push_back(itrd->second);
	}
	pthread_mutex_unlock(&_mutex);
	return dl;
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
TO DataManageM<TALL, TA, TP, TD, TO>::get_order(string fundid, string instrumentid, string sessionid, string privateno, string orderid)
{
	pthread_mutex_lock(&_mutex);

	TO to;

	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr == map_account_data.end())
	{
		pthread_mutex_unlock(&_mutex);
		return to; 
	}

	map<string, TO>& order_map = itr->second.order_map;

	string key = itr->second.get_order_key(sessionid , instrumentid, privateno, orderid);
	typeof(order_map.begin()) itro = order_map.find(key);
	if(itro != order_map.end())
	{
		to = itro->second;
	}
	pthread_mutex_unlock(&_mutex);

	return to;
}


template<typename TALL, typename TA, typename TP, typename TD, typename TO>
int DataManageM<TALL, TA, TP, TD, TO>::add_account(string fundid, TA&  _account)
{
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr == map_account_data.end())
	{
		TALL tmp;
		tmp.account = _account;
		map_account_data[fundid] = tmp;
	}
	else
	{
		itr->second.account = _account;
	}
	pthread_mutex_unlock(&_mutex);
	return 0;
}

	
template<typename TALL, typename TA, typename TP, typename TD, typename TO>
double  DataManageM<TALL, TA, TP, TD, TO>::get_commission(string fundid)
{
	double commission = 0;
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
	{
		commission = itr->second.get_commission();
	}
	pthread_mutex_unlock(&_mutex);
	return commission;
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
double  DataManageM<TALL, TA, TP, TD, TO>::get_closeprofit(string fundid)
{
	double closeprofit = 0;
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
	{
		closeprofit = itr->second.get_closeprofit();
	}
	pthread_mutex_unlock(&_mutex);
	return closeprofit;
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
void DataManageM<TALL, TA, TP, TD, TO>::update_option_capital(string fundid, map<string, CThostFtdcInstrumentField>& mapIns)
{
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
	{
        itr->second.update_option_capital(mapIns);
	}
	pthread_mutex_unlock(&_mutex);
}


template<typename TALL, typename TA, typename TP, typename TD, typename TO>
void DataManageM<TALL, TA, TP, TD, TO>::update_option_capital(map<string, CThostFtdcInstrumentField>& mapIns)
{
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.begin();
	for(; itr != map_account_data.end(); itr++)
	{
        itr->second.update_option_capital(mapIns);
	}
	pthread_mutex_unlock(&_mutex);
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
int  DataManageM<TALL, TA, TP, TD, TO>::update_commission_closeprofit(string fundid, double commission, double closeprofit)
{
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
	{
		itr->second.account.commission = commission;
		itr->second.account.closeprofit = closeprofit;
	}
	else
	{
		LOG_ERROR("fundid=" << fundid << " not in memory.");
	}
	pthread_mutex_unlock(&_mutex);
	return 0;
}

template<typename TALL, typename TA, typename TP, typename TD, typename TO>
int  DataManageM<TALL, TA, TP, TD, TO>::update_balance(string fundid, double positionprofit)
{
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr != map_account_data.end())
	{
		itr->second.account.balance = itr->second.account.capital + positionprofit;
	}
	else
	{
		LOG_ERROR("fundid=" << fundid << " not in memory.");
	}
	pthread_mutex_unlock(&_mutex);
	return 0;
}


template<typename TALL, typename TA, typename TP, typename TD, typename TO>
int DataManageM<TALL, TA, TP, TD, TO>::add_position(string fundid, TP& _position)
{
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr == map_account_data.end())
	{
		LOG_DEBUG("fundid = "<< fundid << " add position fail.");
		pthread_mutex_unlock(&_mutex);
		return -1;
	}

	map<string, TP>& p = itr->second.position_map;
	string key = itr->second.get_position_key(_position);
	p[key] = _position;
	pthread_mutex_unlock(&_mutex);
	return 0;
}
template<typename TALL, typename TA, typename TP, typename TD, typename TO>
int DataManageM<TALL, TA, TP, TD, TO>::add_done(string fundid, TD& _done)
{
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr == map_account_data.end())
	{
		LOG_DEBUG("fundid = "<< fundid << " add done fail.");
		pthread_mutex_unlock(&_mutex);
		return -1;
	}
	map<string, TD>& d = itr->second.done_map;
	string key = itr->second.get_done_key(_done);
	d[key] = _done;
	pthread_mutex_unlock(&_mutex);
	return 0;
}
template<typename TALL, typename TA, typename TP, typename TD, typename TO>
int DataManageM<TALL, TA, TP, TD, TO>::add_order(string fundid, TO& _order)
{
	pthread_mutex_lock(&_mutex);
	typeof(map_account_data.begin()) itr = map_account_data.find(fundid);
	if(itr == map_account_data.end())
	{
		LOG_DEBUG("fundid = "<< fundid << " add order fail.");
		pthread_mutex_unlock(&_mutex);
		return -1;
	}
	map<string, TO>& o = itr->second.order_map;
	string key = itr->second.get_order_key(_order);
	o[key] = _order;
	pthread_mutex_unlock(&_mutex);
	return 0;
}

/*
//记录初始数据的
int read_file(map<string, string>& map_key_value)
{
fstream in;
in.open(".init.dat", ios::in);
if(!in.is_open())
{
LOG_ERROR("open file fail.");
return -1;
}

char buf[1024] = {0};
while(in.getline(buf, sizeof(buf)))
{
vector<string> v;
ut_split(buf, v, "=");
if(v.size() < 2) continue;

eap_key_vanue[ v[0] ] = v[1];
LOG_DEBUG("file status " << v[0] << " = " << v[1])
}
in.close();
return 0;
}


int write_file(map<string, string>& map_key_value)
{
fstream out;
out.open(".init.dat", ios::out|ios::trunc);
if(!out.is_open())
{
LOG_ERROR("open file fail.");
return -1;
}

map<string, string>::iterator itr = map_key_value.begin();
for(; itr!=map_key_value.end(); itr++)
{
out << itr->first << "=" << itr->second << endl;
LOG_DEBUG("file status " << itr->first << " = " << itr->second);
}
out.close();
return 0;
}

*/

#endif






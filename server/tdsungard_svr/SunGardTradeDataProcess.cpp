/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file SunGardTradeDataProcess.cpp
///@brief 处理交易数据  写db push下游
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#include "SunGardTradeDataProcess.h"
#include <stdlib.h>
#include <stdio.h>

#define __SETTLEMENT_ID__ -9999

SunGardTradeDataProcess* g_syncsp = NULL;
		
DataManageM<SunAccountData, STAccountInfo, STPositionInfo, STDoneInfo, STOrderInfo> SunGardTradeDataProcess::sun_data;
		
DataManageM<CMAccountData, ::CM::Account, ::CM::Position, ::CM::Done, ::CM::Order> SunGardTradeDataProcess::local_data; 

map<string, double>  SunGardTradeDataProcess::prebalance;
map<string, double>  SunGardTradeDataProcess::premargin;


//计算子账号信息
void SunGardTradeDataProcess::update_sub_account()
{
	//清空并重新加载子账户初始信息TODO
	vector< ::CM::Account > a =  local_data.get_account_vector();
	for(typeof(a.begin()) itr=a.begin(); itr!=a.end(); itr++)
	{
		if(itr->parentid != "" && itr->parentid != itr->fundid)
		{
			local_data.clear(itr->fundid);
			//先加载虚拟的 
			Json::Value q, q2;
			q2["parentid"] = itr->parentid;
			LoadDb("FUND", tradingday, &SunGardTradeDataProcess::LoadVirtualFund, q2);
			
			//然后加载当前的覆盖掉
			q["fundid"]		= itr->fundid;                                                
			q["tradingday"] = toa(tradingday);          
			q["requestid"]	= toa(__SETTLEMENT_ID__);	//表示结算数据
			LoadDb("ACCOUNT", tradingday, &SunGardTradeDataProcess::LoadAccount, q);     
			LoadDb("POSITION", tradingday, &SunGardTradeDataProcess::LoadPosition, q);     
		}
	}

	//结算子账户
	a =  local_data.get_account_vector();
	for(typeof(a.begin()) itr=a.begin(); itr!=a.end(); itr++)
	{
		//遍历所有order信息表
		vector< ::CM::Order > o  = local_data.get_order_vector(itr->fundid);
		for(typeof(o.begin()) oitr=o.begin();  oitr!=o.end(); oitr++)
		{
			::CM::DoOrder doo;
			::CM::Order& order = *oitr;
			int ret = get_append_info(order.fundid, order.tradingday, order.requestid, doo);
			if(ret == 0 && doo.fundid != oitr->fundid)
			{
				order.parentid      = order.fundid;
				order.fundid		= doo.fundid;
				local_data.add_order(order.fundid, order); 
			}
		}
		//遍历所有的done信息表
		vector< ::CM::Done > d  = local_data.get_done_vector(itr->fundid);
		for(typeof(d.begin()) ditr=d.begin();  ditr!=d.end(); ditr++)
		{
			::CM::DoOrder doo;
			::CM::Done& done = *ditr;
			int ret = get_append_info(done.fundid, done.tradingday, done.requestid, doo);
			if(ret == 0 && doo.fundid != ditr->fundid)
			{
				done.parentid      = done.fundid;
				done.fundid		   = doo.fundid;
				local_data.add_done(done.fundid, done); 
				//计算持仓账户	
				int ret = local_data.update_position_account_by_done(done.fundid, g_syncsp->map_instruments, done);
				if(ret < 0)
				{
					LOG_ERROR("sub-account update_position_account_by_done error.");
					continue;
				}
			}
		}
	}
	for(typeof(a.begin()) itr=a.begin(); itr!=a.end(); itr++)
	{
		if(itr->parentid != "" && itr->parentid != itr->fundid)
		{
			//更新子账号数据入库
			local_data.updateorder(itr->fundid);
			local_data.updatedone(itr->fundid);
		}
	}

}

//子账户结算数据写数据库
void SunGardTradeDataProcess::update_sub_account_2db()
{
	vector< ::CM::Account > a =  local_data.get_account_vector();
	for(typeof(a.begin()) itr=a.begin(); itr!=a.end(); itr++)
	{
		if(itr->parentid != "" && itr->parentid != itr->fundid)
		{
			local_data.updatedb(itr->fundid, get_account(), toa(tradingday),  __SETTLEMENT_ID__);	//更新账户持仓
		}
	}
}



int SunGardTradeDataProcess::dbset(string key, string value)
{
	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("KEYVALUE");
	Json::Value q,r,s;
	q["key"] = key;

	r["key"] = key;
	r["value"] = value;
	s["$set"] = r;
	return _db.Update(s.toStyledString(), q.toStyledString(), true);
}

string SunGardTradeDataProcess::dbget(string key)
{
	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("KEYVALUE");
	Json::Value q;
	q["key"] = key;
	
	if(_db.Query(q.toStyledString()) < 0){LOG_ERROR("fail " << _db.GetLastErr());return "";};
	while(_db.Next())
	{
		try
		{	
			char* result = _db.GetResult();
			Json::Reader reader;
			Json::Value val;
			if(!reader.parse(result, val))  
			{
				LOG_ERROR("REQUESTMAP reader.parse error. result:" << result);
				return "";
			}
			return val["value"].asString();
		}
	    catch(...)
	    {
			 LOG_ERROR("REQUESTMAP get db.Next() fail.");
			 return "";
	    }
	}
	return "";
}

//金仕达的撤单没办法区分
int SunGardTradeDataProcess::get_append_info(string& fundid, string& tradingday, string& requestid, ::CM::DoOrder& order)
{
	Json::Value q;
	q["tradingday"] = tradingday;
	q["fundid"]		= fundid;
	q["requestid"]	= requestid;

	LOG_DEBUG("q=" << q.toStyledString());
	
	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("DOORDER");
	if(_db.Query(q.toStyledString()) < 0)
	{
		LOG_ERROR("fail " << _db.GetLastErr());
		return -1;
	}
	while(_db.Next())
	{
		try
		{	
			char* result = _db.GetResult();
			Json::Reader reader;
			Json::Value val;
			if(!reader.parse(result, val))  
			{
				LOG_ERROR("REQUESTMAP reader.parse error. result:" << result);
				return -1;
			}
			get_struct_doorder(&order, val);
			return 0;
		}
	    catch(...)
	    {
			 LOG_ERROR("REQUESTMAP get db.Next() fail.");
			 return -1;
	    }
	}
	return -1;
}

//这个里面没有请求id  私有号这里写的是id
void SunGardTradeDataProcess::OnErrOrder(void* p,  int req_no, int row_no)
{
	STOrderInfo *p_order_info = (STOrderInfo*)p;
	if(!local_data.find_account(toa(p_order_info->order.cell_id ))){ LOG_DEBUG("not load fundid=" <<  toa(p_order_info->order.cell_id)); return ; }

	string insname = p_order_info->err_msg;
	GBK2UTF8((char*)insname.c_str(), insname.size(), p_order_info->err_msg, sizeof(p_order_info->err_msg));	
	
	insname = p_order_info->cancel_err_msg;
	GBK2UTF8((char*)insname.c_str(), insname.size(), p_order_info->cancel_err_msg, sizeof(p_order_info->cancel_err_msg)); 

	if(p_order_info->order.cell_id == 0) p_order_info->order.cell_id = atoll(g_syncsp->get_account().c_str());

	Json::Value root;
	root = get_STOrderInfo(root, p_order_info);
	root["requestid"]	= p_order_info->order.private_no;	//推送默认0  查询则为查询id
	root["type"]		= req_no;	//委托单类型
	root["tradingday"]	= toa(tradingday);
	root["updatedate"] = stamp_to_isodate(time(NULL));
	root["account"]	    = g_syncsp->get_account();

	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);

	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("STORDER");
	_db.Insert(root.toStyledString());
	
	struct CM::Order     order;
	order.tradingday = toa(tradingday);
	order.requestid  = toa(p_order_info->order.private_no);
	convert_st_order(&order, p_order_info);
	//获取附加信息
	::CM::DoOrder doo;
	int ret = g_syncsp->get_append_info(order.fundid, order.tradingday, order.requestid, doo);
	if(ret == 0)
	{
		order.parentid		= "";
		order.seq			= doo.seq;
		order.strategyid	= doo.strategyid; 
		order.userid		= doo.userid;
		order.signalname	= doo.signalname;
	}
	local_data.add_order(order.fundid, order);
	sun_data.add_order(toa(p_order_info->order.cell_id), *p_order_info);
	SessionM::Instance().send_order(order.fundid,  order);	
	
	local_data.updateorder(order.fundid);

	//更新子账号信息 并推送
	if(ret == 0 && doo.fundid != order.fundid)
	{
		order.parentid      = order.fundid;
		order.fundid		= doo.fundid;
		local_data.add_order(order.fundid, order); 
		SessionM::Instance().send_order(order.fundid, order);	
		local_data.updateorder(order.fundid);
	}

}

//委托推送处理
void SunGardTradeDataProcess::OnOrder(void* p,  int req_no, int row_no)
{
	STOrderInfo *p_order_info = (STOrderInfo*)p;
	
	if(!local_data.find_account(toa(p_order_info->order.cell_id ))){ LOG_DEBUG("not load fundid=" <<  toa(p_order_info->order.cell_id)); return ; }

	string insname = p_order_info->err_msg;
	GBK2UTF8((char*)insname.c_str(), insname.size(), p_order_info->err_msg, sizeof(p_order_info->err_msg));	
	
	insname = p_order_info->cancel_err_msg;
	GBK2UTF8((char*)insname.c_str(), insname.size(), p_order_info->cancel_err_msg, sizeof(p_order_info->cancel_err_msg)); 

	Json::Value root;
	root = get_STOrderInfo(root, p_order_info);
	root["requestid"]	= p_order_info->order.private_no;	//推送默认0  查询则为查询id
	root["type"]		= req_no;	//委托单类型
	root["tradingday"]	= toa(tradingday);
	root["updatedate"] = stamp_to_isodate(time(NULL));
	root["account"]	    = g_syncsp->get_account();
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);

	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("STORDER");
	_db.Insert(root.toStyledString());

	struct CM::Order     order;
	order.tradingday = toa(tradingday);
	order.requestid  = toa(p_order_info->order.private_no);
	convert_st_order(&order, p_order_info);
	//获取附加信息
	::CM::DoOrder doo;
	int ret = g_syncsp->get_append_info(order.fundid, order.tradingday, order.requestid, doo);
	if(ret == 0)
	{
		order.parentid		= "";
		order.seq			= doo.seq;
		order.strategyid	= doo.strategyid; 
		order.userid		= doo.userid;
		order.signalname	= doo.signalname;
	}
	local_data.add_order(order.fundid, order);
	sun_data.add_order(toa(p_order_info->order.cell_id), *p_order_info);
	SessionM::Instance().send_order(order.fundid, order);	
	
	local_data.updateorder(order.fundid);

	//更新子账号信息 并推送
	if(ret == 0 && doo.fundid != order.fundid)
	{
		order.parentid      = order.fundid;
		order.fundid		= doo.fundid;
		local_data.add_order(order.fundid, order); 
		SessionM::Instance().send_order(order.fundid, order);	
		local_data.updateorder(order.fundid);
	}
}
//成交推送
void SunGardTradeDataProcess::OnDone(void* p,  int req_no, int row_no)
{
	STDoneInfo *p_done_info = (STDoneInfo*)p;
	if(!local_data.find_account(toa(p_done_info->cell_id))){ LOG_DEBUG("not load fundid=" <<  toa(p_done_info->cell_id)); return ; }
	
	//LOG_DEBUG("cellid=" << p_done_info->cell_id);
	
	Json::Value root;
	root = get_STDoneInfo(root, p_done_info);
	root["requestid"]	= req_no;
	root["tradingday"]	= toa(tradingday);
	root["updatedate"] = stamp_to_isodate(time(NULL));
	root["account"]	    = g_syncsp->get_account();
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);

	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("STDONE");
	_db.Insert(root.toStyledString());
	
	struct CM::Done     done;
	done.tradingday = toa(tradingday);
	convert_st_done(&done, p_done_info);
	::CM::DoOrder doo;
	int ret = g_syncsp->get_append_info(done.fundid, done.tradingday, done.requestid, doo);
	if(ret == 0)
	{
		done.parentid	= "";
		done.seq		= doo.seq;
		done.strategyid	= doo.strategyid; 
		done.userid		= doo.userid;
		done.signalname	= doo.signalname;
	}
	local_data.add_done(done.fundid, done);
	sun_data.add_done(toa(p_done_info->cell_id), *p_done_info);
	local_data.updatedone(done.fundid);

	//更新子账号信息 并推送
	while(ret == 0 && doo.fundid != done.fundid)
	{
		done.parentid      = done.fundid;
		done.fundid		   = doo.fundid;
		g_syncsp->local_data.add_done(done.fundid, done); 
		local_data.updatedone(done.fundid);
		
		//计算账户持仓 push
		int ret = g_syncsp->local_data.update_position_account_by_done(done.fundid, g_syncsp->map_instruments, done);
		if(ret < 0)
		{
			LOG_ERROR("sub-account update_position_account_by_done error.");
			done.fundid = done.parentid; done.parentid = "";
			break;
		}
		
		SessionM::Instance().send_done(done.fundid, done, g_syncsp->local_data.get_account(done.fundid), g_syncsp->local_data.get_position_vector(done.fundid));
		
		g_syncsp->local_data.updatedb(done.fundid, g_syncsp->get_account(), done.tradingday, 0);
		done.fundid = done.parentid; done.parentid = "";
		break;
	}

	//查询持仓 查询账户信息
	msleep(10);
	int reta = g_syncsp->DoQueryAccount();
	int retp = g_syncsp->DoQueryPosition();
	LOG_DEBUG("reta=" << reta << " retp=" << retp);
	int i = 0;
	while(g_syncsp->WaitReq(reta, 1000)<0 || g_syncsp->WaitReq(retp, 1000) < 0)
	{
		if(i++ > 5) return ;
		LOG_ERROR("recv done to query account position fail");
	}
	
	SessionM::Instance().send_done(done.fundid, done, local_data.get_account(done.fundid), local_data.get_position_vector(done.fundid));

	//用户原始数据更新内部数据
	local_data.updatedb(done.fundid, g_syncsp->get_account(), toa(tradingday),	 reta);
	sun_data.updatedb(done.fundid, g_syncsp->get_account(), toa(tradingday),	 reta);
}


/********************查询有req_no   row_no的概念***********************************/
void SunGardTradeDataProcess::OnQueryOrder(void* p,  int req_no, int row_no)
{
	STOrderInfo *p_order_info = (STOrderInfo*)p;
	if(!local_data.find_account(toa(p_order_info->order.cell_id))){ LOG_DEBUG("not load fundid=" <<  toa(p_order_info->order.cell_id)); return ; }

	sun_data.add_order(toa(p_order_info->order.cell_id), *p_order_info);

	struct CM::Order     order;                      
	order.tradingday = toa(tradingday);             
	//里面是下单的requestid
	convert_st_order(&order, p_order_info);           
	::CM::DoOrder doo;
	int ret = g_syncsp->get_append_info(order.fundid, order.tradingday, order.requestid, doo);
	if(ret == 0)
	{
		order.parentid		= "";
		order.seq			= doo.seq;
		order.strategyid	= doo.strategyid; 
		order.userid		= doo.userid;
		order.signalname	= doo.signalname;
	}
	
	local_data.add_order(order.fundid, order);
	
	LOG_DEBUG("fundid=" << p_order_info->order.cell_id);
}


void SunGardTradeDataProcess::OnQueryDone(void* p,  int req_no, int row_no)
{
	STDoneInfo *p_done_info = (STDoneInfo*)p;
	if(!local_data.find_account(toa(p_done_info->cell_id))){ LOG_DEBUG("not load fundid=" <<  toa(p_done_info->cell_id)); return ; }
	
	sun_data.add_done(toa(p_done_info->cell_id), *p_done_info);

	struct CM::Done     done;                      
	done.tradingday = toa(tradingday);             
	done.requestid	= toa(req_no); 
	convert_st_done(&done, p_done_info);           
	
	::CM::DoOrder doo;
	int ret = g_syncsp->get_append_info(done.fundid, done.tradingday, done.requestid, doo);
	if(ret == 0)
	{
		done.parentid	= "";
		done.seq		= doo.seq;
		done.strategyid	= doo.strategyid; 
		done.userid		= doo.userid;
		done.signalname	= doo.signalname;
	}
	
	local_data.add_done(done.fundid, done);

	LOG_DEBUG("fundid=" << p_done_info->cell_id);
}

void SunGardTradeDataProcess::OnQueryAccount(void* p,  int req_no, int row_no)
{
	STAccountInfo *p_account_info = (STAccountInfo*)p;

	//< 资金账户类型(0-储蓄类,1-证券A股类,2-期货类,3-个股期权)
	if(p_account_info->acc_type != 2) return;
	
	//组合标志,0-资产单元,1-投资组合)
	if(p_account_info->account_type != 0) return;

	//略过不处理的账户
	if(!local_data.find_account(toa(p_account_info->cell_id)))
	{
		LOG_INFO("not load account=" << toa(p_account_info->cell_id));
		return ;
	}
	
	sun_data.add_account(toa(p_account_info->cell_id), *p_account_info);
	
	struct CM::Account  account;                  
	account.tradingday = toa(tradingday);         
	account.requestid  = toa(req_no);             
	convert_st_account(&account, p_account_info); 
	if(prebalance.find(account.fundid) != prebalance.end())
	{
		account.prebalance = prebalance[account.fundid]; 
		account.premargin  = premargin[account.fundid];
	}

	local_data.add_account(account.fundid, account);
	local_data.update_balance(account.fundid, sun_data.get_commission(account.fundid));
	
	LOG_DEBUG("fundid=" << p_account_info->cell_id);
}

void SunGardTradeDataProcess::OnQueryPosition(void* p,  int req_no, int row_no)	 	
{
	STPositionInfo *p_position_info = (STPositionInfo*)p;
	if(!local_data.find_account(toa(p_position_info->cell_id))){ LOG_DEBUG("not load fundid=" <<  toa(p_position_info->cell_id)); return ; }
	
	//组合标志,0-资产单元,1-投资组合)
	if(p_position_info->account_type != 0) return;
	
	sun_data.add_position(toa(p_position_info->cell_id), *p_position_info);
	
	struct CM::Position  position;                    
	position.tradingday = toa(tradingday);            
	position.requestid = toa(req_no);                 
	convert_st_position(&position, p_position_info);  

	local_data.add_position(position.fundid, position);
	local_data.update_commission_closeprofit(position.fundid, sun_data.get_commission(position.fundid), sun_data.get_closeprofit(position.fundid));
	local_data.update_balance(position.fundid, sun_data.get_commission(position.fundid));

	LOG_DEBUG("fundid=" << p_position_info->cell_id);
}

string SunGardTradeDataProcess::queryAccount(int from)
{
	int ret =	g_syncsp->DoQueryAccount();
	if(g_syncsp->WaitReq(ret, 3000) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return "{\"erorr\":\"query account fail.\"}";
	}

	vector<STAccountInfo> accounts =  sun_data.get_account_vector();
	Json::Value root;
	typeof(accounts.begin()) itr = accounts.begin();
	for(; itr!=accounts.end(); itr++)
	{
		Json::Value item;
		if(from != 1)
			get_STAccountInfo(item, &(*itr));
		else
		{
			::CM::Account account;
			convert_st_account(&account, &(*itr));
			if(prebalance.find(account.fundid) != prebalance.end())
			{
				account.prebalance = prebalance[account.fundid]; 
				account.premargin  = premargin[account.fundid];
			}

			get_json_account(&account, item);
		}
		root.append(item);
	}
	
	return root.toStyledString();

}

string SunGardTradeDataProcess::queryAccountOne(int from, string fundid)
{
	int ret =	g_syncsp->DoQueryAccount();
	if(g_syncsp->WaitReq(ret, 3000) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return "{\"erorr\":\"query account fail.\"}";
	}

	STAccountInfo account =  sun_data.get_account(fundid);
	Json::Value root;
	if(from != 1)
		get_STAccountInfo(root, &account);
	else
	{
		::CM::Account a;
		convert_st_account(&a, &account);
			
		if(prebalance.find(a.fundid) != prebalance.end())
		{
			a.prebalance = prebalance[fundid]; 
			a.premargin  = premargin[fundid];
		}
		get_json_account(&a, root);
	}
	
	return root.toStyledString();

}
string SunGardTradeDataProcess::queryDone(int from, string fundid)
{
	int ret =	g_syncsp->DoQueryTrade();
	if(g_syncsp->WaitReq(ret, 3000) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return "{\"erorr\":\"query done fail.\"}";
	}
	
	vector<STDoneInfo> dones =  sun_data.get_done_vector(fundid);
	Json::Value root;
	typeof(dones.begin()) itr = dones.begin();
	for(; itr!=dones.end(); itr++)
	{
		Json::Value item;
		if(from !=1)
		  get_STDoneInfo(item, &(*itr));
		else
		{
			::CM::Done done;
			convert_st_done(&done, &(*itr));
			get_json_done(&done, item);
		}
		root.append(item);
	}
	return root.toStyledString();


}
string SunGardTradeDataProcess::queryPosition(int from, string fundid)
{
	int ret = DoQueryPosition();
	if(WaitReq(ret, 3000) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return "{\"erorr\":\"query position fail.\"}";
	}
	
	sun_data.PrintInfo();
	local_data.PrintInfo();

	vector<STPositionInfo> positions =  sun_data.get_position_vector(fundid);
	LOG_DEBUG("fundid=" << fundid << " positions.size()=" << positions.size());
	
	Json::Value root;
	typeof(positions.begin()) itr = positions.begin();
	for(; itr!=positions.end(); itr++)
	{
		Json::Value item;
		if(from != 1)
			get_STPositionInfo(item, &(*itr));
		else
		{
			::CM::Position position;
			convert_st_position(&position, &(*itr));
			get_json_position(&position, item);
		}
		root.append(item);
	}
	return root.toStyledString();


}

string SunGardTradeDataProcess::queryOrder(int from, string fundid)
{
	int ret =	DoQueryOrder();
	if(WaitReq(ret, 3000) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return "{\"erorr\":\"query order fail.\"}";
	}
	
	vector<STOrderInfo> orders =  sun_data.get_order_vector(fundid);
	Json::Value root;
	typeof(orders.begin()) itr = orders.begin();
	for(; itr!=orders.end(); itr++)
	{
		Json::Value item;
		if(from != 1)
			get_STOrderInfo(item, &(*itr));
		else
		{
			::CM::Order order;
			convert_st_order(&order, &(*itr));
			get_json_order(&order, item);
		}
		root.append(item);
	}
	return root.toStyledString();
}

int SunGardTradeDataProcess::updatePassword(string oldpwd, string newpwd)
{
	//金仕达不支持
	return 0;
}

int SunGardTradeDataProcess::DoOrder(const ::CM::DoOrder& order)
{
	order.tradingday = toa(tradingday);
	
	Json::Value root;                                        
	get_json_doorder((::CM::DoOrder*)&order, root);          
	LOG_DEBUG("DoOrder param " << root.toStyledString());    
	
	STOrder orders = {0};
	snprintf(orders.sec_code, sizeof(orders.sec_code), "%s", order.instrumentid.c_str());
	orders.cell_id		= atoll(order.fundid.c_str());			// 投资单元(使用默认单元)
	orders.portfolio_id = atoll(order.fundid.c_str()) + 1;								// 投资组合(使用默认组合)
	orders.market_code	= my_marketcode_to_st(order.exchangeid);// 市场代码
	orders.hedge_flag	= my_hedgeflag_to_st(order.hedgeflag);  // 套保
	orders.bs			= my_direction_to_st(order.direction);	// 多空
	orders.market_order_flag = '0';								// ‘0’限价 '1'市价 ‘2’最优价
	orders.price		= order.price;							// 委托价格
	orders.order_vol	= order.volume;							// 委托数量
	orders.TradeType	= '1';									//固定1代表期货
	orders.offset_flag	= my_offsetflag_to_st(order.offsetflag);//开平仓

	//'0' GFD当日有效   '1' FOK全成或全撤   '2' FAK剩余即撤销    '3' IOC即时成交剩余自动撤销   '4' JFOK剩余即撤销   '5' 剩余转限价   只有0 2  3有效 
	orders.doneprop = order.donetype.size()>0 ? order.donetype[0]: '0'; //有0 2  3有效 
	
	//'0' 限价  '1' 市价   '2' 最优价  '3' 市价止损  
	string ordertype = order.ordertype;
	if(ordertype == "0")
	{
		orders.market_order_flag = '1';
	}
	else if(ordertype == "1")
	{
		orders.market_order_flag = '0';
		if(order.donetype == "0")
		{
			orders.doneprop ='0';	//'0' GFD当日有效
		}
		else if(order.donetype == "1")
		{
			orders.doneprop ='1';	//'1' FOK全成或全撤
		}
		else if(order.donetype == "3" || order.donetype == "2")
		{
			orders.doneprop ='2'; //'2' FAK剩余即撤销  =  '3' IOC即时成交剩余自动撤销
		}
		else
		{
			return -1;
		}
	}
	else if(ordertype=="2")
	{
		orders.market_order_flag = '2';
	}
	else
	{
		return -1;
	}

    //A 普通委托   B 批量委托   D 预埋普通委托 
	orders.OrderType = 'A';
		 
	int ret = CKFTSTraderSpiI::DoOrder(&orders, 1);
	//if(ret != 0)
	//{
	//	LOG_ERROR("ret=" << ret);
	//}

	root["msg"]         = "下单";
	root["updatedate"] = stamp_to_isodate(time(NULL));
	root["requestid"] = toa(ret);
	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("DOORDER");
	_db.Insert(root.toStyledString());
	
	stringstream ss;			
	ss<<"private_no="		  << orders.private_no	         << ", ";
	ss<<"cell_id="            << orders.cell_id              << ", ";
	ss<<"portfolio_id="       << orders.portfolio_id         << ", ";
	ss<<"market_code="        << orders.market_code          << ", ";
	ss<<"hedge_flag="         << orders.hedge_flag           << ", ";
	ss<<"bs="                 << orders.bs                   << ", ";
	ss<<"market_order_flag="  << orders.market_order_flag    << ", ";   
	ss<<"price="              << orders.price                << ", ";
	ss<<"order_vol="          << orders.order_vol            << ", ";
	ss<<"TradeType="          << orders.TradeType            << ", ";
	ss<<"offset_flag="        << orders.offset_flag          << ", ";           
	ss<<"market_order_flag="  << orders.market_order_flag	 << ", ";
	ss<<"doneprop="			  << orders.doneprop			 << ", ";
	ss<<"OrderType="		  << orders.OrderType ;
	
	LOG_DEBUG(ss.str());

	return ret;
}

int SunGardTradeDataProcess::CancleOrder(string fundid, string sessionid, string instrumentid, string privateno, string orderid)
{
	::CM::Order order = local_data.get_order(fundid, instrumentid, sessionid, privateno, orderid);
	if(order.fundid != fundid)
	{
		LOG_ERROR("[fundid="<< fundid << " instrumentid=" << instrumentid  << " privateno=" << privateno << " orderid=" << orderid << "]order not in  memory.");
		//LOG_ERROR("[fundid="<< order.fundid << " instrumentid=" << order.instrumentid  << " privateno=" << order.privateno << " orderid=" << order.orderid << "] in  memory.");
		return -1;
	}

	STInputOrderAction cancel = {0};
    cancel.TradeType		= '1';	//1固定代表期货
    cancel.MarketCode		= my_marketcode_to_st(order.exchangeid);                
    cancel.OrderLocalID		= atoll(order.orderid.c_str());        
    cancel.SessionID		= atoll(order.sessionid.c_str());
    cancel.PrivateNo		= atoll(order.privateno.c_str());
						  
	int ret = CKFTSTraderSpiI::DoOrderCancel(&cancel);
	//if(ret != 0)
    //{
	//	LOG_ERROR("ret=" << ret);
	//}							
	
	Json::Value root;
	root["fundid"]      = order.fundid; 
	root["brokerid"]    = order.brokerid;
	root["instrumentid"]= order.instrumentid;
	root["ordertype"]   = "6";  //场外撤单
	root["exchangeid"]  = cancel.MarketCode;
	root["privateno"]   = order.privateno;
	root["orderid"]     = order.orderid;
	root["msg"]         = "撤单";
	root["updatedate"] = stamp_to_isodate(time(NULL));
	root["requestid"] = toa(ret);
	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("DOORDER");
	_db.Insert(root.toStyledString());
	
	return ret;				
}

void SunGardTradeDataProcess::Run()	 	
{
	static bool init = false;
	//连接状态的改变
	if(init != get_InitFlag())
	{
		LOG_INFO("old conn_flag=" << init << " new conn_flag=" << get_InitFlag());
		init = get_InitFlag();
	}

	if(!get_InitFlag() || !g_holidays.IsOpenDay(time(NULL)) || tradingday < 20160101) return;
		
	//交易日的切换 触发一次初始化结算 
	static uint32_t td = 0;
	if(td != tradingday)
	{
		local_data.clear();
		sun_data.clear();
		syncreq.clear();
		map_daybar.clear();

		int reta = DoQueryAccount(); sleep(1);
		int retp = DoQueryPosition(); sleep(1);
		int ret1 = DoQueryOrder(); sleep(1);
		int ret2 = DoQueryTrade(); 
		if((g_syncsp->WaitReq(reta)|g_syncsp->WaitReq(retp)|g_syncsp->WaitReq(ret1)|g_syncsp->WaitReq(ret2)) < 0)
		{
			LOG_ERROR("tradingday change  query account position fail");
			return;
		}
	
		//if(td != 0) local_data.updatedb(get_account(), toa(tradingday),	 reta);
		//if(td != 0) sun_data.updatedb(get_account(), toa(tradingday),	reta);
	 
		 //local_data.updatedb(get_account(), toa(tradingday),	reta, true);
		 //sun_data.updatedb(get_account(), toa(tradingday),	 reta, true);
	 

		//重启计算一遍子账号信息 通过母账号来做
		if(td == 0) update_sub_account();	

		//非重启每天重置
		if(td != 0)  dbset("settlement", "0");
		if(td != 0) dbset("tradingday", toa(tradingday));  
		
		vector<string> fundidlist = local_data.get_fundids();
		for(vector<string>::iterator itr=fundidlist.begin(); itr!=fundidlist.end(); itr++)
		{
			if(td == 0)
			{
				SessionM::Instance().send_tradingday(*itr, toa(tradingday));
				LOG_INFO("fundid=" << *itr << " tradingday=" << tradingday);
			}
		}

		LOG_INFO("old  tradingday=" << td << " new tradingday=" << tradingday << " requestid=" << reta);
		td = tradingday;
	}

	//获取数据库的tradingday
	vector<string> fundidlist = local_data.get_fundids();
	for(vector<string>::iterator itr=fundidlist.begin(); itr!=fundidlist.end(); itr++)
	{
		int current_tradingday = db_get_tradingday(*itr);
		if(current_tradingday != (int)tradingday && db_set_tradingday(*itr, tradingday)==0)
		{
			::CM::Account cma = local_data.get_account(*itr);
			prebalance[cma.fundid] = cma.balance; 
			premargin[cma.fundid] =  cma.margin;
			cma.prebalance = cma.balance;
			cma.premargin = cma.margin;
			local_data.add_account(*itr, cma);
			LOG_INFO("update  account=" <<  (*itr) << " prebalance=" << cma.prebalance << " premargin=" << cma.premargin << " positionprofit(need 0)=" << sun_data.get_commission(*itr));
		
		
			//记录当天第一个初始数据
			local_data.updatedb(*itr, get_account(), toa(tradingday),	0, true);
			sun_data.updatedb(*itr, get_account(), toa(tradingday),	 0, true);

			SessionM::Instance().send_tradingday(*itr, toa(tradingday));
			LOG_INFO("fundid=" << *itr << " db_tradingday=" << current_tradingday << " tradingday=" << tradingday);
		}
	}

	
	return;
	//结算子账户
	string settle = dbget("settlement");
	int hour = get_hour();
	if(hour == 16  &&  settle != "1")
	{
		settlement(); //可以作为下一天的初始信息
	}

}

//结算子账户
int SunGardTradeDataProcess::settlement()
{
	//查询order done
	int ret1	= DoQueryTrade();;
	int ret2	= DoQueryOrder();
	if((WaitReq(ret1)|WaitReq(ret2)) < 0)
	{
		LOG_ERROR("tradingday change to query accunt position done order instrument product fail."); 
		return -1;	
	}

	Json::Value q;                                            
	q["tradingday"] = toa(tradingday);                                  
	LoadDb("DAYBAR", tradingday, &SunGardTradeDataProcess::LoadDayBar, q);
	if(map_daybar.size() < 200)
	{
		LOG_ERROR("map_daybar.size()=" << map_daybar.size() << " error.");
		return -1;
	}

	//计算子账户
	update_sub_account();	
	
	int ret = local_data.settlement_all(map_instruments, map_daybar);
	if(ret == 0)
	{
		CMongodb _db = CMongodb::Instance();
		_db.ChangeCollection("KEYVALUE");
		dbset("settlement", "1");
	
		//子账户初始化数据写数据库
		update_sub_account_2db();	
	}
	return 0;
}

//线程主函数 其实这里用libev的事件处理机制最好。
void* thread_run(void* p)
{
	SunGardTradeDataProcess* st = (SunGardTradeDataProcess*)p;
	//主处理线程
	//enum MarketStatus{INTI, SETTLE, SETTLE_OVER, };
	while(true)
	{
		st->Run();
		sleep(30);
	}
	return NULL;
}

void SunGardTradeDataProcess::Init()
{
	//查询节假日表mongo
	g_holidays.Init(CMongodb::Instance());

	//标识他是金仕达帐号
	account_type = "sungard";

	//先去数据库查询
	uint32_t today_trade = atoll(stamptostr(g_holidays.GetTradingDay(), "%Y%m%d").c_str());
	tradingday = today_trade;
		
	Json::Value q, q1, q2;
	q1["trading.account"] = get_account();
	q1["state"] = "online";
	LoadDb("FUND", today_trade, &SunGardTradeDataProcess::LoadFund, q1);
	for(uint32_t i=0; i<vecfundid.size(); i++)
	{
		Json::Value o1,o2,a;
		
		o1["fundid"]		= vecfundid[i];
		a.append(o1);

		o2["parentid"]	= vecfundid[i];
		a.append(o2);
		
		q2["$or"] = a;
		
		LoadDb("FUND", today_trade, &SunGardTradeDataProcess::LoadVirtualFund, q2);
	}

	//没有加载到基金账号这里直接返回失败
	if(m_fundid_cellid.size() < 1)
	{
	    LOG_ERROR("db not load fund account.");
	    exit(-1);
	}
	
	//按账号加载数据 到内存的时候按fundid来划分 会覆盖上面的第一次初始化
	q["account"] = get_account();
	q["tradingday"] = toa(tradingday);
	LoadDb("STACCOUNT", today_trade, &SunGardTradeDataProcess::LoadAccount, q);
	LoadDb("STPOSITION", today_trade, &SunGardTradeDataProcess::LoadPosition, q);
	LoadDb("STDONE", today_trade, &SunGardTradeDataProcess::LoadDone, q);
	LoadDb("STORDER", today_trade, &SunGardTradeDataProcess::LoadOrder, q);

	Set("OnErrOrder", OnErrOrder);
	Set("OnOrder", OnOrder);
	Set("OnDone", OnDone);
	//Set("OnQuerySecCodeRsp", OnQuerySecCodeRsp);
	Set("OnQueryOrder", OnQueryOrder);
	//Set("OnQueryEtfBaseInfo", OnQueryEtfBaseInfo);
	//Set("OnQueryEtfBasketInfo", OnQueryEtfBasketInfo);
	Set("OnQueryDone", OnQueryDone);
	Set("OnQueryAccount", OnQueryAccount);
	Set("OnQueryPosition", OnQueryPosition);
	CKFTSTraderSpiI::Init_Thread();
	if(!get_InitFlag()) msleep(500);

	//交易所的ok优先用交易所的
	if(tradingday < 20160101) tradingday = today_trade;

	LOG_INFO("g_holidays.GetTradingDay=" << today_trade <<  " tradingday=" << tradingday);
	
	LOG_DEBUG("new thread for settle check.");
	pthread_t pthread;
	pthread_create(&pthread, NULL, thread_run, this);
}



void SunGardTradeDataProcess::LoadDb(const char* table, uint32_t tradingday, _functiontype callback, Json::Value& q)
{
	CMongodb db = CMongodb::Instance();
	db.ChangeCollection(table);

	LOG_DEBUG("table=" << table << " sql=" << q.toStyledString());

	if(db.Query(q.toStyledString()) < 0)
	{
		LOG_ERROR("fail " << db.GetLastErr());
		exit(-1);
	}
	while(db.Next())
	{
		try
		{	
			char* result = db.GetResult();
			Json::Reader reader;
			Json::Value val;
			if(!reader.parse(result, val))  
			{
				LOG_ERROR("reader.parse error. result:" << result);
				continue;
			}
			(this->*callback)(val);
		}
	    catch(...)
	    {
			 LOG_ERROR("db.Next() fail." << db.GetLastErr());
	    }
	} 
}

void SunGardTradeDataProcess::LoadFund(Json::Value& root)
{
	if(!root["fundid"].isNull())
	{
		string fundid = root["fundid"].asString();
		vecfundid.push_back(fundid);
		LOG_INFO("init a fundid=" << fundid);
	}

}

void SunGardTradeDataProcess::LoadVirtualFund(Json::Value& root)
{
	string fundid		= root["fundid"].asString();
	string cellid       = root["trading"]["cellid"].asString();
	string fundanme		= root["fundname"].asString();
	string parentid		= root["parentid"].asString();
	double equity		= root["equitybeginning"].asDouble();
	//int	   sequence		= root["sequence"].asInt();
	//由上面生成account信息
	struct CM::Account  account;
	account.tradingday		= toa(tradingday);    
	account.fundid			= fundid;
	account.prebalance		= 0;
	account.premargin		= 0;   
	account.requestid		= "";
	account.balance			= 0; 
	account.available		= equity;
	account.margin			= 0;
	account.incap			= 0;
	account.outcap			= 0;
	account.commission		= 0;
	account.closeprofit		= 0;
	account.positionprofit	= 0;
	//account.updatedate		= stamp_to_isodate(time(NULL));
	local_data.add_account(account.fundid, account);
	
	//更新映射关系
	m_fundid_cellid[atoll(fundid.c_str())] = atoll(cellid.c_str());
	m_cellid_fundid[atoll(cellid.c_str())] = atoll(fundid.c_str());
	
	LOG_INFO("init a virtual account=" <<  account.fundid << " parentid=" << account.parentid << " sun_data size=" << sun_data.get_account_vector().size() << " local_data size=" << local_data.get_account_vector().size());
}


void SunGardTradeDataProcess::LoadAccount(Json::Value& root)
{
	if(!local_data.find_account(root["cell_id"].asString()))
	{
		LOG_INFO("not load account=" << root["cell_id"].asString());
		return ;
	}
	STAccountInfo staccount;
	get_STAccountInfo_Struct(root, &staccount);	
	sun_data.add_account(toa(staccount.cell_id), staccount);
	
	struct CM::Account  account;                  
	account.tradingday = root["tradingday"].asString();
	account.requestid  = root["requestid"].asString();             
	convert_st_account(&account, &staccount);
	if(prebalance.find(account.fundid) != prebalance.end())
	{
		account.prebalance = prebalance[account.fundid]; 
		account.premargin  = premargin[account.fundid];
	}
	local_data.add_account(account.fundid, account);
	local_data.update_balance(account.fundid, sun_data.get_commission(account.fundid));
	LOG_INFO("init real account=" <<  account.fundid << " sun_data size=" << sun_data.get_account_vector().size() << " local_data size=" << local_data.get_account_vector().size() << " prebalance.size=" << prebalance.size());

	//第一条为当天的初始数据
	if(prebalance.find(account.fundid) == prebalance.end())
	{
		::CM::Account cma = local_data.get_account(account.fundid);
		prebalance[account.fundid] = cma.balance; 
		premargin[account.fundid] =  cma.margin;
		cma.prebalance = cma.balance;
		cma.premargin = cma.margin;
		local_data.add_account(account.fundid, cma);
		LOG_INFO("update  account=" <<  account.fundid << " prebalance=" << cma.prebalance << " premargin=" << cma.premargin << " positionprofit(need 0)=" << sun_data.get_commission(account.fundid));
		
	}
}

void SunGardTradeDataProcess::LoadPosition(Json::Value& root)
{
	STPositionInfo stposition;	
	get_STPositionInfo_Struct(root, &stposition);	
	sun_data.add_position(toa(stposition.cell_id), stposition);
	
	struct CM::Position  position;                    
	position.tradingday = root["tradingday"].asString(); 
	position.requestid =  root["requestid"].asString();
	convert_st_position(&position, &stposition);  
	local_data.add_position(position.fundid, position);
	local_data.update_commission_closeprofit(position.fundid, sun_data.get_commission(position.fundid), sun_data.get_closeprofit(position.fundid));
	local_data.update_balance(position.fundid, sun_data.get_commission(position.fundid));

	LOG_INFO("init position account=" <<  position.fundid << " sun_data size=" << sun_data.get_position_vector(position.fundid).size() << " local_data size=" << local_data.get_position_vector(position.fundid).size());
}

void SunGardTradeDataProcess::LoadDone(Json::Value& root)
{
	STDoneInfo stdone;
	get_STDoneInfo_Struct(root, &stdone);
	sun_data.add_done(toa(stdone.cell_id), stdone);

	struct CM::Done     done;
	done.tradingday = root["tradingday"].asString();
	convert_st_done(&done, &stdone);
	local_data.add_done(done.fundid, done);
	LOG_INFO("init done account=" <<  done.fundid << " sun_data size=" << sun_data.get_done_vector(done.fundid).size() << " local_data size=" << local_data.get_done_vector(done.fundid).size());
}

void SunGardTradeDataProcess::LoadOrder(Json::Value& root)
{
	STOrderInfo storder;
	get_STOrderInfo_Struct(root, &storder);	
	sun_data.add_order(toa(storder.order.cell_id), storder);

	struct CM::Order     order;
	order.tradingday = root["tradingday"].asString();
	convert_st_order(&order, &storder);
	local_data.add_order(order.fundid, order);
	LOG_INFO("init order account=" <<  order.fundid << " sun_data size=" << sun_data.get_order_vector(order.fundid).size() << " local_data size=" << local_data.get_order_vector(order.fundid).size());
}


void SunGardTradeDataProcess::LoadDayBar(Json::Value& root)
{
	::CM::DayBar daybar;
	get_struct_daybar(&daybar, root);
	if(!root["instrument"].isNull())
	{
		string instrumentid = root["instrument"].asString();
		map_daybar[instrumentid] = daybar;	
	}
}








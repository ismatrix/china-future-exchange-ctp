/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file CTPTradeDataProcess.cpp
///@brief 处理交易数据  写db push下游
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#include "CTPTradeDataProcess.h"
#include "util.h"
#include "singleton.h"

/*
 * 重启逻辑:
 *  1. 加载fund表中子账号生成初始帐号信息 
 *	2. 加载所有原始数据（按交易日和帐号id）
 *	3. 原始数据缓存 转化母基金缓存
 *	4. 加载所有子账号的初始数据
 *  conntion上游ok 并非节假日 有下面
 *  5. 清空内存，上游请求数据回来更新原始数据和母基金缓存
 *  6. 根据5的数据重新计算一遍子账号 （清空子账号信息 加载子账号初始数据计算）
 *
 * 每天结算逻辑:
 *  1. 16点了没结算 请求上游order和done
 *  2. 查询所有日k数据
 *  3. 同上面6
 *  4. 结算 
 *  5. 结算结果写数据库
 *
 *  原始数据入库问题
 *  1. 切换交易日的时候入库所有初始数据
 *  2. 重启的时候获取上游全量数据入库
 *  3. order done通知数据入库
 *  5. 保证重启的时候数据是全量切最新的, 保证了数据最新时间的数据是最新的
 *  6. 初始化倒内存的时候是覆盖制， 所以最新时间的数据将会加载到内存最终影像
 *
 *  一次下单多次成交问题处理。
 *  1. 一次下单会在连续时间段内，可能有几百个成交信息收到。 此时去查询account金额position会有大量失败。
 *  2. 这里中间查询失败这个成交相关的信息不会向下推送。
 *  3. 但是要确保最后一个成交要推送下去。 这个保证数据一致的。
 *  4. 导致结果下游会丢失部分done
 *
 */


CTPTradeDataProcess* g_syncsp = NULL;

int CTPTradeDataProcess::get_append_info(string& fundid, string& tradingday, string& requestid, ::CM::DoOrder& order)
{
	//CTPQueryObj::get_append_info(fundid, tradingday, requestid, order);

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
            LOG_DEBUG("---------signalname="<<order.signalname);
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

void* ReqAccountPosition(void* args)
{
	struct CM::Done done = *(struct CM::Done*)args;
	::CM::DoOrder doo;
	int ret = g_syncsp->get_append_info(done.fundid, done.tradingday, done.requestid, doo);
	if(ret == 0)
	{
		//done.parentid	= "";
		//done.seq		= doo.seq;;
		done.strategyid	= doo.strategyid; 
		done.userid		= doo.userid;
		done.signalname	= doo.signalname;
	}
	g_syncsp->local_data.add_done(done.fundid, done);                              
	g_syncsp->local_data.updatedone(done.fundid);                              

    //最后一个成交必须推送
	while(g_syncsp->get_need_success_tradeid() == done.tradeid)
	{
		msleep(50);
	
		int retp =g_syncsp->ReqQryInvestorPosition();                                        
		if(g_syncsp->WaitReq(retp) < 0) 
		{
			 LOG_ERROR("recv done to query position fail"); 
			 continue;                   
		} 
        
        sleep(1);
        int reta = g_syncsp->ReqQryTradingAccount();
		if(g_syncsp->WaitReq(reta) < 0)
		{
			LOG_ERROR("recv done to query account  fail");
			continue;
		}
	   
        //每个交易之后是不是要更新这个？？？？ 
        //g_syncsp->local_data.update_option_capital(g_syncsp->map_instruments);                              
        
		SessionM::Instance().send_done(done.fundid, done, g_syncsp->local_data.get_account(done.fundid), g_syncsp->local_data.get_position_vector(done.fundid));
		//用户原始数据更新内部数据
		g_syncsp->local_data.updatedb(done.fundid, g_syncsp->get_account(), done.tradingday, reta);
		g_syncsp->ctp_data.updatedb(done.fundid,   g_syncsp->get_account(), done.tradingday, reta);
		free((struct CM::Done*)args);
		return NULL;
	}

    if(g_syncsp->get_need_success_tradeid() != done.tradeid)
    {
		SessionM::Instance().send_done(done.fundid, done, g_syncsp->local_data.get_account(done.fundid), g_syncsp->local_data.get_position_vector(done.fundid));
        LOG_DEBUG("SessionM::Instance().send_done only.");
    }

	free((struct CM::Done*)args);
	return NULL;
}


//报单通知
void CTPTradeDataProcess::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	CThostFtdcTraderSpiI::OnRtnOrder(pOrder);
	if(NULL == pOrder) return;
	
	if(!local_data.find_account(pOrder->InvestorID)){ LOG_DEBUG("not load fundid=" <<  pOrder->InvestorID); return ; }
	
	RemoveChar(pOrder->InsertTime, ':');
	RemoveChar(pOrder->CancelTime, ':');
	
	Json::Value root;
	get_CThostFtdcOrderField(root,  pOrder);
	
	root["tradingday"] 	= pOrder->TradingDay;
	root["updatedate"] =  stamp_to_isodate(time(NULL));
	root["account"]     = get_account();
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);

	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("CTPORDER");
	_db.Insert(root.toStyledString());

	struct CM::Order     order;
	convert_ctp_order(&order, pOrder);
	//获取附加信息
	::CM::DoOrder doo;
	int ret = get_append_info(order.fundid, order.tradingday, order.requestid, doo);
	if(ret == 0)
	{
		//order.parentid		= "";
		//order.seq			= doo.seq;
		order.strategyid	= doo.strategyid; 
		order.userid		= doo.userid;
		order.signalname	= doo.signalname;
	}
	local_data.add_order(order.fundid, order); 
	ctp_data.add_order(order.fundid, *pOrder);
	SessionM::Instance().send_order(order.fundid, order);	
	local_data.updateorder(order.fundid); 
	
	//更新子账号信息 并推送
	if(ret == 0 && doo.fundid != order.fundid)
	{
		//order.parentid      = order.fundid;
		order.fundid		= doo.fundid;
		local_data.add_order(order.fundid, order); 
		SessionM::Instance().send_order(order.fundid, order);	
		local_data.updateorder(order.fundid); 
	}
}

///合约交易状态通知
void CTPTradeDataProcess::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	if(NULL == pInstrumentStatus) return;
	
	Json::Value root;
	get_CThostFtdcInstrumentStatusField(root,  pInstrumentStatus);
	
	root["tradingday"] 	= tradingday;
	root["requestid"] = 0;
	root["updatedate"] = stamp_to_isodate(time(NULL));

	//Json::FastWriter fast_writer;
	//string ss = fast_writer.write(root);
	//LOG_DEBUG(ss);

	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("CTPINSTRUMENTSTATUS");
	_db.Insert(root.toStyledString());
}


///交易所公告通知
void CTPTradeDataProcess::OnRtnBulletin(CThostFtdcBulletinField *pBulletin)
{
	if(NULL == pBulletin) return;
	
	Json::Value root;
	get_CThostFtdcBulletinField(root,  pBulletin);
		
	root["tradingday"] 	= pBulletin->TradingDay;
	root["requestid"] = 0;
	root["updatedate"] = stamp_to_isodate(time(NULL));

	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);

	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("CTPBULLETIN");
	_db.Insert(root.toStyledString());
}
	
//成交通知
void CTPTradeDataProcess::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	CThostFtdcTraderSpiI::OnRtnTrade(pTrade);

	if(NULL == pTrade) return;
	
	if(!local_data.find_account(pTrade->InvestorID)){ LOG_DEBUG("not load fundid=" <<  pTrade->InvestorID); return ; }

	//remove the char :
	RemoveChar(pTrade->TradeTime, ':');
	
	Json::Value root;
	get_CThostFtdcTradeField(root,  pTrade);
	root["tradingday"] 	= pTrade->TradingDay;
	root["requestid"] = pTrade->OrderRef;
	root["updatedate"] = stamp_to_isodate(time(NULL));
	root["account"]     = get_account();

	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);

	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("CTPDONE");
	_db.Insert(root.toStyledString());
	
	//原始数据
	ctp_data.add_done(toa(pTrade->InvestorID), *pTrade);          

	//异步处理
	struct CM::Done* p = new  CM::Done;
	convert_ctp_done(p, pTrade);

	set_need_success_tradeid(p->tradeid);

	pthread_t pthread;
	pthread_create(&pthread, NULL, ReqAccountPosition, (void*)p);
}

void CTPTradeDataProcess::on_login()
{
    uint32_t c = 0;                                                                 
    if(ReqSettlementInfoConfirm() < 0 && c++ < 10)           
    {                                                                               
        sleep(10);                                                                   
        LOG_DEBUG("ReqSettlementInfoConfirm fail!");                                
    }                                                                               
}

void CTPTradeDataProcess::Run()
{
	static bool init = false;
	if(init != get_InitFlag())
	{
	    LOG_INFO("old conn_flag=" << init << " new conn_flag=" << get_InitFlag());
	    init = get_InitFlag();
	}

    //int32_t tradingday_new = GetTradingDay();
	if(!get_InitFlag() || !g_holidays.IsOpenDay(time(NULL)) || tradingday < 20160101) return ;
    //tradingday = tradingday_new;

	static int32_t td = 0;
	if(td != tradingday)
	{
        int ret1 = 0;
        {
             //初始化账户
             local_data.clear();
             ctp_data.clear();
            //syncreq.clear();
	         map_daybar.clear();

             //必须先查询持仓 在查询账户 因为账户那边期权部分计算依赖持仓
             while(WaitReq(ReqQryInvestorPosition(), 100000) < 0) { sleep(3); LOG_DEBUG("ReqQryInvestorPosition fail!"); }
             sleep(1);
             while(WaitReq(ReqQryTradingAccount(), 100000) < 0) { sleep(3); LOG_DEBUG("ReqQryTradingAccount fail!"); }
             sleep(1);
             while(WaitReq(ReqQryTrade(), 100000) < 0) { sleep(3); LOG_DEBUG("ReqQryTrade fail!"); }
             sleep(1);
             while(WaitReq(ReqQryOrder(), 100000) < 0) { sleep(3); LOG_DEBUG("ReqQryOrder fail!"); }
             sleep(1);
             while(WaitReq(ret1=ReqSettlementInfoConfirm(), 100000) < 0) { sleep(3); LOG_DEBUG("ReqSettlementInfoConfirm fail!"); }
             
        }
   		//if(td != 0) local_data.updatedb(get_account(), toa(tradingday), ret1);
		//if(td != 0) ctp_data.updatedb(get_account(), toa(tradingday),  ret1);

		 local_data.updatedb(get_account(), toa(tradingday),  ret1, true);
		 ctp_data.updatedb(get_account(), toa(tradingday),  ret1, true);
	    
		//重启推送tradingday
		vector<string> fundidlist = local_data.get_fundids();
		for(vector<string>::iterator itr=fundidlist.begin(); itr!=fundidlist.end(); itr++)
		{
			if(td == 0 && db_set_tradingday(*itr, tradingday)==0)
			{
				SessionM::Instance().send_tradingday(*itr, toa(tradingday));
				LOG_INFO("restart fundid=" << *itr << " tradingday=" << tradingday);
			}
		}

		LOG_INFO("old  tradingday=" << td << " new tradingday=" << tradingday << " requestid=" << ret1);
		td = tradingday;
    }

	//获取数据库的tradingday
	vector<string> fundidlist = local_data.get_fundids();
	for(vector<string>::iterator itr=fundidlist.begin(); itr!=fundidlist.end(); itr++)
	{
		int current_tradingday = db_get_tradingday(*itr);
		if(current_tradingday != (int)tradingday && db_set_tradingday(*itr, tradingday)==0)
		{
			SessionM::Instance().send_tradingday(*itr, toa(tradingday));
			LOG_INFO("fundid=" << *itr << " db_tradingday=" << current_tradingday << " tradingday=" << tradingday);
		}
	}

    return;

}

//线程主函数 其实这里用libev的事件处理机制最好。
void* thread_run(void* p)
{
	CTPTradeDataProcess* ctp = (CTPTradeDataProcess*)p;
	
	//主处理线程
	while(true)
	{
		ctp->Run();
		sleep(30);
	}
	return NULL;
}


void CTPTradeDataProcess::Init(TE_RESUME_TYPE module)
{
	_lock = 0;

	//查询节假日表mongo
	g_holidays.Init(CMongodb::Instance());

	//标识他是金仕达帐号
	account_type = "ctp";
	
	//先去数据库查询
	uint32_t today_trade = atoll(stamptostr(g_holidays.GetTradingDay(), "%Y%m%d").c_str());
	tradingday = today_trade;

	//按账号查所有基金 根据基金查寻基金对应的所有子账号信息 --->查询帐号对应的所有基金对应的虚拟帐号
	Json::Value q, q1, q2, q3;
	q1["trading.account"] = get_account();
	q1["state"] = "online";
	LoadDb("FUND", today_trade, &CTPTradeDataProcess::LoadFund, q1);
	for(uint32_t i=0; i<vecfundid.size(); i++)
	{
		q2["fundid"]		= vecfundid[i];
		LoadDb("FUND", today_trade, &CTPTradeDataProcess::LoadVirtualFund, q2);
	}
	//没有加载到基金账号这里直接返回失败
	if(m_fundid_cellid.size() < 1)
	{
		LOG_ERROR("db not load fund account.");
		exit(-1);
	}

    q3["istrading"] = 1;

	//按账号加载数据 到内存的时候按fundid来划分 会覆盖上面的第一次初始化
	q["account"] = get_account();
	q["tradingday"] = toa(tradingday);
	LoadDb("INSTRUMENT", today_trade, &CTPTradeDataProcess::LoadInstrument, q3);
	LoadDb("CTPACCOUNT", today_trade, &CTPTradeDataProcess::LoadAccount, q);
	LoadDb("CTPPOSITION", today_trade, &CTPTradeDataProcess::LoadPosition, q);
	LoadDb("CTPDONE", today_trade, &CTPTradeDataProcess::LoadDone, q);
	LoadDb("CTPORDER", today_trade, &CTPTradeDataProcess::LoadOrder, q);
    //local_data.update_option_capital(account.fundid, map_instruments); //加载数据库之后不需要 数据库的数据已经计算过

	//加载子账号结算信息TODO

	//初始化ctp
	CThostFtdcTraderSpiI::Init(module);
	
	if(!get_InitFlag()) msleep(500);
	
	if(tradingday < 20160101) tradingday = today_trade;

	LOG_INFO("g_holidays.GetTradingDay=" << today_trade <<  " tradingday=" << tradingday);

	pthread_t pthread;
	pthread_create(&pthread, NULL, thread_run, this);
}

void CTPTradeDataProcess::LoadDb(const char* table, uint32_t tradingday, _functiontype callback, Json::Value& q)
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
			 exit(-1);
	    }
	}
	db.Release();
}

void CTPTradeDataProcess::LoadFund(Json::Value& root)
{
	if(!root["fundid"].isNull())
	{
		string fundid = root["fundid"].asString();
		vecfundid.push_back(fundid);
		LOG_INFO("init a fundid=" << fundid)
	}
}

void CTPTradeDataProcess::LoadInstrument(Json::Value& root)
{
    CThostFtdcInstrumentField ins;
    get_CThostFtdcInstrumentField_Struct(root, &ins);
    map_instruments[ins.InstrumentID] = ins;
    LOG_INFO("LoadInstrument " << ins.InstrumentID  << " length:" << map_instruments.size());
}

void CTPTradeDataProcess::LoadVirtualFund(Json::Value& root)
{
	string fundid		= root["fundid"].asString();
	string cellid		= root["trading"]["cellid"].asString();
	string fundanme		= root["fundname"].asString();
	string parentid		= root["parentid"].asString();
	double equity		= root["equitybeginning"].asDouble();
	//int	   sequence		= root["sequence"].asInt();
	//由上面生成account信息
	struct CM::Account  account;
	account.tradingday		= toa(tradingday);    
	account.fundid			= root["fundid"].asString();
	account.parentid		= parentid;
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
	m_fundid_cellid[fundid] = cellid;
	m_cellid_fundid[cellid] = fundid;

	LOG_INFO("init a virtual account=" <<  account.fundid << " parentid=" << account.parentid << " ctp_data size=" << ctp_data.get_account_vector().size() << " local_data size=" << local_data.get_account_vector().size());
}

void CTPTradeDataProcess::LoadAccount(Json::Value& root)
{
	if(!local_data.find_account(root["accountid"].asString()))
	{
		LOG_INFO("not load account=" << root["accountid"].asString());
		return ;
	}

	CThostFtdcTradingAccountField ctpaccount;
	get_CThostFtdcTradingAccountField_Struct(root, &ctpaccount);	
	ctp_data.add_account(toa(ctpaccount.AccountID), ctpaccount);

	struct CM::Account  account;                  
	account.tradingday = root["tradingday"].asString();
	account.requestid  = root["requestid"].asString();             
    convert_ctp_account(&account, &ctpaccount);
	local_data.add_account(account.fundid, account);
    LOG_INFO("init real account=" <<  account.fundid << " ctp_data size=" << ctp_data.get_account_vector().size() << " local_data size=" << local_data.get_account_vector().size());
}

void CTPTradeDataProcess::LoadPosition(Json::Value& root)
{
	CThostFtdcInvestorPositionField ctpposition;	
	get_CThostFtdcInvestorPositionField_Struct(root, &ctpposition);	
	ctp_data.add_position(toa(ctpposition.InvestorID), ctpposition);
	
	struct CM::Position  position;                    
	position.tradingday = root["tradingday"].asString(); 
	position.requestid =  root["requestid"].asString();
	#ifndef  __ROHON__
	    convert_ctp_position(&position, &ctpposition, get_todayopenaverage_ctp(&ctpposition));        
    #else
        convert_ctp_position(&position, &ctpposition, get_todayopenaverage_rohon(&ctpposition));        
    #endif
	local_data.add_position(position.fundid, position);
	
	LOG_INFO("init position account=" <<  position.fundid << " ctp_data size=" << ctp_data.get_position_vector(position.fundid).size() << " local_data size=" << local_data.get_position_vector(position.fundid).size());
}

void CTPTradeDataProcess::LoadDone(Json::Value& root)
{
	CThostFtdcTradeField ctpdone;
	get_CThostFtdcTradeField_Struct(root, &ctpdone);
	ctp_data.add_done(toa(ctpdone.InvestorID), ctpdone);

	struct CM::Done     done;
	done.tradingday = root["tradingday"].asString();
	convert_ctp_done(&done, &ctpdone);
	local_data.add_done(done.fundid, done);
	
	LOG_INFO("init done account=" <<  done.fundid << " ctp_data size=" << ctp_data.get_done_vector(done.fundid).size() << " local_data size=" << local_data.get_done_vector(done.fundid).size());
}

void CTPTradeDataProcess::LoadOrder(Json::Value& root)
{
	CThostFtdcOrderField ctporder;
	get_CThostFtdcOrderField_Struct(root, &ctporder);	
	ctp_data.add_order(toa(ctporder.InvestorID), ctporder);

	struct CM::Order     order;
	order.tradingday = root["tradingday"].asString();
	convert_ctp_order(&order, &ctporder);
	local_data.add_order(order.fundid, order);
	
	LOG_INFO("init order account=" <<  order.fundid << " ctp_data size=" << ctp_data.get_order_vector(order.fundid).size() << " local_data size=" << local_data.get_order_vector(order.fundid).size());
}

void CTPTradeDataProcess::LoadDayBar(Json::Value& root)
{
	::CM::DayBar daybar;
	get_struct_daybar(&daybar, root);
	if(!root["instrument"].isNull())
	{
		string instrumentid = root["instrument"].asString();
		map_daybar[instrumentid] = daybar;	
	}
}








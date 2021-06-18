/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file CTPObjPools.cpp
///@brief 处理交易数据  写db push下游
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#include "CTPObjPools.h"
#include "util.h"
#include "singleton.h"


	
///请求查询资金账户响应
void CTPQueryObj::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryTradingAccount(pTradingAccount, pRspInfo, nRequestID, bIsLast);
	if(NULL == pTradingAccount) return;

	//内存不存在的过滤掉
	if(!local_data.find_account(pTradingAccount->AccountID))
	{
		LOG_INFO("not load account=" << pTradingAccount->AccountID);
		return ;
	}
	ctp_data.add_account(pTradingAccount->AccountID, *pTradingAccount);

	struct CM::Account  account;                       
	account.tradingday = toa(tradingday);              
	account.requestid  = toa(nRequestID);                  
	convert_ctp_account(&account, pTradingAccount);
    //更新期权的影响
	local_data.add_account(account.fundid, account);   
    local_data.update_option_capital(account.fundid, map_instruments);
}

///请求查询投资者持仓响应 初始化的时候需要
void CTPQueryObj::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryInvestorPosition(pInvestorPosition, pRspInfo, nRequestID, bIsLast);
	if(NULL == pInvestorPosition) return;

	if(!local_data.find_account(pInvestorPosition->InvestorID)){ LOG_DEBUG("not load fundid=" <<  pInvestorPosition->InvestorID); return ; }
	
	//过滤掉全部为0的无效数据
	//if(pInvestorPosition->YdPosition==0 && pInvestorPosition->OpenVolume ==0) return;

	ctp_data.add_position(pInvestorPosition->InvestorID, *pInvestorPosition);

	struct CM::Position  position;                          
	position.tradingday = toa(tradingday);                  
	position.requestid = toa(nRequestID);                      
    #ifndef  __ROHON__
	    convert_ctp_position(&position, pInvestorPosition, get_todayopenaverage_ctp(pInvestorPosition));        
    #else
        convert_ctp_position(&position, pInvestorPosition, get_todayopenaverage_rohon(pInvestorPosition));        
    #endif
    local_data.add_position(position.fundid, position);     
}

//查询逐笔详细持仓
void CTPQueryObj::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryInvestorPositionDetail(pInvestorPositionDetail, pRspInfo, nRequestID, bIsLast);
	if(NULL == pInvestorPositionDetail) return;
    
    Json::Value r;
    get_CThostFtdcInvestorPositionDetailField(r, pInvestorPositionDetail);
	r["updatedate"] = stamp_to_isodate(time(NULL));
	r["requestid"] = nRequestID;
	//重启或者结算时候同步请求的 不垮线程
	//CMongodb::Instance().ChangeCollection("CTPPOSITIONDETAIL");
	//CMongodb::Instance().Insert(r.toStyledString());

}

///请求查询成交响应 重启的时候需要（内存已经清空 或者 初始化）
void CTPQueryObj::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryTrade(pTrade, pRspInfo, nRequestID, bIsLast);
	if(NULL == pTrade) return;
	
	if(!local_data.find_account(pTrade->InvestorID)){ LOG_DEBUG("not load fundid=" <<  pTrade->InvestorID); return ; }

	//remove the char :
	RemoveChar(pTrade->TradeTime, ':');

	ctp_data.add_done(pTrade->InvestorID, *pTrade); 

	//done会返回下单时的请求id  需要存储
	struct CM::Done     done;
	//done.requestid = toa(nRequestID);
	convert_ctp_done(&done, pTrade);
	
	//获取附加信息加上
	::CM::DoOrder doo;
	int ret = get_append_info(done.fundid, done.tradingday, done.requestid, doo);
	if(ret == 0)
	{
		//done.parentid		= "";
		//done.seq			= doo.seq;
		done.strategyid		= doo.strategyid; 
		done.userid			= doo.userid;
		done.signalname		= doo.signalname;
	}
	local_data.add_done(done.fundid, done);  
	
	LOG_DEBUG("get_append_info fundid=" << done.fundid << " tradingday=" << done.tradingday << " requestid=" <<done.requestid << " ret=" << ret << " signalname=" << doo.signalname);
	
	LOG_DEBUG("OnRspQryTrade");
}

int CTPQueryObj::get_append_info(string& fundid, string& tradingday, string& requestid, ::CM::DoOrder& order)
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
            LOG_DEBUG("---------signalname="<<order.signalname);
			return 0;
		}
	    catch(...)
	    {
			 LOG_ERROR("REQUESTMAP get db.Next() fail.");
			 return -1;
	    }
	}
	//LOG_DEBUG("get_append_info fundid=" << fundid << " tradingday=" << tradingday << " requestid=" <<requestid);
	return 0;
}


void CTPQueryObj::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryOrder(pOrder, pRspInfo, nRequestID, bIsLast);
	if(NULL == pOrder) return;

	if(!local_data.find_account(pOrder->InvestorID)){ LOG_DEBUG("not load fundid=" <<  pOrder->InvestorID); return ; }
	RemoveChar(pOrder->InsertTime, ':');
	RemoveChar(pOrder->CancelTime, ':');
	
	ctp_data.add_order(pOrder->InvestorID, *pOrder); 

	//order会返回下单时的请求id  需要存储
	struct CM::Order order;
	//order.requestid = toa(nRequestID);
	convert_ctp_order(&order, pOrder);
	
	//获取附加信息加上
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
	
	LOG_DEBUG("get_append_info fundid=" << order.fundid << " tradingday=" << order.tradingday << " requestid=" << order.requestid << " ret=" << ret << " signalname=" << doo.signalname);
	
	LOG_DEBUG("OnRspQryOrder");
}

///请求查询产品响应
void CTPQueryObj::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryProduct(pProduct, pRspInfo, nRequestID, bIsLast);
	if(NULL == pProduct) return;
	string insname = pProduct->ProductName;
	GBK2UTF8((char*)insname.c_str(), insname.size(), pProduct->ProductName, sizeof(pProduct->ProductName));
	IsValidDouble(pProduct->PriceTick);	
	IsValidDouble(pProduct->UnderlyingMultiple);	

	Json::Value root, s;
	get_CThostFtdcProductField(root, pProduct);
	root["updatedate"] = stamp_to_isodate(time(NULL));
	root["requestid"] = nRequestID;
	s["$set"] = root;

	Json::Value q;
	q["productid"] = pProduct->ProductID;	

	if(bIsLast)
	{
		Json::FastWriter fast_writer;
		string ss = fast_writer.write(s);
		LOG_INFO(ss);
	}
	//CMongodb::Instance().ChangeCollection("PRODUCT");
	//CMongodb::Instance().Update(s.toStyledString(), q.toStyledString(), true);
}

///合约信息
void CTPQueryObj::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryInstrument(pInstrument, pRspInfo, nRequestID, bIsLast);
	if(NULL == pInstrument) return;

	//name转化为utf8 
	string insname = pInstrument->InstrumentName;
	GBK2UTF8((char*)insname.c_str(), insname.size(), pInstrument->InstrumentName, sizeof(pInstrument->InstrumentName));
	IsValidDouble(pInstrument->PriceTick);	
	IsValidDouble(pInstrument->StrikePrice);	
	IsValidDouble(pInstrument->LongMarginRatio);	
	IsValidDouble(pInstrument->ShortMarginRatio);	
	IsValidDouble(pInstrument->UnderlyingMultiple);	
	
	//set 不存在 insert
	Json::Value root, s;
	get_CThostFtdcInstrumentField(root, pInstrument);
	root["updatedate"] = stamp_to_isodate(time(NULL));
	root["requestid"] = nRequestID;
	s["$set"]	 = root;

	Json::Value q;
	q["instrumentid"] = pInstrument->InstrumentID;	

	if(bIsLast)
	{	//纸打印最后一条
		Json::FastWriter fast_writer;
		string ss = fast_writer.write(s);
		LOG_INFO(ss);
	}
	
	//重启或者结算时候同步请求的 不垮线程
	//CMongodb::Instance().ChangeCollection("INSTRUMENT");
	//CMongodb::Instance().Update(s.toStyledString(), q.toStyledString(), true);
	map_instruments[pInstrument->InstrumentID] = *pInstrument;
}

void CTPQueryObj::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspOrderInsert(pInputOrder, pRspInfo, nRequestID, bIsLast);
    if(pInputOrder == NULL) return ;
	char ErrorMsg[243]={};
    int ErrorID=0;
	if(pRspInfo!=NULL)
    {
        GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
        ErrorID = pRspInfo->ErrorID;
    }

	LOG_DEBUG("fundid=" << pInputOrder->InvestorID << " orderid=" << pInputOrder->OrderRef << " InstrumentID=" << pInputOrder->InstrumentID << " nRequestID=" << nRequestID << " ErrorMsg=" << ErrorMsg << " bIsLast=" << bIsLast);
	
    struct ::CM::Order order;
	order.requestid = nRequestID;
	convert_ctp_doorder_order(&order, pInputOrder);
	
	order.tradingday  = toa(tradingday); 
	order.orderstatus = "NoTradeQueueing";	//正在报单
    order.orderdate   = stamptostr(time(NULL), "%Y%m%d");
    order.ordertime   = stamptostr(time(NULL), "%H%M%S");
    order.oerrmsg = ErrorMsg; 
    order.oerrno  = ErrorID;

	SessionM::Instance().send_order(order.fundid, order);
}

void CTPQueryObj::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	CThostFtdcTraderSpiI::OnErrRtnOrderInsert(pInputOrder, pRspInfo);
	if(pInputOrder==NULL) return;

    char ErrorMsg[243]={};
    int ErrorID=0;
	if(pRspInfo!=NULL)
    {
        GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
        ErrorID = pRspInfo->ErrorID;
    }
	LOG_DEBUG("fundid=" << pInputOrder->InvestorID << " orderid=" << pInputOrder->OrderRef << " InstrumentID=" << pInputOrder->InstrumentID << " ERRORID=" << ErrorID  << " ErrorMsg=" << ErrorMsg);

	struct ::CM::Order order;
    convert_ctp_doorder_order(&order, pInputOrder);
	order.tradingday  = toa(tradingday); 
	order.orderstatus = "Invalid";			//报单失败废单
	order.oerrmsg	  = ErrorMsg;	//错误信息
	order.oerrno	= ErrorID;//错误码
    order.orderdate   = stamptostr(time(NULL), "%Y%m%d");
    order.ordertime   = stamptostr(time(NULL), "%H%M%S");

	SessionM::Instance().send_order(order.fundid, order);
}

void CTPQueryObj::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspOrderAction(pInputOrderAction, pRspInfo, nRequestID, bIsLast);
    char ErrorMsg[243]={};
    int ErrorID=0;
	if(pRspInfo!=NULL)
    {
        GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
        ErrorID = pRspInfo->ErrorID;
    }

    LOG_DEBUG("fundid=" << pInputOrderAction->InvestorID << " orderid=" << pInputOrderAction->OrderRef << " InstrumentID=" << pInputOrderAction->InstrumentID << " ErrorMsg=" << ErrorMsg);

    struct ::CM::Order order;
	convert_ctp_cancleorder_order(&order, pInputOrderAction);
    order.tradingday  = toa(tradingday); 
	order.orderstatus = "Canceled";			//场外撤单
	order.oerrmsg	  = ErrorMsg;	//错误信息
	order.oerrno	  = ErrorID;//错误码
	order.orderdate   = stamptostr(time(NULL), "%Y%m%d");
    order.ordertime   = stamptostr(time(NULL), "%H%M%S");

    SessionM::Instance().send_order(order.fundid, order);
}

void  CTPQueryObj::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	CThostFtdcTraderSpiI::OnErrRtnOrderAction(pOrderAction, pRspInfo);
	if(pOrderAction == NULL) return;

    char ErrorMsg[243]={};
    int ErrorID=0;
	if(pRspInfo!=NULL)
    {
        GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
        ErrorID = pRspInfo->ErrorID;
    }

	LOG_DEBUG("fundid=" << pOrderAction->InvestorID << " orderid=" << pOrderAction->OrderRef << " InstrumentID=" << pOrderAction->InstrumentID<< " ERRORID=" << ErrorID << " ErrorMsg=" << ErrorMsg);

	struct ::CM::Order order;
	convert_ctp_cancleorder_order(&order, pOrderAction);
	order.tradingday  = toa(tradingday); 
	order.orderstatus = "Canceled";			//场外撤单
	order.oerrmsg	  = ErrorMsg;	//错误信息
	order.oerrno	= ErrorID;//错误码
	order.orderdate   = stamptostr(time(NULL), "%Y%m%d");
    order.ordertime   = stamptostr(time(NULL), "%H%M%S");

    SessionM::Instance().send_order(order.fundid, order);
}

string CTPQueryObj::queryAccount(int from)
{
	int ret =	ReqQryTradingAccount();
	if(WaitReq(ret) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return "{\"erorr\":\"query account fail.\"}";
	}
    //local_data.update_option_capital(map_instruments);

	vector<CThostFtdcTradingAccountField> accounts =  ctp_data.get_account_vector();
	Json::Value root;
	typeof(accounts.begin()) itr = accounts.begin();
	for(; itr!=accounts.end(); itr++)
	{
		Json::Value item;
		if(from != 1)
			get_CThostFtdcTradingAccountField(item, &(*itr));
		else
		{
			::CM::Account account;
			convert_ctp_account(&account, &(*itr));
			get_json_account(&account, item);
		}
		root.append(item);
	}
	
	return root.toStyledString();

}


string CTPQueryObj::queryAccountOne(int from, string fundid)
{
	int ret =	ReqQryTradingAccount();
	if(WaitReq(ret) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return "{\"erorr\":\"query account fail.\"}";
	}
    //local_data.update_option_capital(map_instruments);

	CThostFtdcTradingAccountField account =  ctp_data.get_account(fundid);
	Json::Value root;
	if(from != 1)
		get_CThostFtdcTradingAccountField(root, &account);
	else
	{
		::CM::Account a;
		convert_ctp_account(&a, &account);
		get_json_account(&a, root);
	}
	return root.toStyledString();
}

string CTPQueryObj::queryDone(int from, string fundid)
{
	int ret =	ReqQryTrade();
	if(WaitReq(ret) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return "{\"erorr\":\"query done fail.\"}";
	}
	
	vector<CThostFtdcTradeField> dones =  ctp_data.get_done_vector(fundid);
	Json::Value root;
	typeof(dones.begin()) itr = dones.begin();
	for(; itr!=dones.end(); itr++)
	{
		Json::Value item;
		if(from !=1)
		  get_CThostFtdcTradeField(item, &(*itr));
		else
		{
			::CM::Done done;
			convert_ctp_done(&done, &(*itr));
			get_json_done(&done, item);
		}
		root.append(item);
	}
	return root.toStyledString();


}
string CTPQueryObj::queryPosition(int from, string fundid)
{
	int ret = ReqQryInvestorPosition();
	if(WaitReq(ret) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return "{\"erorr\":\"query position fail.\"}";
	}
	
	vector<CThostFtdcInvestorPositionField> positions =  ctp_data.get_position_vector(fundid);
	Json::Value root;
	typeof(positions.begin()) itr = positions.begin();
	for(; itr!=positions.end(); itr++)
	{
		Json::Value item;
		if(from != 1)
			get_CThostFtdcInvestorPositionField(item, &(*itr));
		else
		{
			::CM::Position position;
			#ifndef  __ROHON__
        	    convert_ctp_position(&position, &(*itr), get_todayopenaverage_ctp(&(*itr)));        
            #else
                convert_ctp_position(&position, &(*itr), get_todayopenaverage_rohon(&(*itr)));        
            #endif
			get_json_position(&position, item);
		}
		root.append(item);
	}
	return root.toStyledString();


}

string CTPQueryObj::queryOrder(int from, string fundid)
{
	int ret =	ReqQryOrder();
	if(WaitReq(ret) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return "{\"erorr\":\"query order fail.\"}";
	}
	
	vector<CThostFtdcOrderField> orders =  ctp_data.get_order_vector(fundid);
	Json::Value root;
	typeof(orders.begin()) itr = orders.begin();
	for(; itr!=orders.end(); itr++)
	{
		Json::Value item;
		if(from != 1)
			get_CThostFtdcOrderField(item, &(*itr));
		else
		{
			::CM::Order order;
			convert_ctp_order(&order, &(*itr));
			get_json_order(&order, item);
		}
		root.append(item);
	}
	return root.toStyledString();
}

	
string CTPQueryObj::queryInsetruments(string ins)
{
    Json::Value root;
    typeof(map_instruments.begin()) itr = map_instruments.find(ins);
    if(itr != map_instruments.end())
    {
	    get_CThostFtdcInstrumentField(root, &itr->second);
    }
    return root.toStyledString();
}


int CTPQueryObj::updatePassword(string oldpwd, string newpwd)
{
	if(oldpwd != get_pwd()) return -1;
	int ret =	ReqUserPasswordUpdate((char*)oldpwd.c_str(), (char*)newpwd.c_str());
	if(WaitReq(ret) < 0)
	{
		LOG_ERROR("query  fail or timeout.");
		return -2;
	}
	set_pwd(newpwd);	
	return 0;
}

void CTPQueryObj::Init(TE_RESUME_TYPE module)
{
	//初始化ctp
	CThostFtdcTraderSpiI::Init(module);
}

int CTPQueryObj::DoOrder(const ::CM::DoOrder& order)
{
    Json::Value root;
	get_json_doorder((::CM::DoOrder*)&order, root);
	LOG_DEBUG("DoOrder param " << root.toStyledString());
	root["tradingday"] = toa(tradingday);
    //root["seq"] = m_fundid_seq[order.fundid];


	//真正下单还是用真实帐号
	string fundid = order.fundid;
	if(order.parentid != "") fundid = order.parentid;
	
	CThostFtdcInputOrderField ord;
	memset(&ord, 0, sizeof(ord));
	snprintf(ord.BrokerID,		sizeof(ord.BrokerID),	 "%s", get_brokerid().c_str());
	snprintf(ord.InvestorID,	sizeof(ord.InvestorID),  "%s", fundid.c_str());
	snprintf(ord.InstrumentID,  sizeof(ord.InstrumentID),"%s", order.instrumentid.c_str());
	snprintf(ord.UserID,        sizeof(ord.UserID),		 "%s", fundid.c_str());
	
	ord.Direction           = my_direction_to_ctp(order.direction);     // 买卖方向
	ord.CombOffsetFlag[0]   = my_offsetflag_to_ctp(order.offsetflag);   //组合开平标志
	ord.CombHedgeFlag[0]    = my_hedgeflag_to_ctp(order.hedgeflag);     //投机套保
	ord.VolumeTotalOriginal = order.volume;					// 数量   
	
	//固定值
	ord.MinVolume           = 1;							// 最小成交量
	ord.ForceCloseReason    = THOST_FTDC_FCC_NotForceClose; // 强平原因
	ord.IsAutoSuspend       = 0;							// 自动挂起标志 默认0永远不挂起
	ord.UserForceClose      = 0;							// 用户强平标志
	ord.VolumeCondition     = THOST_FTDC_VC_AV;			// 成交量类型 CV全成 AV部分 MV最小
	
	ord.ContingentCondition	= THOST_FTDC_CC_Immediately;// 触发条件 立即触发 买一 买一。。。
	
	ord.OrderPriceType		= THOST_FTDC_OPT_AnyPrice;	// 市价单  限价单，，，
	ord.LimitPrice			= order.price;				// 价格
	ord.TimeCondition		= THOST_FTDC_TC_IOC;		//有效时间 立即完成  当日有效...
	
	string ordertype = order.ordertype;
	if(ordertype == "0")
	{	//市价
		ord.OrderPriceType = THOST_FTDC_OPT_AnyPrice;		//市价单
		ord.LimitPrice = 0;									//默认0
		ord.TimeCondition = THOST_FTDC_TC_IOC;				///立即完成，否则撤销
	}
	else if(ordertype == "1")
	{	//限价
		ord.OrderPriceType	= THOST_FTDC_OPT_LimitPrice;	//限价单
		ord.LimitPrice		= order.price;					// 价格
		if(order.donetype == "0")
		{
			ord.TimeCondition	= THOST_FTDC_TC_GFD;			///当日有效
		}
		else if(order.donetype == "1")
		{
			ord.TimeCondition	= THOST_FTDC_TC_IOC;			///当日有效
			ord.VolumeCondition     = THOST_FTDC_VC_AV; //FAK 马上成交 其余撤销
		}
		else if(order.donetype == "2" || order.donetype == "3")
		{
			ord.TimeCondition	= THOST_FTDC_TC_IOC;			///当日有效
			ord.VolumeCondition     = THOST_FTDC_VC_CV;	//FOK全成交 否则全撤销
		}
		else
		{
			LOG_ERROR("unknow donetype.");
			return -1;
		}
	}
	else if(ordertype == "2")
	{	//最优
		ord.OrderPriceType	= THOST_FTDC_OPT_BestPrice;		//最优价格
		ord.LimitPrice		= order.price;					// 价格
		ord.TimeCondition = THOST_FTDC_TC_IOC;				///立即完成，否则撤销
	}
	else
	{
		LOG_ERROR("unknow ordertype.");
		return -1;
	}

	int ret = CThostFtdcTraderSpiI::DoOrder(ord);
	//if(WaitReq(ret) < 0)
	//{
	//	LOG_ERROR("doOrder fail or timeout.");
	//}
	root["requestid"]  = toa(ret);
	root["updatedate"] = stamp_to_isodate(time(NULL));
	
	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("DOORDER");
	_db.Insert(root.toStyledString());
	
	stringstream ss;
	ss << "BrokerID="			<< ord.BrokerID			<<",";
	ss<<"InvestorID="			<< ord.InvestorID		<<",";
	ss<<"InstrumentID="			<< ord.InstrumentID		<<",";
	ss<<"OrderRef="				<< ord.OrderRef			<<",";
	ss<<"RequestID="			<< ord.RequestID		<<",";
	ss<<"UserID="				<< ord.UserID			<<",";
	ss<<"OrderPriceType="		<< ord.OrderPriceType	<<",";
	ss<<"Direction="			<< ord.Direction		<<",";
	ss<<"CombOffsetFlag="		<< ord.CombOffsetFlag	<<",";
	ss<<"CombHedgeFlag="		<< ord.CombHedgeFlag	<<",";
	ss<<"LimitPrice="			<< ord.LimitPrice		<<",";
	ss<<"VolumeTotalOriginal="	<< ord.VolumeTotalOriginal<<",";
	ss<<"TimeCondition="		<< ord.TimeCondition	<<",";
	ss<<"VolumeCondition="		<< ord.VolumeCondition	<<",";
	ss<<"MinVolume="			<< ord.MinVolume		<<",";
	ss<<"ContingentCondition="	<< ord.ContingentCondition<<",";
	ss<<"StopPrice="			<< ord.StopPrice		<<",";
	ss<<"ForceCloseReason="		<< ord.ForceCloseReason	<<",";
	ss<<"IsAutoSuspend="		<< ord.IsAutoSuspend	<<",";
	ss<<"UserForceClose="		<< ord.UserForceClose	<<",";
	//ss<<"ExchangeID="			<< ord.ExchangeID	;
	LOG_INFO(ss.str());

	return ret;
}


//撤单
int CTPQueryObj::CancleOrder(string fundid, string sessionid, string instrumentid, string privateno, string orderid)
{
	//根据orderid 可以定位到
	::CM::Order order = local_data.get_order(fundid, instrumentid, sessionid,  privateno, orderid);
	if(order.fundid != fundid)
	{
		LOG_ERROR("[fundid="<< fundid << " instrumentid=" << instrumentid << " privateno=" << privateno << " orderid=" << orderid << "]order not in  memory.");
		return -1;
	}

	CThostFtdcInputOrderActionField ordaction;
	memset(&ordaction, 0, sizeof(ordaction));
	snprintf(ordaction.BrokerID		,	sizeof(ordaction.BrokerID)		,	"%s", get_brokerid().c_str());
	snprintf(ordaction.InvestorID	,	sizeof(ordaction.InvestorID)	,	"%s", order.fundid.c_str());
	snprintf(ordaction.UserID		,	sizeof(ordaction.UserID)		,	"%s", order.fundid.c_str());
	snprintf(ordaction.InstrumentID	,	sizeof(ordaction.InstrumentID)	,	"%s", order.instrumentid.c_str());
	snprintf(ordaction.ExchangeID	,	sizeof(ordaction.ExchangeID)	,	"%s", order.exchangeid.c_str());
	snprintf(ordaction.OrderRef		,	sizeof(ordaction.OrderRef)		,	"%s", order.privateno.c_str());
	snprintf(ordaction.OrderSysID	,	sizeof(ordaction.OrderSysID)	,	"%s", order.orderid.c_str());

	ordaction.SessionID = atoll(order.sessionid.c_str());
	ordaction.FrontID = atoll(order.frontid.c_str());
	
	ordaction.ActionFlag = THOST_FTDC_AF_Delete;

	int ret = CThostFtdcTraderSpiI::DoCancleOrder(ordaction);
	//if(WaitReq(ret) < 0)
	//{
	//	LOG_ERROR("doOrder fail or timeout.");
	//}

	//场外撤单
	Json::Value root;
	root["fundid"]      = ordaction.InvestorID; 
	root["brokerid"]    = ordaction.BrokerID;
	root["instrumentid"]= ordaction.InstrumentID;
	root["ordertype"]   = "6";	//场外撤单
	root["exchangeid"]  = ordaction.ExchangeID;
	root["privateno"]   = ordaction.OrderRef;
	root["orderid"]     = ordaction.OrderSysID;
	root["msg"]         = "撤单";
	root["updatedate"] = stamp_to_isodate(time(NULL));
	root["requestid"] = toa(ret);
	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("DOORDER");
	_db.Insert(root.toStyledString());
	

	stringstream ss;	
	ss << "BrokerID="		<< ordaction.BrokerID       << ", ";
	ss << "InvestorID="		<< ordaction.InvestorID     << ", ";
	ss << "OrderActionRef=" << ordaction.OrderActionRef << ", ";
	ss << "OrderRef="		<< ordaction.OrderRef       << ", ";
	ss << "RequestID="		<< ordaction.RequestID      << ", ";
	ss << "FrontID="		<< ordaction.FrontID        << ", ";
	ss << "SessionID="		<< ordaction.SessionID      << ", ";
	ss << "ExchangeID="		<< ordaction.ExchangeID     << ", ";
	ss << "OrderSysID="		<< ordaction.OrderSysID     << ", ";
	ss << "ActionFlag="		<< ordaction.ActionFlag     << ", ";
	ss << "LimitPrice="		<< ordaction.LimitPrice     << ", ";
	ss << "VolumeChange="	<< ordaction.VolumeChange   << ", ";
	ss << "UserID="			<< ordaction.UserID         << ", ";
	ss << "InstrumentID="	<< ordaction.InstrumentID ;
	LOG_INFO(ss.str());

	return ret;
}









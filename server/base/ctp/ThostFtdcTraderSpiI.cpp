/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file ThostFtdcTraderSpiI.cpp
///@brief 定义了客户端接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "ThostFtdcTraderSpiI.h"

int32_t tradingday;
//SyncRequest<int> CThostFtdcTraderSpiI::syncreq;

string CThostFtdcTraderSpiI::OnFrontDisconnected_Text(int nReason)
{
	switch(nReason)
	{
		case 0x1001:
			return "网络读失败";
		case 0x1002:
			return "网络写失败";
		case 0x2001:
			return "接收心跳超时";
		case 0x2002:
			return "发送心跳失败";
		case 0x2003:
			return "收到错误报文";
		default:
			return "";
	}
}
void CThostFtdcTraderSpiI::set_front(const char* front)
{
    this->front = front;
	//前置机器
	_pTdApi->RegisterFront((char*)front);
	LOG_DEBUG("front machine " << front);
}

void CThostFtdcTraderSpiI::Init(THOST_TE_RESUME_TYPE module)
{
	// 订阅私有流 
	// THOST_TERT_RESTART:从本交易日开始重传
	// THOST_TERT_RESUME:从上次收到的续传
	// THOST_TERT_QUICK:只传送登录后私有流的内容`
	_pTdApi->SubscribePrivateTopic(module);
	// 订阅公共流 TERT_RESUME:从上次收到的续传
	_pTdApi->SubscribePublicTopic(module);
	LOG_INFO("SubscribePublicTopic  SubscribePrivateTopic");
	_pTdApi->Init();
}


//查询账户信息
int CThostFtdcTraderSpiI::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField account;
	memset(&account, 0, sizeof(account));
	snprintf(account.BrokerID, sizeof(account.BrokerID), "%s", userLoginField.BrokerID);
	snprintf(account.InvestorID, sizeof(account.InvestorID), "%s", userLoginField.UserID);
	//snprintf(account.CurrencyID, sizeof(account.CurrencyID), "%s", "");

	int ret = 0;
	int reqid = getRequestId(); 
	syncreq.set(reqid, 0);
	
	if((ret = _pTdApi->ReqQryTradingAccount(&account, reqid)) != 0)
	{
		LOG_ERROR("ReqQryTradingAccount fail. ret=" << ret);
		return -1;
	}

	LOG_INFO("ReqQryTradingAccount account:" << account.InvestorID << " rquestid:" << reqid);

	return reqid;
}

//查询持仓信息
int CThostFtdcTraderSpiI::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField  position;
	memset(&position, 0, sizeof(position));
	snprintf(position.BrokerID, sizeof(position.BrokerID), "%s", userLoginField.BrokerID);
	snprintf(position.InvestorID, sizeof(position.InvestorID), "%s", userLoginField.UserID);
	//snprintf(position.InstrumentID, sizeof(position.InstrumentID), "%s", "IF1606");

	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);
	
	if((ret = _pTdApi->ReqQryInvestorPosition(&position, reqid)) != 0)
	{
		LOG_ERROR("ReqQryInvestorPosition fail.ret=" << ret);
		return -1;
	}

	LOG_INFO("ReqQryInvestorPosition account:" << position.InvestorID << " rquestid:" << reqid);
	return reqid;
}


//查询成交流水
int CThostFtdcTraderSpiI::ReqQryTrade()
{	
	CThostFtdcQryTradeField  done;                                           
	memset(&done, 0, sizeof(done));
	snprintf(done.BrokerID, sizeof(done.BrokerID), "%s", userLoginField.BrokerID); 
	snprintf(done.InvestorID, sizeof(done.InvestorID), "%s", userLoginField.UserID); 
		
	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);
	
	if((ret = _pTdApi->ReqQryTrade(&done, reqid)) != 0)
	{
		LOG_ERROR("ReqQryTrade fail.ret=" << ret);
		return -1;
	}

	LOG_INFO("ReqQryTrade account:" << done.InvestorID << " rquestid:" << reqid);
	return reqid;
}


//查询订单流水
int CThostFtdcTraderSpiI::ReqQryOrder()
{	
	CThostFtdcQryOrderField  order;                                           
	memset(&order, 0, sizeof(order));
	snprintf(order.BrokerID, sizeof(order.BrokerID), "%s", userLoginField.BrokerID); 
	snprintf(order.InvestorID, sizeof(order.InvestorID), "%s", userLoginField.UserID);        
			
	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);
	
	if((ret = _pTdApi->ReqQryOrder(&order, reqid)) != 0)
	{
		LOG_ERROR("ReqQryOrder fail.ret=" << ret);
		return -1;
	}

	LOG_INFO("ReqQryOrder account:" << order.InvestorID << " rquestid:" << reqid);
	return reqid;
}

//持仓明细
int CThostFtdcTraderSpiI::ReqQryInvestorPositionDetail()
{
    CThostFtdcQryInvestorPositionDetailField positiond;
	memset(&positiond, 0, sizeof(positiond));
	snprintf(positiond.BrokerID, sizeof(positiond.BrokerID), "%s", userLoginField.BrokerID);
	snprintf(positiond.InvestorID, sizeof(positiond.InvestorID), "%s", userLoginField.UserID);

	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);
	
	if((ret = _pTdApi->ReqQryInvestorPositionDetail(&positiond, reqid)) != 0)
	{
		LOG_ERROR("ReqQryInvestorPositionDetail fail.ret=" << ret);
		return -1;
	}

	LOG_INFO("ReqQryInvestorPositionDetail account:" << positiond.InvestorID << " rquestid:" << reqid);
	return reqid;
}

//查询合约
int CThostFtdcTraderSpiI::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField instrument;
	memset(&instrument, 0, sizeof(instrument));

	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);
	
	if((ret = _pTdApi->ReqQryInstrument(&instrument, reqid)) != 0)
	{
		LOG_ERROR("ReqQryInstrumentt fail. ret=" << ret);
		return -1;
	}
	LOG_INFO("ReqQryInstrument rquestid:" << reqid);

	return reqid;
}


//查询产品
int CThostFtdcTraderSpiI::ReqQryProduct()
{
	CThostFtdcQryProductField product;
	memset(&product, 0, sizeof(product));

	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);
	
	if((ret = _pTdApi->ReqQryProduct(&product, reqid)) != 0)
	{
		LOG_ERROR("ReqQryProduct fail.ret=" << ret);
		return -1;
	}
	LOG_INFO("ReqQryProduct  rquestid:" << reqid);

	return reqid;
}


int	CThostFtdcTraderSpiI::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField SettlementInfoConfirm;
	memset(&SettlementInfoConfirm, 0, sizeof(SettlementInfoConfirm));
	snprintf(SettlementInfoConfirm.BrokerID,   sizeof(SettlementInfoConfirm.BrokerID), "%s", userLoginField.BrokerID); 
	snprintf(SettlementInfoConfirm.InvestorID, sizeof(SettlementInfoConfirm.InvestorID), "%s", userLoginField.UserID);        
		
	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);
	
	if((ret = _pTdApi->ReqSettlementInfoConfirm(&SettlementInfoConfirm, reqid)) != 0)
	{
		LOG_ERROR("ReqSettlementInfoConfirm fail.ret=" << ret);
		return -1;
	}
	LOG_INFO("ReqSettlementInfoConfirm  rquestid:" << reqid);


	return reqid;
}

int	CThostFtdcTraderSpiI::ReqUserPasswordUpdate(char* oldpwd, char* newpwd)
{
	CThostFtdcUserPasswordUpdateField UserPasswordUpdate;
	memset(&UserPasswordUpdate, 0, sizeof(UserPasswordUpdate));
	/*
	///经纪公司代码
	TThostFtdcBrokerIDType  BrokerID;      
	///用户代码
	//TThostFtdcUserIDType    UserID;
	/////原来的口令
	//TThostFtdcPasswordType  OldPassword;
	/////新的口令
	//TThostFtdcPasswordType  NewPassword;
	 * */
	snprintf(UserPasswordUpdate.BrokerID, sizeof(UserPasswordUpdate.BrokerID), "%s", userLoginField.BrokerID); 
	snprintf(UserPasswordUpdate.UserID, sizeof(UserPasswordUpdate.UserID),"%s", userLoginField.UserID); 
	snprintf(UserPasswordUpdate.OldPassword, sizeof(UserPasswordUpdate.OldPassword), "%s", oldpwd); 
	snprintf(UserPasswordUpdate.NewPassword, sizeof(UserPasswordUpdate.NewPassword), "%s", newpwd); 
	
	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);
	
	if((ret = _pTdApi->ReqUserPasswordUpdate(&UserPasswordUpdate, reqid)) != 0)
	{
		LOG_ERROR("ReqUserPasswordUpdate fail.ret=" << ret);
		return -1;
	}
	LOG_INFO("ReqUserPasswordUpdate  rquestid:" << reqid);


	return reqid;
}


int CThostFtdcTraderSpiI::ReqQryOptionInstrTradeCost()
{
    CThostFtdcQryOptionInstrTradeCostField OptionInstrTradeCostField;
	memset(&OptionInstrTradeCostField, 0, sizeof(OptionInstrTradeCostField));

	snprintf(OptionInstrTradeCostField.BrokerID,   sizeof(OptionInstrTradeCostField.BrokerID), "%s", userLoginField.BrokerID); 
	snprintf(OptionInstrTradeCostField.InvestorID, sizeof(OptionInstrTradeCostField.InvestorID), "%s", userLoginField.UserID);        
		
	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);
	
	if((ret = _pTdApi->ReqQryOptionInstrTradeCost(&OptionInstrTradeCostField,  reqid)) != 0)
	{
		LOG_ERROR("ReqQryOptionInstrTradeCost fail.ret=" << ret);
		return -1;
	}
	LOG_INFO("ReqQryOptionInstrTradeCost  rquestid:" << reqid);


	return reqid;
}

int	CThostFtdcTraderSpiI::WaitReq(int requestid, int timeout)
{
	if(requestid < 0) return requestid;

	int count=0, val=10;
	while(syncreq.get(requestid) != 1 && count++ * val < timeout)
	{
		msleep(val);
	}
	if(syncreq.get(requestid) != 1) return -1;
	return requestid;
}

int CThostFtdcTraderSpiI::DoOrder(CThostFtdcInputOrderField & order)
{
    pthread_mutex_lock(&_mutex_order);
	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);

	string cellid = order.InvestorID;
	memcpy(order.InvestorID, m_fundid_cellid[order.InvestorID].c_str(), sizeof(order.InvestorID));
	memcpy(order.UserID,	 m_fundid_cellid[order.UserID].c_str()    , sizeof(order.UserID));
	LOG_DEBUG("cellid : " << cellid << " ------> " << m_fundid_cellid[cellid]);

	snprintf(order.OrderRef, sizeof(order.OrderRef), "%d", reqid);
	order.RequestID = reqid;

	if((ret = _pTdApi->ReqOrderInsert(&order, reqid)) != 0)
	{
		LOG_ERROR("ReqOrderInsert fail.ret=" << ret);
		return -1;
	}
	LOG_INFO("ReqOrderInsert  rquestid:" << reqid);

    pthread_mutex_unlock(&_mutex_order);
	return reqid;
}

int CThostFtdcTraderSpiI::DoCancleOrder(CThostFtdcInputOrderActionField& ordaction)
{
	int ret = 0;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);
	
	ordaction.RequestID =reqid;

	string cellid = ordaction.InvestorID;	
	memcpy(ordaction.InvestorID, m_fundid_cellid[ordaction.InvestorID].c_str(), sizeof(ordaction.InvestorID));
	memcpy(ordaction.UserID,	 m_fundid_cellid[ordaction.UserID].c_str()    , sizeof(ordaction.UserID));
	LOG_DEBUG("cellid : " << cellid << " ------> " << m_fundid_cellid[cellid]);

	if((ret = _pTdApi->ReqOrderAction(&ordaction, reqid)) != 0)
	{
		LOG_ERROR("ReqOrderAction fail.ret=" << ret);
		return -1;
	}
	LOG_INFO("ReqOrderAction  rquestid:" << reqid);

	return reqid;
}

CThostFtdcTraderSpiI::CThostFtdcTraderSpiI(string BrokerID, string AccountID, string AccountPWD, string DataDir,  string AuthCode, string UserProductInfo, string AppID)
{
    LOG_INFO("CThostFtdcTraderSpiI init executed.");
    _pTdApi = CThostFtdcTraderApi::CreateFtdcTraderApi(DataDir.c_str());

    strncpy(userLoginField.BrokerID, BrokerID.c_str(), sizeof(userLoginField.BrokerID));
    strncpy(userLoginField.UserID, AccountID.c_str(), sizeof(userLoginField.UserID));
    strncpy(userLoginField.Password, AccountPWD.c_str(), sizeof(userLoginField.Password));

	_pTdApi->RegisterSpi(this);

	InitFlag = false;
	memset(&reqAuthenticate,0,sizeof(reqAuthenticate));
	snprintf(reqAuthenticate.BrokerID,	     sizeof(reqAuthenticate.BrokerID), "%s", BrokerID.c_str());
	snprintf(reqAuthenticate.UserID,		 sizeof(reqAuthenticate.UserID), "%s", AccountID.c_str());
	snprintf(reqAuthenticate.AuthCode,		 sizeof(reqAuthenticate.AuthCode), "%s", AuthCode.c_str());
	snprintf(reqAuthenticate.UserProductInfo,sizeof(reqAuthenticate.UserProductInfo), "%s", UserProductInfo.c_str());
#ifdef  __CTP__VERSION__
    snprintf(reqAuthenticate.AppID,		        sizeof(reqAuthenticate.AppID), "%s", AppID.c_str());
#endif	
    LOG_DEBUG("BrokerID:" << userLoginField.BrokerID << " UserID:" << userLoginField.UserID << " Password:" << userLoginField.Password << " DataDir:" << DataDir << " AuthCode:" << AuthCode << " UserProductInfo:" << UserProductInfo << " AppID:" << AppID);
			
    pthread_mutex_init(&_mutex_order, NULL);
}

void CThostFtdcTraderSpiI::Init(string BrokenID, string AccountID, string AccountPWD, THOST_TE_RESUME_TYPE module, string AuthCode, string UserProductInfo, string AppID)
{
	CThostFtdcTraderSpiI(BrokenID, AccountID, AccountPWD, AuthCode, UserProductInfo, AppID);
	Init(module);
}

void CThostFtdcTraderSpiI::OnFrontConnected()
{
	printf("OnFrontConnected\n");
	//请求登陆
	int loginid = getRequestId();
	if(strlen(reqAuthenticate.AuthCode) < 1)
	{
		ReqUserLogin(&userLoginField, loginid);
		//LOG_DEBUG("logon the CTP td server. loginid:" << loginid);
	}
	else
	{
		_pTdApi->ReqAuthenticate(&reqAuthenticate, loginid);
		LOG_DEBUG("auth the CTP td server. id:" << loginid);
	}
}

void CThostFtdcTraderSpiI::OnFrontDisconnected(int nReason)
{
	// 当发生这个情况后,API会自动重新连接,客户端可不做处理
	InitFlag = false;
	LOG_DEBUG("OnFrontDisconnected executed[" << nReason << "]" << OnFrontDisconnected_Text(nReason));
	
}

void CThostFtdcTraderSpiI::OnHeartBeatWarning(int nTimeLapse)
{
	//需要断开重练一下 TODO
	LOG_DEBUG("OnHeartBeatWarning executed timespan:" << nTimeLapse);
}

//客户认证 认证失败重新登录
void CThostFtdcTraderSpiI::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	char ErrorMsg[243]={0};
	if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
	char UserID[16];
	if(pRspAuthenticateField != NULL) GBK2UTF8(pRspAuthenticateField->UserID, strlen(pRspAuthenticateField->UserID), UserID, sizeof(UserID));
	if(pRspAuthenticateField != NULL)
	{
#ifdef __CTP__VERSION__
		LOG_DEBUG("OnRspAuthenticate Return. BrokerID:" << pRspAuthenticateField->BrokerID << " UserID:" << pRspAuthenticateField->UserID << " UserProductInfo:" <<  pRspAuthenticateField->UserProductInfo  << " AppID:" << pRspAuthenticateField->AppID);
#else
		LOG_DEBUG("OnRspAuthenticate Return. BrokerID:" << pRspAuthenticateField->BrokerID << " UserID:" << pRspAuthenticateField->UserID << " UserProductInfo:" <<  pRspAuthenticateField->UserProductInfo);
#endif
    }
	
	if (pRspInfo!=NULL && pRspInfo->ErrorID == 0) 
	{
		LOG_DEBUG("OnRspAuthenticate ok. nRequestID:" << nRequestID << ", UserID:"<< UserID << ", ErrorMsg:" << ErrorMsg);
		int loginid = getRequestId();
		ReqUserLogin(&userLoginField, loginid);
	} 
	else 
	{
		//认证失败  重新认证
		_pTdApi->ReqAuthenticate(&reqAuthenticate, getRequestId());
		LOG_ERROR("OnRspAuthenticate fail. nRequestID:" << nRequestID << ", UserID:"<< UserID << ", ErrorMsg:" << ErrorMsg);
	}
}

void CThostFtdcTraderSpiI::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	struct tm tmp = {0};
	time_t now = time(NULL);
	localtime_r(&now,  &tmp);
	uint32_t local = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec;


	char ErrorMsg[243]={0};
	if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
	char UserID[16];
	if(pRspUserLogin!=NULL) GBK2UTF8(pRspUserLogin->UserID, strlen(pRspUserLogin->UserID), UserID, sizeof(UserID));

	//判断登录是否成功 失败告警 （继续登录）
	if (pRspInfo!=NULL &&  pRspInfo->ErrorID == 0) 
	{
		 InitFlag   = true;
		 tradingday = atoll(_pTdApi->GetTradingDay());
		 FrontID    = pRspUserLogin->FrontID;
		 SessionID  = pRspUserLogin->SessionID;

		 memset(&tmp, 0, sizeof(tmp));
		 strptime(pRspUserLogin->SHFETime , "%H:%M:%S", &tmp);
		 DiffSHFETime = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec - local;

		 memset(&tmp, 0, sizeof(tmp));
		 strptime(pRspUserLogin->DCETime , "%H:%M:%S", &tmp);
		 DiffDCETime = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec - local;

		 memset(&tmp, 0, sizeof(tmp));
		 strptime(pRspUserLogin->CZCETime , "%H:%M:%S", &tmp);
		 DiffCZCETime = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec - local;

		 memset(&tmp, 0, sizeof(tmp));
		 strptime(pRspUserLogin->FFEXTime , "%H:%M:%S", &tmp);
		 DiffFFEXTime = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec - local;

         if(DiffSHFETime + local == 0) DiffSHFETime = 0;
         if(DiffDCETime + local == 0) DiffDCETime = 0;
         if(DiffCZCETime + local == 0) DiffCZCETime = 0;
         if(DiffFFEXTime + local == 0) DiffFFEXTime = 0;

		LOG_INFO("SHFETime="	<<	pRspUserLogin->SHFETime
			<<" DCETime="	<<	pRspUserLogin->DCETime
			<<" CZCETime="	<<	pRspUserLogin->CZCETime
			<<" FFEXTime="	<<	pRspUserLogin->FFEXTime
			<<" TradingDay="<<	pRspUserLogin->TradingDay
			<<" LoginTime="	<<	pRspUserLogin->LoginTime
			<<" BrokerID="	<<	pRspUserLogin->BrokerID
			<<" UserID="	<<	pRspUserLogin->UserID
			<<" SystemName="<<	pRspUserLogin->SystemName
			<<" FrontID="	<<	pRspUserLogin->FrontID
			<<" SessionID="	<<	pRspUserLogin->SessionID
			<<" MaxOrderRef="<<	pRspUserLogin->MaxOrderRef
			<<" INETime="	<<	pRspUserLogin->INETime
			<<" ErrorMsg="	<<	ErrorMsg);
		LOG_INFO("DiffSHFE="<<DiffSHFETime<<" DiffDCE="<<DiffDCETime<<" DiffCZCE="<<DiffCZCETime<<" DiffFFEX="<<DiffFFEXTime);
	    on_login();
    } 
	else 
	{
		sleep(3); //3s后重试登录
		LOG_ERROR("OnRspUserLogin executed Error. nRequestID:" << nRequestID << ", UserID:"<< UserID << ", ErrorMsg:" << ErrorMsg);
		//登录失败再次请求登录
		int loginid = getRequestId();
		ReqUserLogin(&userLoginField, loginid); 
		//LOG_DEBUG("retry logon the CTP md server. loginid:" << loginid);
		return;
	}
}

void CThostFtdcTraderSpiI::on_login()
{
    //uint32_t c = 0;
    //while(WaitReq(ret1=ReqSettlementInfoConfirm(), 10000) < 0 && c++ < 3) 
    //{ 
    //    sleep(3); 
    //    LOG_DEBUG("ReqSettlementInfoConfirm fail!"); 
    //}
}

void CThostFtdcTraderSpiI::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	char ErrorMsg[243]={0};
	if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
	char UserID[16];
	if(pUserLogout != NULL ) GBK2UTF8(pUserLogout->UserID, strlen(pUserLogout->UserID), UserID, sizeof(UserID));
	LOG_DEBUG("OnRspUserLogout executed. nRequestID:" << nRequestID << ", UserID:"<< UserID << ", ErrorMsg:" << ErrorMsg);

	//这里在login
}

void CThostFtdcTraderSpiI::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///TODO  密码更新
	if(bIsLast) 
	{
		char ErrorMsg[243]={0};
		if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	}

}

void CThostFtdcTraderSpiI::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///TODO  交易密码更新
}

void CThostFtdcTraderSpiI::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if(pInputOrder != NULL)
	{
		string cellid = pInputOrder->InvestorID;
		memcpy(pInputOrder->InvestorID, m_cellid_fundid[pInputOrder->InvestorID].c_str(), sizeof(pInputOrder->InvestorID));
		memcpy(pInputOrder->UserID, m_cellid_fundid[pInputOrder->UserID].c_str(), sizeof(pInputOrder->UserID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}

	///报单录入请求响应
	if(bIsLast)
	{
		syncreq.set(nRequestID, 1);
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast);
	}
}

void CThostFtdcTraderSpiI::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///预埋单录入请求响应
}

void CThostFtdcTraderSpiI::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///预埋撤单录入请求响应
}

void CThostFtdcTraderSpiI::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if(pInputOrderAction != NULL)
	{
		string cellid = pInputOrderAction->InvestorID;
		memcpy(pInputOrderAction->InvestorID, m_cellid_fundid[pInputOrderAction->InvestorID].c_str(), sizeof(pInputOrderAction->InvestorID));
		memcpy(pInputOrderAction->UserID, m_cellid_fundid[pInputOrderAction->UserID].c_str(), sizeof(pInputOrderAction->UserID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}

	///报单操作请求响应
	if(bIsLast) 
	{
		char ErrorMsg[243]={0};
		if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	}
}

void CThostFtdcTraderSpiI::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///查询最大报单数量响应
}

void CThostFtdcTraderSpiI::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///投资者结算结果确认响应
	if(bIsLast) 
	{
		char ErrorMsg[243]={0};
		if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	}
}

void CThostFtdcTraderSpiI::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///删除预埋单响应
}

void CThostFtdcTraderSpiI::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///删除预埋撤单响应
}

void CThostFtdcTraderSpiI::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///执行宣告录入请求响应
}

void CThostFtdcTraderSpiI::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///执行宣告操作请求响应
}

void CThostFtdcTraderSpiI::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///询价录入请求响应
}

void CThostFtdcTraderSpiI::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///报价录入请求响应
}

void CThostFtdcTraderSpiI::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///报价操作请求响应
}

void CThostFtdcTraderSpiI::OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///批量报单操作请求响应
}
void CThostFtdcTraderSpiI::OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///申请组合录入请求响应
}

void CThostFtdcTraderSpiI::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if(pOrder != NULL)
	{
		string StatusMsg = pOrder->StatusMsg;	
		GBK2UTF8((char*)StatusMsg.c_str(), StatusMsg.size(), pOrder->StatusMsg, sizeof(pOrder->StatusMsg));
		
		string cellid = pOrder->InvestorID;
		memcpy(pOrder->InvestorID, m_cellid_fundid[pOrder->InvestorID].c_str(), sizeof(pOrder->InvestorID));
		memcpy(pOrder->UserID, m_cellid_fundid[pOrder->UserID].c_str(), sizeof(pOrder->UserID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}
	///请求查询报单响应
	if(bIsLast) 
	{
		char ErrorMsg[243]={0};
		if(pRspInfo != NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	
	}
}	

void CThostFtdcTraderSpiI::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询成交响应
	if(pTrade != NULL) 
	{
		string cellid = pTrade->InvestorID;
		memcpy(pTrade->InvestorID, m_cellid_fundid[pTrade->InvestorID].c_str(), sizeof(pTrade->InvestorID));
		memcpy(pTrade->UserID, m_cellid_fundid[pTrade->UserID].c_str(), sizeof(pTrade->UserID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}

	if(bIsLast) 
	{
		char ErrorMsg[243]={0};
		if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	
	}
}

void CThostFtdcTraderSpiI::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询投资者持仓响应

	if(pInvestorPosition != NULL) 
	{
		string cellid = pInvestorPosition->InvestorID;
		memcpy(pInvestorPosition->InvestorID, m_cellid_fundid[pInvestorPosition->InvestorID].c_str(), sizeof(pInvestorPosition->InvestorID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}
	if(bIsLast)
	{
		char ErrorMsg[243]={0};
		if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	
	}
	LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast);
}

void CThostFtdcTraderSpiI::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if(pTradingAccount != NULL) 
	{
		string cellid = pTradingAccount->AccountID;
		memcpy(pTradingAccount->AccountID, m_cellid_fundid[pTradingAccount->AccountID].c_str(), sizeof(pTradingAccount->AccountID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}

	///请求查询投资者持仓响应
	if(bIsLast)
	{
		char ErrorMsg[243]={0};
		if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	
	}
}

void CThostFtdcTraderSpiI::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询投资者响应

}

void CThostFtdcTraderSpiI::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询交易编码响应
}

void CThostFtdcTraderSpiI::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询合约保证金率响应
}

void CThostFtdcTraderSpiI::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询合约手续费率响应
}

void CThostFtdcTraderSpiI::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询交易所响应
}

void CThostFtdcTraderSpiI::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询产品响应
	if(bIsLast)
	{
		char ErrorMsg[243]={0};
		if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	
	}
}

void CThostFtdcTraderSpiI::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询合约响应
	if(bIsLast)
	{
		char ErrorMsg[243]={0};
		if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	}
}

void CThostFtdcTraderSpiI::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询行情响应

}

void CThostFtdcTraderSpiI::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询投资者结算结果响应
}

void CThostFtdcTraderSpiI::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询转帐银行响应

}

void CThostFtdcTraderSpiI::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询投资者持仓明细响应
	if(pInvestorPositionDetail != NULL) 
	{
		string cellid = pInvestorPositionDetail->InvestorID;
		memcpy(pInvestorPositionDetail->InvestorID, m_cellid_fundid[pInvestorPositionDetail->InvestorID].c_str(), sizeof(pInvestorPositionDetail->InvestorID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}
	if(bIsLast)
	{
		char ErrorMsg[243]={0};
		if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	
	}
}

void CThostFtdcTraderSpiI::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询客户通知响应
}

void CThostFtdcTraderSpiI::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询结算信息确认响应
}

void CThostFtdcTraderSpiI::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询投资者持仓明细响应
}

void CThostFtdcTraderSpiI::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///查询保证金监管系统经纪公司资金账户密钥响应
}

void CThostFtdcTraderSpiI::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询仓单折抵信息响应
}

void CThostFtdcTraderSpiI::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询投资者品种/跨品种保证金响应
}

void CThostFtdcTraderSpiI::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询交易所保证金率响应
}

void CThostFtdcTraderSpiI::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询交易所调整保证金率响应
}

void CThostFtdcTraderSpiI::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询汇率响应
}

void CThostFtdcTraderSpiI::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询二级代理操作员银期权限响应
}

void CThostFtdcTraderSpiI::OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询产品报价汇率
}

void CThostFtdcTraderSpiI::OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询产品组
}

void CThostFtdcTraderSpiI::OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询做市商合约手续费率响应
}

void CThostFtdcTraderSpiI::OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询做市商期权合约手续费响应
}

void CThostFtdcTraderSpiI::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询报单手续费响应
}

void CThostFtdcTraderSpiI::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询期权交易成本响应
	if(pOptionInstrTradeCost != NULL)
	{
		string cellid = pOptionInstrTradeCost->InvestorID;
		memcpy(pOptionInstrTradeCost->InvestorID, m_cellid_fundid[pOptionInstrTradeCost->InvestorID].c_str(), sizeof(pOptionInstrTradeCost->InvestorID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}

    ///请求查询投资者持仓响应
	if(bIsLast)
	{
		char ErrorMsg[243]={0};
		if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
		
		syncreq.set(nRequestID, 1);
		
		LOG_DEBUG("nRequestID=" << nRequestID << " bIsLast=" << bIsLast << " ErrorMsg:" << ErrorMsg);
	
	}
}

void CThostFtdcTraderSpiI::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询期权合约手续费响应
}

void CThostFtdcTraderSpiI::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询执行宣告响应
}

void CThostFtdcTraderSpiI::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询询价响应
}

void CThostFtdcTraderSpiI::OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询报价响应
}

void CThostFtdcTraderSpiI::OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询组合合约安全系数响应
}

void CThostFtdcTraderSpiI::OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询申请组合响应
}

void CThostFtdcTraderSpiI::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询转帐流水响应
}

void CThostFtdcTraderSpiI::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询银期签约关系响应
}

void CThostFtdcTraderSpiI::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///错误应答
}

void CThostFtdcTraderSpiI::OnRtnOrder(CThostFtdcOrderField *pOrder) 
{
	if(pOrder != NULL)
	{
		string StatusMsg = pOrder->StatusMsg;	
		GBK2UTF8((char*)StatusMsg.c_str(), StatusMsg.size(), pOrder->StatusMsg, sizeof(pOrder->StatusMsg));

		string cellid = pOrder->InvestorID;
		memcpy(pOrder->InvestorID, m_cellid_fundid[pOrder->InvestorID].c_str(), sizeof(pOrder->InvestorID));
		memcpy(pOrder->UserID, m_cellid_fundid[pOrder->UserID].c_str(), sizeof(pOrder->UserID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}
}

void CThostFtdcTraderSpiI::OnRtnTrade(CThostFtdcTradeField *pTrade) 
{
	///成交通知
	if(pTrade != NULL)
	{
		string cellid = pTrade->InvestorID;
		memcpy(pTrade->InvestorID, m_cellid_fundid[pTrade->InvestorID].c_str(), sizeof(pTrade->InvestorID));
		memcpy(pTrade->UserID, m_cellid_fundid[pTrade->UserID].c_str(), sizeof(pTrade->UserID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}
}

void CThostFtdcTraderSpiI::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) 
{
	///报单录入错误回报
	if(pInputOrder != NULL)
	{
		string cellid = pInputOrder->InvestorID;
		memcpy(pInputOrder->InvestorID, m_cellid_fundid[pInputOrder->InvestorID].c_str(), sizeof(pInputOrder->InvestorID));
		memcpy(pInputOrder->UserID, m_cellid_fundid[pInputOrder->UserID].c_str(), sizeof(pInputOrder->UserID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
	}
}

void CThostFtdcTraderSpiI::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) 
{
	if(pOrderAction != NULL)
	{
		string cellid = pOrderAction->InvestorID;
		memcpy(pOrderAction->InvestorID, m_cellid_fundid[pOrderAction->InvestorID].c_str(), sizeof(pOrderAction->InvestorID));
		memcpy(pOrderAction->UserID, m_cellid_fundid[pOrderAction->UserID].c_str(), sizeof(pOrderAction->UserID));
		LOG_DEBUG("cellid : " << cellid << " ------> " << m_cellid_fundid[cellid]);
		
		string StatusMsg = pOrderAction->StatusMsg;	
		GBK2UTF8((char*)StatusMsg.c_str(), StatusMsg.size(), pOrderAction->StatusMsg, sizeof(pOrderAction->StatusMsg));
	}

}

void CThostFtdcTraderSpiI::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) 
{
	///合约交易状态通知
}

void CThostFtdcTraderSpiI::OnRtnBulletin(CThostFtdcBulletinField *pBulletin) 
{
	///交易所公告通知
}

void CThostFtdcTraderSpiI::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) 
{
	///交易通知
}

void CThostFtdcTraderSpiI::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) 
{
	///提示条件单校验错误
}

void CThostFtdcTraderSpiI::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) 
{
	///执行宣告通知
}

void CThostFtdcTraderSpiI::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo) 
{
	///执行宣告录入错误回报
}

void CThostFtdcTraderSpiI::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo) 
{
	///执行宣告操作错误回报
}

void CThostFtdcTraderSpiI::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo) 
{
	///询价录入错误回报
}

void CThostFtdcTraderSpiI::OnRtnQuote(CThostFtdcQuoteField *pQuote) 
{
	///报价通知
}

void CThostFtdcTraderSpiI::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo) 
{
	///报价录入错误回报
}

void CThostFtdcTraderSpiI::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo) 
{
	///报价操作错误回报
}

void CThostFtdcTraderSpiI::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) 
{
	///询价通知
}

void CThostFtdcTraderSpiI::OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) 
{
	///保证金监控中心用户令牌
}
void CThostFtdcTraderSpiI::OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo) 
{
	///批量报单操作错误回报
}
void CThostFtdcTraderSpiI::OnRtnCombAction(CThostFtdcCombActionField *pCombAction) 
{	
	///申请组合通知
}

void CThostFtdcTraderSpiI::OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo) 
{
	///申请组合录入错误回报
}

void CThostFtdcTraderSpiI::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询签约银行响应
}

void CThostFtdcTraderSpiI::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询预埋单响应
}

void CThostFtdcTraderSpiI::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询预埋撤单响应
}

void CThostFtdcTraderSpiI::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询交易通知响应
}

void CThostFtdcTraderSpiI::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询经纪公司交易参数响应
}

void CThostFtdcTraderSpiI::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询经纪公司交易算法响应
}

void CThostFtdcTraderSpiI::OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///请求查询监控中心用户令牌
}

void CThostFtdcTraderSpiI::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) 
{
	///银行发起银行资金转期货通知
}

void CThostFtdcTraderSpiI::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) 
{
	///银行发起期货资金转银行通知
}

void CThostFtdcTraderSpiI::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) 
{
	///银行发起冲正银行转期货通知
}

void CThostFtdcTraderSpiI::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) 
{
	///银行发起冲正期货转银行通知
}

void CThostFtdcTraderSpiI::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	///期货发起银行资金转期货通知
}

void CThostFtdcTraderSpiI::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	///期货发起期货资金转银行通知
}

void CThostFtdcTraderSpiI::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
	///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
}

void CThostFtdcTraderSpiI::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) 
{
	///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
}

void CThostFtdcTraderSpiI::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) 
{
	///期货发起查询银行余额通知
}

void CThostFtdcTraderSpiI::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) 
{
	///期货发起银行资金转期货错误回报
}

void CThostFtdcTraderSpiI::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) 
{
	///期货发起期货资金转银行错误回报
}

void CThostFtdcTraderSpiI::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) 
{
	///系统运行时期货端手工发起冲正银行转期货错误回报
}

void CThostFtdcTraderSpiI::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) 
{
	///系统运行时期货端手工发起冲正期货转银行错误回报
}

void CThostFtdcTraderSpiI::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo) 
{
	///期货发起查询银行余额错误回报
}

void CThostFtdcTraderSpiI::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) 
{
	///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
}

void CThostFtdcTraderSpiI::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) 
{
	///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
}

void CThostFtdcTraderSpiI::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///期货发起银行资金转期货应答
}

void CThostFtdcTraderSpiI::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///期货发起期货资金转银行应答
}

void CThostFtdcTraderSpiI::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	///期货发起查询银行余额应答
}

void CThostFtdcTraderSpiI::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) 
{
	///银行发起银期开户通知
}

void CThostFtdcTraderSpiI::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount)
{
	///银行发起银期销户通知
}

void CThostFtdcTraderSpiI::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount)
{	
		///银行发起变更银行账号通知
}




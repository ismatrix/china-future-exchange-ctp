#include "CTPTradeDataProcess.h"




void CTPTradeDataProcess::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryOrder(pOrder, pRspInfo, nRequestID, bIsLast);
	if(NULL == pOrder) return;
	Json::Value root;
	get_CThostFtdcOrderField(root,  pOrder);
	Json::FastWriter fast_writer;	
	string ss = fast_writer.write(root);
	cout << ss << endl;
}

	///请求查询资金账户响应
void CTPTradeDataProcess::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryTradingAccount(pTradingAccount, pRspInfo, nRequestID, bIsLast);
	//模式1 insert	
	Json::Value root;
	get_CThostFtdcTradingAccountField(root,  pTradingAccount);
	Json::FastWriter fast_writer;	
	string ss = fast_writer.write(root);
	cout << ss << endl;	
}


///请求查询投资者持仓响应
void CTPTradeDataProcess::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryInvestorPosition(pInvestorPosition, pRspInfo, nRequestID, bIsLast);
	
	//insert	
	Json::Value root;
	get_CThostFtdcInvestorPositionField(root,  pInvestorPosition);
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);

	cout << ss << endl;
}

///请求查询投资者持仓明细响应
void CTPTradeDataProcess::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//insert
	Json::Value root;
	get_CThostFtdcInvestorPositionDetailField(root, pInvestorPositionDetail);
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	cout << ss << endl;
}

///请求查询成交响应
void CTPTradeDataProcess::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{	
	CThostFtdcTraderSpiI::OnRspQryTrade(pTrade, pRspInfo, nRequestID, bIsLast);
	Json::Value root;
	get_CThostFtdcTradeField(root, pTrade);
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);

	cout << ss << endl;
}

///请求查询产品响应
void CTPTradeDataProcess::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//2 update if not exist insert
	CThostFtdcTraderSpiI::OnRspQryProduct(pProduct, pRspInfo, nRequestID, bIsLast);
	if(NULL == pProduct) return;
	
	string insname = pProduct->ProductName;
	GBK2UTF8((char*)insname.c_str(), insname.size(), pProduct->ProductName, sizeof(pProduct->ProductName));
	IsValidDouble(pProduct->PriceTick); 
	IsValidDouble(pProduct->UnderlyingMultiple);    

	Json::Value s, ss;
	get_CThostFtdcProductField(s, pProduct);
	s["updatedate"] = stamp_to_isodate(time(NULL));
	s["requestid"]  = nRequestID;
	ss["$set"] = s;

	Json::Value q;
	q["productid"] = pProduct->ProductID;   
	
	CMongodb::Instance().ChangeCollection("PRODUCT");
	if(CMongodb::Instance().Update(ss.toStyledString(), q.toStyledString(), true) < 0)
	{
		printf("error: %s\n", CMongodb::Instance().GetLastErr());
	}
}

///合约信息
void CTPTradeDataProcess::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryInstrument(pInstrument, pRspInfo, nRequestID, bIsLast);
	if(NULL == pInstrument) return;
    
    if(pInstrument->ProductClass!='1'){
        return;
    }
	//name转化为utf8 
	string insname = pInstrument->InstrumentName;
	GBK2UTF8((char*)insname.c_str(), insname.size(), pInstrument->InstrumentName, sizeof(pInstrument->InstrumentName));
	IsValidDouble(pInstrument->PriceTick);  
	IsValidDouble(pInstrument->StrikePrice);    
	IsValidDouble(pInstrument->LongMarginRatio);    
	IsValidDouble(pInstrument->ShortMarginRatio);   
	IsValidDouble(pInstrument->UnderlyingMultiple); 
	
	Json::Value s, ss;
	get_CThostFtdcInstrumentField(s, pInstrument);
	s["updatedate"] = stamp_to_isodate(time(NULL));
	s["requestid"]  = nRequestID;
	ss["$set"] = s;

	Json::Value q;
	q["instrumentid"] = pInstrument->InstrumentID;

	CMongodb::Instance().ChangeCollection("INSTRUMENT");
	if(CMongodb::Instance().Update(ss.toStyledString(), q.toStyledString(), true) < 0)
	{
		printf("error: %s\n", CMongodb::Instance().GetLastErr());
	}
    LOG_DEBUG("last: " << bIsLast << " data: " <<  q.toStyledString());
}

void CTPTradeDataProcess::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspOrderInsert(pInputOrder, pRspInfo, nRequestID, bIsLast);
	char ErrorMsg[243];
	if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
	LOG_DEBUG("fundid=" << pInputOrder->InvestorID << " orderid=" << pInputOrder->OrderRef << " InstrumentID=" << pInputOrder->InstrumentID << " nRequestID=" << nRequestID << " ErrorMsg=" << ErrorMsg << " bIsLast=" << bIsLast);
	
}

void	CTPTradeDataProcess::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	CThostFtdcTraderSpiI::OnErrRtnOrderInsert(pInputOrder, pRspInfo);
	char ErrorMsg[243];
	if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));

	LOG_DEBUG("fundid=" << pInputOrder->InvestorID << " orderid=" << pInputOrder->OrderRef << " InstrumentID=" << pInputOrder->InstrumentID << " ErrorMsg=" << ErrorMsg);
	
}
/*
void* fun_doorder(void* arg)
{
	return NULL;
	CThostFtdcInputOrderField ord;
	memset(&ord, 0, sizeof(ord));
	strcpy(ord.BrokerID,	"9999");
	strcpy(ord.InvestorID,	"068074");
	strcpy(ord.UserID,	"068074");
	strcpy(ord.InstrumentID,"rb1610");
	
	ord.Direction			=THOST_FTDC_D_Buy;		//'0' THOST_FTDC_D_Buy '1' THOST_FTDC_D_Sell
	ord.CombOffsetFlag[0]	=THOST_FTDC_OF_Open;	//'0' THOST_FTDC_OF_Open '1' THOST_FTDC_OF_Close
	ord.CombHedgeFlag[0]	='1';					//'1' THOST_FTDC_HF_Speculation  '2' THOST_FTDC_HF_Arbitrage '3' THOST_FTDC_HF_Hedge  投机 套利 套保
	ord.VolumeTotalOriginal	=1;						//下单量
	
	ord.ContingentCondition=THOST_FTDC_CC_Immediately;	//立即触发
	
	//立即市价单
	//ord.OrderPriceType='1'; //'2';		//'1'任何价格 '2'限价
	//ord.LimitPrice = 2559;
	//ord.TimeCondition=THOST_FTDC_TC_IOC; //'3';

	//立即限价单
	ord.OrderPriceType='2'; //'2';		//'1'任何价格 '2'限价
	ord.LimitPrice = 2559;
	ord.TimeCondition=THOST_FTDC_TC_GFD; //'3';

	//发送触发单
	//ord.ContingentCondition=THOST_FTDC_CC_Immediately;	//立即触发
	//ord.StopPrice=1977;
	//ord.OrderPriceType='2'; //'2';		//'1'任何价格 '2'限价
	//ord.LimitPrice = 1977;
	//ord.TimeCondition=THOST_FTDC_TC_GFD; //'3';
	
	//固定值
	ord.VolumeCondition=THOST_FTDC_VC_AV;	//'1';
	ord.MinVolume=1;
	ord.ForceCloseReason='0';
	ord.IsAutoSuspend=0;
	ord.UserForceClose=0;
	
	int ret = ((CTPTradeDataProcess*)arg)->DoOrder(ord);                   
	if(((CTPTradeDataProcess*)arg)->WaitReq(ret) < 0)
	{
	    LOG_ERROR("doOrder fail or timeout.");
	}
	return NULL;
}


*/


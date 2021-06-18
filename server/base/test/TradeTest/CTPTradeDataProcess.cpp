/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file CTPTradeDataProcess.cpp
///@brief 处理交易数据  写db push下游
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
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
	LOG_DEBUG(ss);

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
	
	LOG_DEBUG(ss);
}

//成交通知
void CTPTradeDataProcess::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	//insert 成交流水	
	Json::Value root;
	get_CThostFtdcTradeField(root,  pTrade);
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);

	LOG_DEBUG(ss);
}


///请求查询投资者持仓响应
void CTPTradeDataProcess::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryInvestorPosition(pInvestorPosition, pRspInfo, nRequestID, bIsLast);
    if(NULL == pInvestorPosition) return;

	//insert	
	Json::Value root;
	get_CThostFtdcInvestorPositionField(root,  pInvestorPosition);
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);

	LOG_DEBUG(ss);
}

///请求查询投资者持仓明细响应
void CTPTradeDataProcess::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//insert
	Json::Value root;
	get_CThostFtdcInvestorPositionDetailField(root, pInvestorPositionDetail);
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);

	LOG_DEBUG(ss);
}

///请求查询成交响应
void CTPTradeDataProcess::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{	
	CThostFtdcTraderSpiI::OnRspQryTrade(pTrade, pRspInfo, nRequestID, bIsLast);
	Json::Value root;
	get_CThostFtdcTradeField(root, pTrade);
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);

	LOG_DEBUG(ss);
}

///请求查询产品响应
void CTPTradeDataProcess::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//2 update if not exist insert
	CThostFtdcTraderSpiI::OnRspQryProduct(pProduct, pRspInfo, nRequestID, bIsLast);
	
	Json::Value root;
	get_CThostFtdcProductField(root, pProduct);
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	
	LOG_DEBUG(ss);
}

///合约信息
void CTPTradeDataProcess::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//2 update if not exist insert
	CThostFtdcTraderSpiI::OnRspQryInstrument(pInstrument, pRspInfo, nRequestID, bIsLast);
	
	Json::Value root;
	get_CThostFtdcInstrumentField(root, pInstrument);
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	
	LOG_DEBUG(ss);
	/*
	Json::Value root_query;
	root_query["InstrumentID"] = p->InstrumentID;
	Json::FastWriter fast_writer;
	string ss_query = fast_writer.write(root);

	CMongodb& db = CMongodb::Instance();
	//db.("mongodb://127.0.0.1", "smartwin", "INSTRUMENT");
	if(db.Update(ss_set, ss_query, true) < 0)
	{
		LOG_ERROR("Insert fail. json:" << ss_set)
	}
	*/
}

void CTPTradeDataProcess::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspOrderInsert(pInputOrder, pRspInfo, nRequestID, bIsLast);
	char ErrorMsg[243];
	if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
	LOG_DEBUG("fundid=" << pInputOrder->InvestorID << " orderid=" << pInputOrder->OrderRef << " InstrumentID=" << pInputOrder->InstrumentID << " nRequestID=" << nRequestID << " ErrorMsg=" << ErrorMsg << " bIsLast=" << bIsLast);
	
}

void CTPTradeDataProcess::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	CThostFtdcTraderSpiI::OnErrRtnOrderInsert(pInputOrder, pRspInfo);
	char ErrorMsg[243];
	if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));

	LOG_DEBUG("fundid=" << pInputOrder->InvestorID << " orderid=" << pInputOrder->OrderRef << " InstrumentID=" << pInputOrder->InstrumentID << " ErrorMsg=" << ErrorMsg);
	
}

void CTPTradeDataProcess::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcTraderSpiI::OnRspQryOptionInstrTradeCost(pOptionInstrTradeCost, pRspInfo, nRequestID, bIsLast);
    if(NULL == pOptionInstrTradeCost) return;

    cout <<   "InstrumentID:" << pOptionInstrTradeCost->InstrumentID
         << "  HedgeFlag:" << pOptionInstrTradeCost->HedgeFlag
         << "  FixedMargin:" << pOptionInstrTradeCost->FixedMargin
         << "  MiniMargin:" << pOptionInstrTradeCost->MiniMargin
         << "  Royalty:" << pOptionInstrTradeCost->Royalty
         << "  ExchFixedMargin:" << pOptionInstrTradeCost->ExchFixedMargin
         << "  ExchMiniMargin:" << pOptionInstrTradeCost->ExchMiniMargin << endl;
}


void* fun_query_account(void* arg)
{
	((CTPTradeDataProcess*)arg)->ReqQryTradingAccount();
}

void* fun_query_position(void* arg)
{
	((CTPTradeDataProcess*)arg)->ReqQryInvestorPosition();
}

void* fun_query_done(void* arg)
{
	((CTPTradeDataProcess*)arg)->ReqQryTrade();
}

void* fun_query_order(void* arg)
{
	((CTPTradeDataProcess*)arg)->ReqQryOrder();
}


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
	
	/*
	//立即市价单
	ord.OrderPriceType='1'; //'2';		//'1'任何价格 '2'限价
	ord.LimitPrice = 2559;
	ord.TimeCondition=THOST_FTDC_TC_IOC; //'3';
	*/

	//立即限价单
	ord.OrderPriceType='2'; //'2';		//'1'任何价格 '2'限价
	ord.LimitPrice = 2559;
	ord.TimeCondition=THOST_FTDC_TC_GFD; //'3';

	/*
	//发送触发单
	ord.ContingentCondition=THOST_FTDC_CC_Immediately;	//立即触发
	ord.StopPrice=1977;
	ord.OrderPriceType='2'; //'2';		//'1'任何价格 '2'限价
	ord.LimitPrice = 1977;
	ord.TimeCondition=THOST_FTDC_TC_GFD; //'3';
	*/
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
}

int main(int argc, char* argv[])
{

	LOG_INIT(__FILE__, 4, 102400000, 10);

	//CTPTradeDataProcess ctp("0268","3000726", "342618");
	//ctp.set_front("tcp://180.166.125.146:41205");
	
	//CTPTradeDataProcess ctp("9999","068074", "123456");
	//ctp.set_front("tcp://180.168.146.187:10000");
	
    //CTPTradeDataProcess ctp("9999","089308", "123465");
    ///ctp.set_front("tcp://180.168.146.187:10000");
	
    	
    //CTPTradeDataProcess ctp("8080","80000528", "151015");
    //ctp.set_front("tcp://180.168.212.228:41205");
	
    CTPTradeDataProcess ctp("1025","317888", "hw170901");
    ctp.set_front("tcp://101.231.43.209:41205");
	

    ctp.Init(TERT_RESUME);


	//sleep(2);
    //ctp.ReqQryInstrument();

	sleep(2);
	ctp.ReqQryTradingAccount();

	sleep(2);
	ctp.ReqQryInvestorPosition();


	//sleep(2);
    //ctp.ReqQryOptionInstrTradeCost();
	//sleep(2);
	//ctp.ReqQryTrade();
	
	sleep(2);
	cout << "order = " << ctp.ReqQryOrder() << endl;
	
	/*
	pthread_t pid;
	for(int i=0; i<500; i++) pthread_create(&pid, NULL, fun_query_account,	&ctp);
	for(int i=0; i<500; i++) pthread_create(&pid, NULL, fun_query_position,	&ctp);
	for(int i=0; i<500; i++) pthread_create(&pid, NULL, fun_query_done,		&ctp);
	for(int i=0; i<500; i++) pthread_create(&pid, NULL, fun_query_order,		&ctp);
	for(int i=0; i<500; i++) pthread_create(&pid, NULL, fun_doorder,	&ctp);

	for(int i=0; i<500; i++) pthread_create(&pid, NULL, fun_query_account,	&ctp);
	for(int i=0; i<500; i++) pthread_create(&pid, NULL, fun_query_position,	&ctp);
	for(int i=0; i<500; i++) pthread_create(&pid, NULL, fun_query_done,		&ctp);
	for(int i=0; i<500; i++) pthread_create(&pid, NULL, fun_query_order,		&ctp);
	for(int i=0; i<500; i++) pthread_create(&pid, NULL, fun_doorder,	&ctp);
*/

	sleep(20);

	return 0;
}







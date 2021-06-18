/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file CTPTradeDataProcess.h
///@brief 默认处理交易数据  写db push下游
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __CTRADE_DATA_PROCESS__
#define	__CTRADE_DATA_PROCESS__

#include "ctp/ThostFtdcTraderSpiI.h"


//回调函数的模式处理。 设置了那些回调函数就处理那些操作
/*
CTPACCOUNT
CTPDONE
CTPPOSITION
FUND
INSTRUMENT
PRODUCT
STACCOUNT
STDONE
STPOSITION



//结构体转换json

Json::Value root;
//root["instrumentid"] = "fu1604";
Json::FastWriter fast_writer;
string ss = fast_writer.write(root);

两种db操作方法
1. insert
2. if exist update else insert

两种响应接口
1. push
2. 查询响应

1. 成交流水
2. 交易所公告

3. 持仓
4. 合约品种信息
5.  合约信息表
6. 账户资金

//使用
CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
CTPTradeDataProcess* TdSpi = new CTPTradeDataProcess(pUserApi, brokerid, account, pwd);
TdSpi.set_front("tcp://172.16.0.31:57205");
TdSpi.Init();

TdSpi.query();



char ErrorMsg[243];
	if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
	char UserID[16];
	if(pRspAuthenticateField != NULL) GBK2UTF8(pRspAuthenticateField->UserID, strlen(pRspAuthenticateField->UserID), UserID, sizeof(UserID));
	
*/

class CTPTradeDataProcess : public CThostFtdcTraderSpiI
{
	public:
		CTPTradeDataProcess()
		{

		}
		CTPTradeDataProcess(string BrokenID, string AccountID, string AccountPWD, string AuthCode="", string UserProductInfo="") : CThostFtdcTraderSpiI(BrokenID, AccountID, AccountPWD, AuthCode, UserProductInfo)
		{
		}
		
		void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

		/********************************************回调数据处理********************************************************************/
		///成交通知		
		virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
		
		///请求查询成交响应

		void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		
		///请求查询资金账户响应
		virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询投资者持仓响应
		virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询投资者持仓明细响应
		virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		
		///请求查询产品响应
		virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询合约响应
		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        //查询期权
        virtual void OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
};

/*

ctp柜台账户信息表，oracle.TRADINGACCOUNT
ctp柜台持仓信息表，oracle.INVESTOR_POSITION
ctp柜台成交信息表，oracle.CTPDONEINFO

合约品种信息表，oracle.PRODUCT
合约信息表，oracle.INSTRUMENT_INFO



金仕达资管账户信息表，oracle.STACCOUNTINFO
金仕达资管持仓信息表，oracle.STPOSITIONINFO
金仕达资管成交信息表，oracle.STDONEINFO



针对到每个账户金仕达:
1. 账户信息
2. 持仓信息
3. 合约静态信息(2)
4. 成交流水

针对到每个账户ctp:
1. 账户信息
2. 持仓信息
3. 合约静态信息(2)
4. 成交流水

*/





#endif













/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file CTPQueryObj.h
///@brief 默认处理交易数据  CTP原始数据处理
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __CTP_OBJ_POOLS____
#define __CTP_OBJ_POOLS____	

#include "ctp/ThostFtdcTraderSpiI.h"

#include "future/TradeDataMan.h"
#include "future/common.h"

#include "timer.h"
#include "SessionM.h"
#include "future/holidays.h"


#define IsValidDouble(value) if((value)<0 || (value)>1000000000000) value = 0; //0-------10000亿


class CTPQueryObj : public CThostFtdcTraderSpiI
{
	public:
		CTPQueryObj(string BrokenID, string AccountID, string AccountPWD, string DataDir="./", string AuthCode="", string UserProductInfo="", string AppID="") : CThostFtdcTraderSpiI(BrokenID, AccountID, AccountPWD, DataDir, AuthCode, UserProductInfo, AppID)
		{
		}
		virtual ~CTPQueryObj(){}
		//初始化并且  起内出数据处理线程
		virtual void Init(TE_RESUME_TYPE module);

		virtual int get_append_info(string& fundid, string& tradingday, string& requestid, ::CM::DoOrder& order);

		
		//下单返回
		void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
		//下单操作
		int DoOrder(const ::CM::DoOrder& order);
		
		//撤单返回
		void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo); 
		//撤单
		int CancleOrder(string fundid, string sessionid, string instrumentid, string privateno, string orderid);
		//改密码
		int updatePassword(string oldpwd, string newpwd);

		/********************************************回调数据处理********************************************************************/
				
		//交易所查询账户信息 返回json数据
		string queryAccount(int from);
		string queryAccountOne(int from, string fundid);
		string queryDone(int from, string fundid);
		string queryPosition(int from, string fundid);
		string queryOrder(int from, string fundid);

        string queryInsetruments(string ins);

		//委托查询响应
		virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询成交响应
		virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询资金账户响应
		virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询投资者持仓响应
		virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
        //查询持仓详细信息
        virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询产品响应
		virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询合约响应
		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		DataManageM<CtpAccountData, CThostFtdcTradingAccountField, CThostFtdcInvestorPositionField, CThostFtdcTradeField, CThostFtdcOrderField> ctp_data;
		DataManageM<CMAccountData, ::CM::Account, ::CM::Position, ::CM::Done, ::CM::Order> local_data; 

	    map<string, CThostFtdcInstrumentField> map_instruments;
	
};



#endif













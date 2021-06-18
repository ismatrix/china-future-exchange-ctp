#ifndef __CTP_TRADE_DATA_PROCESS__
#define __CTP_TRADE_DATA_PROCESS__

#include "ctp/ThostFtdcTraderSpiI.h"
#include "ctp/ThostFtdcMdSpiI.h"

#include "mongoc_json.h"

#define IsValidDouble(value) if((value)<0 || (value)>1000000000000) value = 0; //0-------10000��

class CTPTradeDataProcess : public CThostFtdcTraderSpiI
{
    public:
        CTPTradeDataProcess()
		{

		}
		~CTPTradeDataProcess()
		{

		}
		CTPTradeDataProcess(string BrokenID, string AccountID, string AccountPWD, string AuthCode="", string UserProductInfo="", string AppID="") : CThostFtdcTraderSpiI(BrokenID, AccountID, AccountPWD, "./", AuthCode, UserProductInfo, AppID)
        {
            LOG_DEBUG("BrokenID=" << BrokenID << " AccountID=" << AccountID << " AccountPWD=" << AccountPWD << " AuthCode=" << AuthCode << " UserProductInfo=" << UserProductInfo << " AppID=" << AppID);

        }
        
        void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
        
        ///�����ѯ�ʽ��˻���Ӧ
        virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
        
        ///�����ѯͶ���ֲ߳���Ӧ
        virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
        
        ///�����ѯͶ���ֲ߳���ϸ��Ӧ
        virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
        
        ///�����ѯ��Ʒ��Ӧ
        virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
        
        ///�����ѯ��Լ��Ӧ
        virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


		void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
};




#endif




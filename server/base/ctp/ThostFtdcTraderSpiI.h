/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file ThostFtdcTraderSpiI.h
///@brief 定义了客户端接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __THOSTFTDCTRADERSPI_H__
#define __THOSTFTDCTRADERSPI_H__

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <map>


#include "versions.h"
#include "sync_request.h"
#include "util.h"
#include "RollLog.h"
#include "convert.h"
#include "CtpJsonStruct.h"

using namespace std;
		
		
//type len
//这个线程里面处理 写db   push下游去

/* 使用
CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
CThostFtdcTraderSpiI* TdSpi = new CThostFtdcTraderSpiI(pUserApi, brokerid, account, pwd);
TdSpi.ReqAuthenticate();
TdSpi.Init();

pUserApi->RegisterSpi(&TdSpi);
// 订阅私有流 TERT_RESUME:从上次收到的续传
pUserApi->SubscribePrivateTopic(TERT_RESUME);
// 订阅公共流 TERT_RESUME:从上次收到的续传
pUserApi->SubscribePublicTopic(TERT_RESUME);
pUserApi->RegisterFront("tcp://172.16.0.31:57205");
pUserApi->Init();

//连接建立后登录
//登陆之前验证

CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
CThostFtdcTraderSpiI* TdSpi = new CThostFtdcTraderSpiI(pUserApi, brokerid, account, pwd);
TdSpi.set_front("tcp://172.16.0.31:57205");
TdSpi.Init();

//需要确认订阅流是在登录之后还是之前
*/

//start module
typedef THOST_TE_RESUME_TYPE TE_RESUME_TYPE;
#define TERT_RESTART THOST_TERT_RESTART			//retransmit from day start
#define TERT_RESUME  THOST_TERT_RESUME			//retransmit from last logout
#define TERT_QUICK   THOST_TERT_QUICK			//retransmit from login

//一个实例对用一个用户
class CThostFtdcTraderSpiI : public CThostFtdcTraderSpi
{
	public:
		SyncRequest<int> syncreq;	
		
        pthread_mutex_t _mutex_order;

		//fundid--->cellid
		map<string, string> m_fundid_cellid;
		//cellid--->fundid
		map<string, string> m_cellid_fundid;
        
        string front;

		string OnFrontDisconnected_Text(int nReason);
		int getRequestId()
		{
            //只需要保证每个连接内单调递增
			time_t tmp = time(NULL) +  3600*5; //时间前移5个小时 保证一个tradingday内单调递增
            int32_t hms = 1000000000 + st2seconds(tmp) * 10000;

			static int  id = 1; //1 --- 1000之间
			if(id > 9999) id = 1;
			
			return (hms+id++);
        
            //21亿内必须有重启    一天时间没问题的 
            //static int id = 1;
            //return id++;
		}
	
        int ReqUserLogin(CThostFtdcReqUserLoginField* puserLoginField, int loginid)
        {
            int ret = -1;
            while(ret < 0)
            {
                ret = _pTdApi->ReqUserLogin(puserLoginField, loginid);
		        LOG_DEBUG("logon the CTP td server. loginid:" << loginid << " ret=" << ret);
                if(ret < 0) sleep(3);
            }
            return 0;
        }


        virtual void on_login();

		//加载基金帐号的时候用到
		string get_account(){return userLoginField.UserID;}
		string get_brokerid(){return userLoginField.BrokerID;}
		string get_pwd(){ return userLoginField.Password;}
		string get_auth(){ return reqAuthenticate.AuthCode;}
		string get_user(){ return reqAuthenticate.UserProductInfo;}
		string get_appid()
        { 
            #ifdef __CTP__VERSION__
                    return reqAuthenticate.AppID;
            #else
                    return "";
            #endif
        }
		string get_front(){return front;}
        
        void   set_pwd(string pwd){ snprintf(userLoginField.Password, sizeof(userLoginField.Password), "%s", pwd.c_str());}
	public:
		CThostFtdcTraderSpiI(){}
		//~CThostFtdcTraderSpiI(){if(_pTdApi != NULL) {_pTdApi->Release(); _pTdApi = NULL;}}
		CThostFtdcTraderSpiI(string BrokenID, string AccountID, string AccountPWD, string DataDir = "./", string AuthCode="", string UserProductInfo="", string AppID="");	
	
		void Release(){_pTdApi->Release();};

		void Init(string BrokenID, string AccountID, string AccountPWD, THOST_TE_RESUME_TYPE module, string AuthCode="", string UserProductInfo="", string AppID="");	
	
		void set_front(const char* front);		//注册前置机器
		void Init(THOST_TE_RESUME_TYPE module);			//初始化
		
		bool get_InitFlag(){ return InitFlag;};
		CThostFtdcTraderApi* get_pTdApi(){ return _pTdApi;}
		
		
		/************************************查询动作 start*************************************************/
		//查询账户信息
		int ReqQryTradingAccount();
		//查询持仓信息
		int ReqQryInvestorPosition();
		//持仓明细
		int ReqQryInvestorPositionDetail();
		//查询成交流水
		int ReqQryTrade();
		//查询委托
		int ReqQryOrder();
		//查询产品
		int ReqQryProduct();
		//查询合约
		int ReqQryInstrument();
		
		//结算结果确认
		int	ReqSettlementInfoConfirm();
		//用户口令更新请求
		int ReqUserPasswordUpdate(char* oldpwd, char* newpwd);

        //请求查询期权交易成本
        int ReqQryOptionInstrTradeCost();

		//同步等待请求
		int	WaitReq(int32_t requestid, int timeout=3000);
		/************************************查询动作 end*************************************************/

		/***********************************交易操作 start**************************************************/
		//下单
		int DoOrder(CThostFtdcInputOrderField& order);
		//撤单
		int DoCancleOrder(CThostFtdcInputOrderActionField& ordaction);
		

		/***********************************交易操作 start*************************************************/
		
		///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
		virtual void OnFrontConnected();
		
		///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
		///@param nReason 错误原因
		///        0x1001 网络读失败
		///        0x1002 网络写失败
		///        0x2001 接收心跳超时
		///        0x2002 发送心跳失败
		///        0x2003 收到错误报文
		virtual void OnFrontDisconnected(int nReason);
			
		///心跳超时警告。当长时间未收到报文时，该方法被调用。
		///@param nTimeLapse 距离上次接收报文的时间
		virtual void OnHeartBeatWarning(int nTimeLapse);
		
		///客户端认证响应
		virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		

		///登录请求响应
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///登出请求响应
		virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///用户口令更新请求响应
		virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///资金账户口令更新请求响应
		virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///报单录入请求响应
		virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///预埋单录入请求响应
		virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///预埋撤单录入请求响应
		virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///报单操作请求响应
		virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///查询最大报单数量响应
		virtual void OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///投资者结算结果确认响应
		virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///删除预埋单响应
		virtual void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///删除预埋撤单响应
		virtual void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///执行宣告录入请求响应
		virtual void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///执行宣告操作请求响应
		virtual void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///询价录入请求响应
		virtual void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///报价录入请求响应
		virtual void OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///报价操作请求响应
		virtual void OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///批量报单操作请求响应
		virtual void OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询产品组
		virtual void OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询做市商合约手续费率响应
		virtual void OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询做市商期权合约手续费响应
		virtual void OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询报单手续费响应
		virtual void OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///交易所公告通知
		virtual void OnRtnBulletin(CThostFtdcBulletinField *pBulletin);
		///批量报单操作错误回报
		virtual void OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo);
		///申请组合录入请求响应
		virtual void OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询报单响应
		virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询成交响应
		virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者持仓响应
		virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询资金账户响应
		virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者响应
		virtual void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询交易编码响应
		virtual void OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询合约保证金率响应
		virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询合约手续费率响应
		virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询交易所响应
		virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询产品响应
		virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询合约响应
		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询行情响应
		virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者结算结果响应
		virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询转帐银行响应
		virtual void OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者持仓明细响应
		virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询客户通知响应
		virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询结算信息确认响应
		virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者持仓明细响应
		virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///查询保证金监管系统经纪公司资金账户密钥响应
		virtual void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询仓单折抵信息响应
		virtual void OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者品种/跨品种保证金响应
		virtual void OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询交易所保证金率响应
		virtual void OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询交易所调整保证金率响应
		virtual void OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询汇率响应
		virtual void OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询二级代理操作员银期权限响应
		virtual void OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询产品报价汇率
		virtual void OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询期权交易成本响应
		virtual void OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询期权合约手续费响应
		virtual void OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询执行宣告响应
		virtual void OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询询价响应
		virtual void OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询报价响应
		virtual void OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询组合合约安全系数响应
		virtual void OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询申请组合响应
		virtual void OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询转帐流水响应
		virtual void OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询银期签约关系响应
		virtual void OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///错误应答
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///报单通知
		virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

		///成交通知
		virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

		///报单录入错误回报
		virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

		///报单操作错误回报
		virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

		///合约交易状态通知
		virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

		///交易通知
		virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo);

		///提示条件单校验错误
		virtual void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder);

		///执行宣告通知
		virtual void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder);

		///执行宣告录入错误回报
		virtual void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo);

		///执行宣告操作错误回报
		virtual void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo);

		///询价录入错误回报
		virtual void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo);

		///报价通知
		virtual void OnRtnQuote(CThostFtdcQuoteField *pQuote);

		///报价录入错误回报
		virtual void OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo);

		///报价操作错误回报
		virtual void OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo);

		///询价通知
		virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

		///保证金监控中心用户令牌
		virtual void OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken);

		///申请组合通知
		virtual void OnRtnCombAction(CThostFtdcCombActionField *pCombAction);

		///申请组合录入错误回报
		virtual void OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo);

		///请求查询签约银行响应
		virtual void OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询预埋单响应
		virtual void OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询预埋撤单响应
		virtual void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询交易通知响应
		virtual void OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询经纪公司交易参数响应
		virtual void OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询经纪公司交易算法响应
		virtual void OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询监控中心用户令牌
		virtual void OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///银行发起银行资金转期货通知
		virtual void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer);

		///银行发起期货资金转银行通知
		virtual void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer);

		///银行发起冲正银行转期货通知
		virtual void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal);

		///银行发起冲正期货转银行通知
		virtual void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal);

		///期货发起银行资金转期货通知
		virtual void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer);

		///期货发起期货资金转银行通知
		virtual void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer);

		///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
		virtual void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal);

		///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
		virtual void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal);

		///期货发起查询银行余额通知
		virtual void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount);

		///期货发起银行资金转期货错误回报
		virtual void OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo);

		///期货发起期货资金转银行错误回报
		virtual void OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo);

		///系统运行时期货端手工发起冲正银行转期货错误回报
		virtual void OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo);

		///系统运行时期货端手工发起冲正期货转银行错误回报
		virtual void OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo);

		///期货发起查询银行余额错误回报
		virtual void OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo);

		///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
		virtual void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal);

		///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
		virtual void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal);

		///期货发起银行资金转期货应答
		virtual void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///期货发起期货资金转银行应答
		virtual void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///期货发起查询银行余额应答
		virtual void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///银行发起银期开户通知
		virtual void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount);

		///银行发起银期销户通知
		virtual void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount);

		///银行发起变更银行账号通知
		virtual void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount);
		
		/*
		//CThostFtdcRspUserLoginField rep_logon;
		TThostFtdcDateType  TradingDay;
		 ///登录成功时间
		TThostFtdcTimeType  LoginTime;
		///经纪公司代码
		//TThostFtdcBrokerIDType  BrokerID;
		/////用户代码
		//TThostFtdcUserIDType    UserID;
		/////交易系统名称
		//TThostFtdcSystemNameType    SystemName;
		/////前置编号
		//int FrontID;
		/////会话编号
		//int SessionID;
		/////最大报单引用                                       
		//TThostFtdcOrderRefType  MaxOrderRef;
		/////上期所时间
		//TThostFtdcTimeType  SHFETime;
		/////大商所时间
		//TThostFtdcTimeType  DCETime;
		/////郑商所时间
		//TThostFtdcTimeType  CZCETime;
		/////中金所时间
		//TThostFtdcTimeType  FFEXTime;
		/////能源中心时间
		//TThostFtdcTimeType  INETime;
		*/

		uint32_t DiffSHFETime;
		uint32_t DiffDCETime;
		uint32_t DiffCZCETime;
		uint32_t DiffFFEXTime;
		
		uint32_t GetExchangeTime(string market, uint32_t t=time(NULL))
		{
			if(market == "SHFE") return t + DiffSHFETime;
			else if(market == "DCE") return t + DiffDCETime;
			else if(market == "CZCE") return t + DiffCZCETime;
			else if(market == "CFFEX") return t + DiffFFEXTime;
			else return t;
		}
        
        int32_t GetTradingDay() { return atoll(_pTdApi->GetTradingDay());} 
		int FrontID;
		int SessionID;
		int32_t tradingday;
	private:
		 CThostFtdcTraderApi*    		_pTdApi;        //交易请求结构体
		 CThostFtdcReqUserLoginField 	userLoginField; //登录结构体
		 CThostFtdcReqAuthenticateField reqAuthenticate;//验证
		 bool							InitFlag;		//是否初始化完毕			
};


#endif




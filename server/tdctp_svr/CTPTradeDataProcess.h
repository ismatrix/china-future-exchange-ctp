/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file CTPTradeDataProcess.h
///@brief 默认处理交易数据  CTP原始数据处理
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __CTRADE_DATA_PROCESS__
#define	__CTRADE_DATA_PROCESS__

#include "spin_lock.h"
#include "SessionM.h"
#include "CTPObjPools.h"

//回调函数的模式处理。 设置了那些回调函数就处理那些操作
/*
//使用
CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
CTPTradeDataProcess* TdSpi = new CTPTradeDataProcess(pUserApi, brokerid, account, pwd);
TdSpi.set_front("tcp://172.16.0.31:57205");
TdSpi.Init();

char ErrorMsg[243];
	if(pRspInfo!=NULL) GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
	char UserID[16];
	if(pRspAuthenticateField != NULL) GBK2UTF8(pRspAuthenticateField->UserID, strlen(pRspAuthenticateField->UserID), UserID, sizeof(UserID));
	
	1. 漏洞成交 持仓  账户 这个异步会导致3个信息可能不同步。 只有自己计算账户持仓才能解决。
	2. 漏洞主动查询之后可能没有回复，导致这个成交信息没有下发。 启定时轮询线程可解决。
	3. 可能问题 数据库操作，push下游在回调这个线程中做，可能会影响这个线程。
	4. 问题， 行情变化会时刻影响持仓，账户信息的变化。 CTP账户信息时刻在变但是sungard只有在成交的时候才会变化。
*/


//数据库获取映射信息


class CTPTradeDataProcess : public CTPQueryObj
{
	public:

		CTPTradeDataProcess(string BrokenID, string AccountID, string AccountPWD, string DataDir="./", string AuthCode="", string UserProductInfo="", string AppID="") : CTPQueryObj(BrokenID, AccountID, AccountPWD, DataDir, AuthCode, UserProductInfo, AppID) { _lock=0;}
		virtual ~CTPTradeDataProcess(){}
		//初始化并且  起内出数据处理线程
		void Init(TE_RESUME_TYPE module);
		void Run();         //线程处理主函数                                  

		/********************************************回调数据处理********************************************************************/
		
        //登录成功的回调
        void on_login();
        
        //报单通知
		virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
		///成交通知		
		virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
		///合约交易状态通知
		virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);
		///交易所公告通知
		virtual void OnRtnBulletin(CThostFtdcBulletinField *pBulletin);
		virtual int get_append_info(string& fundid, string& tradingday, string& requestid, ::CM::DoOrder& order);
		
		void set_need_success_tradeid(string tradeid)
		{
			spinlock_lock(&_lock);
			need_success_tradeid = tradeid;
			spinlock_unlock(&_lock);
		}

		string get_need_success_tradeid()
		{
			spinlock_lock(&_lock);
			string trade = need_success_tradeid;
			spinlock_unlock(&_lock);
			return trade;
		}
		
		void set_DataDir2(string dir = "./tmp")
		{
			DataDir2 = dir;
		}
		typedef void (CTPTradeDataProcess::*_functiontype)(Json::Value&);

	private:
		void LoadDb(const char* table, uint32_t tradingday, _functiontype  callback, Json::Value& q);
		void LoadFund(Json::Value& root);		
		void LoadVirtualFund(Json::Value& root);
		void LoadAccount(Json::Value& root);
		void LoadPosition(Json::Value& root);
		void LoadDone(Json::Value& root);
		void LoadOrder(Json::Value& root);
		void LoadDayBar(Json::Value& root);
        void LoadInstrument(Json::Value& root);

		vector<string> vecfundid;		//主张号
		map<string, ::CM::DayBar > map_daybar;	//日k数据列表
		
		//在成交推送里面要确保最后一个成交能成功查询到持仓账户信息推送下去
		string need_success_tradeid;
		spinlock_t _lock;

		string DataDir2; //临时交换数据对象目录

		//void get_append_info(string& fundid, string& tradingday, string& requestid, Json::Value& root);

};

extern CTPTradeDataProcess * g_syncsp; 

	
#endif













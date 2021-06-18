/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file SunGardTradeDataProcess.h
///@brief 金仕达的原始数据处理 入库等
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef __SUNGARD_TRADE_DATA_PROCESS__
#define	__SUNGARD_TRADE_DATA_PROCESS__

#include "util.h"

#include "sungard/KFTSTraderSpiI.h"

#include "timer.h"
#include "singleton.h"
#include "SessionM.h"
#include "future/holidays.h"
#include "future/common.h"
#include "future/TradeDataMan.h"

//接受通知 并处理
class SunGardTradeDataProcess : public CKFTSTraderSpiI
{
	public:
		typedef void (SunGardTradeDataProcess::*_functiontype)(Json::Value&);

		SunGardTradeDataProcess(const char* user, const char* pwd):CKFTSTraderSpiI(user, pwd) 
		{
		};
		void Run(); 	
		void Init();

		//下单
		int DoOrder(const ::CM::DoOrder& order);
		int CancleOrder(string fundid, string sessionid, string instrumentid, string privateno, string orderid);
		
		int updatePassword(string oldpwd, string newpwd);

		//交易所查询账户信息 返回json数据
		string queryAccount(int from);
		string queryAccountOne(int from, string fundid);
		string queryDone(int from, string fundid);
		string queryPosition(int from, string fundid);
		string queryOrder(int from, string fundid);


		static void OnErrOrder(void* p,  int req_no, int row_no);
		static void OnOrder(void* p,  int req_no, int row_no);
		static void OnDone(void* p,  int req_no, int row_no);
	
		
		//static void OnQuerySecCodeRsp(void* p,  int req_no, int row_no);
		static void OnQueryOrder(void* p,  int req_no, int row_no);
		//static void OnQueryEtfBaseInfo(void* p,  int req_no, int row_no);
		//static void OnQueryEtfBasketInfo(void* p,  int req_no, int row_no);
		static void OnQueryDone(void* p,  int req_no, int row_no);
		static void OnQueryAccount(void* p,  int req_no, int row_no);
		static void OnQueryPosition(void* p,  int req_no, int row_no);	 
	

		// STSecCodeInfo 	 seccode_rsp;		//证券基本信息
		// STEtfBaseInfo 	 etfbaseinfo_rsp;	//ETF基本信息
		// STEtfBasketInfo  etfbasketinfo_rsp;	//证券代码信息
		// STOrderInfo 	 orderinfo_rsp;		//委托信息 	order_no 系统流水号为key
		// STDoneInfo		 doneinfo_rsp;		//成交信息 	流水id
		// STAccountInfo 	 account_rsp;		//账户信息  cell_id为key
		// STPositionInfo 	 position_rsp;		//持仓信息  cell_id+marketcode+sec_code为key

		int get_append_info(string& fundid, string& tradingday, string& requestid, ::CM::DoOrder& order);
	
		static DataManageM<SunAccountData, STAccountInfo, STPositionInfo, STDoneInfo, STOrderInfo> sun_data;
		static DataManageM<CMAccountData, ::CM::Account, ::CM::Position, ::CM::Done, ::CM::Order> local_data; 
	
	private:
		void LoadDb(const char* table, uint32_t tradingday, _functiontype  callback, Json::Value& q);
		void LoadFund(Json::Value& root);		
		void LoadVirtualFund(Json::Value& root);
		void LoadAccount(Json::Value& root);
		void LoadPosition(Json::Value& root);
		void LoadDone(Json::Value& root);
		void LoadOrder(Json::Value& root);
		void LoadDayBar(Json::Value& root);
		
		vector<string> vecfundid;

		map<string, ::CM::DayBar > map_daybar;	//日k数据列表
		map<string, CThostFtdcInstrumentField> map_instruments;
	
		static map<string, double>  prebalance;
		static map<string, double> premargin;

		CMongodb m_db;
		int dbset(string key, string value);
		string dbget(string key);
		void get_append_info(string& fundid, string& tradingday, string& requestid, Json::Value& root);

		//通过原始的done order运算子账户信息
		void update_sub_account();
		//结算
		int settlement();
		//更新子账户的结算数据到数据库
		void update_sub_account_2db(); 
};

extern SunGardTradeDataProcess * g_syncsp; 

#endif

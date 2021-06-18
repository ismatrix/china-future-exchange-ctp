/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file SunGardTradeDataProcess.h
///@brief 默认的数据处理函数
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef __SUNGARD_TRADE_DATA_PROCESS__
#define	__SUNGARD_TRADE_DATA_PROCESS__


#include "util.h"
#include "sungard/KFTSTraderSpiI.h"

//一个帐号一个程序 这里用全局唯一
//静态函数作为函数指针 结构不能变化。 回调静态只能用一个实例

/*
	SunGardTradeDataProcess sp;
	sp.set_front(1, "tcp:....");
	sp.Init();
	
账户信息
STAccountInfo
资金信息
STPositionInfo
成交
STDoneInfo  
委托信息
STOrderInfo 
*/
	
class SunGardTradeDataProcess : public CKFTSTraderSpiI
{
	public:
		SunGardTradeDataProcess(const char* user, const char* pwd):CKFTSTraderSpiI(user, pwd) {};
		void Init()
		{
			Set("OnErrOrder", OnErrOrder);
			Set("OnOrder", OnOrder);
			Set("OnDone", OnDone);
			Set("OnAccount", OnAccount);
			Set("OnPosition", OnPosition);
			Set("OnQuerySecCodeRsp", OnQuerySecCodeRsp);
			Set("OnQueryOrder", OnQueryOrder);
			Set("OnQueryEtfBaseInfo", OnQueryEtfBaseInfo);
			Set("OnQueryEtfBasketInfo", OnQueryEtfBasketInfo);
			Set("OnQueryDone", OnQueryDone);
			Set("OnQueryAccount", OnQueryAccount);
			Set("OnQueryPosition", OnQueryPosition);
			CKFTSTraderSpiI::Init();
		}
		
		static void OnErrOrder(void* p,  int req_no, int row_no);
		static void OnOrder(void* p,  int req_no, int row_no);
		static void OnDone(void* p,  int req_no, int row_no);
		static void OnAccount(void* p,  int req_no, int row_no);
		static void OnPosition(void* p,  int req_no, int row_no);
		static void OnQuerySecCodeRsp(void* p,  int req_no, int row_no);
		static void OnQueryOrder(void* p,  int req_no, int row_no);
		static void OnQueryEtfBaseInfo(void* p,  int req_no, int row_no);
		static void OnQueryEtfBasketInfo(void* p,  int req_no, int row_no);
		static void OnQueryDone(void* p,  int req_no, int row_no);
		static void OnQueryAccount(void* p,  int req_no, int row_no);
		static void OnQueryPosition(void* p,  int req_no, int row_no);	 
	
	private:
		STSecCodeInfo 	 seccode_rsp;		//证券基本信息
		STEtfBaseInfo 	 etfbaseinfo_rsp;	//ETF基本信息
		STEtfBasketInfo  etfbasketinfo_rsp;	//证券代码信息
		STOrderInfo 	 orderinfo_rsp;		//委托信息 	order_no 系统流水号为key
		STDoneInfo		 doneinfo_rsp;		//成交信息 	流水id
		STAccountInfo 	 account_rsp;		//账户信息  cell_id为key
		STPositionInfo 	 position_rsp;		//持仓信息  cell_id+marketcode+sec_code为key
};


//记录账户变化流水 和持仓流水
//资金曲线 和持仓曲线

/*
账户信息
STAccountInfo
资金信息
STPositionInfo
成交
STDoneInfo  
委托信息
STOrderInfo 
*/





#endif

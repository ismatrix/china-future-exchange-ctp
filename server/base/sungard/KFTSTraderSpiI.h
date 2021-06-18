/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file KFTSTraderSpiI.h
///@brief 处理交易数据  写db push下游
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef __SUNGARD_TRADE_DATA_SPII__
#define	__SUNGARD_TRADE_DATA_SPII__


#include <iostream>
#include <string>
#include <map>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "StJsonStruct.h"
#include "sync_request.h"
#include "convert.h"
/*	使用 自动登录 自动连接 订阅等
	CKFTSTraderSpiI sp;
	sp.set_front(1, "tcp:....");
	sp.Init();
*/


/*
API与前置通过心跳的设置来检查连接是否正常。API和前置都会向对方发送心跳请求，收到请求时会给对方心跳应答。
如果心跳请求后超出设定时间没有收到应答，则认为发生心跳丢失。如果设定时间内没有收到对方心跳请求，同样认为发生心跳丢失。
心跳丢失次数超出最大可容忍的心跳丢失次数，则认为连接断开。
控制API与前置的连接上的心跳主要有以下参数：
KFTS_OPT_HEARTBEAT_INTERVAL 发送心跳请求间隔(s)
KFTS_OPT_HEARTBEAT_TIMEOUT 接收心跳应答超时时间(s)
KFTS_OPT_HEARTBEAT_MAX_LOSS 最大可容忍心跳丢失次数
如果发送心跳请求间隔、接收心跳应答超时时间、接收心跳请求超时时间都设置为0，则双方都不发送心跳，不通过心跳检查连接是否正常
*
*
* 金仕达这里的基金id只能是整数  与 cell_id 对应. 没有做int---string映射。 因为会非常麻烦。
*
*
*/
//一个帐号一个程序 这里用全局唯一
//静态函数作为函数指针 结构不能变化。 回调静态只能用一个实例
class CKFTSTraderSpiI
{
	public:
		CKFTSTraderSpiI(const char* user, const char* pwd);
		~CKFTSTraderSpiI();
		int Init();
		int Init_Thread();
		int  ThreadRun();
		void UnInit();
		void ReInit();
				
		/*****************************主动操作函数************************************************/
		// 查询账户资金信息
		int  DoQueryAccount();
		// 查询持仓信息
		int DoQueryPosition();
		//查询委托
		int DoQueryOrder();
		// 查询成交信息
		int DoQueryTrade();
		// 查询ETF基本信息
		int DoQueryEtfBaseInfo();
		// 查询证券代码信息
		int DoQuerySecCode(const char* security_code);
		
		// 使用主连接下单
		int DoOrder(STOrder* orders, int count);
		//// 按委托私有号撤单
		int DoOrderCancel(STInputOrderAction* cancel);
		//// 使用辅连接下单
		int DoOrderEx(STOrder* orders, int count);

		//同步等待处理请求接口
		static int WaitReq(int32_t requestid, int timeout=3000);


		/***************************回调结果处理**************************************/
		// 与前置机建立连接结果回调
		static void OnConnected(APIHandle handle, const STRspMsg *p_rsp_info);
		// 登录结果回调
		static void OnLogined(APIHandle handle, const STTraderLoginRsp *p_login_rsp, const STRspMsg *p_rsp_info);
		// 订阅结果回调
		static void OnSubscribeRtn(APIHandle handle, const STRspMsg* msg);
		// 错误回调
		static void OnError(APIHandle handle, const STRspMsg *p_rsp_info, int req_no);
		//连接断开重连回调
		static int OnReconnect(APIHandle handle, const CONNHandle conn_handle, int front_id, const STRspMsg *p_rsp_info);
		
		//注册下单和撤单的报错回调函数
		static void OnErrOrder(APIHandle handle, const STOrderInfo *p_order_info, int type);
		 //委托推送回调
		static void OnOrder(APIHandle handle, const STOrderInfo *p_order_info, int type);
		 // 成交推送回调
		static void OnDone(APIHandle handle, const STDoneInfo *p_done_info);
		 // 资金变动回调
		static void OnAccount(APIHandle handle, const STAccountInfo* p_account_info);
		 // 持仓变动回调
		static void OnPosition(APIHandle handle, const STPositionInfo* p_position_info);
		//证券代码信息查询结果回调
		static void OnQuerySecCodeRsp(APIHandle handle, const STSecCodeInfo *p_seccode_rsp, int req_no, int row_no);
		// 委托查询结果回调
		static void OnQueryOrder(APIHandle handle, const STOrderInfo *p_orderinfo_rsp, int req_no, int row_no);
		 // ETF基本信息查询结果回调
		static void OnQueryEtfBaseInfo(APIHandle handle, const STEtfBaseInfo *p_etfbaseinfo_rsp, int req_no, int row_no);
		 // ETF篮子信息查询结果回调
		static void OnQueryEtfBasketInfo(APIHandle handle, const STEtfBasketInfo *p_etfbasketinfo_rsp, int req_no, int row_no);
		 // 成交查询结果回调
		static void OnQueryDone(APIHandle handle, const STDoneInfo *p_doneinfo_rsp, int req_no, int row_no);
		 // 账户资金查询结果回调
		static void OnQueryAccount(APIHandle handle, const STAccountInfo *p_account_rsp, int req_no, int row_no);
		//持仓查询结果回调
		static void OnQueryPosition(APIHandle handle, const STPositionInfo *p_position_rsp, int req_no, int row_no);

		int  RegistCallBack()
		{
			#define FAIL_PROCESS(fun) {LOG_ERROR("register " << fun << " fail.[" << err_msg.error_no << ":" << err_msg.msg << "]"); return -1;}
			// 注册回调函数
			if(!KFTSAPI_RegConnectedRtnCallBack(apiHandle, OnConnected, NULL, &err_msg)) 				FAIL_PROCESS("OnConnected");
			if(!KFTSAPI_RegLoginedRtnCallBack(apiHandle, OnLogined, NULL, &err_msg))                    FAIL_PROCESS("OnLogined");
			if(!KFTSAPI_RegSubscribeRtnCallBack(apiHandle, OnSubscribeRtn, NULL, &err_msg))             FAIL_PROCESS("OnSubscribeRtn");
			if(!KFTSAPI_RegErrMsgCallBack(apiHandle, OnError, NULL, &err_msg))                          FAIL_PROCESS("OnError");
			if(!KFTSAPI_RegReconnectRtnCallBack(apiHandle, OnReconnect, NULL, &err_msg))                FAIL_PROCESS("OnReconnect");
			//注册下单和撤单的报错回调函数 
			if(!KFTSAPI_RegErrOrderRtnCallBack(apiHandle, OnErrOrder, NULL, &err_msg))                  FAIL_PROCESS("OnErrOrder");
			//注册委托回报推送处理回调函数 
			if(!KFTSAPI_RegOrderRtnCallBack(apiHandle, OnOrder, NULL, &err_msg))                        FAIL_PROCESS("OnOrder");
			//注册成交推送处理回调函数 
			if(!KFTSAPI_RegDoneRtnCallBack(apiHandle, OnDone, NULL, &err_msg))                          FAIL_PROCESS("OnDone");
			//注册资金变动推送处理回调函数 
			if(!KFTSAPI_RegAccountRtnCallBack(apiHandle, OnAccount, NULL, &err_msg))                    FAIL_PROCESS("OnAccount");
			//注册持仓变动推送处理回调函数 
			if(!KFTSAPI_RegPositionRtnCallBack(apiHandle, OnPosition, NULL, &err_msg))                  FAIL_PROCESS("OnPosition");
			
			//查询回调
			if(!KFTSAPI_RegQuerySecCodeCallBack(apiHandle, OnQuerySecCodeRsp, NULL, &err_msg))          FAIL_PROCESS("OnQuerySecCodeRsp");
			if(!KFTSAPI_RegQueryOrderCallBack(apiHandle, OnQueryOrder, NULL, &err_msg))                 FAIL_PROCESS("OnQueryOrder");
		
			if(!KFTSAPI_RegQueryDoneCallBack(apiHandle, OnQueryDone, NULL, &err_msg))                   FAIL_PROCESS("OnQueryDone");
			if(!KFTSAPI_RegQueryEtfBaseInfoCallBack(apiHandle, OnQueryEtfBaseInfo, NULL, &err_msg))     FAIL_PROCESS("OnQueryEtfBaseInfo");
			if(!KFTSAPI_RegQueryEtfBasketInfoCallBack(apiHandle, OnQueryEtfBasketInfo, NULL, &err_msg)) FAIL_PROCESS("OnQueryEtfBasketInfo");
			if(!KFTSAPI_RegQueryAccountCallBack(apiHandle, OnQueryAccount, NULL, &err_msg))             FAIL_PROCESS("OnQueryAccount");
			if(!KFTSAPI_RegQueryPositionCallBack(apiHandle, OnQueryPosition, NULL, &err_msg))           FAIL_PROCESS("OnQueryPosition");
			return 0;
		}

	public:
		typedef void (*_Fun)(void* p,  int req_no, int row_no);
		typedef map<string, _Fun> mapTypeFun;
		
		static mapTypeFun _map_callback_fun;
		static void Set(string key, _Fun fun)
		{
			if(fun != NULL) _map_callback_fun[key] = fun;
		}
		static void CallBack(APIHandle handle, string key, void* p, int req_no=0, int row_no=0)
		{
			if(p == NULL) 
			{
				LOG_ERROR(key << " data point is NULL. row_no=" << row_no);
				return;
			}
			else
			{
				LOG_INFO(key << " execeted. row_no=" << row_no);
			}
			
			if(row_no == EMPTY_NO)
			{
				CKFTSTraderSpiI* p = get_instence_by_APIHandle(handle);
				if(p != NULL) p->syncreq.set(req_no, 1); 
				return ;
			}
		
			//p 可能是空的
			mapTypeFun::iterator itr = _map_callback_fun.find(key);
			if(itr != _map_callback_fun.end())
			{
				(*itr->second)(p, req_no, row_no);
				return;
			}
			LOG_INFO(key << " not process.");
		}
		
		void set_front(const int no, const char* s)
		{
			front[no] = s;
		}
	
		static int getRequestId()
		{
			time_t tmp = time(NULL) + 3600 * 5;

			int32_t hms = atoll(stamptostr(tmp, "1%H%M%S000").c_str());

			static int  id = 1; //1 --- 1000之间
			if(id > 999) id = 1;
			
			return (hms+id++);
		}
		
		//加载基金表的时候用到
		string get_account(){ return login.cust_no; }
		
		bool get_InitFlag() { return  InitFlag;}

	public:
		//另外的线程跑回调 回调函数需要可重入
		static pthread_mutex_t _mutex;
		//APIHandle和实例映射表。 后面回调中根据此获取实例
		static map<APIHandle, CKFTSTraderSpiI*> map_instence_APIHandle;
		static CKFTSTraderSpiI* get_instence_by_APIHandle(APIHandle ah)
		{
			CKFTSTraderSpiI* sp = NULL;
			pthread_mutex_lock(&_mutex);
			if(map_instence_APIHandle.find(ah) != map_instence_APIHandle.end())
			{
				sp = map_instence_APIHandle[ah];
			}
			pthread_mutex_unlock(&_mutex);
			return sp;
		}
		static void set_map_instence_APIHandle(APIHandle ah, CKFTSTraderSpiI* p)
		{
			pthread_mutex_lock(&_mutex);
			map_instence_APIHandle[ah] = p;
			pthread_mutex_unlock(&_mutex);
		}
		static void remove_instence_by_APIHandle(APIHandle ah)
		{
			pthread_mutex_lock(&_mutex);
			map_instence_APIHandle.erase(ah);
			pthread_mutex_unlock(&_mutex);
		}

		//fundid--->cellid
		static map<int, int> m_fundid_cellid;
		////cellid--->fundid
		static map<int, int> m_cellid_fundid;
		
			
		static uint32_t			tradingday;
	protected:
			//标识了一个交易连接
			APIHandle			apiHandle;	// API句柄 void*
			STRspMsg			err_msg;	// 错误信息 0
			CONNHandle			connHandle;	// 辅连接句柄 NULL
			STTraderLogin		login;		//登录
			map<int, string>	front;		//前置机器
			bool				InitFlag;	//是否初始化完毕
	public:
			static SyncRequest<int> syncreq;
};

#endif

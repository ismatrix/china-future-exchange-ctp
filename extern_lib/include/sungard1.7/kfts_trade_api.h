/////////////////////////////////////////////////////////////////////////
///@system 金仕达新一代资管系统
///@company SunGard China
///@file kfts_trade_api.h
///@brief 定义了金仕达资管系统交易接口
///@history
///20141001	Kingstar IRDG		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef __KFTS_TRADE_API_H__
#define __KFTS_TRADE_API_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32
#ifdef TRADE_API_EXPORTS
#define KFTS_TRADE_API __declspec(dllexport)
#else
#define KFTS_TRADE_API __declspec(dllimport)
#endif
#else
#ifdef TRADE_API_EXPORTS
#define KFTS_TRADE_API __attribute__ ((visibility ("default"))) 
#else
#define KFTS_TRADE_API
#endif
#endif 

#include "kfts_trade_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

		/** \defgroup Group21 2.1 句柄定义 */
	/** @{ */
	/// API句柄
	typedef void * APIHandle;

	/// 连接句柄
	typedef void * CONNHandle;
	/** @} */

	/** \defgroup Group22 2.2 回调函数定义 */
	/** @{ */
	/**
	* @brief 与前置机主连接是否成功通知
	* @param[out] hApiHandle API句柄
	* @param[out] pRspMsg 返回信息，其中的错误代码为0时表示与前置机建立主连接成功，非0时表示建立连接失败
	* @details 客户端调用\ref KFTSAPI_StartLogin 与前置建立连接，连接结果通过该回调通知客户端。
	* 客户端需要注册该回调，在收到该回调后进行登录操作
	* @see KFTSAPI_StartLogin
	*/
	typedef void (*PFN_RTN_CONNECTED)(APIHandle hApiHandle, const STRspMsg *pRspMsg);

	/**
	* @brief 与前置机主连接是否登录成功通知
	* @param[out] hApiHandle API句柄
	* @param[out] pRspUserLogin 登录应答消息
	* @param[out] pRspMsg 返回信息，其中的错误代码为0时表示登录成功，非0时表示登录失败
	* @details 客户端调用\ref KFTSAPI_Login 进行登录，登录结果通过该回调通知客户端。
	* @see KFTSAPI_Login
	*/
	typedef void (*PFN_RTN_LOGINED)(APIHandle hApiHandle, const STTraderLoginRsp *pRspUserLogin, const STRspMsg *pRspMsg);

	/**
	* @brief 与前置机断开重新通知
	* @param[out] hApiHandle API句柄
	* @param[out] hConnHandle 断开的连接句柄
	* @param[out] nFrontID 前置机ID
	* @param[out] pRspMsg 错误信息
	* @return -1表示不再重连，0表示由系统选择前置重新连接，返回值大于0时按照该返回值的前置ID重新连接
	* @details 当主(hConnHandle = NULL)辅连接与前置机连接断开时，API通过该回调通知客户端，
	* 客户端可通过该回调的返回值，确定是否重连以及如何重连。
	* 如果没有注册该回调，则当连接断开时，由系统选择前置重新连接。
	*/
	typedef int (*PFN_RTN_RECONNECT)(APIHandle hApiHandle, const CONNHandle hConnHandle, int nFrontID, const STRspMsg *pRspMsg);

	/**
	* @brief 消息订阅完成通知
	* @param[out] hApiHandle API句柄
	* @param[out] pRspMsg 返回信息，其中的错误代码为0时表示订阅成功，非0时表示订阅失败
	* @details 当客户端登录成功后，API会自动向前置机发送订阅请求，以更新API数据。
	* 当前置机将客户端登录前数据全部发送给API后，前置机会发送订阅完成通知给API。
	* API通过该回调函数，将订阅完成信息通知客户端。此时API中的数据已经更新到登录时的数据。
	*/
	typedef void (*PFN_RTN_SUBSCRIBE)(APIHandle hApiHandle, const STRspMsg *pRspMsg);

	/**
	* @brief 报价通知回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pQuote 报价单信息
	* @details 当报价发生变动时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RTN_QUOTE)(APIHandle hApiHandle, const STQuote *pQuote);

	/**
	* @brief 委托回报推送回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pOrder 委托信息
	* @param[out] nDataSrc 类型:1-柜台委托推送,2-报盘委托推送,3-柜台撤单推送,4-报盘撤单推送,5-成交推送
	* @details 当委托发生变动时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RTN_ORDER)(APIHandle hApiHandle, const STOrderInfo *pOrder, int nDataSrc);  

	/**
	* @brief 组合委托回报推送回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pOrderCombine 委托信息
	* @param[out] nDataSrc 类型:1-柜台委托推送,2-报盘委托推送,3-柜台撤单推送,4-报盘撤单推送,5-成交推送
	* @details 当委托发生变动时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RTN_ORDER_COMB)(APIHandle hApiHandle, const STOrderCombInfo *pOrderCombine, int nDataSrc);

	/**
	* @brief 成交回报推送回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pDone 成交信息
	* @details 当发生成交时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RTN_DONE)(APIHandle hApiHandle, const STDoneInfo *pDone);

	/**
	* @brief 资金变动推送回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pAccount 账户资金信息
	* @details 当资金发生变动时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RTN_ACCOUNT)(APIHandle hApiHandle, const STAccountInfo *pAccount);

	/**
	* @brief 持仓变动推送回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pPosition 持仓信息
	* @details 当持仓发生变动时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RTN_POSITION)(APIHandle hApiHandle, const STPositionInfo *pPosition);

	/**
	* @brief 组合持仓变动推送回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pPositionCombine 持仓信息
	* @details 当组合持仓发生变动时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RTN_POSITION_COMB)(APIHandle hApiHandle, const STFutPositionCombInfo *pPositionCombine);

	/**
	* @brief 组合持仓明细变动推送回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pPositionCombineDetail 持仓信息
	* @details 当组合持仓明细发生变动时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RTN_POSITION_COMB_DET)(APIHandle hApiHandle, const STFutPositionCombDetail *pPositionCombineDetail);

	/**
	* @brief 转账请求核心确认回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pTransfer 转账返回信息
	* @param[out] nProcessStatus, 处理状态类型，1API转帐应答 2API转帐成功与否确认 3管理转帐应答 4管理转帐成功与否确认。
	* @details 当转账时，API通过该回调函数通知客户端，转帐的处理结果。
	*/
	typedef void (*PFN_RTN_FUNDTRANS)(APIHandle hApiHandle, const STFundTransANS *pTransfer,int nProcessStatus);

	/**
	* @brief 标的券锁定解锁变动推送回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] p_underlyingstock_action 标的券锁定解锁信息
	* @details 当标的券锁定解锁发生变动时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RTN_UNDERLYINGSTOCKACTION)(APIHandle hApiHandle, const STUnderlyingStockAction *pUnderlyingStockAction);

	/**
	* @brief 通用请求返回回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] nTID 返回消息类型
	* @param[out] pCommonInfo 返回消息内容
	* @param[out] nMsgLen 返回消息长度
	* @details 当收到通用请求处理完成的返回推送时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RTN_COMMONREQUEST)(APIHandle hApiHandle, int nTID, const char* pCommonInfo, int nMsgLen);

	/**
	* @brief SPX交易员检验是否成功通知
	* @param[out] hApiHandle API句柄
	* @param[out] pUserCheck 检验应答消息
	* @param[out] pRspMsg 返回信息，其中的错误代码为0时表示检验成功，非0时表示检验失败
	* @details 客户端调用\ref KFTSAPI_Check 进行检验，检验结果通过该回调通知客户端。
	* @see KFTSAPI_Check
	*/
	typedef void (*PFN_RTN_CHECKED)(APIHandle hApiHandle, const STTraderCheck *pUserCheck, const STRspMsg *pRspMsg);

	/**
	* @brief 错误应答信息回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pRspMsg 错误消息
	* @param[out] nRequestID 请求编号
	* @details 当查询出错或发生其他错误时，API通过该回调函数通知客户端。
	*/
	typedef void (*PFN_RSP_ERRORMSG)(APIHandle hApiHandle, const STRspMsg *pRspMsg, int nRequestID);

	/**
	* @brief 查询客户账户资金应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pAccount 账户资金信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryAccount 查询客户账户资金信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_account_rsp为NULL。
	* @see KFTSAPI_RegQueryAccountCallBack
	* @see KFTSAPI_QueryAccount
	*/
	typedef void (*PFN_RSP_QUERY_ACCOUNT)(APIHandle hApiHandle, const STAccountInfo *pAccount, int nRequestID, int nRecordID);

	/**
	* @brief 查询持仓应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pPosition 持仓信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryPosition 查询持仓信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_position_rsp为NULL。
	* @see KFTSAPI_RegQueryAccountCallBack
	* @see KFTSAPI_QueryPosition
	*/
	typedef void (*PFN_RSP_QUERY_POSITION)(APIHandle hApiHandle, const STPositionInfo *pPosition, int nRequestID, int nRecordID);

		/**
	* @brief 查询报价单回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pQuote 报价单查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_ReqQryQuote 查询报价信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的pQuote为NULL。
	* @see KFTSAPI_RegQuoteCallBack
	* @see KFTSAPI_ReqQryQuote
	*/
	typedef void (*PFN_RSP_QUERY_QUOTE)(APIHandle hApiHandle, const STQuote* pQuote, int nRequestID, int nRecordID);

	/**
	* @brief 查询委托单应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pOrder 委托信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryOrder 查询委托信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_orderinfo_rsp为NULL。
	* @see KFTSAPI_RegQueryOrderCallBack
	* @see KFTSAPI_QueryOrder
	*/
	typedef void (*PFN_RSP_QUERY_ORDER)(APIHandle hApiHandle, const STOrderInfo *pOrder, int nRequestID, int nRecordID);

	/**
	* @brief 查询期货组合单委托应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pOrderCombine 委托信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryOrder 查询委托信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_orderinfo_rsp为NULL。
	* @see KFTSAPI_RegQueryOrderCombCallBack
	* @see KFTSAPI_QueryOrderComb
	*/
	typedef void (*PFN_RSP_QUERY_ORDER_COMB)(APIHandle hApiHandle, const STOrderCombInfo *pOrderCombine, int nRequestID, int nRecordID);

	/**
	* @brief 查询成交应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pDone 成交信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryDone 查询成交信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_doneinfo_rsp为NULL。
	* @see KFTSAPI_RegQueryDoneCallBack
	* @see KFTSAPI_QueryDone
	*/
	typedef void (*PFN_RSP_QUERY_DONE)(APIHandle hApiHandle, const STDoneInfo *pDone, int nRequestID, int nRecordID);

	/**
	* @brief 查询证券信息应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pSecCode 证券信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QuerySecCode 查询证券信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_seccode_rsp为NULL。
	* @see KFTSAPI_RegQuerySecCodeCallBack
	* @see KFTSAPI_QuerySecCode
	*/
	typedef void (*PFN_RSP_QUERY_SECCODE)(APIHandle hApiHandle, const STSecCodeInfo *pSecCode, int nRequestID, int nRecordID);

	/**
	* @brief 查询ETF基本信息应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pETFBaseInfo ETF基本信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryEtfBaseInfo 查询ETF基本信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_etfbaseinfo_rsp为NULL。
	* @see KFTSAPI_RegQueryEtfBaseInfoCallBack
	* @see KFTSAPI_QueryEtfBaseInfo
	*/
	typedef void (*PFN_RSP_QUERY_ETFBASEINFO)(APIHandle hApiHandle, const STEtfBaseInfo *pETFBaseInfo, int nRequestID, int nRecordID);

	/**
	* @brief 查询ETF篮子信息应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pEFTBasketInfo ETF篮子信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryEtfBasketInfo 查询ETF篮子信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_etfbasketinfo_rsp为NULL。
	* @see KFTSAPI_RegQueryEtfBasketInfoCallBack
	* @see KFTSAPI_QueryEtfBasketInfo
	*/
	typedef void (*PFN_RSP_QUERY_ETFBASKETINFO)(APIHandle hApiHandle, const STEtfBasketInfo *pEFTBasketInfo, int nRequestID, int nRecordID);

	/**
	* @brief 查询费率信息应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pFeeInfo ETF篮子信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryFeeInfo 查询ETF篮子信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_feeinfo_rsp为NULL。
	* @see KFTSAPI_QueryFeeInfo
	*/
	typedef void (*PFN_RSP_QUERY_FEEINFO)(APIHandle hApiHandle, const STFeeInfo *pFeeInfo, int nRequestID, int nRecordID);

	/**
	* @brief 下单和撤单的错误信息回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] p_order_info 报单信息
	* @details 当下单或撤单出错，API通过该回调函数通知客户端。
	* @see KFTSAPI_RegErrMsgInvalidCallBack
	*/
	typedef void (*PFN_RTN_ERRORORDER)(APIHandle hApiHandle, const STOrderInfo *pOrder, int nDataSrc);

	/**
	* @brief 询价录入应答
	* @param[out] hApiHandle API句柄
	* @param[out] pRspForQuoteInsert 询价应答消息
	* @param[out] pRspMsg 返回信息，其中的错误代码为0时表示成功，非0时表示失败
	* @details 客户端调用\ref KFTSAPI_ForQuoteInsert 进行询价，询价结果通过该回调通知客户端。
	* @see KFTSAPI_ForQuoteInsert
	*/
	typedef void (*PFN_RSP_FORQUOTEINSERT)(APIHandle hApiHandle, const STRspForQuoteInsert *pRspForQuoteInsert, const STRspMsg *pRspMsg);

	/**
	* @brief 查询股东代码和市场代码回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pHolder 股东代码和市场代码信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_etfbaseinfo_rsp为NULL。
	* @see KFTSAPI_RegQueryHoldernoCallBack
	* @see KFTSAPI_QueryHolderno 
	*/
	typedef void (*PFN_RSP_QUERY_HOLDERNO)(APIHandle hApiHandle, const STHoldernoInfo *pHolder, int nRequestID, int nRecordID);

	/**
	* @brief 查询期货组合合约持仓明细信息应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pPositionCombineDetail 期货组合合约持仓明细信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryPositionCombDet 查询期货组合合约持仓明细信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_pos_detail_rsp为NULL。
	* @see KFTSAPI_QueryPositionCombDet
	*/
	typedef void (*PFN_RSP_QUERY_POSITION_COMB_DET)(APIHandle hApiHandle, const STFutPositionCombDetail *pPositionCombineDetail, int nRequestID, int nRecordID);

		/**
	* @brief 查询期货组合合约持仓信息应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pPositionCombine 期货组合合约持仓信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryPositionComb 查询期货组合合约持仓信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_poscmb_rsp为NULL。
	* @see KFTSAPI_QueryPositionComb
	*/
	typedef void (*PFN_RSP_QUERY_POSITION_COMB)(APIHandle hApiHandle, const STFutPositionCombInfo *pPositionCombine, int nRequestID, int nRecordID);

	/**
	* @brief 查询期货普通合约信息应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pInstrument 期货普通合约信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryFutContInfo 查询期货普通合约信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_coninfo_rsp为NULL。
	* @see KFTSAPI_QueryFutContInfo
	*/
	typedef void (*PFN_RSP_QUERY_FUT_CONT)(APIHandle hApiHandle, const STFutContractInfo *pInstrument, int nRequestID, int nRecordID);

	/**
	* @brief 查询期货组合合约信息应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pCombineInstrument 期货组合合约信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryFutContComb 查询期货组合合约信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_coninfo_rsp为NULL。
	* @see KFTSAPI_QueryFutContCombInfo
	*/
	typedef void (*PFN_RSP_QUERY_FUT_CONT_COMB)(APIHandle hApiHandle, const STFutContractCombInfo *pCombineInstrument, int nRequestID, int nRecordID);

	/**
	* @brief 查询期货合约费率应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pFeeRate 期货合约费率信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryFutFeeRate 查询期货合约费率信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的p_fee_rate_rsp为NULL。
	* @see KFTSAPI_QueryFutFeeRate
	*/
	typedef void (*PFN_RSP_QUERY_FUT_FEE_RATE)(APIHandle hApiHandle, const STFuturefee *pFeeRate, int nRequestID, int nRecordID);

	/**
	* @brief 查询期货组合合约策略代码应答回调函数
	* @param[out] hApiHandle API句柄
	* @param[out] pFutCombStrategyInfo 期货组合合约策略代码信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 客户端调用\ref KFTSAPI_QueryFutCombStrategy 查询期货组合合约策略代码信息，
	* 系统处理后异步通过该回调函数将结果返回给客户端。
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录，结束记录的pFutCombStrategyInfo为NULL。
	* @see KFTSAPI_QueryFutCombStrategy
	*/
	typedef void (*PFN_RSP_QUERY_COMB_STRATEGY)(APIHandle hApiHandle, const STFutCombStrategyInfo *pFutCombStrategyInfo, int nRequestID, int nRecordID);

	/**
	* @brief 查询数据字典
	* @param[out] hApiHandle API句柄
	* @param[out] pDict 数据字典信息查询结果
	* @param[out] nRequestID 请求编号
	* @param[out] nRecordID 记录号
	* @details 
	* nRecordID为\ref EMPTY_ROW 时表示查询结束记录。
	*/
	typedef void (*PFN_RSP_QUERY_DICTIONARYINFO)(APIHandle hApiHandle, const STDictionaryInfo *pDict, int nRequestID, int nRecordID);

	/** @} */

	/** \defgroup Group31 3.1 接口函数-全局功能 */
	/** @{ */
	/**
	* @brief 全局初始化
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details API全局初始化函数，需要且仅需调用一次
	* @see KFTSAPI_Finalize
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Initialize(STRspMsg * pRspMsg);

	/**
	* @brief 获得API自身的信息
	* @param[in] hApiHandle API句柄
	* @param[out] pApiInfo API信息输出
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 查询API自身的信息
	*/
	KFTS_TRADE_API BOOL KFTSAPI_GetApiInfo(STApiInfo* pApiInfo, STRspMsg * pRspMsg);

	/**
	* @brief 全局退出
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details API全局退出函数，需要且仅需调用一次
	* @see KFTSAPI_Initialize
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Finalize(STRspMsg * pRspMsg);
	/** @} */

	/** \defgroup Group32 3.2 接口函数-交易员Api功能 */
	/** @{ */
	/**
	* @brief 创建交易员API句柄
	* @param[out] pRspMsg 失败时的错误信息
	* @return API句柄,创建失败时为NULL,创建成功时非NULL
	* @details 创建交易员API句柄，可多次调用，每个APIHandle对应一个交易员
	* @see KFTSAPI_Destroy
	*/
	KFTS_TRADE_API APIHandle KFTSAPI_Create(STRspMsg * pRspMsg);

	/**
	* @brief 销毁交易员API句柄
	* @param[in] hApiHandle 要销毁的API句柄,不能为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 销毁交易员API句柄，释放资源
	* @see KFTSAPI_Create
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Destroy(APIHandle hApiHandle, STRspMsg * pRspMsg);

	/**
	* @brief 设置API参数
	* @param[in] hApiHandle 要设置的API句柄,如果为NULL则为全局设置
	* @param[in] nOpt 要设置的参数类型，参考\ref KFTS_OPTION
	* @param[in] value 要设置的参数值
	* @param[in] nSize 参数值类型长度
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 设置某个API参数或全局参数，参数选项参见\ref KFTS_OPTION
	* @see KFTS_OPTION
	*/
	KFTS_TRADE_API BOOL KFTSAPI_SetOpt(APIHandle hApiHandle, int nOpt, const void *pValue, int nSize, STRspMsg * pRspMsg);

	/**
	* @brief 设置API上下文信息
	* @param[in] hApiHandle 要设置的API句柄,不能为NULL
	* @param[in] p_context 要设置的上下文信息
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 设置API上下文信息
	* @see KFTSAPI_GetApiContext
	*/
	KFTS_TRADE_API BOOL KFTSAPI_SetApiContext(APIHandle hApiHandle, const void * pContext, STRspMsg * pRspMsg);

	/**
	* @brief 获取API上下文信息
	* @param[in] hApiHandle 要获取上下文信息的API句柄，不能为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return API上下文信息
	* @details 获取API上下文信息
	* @see KFTSAPI_SetApiContext
	*/
	KFTS_TRADE_API void * KFTSAPI_GetApiContext(APIHandle hApiHandle, STRspMsg * pRspMsg);

	/**
	* @brief 获取API当前状态
	* @param[in] hApiHandle 要获取当前回调号的API句柄，不能为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return API当前状态，-1，失败
	* @details 获取API当前状态
	* @see KFTSAPI_GetApiStatus
	*/
	KFTS_TRADE_API int KFTSAPI_GetApiStatus(APIHandle hApiHandle,STRspMsg * pRspMsg);

	/**
	* @brief 获取API当前回调号
	* @param[in] hApiHandle 要获取当前回调号的API句柄，不能为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return API当前回调号，-1,失败
	* @details 获取API当前回调号
	* @see KFTSAPI_GetCallbackNO
	*/
	KFTS_TRADE_API int KFTSAPI_GetCallbackNO(APIHandle hApiHandle,STRspMsg * pRspMsg);

	/**
	* @brief 注册一般错误信息回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的错误信息回调函数
	* @param[out] pOldFunc 已经注册的错误信息回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册一般错误信息回调函数，当发生一般错误时，API会调用注册的错误信息回调函数来通知客户端。
	* 若不关心这些错误信息，可不注册该回调函数。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegErrMsgCallBack(APIHandle hApiHandle, PFN_RSP_ERRORMSG fNewFunc, PFN_RSP_ERRORMSG *pOldFunc, STRspMsg * pRspMsg);
	/** @} */

	/** \defgroup Group33 3.3 接口函数-登录功能 */
	/** @{ */
	/**
	* @brief 注册连接通知回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的连接通知回调函数
	* @param[out] pOldFunc 已经注册的连接通知回调函数，可为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册连接通知回调函数，该函数必须注册。
	* 客户端调用\ref KFTSAPI_StartLogin 之后，API首先连接前置，当与前置建立连接成功时，
	* API会调用当前注册的连接通知回调函数通知客户端，客户端在得到连接成功通知后，
	* 才可以调用\ref KFTSAPI_Login 进行登录。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegConnectedRtnCallBack(APIHandle hApiHandle, PFN_RTN_CONNECTED fNewFunc, PFN_RTN_CONNECTED * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册登陆通知回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的登陆通知回调函数
	* @param[out] pOldFunc 已经注册的登陆通知回调函数，可为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册登陆通知回调函数。客户端调用\ref KFTSAPI_Login 进行登录后，
	* 当登录成功或失败时，API通过登录通知回调函数将结果告知客户端。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegLoginedRtnCallBack(APIHandle hApiHandle, PFN_RTN_LOGINED fNewFunc, PFN_RTN_LOGINED * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册消息订阅完成通知回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的消息订阅完成通知回调函数
	* @param[out] pOldFunc 已经注册的消息订阅完成通知回调函数，可为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册消息订阅完成通知回调函数。当客户登录成功时，API会自动向前置发送订阅请求，
	* 以更新自身信息，当数据全部更新后，API会通过消息订阅完成回调通知客户端。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegSubscribeRtnCallBack(APIHandle hApiHandle, PFN_RTN_SUBSCRIBE fNewFunc, PFN_RTN_SUBSCRIBE * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册前置机
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] nFrontID 前置机编号，必须大于0
	* @param[in] pszFrontAddress 连接字符串，格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。
	* 其中，“tcp”代表传输协议，“127.0.0.1”代表服务器地址，”17001”代表服务器端口号。
	* 目前支持的传输协议为tcp。
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册前置机
	* @see KFTSAPI_RemoveFront
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegFront(APIHandle hApiHandle, int nFrontID, const char * pszFrontAddress, STRspMsg * pRspMsg);

	/**
	* @brief 移除前置机
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] nFrontID 前置机编号，必须大于0
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 移除前置机
	* @see KFTSAPI_RegFront
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RemoveFront(APIHandle hApiHandle, int nFrontID, STRspMsg * pRspMsg);

	/**
	* @brief 连接前置机，开始登陆
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] nFrontID 开始连接的前置机, 如果连接不成功, 则去连接其它前置机
	* @param[in] nConnTimeout 连接超时时间，以毫秒为单位
	* @param[out] pRspMsg 失败时的错误信息
	* @param[in] nSyncMode 同步-异步模式，0:异步模式, 1:同步模式
	* @return TRUE为成功，FALSE为失败
	* @details 连接前置机，开始登陆。
	* 同步模式直接返回连接结果。
	* 异步模式异步返回连接结果，通过\ref PFN_RTN_CONNECTED 将连接结果通知客户端。
	* @see PFN_RTN_CONNECTED
	* @see KFTSAPI_RegConnectedRtnCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_StartLogin(APIHandle hApiHandle, int nFrontID, int nConnTimeout, STRspMsg * pRspMsg, int nSyncMode = 0);

	/**
	* @brief 交易员登录
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pUserLogin 登录信息
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 交易员登录。在和前置建立连接成功通知客户端后，客户端调用该函数进行登录。
	* 异步返回结果，通过\ref PFN_RTN_LOGINED 将结果通知客户端。
	* @see PFN_RTN_LOGINED
	* @see KFTSAPI_RegLoginedRtnCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Login(APIHandle hApiHandle, const STTraderLogin* pRspUserLogin, STRspMsg * pRspMsg);

	/**
	* @brief 交易员登出
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] nTimeout 超时时间，以毫秒为单位
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 交易员登出。
	* 同步返回结果。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Logout(APIHandle hApiHandle, int nTimeout, STRspMsg * pRspMsg);
	/** @} */

	/** \defgroup Group34 3.4 接口函数-连接功能 */
	/** @{ */
	/**
	* @brief 注册断开重连通知回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的断开重连通知回调函数
	* @param[in] pOldFunc 已经注册的断开重连通知回调函数，可为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 客户端可注册断开重连回调函数\ref PFN_RTN_RECONNECT 来处理与前置断开时客户端是否自动重连以及重连时使用哪个前置。
	* 若客户端未注册该回调函数，系统将自动选择前置机进行重新连接。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegReconnectRtnCallBack(APIHandle hApiHandle, PFN_RTN_RECONNECT fNewFunc, PFN_RTN_RECONNECT * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 与前置建立新的连接
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] nFrontID 前置机ID。如果为0, 则系统选择前置机；大于0时，为指定前置机。
	* @param[in] nConnTimeout 超时时间，以毫秒为单位
	* @param[out] pRspMsg 失败时的错误信息
	* @return 连接句柄，失败返回NULL，成功返回非NULL
	* @details 客户端可根据需要，与前置建立多个连接。
	* 必须在login成功之后调用。
	* @see KFTSAPI_Close
	*/
	KFTS_TRADE_API CONNHandle KFTSAPI_Connect(APIHandle hApiHandle, int nFrontID, int nConnTimeout, STRspMsg * pRspMsg);

	/**
	* @brief 关闭与前置机的连接
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 要关闭的连接句柄。可为NULL，为NULL表示关闭所有连接。
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 关闭指定连接
	* @see KFTSAPI_Connect
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Close(APIHandle hApiHandle, CONNHandle hConnHandle, STRspMsg * pRspMsg);

	/**
	* @brief 设置连接上下文信息
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，不能为NULL
	* @param[in] pContext 要设置的上下文信息
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 设置连接上下文信息
	* @see KFTSAPI_GetConnectionContext
	*/
	KFTS_TRADE_API BOOL KFTSAPI_SetConnectionContext(APIHandle hApiHandle, CONNHandle hConnHandle, const void * pContext, STRspMsg * pRspMsg);

	/**
	* @brief 获取连接上下文信息
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，不能为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return 已设置的连接上下文信息
	* @details 获取连接上下文信息
	* @see KFTSAPI_SetConnectionContext
	*/
	KFTS_TRADE_API void * KFTSAPI_GetConnectionContext(APIHandle hApiHandle, CONNHandle hConnHandle, STRspMsg * pRspMsg);
	/** @} */

	/** \defgroup Group35 3.5 接口函数-委托及请求功能 */
	/** @{ */
	/**
	* @brief 注册报价通知回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的报价通知回调函数
	* @param[out] pOldFunc 已经注册的报价通知回调函数，可为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册报价通知回调函数
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQuoteRtnCallBack(APIHandle hApiHandle, PFN_RTN_QUOTE fNewFunc, PFN_RTN_QUOTE * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册委托回报推送处理回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的委托回报推送处理回调函数
	* @param[out] pOldFunc 已经注册的委托回报推送处理回调函数，可为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册委托回报推送处理回调函数
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegOrderRtnCallBack(APIHandle hApiHandle, PFN_RTN_ORDER fNewFunc, PFN_RTN_ORDER * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册成交推送处理回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的成交推送处理回调函数
	* @param[out] pOldFunc 已经注册的成交推送处理回调函数，可为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册成交推送处理回调函数
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegDoneRtnCallBack(APIHandle hApiHandle, PFN_RTN_DONE fNewFunc, PFN_RTN_DONE * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册资金变动推送处理回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的资金变动推送处理回调函数
	* @param[out] pOldFunc 已经注册的资金变动推送处理回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册资金变动推送处理回调函数。当有资金变动时，API通过资金变动推送处理回调函数通知客户端。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegAccountRtnCallBack(APIHandle hApiHandle, PFN_RTN_ACCOUNT fNewFunc, PFN_RTN_ACCOUNT * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册持仓变动推送处理回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的持仓变动推送处理回调函数
	* @param[out] pOldFunc 已经注册的持仓变动推送处理回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册持仓变动推送处理回调函数。当有持仓变动时，API通过持仓变动推送处理回调函数通知客户端。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegPositionRtnCallBack(APIHandle hApiHandle, PFN_RTN_POSITION fNewFunc, PFN_RTN_POSITION * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册标的券锁定解锁推送处理回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的标的券锁定解锁推送处理回调函数
	* @param[out] pOldFunc 已经注册的标的券锁定解锁推送处理回调函数，可为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册标的券锁定解锁推送处理回调函数
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegUnderlyingStockActionRtnCallBack(APIHandle hApiHandle, PFN_RTN_UNDERLYINGSTOCKACTION fNewFunc, PFN_RTN_UNDERLYINGSTOCKACTION * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册通用请求推送处理回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的通用请求推送处理回调函数
	* @param[out] pOldFunc 已经注册的通用请求推送处理回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册通用请求推送处理回调函数。当收到通用请求处理完成的返回推送时，API通过请求推送处理回调函数通知客户端。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegCommonRequestRtnCallBack(APIHandle hApiHandle, PFN_RTN_COMMONREQUEST fNewFunc, PFN_RTN_COMMONREQUEST * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册转账请求返回回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的转账请求返回回调函数
	* @param[out] pOldFunc 已经注册的转账请求返回回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_QueryFeeInfo
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegFundTransCallBack(APIHandle hApiHandle, PFN_RTN_FUNDTRANS fNewFunc, PFN_RTN_FUNDTRANS *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 报价录入请求
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，为NULL时使用主连接
	* @param[in] pInputQuote 报价结构体
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 报价录入指令。
	* @see KFTSAPI_ReqQuoteInsert
	*/
	KFTS_TRADE_API BOOL KFTSAPI_ReqQuoteInsert(APIHandle hApiHandle, CONNHandle hConnHandle, const STInputQuote *pInputQuote, STRspMsg * pRspMsg);
	
	/**
	* @brief 报单操作请求
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，为NULL时使用主连接
	* @param[in] pInputOrderAction 报单操作结构体
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 报单操作指令。
	* @see KFTSAPI_ReqOrderAction
	*/
	KFTS_TRADE_API BOOL KFTSAPI_ReqOrderAction(APIHandle hApiHandle, CONNHandle hConnHandle, const STInputOrderAction *pInputOrderAction, STRspMsg * pRspMsg);

	/**
	* @brief 撤单 - 旧接口
	* @param[in] handle API句柄，不能为NULL
	* @param[in] conn_handle 连接句柄，为NULL时使用主连接
	* @param[in] p_order_cancel 撤单结构数组
	* @param[in] cancel_count 本次撤单数量，不能超出撤单最大数量(\ref MAX_ORDER_CANCEL_COUNT)
	* @param[out] err_msg 失败时的错误信息
	* @param[in] timeout 该笔撤单的超时时间，以毫秒为单位，INFINITE为不限超时，该参数默认值INFINITE - 1为使用默认超时选项(\ref KFTS_OPT_DEFAULT_TIMEOUT)设置的超时时间
	* @return TRUE为成功，FALSE为失败
	* @details 撤单指令将由新一代撤单接口替代。
	* @see KFTSAPI_OrderCancelByOrderNo
	*/
	KFTS_TRADE_API BOOL KFTSAPI_OrderCancelByOrderNo(APIHandle handle, CONNHandle conn_handle, const STOrderCancelByOrderNo *p_order_cancel, int cancel_count, STRspMsg * err_msg, int timeout = INFINITE -1);


	/**
	* @brief 下单指令
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，为NULL时使用主连接
	* @param[in] pOrder 委托结构数组
	* @param[in] nOrderCount 本次委托数量，不能超出委托最大数量(\ref MAX_ORDER_COUNT)
	* @param[out] pRspMsg 失败时的错误信息
	* @param[in] nTimeout 该笔委托的超时时间，以毫秒为单位，INFINITE为不限超时，该参数默认值INFINITE - 1为使用默认超时选项(\ref KFTS_OPT_DEFAULT_TIMEOUT)设置的超时时间
	* @return TRUE为成功，FALSE为失败
	* @details 下单指令。一次下单数量范围为1-\ref MAX_ORDER_COUNT 。
	* @see KFTSAPI_Order
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Order(APIHandle hApiHandle, CONNHandle hConnHandle, const STOrder *pOrder, int nOrderCount, STRspMsg * pRspMsg, int nTimeout = INFINITE - 1);

	/**
	* @brief 增强下单指令
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，为NULL时使用主连接
	* @param[in] pOrder 委托结构数组
	* @param[in] nOrderCount 本次委托数量，不能超出委托最大数量(\ref MAX_ORDER_COUNT)
	* @param[in] nBatchNo 批号
	* @param[in] nOrderProp 批量委托属性 0-允许部分下单成功、部分失败，1-全部下单成功否则全部取消
	* @param[out] pRspMsg 失败时的错误信息
	* @param[in] nTimeout 该笔委托的超时时间，以毫秒为单位，INFINITE为不限超时，该参数默认值INFINITE - 1为使用默认超时选项(\ref KFTS_OPT_DEFAULT_TIMEOUT)设置的超时时间
	* @return TRUE为成功，FALSE为失败
	* @details 增强下单指令。一次下单数量范围为1-\ref MAX_ORDER_COUNT 。
	* 增强下单指令可使用批号将本次下单指令设置为一批，以使撤单时按批号撤单，将本次所下委托全部撤单。
	* 同时，增强下单指令可设置本次下单是否必须全部成功否则取消。
	* @see KFTSAPI_OrderEx
	*/
	KFTS_TRADE_API BOOL KFTSAPI_OrderEx(APIHandle hApiHandle, CONNHandle hConnHandle, const STOrder *pOrder, int nOrderCount, int nBatchNo, int nOrderProp, STRspMsg * pRspMsg, int nTimeout = INFINITE -1);

	/**
	* @brief 通用请求
	* @param[in] hApiHandle hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，为NULL时使用主连接
	* @param[in] nTID 请求类型
	* @param[in] pCommonInfo 请求数据
	* @param[in] nCommonInfoLen 请求数据长度
	* @param[out] pRspMsg 失败时的错误信息
	* @param[in] nTimeout 该笔请求的超时时间，以毫秒为单位，INFINITE为不限超时，该参数默认值INFINITE - 1为使用默认超时选项(\ref KFTS_OPT_DEFAULT_TIMEOUT)设置的超时时间
	* @return TRUE为成功，FALSE为失败
	* @details 通用请求功能。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_CommonRequest(APIHandle hApiHandle, CONNHandle hConnHandle, int nTID, const char* pCommonInfo, int nCommonInfoLen, STRspMsg * pRspMsg, int nTimeout = INFINITE -1);

	/**
	* @brief 转账请求
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，为NULL时使用主连接
	* @param[in] pTransfer 转账请求结构数组
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 转账请求。
	*/
	KFTS_TRADE_API BOOL KFTSAPI_TransFund(APIHandle hApiHandle, CONNHandle hConnHandle, STFundTransREQ *pTransfer, STRspMsg * pRspMsg);

	/**
	* @brief 期货组合单下单指令
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，为NULL时使用主连接
	* @param[in] pOrderCombine 委托结构数组
	* @param[in] nOrderCombineCount 本次委托数量，不能超出委托最大数量(\ref MAX_ORDER_COUNT)
	* @param[out] pRspMsg 失败时的错误信息
	* @param[in] nBatchNo 本次委托批次号
	* @param[in] nTimeout 该笔委托的超时时间，以毫秒为单位，INFINITE为不限超时，该参数默认值INFINITE - 1为使用默认超时选项(\ref KFTS_OPT_DEFAULT_TIMEOUT)设置的超时时间
	* @return TRUE为成功，FALSE为失败
	* @details 下单指令。一次下单数量范围为1-\ref MAX_ORDER_COUNT 。
	* @see KFTSAPI_OrderComb
	*/
	KFTS_TRADE_API BOOL KFTSAPI_OrderComb(APIHandle hApiHandle, CONNHandle hConnHandle, const STOrderComb *pOrderCombine, int nOrderCombineCount, STRspMsg * pRspMsg, int nBatchNo, int nTimeout = INFINITE - 1);

	/**
	* @brief 注册组合单持仓变动推送处理回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的组合单持仓变动推送处理回调函数
	* @param[out] pOldFunc 已经注册的组合单持仓变动推送处理回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册组合单持仓变动推送处理回调函数。当有组合单持仓变动时，API通过组合单持仓变动推送处理回调函数通知客户端。
	* @see PFN_RTN_POSITION_COMB
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegPositionCombRtnCallBack(APIHandle hApiHandle, PFN_RTN_POSITION_COMB fNewFunc, PFN_RTN_POSITION_COMB * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册组合单持仓明细变动推送处理回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的组合单持仓明细变动推送处理回调函数
	* @param[out] pOldFunc 已经注册的组合单持仓明细变动推送处理回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册组合单持仓明细变动推送处理回调函数。当有组合单持仓明细变动时，API通过组合单持仓明细变动推送处理回调函数通知客户端。
	* @see PFN_RTN_POSITION_COMB_DET
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegPositionCombDetRtnCallBack(APIHandle hApiHandle, PFN_RTN_POSITION_COMB_DET fNewFunc, PFN_RTN_POSITION_COMB_DET * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册下单和撤单的报错回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的下单和撤单的报错回调函数
	* @param[out] pOldFunc 已经注册的下单和撤单的报错回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RTN_ERRORORDER
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegErrOrderRtnCallBack(APIHandle hApiHandle, PFN_RTN_ERRORORDER fNewFunc, PFN_RTN_ERRORORDER *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册SPX交易员校验应答回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的校验应答回调函数
	* @param[out] pOldFunc 已经注册的校验应答回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RTN_CHECKED
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegCheckedRtnCallBack(APIHandle hApiHandle, PFN_RTN_CHECKED fNewFunc, PFN_RTN_CHECKED *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief SPX普通交易员校验
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pReqUserLogin 校验信息
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details SPX网关接入资管系统普通交易员校验。在和前置建立连接成功通知客户端后，客户端调用该函数进行校验。
	* 异步返回结果，通过\ref PFN_RTN_LOGINED 将结果通知客户端。
	* @see PFN_RTN_LOGINED
	* @see KFTSAPI_RegLoginedRtnCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Check(APIHandle hApiHandle, const STTraderCheck* pReqUserLogin, STRspMsg * pRspMsg);

	/**
	* @brief 注册询价录入应答回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的询价录入应答回调函数
	* @param[out] pOldFunc 已经注册的询价录入应答回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_FORQUOTEINSERT
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegForQuoteInsertCallBack(APIHandle hApiHandle, PFN_RSP_FORQUOTEINSERT fNewFunc, PFN_RSP_FORQUOTEINSERT *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 询价录入请求
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，为NULL时使用主连接
	* @param[in] pForQuoteInsert 询价数组
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 询价录入请求。
	*/
	KFTS_TRADE_API  BOOL KFTSAPI_ForQuoteInsert(APIHandle hApiHandle, CONNHandle hConnHandle, const STForQuoteInsert *pForQuoteInsert, STRspMsg * pRspMsg);
	
	/**
	* @brief 标的券锁定\解锁请求
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] hConnHandle 连接句柄，为NULL时使用主连接
	* @param[in] pReqUnderlyingStockAction 标的券锁定解锁请求
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 标的券锁定\解锁请求录入请求。
	*/
	KFTS_TRADE_API  BOOL KFTSAPI_ReqUnderlyingStockAction(APIHandle hApiHandle, CONNHandle hConnHandle, const STReqUnderlyingStockAction *pReqUnderlyingStockAction, STRspMsg * pRspMsg);
	
	/** @} */

	/** \defgroup Group36 3.6 接口函数-查询功能 */
	/** @{ */
	/**
	* @brief 注册账户资金查询返回回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的账户资金查询返回回调函数
	* @param[out] pOldFunc 已经注册的账户资金查询返回回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_QueryAccount
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryAccountCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_ACCOUNT fNewFunc, PFN_RSP_QUERY_ACCOUNT *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册持仓查询返回回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的持仓查询返回回调函数
	* @param[out] pOldFunc 已经注册的持仓查询返回回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_QueryPosition
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryPositionCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_POSITION fNewFunc, PFN_RSP_QUERY_POSITION *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册报价单查询回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的报价单查询回调函数
	* @param[out] pOldFunc 已经注册的报价单查询回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_ReqQryQuote
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQryQuoteCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_QUOTE fNewFunc, PFN_RSP_QUERY_QUOTE *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册委托查询返回回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的委托查询返回回调函数
	* @param[out] pOldFunc 已经注册的委托查询返回回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_QueryOrder
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryOrderCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_ORDER fNewFunc, PFN_RSP_QUERY_ORDER *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册成交查询返回回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的成交查询返回回调函数
	* @param[out] pOldFunc 已经注册的成交查询返回回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_QueryDone
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryDoneCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_DONE fNewFunc, PFN_RSP_QUERY_DONE *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册证券信息查询返回回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的证券信息查询返回回调函数
	* @param[out] pOldFunc 已经注册的证券信息查询返回回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_QuerySecCode
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQuerySecCodeCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_SECCODE fNewFunc, PFN_RSP_QUERY_SECCODE *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册ETF基本信息查询返回回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的ETF基本信息查询返回回调函数
	* @param[out] pOldFunc 已经注册的ETF基本信息查询返回回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_QueryEtfBaseInfo
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryEtfBaseInfoCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_ETFBASEINFO fNewFunc, PFN_RSP_QUERY_ETFBASEINFO *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册ETF篮子信息查询返回回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的ETF篮子信息查询返回回调函数
	* @param[out] pOldFunc 已经注册的ETF篮子信息查询返回回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_QueryEtfBasketInfo
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryEtfBasketInfoCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_ETFBASKETINFO fNewFunc, PFN_RSP_QUERY_ETFBASKETINFO *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 查询账户资金
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryAccount 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_ACCOUNT
	* @see KFTSAPI_RegQueryAccountCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryAccount(APIHandle hApiHandle, STQueryAccount *pQryAccount, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 查询持仓
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryPosition 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_POSITION
	* @see KFTSAPI_RegQueryPositionCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryPosition(APIHandle hApiHandle, STQueryPosition *pQryPosition, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 查询报价单
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryQuote 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_QUOTE
	* @see KFTSAPI_RegQryQuoteCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_ReqQryQuote(APIHandle hApiHandle, STQryQuote* pQryQuote, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 查询委托
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryOrder 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_ORDER
	* @see KFTSAPI_RegQueryOrderCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryOrder(APIHandle hApiHandle, STQueryOrder *pQryOrder, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 查询成交
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryDone 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_DONE
	* @see KFTSAPI_RegQueryDoneCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryDone(APIHandle hApiHandle, STQueryDone *pQryDone, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 查询证券信息
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQrySecCode 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_SECCODE
	* @see KFTSAPI_RegQuerySecCodeCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QuerySecCode(APIHandle hApiHandle, STQuerySecCode *pQrySecCode, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 查询ETF基本信息
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryETFBaseInfo 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_ETFBASEINFO
	* @see KFTSAPI_RegQueryEtfBaseInfoCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryEtfBaseInfo(APIHandle hApiHandle, STQueryEtfBaseInfo *pQryETFBaseInfo, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 查询ETF篮子信息
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryEFTBasketInfo 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_ETFBASKETINFO
	* @see KFTSAPI_RegQueryEtfBasketInfoCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryEtfBasketInfo(APIHandle hApiHandle, STQueryEtfBasketInfo *pQryEFTBasketInfo, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 查询费率信息
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryFeeInfo 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_FEEINFO
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryFeeInfo(APIHandle hApiHandle, STQueryFeeInfo *pQryFeeInfo, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 注册费率信息查询返回回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的费率信息查询返回回调函数
	* @param[out] pOldFunc 已经注册的费率信息查询返回回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_QueryFeeInfo
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryFeeInfoCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_FEEINFO fNewFunc, PFN_RSP_QUERY_FEEINFO *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 根据委托流水号查询会话号和私有号
	* @param[in]  hApiHandle API句柄，不能为NULL
	* @param[in]  pQryPrivateInfoByNo 查询时需要输入委托流水号
	* @param[out] pPrivateInfo 返回的结构,包含会话号和私有号
	* @param[out] pRspMsg 失败时的错误信息
	* 该函数采用同步模式
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QuerySesidPrinoDirect(APIHandle hApiHandle, STQuerySesidPrino *pQryPrivateInfoByNo, STSesidPrinoInfo* pPrivateInfo, STRspMsg * pRspMsg);

	/**
	* @brief 注册股东代码和市场代码查询回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的股东代码和市场代码查询回调函数
	* @param[out] pOldFunc 已经注册的股东代码和市场代码查询回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 在SPX普通交易员登录时,主动回调该函数向SPX返回交易员对应市场代码和股东代码，暂不提供主动查询接口
	* @see PFN_RSP_QUERY_HOLDERNO 
	* @see KFTSAPI_QueryHolderno 
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryHoldernoCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_HOLDERNO fNewFunc, PFN_RSP_QUERY_HOLDERNO *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 股东代码和市场代码查询
	* @param[in]  hApiHandle API句柄，不能为NULL
	* @param[in]  pQryHolder 查询时输入客户号和市场代码
	* @param[in] nRequestID 请求编号
	* @param[out] pRspMsg 失败时的错误信息
	* @details 市场代码为0时查询所有市场的股东代码
	* @see PFN_RSP_QUERY_HOLDERNO 
	* @see KFTSAPI_RegQueryHoldernoCallBack 
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryHolderno(APIHandle hApiHandle, STQueryHolderno *pQryHolder, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 查询数据字典信息
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryDict 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_FEEINFO
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryDictionaryInfo(APIHandle hApiHandle, STQueryDictionaryInfo *pQryDict, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 查询期货普通合约
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryInstrument 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_FUT_CONT_INFO
	* @see KFTSAPI_RegQueryFutContCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryFutContInfo(APIHandle hApiHandle, STQueryFutContCode *pQryInstrument, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 注册查询期货合约应答回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的期货合约应答回调函数
	* @param[out] pOldFunc 已经注册的期货合约应答回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_FUT_CONT
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryFutContCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_FUT_CONT fNewFunc, PFN_RSP_QUERY_FUT_CONT *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 注册期货组合单委托回报推送处理回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的期货组合单委托回报推送处理回调函数
	* @param[out] pOldFunc 已经注册的期货组合单委托回报推送处理回调函数，可为NULL
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 注册期货组合单委托回报推送处理回调函数
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegOrderCombRtnCallBack(APIHandle hApiHandle, PFN_RTN_ORDER_COMB fNewFunc, PFN_RTN_ORDER_COMB * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 查询期货组合单委托
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryOrderCombine 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_ORDER_COMB
	* @see KFTSAPI_RegQueryOrderCombCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryOrderComb(APIHandle hApiHandle, STQueryOrderComb *pQryOrderCombine, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 注册委托查询返回回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的委托查询返回回调函数
	* @param[out] pOldFunc 已经注册的委托查询返回回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see KFTSAPI_QueryOrderComb
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryOrderCombCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_ORDER_COMB fNewFunc, PFN_RSP_QUERY_ORDER_COMB *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 查询期货组合合约
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryCombInstrument 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_FUT_CONT_COMB
	* @see KFTSAPI_RegQueryFutContCombCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryFutContComb(APIHandle hApiHandle, STQueryFutContCombCode *pQryCombInstrument, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 注册查询期货组合合约应答回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的期货组合合约应答回调函数
	* @param[out] pOldFunc 已经注册的期货组合合约应答回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_FUT_CONT_COMB
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryFutContCombCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_FUT_CONT_COMB fNewFunc, PFN_RSP_QUERY_FUT_CONT_COMB *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 查询期货组合合约持仓
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryPositionCombine 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_POSITION_COMB
	* @see KFTSAPI_RegQueryPositionCombCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryPositionComb(APIHandle hApiHandle, STQueryFutPositionComb *pQryPositionCombine, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 注册查询期货组合合约持仓应答回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的期货组合合约持仓应答回调函数
	* @param[out] pOldFunc 已经注册的期货组合合约持仓应答回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_POSITION_COMB
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryPositionCombCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_POSITION_COMB fNewFunc, PFN_RSP_QUERY_POSITION_COMB *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 查询期货组合合约持仓
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryPositionCombineDetail 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_POSITION_COMB_DET
	* @see KFTSAPI_RegQueryPositionCombDetCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryPositionCombDet(APIHandle hApiHandle, STQueryFutPositionCombDetail *pQryPositionCombineDetail, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 注册查询期货组合合约持仓明细应答回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的期货组合合约持仓明细应答回调函数
	* @param[out] pOldFunc 已经注册的期货组合合约持仓明细应答回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_POSITION_COMB_DET
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryPositionCombDetCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_POSITION_COMB_DET fNewFunc, PFN_RSP_QUERY_POSITION_COMB_DET *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 查询期货合约费率信息
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryFee 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_FUT_FEE_RATE
	* @see KFTSAPI_RegQueryFutFeeCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryFutFeeRate(APIHandle hApiHandle, STQueryFutFeeRate *pQryFee, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 注册查询期货合约费率应答回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的期货合约费率应答回调函数
	* @param[out] pOldFunc 已经注册的期货合约费率应答回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_FUT_FEE_RATE
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryFutFeeCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_FUT_FEE_RATE fNewFunc, PFN_RSP_QUERY_FUT_FEE_RATE *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief 查询期货组合合约策略代码信息
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] pQryFutCombStrategyInfo 查询条件，为NULL则查询所有记录
	* @param[in] nRequestID 查询序号
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_COMB_STRAT_CODE
	* @see KFTSAPI_RegQueryCombStratCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryFutCombStrategy(APIHandle hApiHandle, STQueryFutCombStrategyInfo *pQryFutCombStrategyInfo, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief 注册查询期货组合合约策略代码应答回调函数
	* @param[in] hApiHandle API句柄，不能为NULL
	* @param[in] fNewFunc 要使用的期货组合合约策略代码应答回调函数
	* @param[out] pOldFunc 已经注册的期货组合合约策略代码应答回调函数
	* @param[out] pRspMsg 失败时的错误信息
	* @return TRUE为成功，FALSE为失败
	* @details 
	* @see PFN_RSP_QUERY_COMB_STRATEGY_CODE
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryCombStratCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_COMB_STRATEGY fNewFunc, PFN_RSP_QUERY_COMB_STRATEGY *pOldFunc, STRspMsg * pRspMsg);
	
	/** @} */
#ifdef __cplusplus
}
#endif

#endif //__KFTS_TRADE_API_H__

/////////////////////////////////////////////////////////////////////////
///@system 金仕达新一代资管系统
///@company SunGard China
///@file kfts_trade_struct.h
///@brief 定义了金仕达资管系统数据结构
///@history
///20141001	Kingstar IRDG		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef __KFTS_TRADE_STRUCT_H__
#define __KFTS_TRADE_STRUCT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "kfts_trade_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define __PACKED__
#pragma pack(push, 4)
#else
#ifdef __GNUC__
#define __PACKED__    __attribute__ ((packed))
#else
#ifdef HP_UX
#define __PACKED__
#pragma pack 4
#else
#define __PACKED__
#pragma options align = packed
#endif
#endif
#endif

	/** \defgroup Group13 1.3 枚举定义 */
	/** @{ */
	/**
	* @brief APIHandle状态
	* @details 客户端可直接获取APIHandle状态。
	*/
	enum KFTS_API_STATUS
	{
		KFTS_API_INITIAL = 0,					///<初始化
		KFTS_API_CONNECTING,				///<连接中
		KFTS_API_CONNECTED,				///<已连接
		KFTS_API_LOGINING,					///<登录中
		KFTS_API_LOGINED,						///<已登录
		KFTS_API_RECONNECTING			///<重连中
	};

	/**
	* @brief 消息确认类型
	* @details 在调用下单或撤单函数时，API支持不同的消息确认类型。
	* 消息确认到API时，在API收到该请求后函数返回。
	* 消息确认到前置时，API会保证前置收到该请求或超时后函数才会返回。
	* 消息确认到交易服务时，API会保证交易服务收到该请求或超时后函数才会返回。
	* 以此类推。
	* @see KFTSAPI_Order
	* @see KFTSAPI_OrderEx
	* @see KFTSAPI_OrderCancel
	*/
	enum KFTS_MSG_CONFIRM_TYPE
	{
		KFTS_FROM_API            = 0,   ///< 消息确认到API(缺省选项)
		KFTS_FROM_FRONT_SERVER,         ///< 消息确认到前置
		KFTS_FROM_TRADE_SERVER,         ///< 消息确认到交易服务
		KFTS_FROM_OFFER_SERVER,         ///< 消息确认到报盘服务
	};

	/**
	* @brief 重登录接收推送类型
	* @details 重新登录成功时，API会将之前从前置收到的所有消息重新反演一遍。
	* 客户端可以选择API以何种方式回调客户端。
	*/
	enum KFTS_RELOGIN_TYPE
	{
		KFTS_RELOGIN_RESUME     = 0,    ///< 从上次接收推送结束位置开始回调(缺省选项)
		KFTS_RELOGIN_RESTART,           ///< 重新执行所有回调
		KFTS_RELOGIN_QUICK,             ///< 仅对后续推送执行回调
	};

	/**
	* @brief 日志级别
	* @details API记录日志的级别，设置为某一级别后，该级别及更严重级别的日志都会记录到日志文件中。
	*/
	enum KFTS_LOG_LEVEL
	{
		KFTS_LL_DEBUG           = 0,    ///< 调试级别
		KFTS_LL_INFO,                   ///< 信息级别(缺省选项)
		KFTS_LL_WARNING,                ///< 警告级别
		KFTS_LL_ERROR,                  ///< 错误级别
		KFTS_LL_FATAL,                  ///< 严重错误级别
	};

	/**
	* @brief 选项类型
	* @details 客户端可以根据需要，修改API中的一些选项参数值。
	* @see KFTSAPI_SetOpt
	*/
	enum KFTS_OPTION
	{
		KFTS_OPT_SESSION_ID     = 0,    ///< 设置会话号,选项值为int类型,缺省值为0
		KFTS_OPT_RELOGIN_TYPE,          ///< 重登录接收推送类型,选项值为\ref KFTS_RELOGIN_TYPE 类型，缺省值为\ref KFTS_RELOGIN_RESUME
		KFTS_OPT_CONFIRM_TYPE,          ///< 消息确认类型,选项值为\ref KFTS_MSG_CONFIRM_TYPE 类型，缺省值为\ref KFTS_FROM_API
		KFTS_OPT_DEFAULT_TIMEOUT,       ///< 委托撤单默认超时时间(ms),选项值为int类型,缺省值为3000
		KFTS_OPT_LOG_LEVEL,             ///< 日志级别,选项值为\ref KFTS_LOG_LEVEL 类型，缺省值为\ref KFTS_LL_INFO
		KFTS_OPT_HEARTBEAT_INTERVAL,    ///< 发送心跳请求间隔(s),选项值为int类型,缺省值为2
		KFTS_OPT_HEARTBEAT_TIMEOUT,     ///< 接收心跳应答超时时间(s),必须小于等于心跳请求间隔,选项值为int类型,缺省值为2
		KFTS_OPT_HEARTBEAT_MAX_LOSS,    ///< 最大可容忍心跳丢失次数,选项值为int类型,缺省值为5
		KFTS_OPT_CALLBACK_START,        ///< 期望开始接收的回调号,无符号整数
		KFTS_OPT_STREAM_FILEPATH,       ///< 期望使用的流文件目录，要求不同handle不能共享
	};
	/** @} */

	/** \defgroup Group14 1.4 数据结构定义 */
	/** @{ */
	/// 无超时限制
#ifndef INFINITE
#define INFINITE            0xFFFFFFFF
#endif

	/// 批量委托最大数量
#define MAX_ORDER_COUNT 300
	/// 批量撤单最大数量
#define MAX_ORDER_CANCEL_COUNT  300


	/// 空行
#define EMPTY_ROW       -1
	/// 空ID
#define EMPTY_ID        -1
	/// 空NO
#define EMPTY_NO        -1

	/// 应答消息
	typedef struct _STRspMsg
	{
		TErrorIDType        error_no;                   ///< 错误代码
		TErrorMsgType       msg;                        ///< 错误信息
	}__PACKED__ STRspMsg, *PSTRspMsg;

	/// 交易员登录指令
	typedef struct _STTraderLogin
	{
		TCustNoType         cust_no;							 ///< 客户号
		TCustPwdType        cust_pwd;						///< 客户密码
		TTerminalTypeType   terminal_type;              ///< 终端类型
		TNoteType           note;									///< 备注
		TMacType            mac_addr;							///< MAC地址，一般情况下，登录时无需填写该字段，API会自动取得并填充。
																			///< 某些情况下，API无法确定当前使用的MAC地址，此处客户端必须填写该字段。
		TTerminalNameType   terminal_name;              ///< 客户端名称和版本号
		TLicenseCodeType    license_code;               ///< 授权码
		TDomainNameType     domain_id ;                 ///< 客户域编号
	} __PACKED__ STTraderLogin, *PSTTraderLogin;

	/// 登录应答
	typedef struct _STTraderLoginRsp
	{
		TSessionIDType      session_id;						///< 会话号
		TConnSessionIDType  conn_session_id;         ///< 连接会话号，整个系统全局唯一
		TInstanceIDType     instance_id;						///< 柜台实例号(非0,char[16])
		TPrivateNoType      max_private_no;             ///< 上次会话最大私有号
		TCustNameType       cust_name;                  ///< 客户姓名
		TDateType           tx_date;							///< 交易日期

		TTRADER_ID_TYPE     trader_id;                  ///< 交易员编号 
		TDateType           login_date;                 ///< 登陆日期yyyymmdd,服务器当前日期
		TDateType           login_time;                 ///< 登陆时间0-235959,服务器当前时间 
		TFRONT_KEY_TYPE     front_key;                   ///< 前置机编号 
		TSystemNameType     system_name ;               ///< 资管系统名称

	} __PACKED__ STTraderLoginRsp, *PSTTraderLoginRsp;

	/// SPX交易员校验指令
	typedef struct _STTraderCheck
	{
		TCustNoType         cust_no;								 ///< 客户号
		TCustPwdType        cust_pwd;							  ///< 客户密码
		TTerminalTypeType   terminal_type;					 ///< 终端类型
		TSPXCheckUserType	spx_check_type;				///< 校验类型 1 只校验TCP关系 2 只校验密码 3 皆校验,对应SPX2号功能 
																				///<  4 皆校验,对应SPX61号功能
	} __PACKED__ STTraderCheck, *PSTTraderCheck;	


	/// Api信息
	typedef struct _STApiInfo
	{
		TApiVersionType     api_version;                ///< Api版本号
	} __PACKED__ STApiInfo, *PSTApiInfo;

	/// 委托请求
	typedef struct _STOrder
	{
		TPrivateNoType      private_no;                 ///< 私有号
		TCellIDType         cell_id;                    ///< 资产单元编号(交易员有多个资产单元时需要填写，否则仅需填为EMPTY_ID)
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号(资产单元有多个投资组合时需要填写，否则仅需填为EMPTY_ID)
		TMarketCodeType     market_code;                ///< 市场代码(见数据字典-市场代码)
		TInstrumentIDType        sec_code;                   ///< 合约代码
		TOffsetFlagType      offset_flag;                 ///< 开平标志类型(见数据字典-开平标志类型)
		TDirectionType             bs;                         ///< 买卖标记(见数据字典-买卖类型)
		TMarketOrderFlagType market_order_flag;         ///< 市价委托标记(见数据字典-市价单类型)
		TPriceType          price;                      ///< 委托价格
		TLargeVolumeType       order_vol;                  ///< 委托数量
		TOrderPropType      order_prop;                 ///< 委托属性(预留)
		//		TForceFlag          forceflag;                  ///< 强平标记(期货)
		TDoneProp           doneprop;                   ///< 成交属性(期货)
		TInstructID         instructid;                 ///< 指令编号		
		//		TTimeType			order_date;					///< 交易日期(期货)
		TFutOrderType		OrderType;					///< 委托单类型(期货)
		THedgeFlag		hedge_flag;					///< 投保标记(期货)
		TPolicyNo			policy_no;					///< 下单策略号
		TTradeType TradeType;						///< 交易类型
	}__PACKED__ STOrder, *PSTOrder;

	/// 委托请求(期货组合单)
	typedef struct _STOrderComb
	{
		TPrivateNoType      private_no;                 ///< 私有号
		TCellIDType         cell_id;                    ///< 资产单元编号(交易员有多个资产单元时需要填写，否则仅需填为EMPTY_ID)
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号(资产单元有多个投资组合时需要填写，否则仅需填为EMPTY_ID)
		TMarketCodeType     market_code;                ///< 市场代码(见数据字典-市场代码)
		TCombinInstrIDType    sec_code;                   ///< 期货组合单合约代码
		TOffsetFlagType      offset_flag;                 ///< 开平标志类型(见数据字典-开平标志类型)
		TDirectionType             bs;                         ///< 买卖标记(见数据字典-买卖类型)
		TMarketOrderFlagType market_order_flag;         ///< 市价委托标记(见数据字典-市价单类型)
		TPriceType          price;                      ///< 委托价格
		TLargeVolumeType       order_vol;                  ///< 委托数量
		TOrderPropType      order_prop;                 ///< 委托属性(预留)

		TInstructID         instructid;                 ///< 指令编号
		//		TForceFlag          forceflag;                  ///< 强平标记(期货)
		TDoneProp           doneprop;                   ///< 成交属性(期货)
		//		TTimeType			order_date;					///< 交易日期(期货)
		TStrategyCode		StrategyCode;				///< 组合策略代码(期货)
		TCombType			CombinationType;			///< 组合类型(期货)
		TFutOrderType		OrderType;					///< 委托单类型(期货)
		TPolicyNo			policy_no;					///< 下单策略号
	}__PACKED__ STOrderComb, *PSTOrderComb;

	/// 撤单请求(按委托号撤单)
	/**
	* @details 下一版本将取消该结构体，由新撤单结构体替代
	* @see _STInputOrderAction
	*/
	typedef struct _STOrderCancelByOrderNo
	{
		TMarketCodeType		market_code;				///< 市场代码
		TOrderNoType		order_no;					///< 交易核心委托流水号
	}__PACKED__ STOrderCancelByOrderNo, *PSTOrderCancelByOrderNo;

	/// 输入报单操作
	/**
	* @details 
	* 撤单方式一:MarketCode+OrderLocalID 根据本地委托号撤单；<br>
	* 撤单方式二:SessionID+PrivateNo 根据私有号撤单；<br>
	* 若采用撤单方式二，客户端则需自行维护该私有号，避免同一会话号上重复。
	*/
	typedef struct _STInputOrderAction
	{
		TTradeType					   TradeType;				///< 交易类型(必填)
		TMarketCodeType            MarketCode;				///< 市场代码
		TOrderNoType				   OrderLocalID;				///< 交易核心委托流水号
		TSessionIDType                SessionID;                 ///< 会话号
		TPrivateNoType                PrivateNo;                 ///< 私有号
	}__PACKED__ STInputOrderAction, *PSTInputOrderAction;

	/// 委托信息(委托应答、委托查询应答)
	typedef struct _STOrderInfo
	{
		STOrder             order;                      ///< 委托结构
		TSessionIDType      session_id;                 ///< 会话号
		TBatchNoType        batch_no;                   ///< 批号
		TOrderNoType        order_no;                   ///< 系统委托流水号
		TOrderStatusType    order_status;               ///< 委托状态(见数据字典-委托状态)
		TAmountType     frozen_amount;              ///< 冻结金额. 期货为冻结保证金
		TLargeVolumeType       frozen_vol;                 ///< 冻结数量
		TAmountType     done_amount;                ///< 成交金额
		TLargeVolumeType       done_vol;                   ///< 成交数量
		TLargeVolumeType       cancel_vol;                 ///< 已撤销数量
		TTimeType			order_date;					///< 委托日期
		TTimeType           order_time;                 ///< 委托时间
		TTimeType			cancel_time;                ///< 撤单时间
		TTimeType			cancel_submit_time;           ///< 撤单提交交易所时间
		TErrorIDType        error_no;                   ///< 委托错误代码
		TErrorMsgType       err_msg;                    ///< 委托错误信息
		TErrorIDType        cancel_error_no;            ///< 撤单错误代码
		TErrorMsgType       cancel_err_msg;             ///< 撤单错误信息
		TTimeType			order_submit_time;                  ///< 报单提交交易所时间
		TAmountType     fee;                        ///< 费用
		// 1.0.2版本新加字段
		TAmountType     dis_fee;								///< 分笔费用
		TAmountType     FrozenRoyalty;                    ///< 冻结权利金
		TOrderNoType	  QuoteLocalID;						//本地报价编号
	}__PACKED__ STOrderInfo, *PSTOrderInfo;

	/// 委托信息(期货组合单委托应答、期货组合单委托查询应答)
	typedef struct _STOrderCombInfo
	{
		STOrderComb         order;                      ///< 委托结构
		TSessionIDType      session_id;                 ///< 会话号
		TBatchNoType        batch_no;                   ///< 批号
		TOrderNoType        order_no;                   ///< 系统委托流水号
		TOrderStatusType    order_status;               ///< 委托状态(见数据字典-委托状态)
		TAmountType     frozen_amount;              ///< 冻结金额. 期货为冻结保证金
		TLargeVolumeType       frozen_vol;                 ///< 冻结数量
		TAmountType     done_amount;                ///< 成交金额
		TLargeVolumeType       done_vol;                   ///< 成交数量
		TLargeVolumeType       cancel_vol;                 ///< 已撤销数量
		TTimeType			order_date;					///< 委托日期
		TTimeType           order_time;                 ///< 委托时间
		TTimeType			cancel_time;                ///< 撤单时间
		TTimeType			cancel_submit_time;           ///< 撤单报入交易所时间
		TErrorIDType        error_no;                   ///< 委托错误代码
		TErrorMsgType       err_msg;                    ///< 委托错误信息
		TErrorIDType        cancel_error_no;            ///< 撤单错误代码
		TErrorMsgType       cancel_err_msg;             ///< 撤单错误信息
		TTimeType			order_submit_time;                  ///< 委托报入交易所时间
		TAmountType     fee;                        ///< 费用
		// 1.0.2版本新加字段
		TAmountType     dis_fee;                    ///< 分笔费用
	}__PACKED__ STOrderCombInfo, *PSTOrderCombInfo;

	/// 成交信息(成交返回、成交查询应答)
	typedef struct _STDoneInfo
	{
		TDoneNoType         done_no;                    ///< 系统成交号
		TTradeIDType         trade_id;						///< 交易所成交号
		TSessionIDType      session_id;                 ///< 会话号
		TPrivateNoType      private_no;                 ///< 私有号
		TCellIDType         cell_id;                    ///< 资产单元编号
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号
		TMarketCodeType     market_code;                ///< 市场代码
		TInstrumentIDType        sec_code;                   ///< 合约代码
		TBatchNoType        batch_no;                   ///< 批号
		TOrderNoType        order_no;                   ///< 系统委托流水号
		TPriceType          done_price;                 ///< 成交价格
		TLargeVolumeType       done_vol;                   ///< 成交数量
		TAmountType     done_amount;                ///< 成交金额
		TAmountType     fund_change_amount;         ///< 资金变动数量
		TLargeVolumeType       stock_vol_amount;           ///< 股份变动数量
		TTimeType           done_time;                  ///< 成交时间
		TTimeType			done_date;					///< 成交日期(期货)
		TAmountType     Margin;                ///< 保证金
		TDirectionType             bs;                         ///< 买卖标记(见数据字典-买卖类型)
		TOffsetFlagType      offset_flag;                 ///< 开平标志类型(见数据字典-开平标志类型)
		THedgeFlag		hedge_flag;					///< 投保标记(期货)
		TTradeType      trade_type;					///<交易类型
	}__PACKED__ STDoneInfo, *PSTDoneInfo;

	/// 查询账户资金请求
	typedef struct _STQueryAccount
	{
		TCellIDType         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		TAccountTypeType    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合,-1为全部
		TCurrencyIDType   currency_type;              ///< 币种(0-所有币种，1-人民币)
		TQueryAccountProp   query_account_prop;         ///< 查询账户资金属性(0-查询当前账户资金,1-查询开盘前账户资金)
		TAccType            acc_type;					///< 资金账户类型(0-储蓄类,1-证券A股类,2-期货类,3-个股期权)
	}__PACKED__ STQueryAccount, *PSTQueryAccount;

	/// 账户资金信息
	typedef struct _STAccountInfo
	{
		TCellIDType         cell_id;                    ///< 资产单元编号
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号
		TAccountTypeType    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合
		TCurrencyIDType   currency_type;              ///< 币种

		TAccType            acc_type;					///< 资金账户类型(0-储蓄类,1-证券A股类,2-期货类,3-个股期权)

		TAmountType     deposite;                   ///< 资金余额
		TAmountType     available;                  ///< 可取资金余额
		TAmountType     buyable;                    ///< 可用资金余额
		/// 1.0.1版本 新增字段
		TAmountType     buy_frz_amt;                ///< 买入冻结金额
		TAmountType     buy_done_amt;               ///< 买入成交金额
		TAmountType     sell_done_amt;              ///< 卖出成交金额

		/// 新增
		TAmountType     dealfrozcap;                ///< 未成交的委托冻结金额
		TAmountType     abnormal_frzn_amt;          ///< 异常冻结
		TAmountType     manual_unfrzn_amt;          ///< 手动解冻资金
		TAmountType     margin;                     ///< 保证金
		TAmountType     outcap;                     ///< 当日划出资金
		TAmountType     incap;                      ///< 当日划入资金
		TAmountType     realprofit;                 ///< 实现盈亏
		TAmountType     forbid_asset;               ///< 禁取资产
		TAmountType	 dthis_bal;					///< 昨日余额(证券期货整合新增)
		TAmountType     manual_frzn_amt;			///< 手动冻结资金(期货)
		TAmountType     RoyaltyIn;               ///< 权利金收入
		TAmountType	 RoyaltyOut;					///< 权利金支出
		TAmountType     RoyaltyFrozen;			///< 权利金冻结
	}__PACKED__ STAccountInfo, *PSTAccountInfo;

	/// 查询持仓请求
	typedef struct _STQueryPosition
	{
		TCellIDType         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		TAccountTypeType    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合,-1为全部
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		TQueryPositionProp  query_positon_prop;         ///< 查询持仓属性(0-查询当前持仓,1-查询开盘前持仓)

		THedgeFlag        hedgingflag;                ///< 投机套保标志(证券和期货整合新增)
		TPosiDirectionType           PosiDirection;                   ///< 持仓多空类型(0-多仓,1-空仓)(证券和期货整合新增)
		TTradeType         TradeType;						  ///<交易类型
	}__PACKED__ STQueryPosition, *PSTQueryPosition;

	/// 持仓信息
	typedef struct _STPositionInfo
	{
		TCellIDType         cell_id;                    ///< 资产单元编号
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号
		TAccountTypeType    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合
		TMarketCodeType     market_code;                ///< 市场代码
		TInstrumentIDType        sec_code;                   ///< 合约代码
		THedgeFlag        hedgingflag;                ///< 投机套保标志
		TPosiDirectionType           PosiDirection;                   ///< 持仓多空类型(0-多仓,1-空仓)
		TLargeVolumeType       pre_total_vol;                   ///< 昨日持仓总量
		TLargeVolumeType       pre_remain_vol;                   ///< 昨日持仓余量
		TLargeVolumeType       total_vol;                  ///< 当前总持仓
		TLargeVolumeType       avail_vol;                  ///< 当前可用余额
		TLargeVolumeType       buy_vol;                    ///< 当日买入成交数量
		TLargeVolumeType       sell_vol;                   ///< 当日卖出成交数量
		TAmountType     total_cost;                 ///< 持仓成本
		TPriceType          avg_price;                  ///< 持仓均价
		TAmountType     commission;            ///< 手续费
		TAmountType     OpenCommission;    ///< 开仓手续费
		TAmountType     CloseCommission;     ///< 平仓手续费
		TAmountType     realized_profit;            ///< 已实现盈亏
		TLargeVolumeType       sell_frz_vol;               ///< 卖出冻结数量
		TAmountType     buy_done_amt;               ///< 买入成交金额
		TAmountType     sell_done_amt;              ///< 卖出成交金额
		TLargeVolumeType       etf_frz_vol;                ///< ETF申赎冻结数量
		TLargeVolumeType       etf_rtn_vol;                ///< ETF申赎成交数量
		TLargeVolumeType       sell_frz_undone_vol;        ///< 卖出临时冻结数量
		TLargeVolumeType       etf_frz_undone_vol;         ///< ETF申赎临时冻结数量
		TLargeVolumeType       abnormal_frzn_vol;          ///< 异常冻结
		TLargeVolumeType       manual_unfrzn_vol;          ///< 可用解冻
		TLargeVolumeType       mortgage_frozen_vol;        ///< 质押入库冻结数量
		TLargeVolumeType       etf_left_vol;               ///< ETF申赎优先卖出剩余数量
		TLargeVolumeType       cur_buy_etf_left_vol;       ///< 当日买入优先申赎剩余数量
		TLargeVolumeType		curr_froz;					///< 手工冻结数量(期货)
		TLargeVolumeType		curr_unfroz;				///< 手工解冻数量(期货)
		TLargeVolumeType        detf_frozen_vol;            ///< ETF申赎成交冻结数量
		/// 期货整合新增
		//TLargeVolumeType       stkfrozamt;                 ///< 未成交的委托冻结数量，不含ETF申赎冻结
		TAmountType     usemargin;		       ///<占用保证金
		TAmountType     yd_usemargin;		  ///<昨占用保证金
		TAmountType     total_margin;		  ///<总保证金
		TLargeVolumeType     discount;		              ///<优惠数量
		TAmountType     total_profit;						///< 累计实现盈亏
		TTradeType     TradeType;							 ///<交易类型
		TLargeVolumeType locked_vol;					///<已锁定数量
		TLargeVolumeType avail_lock_vol;				///<可锁定数量
		TLargeVolumeType unlocked_vol;				///<已解锁数量
		TLargeVolumeType avail_unlock_vol;			/// <可解锁数量
		TLargeVolumeType coverd_frozen_vol;		/// <备兑冻结数量
		TLargeVolumeType MarginTradeFrozenVolume;	/// <融资回购标准券冻结数量
	}__PACKED__ STPositionInfo, *PSTPositionInfo;

	/// 查询委托请求
	typedef struct _STQueryOrder
	{
		TSessionIDType      session_id;                 ///< 会话号(非EMPTY_ID时按会话号查询)
		TPrivateNoType      private_no;                 ///< 私有号(非EMPTY_NO时按私有号查询)
		TBatchNoType        batch_no;                   ///< 批号(非EMPTY_NO时按批号查询)
		TCellIDType         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		TQueryOrderPropType query_order_prop;           ///< 查询委托属性(0为查询所有,1为仅查询可撤单委托)
		TTradeType     TradeType;						  ///<交易类型
	}__PACKED__ STQueryOrder, *PSTQueryOrder;

	/// 查询委托请求(期货组合单)
	typedef struct _STQueryOrderComb
	{
		TSessionIDType      session_id;                 ///< 会话号(非EMPTY_ID时按会话号查询)
		TPrivateNoType      private_no;                 ///< 私有号(非EMPTY_NO时按私有号查询)
		TBatchNoType        batch_no;                   ///< 批号(非EMPTY_NO时按批号查询)
		TCellIDType         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TCombinInstrIDType    sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		TQueryOrderPropType query_order_prop;           ///< 查询委托属性(0为查询所有,1为仅查询可撤单委托)
	}__PACKED__ STQueryOrderComb, *PSTQueryOrderCombr;


	/// 查询成交请求
	typedef struct _STQueryDone
	{
		TSessionIDType      session_id;                 ///< 会话号(非EMPTY_ID时按会话号查询)
		TPrivateNoType      private_no;                 ///< 私有号(非EMPTY_NO时按私有号查询)
		TBatchNoType        batch_no;                   ///< 批号(非EMPTY_NO时按批号查询)
		TCellIDType         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		TTradeType     TradeType;						  ///<交易类型
	}__PACKED__ STQueryDone, *PSTQueryDone;

	/// 证券基本信息查询
	typedef struct _STQuerySecCode
	{
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
	}__PACKED__ STQuerySecCode, *PSTQuerySecCode;

	/// 证券基本信息
	typedef struct _STSecCodeInfo
	{
		TMarketCodeType     market_code;                ///< 市场代码
		TInstrumentIDType        sec_code;                   ///< 合约代码
		TInstrumentNameType        sec_name;                   ///< 合约简称
		TProductTypeType     sec_variety;                ///< 品种类型(见数据字典-品种类型)
		TSecStatusType      sec_status;                 ///< 证券状态
		TPriceType          raise_limit_price;          ///< 涨停价格
		TPriceType          fall_limit_price;           ///< 跌停价格
		TLargeVolumeType		equity;						///< 总股本
		TPriceType			last_price;					///< 昨收盘价格
	}__PACKED__ STSecCodeInfo, *PSTSecCodeInfo;

	/// ETF基本信息查询
	typedef struct _STQueryEtfBaseInfo
	{
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sgsh_tx_code;               ///< 申购赎回合约代码(为空时查询所有申购赎回合约代码)
	}__PACKED__ STQueryEtfBaseInfo, *PSTQueryEtfBaseInfo;

	/// ETF基本信息
	typedef struct _STEtfBaseInfo
	{
		TMarketCodeType     market_code;                ///< 市场代码
		TInstrumentIDType        sgsh_tx_code;               ///< 基金申购赎回合约代码
		TInstrumentIDType        bs_code;                    ///< 基金买卖合约代码
		TEtfNameType        etf_name;                   ///< ETF简称
		TUnitType           sgsh_min_unit;              ///< 申购赎回最小份额
		TAmountType     estimate_cash_bal;          ///< T日预估现金余额
		TAmountType     cash_bal;                   ///< T减1日现金余额差额
		TRatioType          max_cash_ratio;             ///< 现金替代比例上限
		TPublishIOPVFlag    publish_iopv_flag;          ///< 发布单位净参考值标志
		TAmountType     etf_nav;                    ///< 参考基金净值
		TEtfStatusType      etf_status;                 ///< ETF交易状态(见数据字典-ETF交易状态)
		TRecordNumType      record_num;                 ///< 篮子股票个数
	}__PACKED__ STEtfBaseInfo, *PSTEtfBaseInfo;

	/// ETF篮子信息查询
	typedef struct _STQueryEtfBasketInfo
	{
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sgsh_tx_code;               ///< 申购赎回合约代码(为空时查询所有申购赎回合约代码)
		TMarketCodeType     belong_market_code;         ///< 挂牌市场(为空时查询所有挂牌市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
	}__PACKED__ STQueryEtfBasketInfo, *PSTQueryEtfBasketInfo;

	/// ETF篮子信息
	typedef struct _STEtfBasketInfo
	{
		TMarketCodeType     market_code;                ///< 市场代码
		TInstrumentIDType        sgsh_tx_code;               ///< 申购赎回合约代码
		TMarketCodeType     belong_market_code;         ///< 挂牌市场
		TInstrumentIDType        sec_code;                   ///< 合约代码
		TInstrumentNameType        sec_name;                   ///< 合约简称
		TLargeVolumeType       stock_vol;                  ///< 股票数量
		TCashReplFlagType   cash_repl_flag;             ///< 现金替代标志(见数据字典-现金替代标志)
		TRatioType          over_ratio;                 ///< 溢价比例
		TAmountType     cash_repl_amt;              ///< 现金替代金额
	}__PACKED__ STEtfBasketInfo, *PSTEtfBasketInfo;

	///查询证券费率信息
	typedef struct _STQueryFeeInfo
	{		
		TInstrumentIDType        sec_code;                   ///< 合约代码     , sec_code[0]=-1,表示不关心该字段
		TMarketCodeType     market_code;                ///< 市场代码     , -1,表示不关心该字段      
		TDirectionType             bs;                         ///< 买卖标记     , -1,表示不关心该字段
		TMarketOrderFlagType market_order_flag;         ///< 市价委托标记 , -1,表示不关心该字段
		TCellIDType         cell_id;                    ///< 资产单元编号 , -1,表示不关心该字段
		TFeeCodeType        fee_code;					///< 费用代码      , -1,表示不关心该字段
		TProductTypeType     sec_variety;                ///< 品种类型     , -1,表示不关心该字段
	}__PACKED__ STQueryFeeInfo, *PSTQueryFeeInfo;

	///证券费率信息
	typedef struct _STFeeInfo
	{
		TInstrumentIDType        sec_code;                   ///< 合约代码
		TMarketCodeType     market_code;                ///< 市场代码       
		TDirectionType             bs;                         ///< 买卖标记
		TMarketOrderFlagType market_order_flag;         ///< 市价委托标记
		TCellIDType         cell_id;                    ///< 资产单元编号
		TFeeCodeType        fee_code;					///< 费用代码
		TProductTypeType     sec_variety;                ///< 品种类型
		TFeeCalcType        calc_method;				///< 计算方式
		TFeeValueType       fee_rate;					///< 直接费率
		TFeeValueType       max_fee;					///< 最高费用
		TFeeValueType       min_fee;					///< 最低费用
		TFeeDispartFlag     dispart_count_flag;			///< 分笔计算标志
	}__PACKED__ STFeeInfo,*PSTFeeInfo;

	///转账或查询集中交易请求,查询与转账不可在一次中发起，不支持一次多种币种
	typedef struct _STFundTransREQ
	{
		TCellIDType         cell_id;                    ///< 资产单元编号,不可为EMPTY_ID
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号
		TAccountTypeType    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合
		TPrivateNoType      private_no;                 ///< 转账私有号,非EMPTY_NO，保证唯一
		TCurrencyIDType   currency_type;              ///< 币种
		TTransFundFlag      type;                       ///<转账类型 [0]代表('1' 同步转入 '2' 同步转出 '3' 查询 )，[1]代表是否先查询('0'不查询，'1'查询)
		TCustPwdType        passwd;                     ///<在集中交易中的取款密码,必须输入
		TAmountType     deposite;                   ///< 转账金额,不可为负      
	}__PACKED__ STFundTransREQ, *PSTFundTransREQ;

	///转账或查询集中交易应答
	typedef struct _STFundTransANS
	{
		TCellIDType         cell_id;                    ///< 资产单元编号,不可为EMPTY_ID
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号，EMPTY_ID表示仅关心资产单元
		TAccountTypeType    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合
		TPrivateNoType      private_no;                 ///< 转账私有号,非EMPTY_NO，保证唯一
		TCurrencyIDType   currency_type;              ///< 币种        
		TTransFundFlag      type;                       ///<转账类型 [0]代表('1' 同步转入 '2' 同步转出 '3' 查询 )，[1]代表是否先查询('0'不查询，'1'查询)

		TAmountType     deposite;                   ///< 转账金额，查询时，表示返回的集中交易中的可转金额
		TAmountType     balance;                    ///<仅对查询，表示返回的集中交易中的资金余额
		TErrorIDType        success;                    ///<是否操作成功  1 成功  否则 不成功
		TErrorMsgType       err_msg;                    ///错误信息
		TAmountType     dthis_bal;                  ///昨日余额
		TAmountType     useful;                     ///可用资金
		TAmountType     total;                      ///资金余额
		TAmountType     takeout;                    ///可取资金

		TAmountType     outcap;                     ///< 当日划出资金 	  	 
		TAmountType     incap;                      ///< 当日划入资金
	}__PACKED__ STFundTransANS, *PSTFundTransANS;

	///查询会话号私有号
	typedef struct _STQuerySesidPrino
	{
		TOrderNoType        order_no;                   ///< 系统委托流水号
	}__PACKED__ STQuerySesidPrino, *PSTQuerySesidPrino;

	///查询会话号私有号返回信息
	typedef struct _STSesidPrinoInfo
	{
		TOrderNoType        order_no;                   ///< 系统委托流水号
		TSessionIDType      session_id;                 ///< 会话号
		TPrivateNoType      private_no;                 ///< 私有号
	}__PACKED__ STSesidPrinoInfo, *PSTSesidPrinoInfo;

	///查询股东代码
	typedef struct _STQueryHolderno
	{
		TCustNoType         cust_no;                    ///< 客户号
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		//新增
		TMarketCodeType     interface_type;             ///< 接口类型
	}__PACKED__ STQueryHolderno, *PSTQueryHolderno;

	///查询股东代码返回信息
	typedef struct _STHoldernoInfo
	{
		TCustNoType         cust_no;                    ///< 客户号
		TMarketCodeType     market_code;                ///< 市场代码

		//新增
		TMarketCodeType     interface_type;             ///< 接口类型

		THoldernoType		holder_no;					///< 股东代码
	}__PACKED__ STHoldernoInfo, *PSTHoldernoInfo;

	/// 应答消息
	typedef struct _STRspMsgInvalid
	{
		TSessionIDType      session_id;                 ///< 会话号
		TPrivateNoType      private_no;                 ///< 私有号
		TErrorIDType        error_no;                   ///< 错误代码
		TErrorMsgType       msg;                        ///< 错误信息
	}__PACKED__ STMsgInvalidInfo, *PSTRspMsgInvalid;

	/// 数据字典查询结构
	typedef struct _STQueryDictionaryInfo
	{
		int dict_entry;									///< 字典主项(非EMPTY_NO时按主项值查询)
		char sub_entry[20];								///< 字典子项(非EMPTY_NO时按子项值查询)
	}__PACKED__ STQueryDictionaryInfo,*PSTQueryDictionaryInfo;

	/// 数据字典信息
	typedef struct _STDictionaryInfo
	{
		int dict_entry;									///< 字典主项值
		char sub_entry[20];								///< 字典子项值
		char data_type;									///< 字典子项值数据类型,1-字符串 2-整数
		char item_comment[80];							///< 字典子项注释
	}__PACKED__ STDictionaryInfo,*PSTDictionaryInfo;

	/// 查询期货普通合约信息
	typedef struct _STQueryFutContCode
	{
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
	}__PACKED__ STQueryFutContCode, *PSTQueryFutContCode;

	/// 期货普通合约信息
	typedef struct _STFutContractInfo
	{
		TContractIndex		index;						///< 合约索引
		TInstrumentIDType		VariCode;					///< 合约代码
		TMarketCodeType		MarketCode;					///< 市场代码
		TContractStatus		VariStatus;					///< 合约状态
		TVolumeType		Hands;						///< 每手数量
		TPriceType			PriceUnit;					///< 最小变动价位
		TVolumeType		Maxhold;					///< 最大持仓数量
		TVolumeType		MinHand;					///< 最小下单手数
		TVolumeType		MaxHand;					///< 最大下单手数
		TVolumeType		MarketMaxhand;				///< 市价最大手数
		TPriceType			SettlementPrice;			///< 昨结算价		
		TPriceType		UpperLimitPrice;						//<今涨停价	
		TPriceType		LowerLimitPrice;						//<今跌停价	
		TStrikeModeType	StrikeMode;						//<执行类型
		TOptionsTypeType OptionsType;						//<期权类型
		TPriceType		StrikePrice;								//<执行价
		TInstrumentIDType	SubjectVariCode;					///< 标的物合约代码
		TPriceType		SubjectSettlementPrice;		//<标的物结算价
		TRatioType		Numberator;							//<最低保障系数分子
		TRatioType		Denominator;						//<最低保障系数分母
		TTradeType		TradeType;					 //<交易类型
		TOperDateType ExpireDate;					//<到期日
	}__PACKED__ STFutContractInfo, *PSTFutContractInfo;

	/// 查询期货组合合约信息
	typedef struct _STFutQueryContCombCode
	{
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TCombinInstrIDType    sec_code;                   ///< 合约代码(为空时查询所有合约代码)
	}__PACKED__ STQueryFutContCombCode, *PSTFutQueryContCombCode;

	/// 期货组合合约信息
	typedef struct _STFutContractCombInfo
	{
		TContractIndex		index;						///< 合约索引
		TCombinInstrIDType	VariCode;					///< 合约代码
		TMarketCodeType		MarketCode;					///< 市场代码
		TStrategyCode		StrategyCode;				///< 组合策略代码
		TLegsCnt			LegCount;					///< 腿数量
		TInstrumentIDType		VariCode1;					///< 合约代码1
		TInstrumentIDType		VariCode2;					///< 合约代码2
		TInstrumentIDType		VariCode3;					///< 合约代码3
		TInstrumentIDType		VariCode4;					///< 合约代码4
		TContractStatus		VariStatus;					///< 合约状态
		TVolumeType		Hands;						///< 每手数量
		TPriceType			PriceUnit;					///< 最小变动价位
		TVolumeType		Maxhold;					///< 最大持仓数量
		TVolumeType		MinHand;					///< 最小下单手数
		TVolumeType		MaxHand;					///< 最大下单手数
		TVolumeType		MarketMaxhand;				///< 市价最大手数
		TPriceType			SettlementPrice;			///< 昨结算价
	}__PACKED__ STFutContractCombInfo, *PSTFutContractCombInfo;

	/// 查询期货组合合约持仓请求
	typedef struct _STFutQueryPositionComb
	{
		TCellIDType         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		TAccountTypeType    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合,-1为全部
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		TQueryPositionProp  query_positon_prop;         ///< 查询持仓属性(0-查询当前持仓,1-查询开盘前持仓)
	}__PACKED__ STQueryFutPositionComb, *PSTFutQueryPositionComb;

	/// 期货组合合约持仓信息
	typedef struct  _STFutPositionCombInfo
	{
		TCellIDType			CellID;						///< 单元编号
		TPortfolioIDType	PortfolioID;					///< 组合编号
		TMarketCodeType		MarketCode;					///< 市场代码
		TCombinInstrIDType	VariCode;					///< 合约代码
		TDirectionType				BS;							///< 组合买卖方向
		THedgeFlag		SH;							///< 组合投保方向
		TVolumeType		YesterdayTotal;				///< 昨持仓量
		TVolumeType		YesterdayAvailable;			///< 昨可平仓量
		TAmountType	YesterdayTotalCost;			///< 昨持仓成本
		TVolumeType		Total;						///< 今总持仓量=昨+今
		TVolumeType		TotalAvailable;				///< 今总可平仓量=昨+今
		TAmountType	TotalCost;					///< 今总持仓成本=昨+今
		TStrategyCode		StrategyCode;				///< 组合策略代码
		TVolumeType		TradeFrozen;				///< 交易冻结数量=昨+今
		TCombType			CombinationType;			///< 组合类型	
		TAccountTypeType    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合
	}__PACKED__ STFutPositionCombInfo, *PSTFutPositionCombInfo;

	/// 查询期货组合合约持仓明细请求
	typedef struct _STFutQueryPositionCombDetail
	{
		TCellIDType         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		TPortfolioIDType    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		TMarketCodeType     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		TQueryPositionProp  query_positon_prop;         ///< 查询持仓属性(0-查询当前持仓,1-查询开盘前持仓)
	}__PACKED__ STQueryFutPositionCombDetail, *PSTFutQueryPositionCombDetail;

	/// 期货组合合约持仓明细信息
	typedef struct  _STFutPositionCombDetail
	{
		TCellIDType			CellID;						///< 单元编号
		TPortfolioIDType	PortfolioID;					///< 组合编号
		TMarketCodeType		MarketCode;					///< 市场代码
		TInstrumentIDType		VariCode;					///< 合约代码
		TDirectionType				BS;							///< 买卖方向
		THedgeFlag		SH;							///< 投保方向
		TDateType			OpenDate;					///< 开仓日期
		TDoneNoType			DealNo;						///< 成交号
		TPriceType			HoldPrice;					///< 持仓价格
		TPriceType			OpenPrice;					///< 开仓价格
		TAmountType	HoldMargin;					///< 持仓保证金
		TAmountType	FProfitOrLoss;				///< 浮动盈亏
		TVolumeType		HoldHands;					///< 持仓手数
		TTradeType			TradeType;					///<交易类型
	}__PACKED__ STFutPositionCombDetail, *PSTFutPositionCombDetail;

	/// 查询期货费率
	typedef struct _STQueryFutFeeRate
	{
		TCellIDType         CellID;                    ///< 资产单元编号
		TInstrumentIDType	    InstrumentID;                   ///< 合约代码
		TMarketCodeType		MarketCode;					///< 市场代码
		TTradeType TradeType;							///< 交易类型
	}__PACKED__ STQueryFutFeeRate, *PSTQueryFutFeeRate;

	/// 期货费率信息
	typedef struct _STFuturefee
	{
		TCellIDType         CellID;                    ///< 资产单元编号
		TInstrumentIDType     InstrumentID;                   ///< 合约代码
		TMarketCodeType		MarketCode;					///< 市场代码
		TTradeType			TradeType;							///< 交易类型
		TRatioType			BspecMarginAmt;				///< 买投机保证金按手数
		TRatioType			BspecMarginRate;			///< 买投机保证金按金额
		TRatioType			BhedgeMarginAmt;			///< 买保值保证金按手数
		TRatioType			BhedgeMarginRate;			///< 买保值保证金按金额
		TRatioType			SspecMarginAmt;				///< 卖投机保证金按手数
		TRatioType			SspecMarginRate;			///< 卖投机保证金按金额
		TRatioType			ShedgeMarginAmt;			///< 卖保值保证金按手数
		TRatioType			ShedgeMarginRate;			///< 卖保值保证金按金额
		TRatioType			BarbitrageMarginAmt;		///< 买套利保证金按手数
		TRatioType			BarbitrageMarginRate;		///< 买套利保证金按金额
		TRatioType			SarbitrageMarginAmt;		///< 卖套利保证金按手数
		TRatioType			SarbitrageMarginRate;		///< 卖套利保证金按金额
		TRatioType			EOcommiAmt;					///< 开平手续费按手数
		TRatioType			EOcommiRate;				///< 开平手续费按金额
		TRatioType			OTcommiAmt;					///< 平今手续费按手数
		TRatioType			OTcommiRate;				///< 平今手续费按金额
		TRatioType			hedgeEOcommiAmt;			///< 保值开平手续费按手数
		TRatioType			hedgeEOcommiRate;			///< 保值开平手续费按金额
		TRatioType			hedgeOTcommiAmt;			///< 保值平今手续费按手数
		TRatioType			hedgeOTcommiRate;			///< 保值平今手续费按金额
		TRatioType			arbitrageEOcommiAmt;		///< 套利开平手续费按手数
		TRatioType			arbitrageEOcommiRate;		///< 套利开平手续费按金额
		TRatioType			arbitrageOTcommiAmt;		///< 套利平今手续费按手数
		TRatioType			arbitrageOTcommiRate;		///< 套利平今手续费按金额
	}__PACKED__ STFuturefee,*PSTFuturefee;

	/// 查询期货组合合约策略信息
	typedef struct _STQueryFutCombStrategyInfo
	{
		TMarketCodeType		MarketCode;					///< 市场代码
		TStrategyCode		StrategyCode;				///< 组合策略代码
	}__PACKED__ STQueryFutCombStrategyInfo, *PSTQueryFutCombStrategyInfo;

	/// 期货组合合约策略信息
	typedef struct  _STFutCombStrategyInfo
	{
		TMarketCodeType		MarketCode;					///< 市场代码
		TStrategyCode		StrategyCode;				///< 组合策略代码
		TStrategyName		StrategyName;				///< 组合策略名称
		TCombType			CombinationType;			///< 组合类型
		TDirectionType				CombinationBS;				///< 组合买卖
		TOffsetFlagType		CombinationEO;				///< 组合开平
		TLegsCnt			LegCount;					///< 腿数量
		TDirectionType				BS1;						///< 买卖方向(腿1)
		TOffsetFlagType		EO1;						///< 开平方向(腿1)
		TLegRate			QtyScale1;					///< 数量比例(腿1)
		TLegCalSign			CalSymbol1;					///< 计算符号(腿1)
		TRatioType			MarginScale1;				///< 保证金比例(腿1)
		TDirectionType				BS2;						///< 买卖方向(腿2)
		TOffsetFlagType		EO2;						///< 开平方向(腿2)
		TLegRate			QtyScale2;					///< 数量比例(腿2)
		TLegCalSign			CalSymbol2;					///< 计算符号(腿2)
		TRatioType			MarginScale2;				///< 保证金比例(腿2)
		TDirectionType				BS3;						///< 买卖方向(腿3)
		TOffsetFlagType		EO3;						///< 开平方向(腿3)
		TLegRate			QtyScale3;					///< 数量比例(腿3)
		TLegCalSign			CalSymbol3;					///< 计算符号(腿3)
		TRatioType			MarginScale3;				///< 保证金比例(腿3)
		TDirectionType				BS4;						///< 买卖方向(腿4)
		TOffsetFlagType		EO4;						///< 开平方向(腿4)
		TLegRate			QtyScale4;					///< 数量比例((腿4)
		TLegCalSign			CalSymbol4;					///< 计算符号((腿4)
		TRatioType			MarginScale4;				///< 保证金比例((腿4)
	}__PACKED__ STFutCombStrategyInfo, *PSTFutCombStrategyInfo;

	/// 询价请求
	typedef struct  _STForQuoteInsert
	{
		TCellIDType         cell_id;                            /// <资产单元编号
		TMarketCodeType		MarketCode;			    ///< 市场代码
		TInstrumentIDType     sec_code;                     ///< 合约代码
		TPrivateNoType      private_no;                 ///< 私有号
		TOperTimeType		InsertTime;					//询价录入时间
	}__PACKED__ STForQuoteInsert, *PSTForQuoteInsert;

	/// 询价应答
	typedef struct  _STRspForQuoteInsert
	{
		TCellIDType         cell_id;                            /// <资产单元编号
		TPrivateNoType      private_no;                 ///< 私有号
	}__PACKED__ STRspForQuoteInsert, *PSTRspForQuoteInsert;

	/// 标的券锁定解锁请求
	typedef struct  _STReqUnderlyingStockAction
	{
		TPrivateNoType      private_no;                  ///< 私有号
		TCellIDType         cell_id;                            ///< 资产单元编号
		TPortfolioIDType    portfolio_id;                     ///< 投资组合编号
		TMarketCodeType		market_code;		    ///< 市场代码
		TInstrumentIDType     sec_code;                     ///< 合约代码
		TPolicyNo			policy_no;					        ///< 策略编号
		TLargeVolumeType  stock_vol;						///<数量
		TLockType		lock_side;							///<锁定解锁类型
	}__PACKED__ STReqUnderlyingStockAction, *PSTReqUnderlyingStockAction;

	/// 标的券锁定解锁信息
	typedef struct  _STUnderlyingStockAction
	{
		TSessionIDType		 session_id;				     ///<会话号
		TPrivateNoType      private_no;                  ///< 私有号
		TCellIDType         cell_id;                            ///< 资产单元编号
		TPortfolioIDType    portfolio_id;                     ///< 投资组合编号
		TPolicyNo			policy_no;					        ///< 策略编号
		TMarketCodeType		market_code;		    ///< 市场代码
		TInstrumentIDType     sec_code;                     ///< 合约代码
		TLockType		lock_side;							///<锁定解锁类型
		TLargeVolumeType  stock_vol;						///<数量
		TOrderNoType        order_no;                   ///< 系统委托流水号
		TBatchNoType        batch_no;                   ///< 批号
		TOrderStatusType    order_status;            ///< 委托状态
		TErrorIDType        error_no;                     ///< 错误代码
		TErrorMsgType       error_msg;                 ///< 错误信息
		TLargeVolumeType       frozen_vol;                 ///< 冻结数量
		TLargeVolumeType       done_vol;                   ///< 成交总数量
		TLargeVolumeType		buy_call_vol;				   ///<可用于卖出和申赎的成交数量
		TLargeVolumeType		notbuy_call_vol;		   ///<不可用于卖出，可申赎的成交数量
		TLargeVolumeType		buy_notcall_vol;		  ///<可用于卖出，不可申赎的成交数量
		TTimeType           order_time;                 ///< 委托时间
		TTimeType			cancel_time;                ///< 撤单时间
		TTimeType			cancel_done_time;       ///< 撤单成交时间
		TTimeType			done_time;                  ///< 最后成交时间
		TContractType		RrtContract;				  ///<交易所订单合同号
		TContractType		CancelContract;		 ///<交易所订单撤单合同号
	}__PACKED__ STUnderlyingStockAction, *PSTUnderlyingStockAction;

	/// 输入的报价
	typedef struct _STInputQuote
	{
		TPrivateNoType			PrivateNo;							///<前端私有号
		TCellIDType			    CellID;								///<单元编号
		TPortfolioIDType		PortfolioID;				        ///<组合编号
		TMarketCodeType		MarketCode;					    ///<市场代码
		TCombinInstrIDType	SecCode;							///<合约代码
		TOffsetFlagType		BidOffsetFlag;					///<买开平方向
		THedgeFlag				BidHedgeFlag;					///<买投保方向
		TPriceType				BidPrice;							///<买委托价格
		TVolumeType			BidVolume;						///<买委托手数
		TOffsetFlagType		AskOffsetFlag;					///<卖开平
		THedgeFlag				AskHedgeFlag;					///<卖投保方向
		TPriceType				AskPrice;						    ///<卖委托价格
		TVolumeType			AskVolume;						///<卖委托手数
		TMarketOrderFlagType		MarketOrderFlag;		///<定单类型
		TDoneProp				DoneProp;						///<成交属性
		TFutOrderType			OrderType;						///<委托单类型
		TBatchNoType			BatchNo;							///<批号
		TPolicyNo					PolicyNo;						    ///<策略号
		TTradeType				TradeType;						///<交易类型
		TOrderPropType		OrderProp;					    ///<委托属性（保留）
		TQuoteType				QuoteType;					    ///<报价类型
		TOrderNoType			ForQuoteSysID;				///<应价编号
	}__PACKED__ STInputQuote, *PSTInputQuote;

	/// 报价
	typedef struct _STQuote
	{
		STInputQuote	InputQuote;					///<输入的报价
		TSessionIDType	 SessionID;				    ///<会话号
		TOrderNoType	QuoteLocalID;				///<本地报价编号
		TTimeType		ActiveTime;					///<激活时间（交易核心）
		TTimeType		UpdateTime;					///<最后修改时间（交易所）
		TOrderNoType	BidOrderLocalID;			///<买方报单编号
		TOrderNoType	AskOrderLocalID;		    ///<卖方报单编号
		TVolumeType	LongFrozen;					///<买持仓冻结
		TVolumeType	ShortFrozen;				///<卖持仓冻结
		TAmountType	FrozenCash;					///<冻结资金
		TErrorIDType		ErrorID;						///<错误代码（0:核心受理成功;1：交易所受理成功; 大于1:核心处理失败;-1: 报盘、柜台、及交易所错误）
		TErrorMsgType	ErrorMsg;						///<错误信息
	}__PACKED__ STQuote, *PSTQuote;

	/// 报价查询
	typedef struct _STQryQuote
	{
		TSessionIDType                SessionID;                 ///< 会话号(非EMPTY_ID时按会话号查询)
		TPrivateNoType                PrivateNo;                 ///< 私有号(非EMPTY_NO时按私有号查询)
		TBatchNoType                  BatchNo;                   ///< 批号(非EMPTY_NO时按批号查询)
		TCellIDType                      CellID;                       ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		TPortfolioIDType               PortfolioID;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		TMarketCodeType            MarketCode;             ///< 市场代码(为0时查询所有市场)
		TCombinInstrIDType        SecCode;                  ///< 合约代码(为空时查询所有合约代码)
	}__PACKED__ STQryQuote, *PSTQryQuote;
	
	/** @} */	
#ifdef WIN32
#pragma pack(pop)
#else
#ifdef __GNUC__
#else
#ifdef HP_UX
#pragma pack 0
#else
#pragma options align = reset
#endif
#endif
#endif
#undef __PACKED__
#ifdef __cplusplus
}
#endif

#endif //__KFTS_TRADE_STRUCT_H__

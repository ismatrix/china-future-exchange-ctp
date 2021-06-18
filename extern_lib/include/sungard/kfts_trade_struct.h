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
		int        error_no;                   ///< 错误代码
		TErrorMsgType       msg;                        ///< 错误信息
	}__PACKED__ STRspMsg, *PSTRspMsg;

	/// 交易员登录指令
	typedef struct _STTraderLogin
	{
		TCustNoType         cust_no;							 ///< 客户号
		TCustPwdType        cust_pwd;						///< 客户密码
		int   terminal_type;              ///< 终端类型
		TNoteType           note;									///< 备注
		TMacType            mac_addr;							///< MAC地址，一般情况下，登录时无需填写该字段，API会自动取得并填充。
																			///< 某些情况下，API无法确定当前使用的MAC地址，此处客户端必须填写该字段。
	} __PACKED__ STTraderLogin, *PSTTraderLogin;

	/// 登录应答
	typedef struct _STTraderLoginRsp
	{
		int      session_id;						///< 会话号
		int  conn_session_id;         ///< 连接会话号，整个系统全局唯一
		TInstanceIDType     instance_id;						///< 柜台实例号(非0,char[16])
		int      max_private_no;             ///< 上次会话最大私有号
		TCustNameType       cust_name;                  ///< 客户姓名
		int           tx_date;							///< 交易日期
	} __PACKED__ STTraderLoginRsp, *PSTTraderLoginRsp;

	/// SPX交易员校验指令
	typedef struct _STTraderCheck
	{
		TCustNoType         cust_no;								 ///< 客户号
		TCustPwdType        cust_pwd;							  ///< 客户密码
		int   terminal_type;					 ///< 终端类型
		int	spx_check_type;				///< 校验类型 1 只校验TCP关系 2 只校验密码 3 皆校验,对应SPX2号功能 
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
		int      private_no;                 ///< 私有号
		int         cell_id;                    ///< 资产单元编号(交易员有多个资产单元时需要填写，否则仅需填为EMPTY_ID)
		int    portfolio_id;               ///< 投资组合编号(资产单元有多个投资组合时需要填写，否则仅需填为EMPTY_ID)
		char     market_code;                ///< 市场代码(见数据字典-市场代码)
		TInstrumentIDType        sec_code;                   ///< 合约代码
		char      offset_flag;                 ///< 开平标志类型(见数据字典-开平标志类型)
		char             bs;                         ///< 买卖标记(见数据字典-买卖类型)
		char market_order_flag;         ///< 市价委托标记(见数据字典-市价单类型)
		double          price;                      ///< 委托价格
		double       order_vol;                  ///< 委托数量
		int      order_prop;                 ///< 委托属性(预留)
		//		int          forceflag;                  ///< 强平标记(期货)
		char           doneprop;                   ///< 成交属性(期货)
		int         instructid;                 ///< 指令编号		
		//		int			order_date;					///< 交易日期(期货)
		char		OrderType;					///< 委托单类型(期货)
		char		hedge_flag;					///< 投保标记(期货)
		int			policy_no;					///< 下单策略号
		char TradeType;						///< 交易类型
	}__PACKED__ STOrder, *PSTOrder;

	/// 委托请求(期货组合单)
	typedef struct _STOrderComb
	{
		int      private_no;                 ///< 私有号
		int         cell_id;                    ///< 资产单元编号(交易员有多个资产单元时需要填写，否则仅需填为EMPTY_ID)
		int    portfolio_id;               ///< 投资组合编号(资产单元有多个投资组合时需要填写，否则仅需填为EMPTY_ID)
		char     market_code;                ///< 市场代码(见数据字典-市场代码)
		TCombinInstrIDType    sec_code;                   ///< 期货组合单合约代码
		char      offset_flag;                 ///< 开平标志类型(见数据字典-开平标志类型)
		char             bs;                         ///< 买卖标记(见数据字典-买卖类型)
		char market_order_flag;         ///< 市价委托标记(见数据字典-市价单类型)
		double          price;                      ///< 委托价格
		double       order_vol;                  ///< 委托数量
		int      order_prop;                 ///< 委托属性(预留)

		int         instructid;                 ///< 指令编号
		//		int          forceflag;                  ///< 强平标记(期货)
		char           doneprop;                   ///< 成交属性(期货)
		//		int			order_date;					///< 交易日期(期货)
		TStrategyCode		StrategyCode;				///< 组合策略代码(期货)
		char			CombinationType;			///< 组合类型(期货)
		char		OrderType;					///< 委托单类型(期货)
		int			policy_no;					///< 下单策略号
	}__PACKED__ STOrderComb, *PSTOrderComb;

	/// 撤单请求(按委托号撤单)
	/**
	* @details 下一版本将取消该结构体，由新撤单结构体替代
	* @see _STInputOrderAction
	*/
	typedef struct _STOrderCancelByOrderNo
	{
		char		market_code;				///< 市场代码
		int		order_no;					///< 交易核心委托流水号
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
		char					   TradeType;				///< 交易类型(必填)
		char            MarketCode;				///< 市场代码
		int				   OrderLocalID;				///< 交易核心委托流水号
		int                SessionID;                 ///< 会话号
		int                PrivateNo;                 ///< 私有号
	}__PACKED__ STInputOrderAction, *PSTInputOrderAction;

	/// 委托信息(委托应答、委托查询应答)
	typedef struct _STOrderInfo
	{
		STOrder             order;                      ///< 委托结构
		int      session_id;                 ///< 会话号
		int        batch_no;                   ///< 批号
		int        order_no;                   ///< 系统委托流水号
		short    order_status;               ///< 委托状态(见数据字典-委托状态)
		double     frozen_amount;              ///< 冻结金额. 期货为冻结保证金
		double       frozen_vol;                 ///< 冻结数量
		double     done_amount;                ///< 成交金额
		double       done_vol;                   ///< 成交数量
		double       cancel_vol;                 ///< 已撤销数量
		int			order_date;					///< 委托日期
		int           order_time;                 ///< 委托时间
		int			cancel_time;                ///< 撤单时间
		int			cancel_submit_time;           ///< 撤单提交交易所时间
		int        error_no;                   ///< 委托错误代码
		TErrorMsgType       err_msg;                    ///< 委托错误信息
		int        cancel_error_no;            ///< 撤单错误代码
		TErrorMsgType       cancel_err_msg;             ///< 撤单错误信息
		int			order_submit_time;                  ///< 报单提交交易所时间
		double     fee;                        ///< 费用
		// 1.0.2版本新加字段
		double     dis_fee;								///< 分笔费用
		double     FrozenRoyalty;                    ///< 冻结权利金
		int	  QuoteLocalID;						//本地报价编号
	}__PACKED__ STOrderInfo, *PSTOrderInfo;

	/// 委托信息(期货组合单委托应答、期货组合单委托查询应答)
	typedef struct _STOrderCombInfo
	{
		STOrderComb         order;                      ///< 委托结构
		int      session_id;                 ///< 会话号
		int        batch_no;                   ///< 批号
		int        order_no;                   ///< 系统委托流水号
		TOrderStatusType    order_status;               ///< 委托状态(见数据字典-委托状态)
		double     frozen_amount;              ///< 冻结金额. 期货为冻结保证金
		double       frozen_vol;                 ///< 冻结数量
		double     done_amount;                ///< 成交金额
		double       done_vol;                   ///< 成交数量
		double       cancel_vol;                 ///< 已撤销数量
		int			order_date;					///< 委托日期
		int           order_time;                 ///< 委托时间
		int			cancel_time;                ///< 撤单时间
		int			cancel_submit_time;           ///< 撤单报入交易所时间
		int        error_no;                   ///< 委托错误代码
		TErrorMsgType       err_msg;                    ///< 委托错误信息
		int        cancel_error_no;            ///< 撤单错误代码
		TErrorMsgType       cancel_err_msg;             ///< 撤单错误信息
		int			order_submit_time;                  ///< 委托报入交易所时间
		double     fee;                        ///< 费用
		// 1.0.2版本新加字段
		double     dis_fee;                    ///< 分笔费用
	}__PACKED__ STOrderCombInfo, *PSTOrderCombInfo;

	/// 成交信息(成交返回、成交查询应答)
	typedef struct _STDoneInfo
	{
		int         done_no;                    ///< 系统成交号
		TTradeIDType         trade_id;						///< 交易所成交号
		int      session_id;                 ///< 会话号
		int      private_no;                 ///< 私有号
		int         cell_id;                    ///< 资产单元编号
		int    portfolio_id;               ///< 投资组合编号
		char     market_code;                ///< 市场代码
		TInstrumentIDType        sec_code;                   ///< 合约代码
		int        batch_no;                   ///< 批号
		int        order_no;                   ///< 系统委托流水号
		double          done_price;                 ///< 成交价格
		double       done_vol;                   ///< 成交数量
		double     done_amount;                ///< 成交金额
		double     fund_change_amount;         ///< 资金变动数量
		double       stock_vol_amount;           ///< 股份变动数量
		int           done_time;                  ///< 成交时间
		int			done_date;					///< 成交日期(期货)
		double     Margin;                ///< 保证金
		char             bs;                         ///< 买卖标记(见数据字典-买卖类型)
		char      offset_flag;                 ///< 开平标志类型(见数据字典-开平标志类型)
		char		hedge_flag;					///< 投保标记(期货)
		char      trade_type;					///<交易类型
	}__PACKED__ STDoneInfo, *PSTDoneInfo;

	/// 查询账户资金请求
	typedef struct _STQueryAccount
	{
		int         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		int    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		int    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合,-1为全部
		char   currency_type;              ///< 币种(0-所有币种，1-人民币)
		int   query_account_prop;         ///< 查询账户资金属性(0-查询当前账户资金,1-查询开盘前账户资金)
		int            acc_type;					///< 资金账户类型(0-储蓄类,1-证券A股类,2-期货类,3-个股期权)
	}__PACKED__ STQueryAccount, *PSTQueryAccount;

	/// 账户资金信息
	typedef struct _STAccountInfo
	{
		int         cell_id;                    ///< 资产单元编号
		int    portfolio_id;               ///< 投资组合编号
		int    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合
		char   currency_type;              ///< 币种

		int            acc_type;					///< 资金账户类型(0-储蓄类,1-证券A股类,2-期货类,3-个股期权)

		double     deposite;                   ///< 资金余额
		double     available;                  ///< 可取资金余额
		double     buyable;                    ///< 可用资金余额
		/// 1.0.1版本 新增字段
		double     buy_frz_amt;                ///< 买入冻结金额
		double     buy_done_amt;               ///< 买入成交金额
		double     sell_done_amt;              ///< 卖出成交金额

		/// 新增
		double     dealfrozcap;                ///< 未成交的委托冻结金额
		double     abnormal_frzn_amt;          ///< 异常冻结
		double     manual_unfrzn_amt;          ///< 手动解冻资金
		double     margin;                     ///< 保证金
		double     outcap;                     ///< 当日划出资金
		double     incap;                      ///< 当日划入资金
		double     realprofit;                 ///< 实现盈亏
		double     forbid_asset;               ///< 禁取资产
		double	 dthis_bal;					///< 昨日余额(证券期货整合新增)
		double     manual_frzn_amt;			///< 手动冻结资金(期货)
		double     RoyaltyIn;               ///< 权利金收入
		double	 RoyaltyOut;					///< 权利金支出
		double     RoyaltyFrozen;			///< 权利金冻结
	}__PACKED__ STAccountInfo, *PSTAccountInfo;

	/// 查询持仓请求
	typedef struct _STQueryPosition
	{
		int         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		int    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		int    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合,-1为全部
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		int  query_positon_prop;         ///< 查询持仓属性(0-查询当前持仓,1-查询开盘前持仓)

		char        hedgingflag;                ///< 投机套保标志(证券和期货整合新增)
		char           PosiDirection;                   ///< 持仓多空类型(0-多仓,1-空仓)(证券和期货整合新增)
		char         TradeType;						  ///<交易类型
	}__PACKED__ STQueryPosition, *PSTQueryPosition;

	/// 持仓信息
	typedef struct _STPositionInfo
	{
		int         cell_id;                    ///< 资产单元编号
		int    portfolio_id;               ///< 投资组合编号
		int    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合
		char     market_code;                ///< 市场代码
		TInstrumentIDType        sec_code;                   ///< 合约代码
		char        hedgingflag;                ///< 投机套保标志
		char           PosiDirection;                   ///< 持仓多空类型(0-多仓,1-空仓)
		double       pre_total_vol;                   ///< 昨日持仓总量
		double       pre_remain_vol;                   ///< 昨日持仓余量
		double       total_vol;                  ///< 当前总持仓
		double       avail_vol;                  ///< 当前可用余额
		double       buy_vol;                    ///< 当日买入成交数量
		double       sell_vol;                   ///< 当日卖出成交数量
		double     total_cost;                 ///< 持仓成本
		double          avg_price;                  ///< 持仓均价
		double     commission;            ///< 手续费
		double     OpenCommission;    ///< 开仓手续费
		double     CloseCommission;     ///< 平仓手续费
		double     realized_profit;            ///< 已实现盈亏
		double       sell_frz_vol;               ///< 卖出冻结数量
		double     buy_done_amt;               ///< 买入成交金额
		double     sell_done_amt;              ///< 卖出成交金额
		double       etf_frz_vol;                ///< ETF申赎冻结数量
		double       etf_rtn_vol;                ///< ETF申赎成交数量
		double       sell_frz_undone_vol;        ///< 卖出临时冻结数量
		double       etf_frz_undone_vol;         ///< ETF申赎临时冻结数量
		double       abnormal_frzn_vol;          ///< 异常冻结
		double       manual_unfrzn_vol;          ///< 可用解冻
		double       mortgage_frozen_vol;        ///< 质押入库冻结数量
		double       etf_left_vol;               ///< ETF申赎优先卖出剩余数量
		double       cur_buy_etf_left_vol;       ///< 当日买入优先申赎剩余数量
		double		curr_froz;					///< 手工冻结数量(期货)
		double		curr_unfroz;				///< 手工解冻数量(期货)
		double        detf_frozen_vol;            ///< ETF申赎成交冻结数量
		/// 期货整合新增
		//TLargeVolumeType       stkfrozamt;                 ///< 未成交的委托冻结数量，不含ETF申赎冻结
		double     usemargin;		       ///<占用保证金
		double     yd_usemargin;		  ///<昨占用保证金
		double     total_margin;		  ///<总保证金
		double     discount;		              ///<优惠数量
		double     total_profit;						///< 累计实现盈亏
		char     TradeType;							 ///<交易类型
		double locked_vol;					///<已锁定数量
		double avail_lock_vol;				///<可锁定数量
		double unlocked_vol;				///<已解锁数量
		double avail_unlock_vol;			/// <可解锁数量
		double coverd_frozen_vol;		/// <备兑冻结数量
		double MarginTradeFrozenVolume;	/// <融资回购标准券冻结数量
	}__PACKED__ STPositionInfo, *PSTPositionInfo;

	/// 查询委托请求
	typedef struct _STQueryOrder
	{
		int      session_id;                 ///< 会话号(非EMPTY_ID时按会话号查询)
		int      private_no;                 ///< 私有号(非EMPTY_NO时按私有号查询)
		int        batch_no;                   ///< 批号(非EMPTY_NO时按批号查询)
		int         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		int    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		int query_order_prop;           ///< 查询委托属性(0为查询所有,1为仅查询可撤单委托)
		char     TradeType;						  ///<交易类型
	}__PACKED__ STQueryOrder, *PSTQueryOrder;

	/// 查询委托请求(期货组合单)
	typedef struct _STQueryOrderComb
	{
		int      session_id;                 ///< 会话号(非EMPTY_ID时按会话号查询)
		int      private_no;                 ///< 私有号(非EMPTY_NO时按私有号查询)
		int        batch_no;                   ///< 批号(非EMPTY_NO时按批号查询)
		int         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		int    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TCombinInstrIDType    sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		int query_order_prop;           ///< 查询委托属性(0为查询所有,1为仅查询可撤单委托)
	}__PACKED__ STQueryOrderComb, *PSTQueryOrderCombr;


	/// 查询成交请求
	typedef struct _STQueryDone
	{
		int      session_id;                 ///< 会话号(非EMPTY_ID时按会话号查询)
		int      private_no;                 ///< 私有号(非EMPTY_NO时按私有号查询)
		int        batch_no;                   ///< 批号(非EMPTY_NO时按批号查询)
		int         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		int    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		char     TradeType;						  ///<交易类型
	}__PACKED__ STQueryDone, *PSTQueryDone;

	/// 证券基本信息查询
	typedef struct _STQuerySecCode
	{
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
	}__PACKED__ STQuerySecCode, *PSTQuerySecCode;

	/// 证券基本信息
	typedef struct _STSecCodeInfo
	{
		char     market_code;                ///< 市场代码
		TInstrumentIDType        sec_code;                   ///< 合约代码
		TInstrumentNameType        sec_name;                   ///< 合约简称
		int     sec_variety;                ///< 品种类型(见数据字典-品种类型)
		TSecStatusType      sec_status;                 ///< 证券状态
		double          raise_limit_price;          ///< 涨停价格
		double          fall_limit_price;           ///< 跌停价格
		double		equity;						///< 总股本
		double			last_price;					///< 昨收盘价格
	}__PACKED__ STSecCodeInfo, *PSTSecCodeInfo;

	/// ETF基本信息查询
	typedef struct _STQueryEtfBaseInfo
	{
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sgsh_tx_code;               ///< 申购赎回合约代码(为空时查询所有申购赎回合约代码)
	}__PACKED__ STQueryEtfBaseInfo, *PSTQueryEtfBaseInfo;

	/// ETF基本信息
	typedef struct _STEtfBaseInfo
	{
		char     market_code;                ///< 市场代码
		TInstrumentIDType        sgsh_tx_code;               ///< 基金申购赎回合约代码
		TInstrumentIDType        bs_code;                    ///< 基金买卖合约代码
		TEtfNameType        etf_name;                   ///< ETF简称
		int           sgsh_min_unit;              ///< 申购赎回最小份额
		double     estimate_cash_bal;          ///< T日预估现金余额
		double     cash_bal;                   ///< T减1日现金余额差额
		double          max_cash_ratio;             ///< 现金替代比例上限
		char    publish_iopv_flag;          ///< 发布单位净参考值标志
		double     etf_nav;                    ///< 参考基金净值
		char      etf_status;                 ///< ETF交易状态(见数据字典-ETF交易状态)
		int      record_num;                 ///< 篮子股票个数
	}__PACKED__ STEtfBaseInfo, *PSTEtfBaseInfo;

	/// ETF篮子信息查询
	typedef struct _STQueryEtfBasketInfo
	{
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sgsh_tx_code;               ///< 申购赎回合约代码(为空时查询所有申购赎回合约代码)
		char     belong_market_code;         ///< 挂牌市场(为空时查询所有挂牌市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
	}__PACKED__ STQueryEtfBasketInfo, *PSTQueryEtfBasketInfo;

	/// ETF篮子信息
	typedef struct _STEtfBasketInfo
	{
		char     market_code;                ///< 市场代码
		TInstrumentIDType        sgsh_tx_code;               ///< 申购赎回合约代码
		char     belong_market_code;         ///< 挂牌市场
		TInstrumentIDType        sec_code;                   ///< 合约代码
		TInstrumentNameType        sec_name;                   ///< 合约简称
		double       stock_vol;                  ///< 股票数量
		char   cash_repl_flag;             ///< 现金替代标志(见数据字典-现金替代标志)
		double          over_ratio;                 ///< 溢价比例
		double     cash_repl_amt;              ///< 现金替代金额
	}__PACKED__ STEtfBasketInfo, *PSTEtfBasketInfo;

	///查询证券费率信息
	typedef struct _STQueryFeeInfo
	{		
		TInstrumentIDType        sec_code;                   ///< 合约代码     , sec_code[0]=-1,表示不关心该字段
		char     market_code;                ///< 市场代码     , -1,表示不关心该字段      
		char             bs;                         ///< 买卖标记     , -1,表示不关心该字段
		char market_order_flag;         ///< 市价委托标记 , -1,表示不关心该字段
		int         cell_id;                    ///< 资产单元编号 , -1,表示不关心该字段
		int        fee_code;					///< 费用代码      , -1,表示不关心该字段
		int     sec_variety;                ///< 品种类型     , -1,表示不关心该字段
	}__PACKED__ STQueryFeeInfo, *PSTQueryFeeInfo;

	///证券费率信息
	typedef struct _STFeeInfo
	{
		TInstrumentIDType        sec_code;                   ///< 合约代码
		char     market_code;                ///< 市场代码       
		char             bs;                         ///< 买卖标记
		char market_order_flag;         ///< 市价委托标记
		int         cell_id;                    ///< 资产单元编号
		int        fee_code;					///< 费用代码
		int     sec_variety;                ///< 品种类型
		int        calc_method;				///< 计算方式
		double       fee_rate;					///< 直接费率
		double       max_fee;					///< 最高费用
		double       min_fee;					///< 最低费用
		char     dispart_count_flag;			///< 分笔计算标志
	}__PACKED__ STFeeInfo,*PSTFeeInfo;

	///转账或查询集中交易请求,查询与转账不可在一次中发起，不支持一次多种币种
	typedef struct _STFundTransREQ
	{
		int         cell_id;                    ///< 资产单元编号,不可为EMPTY_ID
		int    portfolio_id;               ///< 投资组合编号
		int    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合
		int      private_no;                 ///< 转账私有号,非EMPTY_NO，保证唯一
		char   currency_type;              ///< 币种
		TTransFundFlag      type;                       ///<转账类型 [0]代表('1' 同步转入 '2' 同步转出 '3' 查询 )，[1]代表是否先查询('0'不查询，'1'查询)
		TCustPwdType        passwd;                     ///<在集中交易中的取款密码,必须输入
		double     deposite;                   ///< 转账金额,不可为负      
	}__PACKED__ STFundTransREQ, *PSTFundTransREQ;

	///转账或查询集中交易应答
	typedef struct _STFundTransANS
	{
		int         cell_id;                    ///< 资产单元编号,不可为EMPTY_ID
		int    portfolio_id;               ///< 投资组合编号，EMPTY_ID表示仅关心资产单元
		int    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合
		int      private_no;                 ///< 转账私有号,非EMPTY_NO，保证唯一
		char   currency_type;              ///< 币种        
		TTransFundFlag      type;                       ///<转账类型 [0]代表('1' 同步转入 '2' 同步转出 '3' 查询 )，[1]代表是否先查询('0'不查询，'1'查询)

		double     deposite;                   ///< 转账金额，查询时，表示返回的集中交易中的可转金额
		double     balance;                    ///<仅对查询，表示返回的集中交易中的资金余额
		int        success;                    ///<是否操作成功  1 成功  否则 不成功
		TErrorMsgType       err_msg;                    ///错误信息
		double     dthis_bal;                  ///昨日余额
		double     useful;                     ///可用资金
		double     total;                      ///资金余额
		double     takeout;                    ///可取资金

		double     outcap;                     ///< 当日划出资金 	  	 
		double     incap;                      ///< 当日划入资金
	}__PACKED__ STFundTransANS, *PSTFundTransANS;

	///查询会话号私有号
	typedef struct _STQuerySesidPrino
	{
		int        order_no;                   ///< 系统委托流水号
	}__PACKED__ STQuerySesidPrino, *PSTQuerySesidPrino;

	///查询会话号私有号返回信息
	typedef struct _STSesidPrinoInfo
	{
		int        order_no;                   ///< 系统委托流水号
		int      session_id;                 ///< 会话号
		int      private_no;                 ///< 私有号
	}__PACKED__ STSesidPrinoInfo, *PSTSesidPrinoInfo;

	///查询股东代码
	typedef struct _STQueryHolderno
	{
		TCustNoType         cust_no;                    ///< 客户号
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		//新增
		char     interface_type;             ///< 接口类型
	}__PACKED__ STQueryHolderno, *PSTQueryHolderno;

	///查询股东代码返回信息
	typedef struct _STHoldernoInfo
	{
		TCustNoType         cust_no;                    ///< 客户号
		char     market_code;                ///< 市场代码

		//新增
		char     interface_type;             ///< 接口类型

		THoldernoType		holder_no;					///< 股东代码
	}__PACKED__ STHoldernoInfo, *PSTHoldernoInfo;

	/// 应答消息
	typedef struct _STRspMsgInvalid
	{
		int      session_id;                 ///< 会话号
		int      private_no;                 ///< 私有号
		int        error_no;                   ///< 错误代码
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
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
	}__PACKED__ STQueryFutContCode, *PSTQueryFutContCode;

	/// 期货普通合约信息
	typedef struct _STFutContractInfo
	{
		int		index;						///< 合约索引
		TInstrumentIDType		VariCode;					///< 合约代码
		char		MarketCode;					///< 市场代码
		char		VariStatus;					///< 合约状态
		int		Hands;						///< 每手数量
		double			PriceUnit;					///< 最小变动价位
		int		Maxhold;					///< 最大持仓数量
		int		MinHand;					///< 最小下单手数
		int		MaxHand;					///< 最大下单手数
		int		MarketMaxhand;				///< 市价最大手数
		double			SettlementPrice;			///< 昨结算价		
		double		UpperLimitPrice;						//<今涨停价	
		double		LowerLimitPrice;						//<今跌停价	
		char	StrikeMode;						//<执行类型
		char OptionsType;						//<期权类型
		double		StrikePrice;								//<执行价
		TInstrumentIDType	SubjectVariCode;					///< 标的物合约代码
		double		SubjectSettlementPrice;		//<标的物结算价
		double		Numberator;							//<最低保障系数分子
		double		Denominator;						//<最低保障系数分母
		char		TradeType;					 //<交易类型
		TOperDateType ExpireDate;					//<到期日
	}__PACKED__ STFutContractInfo, *PSTFutContractInfo;

	/// 查询期货组合合约信息
	typedef struct _STFutQueryContCombCode
	{
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TCombinInstrIDType    sec_code;                   ///< 合约代码(为空时查询所有合约代码)
	}__PACKED__ STQueryFutContCombCode, *PSTFutQueryContCombCode;

	/// 期货组合合约信息
	typedef struct _STFutContractCombInfo
	{
		int		index;						///< 合约索引
		TCombinInstrIDType	VariCode;					///< 合约代码
		char		MarketCode;					///< 市场代码
		TStrategyCode		StrategyCode;				///< 组合策略代码
		int			LegCount;					///< 腿数量
		TInstrumentIDType		VariCode1;					///< 合约代码1
		TInstrumentIDType		VariCode2;					///< 合约代码2
		TInstrumentIDType		VariCode3;					///< 合约代码3
		TInstrumentIDType		VariCode4;					///< 合约代码4
		char		VariStatus;					///< 合约状态
		int		Hands;						///< 每手数量
		double			PriceUnit;					///< 最小变动价位
		int		Maxhold;					///< 最大持仓数量
		int		MinHand;					///< 最小下单手数
		int		MaxHand;					///< 最大下单手数
		int		MarketMaxhand;				///< 市价最大手数
		double			SettlementPrice;			///< 昨结算价
	}__PACKED__ STFutContractCombInfo, *PSTFutContractCombInfo;

	/// 查询期货组合合约持仓请求
	typedef struct _STFutQueryPositionComb
	{
		int         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		int    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		int    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合,-1为全部
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		int  query_positon_prop;         ///< 查询持仓属性(0-查询当前持仓,1-查询开盘前持仓)
	}__PACKED__ STQueryFutPositionComb, *PSTFutQueryPositionComb;

	/// 期货组合合约持仓信息
	typedef struct  _STFutPositionCombInfo
	{
		int			CellID;						///< 单元编号
		int	PortfolioID;					///< 组合编号
		char		MarketCode;					///< 市场代码
		TCombinInstrIDType	VariCode;					///< 合约代码
		char				BS;							///< 组合买卖方向
		char		SH;							///< 组合投保方向
		int		YesterdayTotal;				///< 昨持仓量
		int		YesterdayAvailable;			///< 昨可平仓量
		double	YesterdayTotalCost;			///< 昨持仓成本
		int		Total;						///< 今总持仓量=昨+今
		int		TotalAvailable;				///< 今总可平仓量=昨+今
		double	TotalCost;					///< 今总持仓成本=昨+今
		TStrategyCode		StrategyCode;				///< 组合策略代码
		int		TradeFrozen;				///< 交易冻结数量=昨+今
		char			CombinationType;			///< 组合类型	
		int    account_type;               ///< 资产单元/投资组合标志,0-资产单元,1-投资组合
	}__PACKED__ STFutPositionCombInfo, *PSTFutPositionCombInfo;

	/// 查询期货组合合约持仓明细请求
	typedef struct _STFutQueryPositionCombDetail
	{
		int         cell_id;                    ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		int    portfolio_id;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		char     market_code;                ///< 市场代码(为0时查询所有市场)
		TInstrumentIDType        sec_code;                   ///< 合约代码(为空时查询所有合约代码)
		int  query_positon_prop;         ///< 查询持仓属性(0-查询当前持仓,1-查询开盘前持仓)
	}__PACKED__ STQueryFutPositionCombDetail, *PSTFutQueryPositionCombDetail;

	/// 期货组合合约持仓明细信息
	typedef struct  _STFutPositionCombDetail
	{
		int			CellID;						///< 单元编号
		int	PortfolioID;					///< 组合编号
		char		MarketCode;					///< 市场代码
		TInstrumentIDType		VariCode;					///< 合约代码
		char				BS;							///< 买卖方向
		char		SH;							///< 投保方向
		int			OpenDate;					///< 开仓日期
		int			DealNo;						///< 成交号
		double			HoldPrice;					///< 持仓价格
		double			OpenPrice;					///< 开仓价格
		double	HoldMargin;					///< 持仓保证金
		double	FProfitOrLoss;				///< 浮动盈亏
		int		HoldHands;					///< 持仓手数
		char			TradeType;					///<交易类型
	}__PACKED__ STFutPositionCombDetail, *PSTFutPositionCombDetail;

	/// 查询期货费率
	typedef struct _STQueryFutFeeRate
	{
		int         CellID;                    ///< 资产单元编号
		TInstrumentIDType	    InstrumentID;                   ///< 合约代码
		char		MarketCode;					///< 市场代码
		char TradeType;							///< 交易类型
	}__PACKED__ STQueryFutFeeRate, *PSTQueryFutFeeRate;

	/// 期货费率信息
	typedef struct _STFuturefee
	{
		int         CellID;                    ///< 资产单元编号
		TInstrumentIDType     InstrumentID;                   ///< 合约代码
		char		MarketCode;					///< 市场代码
		char			TradeType;							///< 交易类型
		double			BspecMarginAmt;				///< 买投机保证金按手数
		double			BspecMarginRate;			///< 买投机保证金按金额
		double			BhedgeMarginAmt;			///< 买保值保证金按手数
		double			BhedgeMarginRate;			///< 买保值保证金按金额
		double			SspecMarginAmt;				///< 卖投机保证金按手数
		double			SspecMarginRate;			///< 卖投机保证金按金额
		double			ShedgeMarginAmt;			///< 卖保值保证金按手数
		double			ShedgeMarginRate;			///< 卖保值保证金按金额
		double			BarbitrageMarginAmt;		///< 买套利保证金按手数
		double			BarbitrageMarginRate;		///< 买套利保证金按金额
		double			SarbitrageMarginAmt;		///< 卖套利保证金按手数
		double			SarbitrageMarginRate;		///< 卖套利保证金按金额
		double			EOcommiAmt;					///< 开平手续费按手数
		double			EOcommiRate;				///< 开平手续费按金额
		double			OTcommiAmt;					///< 平今手续费按手数
		double			OTcommiRate;				///< 平今手续费按金额
		double			hedgeEOcommiAmt;			///< 保值开平手续费按手数
		double			hedgeEOcommiRate;			///< 保值开平手续费按金额
		double			hedgeOTcommiAmt;			///< 保值平今手续费按手数
		double			hedgeOTcommiRate;			///< 保值平今手续费按金额
		double			arbitrageEOcommiAmt;		///< 套利开平手续费按手数
		double			arbitrageEOcommiRate;		///< 套利开平手续费按金额
		double			arbitrageOTcommiAmt;		///< 套利平今手续费按手数
		double			arbitrageOTcommiRate;		///< 套利平今手续费按金额
	}__PACKED__ STFuturefee,*PSTFuturefee;

	/// 查询期货组合合约策略信息
	typedef struct _STQueryFutCombStrategyInfo
	{
		char		MarketCode;					///< 市场代码
		TStrategyCode		StrategyCode;				///< 组合策略代码
	}__PACKED__ STQueryFutCombStrategyInfo, *PSTQueryFutCombStrategyInfo;

	/// 期货组合合约策略信息
	typedef struct  _STFutCombStrategyInfo
	{
		char		MarketCode;					///< 市场代码
		TStrategyCode		StrategyCode;				///< 组合策略代码
		TStrategyName		StrategyName;				///< 组合策略名称
		char			CombinationType;			///< 组合类型
		char				CombinationBS;				///< 组合买卖
		char		CombinationEO;				///< 组合开平
		int			LegCount;					///< 腿数量
		char				BS1;						///< 买卖方向(腿1)
		char		EO1;						///< 开平方向(腿1)
		int			QtyScale1;					///< 数量比例(腿1)
		int			CalSymbol1;					///< 计算符号(腿1)
		double			MarginScale1;				///< 保证金比例(腿1)
		char				BS2;						///< 买卖方向(腿2)
		char		EO2;						///< 开平方向(腿2)
		int			QtyScale2;					///< 数量比例(腿2)
		int			CalSymbol2;					///< 计算符号(腿2)
		double			MarginScale2;				///< 保证金比例(腿2)
		char				BS3;						///< 买卖方向(腿3)
		char		EO3;						///< 开平方向(腿3)
		int			QtyScale3;					///< 数量比例(腿3)
		int			CalSymbol3;					///< 计算符号(腿3)
		double			MarginScale3;				///< 保证金比例(腿3)
		char				BS4;						///< 买卖方向(腿4)
		char		EO4;						///< 开平方向(腿4)
		int			QtyScale4;					///< 数量比例((腿4)
		int			CalSymbol4;					///< 计算符号((腿4)
		double			MarginScale4;				///< 保证金比例((腿4)
	}__PACKED__ STFutCombStrategyInfo, *PSTFutCombStrategyInfo;

	/// 询价请求
	typedef struct  _STForQuoteInsert
	{
		int         cell_id;                            /// <资产单元编号
		char		MarketCode;			    ///< 市场代码
		TInstrumentIDType     sec_code;                     ///< 合约代码
		int      private_no;                 ///< 私有号
		TOperTimeType		InsertTime;					//询价录入时间
	}__PACKED__ STForQuoteInsert, *PSTForQuoteInsert;

	/// 询价应答
	typedef struct  _STRspForQuoteInsert
	{
		int         cell_id;                            /// <资产单元编号
		int      private_no;                 ///< 私有号
	}__PACKED__ STRspForQuoteInsert, *PSTRspForQuoteInsert;

	/// 标的券锁定解锁请求
	typedef struct  _STReqUnderlyingStockAction
	{
		int      private_no;                  ///< 私有号
		int         cell_id;                            ///< 资产单元编号
		int    portfolio_id;                     ///< 投资组合编号
		char		market_code;		    ///< 市场代码
		TInstrumentIDType     sec_code;                     ///< 合约代码
		int			policy_no;					        ///< 策略编号
		double  stock_vol;						///<数量
		char		lock_side;							///<锁定解锁类型
	}__PACKED__ STReqUnderlyingStockAction, *PSTReqUnderlyingStockAction;

	/// 标的券锁定解锁信息
	typedef struct  _STUnderlyingStockAction
	{
		int		 session_id;				     ///<会话号
		int      private_no;                  ///< 私有号
		int         cell_id;                            ///< 资产单元编号
		int    portfolio_id;                     ///< 投资组合编号
		int			policy_no;					        ///< 策略编号
		char		market_code;		    ///< 市场代码
		TInstrumentIDType     sec_code;                     ///< 合约代码
		char		lock_side;							///<锁定解锁类型
		double  stock_vol;						///<数量
		int        order_no;                   ///< 系统委托流水号
		int        batch_no;                   ///< 批号
		TOrderStatusType    order_status;            ///< 委托状态
		int        error_no;                     ///< 错误代码
		TErrorMsgType       error_msg;                 ///< 错误信息
		double       frozen_vol;                 ///< 冻结数量
		double       done_vol;                   ///< 成交总数量
		double		buy_call_vol;				   ///<可用于卖出和申赎的成交数量
		double		notbuy_call_vol;		   ///<不可用于卖出，可申赎的成交数量
		double		buy_notcall_vol;		  ///<可用于卖出，不可申赎的成交数量
		int           order_time;                 ///< 委托时间
		int			cancel_time;                ///< 撤单时间
		int			cancel_done_time;       ///< 撤单成交时间
		int			done_time;                  ///< 最后成交时间
		TContractType		RrtContract;				  ///<交易所订单合同号
		TContractType		CancelContract;		 ///<交易所订单撤单合同号
	}__PACKED__ STUnderlyingStockAction, *PSTUnderlyingStockAction;

	/// 输入的报价
	typedef struct _STInputQuote
	{
		int			PrivateNo;							///<前端私有号
		int			    CellID;								///<单元编号
		int		PortfolioID;				        ///<组合编号
		char		MarketCode;					    ///<市场代码
		TCombinInstrIDType	SecCode;							///<合约代码
		char		BidOffsetFlag;					///<买开平方向
		char				BidHedgeFlag;					///<买投保方向
		double				BidPrice;							///<买委托价格
		int			BidVolume;						///<买委托手数
		char		AskOffsetFlag;					///<卖开平
		char				AskHedgeFlag;					///<卖投保方向
		double				AskPrice;						    ///<卖委托价格
		int			AskVolume;						///<卖委托手数
		char		MarketOrderFlag;		///<定单类型
		char				DoneProp;						///<成交属性
		char			OrderType;						///<委托单类型
		int			BatchNo;							///<批号
		int					PolicyNo;						    ///<策略号
		char				TradeType;						///<交易类型
		int		OrderProp;					    ///<委托属性（保留）
		char				QuoteType;					    ///<报价类型
		int			ForQuoteSysID;				///<应价编号
	}__PACKED__ STInputQuote, *PSTInputQuote;

	/// 报价
	typedef struct _STQuote
	{
		STInputQuote	InputQuote;					///<输入的报价
		int	 SessionID;				    ///<会话号
		int	QuoteLocalID;				///<本地报价编号
		int		ActiveTime;					///<激活时间（交易核心）
		int		UpdateTime;					///<最后修改时间（交易所）
		int	BidOrderLocalID;			///<买方报单编号
		int	AskOrderLocalID;		    ///<卖方报单编号
		int	LongFrozen;					///<买持仓冻结
		int	ShortFrozen;				///<卖持仓冻结
		double	FrozenCash;					///<冻结资金
		int		ErrorID;						///<错误代码（0:核心受理成功;1：交易所受理成功; 大于1:核心处理失败;-1: 报盘、柜台、及交易所错误）
		TErrorMsgType	ErrorMsg;						///<错误信息
	}__PACKED__ STQuote, *PSTQuote;

	/// 报价查询
	typedef struct _STQryQuote
	{
		int                SessionID;                 ///< 会话号(非EMPTY_ID时按会话号查询)
		int                PrivateNo;                 ///< 私有号(非EMPTY_NO时按私有号查询)
		int                  BatchNo;                   ///< 批号(非EMPTY_NO时按批号查询)
		int                      CellID;                       ///< 资产单元编号(EMPTY_ID表示查询所有资产单元)
		int               PortfolioID;               ///< 投资组合编号(EMPTY_ID表示查询所有组合)
		char            MarketCode;             ///< 市场代码(为0时查询所有市场)
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

/////////////////////////////////////////////////////////////////////////
///@system ���˴���һ���ʹ�ϵͳ
///@company SunGard China
///@file kfts_trade_struct.h
///@brief �����˽��˴��ʹ�ϵͳ���ݽṹ
///@history
///20141001	Kingstar IRDG		�������ļ�
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

	/** \defgroup Group13 1.3 ö�ٶ��� */
	/** @{ */
	/**
	* @brief APIHandle״̬
	* @details �ͻ��˿�ֱ�ӻ�ȡAPIHandle״̬��
	*/
	enum KFTS_API_STATUS
	{
		KFTS_API_INITIAL = 0,					///<��ʼ��
		KFTS_API_CONNECTING,				///<������
		KFTS_API_CONNECTED,				///<������
		KFTS_API_LOGINING,					///<��¼��
		KFTS_API_LOGINED,						///<�ѵ�¼
		KFTS_API_RECONNECTING			///<������
	};

	/**
	* @brief ��Ϣȷ������
	* @details �ڵ����µ��򳷵�����ʱ��API֧�ֲ�ͬ����Ϣȷ�����͡�
	* ��Ϣȷ�ϵ�APIʱ����API�յ�������������ء�
	* ��Ϣȷ�ϵ�ǰ��ʱ��API�ᱣ֤ǰ���յ��������ʱ�����Ż᷵�ء�
	* ��Ϣȷ�ϵ����׷���ʱ��API�ᱣ֤���׷����յ��������ʱ�����Ż᷵�ء�
	* �Դ����ơ�
	* @see KFTSAPI_Order
	* @see KFTSAPI_OrderEx
	* @see KFTSAPI_OrderCancel
	*/
	enum KFTS_MSG_CONFIRM_TYPE
	{
		KFTS_FROM_API            = 0,   ///< ��Ϣȷ�ϵ�API(ȱʡѡ��)
		KFTS_FROM_FRONT_SERVER,         ///< ��Ϣȷ�ϵ�ǰ��
		KFTS_FROM_TRADE_SERVER,         ///< ��Ϣȷ�ϵ����׷���
		KFTS_FROM_OFFER_SERVER,         ///< ��Ϣȷ�ϵ����̷���
	};

	/**
	* @brief �ص�¼������������
	* @details ���µ�¼�ɹ�ʱ��API�Ὣ֮ǰ��ǰ���յ���������Ϣ���·���һ�顣
	* �ͻ��˿���ѡ��API�Ժ��ַ�ʽ�ص��ͻ��ˡ�
	*/
	enum KFTS_RELOGIN_TYPE
	{
		KFTS_RELOGIN_RESUME     = 0,    ///< ���ϴν������ͽ���λ�ÿ�ʼ�ص�(ȱʡѡ��)
		KFTS_RELOGIN_RESTART,           ///< ����ִ�����лص�
		KFTS_RELOGIN_QUICK,             ///< ���Ժ�������ִ�лص�
	};

	/**
	* @brief ��־����
	* @details API��¼��־�ļ�������Ϊĳһ����󣬸ü��𼰸����ؼ������־�����¼����־�ļ��С�
	*/
	enum KFTS_LOG_LEVEL
	{
		KFTS_LL_DEBUG           = 0,    ///< ���Լ���
		KFTS_LL_INFO,                   ///< ��Ϣ����(ȱʡѡ��)
		KFTS_LL_WARNING,                ///< ���漶��
		KFTS_LL_ERROR,                  ///< ���󼶱�
		KFTS_LL_FATAL,                  ///< ���ش��󼶱�
	};

	/**
	* @brief ѡ������
	* @details �ͻ��˿��Ը�����Ҫ���޸�API�е�һЩѡ�����ֵ��
	* @see KFTSAPI_SetOpt
	*/
	enum KFTS_OPTION
	{
		KFTS_OPT_SESSION_ID     = 0,    ///< ���ûỰ��,ѡ��ֵΪint����,ȱʡֵΪ0
		KFTS_OPT_RELOGIN_TYPE,          ///< �ص�¼������������,ѡ��ֵΪ\ref KFTS_RELOGIN_TYPE ���ͣ�ȱʡֵΪ\ref KFTS_RELOGIN_RESUME
		KFTS_OPT_CONFIRM_TYPE,          ///< ��Ϣȷ������,ѡ��ֵΪ\ref KFTS_MSG_CONFIRM_TYPE ���ͣ�ȱʡֵΪ\ref KFTS_FROM_API
		KFTS_OPT_DEFAULT_TIMEOUT,       ///< ί�г���Ĭ�ϳ�ʱʱ��(ms),ѡ��ֵΪint����,ȱʡֵΪ3000
		KFTS_OPT_LOG_LEVEL,             ///< ��־����,ѡ��ֵΪ\ref KFTS_LOG_LEVEL ���ͣ�ȱʡֵΪ\ref KFTS_LL_INFO
		KFTS_OPT_HEARTBEAT_INTERVAL,    ///< ��������������(s),ѡ��ֵΪint����,ȱʡֵΪ2
		KFTS_OPT_HEARTBEAT_TIMEOUT,     ///< ��������Ӧ��ʱʱ��(s),����С�ڵ�������������,ѡ��ֵΪint����,ȱʡֵΪ2
		KFTS_OPT_HEARTBEAT_MAX_LOSS,    ///< ��������������ʧ����,ѡ��ֵΪint����,ȱʡֵΪ5
		KFTS_OPT_CALLBACK_START,        ///< ������ʼ���յĻص���,�޷�������
		KFTS_OPT_STREAM_FILEPATH,       ///< ����ʹ�õ����ļ�Ŀ¼��Ҫ��ͬhandle���ܹ���
	};
	/** @} */

	/** \defgroup Group14 1.4 ���ݽṹ���� */
	/** @{ */
	/// �޳�ʱ����
#ifndef INFINITE
#define INFINITE            0xFFFFFFFF
#endif

	/// ����ί���������
#define MAX_ORDER_COUNT 300
	/// ���������������
#define MAX_ORDER_CANCEL_COUNT  300


	/// ����
#define EMPTY_ROW       -1
	/// ��ID
#define EMPTY_ID        -1
	/// ��NO
#define EMPTY_NO        -1

	/// Ӧ����Ϣ
	typedef struct _STRspMsg
	{
		TErrorIDType        error_no;                   ///< �������
		TErrorMsgType       msg;                        ///< ������Ϣ
	}__PACKED__ STRspMsg, *PSTRspMsg;

	/// ����Ա��¼ָ��
	typedef struct _STTraderLogin
	{
		TCustNoType         cust_no;							 ///< �ͻ���
		TCustPwdType        cust_pwd;						///< �ͻ�����
		TTerminalTypeType   terminal_type;              ///< �ն�����
		TNoteType           note;									///< ��ע
		TMacType            mac_addr;							///< MAC��ַ��һ������£���¼ʱ������д���ֶΣ�API���Զ�ȡ�ò���䡣
																			///< ĳЩ����£�API�޷�ȷ����ǰʹ�õ�MAC��ַ���˴��ͻ��˱�����д���ֶΡ�
		TTerminalNameType   terminal_name;              ///< �ͻ������ƺͰ汾��
		TLicenseCodeType    license_code;               ///< ��Ȩ��
		TDomainNameType     domain_id ;                 ///< �ͻ�����
	} __PACKED__ STTraderLogin, *PSTTraderLogin;

	/// ��¼Ӧ��
	typedef struct _STTraderLoginRsp
	{
		TSessionIDType      session_id;						///< �Ự��
		TConnSessionIDType  conn_session_id;         ///< ���ӻỰ�ţ�����ϵͳȫ��Ψһ
		TInstanceIDType     instance_id;						///< ��̨ʵ����(��0,char[16])
		TPrivateNoType      max_private_no;             ///< �ϴλỰ���˽�к�
		TCustNameType       cust_name;                  ///< �ͻ�����
		TDateType           tx_date;							///< ��������

		TTRADER_ID_TYPE     trader_id;                  ///< ����Ա��� 
		TDateType           login_date;                 ///< ��½����yyyymmdd,��������ǰ����
		TDateType           login_time;                 ///< ��½ʱ��0-235959,��������ǰʱ�� 
		TFRONT_KEY_TYPE     front_key;                   ///< ǰ�û���� 
		TSystemNameType     system_name ;               ///< �ʹ�ϵͳ����

	} __PACKED__ STTraderLoginRsp, *PSTTraderLoginRsp;

	/// SPX����ԱУ��ָ��
	typedef struct _STTraderCheck
	{
		TCustNoType         cust_no;								 ///< �ͻ���
		TCustPwdType        cust_pwd;							  ///< �ͻ�����
		TTerminalTypeType   terminal_type;					 ///< �ն�����
		TSPXCheckUserType	spx_check_type;				///< У������ 1 ֻУ��TCP��ϵ 2 ֻУ������ 3 ��У��,��ӦSPX2�Ź��� 
																				///<  4 ��У��,��ӦSPX61�Ź���
	} __PACKED__ STTraderCheck, *PSTTraderCheck;	


	/// Api��Ϣ
	typedef struct _STApiInfo
	{
		TApiVersionType     api_version;                ///< Api�汾��
	} __PACKED__ STApiInfo, *PSTApiInfo;

	/// ί������
	typedef struct _STOrder
	{
		TPrivateNoType      private_no;                 ///< ˽�к�
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���(����Ա�ж���ʲ���Ԫʱ��Ҫ��д�����������ΪEMPTY_ID)
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��(�ʲ���Ԫ�ж��Ͷ�����ʱ��Ҫ��д�����������ΪEMPTY_ID)
		TMarketCodeType     market_code;                ///< �г�����(�������ֵ�-�г�����)
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		TOffsetFlagType      offset_flag;                 ///< ��ƽ��־����(�������ֵ�-��ƽ��־����)
		TDirectionType             bs;                         ///< �������(�������ֵ�-��������)
		TMarketOrderFlagType market_order_flag;         ///< �м�ί�б��(�������ֵ�-�м۵�����)
		TPriceType          price;                      ///< ί�м۸�
		TLargeVolumeType       order_vol;                  ///< ί������
		TOrderPropType      order_prop;                 ///< ί������(Ԥ��)
		//		TForceFlag          forceflag;                  ///< ǿƽ���(�ڻ�)
		TDoneProp           doneprop;                   ///< �ɽ�����(�ڻ�)
		TInstructID         instructid;                 ///< ָ����		
		//		TTimeType			order_date;					///< ��������(�ڻ�)
		TFutOrderType		OrderType;					///< ί�е�����(�ڻ�)
		THedgeFlag		hedge_flag;					///< Ͷ�����(�ڻ�)
		TPolicyNo			policy_no;					///< �µ����Ժ�
		TTradeType TradeType;						///< ��������
	}__PACKED__ STOrder, *PSTOrder;

	/// ί������(�ڻ���ϵ�)
	typedef struct _STOrderComb
	{
		TPrivateNoType      private_no;                 ///< ˽�к�
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���(����Ա�ж���ʲ���Ԫʱ��Ҫ��д�����������ΪEMPTY_ID)
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��(�ʲ���Ԫ�ж��Ͷ�����ʱ��Ҫ��д�����������ΪEMPTY_ID)
		TMarketCodeType     market_code;                ///< �г�����(�������ֵ�-�г�����)
		TCombinInstrIDType    sec_code;                   ///< �ڻ���ϵ���Լ����
		TOffsetFlagType      offset_flag;                 ///< ��ƽ��־����(�������ֵ�-��ƽ��־����)
		TDirectionType             bs;                         ///< �������(�������ֵ�-��������)
		TMarketOrderFlagType market_order_flag;         ///< �м�ί�б��(�������ֵ�-�м۵�����)
		TPriceType          price;                      ///< ί�м۸�
		TLargeVolumeType       order_vol;                  ///< ί������
		TOrderPropType      order_prop;                 ///< ί������(Ԥ��)

		TInstructID         instructid;                 ///< ָ����
		//		TForceFlag          forceflag;                  ///< ǿƽ���(�ڻ�)
		TDoneProp           doneprop;                   ///< �ɽ�����(�ڻ�)
		//		TTimeType			order_date;					///< ��������(�ڻ�)
		TStrategyCode		StrategyCode;				///< ��ϲ��Դ���(�ڻ�)
		TCombType			CombinationType;			///< �������(�ڻ�)
		TFutOrderType		OrderType;					///< ί�е�����(�ڻ�)
		TPolicyNo			policy_no;					///< �µ����Ժ�
	}__PACKED__ STOrderComb, *PSTOrderComb;

	/// ��������(��ί�кų���)
	/**
	* @details ��һ�汾��ȡ���ýṹ�壬���³����ṹ�����
	* @see _STInputOrderAction
	*/
	typedef struct _STOrderCancelByOrderNo
	{
		TMarketCodeType		market_code;				///< �г�����
		TOrderNoType		order_no;					///< ���׺���ί����ˮ��
	}__PACKED__ STOrderCancelByOrderNo, *PSTOrderCancelByOrderNo;

	/// ���뱨������
	/**
	* @details 
	* ������ʽһ:MarketCode+OrderLocalID ���ݱ���ί�кų�����<br>
	* ������ʽ��:SessionID+PrivateNo ����˽�кų�����<br>
	* �����ó�����ʽ�����ͻ�����������ά����˽�кţ�����ͬһ�Ự�����ظ���
	*/
	typedef struct _STInputOrderAction
	{
		TTradeType					   TradeType;				///< ��������(����)
		TMarketCodeType            MarketCode;				///< �г�����
		TOrderNoType				   OrderLocalID;				///< ���׺���ί����ˮ��
		TSessionIDType                SessionID;                 ///< �Ự��
		TPrivateNoType                PrivateNo;                 ///< ˽�к�
	}__PACKED__ STInputOrderAction, *PSTInputOrderAction;

	/// ί����Ϣ(ί��Ӧ��ί�в�ѯӦ��)
	typedef struct _STOrderInfo
	{
		STOrder             order;                      ///< ί�нṹ
		TSessionIDType      session_id;                 ///< �Ự��
		TBatchNoType        batch_no;                   ///< ����
		TOrderNoType        order_no;                   ///< ϵͳί����ˮ��
		TOrderStatusType    order_status;               ///< ί��״̬(�������ֵ�-ί��״̬)
		TAmountType     frozen_amount;              ///< ������. �ڻ�Ϊ���ᱣ֤��
		TLargeVolumeType       frozen_vol;                 ///< ��������
		TAmountType     done_amount;                ///< �ɽ����
		TLargeVolumeType       done_vol;                   ///< �ɽ�����
		TLargeVolumeType       cancel_vol;                 ///< �ѳ�������
		TTimeType			order_date;					///< ί������
		TTimeType           order_time;                 ///< ί��ʱ��
		TTimeType			cancel_time;                ///< ����ʱ��
		TTimeType			cancel_submit_time;           ///< �����ύ������ʱ��
		TErrorIDType        error_no;                   ///< ί�д������
		TErrorMsgType       err_msg;                    ///< ί�д�����Ϣ
		TErrorIDType        cancel_error_no;            ///< �����������
		TErrorMsgType       cancel_err_msg;             ///< ����������Ϣ
		TTimeType			order_submit_time;                  ///< �����ύ������ʱ��
		TAmountType     fee;                        ///< ����
		// 1.0.2�汾�¼��ֶ�
		TAmountType     dis_fee;								///< �ֱʷ���
		TAmountType     FrozenRoyalty;                    ///< ����Ȩ����
		TOrderNoType	  QuoteLocalID;						//���ر��۱��
	}__PACKED__ STOrderInfo, *PSTOrderInfo;

	/// ί����Ϣ(�ڻ���ϵ�ί��Ӧ���ڻ���ϵ�ί�в�ѯӦ��)
	typedef struct _STOrderCombInfo
	{
		STOrderComb         order;                      ///< ί�нṹ
		TSessionIDType      session_id;                 ///< �Ự��
		TBatchNoType        batch_no;                   ///< ����
		TOrderNoType        order_no;                   ///< ϵͳί����ˮ��
		TOrderStatusType    order_status;               ///< ί��״̬(�������ֵ�-ί��״̬)
		TAmountType     frozen_amount;              ///< ������. �ڻ�Ϊ���ᱣ֤��
		TLargeVolumeType       frozen_vol;                 ///< ��������
		TAmountType     done_amount;                ///< �ɽ����
		TLargeVolumeType       done_vol;                   ///< �ɽ�����
		TLargeVolumeType       cancel_vol;                 ///< �ѳ�������
		TTimeType			order_date;					///< ί������
		TTimeType           order_time;                 ///< ί��ʱ��
		TTimeType			cancel_time;                ///< ����ʱ��
		TTimeType			cancel_submit_time;           ///< �������뽻����ʱ��
		TErrorIDType        error_no;                   ///< ί�д������
		TErrorMsgType       err_msg;                    ///< ί�д�����Ϣ
		TErrorIDType        cancel_error_no;            ///< �����������
		TErrorMsgType       cancel_err_msg;             ///< ����������Ϣ
		TTimeType			order_submit_time;                  ///< ί�б��뽻����ʱ��
		TAmountType     fee;                        ///< ����
		// 1.0.2�汾�¼��ֶ�
		TAmountType     dis_fee;                    ///< �ֱʷ���
	}__PACKED__ STOrderCombInfo, *PSTOrderCombInfo;

	/// �ɽ���Ϣ(�ɽ����ء��ɽ���ѯӦ��)
	typedef struct _STDoneInfo
	{
		TDoneNoType         done_no;                    ///< ϵͳ�ɽ���
		TTradeIDType         trade_id;						///< �������ɽ���
		TSessionIDType      session_id;                 ///< �Ự��
		TPrivateNoType      private_no;                 ///< ˽�к�
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��
		TMarketCodeType     market_code;                ///< �г�����
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		TBatchNoType        batch_no;                   ///< ����
		TOrderNoType        order_no;                   ///< ϵͳί����ˮ��
		TPriceType          done_price;                 ///< �ɽ��۸�
		TLargeVolumeType       done_vol;                   ///< �ɽ�����
		TAmountType     done_amount;                ///< �ɽ����
		TAmountType     fund_change_amount;         ///< �ʽ�䶯����
		TLargeVolumeType       stock_vol_amount;           ///< �ɷݱ䶯����
		TTimeType           done_time;                  ///< �ɽ�ʱ��
		TTimeType			done_date;					///< �ɽ�����(�ڻ�)
		TAmountType     Margin;                ///< ��֤��
		TDirectionType             bs;                         ///< �������(�������ֵ�-��������)
		TOffsetFlagType      offset_flag;                 ///< ��ƽ��־����(�������ֵ�-��ƽ��־����)
		THedgeFlag		hedge_flag;					///< Ͷ�����(�ڻ�)
		TTradeType      trade_type;					///<��������
	}__PACKED__ STDoneInfo, *PSTDoneInfo;

	/// ��ѯ�˻��ʽ�����
	typedef struct _STQueryAccount
	{
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		TAccountTypeType    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����,-1Ϊȫ��
		TCurrencyIDType   currency_type;              ///< ����(0-���б��֣�1-�����)
		TQueryAccountProp   query_account_prop;         ///< ��ѯ�˻��ʽ�����(0-��ѯ��ǰ�˻��ʽ�,1-��ѯ����ǰ�˻��ʽ�)
		TAccType            acc_type;					///< �ʽ��˻�����(0-������,1-֤ȯA����,2-�ڻ���,3-������Ȩ)
	}__PACKED__ STQueryAccount, *PSTQueryAccount;

	/// �˻��ʽ���Ϣ
	typedef struct _STAccountInfo
	{
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��
		TAccountTypeType    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����
		TCurrencyIDType   currency_type;              ///< ����

		TAccType            acc_type;					///< �ʽ��˻�����(0-������,1-֤ȯA����,2-�ڻ���,3-������Ȩ)

		TAmountType     deposite;                   ///< �ʽ����
		TAmountType     available;                  ///< ��ȡ�ʽ����
		TAmountType     buyable;                    ///< �����ʽ����
		/// 1.0.1�汾 �����ֶ�
		TAmountType     buy_frz_amt;                ///< ���붳����
		TAmountType     buy_done_amt;               ///< ����ɽ����
		TAmountType     sell_done_amt;              ///< �����ɽ����

		/// ����
		TAmountType     dealfrozcap;                ///< δ�ɽ���ί�ж�����
		TAmountType     abnormal_frzn_amt;          ///< �쳣����
		TAmountType     manual_unfrzn_amt;          ///< �ֶ��ⶳ�ʽ�
		TAmountType     margin;                     ///< ��֤��
		TAmountType     outcap;                     ///< ���ջ����ʽ�
		TAmountType     incap;                      ///< ���ջ����ʽ�
		TAmountType     realprofit;                 ///< ʵ��ӯ��
		TAmountType     forbid_asset;               ///< ��ȡ�ʲ�
		TAmountType	 dthis_bal;					///< �������(֤ȯ�ڻ���������)
		TAmountType     manual_frzn_amt;			///< �ֶ������ʽ�(�ڻ�)
		TAmountType     RoyaltyIn;               ///< Ȩ��������
		TAmountType	 RoyaltyOut;					///< Ȩ����֧��
		TAmountType     RoyaltyFrozen;			///< Ȩ���𶳽�
	}__PACKED__ STAccountInfo, *PSTAccountInfo;

	/// ��ѯ�ֲ�����
	typedef struct _STQueryPosition
	{
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		TAccountTypeType    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����,-1Ϊȫ��
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		TQueryPositionProp  query_positon_prop;         ///< ��ѯ�ֲ�����(0-��ѯ��ǰ�ֲ�,1-��ѯ����ǰ�ֲ�)

		THedgeFlag        hedgingflag;                ///< Ͷ���ױ���־(֤ȯ���ڻ���������)
		TPosiDirectionType           PosiDirection;                   ///< �ֲֶ������(0-���,1-�ղ�)(֤ȯ���ڻ���������)
		TTradeType         TradeType;						  ///<��������
	}__PACKED__ STQueryPosition, *PSTQueryPosition;

	/// �ֲ���Ϣ
	typedef struct _STPositionInfo
	{
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��
		TAccountTypeType    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����
		TMarketCodeType     market_code;                ///< �г�����
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		THedgeFlag        hedgingflag;                ///< Ͷ���ױ���־
		TPosiDirectionType           PosiDirection;                   ///< �ֲֶ������(0-���,1-�ղ�)
		TLargeVolumeType       pre_total_vol;                   ///< ���ճֲ�����
		TLargeVolumeType       pre_remain_vol;                   ///< ���ճֲ�����
		TLargeVolumeType       total_vol;                  ///< ��ǰ�ֲܳ�
		TLargeVolumeType       avail_vol;                  ///< ��ǰ�������
		TLargeVolumeType       buy_vol;                    ///< ��������ɽ�����
		TLargeVolumeType       sell_vol;                   ///< ���������ɽ�����
		TAmountType     total_cost;                 ///< �ֲֳɱ�
		TPriceType          avg_price;                  ///< �ֲ־���
		TAmountType     commission;            ///< ������
		TAmountType     OpenCommission;    ///< ����������
		TAmountType     CloseCommission;     ///< ƽ��������
		TAmountType     realized_profit;            ///< ��ʵ��ӯ��
		TLargeVolumeType       sell_frz_vol;               ///< ������������
		TAmountType     buy_done_amt;               ///< ����ɽ����
		TAmountType     sell_done_amt;              ///< �����ɽ����
		TLargeVolumeType       etf_frz_vol;                ///< ETF���궳������
		TLargeVolumeType       etf_rtn_vol;                ///< ETF����ɽ�����
		TLargeVolumeType       sell_frz_undone_vol;        ///< ������ʱ��������
		TLargeVolumeType       etf_frz_undone_vol;         ///< ETF������ʱ��������
		TLargeVolumeType       abnormal_frzn_vol;          ///< �쳣����
		TLargeVolumeType       manual_unfrzn_vol;          ///< ���ýⶳ
		TLargeVolumeType       mortgage_frozen_vol;        ///< ��Ѻ��ⶳ������
		TLargeVolumeType       etf_left_vol;               ///< ETF������������ʣ������
		TLargeVolumeType       cur_buy_etf_left_vol;       ///< ����������������ʣ������
		TLargeVolumeType		curr_froz;					///< �ֹ���������(�ڻ�)
		TLargeVolumeType		curr_unfroz;				///< �ֹ��ⶳ����(�ڻ�)
		TLargeVolumeType        detf_frozen_vol;            ///< ETF����ɽ���������
		/// �ڻ���������
		//TLargeVolumeType       stkfrozamt;                 ///< δ�ɽ���ί�ж�������������ETF���궳��
		TAmountType     usemargin;		       ///<ռ�ñ�֤��
		TAmountType     yd_usemargin;		  ///<��ռ�ñ�֤��
		TAmountType     total_margin;		  ///<�ܱ�֤��
		TLargeVolumeType     discount;		              ///<�Ż�����
		TAmountType     total_profit;						///< �ۼ�ʵ��ӯ��
		TTradeType     TradeType;							 ///<��������
		TLargeVolumeType locked_vol;					///<����������
		TLargeVolumeType avail_lock_vol;				///<����������
		TLargeVolumeType unlocked_vol;				///<�ѽ�������
		TLargeVolumeType avail_unlock_vol;			/// <�ɽ�������
		TLargeVolumeType coverd_frozen_vol;		/// <���Ҷ�������
		TLargeVolumeType MarginTradeFrozenVolume;	/// <���ʻع���׼ȯ��������
	}__PACKED__ STPositionInfo, *PSTPositionInfo;

	/// ��ѯί������
	typedef struct _STQueryOrder
	{
		TSessionIDType      session_id;                 ///< �Ự��(��EMPTY_IDʱ���Ự�Ų�ѯ)
		TPrivateNoType      private_no;                 ///< ˽�к�(��EMPTY_NOʱ��˽�кŲ�ѯ)
		TBatchNoType        batch_no;                   ///< ����(��EMPTY_NOʱ�����Ų�ѯ)
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		TQueryOrderPropType query_order_prop;           ///< ��ѯί������(0Ϊ��ѯ����,1Ϊ����ѯ�ɳ���ί��)
		TTradeType     TradeType;						  ///<��������
	}__PACKED__ STQueryOrder, *PSTQueryOrder;

	/// ��ѯί������(�ڻ���ϵ�)
	typedef struct _STQueryOrderComb
	{
		TSessionIDType      session_id;                 ///< �Ự��(��EMPTY_IDʱ���Ự�Ų�ѯ)
		TPrivateNoType      private_no;                 ///< ˽�к�(��EMPTY_NOʱ��˽�кŲ�ѯ)
		TBatchNoType        batch_no;                   ///< ����(��EMPTY_NOʱ�����Ų�ѯ)
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TCombinInstrIDType    sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		TQueryOrderPropType query_order_prop;           ///< ��ѯί������(0Ϊ��ѯ����,1Ϊ����ѯ�ɳ���ί��)
	}__PACKED__ STQueryOrderComb, *PSTQueryOrderCombr;


	/// ��ѯ�ɽ�����
	typedef struct _STQueryDone
	{
		TSessionIDType      session_id;                 ///< �Ự��(��EMPTY_IDʱ���Ự�Ų�ѯ)
		TPrivateNoType      private_no;                 ///< ˽�к�(��EMPTY_NOʱ��˽�кŲ�ѯ)
		TBatchNoType        batch_no;                   ///< ����(��EMPTY_NOʱ�����Ų�ѯ)
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		TTradeType     TradeType;						  ///<��������
	}__PACKED__ STQueryDone, *PSTQueryDone;

	/// ֤ȯ������Ϣ��ѯ
	typedef struct _STQuerySecCode
	{
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
	}__PACKED__ STQuerySecCode, *PSTQuerySecCode;

	/// ֤ȯ������Ϣ
	typedef struct _STSecCodeInfo
	{
		TMarketCodeType     market_code;                ///< �г�����
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		TInstrumentNameType        sec_name;                   ///< ��Լ���
		TProductTypeType     sec_variety;                ///< Ʒ������(�������ֵ�-Ʒ������)
		TSecStatusType      sec_status;                 ///< ֤ȯ״̬
		TPriceType          raise_limit_price;          ///< ��ͣ�۸�
		TPriceType          fall_limit_price;           ///< ��ͣ�۸�
		TLargeVolumeType		equity;						///< �ܹɱ�
		TPriceType			last_price;					///< �����̼۸�
	}__PACKED__ STSecCodeInfo, *PSTSecCodeInfo;

	/// ETF������Ϣ��ѯ
	typedef struct _STQueryEtfBaseInfo
	{
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sgsh_tx_code;               ///< �깺��غ�Լ����(Ϊ��ʱ��ѯ�����깺��غ�Լ����)
	}__PACKED__ STQueryEtfBaseInfo, *PSTQueryEtfBaseInfo;

	/// ETF������Ϣ
	typedef struct _STEtfBaseInfo
	{
		TMarketCodeType     market_code;                ///< �г�����
		TInstrumentIDType        sgsh_tx_code;               ///< �����깺��غ�Լ����
		TInstrumentIDType        bs_code;                    ///< ����������Լ����
		TEtfNameType        etf_name;                   ///< ETF���
		TUnitType           sgsh_min_unit;              ///< �깺�����С�ݶ�
		TAmountType     estimate_cash_bal;          ///< T��Ԥ���ֽ����
		TAmountType     cash_bal;                   ///< T��1���ֽ������
		TRatioType          max_cash_ratio;             ///< �ֽ������������
		TPublishIOPVFlag    publish_iopv_flag;          ///< ������λ���ο�ֵ��־
		TAmountType     etf_nav;                    ///< �ο�����ֵ
		TEtfStatusType      etf_status;                 ///< ETF����״̬(�������ֵ�-ETF����״̬)
		TRecordNumType      record_num;                 ///< ���ӹ�Ʊ����
	}__PACKED__ STEtfBaseInfo, *PSTEtfBaseInfo;

	/// ETF������Ϣ��ѯ
	typedef struct _STQueryEtfBasketInfo
	{
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sgsh_tx_code;               ///< �깺��غ�Լ����(Ϊ��ʱ��ѯ�����깺��غ�Լ����)
		TMarketCodeType     belong_market_code;         ///< �����г�(Ϊ��ʱ��ѯ���й����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
	}__PACKED__ STQueryEtfBasketInfo, *PSTQueryEtfBasketInfo;

	/// ETF������Ϣ
	typedef struct _STEtfBasketInfo
	{
		TMarketCodeType     market_code;                ///< �г�����
		TInstrumentIDType        sgsh_tx_code;               ///< �깺��غ�Լ����
		TMarketCodeType     belong_market_code;         ///< �����г�
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		TInstrumentNameType        sec_name;                   ///< ��Լ���
		TLargeVolumeType       stock_vol;                  ///< ��Ʊ����
		TCashReplFlagType   cash_repl_flag;             ///< �ֽ������־(�������ֵ�-�ֽ������־)
		TRatioType          over_ratio;                 ///< ��۱���
		TAmountType     cash_repl_amt;              ///< �ֽ�������
	}__PACKED__ STEtfBasketInfo, *PSTEtfBasketInfo;

	///��ѯ֤ȯ������Ϣ
	typedef struct _STQueryFeeInfo
	{		
		TInstrumentIDType        sec_code;                   ///< ��Լ����     , sec_code[0]=-1,��ʾ�����ĸ��ֶ�
		TMarketCodeType     market_code;                ///< �г�����     , -1,��ʾ�����ĸ��ֶ�      
		TDirectionType             bs;                         ///< �������     , -1,��ʾ�����ĸ��ֶ�
		TMarketOrderFlagType market_order_flag;         ///< �м�ί�б�� , -1,��ʾ�����ĸ��ֶ�
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ��� , -1,��ʾ�����ĸ��ֶ�
		TFeeCodeType        fee_code;					///< ���ô���      , -1,��ʾ�����ĸ��ֶ�
		TProductTypeType     sec_variety;                ///< Ʒ������     , -1,��ʾ�����ĸ��ֶ�
	}__PACKED__ STQueryFeeInfo, *PSTQueryFeeInfo;

	///֤ȯ������Ϣ
	typedef struct _STFeeInfo
	{
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		TMarketCodeType     market_code;                ///< �г�����       
		TDirectionType             bs;                         ///< �������
		TMarketOrderFlagType market_order_flag;         ///< �м�ί�б��
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���
		TFeeCodeType        fee_code;					///< ���ô���
		TProductTypeType     sec_variety;                ///< Ʒ������
		TFeeCalcType        calc_method;				///< ���㷽ʽ
		TFeeValueType       fee_rate;					///< ֱ�ӷ���
		TFeeValueType       max_fee;					///< ��߷���
		TFeeValueType       min_fee;					///< ��ͷ���
		TFeeDispartFlag     dispart_count_flag;			///< �ֱʼ����־
	}__PACKED__ STFeeInfo,*PSTFeeInfo;

	///ת�˻��ѯ���н�������,��ѯ��ת�˲�����һ���з��𣬲�֧��һ�ζ��ֱ���
	typedef struct _STFundTransREQ
	{
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���,����ΪEMPTY_ID
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��
		TAccountTypeType    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����
		TPrivateNoType      private_no;                 ///< ת��˽�к�,��EMPTY_NO����֤Ψһ
		TCurrencyIDType   currency_type;              ///< ����
		TTransFundFlag      type;                       ///<ת������ [0]����('1' ͬ��ת�� '2' ͬ��ת�� '3' ��ѯ )��[1]�����Ƿ��Ȳ�ѯ('0'����ѯ��'1'��ѯ)
		TCustPwdType        passwd;                     ///<�ڼ��н����е�ȡ������,��������
		TAmountType     deposite;                   ///< ת�˽��,����Ϊ��      
	}__PACKED__ STFundTransREQ, *PSTFundTransREQ;

	///ת�˻��ѯ���н���Ӧ��
	typedef struct _STFundTransANS
	{
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���,����ΪEMPTY_ID
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ�ţ�EMPTY_ID��ʾ�������ʲ���Ԫ
		TAccountTypeType    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����
		TPrivateNoType      private_no;                 ///< ת��˽�к�,��EMPTY_NO����֤Ψһ
		TCurrencyIDType   currency_type;              ///< ����        
		TTransFundFlag      type;                       ///<ת������ [0]����('1' ͬ��ת�� '2' ͬ��ת�� '3' ��ѯ )��[1]�����Ƿ��Ȳ�ѯ('0'����ѯ��'1'��ѯ)

		TAmountType     deposite;                   ///< ת�˽���ѯʱ����ʾ���صļ��н����еĿ�ת���
		TAmountType     balance;                    ///<���Բ�ѯ����ʾ���صļ��н����е��ʽ����
		TErrorIDType        success;                    ///<�Ƿ�����ɹ�  1 �ɹ�  ���� ���ɹ�
		TErrorMsgType       err_msg;                    ///������Ϣ
		TAmountType     dthis_bal;                  ///�������
		TAmountType     useful;                     ///�����ʽ�
		TAmountType     total;                      ///�ʽ����
		TAmountType     takeout;                    ///��ȡ�ʽ�

		TAmountType     outcap;                     ///< ���ջ����ʽ� 	  	 
		TAmountType     incap;                      ///< ���ջ����ʽ�
	}__PACKED__ STFundTransANS, *PSTFundTransANS;

	///��ѯ�Ự��˽�к�
	typedef struct _STQuerySesidPrino
	{
		TOrderNoType        order_no;                   ///< ϵͳί����ˮ��
	}__PACKED__ STQuerySesidPrino, *PSTQuerySesidPrino;

	///��ѯ�Ự��˽�кŷ�����Ϣ
	typedef struct _STSesidPrinoInfo
	{
		TOrderNoType        order_no;                   ///< ϵͳί����ˮ��
		TSessionIDType      session_id;                 ///< �Ự��
		TPrivateNoType      private_no;                 ///< ˽�к�
	}__PACKED__ STSesidPrinoInfo, *PSTSesidPrinoInfo;

	///��ѯ�ɶ�����
	typedef struct _STQueryHolderno
	{
		TCustNoType         cust_no;                    ///< �ͻ���
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		//����
		TMarketCodeType     interface_type;             ///< �ӿ�����
	}__PACKED__ STQueryHolderno, *PSTQueryHolderno;

	///��ѯ�ɶ����뷵����Ϣ
	typedef struct _STHoldernoInfo
	{
		TCustNoType         cust_no;                    ///< �ͻ���
		TMarketCodeType     market_code;                ///< �г�����

		//����
		TMarketCodeType     interface_type;             ///< �ӿ�����

		THoldernoType		holder_no;					///< �ɶ�����
	}__PACKED__ STHoldernoInfo, *PSTHoldernoInfo;

	/// Ӧ����Ϣ
	typedef struct _STRspMsgInvalid
	{
		TSessionIDType      session_id;                 ///< �Ự��
		TPrivateNoType      private_no;                 ///< ˽�к�
		TErrorIDType        error_no;                   ///< �������
		TErrorMsgType       msg;                        ///< ������Ϣ
	}__PACKED__ STMsgInvalidInfo, *PSTRspMsgInvalid;

	/// �����ֵ��ѯ�ṹ
	typedef struct _STQueryDictionaryInfo
	{
		int dict_entry;									///< �ֵ�����(��EMPTY_NOʱ������ֵ��ѯ)
		char sub_entry[20];								///< �ֵ�����(��EMPTY_NOʱ������ֵ��ѯ)
	}__PACKED__ STQueryDictionaryInfo,*PSTQueryDictionaryInfo;

	/// �����ֵ���Ϣ
	typedef struct _STDictionaryInfo
	{
		int dict_entry;									///< �ֵ�����ֵ
		char sub_entry[20];								///< �ֵ�����ֵ
		char data_type;									///< �ֵ�����ֵ��������,1-�ַ��� 2-����
		char item_comment[80];							///< �ֵ�����ע��
	}__PACKED__ STDictionaryInfo,*PSTDictionaryInfo;

	/// ��ѯ�ڻ���ͨ��Լ��Ϣ
	typedef struct _STQueryFutContCode
	{
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
	}__PACKED__ STQueryFutContCode, *PSTQueryFutContCode;

	/// �ڻ���ͨ��Լ��Ϣ
	typedef struct _STFutContractInfo
	{
		TContractIndex		index;						///< ��Լ����
		TInstrumentIDType		VariCode;					///< ��Լ����
		TMarketCodeType		MarketCode;					///< �г�����
		TContractStatus		VariStatus;					///< ��Լ״̬
		TVolumeType		Hands;						///< ÿ������
		TPriceType			PriceUnit;					///< ��С�䶯��λ
		TVolumeType		Maxhold;					///< ���ֲ�����
		TVolumeType		MinHand;					///< ��С�µ�����
		TVolumeType		MaxHand;					///< ����µ�����
		TVolumeType		MarketMaxhand;				///< �м��������
		TPriceType			SettlementPrice;			///< ������		
		TPriceType		UpperLimitPrice;						//<����ͣ��	
		TPriceType		LowerLimitPrice;						//<���ͣ��	
		TStrikeModeType	StrikeMode;						//<ִ������
		TOptionsTypeType OptionsType;						//<��Ȩ����
		TPriceType		StrikePrice;								//<ִ�м�
		TInstrumentIDType	SubjectVariCode;					///< ������Լ����
		TPriceType		SubjectSettlementPrice;		//<���������
		TRatioType		Numberator;							//<��ͱ���ϵ������
		TRatioType		Denominator;						//<��ͱ���ϵ����ĸ
		TTradeType		TradeType;					 //<��������
		TOperDateType ExpireDate;					//<������
	}__PACKED__ STFutContractInfo, *PSTFutContractInfo;

	/// ��ѯ�ڻ���Ϻ�Լ��Ϣ
	typedef struct _STFutQueryContCombCode
	{
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TCombinInstrIDType    sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
	}__PACKED__ STQueryFutContCombCode, *PSTFutQueryContCombCode;

	/// �ڻ���Ϻ�Լ��Ϣ
	typedef struct _STFutContractCombInfo
	{
		TContractIndex		index;						///< ��Լ����
		TCombinInstrIDType	VariCode;					///< ��Լ����
		TMarketCodeType		MarketCode;					///< �г�����
		TStrategyCode		StrategyCode;				///< ��ϲ��Դ���
		TLegsCnt			LegCount;					///< ������
		TInstrumentIDType		VariCode1;					///< ��Լ����1
		TInstrumentIDType		VariCode2;					///< ��Լ����2
		TInstrumentIDType		VariCode3;					///< ��Լ����3
		TInstrumentIDType		VariCode4;					///< ��Լ����4
		TContractStatus		VariStatus;					///< ��Լ״̬
		TVolumeType		Hands;						///< ÿ������
		TPriceType			PriceUnit;					///< ��С�䶯��λ
		TVolumeType		Maxhold;					///< ���ֲ�����
		TVolumeType		MinHand;					///< ��С�µ�����
		TVolumeType		MaxHand;					///< ����µ�����
		TVolumeType		MarketMaxhand;				///< �м��������
		TPriceType			SettlementPrice;			///< ������
	}__PACKED__ STFutContractCombInfo, *PSTFutContractCombInfo;

	/// ��ѯ�ڻ���Ϻ�Լ�ֲ�����
	typedef struct _STFutQueryPositionComb
	{
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		TAccountTypeType    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����,-1Ϊȫ��
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		TQueryPositionProp  query_positon_prop;         ///< ��ѯ�ֲ�����(0-��ѯ��ǰ�ֲ�,1-��ѯ����ǰ�ֲ�)
	}__PACKED__ STQueryFutPositionComb, *PSTFutQueryPositionComb;

	/// �ڻ���Ϻ�Լ�ֲ���Ϣ
	typedef struct  _STFutPositionCombInfo
	{
		TCellIDType			CellID;						///< ��Ԫ���
		TPortfolioIDType	PortfolioID;					///< ��ϱ��
		TMarketCodeType		MarketCode;					///< �г�����
		TCombinInstrIDType	VariCode;					///< ��Լ����
		TDirectionType				BS;							///< �����������
		THedgeFlag		SH;							///< ���Ͷ������
		TVolumeType		YesterdayTotal;				///< ��ֲ���
		TVolumeType		YesterdayAvailable;			///< ���ƽ����
		TAmountType	YesterdayTotalCost;			///< ��ֲֳɱ�
		TVolumeType		Total;						///< ���ֲܳ���=��+��
		TVolumeType		TotalAvailable;				///< ���ܿ�ƽ����=��+��
		TAmountType	TotalCost;					///< ���ֲֳܳɱ�=��+��
		TStrategyCode		StrategyCode;				///< ��ϲ��Դ���
		TVolumeType		TradeFrozen;				///< ���׶�������=��+��
		TCombType			CombinationType;			///< �������	
		TAccountTypeType    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����
	}__PACKED__ STFutPositionCombInfo, *PSTFutPositionCombInfo;

	/// ��ѯ�ڻ���Ϻ�Լ�ֲ���ϸ����
	typedef struct _STFutQueryPositionCombDetail
	{
		TCellIDType         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		TPortfolioIDType    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		TMarketCodeType     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		TQueryPositionProp  query_positon_prop;         ///< ��ѯ�ֲ�����(0-��ѯ��ǰ�ֲ�,1-��ѯ����ǰ�ֲ�)
	}__PACKED__ STQueryFutPositionCombDetail, *PSTFutQueryPositionCombDetail;

	/// �ڻ���Ϻ�Լ�ֲ���ϸ��Ϣ
	typedef struct  _STFutPositionCombDetail
	{
		TCellIDType			CellID;						///< ��Ԫ���
		TPortfolioIDType	PortfolioID;					///< ��ϱ��
		TMarketCodeType		MarketCode;					///< �г�����
		TInstrumentIDType		VariCode;					///< ��Լ����
		TDirectionType				BS;							///< ��������
		THedgeFlag		SH;							///< Ͷ������
		TDateType			OpenDate;					///< ��������
		TDoneNoType			DealNo;						///< �ɽ���
		TPriceType			HoldPrice;					///< �ֲּ۸�
		TPriceType			OpenPrice;					///< ���ּ۸�
		TAmountType	HoldMargin;					///< �ֱֲ�֤��
		TAmountType	FProfitOrLoss;				///< ����ӯ��
		TVolumeType		HoldHands;					///< �ֲ�����
		TTradeType			TradeType;					///<��������
	}__PACKED__ STFutPositionCombDetail, *PSTFutPositionCombDetail;

	/// ��ѯ�ڻ�����
	typedef struct _STQueryFutFeeRate
	{
		TCellIDType         CellID;                    ///< �ʲ���Ԫ���
		TInstrumentIDType	    InstrumentID;                   ///< ��Լ����
		TMarketCodeType		MarketCode;					///< �г�����
		TTradeType TradeType;							///< ��������
	}__PACKED__ STQueryFutFeeRate, *PSTQueryFutFeeRate;

	/// �ڻ�������Ϣ
	typedef struct _STFuturefee
	{
		TCellIDType         CellID;                    ///< �ʲ���Ԫ���
		TInstrumentIDType     InstrumentID;                   ///< ��Լ����
		TMarketCodeType		MarketCode;					///< �г�����
		TTradeType			TradeType;							///< ��������
		TRatioType			BspecMarginAmt;				///< ��Ͷ����֤������
		TRatioType			BspecMarginRate;			///< ��Ͷ����֤�𰴽��
		TRatioType			BhedgeMarginAmt;			///< ��ֵ��֤������
		TRatioType			BhedgeMarginRate;			///< ��ֵ��֤�𰴽��
		TRatioType			SspecMarginAmt;				///< ��Ͷ����֤������
		TRatioType			SspecMarginRate;			///< ��Ͷ����֤�𰴽��
		TRatioType			ShedgeMarginAmt;			///< ����ֵ��֤������
		TRatioType			ShedgeMarginRate;			///< ����ֵ��֤�𰴽��
		TRatioType			BarbitrageMarginAmt;		///< ��������֤������
		TRatioType			BarbitrageMarginRate;		///< ��������֤�𰴽��
		TRatioType			SarbitrageMarginAmt;		///< ��������֤������
		TRatioType			SarbitrageMarginRate;		///< ��������֤�𰴽��
		TRatioType			EOcommiAmt;					///< ��ƽ�����Ѱ�����
		TRatioType			EOcommiRate;				///< ��ƽ�����Ѱ����
		TRatioType			OTcommiAmt;					///< ƽ�������Ѱ�����
		TRatioType			OTcommiRate;				///< ƽ�������Ѱ����
		TRatioType			hedgeEOcommiAmt;			///< ��ֵ��ƽ�����Ѱ�����
		TRatioType			hedgeEOcommiRate;			///< ��ֵ��ƽ�����Ѱ����
		TRatioType			hedgeOTcommiAmt;			///< ��ֵƽ�������Ѱ�����
		TRatioType			hedgeOTcommiRate;			///< ��ֵƽ�������Ѱ����
		TRatioType			arbitrageEOcommiAmt;		///< ������ƽ�����Ѱ�����
		TRatioType			arbitrageEOcommiRate;		///< ������ƽ�����Ѱ����
		TRatioType			arbitrageOTcommiAmt;		///< ����ƽ�������Ѱ�����
		TRatioType			arbitrageOTcommiRate;		///< ����ƽ�������Ѱ����
	}__PACKED__ STFuturefee,*PSTFuturefee;

	/// ��ѯ�ڻ���Ϻ�Լ������Ϣ
	typedef struct _STQueryFutCombStrategyInfo
	{
		TMarketCodeType		MarketCode;					///< �г�����
		TStrategyCode		StrategyCode;				///< ��ϲ��Դ���
	}__PACKED__ STQueryFutCombStrategyInfo, *PSTQueryFutCombStrategyInfo;

	/// �ڻ���Ϻ�Լ������Ϣ
	typedef struct  _STFutCombStrategyInfo
	{
		TMarketCodeType		MarketCode;					///< �г�����
		TStrategyCode		StrategyCode;				///< ��ϲ��Դ���
		TStrategyName		StrategyName;				///< ��ϲ�������
		TCombType			CombinationType;			///< �������
		TDirectionType				CombinationBS;				///< �������
		TOffsetFlagType		CombinationEO;				///< ��Ͽ�ƽ
		TLegsCnt			LegCount;					///< ������
		TDirectionType				BS1;						///< ��������(��1)
		TOffsetFlagType		EO1;						///< ��ƽ����(��1)
		TLegRate			QtyScale1;					///< ��������(��1)
		TLegCalSign			CalSymbol1;					///< �������(��1)
		TRatioType			MarginScale1;				///< ��֤�����(��1)
		TDirectionType				BS2;						///< ��������(��2)
		TOffsetFlagType		EO2;						///< ��ƽ����(��2)
		TLegRate			QtyScale2;					///< ��������(��2)
		TLegCalSign			CalSymbol2;					///< �������(��2)
		TRatioType			MarginScale2;				///< ��֤�����(��2)
		TDirectionType				BS3;						///< ��������(��3)
		TOffsetFlagType		EO3;						///< ��ƽ����(��3)
		TLegRate			QtyScale3;					///< ��������(��3)
		TLegCalSign			CalSymbol3;					///< �������(��3)
		TRatioType			MarginScale3;				///< ��֤�����(��3)
		TDirectionType				BS4;						///< ��������(��4)
		TOffsetFlagType		EO4;						///< ��ƽ����(��4)
		TLegRate			QtyScale4;					///< ��������((��4)
		TLegCalSign			CalSymbol4;					///< �������((��4)
		TRatioType			MarginScale4;				///< ��֤�����((��4)
	}__PACKED__ STFutCombStrategyInfo, *PSTFutCombStrategyInfo;

	/// ѯ������
	typedef struct  _STForQuoteInsert
	{
		TCellIDType         cell_id;                            /// <�ʲ���Ԫ���
		TMarketCodeType		MarketCode;			    ///< �г�����
		TInstrumentIDType     sec_code;                     ///< ��Լ����
		TPrivateNoType      private_no;                 ///< ˽�к�
		TOperTimeType		InsertTime;					//ѯ��¼��ʱ��
	}__PACKED__ STForQuoteInsert, *PSTForQuoteInsert;

	/// ѯ��Ӧ��
	typedef struct  _STRspForQuoteInsert
	{
		TCellIDType         cell_id;                            /// <�ʲ���Ԫ���
		TPrivateNoType      private_no;                 ///< ˽�к�
	}__PACKED__ STRspForQuoteInsert, *PSTRspForQuoteInsert;

	/// ���ȯ������������
	typedef struct  _STReqUnderlyingStockAction
	{
		TPrivateNoType      private_no;                  ///< ˽�к�
		TCellIDType         cell_id;                            ///< �ʲ���Ԫ���
		TPortfolioIDType    portfolio_id;                     ///< Ͷ����ϱ��
		TMarketCodeType		market_code;		    ///< �г�����
		TInstrumentIDType     sec_code;                     ///< ��Լ����
		TPolicyNo			policy_no;					        ///< ���Ա��
		TLargeVolumeType  stock_vol;						///<����
		TLockType		lock_side;							///<������������
	}__PACKED__ STReqUnderlyingStockAction, *PSTReqUnderlyingStockAction;

	/// ���ȯ����������Ϣ
	typedef struct  _STUnderlyingStockAction
	{
		TSessionIDType		 session_id;				     ///<�Ự��
		TPrivateNoType      private_no;                  ///< ˽�к�
		TCellIDType         cell_id;                            ///< �ʲ���Ԫ���
		TPortfolioIDType    portfolio_id;                     ///< Ͷ����ϱ��
		TPolicyNo			policy_no;					        ///< ���Ա��
		TMarketCodeType		market_code;		    ///< �г�����
		TInstrumentIDType     sec_code;                     ///< ��Լ����
		TLockType		lock_side;							///<������������
		TLargeVolumeType  stock_vol;						///<����
		TOrderNoType        order_no;                   ///< ϵͳί����ˮ��
		TBatchNoType        batch_no;                   ///< ����
		TOrderStatusType    order_status;            ///< ί��״̬
		TErrorIDType        error_no;                     ///< �������
		TErrorMsgType       error_msg;                 ///< ������Ϣ
		TLargeVolumeType       frozen_vol;                 ///< ��������
		TLargeVolumeType       done_vol;                   ///< �ɽ�������
		TLargeVolumeType		buy_call_vol;				   ///<����������������ĳɽ�����
		TLargeVolumeType		notbuy_call_vol;		   ///<��������������������ĳɽ�����
		TLargeVolumeType		buy_notcall_vol;		  ///<��������������������ĳɽ�����
		TTimeType           order_time;                 ///< ί��ʱ��
		TTimeType			cancel_time;                ///< ����ʱ��
		TTimeType			cancel_done_time;       ///< �����ɽ�ʱ��
		TTimeType			done_time;                  ///< ���ɽ�ʱ��
		TContractType		RrtContract;				  ///<������������ͬ��
		TContractType		CancelContract;		 ///<����������������ͬ��
	}__PACKED__ STUnderlyingStockAction, *PSTUnderlyingStockAction;

	/// ����ı���
	typedef struct _STInputQuote
	{
		TPrivateNoType			PrivateNo;							///<ǰ��˽�к�
		TCellIDType			    CellID;								///<��Ԫ���
		TPortfolioIDType		PortfolioID;				        ///<��ϱ��
		TMarketCodeType		MarketCode;					    ///<�г�����
		TCombinInstrIDType	SecCode;							///<��Լ����
		TOffsetFlagType		BidOffsetFlag;					///<��ƽ����
		THedgeFlag				BidHedgeFlag;					///<��Ͷ������
		TPriceType				BidPrice;							///<��ί�м۸�
		TVolumeType			BidVolume;						///<��ί������
		TOffsetFlagType		AskOffsetFlag;					///<����ƽ
		THedgeFlag				AskHedgeFlag;					///<��Ͷ������
		TPriceType				AskPrice;						    ///<��ί�м۸�
		TVolumeType			AskVolume;						///<��ί������
		TMarketOrderFlagType		MarketOrderFlag;		///<��������
		TDoneProp				DoneProp;						///<�ɽ�����
		TFutOrderType			OrderType;						///<ί�е�����
		TBatchNoType			BatchNo;							///<����
		TPolicyNo					PolicyNo;						    ///<���Ժ�
		TTradeType				TradeType;						///<��������
		TOrderPropType		OrderProp;					    ///<ί�����ԣ�������
		TQuoteType				QuoteType;					    ///<��������
		TOrderNoType			ForQuoteSysID;				///<Ӧ�۱��
	}__PACKED__ STInputQuote, *PSTInputQuote;

	/// ����
	typedef struct _STQuote
	{
		STInputQuote	InputQuote;					///<����ı���
		TSessionIDType	 SessionID;				    ///<�Ự��
		TOrderNoType	QuoteLocalID;				///<���ر��۱��
		TTimeType		ActiveTime;					///<����ʱ�䣨���׺��ģ�
		TTimeType		UpdateTime;					///<����޸�ʱ�䣨��������
		TOrderNoType	BidOrderLocalID;			///<�򷽱������
		TOrderNoType	AskOrderLocalID;		    ///<�����������
		TVolumeType	LongFrozen;					///<��ֲֶ���
		TVolumeType	ShortFrozen;				///<���ֲֶ���
		TAmountType	FrozenCash;					///<�����ʽ�
		TErrorIDType		ErrorID;						///<������루0:��������ɹ�;1������������ɹ�; ����1:���Ĵ���ʧ��;-1: ���̡���̨��������������
		TErrorMsgType	ErrorMsg;						///<������Ϣ
	}__PACKED__ STQuote, *PSTQuote;

	/// ���۲�ѯ
	typedef struct _STQryQuote
	{
		TSessionIDType                SessionID;                 ///< �Ự��(��EMPTY_IDʱ���Ự�Ų�ѯ)
		TPrivateNoType                PrivateNo;                 ///< ˽�к�(��EMPTY_NOʱ��˽�кŲ�ѯ)
		TBatchNoType                  BatchNo;                   ///< ����(��EMPTY_NOʱ�����Ų�ѯ)
		TCellIDType                      CellID;                       ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		TPortfolioIDType               PortfolioID;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		TMarketCodeType            MarketCode;             ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TCombinInstrIDType        SecCode;                  ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
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

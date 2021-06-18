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
		int        error_no;                   ///< �������
		TErrorMsgType       msg;                        ///< ������Ϣ
	}__PACKED__ STRspMsg, *PSTRspMsg;

	/// ����Ա��¼ָ��
	typedef struct _STTraderLogin
	{
		TCustNoType         cust_no;							 ///< �ͻ���
		TCustPwdType        cust_pwd;						///< �ͻ�����
		int   terminal_type;              ///< �ն�����
		TNoteType           note;									///< ��ע
		TMacType            mac_addr;							///< MAC��ַ��һ������£���¼ʱ������д���ֶΣ�API���Զ�ȡ�ò���䡣
																			///< ĳЩ����£�API�޷�ȷ����ǰʹ�õ�MAC��ַ���˴��ͻ��˱�����д���ֶΡ�
	} __PACKED__ STTraderLogin, *PSTTraderLogin;

	/// ��¼Ӧ��
	typedef struct _STTraderLoginRsp
	{
		int      session_id;						///< �Ự��
		int  conn_session_id;         ///< ���ӻỰ�ţ�����ϵͳȫ��Ψһ
		TInstanceIDType     instance_id;						///< ��̨ʵ����(��0,char[16])
		int      max_private_no;             ///< �ϴλỰ���˽�к�
		TCustNameType       cust_name;                  ///< �ͻ�����
		int           tx_date;							///< ��������
	} __PACKED__ STTraderLoginRsp, *PSTTraderLoginRsp;

	/// SPX����ԱУ��ָ��
	typedef struct _STTraderCheck
	{
		TCustNoType         cust_no;								 ///< �ͻ���
		TCustPwdType        cust_pwd;							  ///< �ͻ�����
		int   terminal_type;					 ///< �ն�����
		int	spx_check_type;				///< У������ 1 ֻУ��TCP��ϵ 2 ֻУ������ 3 ��У��,��ӦSPX2�Ź��� 
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
		int      private_no;                 ///< ˽�к�
		int         cell_id;                    ///< �ʲ���Ԫ���(����Ա�ж���ʲ���Ԫʱ��Ҫ��д�����������ΪEMPTY_ID)
		int    portfolio_id;               ///< Ͷ����ϱ��(�ʲ���Ԫ�ж��Ͷ�����ʱ��Ҫ��д�����������ΪEMPTY_ID)
		char     market_code;                ///< �г�����(�������ֵ�-�г�����)
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		char      offset_flag;                 ///< ��ƽ��־����(�������ֵ�-��ƽ��־����)
		char             bs;                         ///< �������(�������ֵ�-��������)
		char market_order_flag;         ///< �м�ί�б��(�������ֵ�-�м۵�����)
		double          price;                      ///< ί�м۸�
		double       order_vol;                  ///< ί������
		int      order_prop;                 ///< ί������(Ԥ��)
		//		int          forceflag;                  ///< ǿƽ���(�ڻ�)
		char           doneprop;                   ///< �ɽ�����(�ڻ�)
		int         instructid;                 ///< ָ����		
		//		int			order_date;					///< ��������(�ڻ�)
		char		OrderType;					///< ί�е�����(�ڻ�)
		char		hedge_flag;					///< Ͷ�����(�ڻ�)
		int			policy_no;					///< �µ����Ժ�
		char TradeType;						///< ��������
	}__PACKED__ STOrder, *PSTOrder;

	/// ί������(�ڻ���ϵ�)
	typedef struct _STOrderComb
	{
		int      private_no;                 ///< ˽�к�
		int         cell_id;                    ///< �ʲ���Ԫ���(����Ա�ж���ʲ���Ԫʱ��Ҫ��д�����������ΪEMPTY_ID)
		int    portfolio_id;               ///< Ͷ����ϱ��(�ʲ���Ԫ�ж��Ͷ�����ʱ��Ҫ��д�����������ΪEMPTY_ID)
		char     market_code;                ///< �г�����(�������ֵ�-�г�����)
		TCombinInstrIDType    sec_code;                   ///< �ڻ���ϵ���Լ����
		char      offset_flag;                 ///< ��ƽ��־����(�������ֵ�-��ƽ��־����)
		char             bs;                         ///< �������(�������ֵ�-��������)
		char market_order_flag;         ///< �м�ί�б��(�������ֵ�-�м۵�����)
		double          price;                      ///< ί�м۸�
		double       order_vol;                  ///< ί������
		int      order_prop;                 ///< ί������(Ԥ��)

		int         instructid;                 ///< ָ����
		//		int          forceflag;                  ///< ǿƽ���(�ڻ�)
		char           doneprop;                   ///< �ɽ�����(�ڻ�)
		//		int			order_date;					///< ��������(�ڻ�)
		TStrategyCode		StrategyCode;				///< ��ϲ��Դ���(�ڻ�)
		char			CombinationType;			///< �������(�ڻ�)
		char		OrderType;					///< ί�е�����(�ڻ�)
		int			policy_no;					///< �µ����Ժ�
	}__PACKED__ STOrderComb, *PSTOrderComb;

	/// ��������(��ί�кų���)
	/**
	* @details ��һ�汾��ȡ���ýṹ�壬���³����ṹ�����
	* @see _STInputOrderAction
	*/
	typedef struct _STOrderCancelByOrderNo
	{
		char		market_code;				///< �г�����
		int		order_no;					///< ���׺���ί����ˮ��
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
		char					   TradeType;				///< ��������(����)
		char            MarketCode;				///< �г�����
		int				   OrderLocalID;				///< ���׺���ί����ˮ��
		int                SessionID;                 ///< �Ự��
		int                PrivateNo;                 ///< ˽�к�
	}__PACKED__ STInputOrderAction, *PSTInputOrderAction;

	/// ί����Ϣ(ί��Ӧ��ί�в�ѯӦ��)
	typedef struct _STOrderInfo
	{
		STOrder             order;                      ///< ί�нṹ
		int      session_id;                 ///< �Ự��
		int        batch_no;                   ///< ����
		int        order_no;                   ///< ϵͳί����ˮ��
		short    order_status;               ///< ί��״̬(�������ֵ�-ί��״̬)
		double     frozen_amount;              ///< ������. �ڻ�Ϊ���ᱣ֤��
		double       frozen_vol;                 ///< ��������
		double     done_amount;                ///< �ɽ����
		double       done_vol;                   ///< �ɽ�����
		double       cancel_vol;                 ///< �ѳ�������
		int			order_date;					///< ί������
		int           order_time;                 ///< ί��ʱ��
		int			cancel_time;                ///< ����ʱ��
		int			cancel_submit_time;           ///< �����ύ������ʱ��
		int        error_no;                   ///< ί�д������
		TErrorMsgType       err_msg;                    ///< ί�д�����Ϣ
		int        cancel_error_no;            ///< �����������
		TErrorMsgType       cancel_err_msg;             ///< ����������Ϣ
		int			order_submit_time;                  ///< �����ύ������ʱ��
		double     fee;                        ///< ����
		// 1.0.2�汾�¼��ֶ�
		double     dis_fee;								///< �ֱʷ���
		double     FrozenRoyalty;                    ///< ����Ȩ����
		int	  QuoteLocalID;						//���ر��۱��
	}__PACKED__ STOrderInfo, *PSTOrderInfo;

	/// ί����Ϣ(�ڻ���ϵ�ί��Ӧ���ڻ���ϵ�ί�в�ѯӦ��)
	typedef struct _STOrderCombInfo
	{
		STOrderComb         order;                      ///< ί�нṹ
		int      session_id;                 ///< �Ự��
		int        batch_no;                   ///< ����
		int        order_no;                   ///< ϵͳί����ˮ��
		TOrderStatusType    order_status;               ///< ί��״̬(�������ֵ�-ί��״̬)
		double     frozen_amount;              ///< ������. �ڻ�Ϊ���ᱣ֤��
		double       frozen_vol;                 ///< ��������
		double     done_amount;                ///< �ɽ����
		double       done_vol;                   ///< �ɽ�����
		double       cancel_vol;                 ///< �ѳ�������
		int			order_date;					///< ί������
		int           order_time;                 ///< ί��ʱ��
		int			cancel_time;                ///< ����ʱ��
		int			cancel_submit_time;           ///< �������뽻����ʱ��
		int        error_no;                   ///< ί�д������
		TErrorMsgType       err_msg;                    ///< ί�д�����Ϣ
		int        cancel_error_no;            ///< �����������
		TErrorMsgType       cancel_err_msg;             ///< ����������Ϣ
		int			order_submit_time;                  ///< ί�б��뽻����ʱ��
		double     fee;                        ///< ����
		// 1.0.2�汾�¼��ֶ�
		double     dis_fee;                    ///< �ֱʷ���
	}__PACKED__ STOrderCombInfo, *PSTOrderCombInfo;

	/// �ɽ���Ϣ(�ɽ����ء��ɽ���ѯӦ��)
	typedef struct _STDoneInfo
	{
		int         done_no;                    ///< ϵͳ�ɽ���
		TTradeIDType         trade_id;						///< �������ɽ���
		int      session_id;                 ///< �Ự��
		int      private_no;                 ///< ˽�к�
		int         cell_id;                    ///< �ʲ���Ԫ���
		int    portfolio_id;               ///< Ͷ����ϱ��
		char     market_code;                ///< �г�����
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		int        batch_no;                   ///< ����
		int        order_no;                   ///< ϵͳί����ˮ��
		double          done_price;                 ///< �ɽ��۸�
		double       done_vol;                   ///< �ɽ�����
		double     done_amount;                ///< �ɽ����
		double     fund_change_amount;         ///< �ʽ�䶯����
		double       stock_vol_amount;           ///< �ɷݱ䶯����
		int           done_time;                  ///< �ɽ�ʱ��
		int			done_date;					///< �ɽ�����(�ڻ�)
		double     Margin;                ///< ��֤��
		char             bs;                         ///< �������(�������ֵ�-��������)
		char      offset_flag;                 ///< ��ƽ��־����(�������ֵ�-��ƽ��־����)
		char		hedge_flag;					///< Ͷ�����(�ڻ�)
		char      trade_type;					///<��������
	}__PACKED__ STDoneInfo, *PSTDoneInfo;

	/// ��ѯ�˻��ʽ�����
	typedef struct _STQueryAccount
	{
		int         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		int    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		int    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����,-1Ϊȫ��
		char   currency_type;              ///< ����(0-���б��֣�1-�����)
		int   query_account_prop;         ///< ��ѯ�˻��ʽ�����(0-��ѯ��ǰ�˻��ʽ�,1-��ѯ����ǰ�˻��ʽ�)
		int            acc_type;					///< �ʽ��˻�����(0-������,1-֤ȯA����,2-�ڻ���,3-������Ȩ)
	}__PACKED__ STQueryAccount, *PSTQueryAccount;

	/// �˻��ʽ���Ϣ
	typedef struct _STAccountInfo
	{
		int         cell_id;                    ///< �ʲ���Ԫ���
		int    portfolio_id;               ///< Ͷ����ϱ��
		int    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����
		char   currency_type;              ///< ����

		int            acc_type;					///< �ʽ��˻�����(0-������,1-֤ȯA����,2-�ڻ���,3-������Ȩ)

		double     deposite;                   ///< �ʽ����
		double     available;                  ///< ��ȡ�ʽ����
		double     buyable;                    ///< �����ʽ����
		/// 1.0.1�汾 �����ֶ�
		double     buy_frz_amt;                ///< ���붳����
		double     buy_done_amt;               ///< ����ɽ����
		double     sell_done_amt;              ///< �����ɽ����

		/// ����
		double     dealfrozcap;                ///< δ�ɽ���ί�ж�����
		double     abnormal_frzn_amt;          ///< �쳣����
		double     manual_unfrzn_amt;          ///< �ֶ��ⶳ�ʽ�
		double     margin;                     ///< ��֤��
		double     outcap;                     ///< ���ջ����ʽ�
		double     incap;                      ///< ���ջ����ʽ�
		double     realprofit;                 ///< ʵ��ӯ��
		double     forbid_asset;               ///< ��ȡ�ʲ�
		double	 dthis_bal;					///< �������(֤ȯ�ڻ���������)
		double     manual_frzn_amt;			///< �ֶ������ʽ�(�ڻ�)
		double     RoyaltyIn;               ///< Ȩ��������
		double	 RoyaltyOut;					///< Ȩ����֧��
		double     RoyaltyFrozen;			///< Ȩ���𶳽�
	}__PACKED__ STAccountInfo, *PSTAccountInfo;

	/// ��ѯ�ֲ�����
	typedef struct _STQueryPosition
	{
		int         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		int    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		int    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����,-1Ϊȫ��
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		int  query_positon_prop;         ///< ��ѯ�ֲ�����(0-��ѯ��ǰ�ֲ�,1-��ѯ����ǰ�ֲ�)

		char        hedgingflag;                ///< Ͷ���ױ���־(֤ȯ���ڻ���������)
		char           PosiDirection;                   ///< �ֲֶ������(0-���,1-�ղ�)(֤ȯ���ڻ���������)
		char         TradeType;						  ///<��������
	}__PACKED__ STQueryPosition, *PSTQueryPosition;

	/// �ֲ���Ϣ
	typedef struct _STPositionInfo
	{
		int         cell_id;                    ///< �ʲ���Ԫ���
		int    portfolio_id;               ///< Ͷ����ϱ��
		int    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����
		char     market_code;                ///< �г�����
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		char        hedgingflag;                ///< Ͷ���ױ���־
		char           PosiDirection;                   ///< �ֲֶ������(0-���,1-�ղ�)
		double       pre_total_vol;                   ///< ���ճֲ�����
		double       pre_remain_vol;                   ///< ���ճֲ�����
		double       total_vol;                  ///< ��ǰ�ֲܳ�
		double       avail_vol;                  ///< ��ǰ�������
		double       buy_vol;                    ///< ��������ɽ�����
		double       sell_vol;                   ///< ���������ɽ�����
		double     total_cost;                 ///< �ֲֳɱ�
		double          avg_price;                  ///< �ֲ־���
		double     commission;            ///< ������
		double     OpenCommission;    ///< ����������
		double     CloseCommission;     ///< ƽ��������
		double     realized_profit;            ///< ��ʵ��ӯ��
		double       sell_frz_vol;               ///< ������������
		double     buy_done_amt;               ///< ����ɽ����
		double     sell_done_amt;              ///< �����ɽ����
		double       etf_frz_vol;                ///< ETF���궳������
		double       etf_rtn_vol;                ///< ETF����ɽ�����
		double       sell_frz_undone_vol;        ///< ������ʱ��������
		double       etf_frz_undone_vol;         ///< ETF������ʱ��������
		double       abnormal_frzn_vol;          ///< �쳣����
		double       manual_unfrzn_vol;          ///< ���ýⶳ
		double       mortgage_frozen_vol;        ///< ��Ѻ��ⶳ������
		double       etf_left_vol;               ///< ETF������������ʣ������
		double       cur_buy_etf_left_vol;       ///< ����������������ʣ������
		double		curr_froz;					///< �ֹ���������(�ڻ�)
		double		curr_unfroz;				///< �ֹ��ⶳ����(�ڻ�)
		double        detf_frozen_vol;            ///< ETF����ɽ���������
		/// �ڻ���������
		//TLargeVolumeType       stkfrozamt;                 ///< δ�ɽ���ί�ж�������������ETF���궳��
		double     usemargin;		       ///<ռ�ñ�֤��
		double     yd_usemargin;		  ///<��ռ�ñ�֤��
		double     total_margin;		  ///<�ܱ�֤��
		double     discount;		              ///<�Ż�����
		double     total_profit;						///< �ۼ�ʵ��ӯ��
		char     TradeType;							 ///<��������
		double locked_vol;					///<����������
		double avail_lock_vol;				///<����������
		double unlocked_vol;				///<�ѽ�������
		double avail_unlock_vol;			/// <�ɽ�������
		double coverd_frozen_vol;		/// <���Ҷ�������
		double MarginTradeFrozenVolume;	/// <���ʻع���׼ȯ��������
	}__PACKED__ STPositionInfo, *PSTPositionInfo;

	/// ��ѯί������
	typedef struct _STQueryOrder
	{
		int      session_id;                 ///< �Ự��(��EMPTY_IDʱ���Ự�Ų�ѯ)
		int      private_no;                 ///< ˽�к�(��EMPTY_NOʱ��˽�кŲ�ѯ)
		int        batch_no;                   ///< ����(��EMPTY_NOʱ�����Ų�ѯ)
		int         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		int    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		int query_order_prop;           ///< ��ѯί������(0Ϊ��ѯ����,1Ϊ����ѯ�ɳ���ί��)
		char     TradeType;						  ///<��������
	}__PACKED__ STQueryOrder, *PSTQueryOrder;

	/// ��ѯί������(�ڻ���ϵ�)
	typedef struct _STQueryOrderComb
	{
		int      session_id;                 ///< �Ự��(��EMPTY_IDʱ���Ự�Ų�ѯ)
		int      private_no;                 ///< ˽�к�(��EMPTY_NOʱ��˽�кŲ�ѯ)
		int        batch_no;                   ///< ����(��EMPTY_NOʱ�����Ų�ѯ)
		int         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		int    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TCombinInstrIDType    sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		int query_order_prop;           ///< ��ѯί������(0Ϊ��ѯ����,1Ϊ����ѯ�ɳ���ί��)
	}__PACKED__ STQueryOrderComb, *PSTQueryOrderCombr;


	/// ��ѯ�ɽ�����
	typedef struct _STQueryDone
	{
		int      session_id;                 ///< �Ự��(��EMPTY_IDʱ���Ự�Ų�ѯ)
		int      private_no;                 ///< ˽�к�(��EMPTY_NOʱ��˽�кŲ�ѯ)
		int        batch_no;                   ///< ����(��EMPTY_NOʱ�����Ų�ѯ)
		int         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		int    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		char     TradeType;						  ///<��������
	}__PACKED__ STQueryDone, *PSTQueryDone;

	/// ֤ȯ������Ϣ��ѯ
	typedef struct _STQuerySecCode
	{
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
	}__PACKED__ STQuerySecCode, *PSTQuerySecCode;

	/// ֤ȯ������Ϣ
	typedef struct _STSecCodeInfo
	{
		char     market_code;                ///< �г�����
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		TInstrumentNameType        sec_name;                   ///< ��Լ���
		int     sec_variety;                ///< Ʒ������(�������ֵ�-Ʒ������)
		TSecStatusType      sec_status;                 ///< ֤ȯ״̬
		double          raise_limit_price;          ///< ��ͣ�۸�
		double          fall_limit_price;           ///< ��ͣ�۸�
		double		equity;						///< �ܹɱ�
		double			last_price;					///< �����̼۸�
	}__PACKED__ STSecCodeInfo, *PSTSecCodeInfo;

	/// ETF������Ϣ��ѯ
	typedef struct _STQueryEtfBaseInfo
	{
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sgsh_tx_code;               ///< �깺��غ�Լ����(Ϊ��ʱ��ѯ�����깺��غ�Լ����)
	}__PACKED__ STQueryEtfBaseInfo, *PSTQueryEtfBaseInfo;

	/// ETF������Ϣ
	typedef struct _STEtfBaseInfo
	{
		char     market_code;                ///< �г�����
		TInstrumentIDType        sgsh_tx_code;               ///< �����깺��غ�Լ����
		TInstrumentIDType        bs_code;                    ///< ����������Լ����
		TEtfNameType        etf_name;                   ///< ETF���
		int           sgsh_min_unit;              ///< �깺�����С�ݶ�
		double     estimate_cash_bal;          ///< T��Ԥ���ֽ����
		double     cash_bal;                   ///< T��1���ֽ������
		double          max_cash_ratio;             ///< �ֽ������������
		char    publish_iopv_flag;          ///< ������λ���ο�ֵ��־
		double     etf_nav;                    ///< �ο�����ֵ
		char      etf_status;                 ///< ETF����״̬(�������ֵ�-ETF����״̬)
		int      record_num;                 ///< ���ӹ�Ʊ����
	}__PACKED__ STEtfBaseInfo, *PSTEtfBaseInfo;

	/// ETF������Ϣ��ѯ
	typedef struct _STQueryEtfBasketInfo
	{
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sgsh_tx_code;               ///< �깺��غ�Լ����(Ϊ��ʱ��ѯ�����깺��غ�Լ����)
		char     belong_market_code;         ///< �����г�(Ϊ��ʱ��ѯ���й����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
	}__PACKED__ STQueryEtfBasketInfo, *PSTQueryEtfBasketInfo;

	/// ETF������Ϣ
	typedef struct _STEtfBasketInfo
	{
		char     market_code;                ///< �г�����
		TInstrumentIDType        sgsh_tx_code;               ///< �깺��غ�Լ����
		char     belong_market_code;         ///< �����г�
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		TInstrumentNameType        sec_name;                   ///< ��Լ���
		double       stock_vol;                  ///< ��Ʊ����
		char   cash_repl_flag;             ///< �ֽ������־(�������ֵ�-�ֽ������־)
		double          over_ratio;                 ///< ��۱���
		double     cash_repl_amt;              ///< �ֽ�������
	}__PACKED__ STEtfBasketInfo, *PSTEtfBasketInfo;

	///��ѯ֤ȯ������Ϣ
	typedef struct _STQueryFeeInfo
	{		
		TInstrumentIDType        sec_code;                   ///< ��Լ����     , sec_code[0]=-1,��ʾ�����ĸ��ֶ�
		char     market_code;                ///< �г�����     , -1,��ʾ�����ĸ��ֶ�      
		char             bs;                         ///< �������     , -1,��ʾ�����ĸ��ֶ�
		char market_order_flag;         ///< �м�ί�б�� , -1,��ʾ�����ĸ��ֶ�
		int         cell_id;                    ///< �ʲ���Ԫ��� , -1,��ʾ�����ĸ��ֶ�
		int        fee_code;					///< ���ô���      , -1,��ʾ�����ĸ��ֶ�
		int     sec_variety;                ///< Ʒ������     , -1,��ʾ�����ĸ��ֶ�
	}__PACKED__ STQueryFeeInfo, *PSTQueryFeeInfo;

	///֤ȯ������Ϣ
	typedef struct _STFeeInfo
	{
		TInstrumentIDType        sec_code;                   ///< ��Լ����
		char     market_code;                ///< �г�����       
		char             bs;                         ///< �������
		char market_order_flag;         ///< �м�ί�б��
		int         cell_id;                    ///< �ʲ���Ԫ���
		int        fee_code;					///< ���ô���
		int     sec_variety;                ///< Ʒ������
		int        calc_method;				///< ���㷽ʽ
		double       fee_rate;					///< ֱ�ӷ���
		double       max_fee;					///< ��߷���
		double       min_fee;					///< ��ͷ���
		char     dispart_count_flag;			///< �ֱʼ����־
	}__PACKED__ STFeeInfo,*PSTFeeInfo;

	///ת�˻��ѯ���н�������,��ѯ��ת�˲�����һ���з��𣬲�֧��һ�ζ��ֱ���
	typedef struct _STFundTransREQ
	{
		int         cell_id;                    ///< �ʲ���Ԫ���,����ΪEMPTY_ID
		int    portfolio_id;               ///< Ͷ����ϱ��
		int    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����
		int      private_no;                 ///< ת��˽�к�,��EMPTY_NO����֤Ψһ
		char   currency_type;              ///< ����
		TTransFundFlag      type;                       ///<ת������ [0]����('1' ͬ��ת�� '2' ͬ��ת�� '3' ��ѯ )��[1]�����Ƿ��Ȳ�ѯ('0'����ѯ��'1'��ѯ)
		TCustPwdType        passwd;                     ///<�ڼ��н����е�ȡ������,��������
		double     deposite;                   ///< ת�˽��,����Ϊ��      
	}__PACKED__ STFundTransREQ, *PSTFundTransREQ;

	///ת�˻��ѯ���н���Ӧ��
	typedef struct _STFundTransANS
	{
		int         cell_id;                    ///< �ʲ���Ԫ���,����ΪEMPTY_ID
		int    portfolio_id;               ///< Ͷ����ϱ�ţ�EMPTY_ID��ʾ�������ʲ���Ԫ
		int    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����
		int      private_no;                 ///< ת��˽�к�,��EMPTY_NO����֤Ψһ
		char   currency_type;              ///< ����        
		TTransFundFlag      type;                       ///<ת������ [0]����('1' ͬ��ת�� '2' ͬ��ת�� '3' ��ѯ )��[1]�����Ƿ��Ȳ�ѯ('0'����ѯ��'1'��ѯ)

		double     deposite;                   ///< ת�˽���ѯʱ����ʾ���صļ��н����еĿ�ת���
		double     balance;                    ///<���Բ�ѯ����ʾ���صļ��н����е��ʽ����
		int        success;                    ///<�Ƿ�����ɹ�  1 �ɹ�  ���� ���ɹ�
		TErrorMsgType       err_msg;                    ///������Ϣ
		double     dthis_bal;                  ///�������
		double     useful;                     ///�����ʽ�
		double     total;                      ///�ʽ����
		double     takeout;                    ///��ȡ�ʽ�

		double     outcap;                     ///< ���ջ����ʽ� 	  	 
		double     incap;                      ///< ���ջ����ʽ�
	}__PACKED__ STFundTransANS, *PSTFundTransANS;

	///��ѯ�Ự��˽�к�
	typedef struct _STQuerySesidPrino
	{
		int        order_no;                   ///< ϵͳί����ˮ��
	}__PACKED__ STQuerySesidPrino, *PSTQuerySesidPrino;

	///��ѯ�Ự��˽�кŷ�����Ϣ
	typedef struct _STSesidPrinoInfo
	{
		int        order_no;                   ///< ϵͳί����ˮ��
		int      session_id;                 ///< �Ự��
		int      private_no;                 ///< ˽�к�
	}__PACKED__ STSesidPrinoInfo, *PSTSesidPrinoInfo;

	///��ѯ�ɶ�����
	typedef struct _STQueryHolderno
	{
		TCustNoType         cust_no;                    ///< �ͻ���
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		//����
		char     interface_type;             ///< �ӿ�����
	}__PACKED__ STQueryHolderno, *PSTQueryHolderno;

	///��ѯ�ɶ����뷵����Ϣ
	typedef struct _STHoldernoInfo
	{
		TCustNoType         cust_no;                    ///< �ͻ���
		char     market_code;                ///< �г�����

		//����
		char     interface_type;             ///< �ӿ�����

		THoldernoType		holder_no;					///< �ɶ�����
	}__PACKED__ STHoldernoInfo, *PSTHoldernoInfo;

	/// Ӧ����Ϣ
	typedef struct _STRspMsgInvalid
	{
		int      session_id;                 ///< �Ự��
		int      private_no;                 ///< ˽�к�
		int        error_no;                   ///< �������
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
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
	}__PACKED__ STQueryFutContCode, *PSTQueryFutContCode;

	/// �ڻ���ͨ��Լ��Ϣ
	typedef struct _STFutContractInfo
	{
		int		index;						///< ��Լ����
		TInstrumentIDType		VariCode;					///< ��Լ����
		char		MarketCode;					///< �г�����
		char		VariStatus;					///< ��Լ״̬
		int		Hands;						///< ÿ������
		double			PriceUnit;					///< ��С�䶯��λ
		int		Maxhold;					///< ���ֲ�����
		int		MinHand;					///< ��С�µ�����
		int		MaxHand;					///< ����µ�����
		int		MarketMaxhand;				///< �м��������
		double			SettlementPrice;			///< ������		
		double		UpperLimitPrice;						//<����ͣ��	
		double		LowerLimitPrice;						//<���ͣ��	
		char	StrikeMode;						//<ִ������
		char OptionsType;						//<��Ȩ����
		double		StrikePrice;								//<ִ�м�
		TInstrumentIDType	SubjectVariCode;					///< ������Լ����
		double		SubjectSettlementPrice;		//<���������
		double		Numberator;							//<��ͱ���ϵ������
		double		Denominator;						//<��ͱ���ϵ����ĸ
		char		TradeType;					 //<��������
		TOperDateType ExpireDate;					//<������
	}__PACKED__ STFutContractInfo, *PSTFutContractInfo;

	/// ��ѯ�ڻ���Ϻ�Լ��Ϣ
	typedef struct _STFutQueryContCombCode
	{
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TCombinInstrIDType    sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
	}__PACKED__ STQueryFutContCombCode, *PSTFutQueryContCombCode;

	/// �ڻ���Ϻ�Լ��Ϣ
	typedef struct _STFutContractCombInfo
	{
		int		index;						///< ��Լ����
		TCombinInstrIDType	VariCode;					///< ��Լ����
		char		MarketCode;					///< �г�����
		TStrategyCode		StrategyCode;				///< ��ϲ��Դ���
		int			LegCount;					///< ������
		TInstrumentIDType		VariCode1;					///< ��Լ����1
		TInstrumentIDType		VariCode2;					///< ��Լ����2
		TInstrumentIDType		VariCode3;					///< ��Լ����3
		TInstrumentIDType		VariCode4;					///< ��Լ����4
		char		VariStatus;					///< ��Լ״̬
		int		Hands;						///< ÿ������
		double			PriceUnit;					///< ��С�䶯��λ
		int		Maxhold;					///< ���ֲ�����
		int		MinHand;					///< ��С�µ�����
		int		MaxHand;					///< ����µ�����
		int		MarketMaxhand;				///< �м��������
		double			SettlementPrice;			///< ������
	}__PACKED__ STFutContractCombInfo, *PSTFutContractCombInfo;

	/// ��ѯ�ڻ���Ϻ�Լ�ֲ�����
	typedef struct _STFutQueryPositionComb
	{
		int         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		int    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		int    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����,-1Ϊȫ��
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		int  query_positon_prop;         ///< ��ѯ�ֲ�����(0-��ѯ��ǰ�ֲ�,1-��ѯ����ǰ�ֲ�)
	}__PACKED__ STQueryFutPositionComb, *PSTFutQueryPositionComb;

	/// �ڻ���Ϻ�Լ�ֲ���Ϣ
	typedef struct  _STFutPositionCombInfo
	{
		int			CellID;						///< ��Ԫ���
		int	PortfolioID;					///< ��ϱ��
		char		MarketCode;					///< �г�����
		TCombinInstrIDType	VariCode;					///< ��Լ����
		char				BS;							///< �����������
		char		SH;							///< ���Ͷ������
		int		YesterdayTotal;				///< ��ֲ���
		int		YesterdayAvailable;			///< ���ƽ����
		double	YesterdayTotalCost;			///< ��ֲֳɱ�
		int		Total;						///< ���ֲܳ���=��+��
		int		TotalAvailable;				///< ���ܿ�ƽ����=��+��
		double	TotalCost;					///< ���ֲֳܳɱ�=��+��
		TStrategyCode		StrategyCode;				///< ��ϲ��Դ���
		int		TradeFrozen;				///< ���׶�������=��+��
		char			CombinationType;			///< �������	
		int    account_type;               ///< �ʲ���Ԫ/Ͷ����ϱ�־,0-�ʲ���Ԫ,1-Ͷ�����
	}__PACKED__ STFutPositionCombInfo, *PSTFutPositionCombInfo;

	/// ��ѯ�ڻ���Ϻ�Լ�ֲ���ϸ����
	typedef struct _STFutQueryPositionCombDetail
	{
		int         cell_id;                    ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		int    portfolio_id;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		char     market_code;                ///< �г�����(Ϊ0ʱ��ѯ�����г�)
		TInstrumentIDType        sec_code;                   ///< ��Լ����(Ϊ��ʱ��ѯ���к�Լ����)
		int  query_positon_prop;         ///< ��ѯ�ֲ�����(0-��ѯ��ǰ�ֲ�,1-��ѯ����ǰ�ֲ�)
	}__PACKED__ STQueryFutPositionCombDetail, *PSTFutQueryPositionCombDetail;

	/// �ڻ���Ϻ�Լ�ֲ���ϸ��Ϣ
	typedef struct  _STFutPositionCombDetail
	{
		int			CellID;						///< ��Ԫ���
		int	PortfolioID;					///< ��ϱ��
		char		MarketCode;					///< �г�����
		TInstrumentIDType		VariCode;					///< ��Լ����
		char				BS;							///< ��������
		char		SH;							///< Ͷ������
		int			OpenDate;					///< ��������
		int			DealNo;						///< �ɽ���
		double			HoldPrice;					///< �ֲּ۸�
		double			OpenPrice;					///< ���ּ۸�
		double	HoldMargin;					///< �ֱֲ�֤��
		double	FProfitOrLoss;				///< ����ӯ��
		int		HoldHands;					///< �ֲ�����
		char			TradeType;					///<��������
	}__PACKED__ STFutPositionCombDetail, *PSTFutPositionCombDetail;

	/// ��ѯ�ڻ�����
	typedef struct _STQueryFutFeeRate
	{
		int         CellID;                    ///< �ʲ���Ԫ���
		TInstrumentIDType	    InstrumentID;                   ///< ��Լ����
		char		MarketCode;					///< �г�����
		char TradeType;							///< ��������
	}__PACKED__ STQueryFutFeeRate, *PSTQueryFutFeeRate;

	/// �ڻ�������Ϣ
	typedef struct _STFuturefee
	{
		int         CellID;                    ///< �ʲ���Ԫ���
		TInstrumentIDType     InstrumentID;                   ///< ��Լ����
		char		MarketCode;					///< �г�����
		char			TradeType;							///< ��������
		double			BspecMarginAmt;				///< ��Ͷ����֤������
		double			BspecMarginRate;			///< ��Ͷ����֤�𰴽��
		double			BhedgeMarginAmt;			///< ��ֵ��֤������
		double			BhedgeMarginRate;			///< ��ֵ��֤�𰴽��
		double			SspecMarginAmt;				///< ��Ͷ����֤������
		double			SspecMarginRate;			///< ��Ͷ����֤�𰴽��
		double			ShedgeMarginAmt;			///< ����ֵ��֤������
		double			ShedgeMarginRate;			///< ����ֵ��֤�𰴽��
		double			BarbitrageMarginAmt;		///< ��������֤������
		double			BarbitrageMarginRate;		///< ��������֤�𰴽��
		double			SarbitrageMarginAmt;		///< ��������֤������
		double			SarbitrageMarginRate;		///< ��������֤�𰴽��
		double			EOcommiAmt;					///< ��ƽ�����Ѱ�����
		double			EOcommiRate;				///< ��ƽ�����Ѱ����
		double			OTcommiAmt;					///< ƽ�������Ѱ�����
		double			OTcommiRate;				///< ƽ�������Ѱ����
		double			hedgeEOcommiAmt;			///< ��ֵ��ƽ�����Ѱ�����
		double			hedgeEOcommiRate;			///< ��ֵ��ƽ�����Ѱ����
		double			hedgeOTcommiAmt;			///< ��ֵƽ�������Ѱ�����
		double			hedgeOTcommiRate;			///< ��ֵƽ�������Ѱ����
		double			arbitrageEOcommiAmt;		///< ������ƽ�����Ѱ�����
		double			arbitrageEOcommiRate;		///< ������ƽ�����Ѱ����
		double			arbitrageOTcommiAmt;		///< ����ƽ�������Ѱ�����
		double			arbitrageOTcommiRate;		///< ����ƽ�������Ѱ����
	}__PACKED__ STFuturefee,*PSTFuturefee;

	/// ��ѯ�ڻ���Ϻ�Լ������Ϣ
	typedef struct _STQueryFutCombStrategyInfo
	{
		char		MarketCode;					///< �г�����
		TStrategyCode		StrategyCode;				///< ��ϲ��Դ���
	}__PACKED__ STQueryFutCombStrategyInfo, *PSTQueryFutCombStrategyInfo;

	/// �ڻ���Ϻ�Լ������Ϣ
	typedef struct  _STFutCombStrategyInfo
	{
		char		MarketCode;					///< �г�����
		TStrategyCode		StrategyCode;				///< ��ϲ��Դ���
		TStrategyName		StrategyName;				///< ��ϲ�������
		char			CombinationType;			///< �������
		char				CombinationBS;				///< �������
		char		CombinationEO;				///< ��Ͽ�ƽ
		int			LegCount;					///< ������
		char				BS1;						///< ��������(��1)
		char		EO1;						///< ��ƽ����(��1)
		int			QtyScale1;					///< ��������(��1)
		int			CalSymbol1;					///< �������(��1)
		double			MarginScale1;				///< ��֤�����(��1)
		char				BS2;						///< ��������(��2)
		char		EO2;						///< ��ƽ����(��2)
		int			QtyScale2;					///< ��������(��2)
		int			CalSymbol2;					///< �������(��2)
		double			MarginScale2;				///< ��֤�����(��2)
		char				BS3;						///< ��������(��3)
		char		EO3;						///< ��ƽ����(��3)
		int			QtyScale3;					///< ��������(��3)
		int			CalSymbol3;					///< �������(��3)
		double			MarginScale3;				///< ��֤�����(��3)
		char				BS4;						///< ��������(��4)
		char		EO4;						///< ��ƽ����(��4)
		int			QtyScale4;					///< ��������((��4)
		int			CalSymbol4;					///< �������((��4)
		double			MarginScale4;				///< ��֤�����((��4)
	}__PACKED__ STFutCombStrategyInfo, *PSTFutCombStrategyInfo;

	/// ѯ������
	typedef struct  _STForQuoteInsert
	{
		int         cell_id;                            /// <�ʲ���Ԫ���
		char		MarketCode;			    ///< �г�����
		TInstrumentIDType     sec_code;                     ///< ��Լ����
		int      private_no;                 ///< ˽�к�
		TOperTimeType		InsertTime;					//ѯ��¼��ʱ��
	}__PACKED__ STForQuoteInsert, *PSTForQuoteInsert;

	/// ѯ��Ӧ��
	typedef struct  _STRspForQuoteInsert
	{
		int         cell_id;                            /// <�ʲ���Ԫ���
		int      private_no;                 ///< ˽�к�
	}__PACKED__ STRspForQuoteInsert, *PSTRspForQuoteInsert;

	/// ���ȯ������������
	typedef struct  _STReqUnderlyingStockAction
	{
		int      private_no;                  ///< ˽�к�
		int         cell_id;                            ///< �ʲ���Ԫ���
		int    portfolio_id;                     ///< Ͷ����ϱ��
		char		market_code;		    ///< �г�����
		TInstrumentIDType     sec_code;                     ///< ��Լ����
		int			policy_no;					        ///< ���Ա��
		double  stock_vol;						///<����
		char		lock_side;							///<������������
	}__PACKED__ STReqUnderlyingStockAction, *PSTReqUnderlyingStockAction;

	/// ���ȯ����������Ϣ
	typedef struct  _STUnderlyingStockAction
	{
		int		 session_id;				     ///<�Ự��
		int      private_no;                  ///< ˽�к�
		int         cell_id;                            ///< �ʲ���Ԫ���
		int    portfolio_id;                     ///< Ͷ����ϱ��
		int			policy_no;					        ///< ���Ա��
		char		market_code;		    ///< �г�����
		TInstrumentIDType     sec_code;                     ///< ��Լ����
		char		lock_side;							///<������������
		double  stock_vol;						///<����
		int        order_no;                   ///< ϵͳί����ˮ��
		int        batch_no;                   ///< ����
		TOrderStatusType    order_status;            ///< ί��״̬
		int        error_no;                     ///< �������
		TErrorMsgType       error_msg;                 ///< ������Ϣ
		double       frozen_vol;                 ///< ��������
		double       done_vol;                   ///< �ɽ�������
		double		buy_call_vol;				   ///<����������������ĳɽ�����
		double		notbuy_call_vol;		   ///<��������������������ĳɽ�����
		double		buy_notcall_vol;		  ///<��������������������ĳɽ�����
		int           order_time;                 ///< ί��ʱ��
		int			cancel_time;                ///< ����ʱ��
		int			cancel_done_time;       ///< �����ɽ�ʱ��
		int			done_time;                  ///< ���ɽ�ʱ��
		TContractType		RrtContract;				  ///<������������ͬ��
		TContractType		CancelContract;		 ///<����������������ͬ��
	}__PACKED__ STUnderlyingStockAction, *PSTUnderlyingStockAction;

	/// ����ı���
	typedef struct _STInputQuote
	{
		int			PrivateNo;							///<ǰ��˽�к�
		int			    CellID;								///<��Ԫ���
		int		PortfolioID;				        ///<��ϱ��
		char		MarketCode;					    ///<�г�����
		TCombinInstrIDType	SecCode;							///<��Լ����
		char		BidOffsetFlag;					///<��ƽ����
		char				BidHedgeFlag;					///<��Ͷ������
		double				BidPrice;							///<��ί�м۸�
		int			BidVolume;						///<��ί������
		char		AskOffsetFlag;					///<����ƽ
		char				AskHedgeFlag;					///<��Ͷ������
		double				AskPrice;						    ///<��ί�м۸�
		int			AskVolume;						///<��ί������
		char		MarketOrderFlag;		///<��������
		char				DoneProp;						///<�ɽ�����
		char			OrderType;						///<ί�е�����
		int			BatchNo;							///<����
		int					PolicyNo;						    ///<���Ժ�
		char				TradeType;						///<��������
		int		OrderProp;					    ///<ί�����ԣ�������
		char				QuoteType;					    ///<��������
		int			ForQuoteSysID;				///<Ӧ�۱��
	}__PACKED__ STInputQuote, *PSTInputQuote;

	/// ����
	typedef struct _STQuote
	{
		STInputQuote	InputQuote;					///<����ı���
		int	 SessionID;				    ///<�Ự��
		int	QuoteLocalID;				///<���ر��۱��
		int		ActiveTime;					///<����ʱ�䣨���׺��ģ�
		int		UpdateTime;					///<����޸�ʱ�䣨��������
		int	BidOrderLocalID;			///<�򷽱������
		int	AskOrderLocalID;		    ///<�����������
		int	LongFrozen;					///<��ֲֶ���
		int	ShortFrozen;				///<���ֲֶ���
		double	FrozenCash;					///<�����ʽ�
		int		ErrorID;						///<������루0:��������ɹ�;1������������ɹ�; ����1:���Ĵ���ʧ��;-1: ���̡���̨��������������
		TErrorMsgType	ErrorMsg;						///<������Ϣ
	}__PACKED__ STQuote, *PSTQuote;

	/// ���۲�ѯ
	typedef struct _STQryQuote
	{
		int                SessionID;                 ///< �Ự��(��EMPTY_IDʱ���Ự�Ų�ѯ)
		int                PrivateNo;                 ///< ˽�к�(��EMPTY_NOʱ��˽�кŲ�ѯ)
		int                  BatchNo;                   ///< ����(��EMPTY_NOʱ�����Ų�ѯ)
		int                      CellID;                       ///< �ʲ���Ԫ���(EMPTY_ID��ʾ��ѯ�����ʲ���Ԫ)
		int               PortfolioID;               ///< Ͷ����ϱ��(EMPTY_ID��ʾ��ѯ�������)
		char            MarketCode;             ///< �г�����(Ϊ0ʱ��ѯ�����г�)
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

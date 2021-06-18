/////////////////////////////////////////////////////////////////////////
///@system ���˴���һ���ʹ�ϵͳ
///@company SunGard China
///@file kfts_trade_api.h
///@brief �����˽��˴��ʹ�ϵͳ���׽ӿ�
///@history
///20141001	Kingstar IRDG		�������ļ�
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

		/** \defgroup Group21 2.1 ������� */
	/** @{ */
	/// API���
	typedef void * APIHandle;

	/// ���Ӿ��
	typedef void * CONNHandle;
	/** @} */

	/** \defgroup Group22 2.2 �ص��������� */
	/** @{ */
	/**
	* @brief ��ǰ�û��������Ƿ�ɹ�֪ͨ
	* @param[out] hApiHandle API���
	* @param[out] pRspMsg ������Ϣ�����еĴ������Ϊ0ʱ��ʾ��ǰ�û����������ӳɹ�����0ʱ��ʾ��������ʧ��
	* @details �ͻ��˵���\ref KFTSAPI_StartLogin ��ǰ�ý������ӣ����ӽ��ͨ���ûص�֪ͨ�ͻ��ˡ�
	* �ͻ�����Ҫע��ûص������յ��ûص�����е�¼����
	* @see KFTSAPI_StartLogin
	*/
	typedef void (*PFN_RTN_CONNECTED)(APIHandle hApiHandle, const STRspMsg *pRspMsg);

	/**
	* @brief ��ǰ�û��������Ƿ��¼�ɹ�֪ͨ
	* @param[out] hApiHandle API���
	* @param[out] pRspUserLogin ��¼Ӧ����Ϣ
	* @param[out] pRspMsg ������Ϣ�����еĴ������Ϊ0ʱ��ʾ��¼�ɹ�����0ʱ��ʾ��¼ʧ��
	* @details �ͻ��˵���\ref KFTSAPI_Login ���е�¼����¼���ͨ���ûص�֪ͨ�ͻ��ˡ�
	* @see KFTSAPI_Login
	*/
	typedef void (*PFN_RTN_LOGINED)(APIHandle hApiHandle, const STTraderLoginRsp *pRspUserLogin, const STRspMsg *pRspMsg);

	/**
	* @brief ��ǰ�û��Ͽ�����֪ͨ
	* @param[out] hApiHandle API���
	* @param[out] hConnHandle �Ͽ������Ӿ��
	* @param[out] nFrontID ǰ�û�ID
	* @param[out] pRspMsg ������Ϣ
	* @return -1��ʾ����������0��ʾ��ϵͳѡ��ǰ���������ӣ�����ֵ����0ʱ���ո÷���ֵ��ǰ��ID��������
	* @details ����(hConnHandle = NULL)��������ǰ�û����ӶϿ�ʱ��APIͨ���ûص�֪ͨ�ͻ��ˣ�
	* �ͻ��˿�ͨ���ûص��ķ���ֵ��ȷ���Ƿ������Լ����������
	* ���û��ע��ûص��������ӶϿ�ʱ����ϵͳѡ��ǰ���������ӡ�
	*/
	typedef int (*PFN_RTN_RECONNECT)(APIHandle hApiHandle, const CONNHandle hConnHandle, int nFrontID, const STRspMsg *pRspMsg);

	/**
	* @brief ��Ϣ�������֪ͨ
	* @param[out] hApiHandle API���
	* @param[out] pRspMsg ������Ϣ�����еĴ������Ϊ0ʱ��ʾ���ĳɹ�����0ʱ��ʾ����ʧ��
	* @details ���ͻ��˵�¼�ɹ���API���Զ���ǰ�û����Ͷ��������Ը���API���ݡ�
	* ��ǰ�û����ͻ��˵�¼ǰ����ȫ�����͸�API��ǰ�û��ᷢ�Ͷ������֪ͨ��API��
	* APIͨ���ûص������������������Ϣ֪ͨ�ͻ��ˡ���ʱAPI�е������Ѿ����µ���¼ʱ�����ݡ�
	*/
	typedef void (*PFN_RTN_SUBSCRIBE)(APIHandle hApiHandle, const STRspMsg *pRspMsg);

	/**
	* @brief ����֪ͨ�ص�����
	* @param[out] hApiHandle API���
	* @param[out] pQuote ���۵���Ϣ
	* @details �����۷����䶯ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RTN_QUOTE)(APIHandle hApiHandle, const STQuote *pQuote);

	/**
	* @brief ί�лر����ͻص�����
	* @param[out] hApiHandle API���
	* @param[out] pOrder ί����Ϣ
	* @param[out] nDataSrc ����:1-��̨ί������,2-����ί������,3-��̨��������,4-���̳�������,5-�ɽ�����
	* @details ��ί�з����䶯ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RTN_ORDER)(APIHandle hApiHandle, const STOrderInfo *pOrder, int nDataSrc);  

	/**
	* @brief ���ί�лر����ͻص�����
	* @param[out] hApiHandle API���
	* @param[out] pOrderCombine ί����Ϣ
	* @param[out] nDataSrc ����:1-��̨ί������,2-����ί������,3-��̨��������,4-���̳�������,5-�ɽ�����
	* @details ��ί�з����䶯ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RTN_ORDER_COMB)(APIHandle hApiHandle, const STOrderCombInfo *pOrderCombine, int nDataSrc);

	/**
	* @brief �ɽ��ر����ͻص�����
	* @param[out] hApiHandle API���
	* @param[out] pDone �ɽ���Ϣ
	* @details �������ɽ�ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RTN_DONE)(APIHandle hApiHandle, const STDoneInfo *pDone);

	/**
	* @brief �ʽ�䶯���ͻص�����
	* @param[out] hApiHandle API���
	* @param[out] pAccount �˻��ʽ���Ϣ
	* @details ���ʽ����䶯ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RTN_ACCOUNT)(APIHandle hApiHandle, const STAccountInfo *pAccount);

	/**
	* @brief �ֱֲ䶯���ͻص�����
	* @param[out] hApiHandle API���
	* @param[out] pPosition �ֲ���Ϣ
	* @details ���ֲַ����䶯ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RTN_POSITION)(APIHandle hApiHandle, const STPositionInfo *pPosition);

	/**
	* @brief ��ϳֱֲ䶯���ͻص�����
	* @param[out] hApiHandle API���
	* @param[out] pPositionCombine �ֲ���Ϣ
	* @details ����ϳֲַ����䶯ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RTN_POSITION_COMB)(APIHandle hApiHandle, const STFutPositionCombInfo *pPositionCombine);

	/**
	* @brief ��ϳֲ���ϸ�䶯���ͻص�����
	* @param[out] hApiHandle API���
	* @param[out] pPositionCombineDetail �ֲ���Ϣ
	* @details ����ϳֲ���ϸ�����䶯ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RTN_POSITION_COMB_DET)(APIHandle hApiHandle, const STFutPositionCombDetail *pPositionCombineDetail);

	/**
	* @brief ת���������ȷ�ϻص�����
	* @param[out] hApiHandle API���
	* @param[out] pTransfer ת�˷�����Ϣ
	* @param[out] nProcessStatus, ����״̬���ͣ�1APIת��Ӧ�� 2APIת�ʳɹ����ȷ�� 3����ת��Ӧ�� 4����ת�ʳɹ����ȷ�ϡ�
	* @details ��ת��ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˣ�ת�ʵĴ�������
	*/
	typedef void (*PFN_RTN_FUNDTRANS)(APIHandle hApiHandle, const STFundTransANS *pTransfer,int nProcessStatus);

	/**
	* @brief ���ȯ���������䶯���ͻص�����
	* @param[out] hApiHandle API���
	* @param[out] p_underlyingstock_action ���ȯ����������Ϣ
	* @details �����ȯ�������������䶯ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RTN_UNDERLYINGSTOCKACTION)(APIHandle hApiHandle, const STUnderlyingStockAction *pUnderlyingStockAction);

	/**
	* @brief ͨ�����󷵻ػص�����
	* @param[out] hApiHandle API���
	* @param[out] nTID ������Ϣ����
	* @param[out] pCommonInfo ������Ϣ����
	* @param[out] nMsgLen ������Ϣ����
	* @details ���յ�ͨ����������ɵķ�������ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RTN_COMMONREQUEST)(APIHandle hApiHandle, int nTID, const char* pCommonInfo, int nMsgLen);

	/**
	* @brief SPX����Ա�����Ƿ�ɹ�֪ͨ
	* @param[out] hApiHandle API���
	* @param[out] pUserCheck ����Ӧ����Ϣ
	* @param[out] pRspMsg ������Ϣ�����еĴ������Ϊ0ʱ��ʾ����ɹ�����0ʱ��ʾ����ʧ��
	* @details �ͻ��˵���\ref KFTSAPI_Check ���м��飬������ͨ���ûص�֪ͨ�ͻ��ˡ�
	* @see KFTSAPI_Check
	*/
	typedef void (*PFN_RTN_CHECKED)(APIHandle hApiHandle, const STTraderCheck *pUserCheck, const STRspMsg *pRspMsg);

	/**
	* @brief ����Ӧ����Ϣ�ص�����
	* @param[out] hApiHandle API���
	* @param[out] pRspMsg ������Ϣ
	* @param[out] nRequestID ������
	* @details ����ѯ���������������ʱ��APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	*/
	typedef void (*PFN_RSP_ERRORMSG)(APIHandle hApiHandle, const STRspMsg *pRspMsg, int nRequestID);

	/**
	* @brief ��ѯ�ͻ��˻��ʽ�Ӧ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pAccount �˻��ʽ���Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryAccount ��ѯ�ͻ��˻��ʽ���Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_account_rspΪNULL��
	* @see KFTSAPI_RegQueryAccountCallBack
	* @see KFTSAPI_QueryAccount
	*/
	typedef void (*PFN_RSP_QUERY_ACCOUNT)(APIHandle hApiHandle, const STAccountInfo *pAccount, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ�ֲ�Ӧ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pPosition �ֲ���Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryPosition ��ѯ�ֲ���Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_position_rspΪNULL��
	* @see KFTSAPI_RegQueryAccountCallBack
	* @see KFTSAPI_QueryPosition
	*/
	typedef void (*PFN_RSP_QUERY_POSITION)(APIHandle hApiHandle, const STPositionInfo *pPosition, int nRequestID, int nRecordID);

		/**
	* @brief ��ѯ���۵��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pQuote ���۵���ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_ReqQryQuote ��ѯ������Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��pQuoteΪNULL��
	* @see KFTSAPI_RegQuoteCallBack
	* @see KFTSAPI_ReqQryQuote
	*/
	typedef void (*PFN_RSP_QUERY_QUOTE)(APIHandle hApiHandle, const STQuote* pQuote, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯί�е�Ӧ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pOrder ί����Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryOrder ��ѯί����Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_orderinfo_rspΪNULL��
	* @see KFTSAPI_RegQueryOrderCallBack
	* @see KFTSAPI_QueryOrder
	*/
	typedef void (*PFN_RSP_QUERY_ORDER)(APIHandle hApiHandle, const STOrderInfo *pOrder, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ�ڻ���ϵ�ί��Ӧ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pOrderCombine ί����Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryOrder ��ѯί����Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_orderinfo_rspΪNULL��
	* @see KFTSAPI_RegQueryOrderCombCallBack
	* @see KFTSAPI_QueryOrderComb
	*/
	typedef void (*PFN_RSP_QUERY_ORDER_COMB)(APIHandle hApiHandle, const STOrderCombInfo *pOrderCombine, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ�ɽ�Ӧ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pDone �ɽ���Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryDone ��ѯ�ɽ���Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_doneinfo_rspΪNULL��
	* @see KFTSAPI_RegQueryDoneCallBack
	* @see KFTSAPI_QueryDone
	*/
	typedef void (*PFN_RSP_QUERY_DONE)(APIHandle hApiHandle, const STDoneInfo *pDone, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ֤ȯ��ϢӦ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pSecCode ֤ȯ��Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QuerySecCode ��ѯ֤ȯ��Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_seccode_rspΪNULL��
	* @see KFTSAPI_RegQuerySecCodeCallBack
	* @see KFTSAPI_QuerySecCode
	*/
	typedef void (*PFN_RSP_QUERY_SECCODE)(APIHandle hApiHandle, const STSecCodeInfo *pSecCode, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯETF������ϢӦ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pETFBaseInfo ETF������Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryEtfBaseInfo ��ѯETF������Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_etfbaseinfo_rspΪNULL��
	* @see KFTSAPI_RegQueryEtfBaseInfoCallBack
	* @see KFTSAPI_QueryEtfBaseInfo
	*/
	typedef void (*PFN_RSP_QUERY_ETFBASEINFO)(APIHandle hApiHandle, const STEtfBaseInfo *pETFBaseInfo, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯETF������ϢӦ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pEFTBasketInfo ETF������Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryEtfBasketInfo ��ѯETF������Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_etfbasketinfo_rspΪNULL��
	* @see KFTSAPI_RegQueryEtfBasketInfoCallBack
	* @see KFTSAPI_QueryEtfBasketInfo
	*/
	typedef void (*PFN_RSP_QUERY_ETFBASKETINFO)(APIHandle hApiHandle, const STEtfBasketInfo *pEFTBasketInfo, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ������ϢӦ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pFeeInfo ETF������Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryFeeInfo ��ѯETF������Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_feeinfo_rspΪNULL��
	* @see KFTSAPI_QueryFeeInfo
	*/
	typedef void (*PFN_RSP_QUERY_FEEINFO)(APIHandle hApiHandle, const STFeeInfo *pFeeInfo, int nRequestID, int nRecordID);

	/**
	* @brief �µ��ͳ����Ĵ�����Ϣ�ص�����
	* @param[out] hApiHandle API���
	* @param[out] p_order_info ������Ϣ
	* @details ���µ��򳷵�����APIͨ���ûص�����֪ͨ�ͻ��ˡ�
	* @see KFTSAPI_RegErrMsgInvalidCallBack
	*/
	typedef void (*PFN_RTN_ERRORORDER)(APIHandle hApiHandle, const STOrderInfo *pOrder, int nDataSrc);

	/**
	* @brief ѯ��¼��Ӧ��
	* @param[out] hApiHandle API���
	* @param[out] pRspForQuoteInsert ѯ��Ӧ����Ϣ
	* @param[out] pRspMsg ������Ϣ�����еĴ������Ϊ0ʱ��ʾ�ɹ�����0ʱ��ʾʧ��
	* @details �ͻ��˵���\ref KFTSAPI_ForQuoteInsert ����ѯ�ۣ�ѯ�۽��ͨ���ûص�֪ͨ�ͻ��ˡ�
	* @see KFTSAPI_ForQuoteInsert
	*/
	typedef void (*PFN_RSP_FORQUOTEINSERT)(APIHandle hApiHandle, const STRspForQuoteInsert *pRspForQuoteInsert, const STRspMsg *pRspMsg);

	/**
	* @brief ��ѯ�ɶ�������г�����ص�����
	* @param[out] hApiHandle API���
	* @param[out] pHolder �ɶ�������г�������Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details 
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_etfbaseinfo_rspΪNULL��
	* @see KFTSAPI_RegQueryHoldernoCallBack
	* @see KFTSAPI_QueryHolderno 
	*/
	typedef void (*PFN_RSP_QUERY_HOLDERNO)(APIHandle hApiHandle, const STHoldernoInfo *pHolder, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ�ڻ���Ϻ�Լ�ֲ���ϸ��ϢӦ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pPositionCombineDetail �ڻ���Ϻ�Լ�ֲ���ϸ��Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryPositionCombDet ��ѯ�ڻ���Ϻ�Լ�ֲ���ϸ��Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_pos_detail_rspΪNULL��
	* @see KFTSAPI_QueryPositionCombDet
	*/
	typedef void (*PFN_RSP_QUERY_POSITION_COMB_DET)(APIHandle hApiHandle, const STFutPositionCombDetail *pPositionCombineDetail, int nRequestID, int nRecordID);

		/**
	* @brief ��ѯ�ڻ���Ϻ�Լ�ֲ���ϢӦ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pPositionCombine �ڻ���Ϻ�Լ�ֲ���Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryPositionComb ��ѯ�ڻ���Ϻ�Լ�ֲ���Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_poscmb_rspΪNULL��
	* @see KFTSAPI_QueryPositionComb
	*/
	typedef void (*PFN_RSP_QUERY_POSITION_COMB)(APIHandle hApiHandle, const STFutPositionCombInfo *pPositionCombine, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ�ڻ���ͨ��Լ��ϢӦ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pInstrument �ڻ���ͨ��Լ��Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryFutContInfo ��ѯ�ڻ���ͨ��Լ��Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_coninfo_rspΪNULL��
	* @see KFTSAPI_QueryFutContInfo
	*/
	typedef void (*PFN_RSP_QUERY_FUT_CONT)(APIHandle hApiHandle, const STFutContractInfo *pInstrument, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ�ڻ���Ϻ�Լ��ϢӦ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pCombineInstrument �ڻ���Ϻ�Լ��Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryFutContComb ��ѯ�ڻ���Ϻ�Լ��Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_coninfo_rspΪNULL��
	* @see KFTSAPI_QueryFutContCombInfo
	*/
	typedef void (*PFN_RSP_QUERY_FUT_CONT_COMB)(APIHandle hApiHandle, const STFutContractCombInfo *pCombineInstrument, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ�ڻ���Լ����Ӧ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pFeeRate �ڻ���Լ������Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryFutFeeRate ��ѯ�ڻ���Լ������Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��p_fee_rate_rspΪNULL��
	* @see KFTSAPI_QueryFutFeeRate
	*/
	typedef void (*PFN_RSP_QUERY_FUT_FEE_RATE)(APIHandle hApiHandle, const STFuturefee *pFeeRate, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ�ڻ���Ϻ�Լ���Դ���Ӧ��ص�����
	* @param[out] hApiHandle API���
	* @param[out] pFutCombStrategyInfo �ڻ���Ϻ�Լ���Դ�����Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details �ͻ��˵���\ref KFTSAPI_QueryFutCombStrategy ��ѯ�ڻ���Ϻ�Լ���Դ�����Ϣ��
	* ϵͳ������첽ͨ���ûص�������������ظ��ͻ��ˡ�
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��������¼��pFutCombStrategyInfoΪNULL��
	* @see KFTSAPI_QueryFutCombStrategy
	*/
	typedef void (*PFN_RSP_QUERY_COMB_STRATEGY)(APIHandle hApiHandle, const STFutCombStrategyInfo *pFutCombStrategyInfo, int nRequestID, int nRecordID);

	/**
	* @brief ��ѯ�����ֵ�
	* @param[out] hApiHandle API���
	* @param[out] pDict �����ֵ���Ϣ��ѯ���
	* @param[out] nRequestID ������
	* @param[out] nRecordID ��¼��
	* @details 
	* nRecordIDΪ\ref EMPTY_ROW ʱ��ʾ��ѯ������¼��
	*/
	typedef void (*PFN_RSP_QUERY_DICTIONARYINFO)(APIHandle hApiHandle, const STDictionaryInfo *pDict, int nRequestID, int nRecordID);

	/** @} */

	/** \defgroup Group31 3.1 �ӿں���-ȫ�ֹ��� */
	/** @{ */
	/**
	* @brief ȫ�ֳ�ʼ��
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details APIȫ�ֳ�ʼ����������Ҫ�ҽ������һ��
	* @see KFTSAPI_Finalize
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Initialize(STRspMsg * pRspMsg);

	/**
	* @brief ���API�������Ϣ
	* @param[in] hApiHandle API���
	* @param[out] pApiInfo API��Ϣ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ��ѯAPI�������Ϣ
	*/
	KFTS_TRADE_API BOOL KFTSAPI_GetApiInfo(STApiInfo* pApiInfo, STRspMsg * pRspMsg);

	/**
	* @brief ȫ���˳�
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details APIȫ���˳���������Ҫ�ҽ������һ��
	* @see KFTSAPI_Initialize
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Finalize(STRspMsg * pRspMsg);
	/** @} */

	/** \defgroup Group32 3.2 �ӿں���-����ԱApi���� */
	/** @{ */
	/**
	* @brief ��������ԱAPI���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return API���,����ʧ��ʱΪNULL,�����ɹ�ʱ��NULL
	* @details ��������ԱAPI������ɶ�ε��ã�ÿ��APIHandle��Ӧһ������Ա
	* @see KFTSAPI_Destroy
	*/
	KFTS_TRADE_API APIHandle KFTSAPI_Create(STRspMsg * pRspMsg);

	/**
	* @brief ���ٽ���ԱAPI���
	* @param[in] hApiHandle Ҫ���ٵ�API���,����ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ���ٽ���ԱAPI������ͷ���Դ
	* @see KFTSAPI_Create
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Destroy(APIHandle hApiHandle, STRspMsg * pRspMsg);

	/**
	* @brief ����API����
	* @param[in] hApiHandle Ҫ���õ�API���,���ΪNULL��Ϊȫ������
	* @param[in] nOpt Ҫ���õĲ������ͣ��ο�\ref KFTS_OPTION
	* @param[in] value Ҫ���õĲ���ֵ
	* @param[in] nSize ����ֵ���ͳ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ����ĳ��API������ȫ�ֲ���������ѡ��μ�\ref KFTS_OPTION
	* @see KFTS_OPTION
	*/
	KFTS_TRADE_API BOOL KFTSAPI_SetOpt(APIHandle hApiHandle, int nOpt, const void *pValue, int nSize, STRspMsg * pRspMsg);

	/**
	* @brief ����API��������Ϣ
	* @param[in] hApiHandle Ҫ���õ�API���,����ΪNULL
	* @param[in] p_context Ҫ���õ���������Ϣ
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ����API��������Ϣ
	* @see KFTSAPI_GetApiContext
	*/
	KFTS_TRADE_API BOOL KFTSAPI_SetApiContext(APIHandle hApiHandle, const void * pContext, STRspMsg * pRspMsg);

	/**
	* @brief ��ȡAPI��������Ϣ
	* @param[in] hApiHandle Ҫ��ȡ��������Ϣ��API���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return API��������Ϣ
	* @details ��ȡAPI��������Ϣ
	* @see KFTSAPI_SetApiContext
	*/
	KFTS_TRADE_API void * KFTSAPI_GetApiContext(APIHandle hApiHandle, STRspMsg * pRspMsg);

	/**
	* @brief ��ȡAPI��ǰ״̬
	* @param[in] hApiHandle Ҫ��ȡ��ǰ�ص��ŵ�API���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return API��ǰ״̬��-1��ʧ��
	* @details ��ȡAPI��ǰ״̬
	* @see KFTSAPI_GetApiStatus
	*/
	KFTS_TRADE_API int KFTSAPI_GetApiStatus(APIHandle hApiHandle,STRspMsg * pRspMsg);

	/**
	* @brief ��ȡAPI��ǰ�ص���
	* @param[in] hApiHandle Ҫ��ȡ��ǰ�ص��ŵ�API���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return API��ǰ�ص��ţ�-1,ʧ��
	* @details ��ȡAPI��ǰ�ص���
	* @see KFTSAPI_GetCallbackNO
	*/
	KFTS_TRADE_API int KFTSAPI_GetCallbackNO(APIHandle hApiHandle,STRspMsg * pRspMsg);

	/**
	* @brief ע��һ�������Ϣ�ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õĴ�����Ϣ�ص�����
	* @param[out] pOldFunc �Ѿ�ע��Ĵ�����Ϣ�ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע��һ�������Ϣ�ص�������������һ�����ʱ��API�����ע��Ĵ�����Ϣ�ص�������֪ͨ�ͻ��ˡ�
	* ����������Щ������Ϣ���ɲ�ע��ûص�������
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegErrMsgCallBack(APIHandle hApiHandle, PFN_RSP_ERRORMSG fNewFunc, PFN_RSP_ERRORMSG *pOldFunc, STRspMsg * pRspMsg);
	/** @} */

	/** \defgroup Group33 3.3 �ӿں���-��¼���� */
	/** @{ */
	/**
	* @brief ע������֪ͨ�ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�����֪ͨ�ص�����
	* @param[out] pOldFunc �Ѿ�ע�������֪ͨ�ص���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע������֪ͨ�ص��������ú�������ע�ᡣ
	* �ͻ��˵���\ref KFTSAPI_StartLogin ֮��API��������ǰ�ã�����ǰ�ý������ӳɹ�ʱ��
	* API����õ�ǰע�������֪ͨ�ص�����֪ͨ�ͻ��ˣ��ͻ����ڵõ����ӳɹ�֪ͨ��
	* �ſ��Ե���\ref KFTSAPI_Login ���е�¼��
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegConnectedRtnCallBack(APIHandle hApiHandle, PFN_RTN_CONNECTED fNewFunc, PFN_RTN_CONNECTED * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע���½֪ͨ�ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õĵ�½֪ͨ�ص�����
	* @param[out] pOldFunc �Ѿ�ע��ĵ�½֪ͨ�ص���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע���½֪ͨ�ص��������ͻ��˵���\ref KFTSAPI_Login ���е�¼��
	* ����¼�ɹ���ʧ��ʱ��APIͨ����¼֪ͨ�ص������������֪�ͻ��ˡ�
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegLoginedRtnCallBack(APIHandle hApiHandle, PFN_RTN_LOGINED fNewFunc, PFN_RTN_LOGINED * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע����Ϣ�������֪ͨ�ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ���Ϣ�������֪ͨ�ص�����
	* @param[out] pOldFunc �Ѿ�ע�����Ϣ�������֪ͨ�ص���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע����Ϣ�������֪ͨ�ص����������ͻ���¼�ɹ�ʱ��API���Զ���ǰ�÷��Ͷ�������
	* �Ը���������Ϣ��������ȫ�����º�API��ͨ����Ϣ������ɻص�֪ͨ�ͻ��ˡ�
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegSubscribeRtnCallBack(APIHandle hApiHandle, PFN_RTN_SUBSCRIBE fNewFunc, PFN_RTN_SUBSCRIBE * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ǰ�û�
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] nFrontID ǰ�û���ţ��������0
	* @param[in] pszFrontAddress �����ַ�������ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001����
	* ���У���tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	* Ŀǰ֧�ֵĴ���Э��Ϊtcp��
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע��ǰ�û�
	* @see KFTSAPI_RemoveFront
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegFront(APIHandle hApiHandle, int nFrontID, const char * pszFrontAddress, STRspMsg * pRspMsg);

	/**
	* @brief �Ƴ�ǰ�û�
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] nFrontID ǰ�û���ţ��������0
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details �Ƴ�ǰ�û�
	* @see KFTSAPI_RegFront
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RemoveFront(APIHandle hApiHandle, int nFrontID, STRspMsg * pRspMsg);

	/**
	* @brief ����ǰ�û�����ʼ��½
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] nFrontID ��ʼ���ӵ�ǰ�û�, ������Ӳ��ɹ�, ��ȥ��������ǰ�û�
	* @param[in] nConnTimeout ���ӳ�ʱʱ�䣬�Ժ���Ϊ��λ
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @param[in] nSyncMode ͬ��-�첽ģʽ��0:�첽ģʽ, 1:ͬ��ģʽ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ����ǰ�û�����ʼ��½��
	* ͬ��ģʽֱ�ӷ������ӽ����
	* �첽ģʽ�첽�������ӽ����ͨ��\ref PFN_RTN_CONNECTED �����ӽ��֪ͨ�ͻ��ˡ�
	* @see PFN_RTN_CONNECTED
	* @see KFTSAPI_RegConnectedRtnCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_StartLogin(APIHandle hApiHandle, int nFrontID, int nConnTimeout, STRspMsg * pRspMsg, int nSyncMode = 0);

	/**
	* @brief ����Ա��¼
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pUserLogin ��¼��Ϣ
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ����Ա��¼���ں�ǰ�ý������ӳɹ�֪ͨ�ͻ��˺󣬿ͻ��˵��øú������е�¼��
	* �첽���ؽ����ͨ��\ref PFN_RTN_LOGINED �����֪ͨ�ͻ��ˡ�
	* @see PFN_RTN_LOGINED
	* @see KFTSAPI_RegLoginedRtnCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Login(APIHandle hApiHandle, const STTraderLogin* pRspUserLogin, STRspMsg * pRspMsg);

	/**
	* @brief ����Ա�ǳ�
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] nTimeout ��ʱʱ�䣬�Ժ���Ϊ��λ
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ����Ա�ǳ���
	* ͬ�����ؽ����
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Logout(APIHandle hApiHandle, int nTimeout, STRspMsg * pRspMsg);
	/** @} */

	/** \defgroup Group34 3.4 �ӿں���-���ӹ��� */
	/** @{ */
	/**
	* @brief ע��Ͽ�����֪ͨ�ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õĶϿ�����֪ͨ�ص�����
	* @param[in] pOldFunc �Ѿ�ע��ĶϿ�����֪ͨ�ص���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details �ͻ��˿�ע��Ͽ������ص�����\ref PFN_RTN_RECONNECT ��������ǰ�öϿ�ʱ�ͻ����Ƿ��Զ������Լ�����ʱʹ���ĸ�ǰ�á�
	* ���ͻ���δע��ûص�������ϵͳ���Զ�ѡ��ǰ�û������������ӡ�
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegReconnectRtnCallBack(APIHandle hApiHandle, PFN_RTN_RECONNECT fNewFunc, PFN_RTN_RECONNECT * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ��ǰ�ý����µ�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] nFrontID ǰ�û�ID�����Ϊ0, ��ϵͳѡ��ǰ�û�������0ʱ��Ϊָ��ǰ�û���
	* @param[in] nConnTimeout ��ʱʱ�䣬�Ժ���Ϊ��λ
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return ���Ӿ����ʧ�ܷ���NULL���ɹ����ط�NULL
	* @details �ͻ��˿ɸ�����Ҫ����ǰ�ý���������ӡ�
	* ������login�ɹ�֮����á�
	* @see KFTSAPI_Close
	*/
	KFTS_TRADE_API CONNHandle KFTSAPI_Connect(APIHandle hApiHandle, int nFrontID, int nConnTimeout, STRspMsg * pRspMsg);

	/**
	* @brief �ر���ǰ�û�������
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle Ҫ�رյ����Ӿ������ΪNULL��ΪNULL��ʾ�ر��������ӡ�
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details �ر�ָ������
	* @see KFTSAPI_Connect
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Close(APIHandle hApiHandle, CONNHandle hConnHandle, STRspMsg * pRspMsg);

	/**
	* @brief ����������������Ϣ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ��������ΪNULL
	* @param[in] pContext Ҫ���õ���������Ϣ
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ����������������Ϣ
	* @see KFTSAPI_GetConnectionContext
	*/
	KFTS_TRADE_API BOOL KFTSAPI_SetConnectionContext(APIHandle hApiHandle, CONNHandle hConnHandle, const void * pContext, STRspMsg * pRspMsg);

	/**
	* @brief ��ȡ������������Ϣ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ��������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return �����õ�������������Ϣ
	* @details ��ȡ������������Ϣ
	* @see KFTSAPI_SetConnectionContext
	*/
	KFTS_TRADE_API void * KFTSAPI_GetConnectionContext(APIHandle hApiHandle, CONNHandle hConnHandle, STRspMsg * pRspMsg);
	/** @} */

	/** \defgroup Group35 3.5 �ӿں���-ί�м������� */
	/** @{ */
	/**
	* @brief ע�ᱨ��֪ͨ�ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õı���֪ͨ�ص�����
	* @param[out] pOldFunc �Ѿ�ע��ı���֪ͨ�ص���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע�ᱨ��֪ͨ�ص�����
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQuoteRtnCallBack(APIHandle hApiHandle, PFN_RTN_QUOTE fNewFunc, PFN_RTN_QUOTE * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ί�лر����ʹ���ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�ί�лر����ʹ���ص�����
	* @param[out] pOldFunc �Ѿ�ע���ί�лر����ʹ���ص���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע��ί�лر����ʹ���ص�����
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegOrderRtnCallBack(APIHandle hApiHandle, PFN_RTN_ORDER fNewFunc, PFN_RTN_ORDER * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ɽ����ʹ���ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õĳɽ����ʹ���ص�����
	* @param[out] pOldFunc �Ѿ�ע��ĳɽ����ʹ���ص���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע��ɽ����ʹ���ص�����
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegDoneRtnCallBack(APIHandle hApiHandle, PFN_RTN_DONE fNewFunc, PFN_RTN_DONE * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע���ʽ�䶯���ʹ���ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ��ʽ�䶯���ʹ���ص�����
	* @param[out] pOldFunc �Ѿ�ע����ʽ�䶯���ʹ���ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע���ʽ�䶯���ʹ���ص������������ʽ�䶯ʱ��APIͨ���ʽ�䶯���ʹ���ص�����֪ͨ�ͻ��ˡ�
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegAccountRtnCallBack(APIHandle hApiHandle, PFN_RTN_ACCOUNT fNewFunc, PFN_RTN_ACCOUNT * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ֱֲ䶯���ʹ���ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õĳֱֲ䶯���ʹ���ص�����
	* @param[out] pOldFunc �Ѿ�ע��ĳֱֲ䶯���ʹ���ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע��ֱֲ䶯���ʹ���ص����������гֱֲ䶯ʱ��APIͨ���ֱֲ䶯���ʹ���ص�����֪ͨ�ͻ��ˡ�
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegPositionRtnCallBack(APIHandle hApiHandle, PFN_RTN_POSITION fNewFunc, PFN_RTN_POSITION * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע����ȯ�����������ʹ���ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õı��ȯ�����������ʹ���ص�����
	* @param[out] pOldFunc �Ѿ�ע��ı��ȯ�����������ʹ���ص���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע����ȯ�����������ʹ���ص�����
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegUnderlyingStockActionRtnCallBack(APIHandle hApiHandle, PFN_RTN_UNDERLYINGSTOCKACTION fNewFunc, PFN_RTN_UNDERLYINGSTOCKACTION * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ͨ���������ʹ���ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�ͨ���������ʹ���ص�����
	* @param[out] pOldFunc �Ѿ�ע���ͨ���������ʹ���ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע��ͨ���������ʹ���ص����������յ�ͨ����������ɵķ�������ʱ��APIͨ���������ʹ���ص�����֪ͨ�ͻ��ˡ�
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegCommonRequestRtnCallBack(APIHandle hApiHandle, PFN_RTN_COMMONREQUEST fNewFunc, PFN_RTN_COMMONREQUEST * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ת�����󷵻ػص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�ת�����󷵻ػص�����
	* @param[out] pOldFunc �Ѿ�ע���ת�����󷵻ػص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_QueryFeeInfo
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegFundTransCallBack(APIHandle hApiHandle, PFN_RTN_FUNDTRANS fNewFunc, PFN_RTN_FUNDTRANS *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ����¼������
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ����ΪNULLʱʹ��������
	* @param[in] pInputQuote ���۽ṹ��
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ����¼��ָ�
	* @see KFTSAPI_ReqQuoteInsert
	*/
	KFTS_TRADE_API BOOL KFTSAPI_ReqQuoteInsert(APIHandle hApiHandle, CONNHandle hConnHandle, const STInputQuote *pInputQuote, STRspMsg * pRspMsg);
	
	/**
	* @brief ������������
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ����ΪNULLʱʹ��������
	* @param[in] pInputOrderAction ���������ṹ��
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ��������ָ�
	* @see KFTSAPI_ReqOrderAction
	*/
	KFTS_TRADE_API BOOL KFTSAPI_ReqOrderAction(APIHandle hApiHandle, CONNHandle hConnHandle, const STInputOrderAction *pInputOrderAction, STRspMsg * pRspMsg);

	/**
	* @brief ���� - �ɽӿ�
	* @param[in] handle API���������ΪNULL
	* @param[in] conn_handle ���Ӿ����ΪNULLʱʹ��������
	* @param[in] p_order_cancel �����ṹ����
	* @param[in] cancel_count ���γ������������ܳ��������������(\ref MAX_ORDER_CANCEL_COUNT)
	* @param[out] err_msg ʧ��ʱ�Ĵ�����Ϣ
	* @param[in] timeout �ñʳ����ĳ�ʱʱ�䣬�Ժ���Ϊ��λ��INFINITEΪ���޳�ʱ���ò���Ĭ��ֵINFINITE - 1Ϊʹ��Ĭ�ϳ�ʱѡ��(\ref KFTS_OPT_DEFAULT_TIMEOUT)���õĳ�ʱʱ��
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ����ָ�����һ�������ӿ������
	* @see KFTSAPI_OrderCancelByOrderNo
	*/
	KFTS_TRADE_API BOOL KFTSAPI_OrderCancelByOrderNo(APIHandle handle, CONNHandle conn_handle, const STOrderCancelByOrderNo *p_order_cancel, int cancel_count, STRspMsg * err_msg, int timeout = INFINITE -1);


	/**
	* @brief �µ�ָ��
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ����ΪNULLʱʹ��������
	* @param[in] pOrder ί�нṹ����
	* @param[in] nOrderCount ����ί�����������ܳ���ί���������(\ref MAX_ORDER_COUNT)
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @param[in] nTimeout �ñ�ί�еĳ�ʱʱ�䣬�Ժ���Ϊ��λ��INFINITEΪ���޳�ʱ���ò���Ĭ��ֵINFINITE - 1Ϊʹ��Ĭ�ϳ�ʱѡ��(\ref KFTS_OPT_DEFAULT_TIMEOUT)���õĳ�ʱʱ��
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details �µ�ָ�һ���µ�������ΧΪ1-\ref MAX_ORDER_COUNT ��
	* @see KFTSAPI_Order
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Order(APIHandle hApiHandle, CONNHandle hConnHandle, const STOrder *pOrder, int nOrderCount, STRspMsg * pRspMsg, int nTimeout = INFINITE - 1);

	/**
	* @brief ��ǿ�µ�ָ��
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ����ΪNULLʱʹ��������
	* @param[in] pOrder ί�нṹ����
	* @param[in] nOrderCount ����ί�����������ܳ���ί���������(\ref MAX_ORDER_COUNT)
	* @param[in] nBatchNo ����
	* @param[in] nOrderProp ����ί������ 0-�������µ��ɹ�������ʧ�ܣ�1-ȫ���µ��ɹ�����ȫ��ȡ��
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @param[in] nTimeout �ñ�ί�еĳ�ʱʱ�䣬�Ժ���Ϊ��λ��INFINITEΪ���޳�ʱ���ò���Ĭ��ֵINFINITE - 1Ϊʹ��Ĭ�ϳ�ʱѡ��(\ref KFTS_OPT_DEFAULT_TIMEOUT)���õĳ�ʱʱ��
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ��ǿ�µ�ָ�һ���µ�������ΧΪ1-\ref MAX_ORDER_COUNT ��
	* ��ǿ�µ�ָ���ʹ�����Ž������µ�ָ������Ϊһ������ʹ����ʱ�����ų���������������ί��ȫ��������
	* ͬʱ����ǿ�µ�ָ������ñ����µ��Ƿ����ȫ���ɹ�����ȡ����
	* @see KFTSAPI_OrderEx
	*/
	KFTS_TRADE_API BOOL KFTSAPI_OrderEx(APIHandle hApiHandle, CONNHandle hConnHandle, const STOrder *pOrder, int nOrderCount, int nBatchNo, int nOrderProp, STRspMsg * pRspMsg, int nTimeout = INFINITE -1);

	/**
	* @brief ͨ������
	* @param[in] hApiHandle hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ����ΪNULLʱʹ��������
	* @param[in] nTID ��������
	* @param[in] pCommonInfo ��������
	* @param[in] nCommonInfoLen �������ݳ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @param[in] nTimeout �ñ�����ĳ�ʱʱ�䣬�Ժ���Ϊ��λ��INFINITEΪ���޳�ʱ���ò���Ĭ��ֵINFINITE - 1Ϊʹ��Ĭ�ϳ�ʱѡ��(\ref KFTS_OPT_DEFAULT_TIMEOUT)���õĳ�ʱʱ��
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ͨ�������ܡ�
	*/
	KFTS_TRADE_API BOOL KFTSAPI_CommonRequest(APIHandle hApiHandle, CONNHandle hConnHandle, int nTID, const char* pCommonInfo, int nCommonInfoLen, STRspMsg * pRspMsg, int nTimeout = INFINITE -1);

	/**
	* @brief ת������
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ����ΪNULLʱʹ��������
	* @param[in] pTransfer ת������ṹ����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ת������
	*/
	KFTS_TRADE_API BOOL KFTSAPI_TransFund(APIHandle hApiHandle, CONNHandle hConnHandle, STFundTransREQ *pTransfer, STRspMsg * pRspMsg);

	/**
	* @brief �ڻ���ϵ��µ�ָ��
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ����ΪNULLʱʹ��������
	* @param[in] pOrderCombine ί�нṹ����
	* @param[in] nOrderCombineCount ����ί�����������ܳ���ί���������(\ref MAX_ORDER_COUNT)
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @param[in] nBatchNo ����ί�����κ�
	* @param[in] nTimeout �ñ�ί�еĳ�ʱʱ�䣬�Ժ���Ϊ��λ��INFINITEΪ���޳�ʱ���ò���Ĭ��ֵINFINITE - 1Ϊʹ��Ĭ�ϳ�ʱѡ��(\ref KFTS_OPT_DEFAULT_TIMEOUT)���õĳ�ʱʱ��
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details �µ�ָ�һ���µ�������ΧΪ1-\ref MAX_ORDER_COUNT ��
	* @see KFTSAPI_OrderComb
	*/
	KFTS_TRADE_API BOOL KFTSAPI_OrderComb(APIHandle hApiHandle, CONNHandle hConnHandle, const STOrderComb *pOrderCombine, int nOrderCombineCount, STRspMsg * pRspMsg, int nBatchNo, int nTimeout = INFINITE - 1);

	/**
	* @brief ע����ϵ��ֱֲ䶯���ʹ���ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ���ϵ��ֱֲ䶯���ʹ���ص�����
	* @param[out] pOldFunc �Ѿ�ע�����ϵ��ֱֲ䶯���ʹ���ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע����ϵ��ֱֲ䶯���ʹ���ص�������������ϵ��ֱֲ䶯ʱ��APIͨ����ϵ��ֱֲ䶯���ʹ���ص�����֪ͨ�ͻ��ˡ�
	* @see PFN_RTN_POSITION_COMB
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegPositionCombRtnCallBack(APIHandle hApiHandle, PFN_RTN_POSITION_COMB fNewFunc, PFN_RTN_POSITION_COMB * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע����ϵ��ֲ���ϸ�䶯���ʹ���ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ���ϵ��ֲ���ϸ�䶯���ʹ���ص�����
	* @param[out] pOldFunc �Ѿ�ע�����ϵ��ֲ���ϸ�䶯���ʹ���ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע����ϵ��ֲ���ϸ�䶯���ʹ���ص�������������ϵ��ֲ���ϸ�䶯ʱ��APIͨ����ϵ��ֲ���ϸ�䶯���ʹ���ص�����֪ͨ�ͻ��ˡ�
	* @see PFN_RTN_POSITION_COMB_DET
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegPositionCombDetRtnCallBack(APIHandle hApiHandle, PFN_RTN_POSITION_COMB_DET fNewFunc, PFN_RTN_POSITION_COMB_DET * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע���µ��ͳ����ı���ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ��µ��ͳ����ı���ص�����
	* @param[out] pOldFunc �Ѿ�ע����µ��ͳ����ı���ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RTN_ERRORORDER
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegErrOrderRtnCallBack(APIHandle hApiHandle, PFN_RTN_ERRORORDER fNewFunc, PFN_RTN_ERRORORDER *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��SPX����ԱУ��Ӧ��ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�У��Ӧ��ص�����
	* @param[out] pOldFunc �Ѿ�ע���У��Ӧ��ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RTN_CHECKED
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegCheckedRtnCallBack(APIHandle hApiHandle, PFN_RTN_CHECKED fNewFunc, PFN_RTN_CHECKED *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief SPX��ͨ����ԱУ��
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pReqUserLogin У����Ϣ
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details SPX���ؽ����ʹ�ϵͳ��ͨ����ԱУ�顣�ں�ǰ�ý������ӳɹ�֪ͨ�ͻ��˺󣬿ͻ��˵��øú�������У�顣
	* �첽���ؽ����ͨ��\ref PFN_RTN_LOGINED �����֪ͨ�ͻ��ˡ�
	* @see PFN_RTN_LOGINED
	* @see KFTSAPI_RegLoginedRtnCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_Check(APIHandle hApiHandle, const STTraderCheck* pReqUserLogin, STRspMsg * pRspMsg);

	/**
	* @brief ע��ѯ��¼��Ӧ��ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�ѯ��¼��Ӧ��ص�����
	* @param[out] pOldFunc �Ѿ�ע���ѯ��¼��Ӧ��ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_FORQUOTEINSERT
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegForQuoteInsertCallBack(APIHandle hApiHandle, PFN_RSP_FORQUOTEINSERT fNewFunc, PFN_RSP_FORQUOTEINSERT *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ѯ��¼������
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ����ΪNULLʱʹ��������
	* @param[in] pForQuoteInsert ѯ������
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ѯ��¼������
	*/
	KFTS_TRADE_API  BOOL KFTSAPI_ForQuoteInsert(APIHandle hApiHandle, CONNHandle hConnHandle, const STForQuoteInsert *pForQuoteInsert, STRspMsg * pRspMsg);
	
	/**
	* @brief ���ȯ����\��������
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] hConnHandle ���Ӿ����ΪNULLʱʹ��������
	* @param[in] pReqUnderlyingStockAction ���ȯ������������
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ���ȯ����\��������¼������
	*/
	KFTS_TRADE_API  BOOL KFTSAPI_ReqUnderlyingStockAction(APIHandle hApiHandle, CONNHandle hConnHandle, const STReqUnderlyingStockAction *pReqUnderlyingStockAction, STRspMsg * pRspMsg);
	
	/** @} */

	/** \defgroup Group36 3.6 �ӿں���-��ѯ���� */
	/** @{ */
	/**
	* @brief ע���˻��ʽ��ѯ���ػص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ��˻��ʽ��ѯ���ػص�����
	* @param[out] pOldFunc �Ѿ�ע����˻��ʽ��ѯ���ػص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_QueryAccount
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryAccountCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_ACCOUNT fNewFunc, PFN_RSP_QUERY_ACCOUNT *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ֲֲ�ѯ���ػص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õĳֲֲ�ѯ���ػص�����
	* @param[out] pOldFunc �Ѿ�ע��ĳֲֲ�ѯ���ػص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_QueryPosition
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryPositionCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_POSITION fNewFunc, PFN_RSP_QUERY_POSITION *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע�ᱨ�۵���ѯ�ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õı��۵���ѯ�ص�����
	* @param[out] pOldFunc �Ѿ�ע��ı��۵���ѯ�ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_ReqQryQuote
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQryQuoteCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_QUOTE fNewFunc, PFN_RSP_QUERY_QUOTE *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ί�в�ѯ���ػص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�ί�в�ѯ���ػص�����
	* @param[out] pOldFunc �Ѿ�ע���ί�в�ѯ���ػص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_QueryOrder
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryOrderCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_ORDER fNewFunc, PFN_RSP_QUERY_ORDER *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ɽ���ѯ���ػص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õĳɽ���ѯ���ػص�����
	* @param[out] pOldFunc �Ѿ�ע��ĳɽ���ѯ���ػص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_QueryDone
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryDoneCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_DONE fNewFunc, PFN_RSP_QUERY_DONE *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��֤ȯ��Ϣ��ѯ���ػص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�֤ȯ��Ϣ��ѯ���ػص�����
	* @param[out] pOldFunc �Ѿ�ע���֤ȯ��Ϣ��ѯ���ػص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_QuerySecCode
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQuerySecCodeCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_SECCODE fNewFunc, PFN_RSP_QUERY_SECCODE *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ETF������Ϣ��ѯ���ػص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�ETF������Ϣ��ѯ���ػص�����
	* @param[out] pOldFunc �Ѿ�ע���ETF������Ϣ��ѯ���ػص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_QueryEtfBaseInfo
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryEtfBaseInfoCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_ETFBASEINFO fNewFunc, PFN_RSP_QUERY_ETFBASEINFO *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע��ETF������Ϣ��ѯ���ػص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�ETF������Ϣ��ѯ���ػص�����
	* @param[out] pOldFunc �Ѿ�ע���ETF������Ϣ��ѯ���ػص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_QueryEtfBasketInfo
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryEtfBasketInfoCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_ETFBASKETINFO fNewFunc, PFN_RSP_QUERY_ETFBASKETINFO *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�˻��ʽ�
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryAccount ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_ACCOUNT
	* @see KFTSAPI_RegQueryAccountCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryAccount(APIHandle hApiHandle, STQueryAccount *pQryAccount, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�ֲ�
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryPosition ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_POSITION
	* @see KFTSAPI_RegQueryPositionCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryPosition(APIHandle hApiHandle, STQueryPosition *pQryPosition, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ���۵�
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryQuote ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_QUOTE
	* @see KFTSAPI_RegQryQuoteCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_ReqQryQuote(APIHandle hApiHandle, STQryQuote* pQryQuote, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯί��
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryOrder ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_ORDER
	* @see KFTSAPI_RegQueryOrderCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryOrder(APIHandle hApiHandle, STQueryOrder *pQryOrder, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�ɽ�
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryDone ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_DONE
	* @see KFTSAPI_RegQueryDoneCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryDone(APIHandle hApiHandle, STQueryDone *pQryDone, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ֤ȯ��Ϣ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQrySecCode ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_SECCODE
	* @see KFTSAPI_RegQuerySecCodeCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QuerySecCode(APIHandle hApiHandle, STQuerySecCode *pQrySecCode, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯETF������Ϣ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryETFBaseInfo ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_ETFBASEINFO
	* @see KFTSAPI_RegQueryEtfBaseInfoCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryEtfBaseInfo(APIHandle hApiHandle, STQueryEtfBaseInfo *pQryETFBaseInfo, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯETF������Ϣ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryEFTBasketInfo ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_ETFBASKETINFO
	* @see KFTSAPI_RegQueryEtfBasketInfoCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryEtfBasketInfo(APIHandle hApiHandle, STQueryEtfBasketInfo *pQryEFTBasketInfo, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ������Ϣ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryFeeInfo ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_FEEINFO
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryFeeInfo(APIHandle hApiHandle, STQueryFeeInfo *pQryFeeInfo, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ע�������Ϣ��ѯ���ػص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õķ�����Ϣ��ѯ���ػص�����
	* @param[out] pOldFunc �Ѿ�ע��ķ�����Ϣ��ѯ���ػص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_QueryFeeInfo
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryFeeInfoCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_FEEINFO fNewFunc, PFN_RSP_QUERY_FEEINFO *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ����ί����ˮ�Ų�ѯ�Ự�ź�˽�к�
	* @param[in]  hApiHandle API���������ΪNULL
	* @param[in]  pQryPrivateInfoByNo ��ѯʱ��Ҫ����ί����ˮ��
	* @param[out] pPrivateInfo ���صĽṹ,�����Ự�ź�˽�к�
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* �ú�������ͬ��ģʽ
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QuerySesidPrinoDirect(APIHandle hApiHandle, STQuerySesidPrino *pQryPrivateInfoByNo, STSesidPrinoInfo* pPrivateInfo, STRspMsg * pRspMsg);

	/**
	* @brief ע��ɶ�������г������ѯ�ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õĹɶ�������г������ѯ�ص�����
	* @param[out] pOldFunc �Ѿ�ע��Ĺɶ�������г������ѯ�ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ��SPX��ͨ����Ա��¼ʱ,�����ص��ú�����SPX���ؽ���Ա��Ӧ�г�����͹ɶ����룬�ݲ��ṩ������ѯ�ӿ�
	* @see PFN_RSP_QUERY_HOLDERNO 
	* @see KFTSAPI_QueryHolderno 
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryHoldernoCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_HOLDERNO fNewFunc, PFN_RSP_QUERY_HOLDERNO *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief �ɶ�������г������ѯ
	* @param[in]  hApiHandle API���������ΪNULL
	* @param[in]  pQryHolder ��ѯʱ����ͻ��ź��г�����
	* @param[in] nRequestID ������
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @details �г�����Ϊ0ʱ��ѯ�����г��Ĺɶ�����
	* @see PFN_RSP_QUERY_HOLDERNO 
	* @see KFTSAPI_RegQueryHoldernoCallBack 
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryHolderno(APIHandle hApiHandle, STQueryHolderno *pQryHolder, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�����ֵ���Ϣ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryDict ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_FEEINFO
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryDictionaryInfo(APIHandle hApiHandle, STQueryDictionaryInfo *pQryDict, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�ڻ���ͨ��Լ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryInstrument ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_FUT_CONT_INFO
	* @see KFTSAPI_RegQueryFutContCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryFutContInfo(APIHandle hApiHandle, STQueryFutContCode *pQryInstrument, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ע���ѯ�ڻ���ԼӦ��ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ��ڻ���ԼӦ��ص�����
	* @param[out] pOldFunc �Ѿ�ע����ڻ���ԼӦ��ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_FUT_CONT
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryFutContCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_FUT_CONT fNewFunc, PFN_RSP_QUERY_FUT_CONT *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ע���ڻ���ϵ�ί�лر����ʹ���ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ��ڻ���ϵ�ί�лر����ʹ���ص�����
	* @param[out] pOldFunc �Ѿ�ע����ڻ���ϵ�ί�лر����ʹ���ص���������ΪNULL
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details ע���ڻ���ϵ�ί�лر����ʹ���ص�����
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegOrderCombRtnCallBack(APIHandle hApiHandle, PFN_RTN_ORDER_COMB fNewFunc, PFN_RTN_ORDER_COMB * pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�ڻ���ϵ�ί��
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryOrderCombine ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_ORDER_COMB
	* @see KFTSAPI_RegQueryOrderCombCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryOrderComb(APIHandle hApiHandle, STQueryOrderComb *pQryOrderCombine, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ע��ί�в�ѯ���ػص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ�ί�в�ѯ���ػص�����
	* @param[out] pOldFunc �Ѿ�ע���ί�в�ѯ���ػص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see KFTSAPI_QueryOrderComb
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryOrderCombCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_ORDER_COMB fNewFunc, PFN_RSP_QUERY_ORDER_COMB *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�ڻ���Ϻ�Լ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryCombInstrument ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_FUT_CONT_COMB
	* @see KFTSAPI_RegQueryFutContCombCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryFutContComb(APIHandle hApiHandle, STQueryFutContCombCode *pQryCombInstrument, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ע���ѯ�ڻ���Ϻ�ԼӦ��ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ��ڻ���Ϻ�ԼӦ��ص�����
	* @param[out] pOldFunc �Ѿ�ע����ڻ���Ϻ�ԼӦ��ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_FUT_CONT_COMB
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryFutContCombCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_FUT_CONT_COMB fNewFunc, PFN_RSP_QUERY_FUT_CONT_COMB *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�ڻ���Ϻ�Լ�ֲ�
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryPositionCombine ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_POSITION_COMB
	* @see KFTSAPI_RegQueryPositionCombCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryPositionComb(APIHandle hApiHandle, STQueryFutPositionComb *pQryPositionCombine, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ע���ѯ�ڻ���Ϻ�Լ�ֲ�Ӧ��ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ��ڻ���Ϻ�Լ�ֲ�Ӧ��ص�����
	* @param[out] pOldFunc �Ѿ�ע����ڻ���Ϻ�Լ�ֲ�Ӧ��ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_POSITION_COMB
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryPositionCombCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_POSITION_COMB fNewFunc, PFN_RSP_QUERY_POSITION_COMB *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�ڻ���Ϻ�Լ�ֲ�
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryPositionCombineDetail ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_POSITION_COMB_DET
	* @see KFTSAPI_RegQueryPositionCombDetCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryPositionCombDet(APIHandle hApiHandle, STQueryFutPositionCombDetail *pQryPositionCombineDetail, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ע���ѯ�ڻ���Ϻ�Լ�ֲ���ϸӦ��ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ��ڻ���Ϻ�Լ�ֲ���ϸӦ��ص�����
	* @param[out] pOldFunc �Ѿ�ע����ڻ���Ϻ�Լ�ֲ���ϸӦ��ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_POSITION_COMB_DET
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryPositionCombDetCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_POSITION_COMB_DET fNewFunc, PFN_RSP_QUERY_POSITION_COMB_DET *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�ڻ���Լ������Ϣ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryFee ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_FUT_FEE_RATE
	* @see KFTSAPI_RegQueryFutFeeCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryFutFeeRate(APIHandle hApiHandle, STQueryFutFeeRate *pQryFee, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ע���ѯ�ڻ���Լ����Ӧ��ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ��ڻ���Լ����Ӧ��ص�����
	* @param[out] pOldFunc �Ѿ�ע����ڻ���Լ����Ӧ��ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_FUT_FEE_RATE
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryFutFeeCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_FUT_FEE_RATE fNewFunc, PFN_RSP_QUERY_FUT_FEE_RATE *pOldFunc, STRspMsg * pRspMsg);

	/**
	* @brief ��ѯ�ڻ���Ϻ�Լ���Դ�����Ϣ
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] pQryFutCombStrategyInfo ��ѯ������ΪNULL���ѯ���м�¼
	* @param[in] nRequestID ��ѯ���
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_COMB_STRAT_CODE
	* @see KFTSAPI_RegQueryCombStratCallBack
	*/
	KFTS_TRADE_API BOOL KFTSAPI_QueryFutCombStrategy(APIHandle hApiHandle, STQueryFutCombStrategyInfo *pQryFutCombStrategyInfo, int nRequestID, STRspMsg * pRspMsg);

	/**
	* @brief ע���ѯ�ڻ���Ϻ�Լ���Դ���Ӧ��ص�����
	* @param[in] hApiHandle API���������ΪNULL
	* @param[in] fNewFunc Ҫʹ�õ��ڻ���Ϻ�Լ���Դ���Ӧ��ص�����
	* @param[out] pOldFunc �Ѿ�ע����ڻ���Ϻ�Լ���Դ���Ӧ��ص�����
	* @param[out] pRspMsg ʧ��ʱ�Ĵ�����Ϣ
	* @return TRUEΪ�ɹ���FALSEΪʧ��
	* @details 
	* @see PFN_RSP_QUERY_COMB_STRATEGY_CODE
	*/
	KFTS_TRADE_API BOOL KFTSAPI_RegQueryCombStratCallBack(APIHandle hApiHandle, PFN_RSP_QUERY_COMB_STRATEGY fNewFunc, PFN_RSP_QUERY_COMB_STRATEGY *pOldFunc, STRspMsg * pRspMsg);
	
	/** @} */
#ifdef __cplusplus
}
#endif

#endif //__KFTS_TRADE_API_H__

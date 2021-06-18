/////////////////////////////////////////////////////////////////////////
///@system ���˴���һ���ʹ�ϵͳ
///@company SunGard China
///@file kfts_trade_type.h
///@brief �����˽��˴��ʹ�ϵͳ��������
///@history
///20141001	Kingstar IRDG		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef __KFTS_TRADE_TYPE_H__
#define __KFTS_TRADE_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

	/** \defgroup Group11 1.1 ���ݳ��ȶ��� */
	/** @{ */
	/// API�汾����
#define API_VERSION_LEN         20
	/// ��Ϣ����
#define MSG_LEN                 256
	/// �ͻ��ų���
#define CUST_NO_LEN             16
	/// ���볤��
#define PWD_LEN                 129
	/// ��Լ���볤��
#define INST_ID_LEN            20 
	/// ��Լ���Ƴ���
#define INST_NAME_LEN            16
	/// ��Ϻ�Լ���볤��
#define INST_COMBID_LEN      81
	/// ��ע����
#define NOTE_LEN                256
	/// ��̨ʵ���ų���
#define INSTANCE_ID_LEN         32
	/// �ͻ����Ƴ���
#define CUST_NAME_LEN           16
	/// ETF���Ƴ���
#define ETF_NAME_LEN            20
	/// ������ַ����
#define MAC_ADDR_LEN            23
	/// �ɶ��˺ų���
#define HOLDERNO_LEN			17
	/** @} */

	/** \defgroup Group12 1.2 �������Ͷ��� */
	/** @{ */
	/// ��������
#ifndef BOOL
	typedef int BOOL;
#endif

	/// ��ֵ
#ifndef TRUE
#define TRUE	1
#endif
	/// ��ֵ
#ifndef FALSE
#define FALSE	0
#endif

	/// API�汾����
	typedef char            TApiVersionType[API_VERSION_LEN];
	/// �����������
	typedef int             TErrorIDType;
	/// ������Ϣ����
	typedef char            TErrorMsgType[MSG_LEN];
	/// �ͻ�������
	typedef char            TCustNoType[CUST_NO_LEN];
	/// �ͻ���¼��������
	typedef char            TCustPwdType[PWD_LEN];
	/// �ն���������
	typedef int             TTerminalTypeType;
	/// SPX����Ա���鷽ʽ
	typedef int				TSPXCheckUserType;
	/// ��ע����
	typedef char            TNoteType[NOTE_LEN];
	/// ��̨ʵ��������
	typedef char            TInstanceIDType[INSTANCE_ID_LEN];
	/// ������ַ����
	typedef char            TMacType[MAC_ADDR_LEN];    
	/// �Ự������
	typedef int             TSessionIDType;
	/// ���ӻỰ������
	typedef int             TConnSessionIDType;
	/// �ͻ���������
	typedef char            TCustNameType[CUST_NAME_LEN];
	/// ������������
	typedef int             TDateType;
	/// ˽�к�����
	typedef int             TPrivateNoType;
	/// �ʲ���Ԫ�������
	typedef int             TCellIDType;
	/// Ͷ����ϱ������
	typedef int             TPortfolioIDType;
	/// �ʲ���Ԫ/Ͷ����ϱ�־���ͣ�0-�ʲ���Ԫ,1-Ͷ�����
	typedef int             TAccountTypeType;
	/// �г��������ͣ�����ֵ����������ֵ�-�г�����
	typedef char            TMarketCodeType;
	/// �ɶ���������
	typedef char			THoldernoType[HOLDERNO_LEN];
	/// ��Լ��������
	typedef char            TInstrumentIDType[INST_ID_LEN];
	/// ��ƽ��־���ͣ�����ֵ����������ֵ�-��ƽ��־����
	typedef char            TOffsetFlagType;
	/// �����������ͣ�����ֵ����������ֵ�-��������
	typedef char            TDirectionType;
	/// �м۵��������ͣ�����ֵ����������ֵ�-�м۵�����
	typedef char            TMarketOrderFlagType;
	/// ί�м۸�����
	typedef double          TPriceType;
	/// ��������
	typedef int				TVolumeType;
	/// ί����������
	typedef double          TLargeVolumeType;
	/// ί����������
	typedef int             TOrderPropType;
	/// ���Ա��/��������
	typedef int             TBatchNoType;
	/// ί����ˮ������
	typedef int             TOrderNoType;
	/// ί��״̬���ͣ�����ֵ����������ֵ�-ί��״̬
	typedef short           TOrderStatusType;
	/// ί��ʱ������
	typedef int             TTimeType;
	/// �ɽ���������
	typedef int             TDoneCountType;    
	/// �ɽ��������
	typedef int             TDoneNoType;    
	/// �����������ͣ�����ֵ����������ֵ�-��������
	typedef char            TCurrencyIDType;
	/// �ʽ���������
	typedef double          TAmountType;
	/// ��ѯ�˻��ʽ�����
	typedef int             TQueryAccountProp;
	/// ��ѯ�ֲ�����
	typedef int             TQueryPositionProp;    
	/// ��ѯί����������
	typedef int             TQueryOrderPropType;    
	/// ��Լ��������
	typedef char            TInstrumentNameType[INST_NAME_LEN];
	/// ��Լ״̬����
	typedef short           TSecStatusType;
	/// Ʒ�����ͣ�����ֵ����������ֵ�-Ʒ������
	typedef int             TProductTypeType;
	/// ETF�������
	typedef char            TEtfNameType[ETF_NAME_LEN];
	/// ��λ����
	typedef int             TUnitType;
	/// ��������
	typedef double          TRatioType;
	/// ������λ���ο�ֵ��־����
	typedef char            TPublishIOPVFlag;
	/// ETF״̬���ͣ�����ֵ����������ֵ�-ETF����״̬
	typedef char            TEtfStatusType;
	/// ���ӹ�Ʊ��������
	typedef int             TRecordNumType;
	/// �ֽ������־���ͣ�����ֵ����������ֵ�-�ֽ������־
	typedef char            TCashReplFlagType;
	/// ���ô����������
	typedef int             TFeeCodeType;
	/// ���ʼ��㷽ʽ�������
	typedef int             TFeeCalcType;
	/// ����ֵ�������
	typedef double          TFeeValueType;
	/// ���ʷֱʼ����־
	typedef char            TFeeDispartFlag;
	/// ת�����ͱ�־
	typedef char            TTransFundFlag[2];
	/// ǿƽ���
	typedef int				TForceFlag;
	/// �ɽ�����,����ֵ����������ֵ�
	typedef char			TDoneProp;
	/// ָ����
	typedef int				TInstructID;
	/// �ʽ��˻�����
	typedef int				TAccType;
	/// Ͷ���ױ���־
	typedef char			THedgeFlag;
	/// �ֲֶ������
	typedef char			TPosiDirectionType;
	/// ��ϲ��Դ���,����ֵ����������ֵ�
	typedef char			TStrategyCode[11];
	/// �������,����ֵ����������ֵ�
	typedef char			TCombType;
	/// ί�е�����,����ֵ����������ֵ�
	typedef char			TFutOrderType;
	/// ��Ϻ�Լ��������
	typedef char            TCombinInstrIDType[INST_COMBID_LEN];
	/// ��Ϻ�Լ����,����ֵ����������ֵ�
	typedef int			TFutCombFlag;
	/// ��Լ״̬����,����ֵ����������ֵ�
	typedef char			TContractStatus;
	/// �ɽ���״̬,����ֵ����������ֵ�
	typedef char			TContTradeStatus;
	/// ��Լ����
	typedef int				TContractIndex;
	/// ί�в�������
	typedef char			TStrategyName[41];
	/// ��ϵ�����
	typedef int				TLegsCnt;
	/// ��ϵ�������������
	typedef int				TLegRate;
	/// ��ϵ����ȼ������
	typedef int				TLegCalSign;
	/// �µ����Ժ�
	typedef int				TPolicyNo;
	/// ��������
	typedef char			TTradeType;
	/// ִ������
	typedef char			TStrikeModeType;
	/// ��Ȩ����
	typedef char			TOptionsTypeType;
	/// ������������
	typedef char             TOperDateType[9];
	/// ����ʱ������
	typedef char             TOperTimeType[9];
	/// ������������
	typedef char             TLockType;
	/// ��ͬ��
	typedef char             TContractType[20];
	/// �ɽ���
	typedef char             TTradeIDType[32];
	/// ��������
	typedef char             TQuoteType;
	/// �ͻ������ƺͰ汾��
	typedef char             TTerminalNameType[30];	
	/// �ʹ�ϵͳ����
	typedef char             TSystemNameType[22]; 
	/// trader_id ����
	typedef int              TTRADER_ID_TYPE; 
	/// ǰ�û�����id����
	typedef char             TFRONT_KEY_TYPE[12];
	/// ��Ȩ��
	typedef char             TLicenseCodeType[256];
	/// ����
	typedef int              TDomainNameType;
	/// ������ϵͳί����ˮ��
	typedef char             TMarketOrderIDType[32];
	/** @} */

	/**
	* \page page_dict �����ֵ�
	* \par 1. �г�����(֤ȯ/�ڻ�/�ƽ�/���)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'1'</td><td>�Ϻ�A��</td></tr>
	* <tr><td>'2'</td><td>����A��</td></tr>
	* <tr><td>'3'</td><td>�н���</td></tr>
	* <tr><td>'4'</td><td>������</td></tr>
	* <tr><td>'5'</td><td>֣����</td></tr>
	* <tr><td>'6'</td><td>������</td></tr>
	* <tr><td>'7'</td><td>�ƽ�����</td></tr>
	* <tr><td>'8'</td><td>��㽻������</td></tr>
	* </table>
	* @see TMarketCodeType
	* \par 2. ���Ҵ���(֤ȯ/�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'1'</td><td>�����</td></tr>
	* <tr><td>'2'</td><td>��Ԫ</td></tr>
	* <tr><td>'3'</td><td>�۱�</td></tr>
	* </table>
	* @see TCurrencyIDType
	* \par 3. ��������(֤ȯ/�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'0'</td><td>����</td></tr>
	* <tr><td>'1'</td><td>����</td></tr>
	* <tr><td>'2'</td><td>�깺</td></tr>
	* <tr><td>'3'</td><td>���</td></tr>
	* <tr><td>'4'</td><td>��Ѻ���</td></tr>
	* <tr><td>'5'</td><td>��Ѻ����</td></tr>
	* <tr><td>'6'</td><td>ת�й�</td></tr>
	* <tr><td>'7'</td><td>�ϲ�</td></tr>
	* <tr><td>'8'</td><td>�ֲ�</td></tr>
	* <tr><td>'A'</td><td>�ǹ�����ת��</td></tr>
	* <tr><td>'B'</td><td>�ǹ�����ת��</td></tr>
	* </table>
	* @see TDirectionType
	* \par 4. ��ƽ��־����(֤ȯ/�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'0'</td><td>����</td></tr>
	* <tr><td>'1'</td><td>ƽ��</td></tr>
	* <tr><td>'2'</td><td>ƽ��</td></tr>
	* </table>
	* @see TOffsetFlagType
	* \par 5. �м۵�����(֤ȯ)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'0'</td><td>��Ʊ�����г��޼�����</td></tr>
	* <tr><td>'1'</td><td>�����嵵������ί��</td></tr>
	* <tr><td>'2'</td><td>�����嵵��ת�޼�(�����֧��),</td></tr>
	* <tr><td>'3'</td><td>�Է����ż۸�(�Ͻ�����֧��)</td></tr>
	* <tr><td>'4'</td><td>�������ż۸�(�Ͻ�����֧��)</td></tr>
	* <tr><td>'5'</td><td>��ʱ�ɽ�ʣ�೷��(�Ͻ�����֧��)</td></tr>
	* <tr><td>'6'</td><td>ȫ��ɽ����� (�Ͻ�����֧��)</td></tr>
	* </table>
	* @see TMarketOrderFlagType
	* \par 6. ί��״̬(֤ȯ)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>0</td><td>δ��</td></tr>
	* <tr><td>1</td><td>����</td></tr>
	* <tr><td>2</td><td>�ѱ�</td></tr>
	* <tr><td>3</td><td>�ѱ�����</td></tr>
	* <tr><td>4</td><td>���ɴ���</td></tr>
	* <tr><td>5</td><td>����</td></tr>
	* <tr><td>6</td><td>���⳷��</td></tr>
	* <tr><td>7</td><td>����</td></tr>
	* <tr><td>8</td><td>�ѳ�</td></tr>
	* <tr><td>9</td><td>�ϵ�</td></tr>
	* <tr><td>10</td><td>���ڳ���</td></tr>
	* <tr><td>11</td><td>����δ��</td></tr>
	* <tr><td>12</td><td>��������</td></tr>
	* </table>
	* @see TOrderStatusType
	* \par 7. ETF����״̬(֤ȯ)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>0</td><td>����������깺�����</td></tr>
	* <tr><td>1</td><td>�����깺�����</td></tr>
	* <tr><td>2</td><td>�����깺�����������</td></tr>
	* <tr><td>3</td><td>�������깺���������ÿ�տ�ʼ���׺�����ı�</td></tr>
	* </table>
	* @see TEtfStatusType
	* \par 8. �ֽ������־(֤ȯ)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>0</td><td>��ֹ�ֽ�����������й�Ʊ��</td></tr>
	* <tr><td>1</td><td>���Խ����ֽ���������ù�Ʊ����Ʊ����Ļ����ֽ������</td></tr>
	* <tr><td>2</td><td>�������ֽ����</td></tr>
	* <tr><td>3</td><td>���г��˲��ֽ����</td></tr>
	* <tr><td>4</td><td>���г������ֽ����</td></tr>
	* <tr><td>5</td><td>�ǻ����˲��ֽ����</td></tr>
	* <tr><td>6</td><td>�ǻ�������ֽ����</td></tr>
	* </table>
	* @see TCashReplFlagType
	* \par 9. Ʒ������(֤ȯ)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>9</td><td>��ծ</td></tr>
	* <tr><td>10</td><td>�ط�ծ</td></tr>
	* <tr><td>11</td><td>��˾ծ</td></tr>
	* <tr><td>12</td><td>��ҵծ</td></tr>
	* <tr><td>13</td><td>���뽻�׵Ŀ�ת����˾ծ</td></tr>
	* <tr><td>14</td><td>��תծ</td></tr>
	* <tr><td>15</td><td>�ʲ�֤ȯ��</td></tr>
	* <tr><td>23</td><td>��ծ��Ѻ</td></tr>
	* <tr><td>24</td><td>�ط�ծ��Ѻ</td></tr>
	* <tr><td>25</td><td>��˾ծ��Ѻ</td></tr>
	* <tr><td>26</td><td>��ҵծ��Ѻ</td></tr>
	* <tr><td>27</td><td>�ɷ���תծ��Ѻ</td></tr>
	* <tr><td>30</td><td>�˻�ʽ�ع�</td></tr>
	* <tr><td>42</td><td>�����Ʊ����</td></tr>
	* <tr><td>43</td><td>��С���Ʊ����</td></tr>
	* <tr><td>44</td><td>��ҵ���Ʊ����</td></tr>
	* <tr><td>56</td><td>ETF������</td></tr>
	* <tr><td>57</td><td>ETF��������</td></tr>
	* <tr><td>71</td><td>˽ļծ</td></tr>
	* </table>
	* @see TProductTypeType
	* \par 10. ���ô���(֤ȯ)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>1</td><td>Ӷ��</td></tr>
	* <tr><td>2</td><td>������</td></tr>
	* <tr><td>3</td><td>ӡ��˰</td></tr>
	* <tr><td>4</td><td>���׹��</td></tr>
	* <tr><td>5</td><td>�����</td></tr>
	* <tr><td>6</td><td>��������</td></tr>
	* </table>
	* @see TFeeCodeType
	* \par 11. ���ʼ��㷽ʽ(֤ȯ)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>1</td><td>�ɽ����</td></tr>
	* <tr><td>2</td><td>�ɽ���ֵ</td></tr>
	* <tr><td>3</td><td>�걨����</td></tr>
	* <tr><td>4</td><td>�ɽ�����</td></tr>
	* <tr><td>5</td><td>�ɽ�����</td></tr>
	* </table>
	* @see TFeeCalcType
	* \par 12. ���ʷֱʼ����־(֤ȯ)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>0</td><td>���ֱ�</td></tr>
	* <tr><td>1</td><td>�ֱ�</td></tr>
	* </table>
	* @see TFeeDispartFlag
	* \par 13. ��������(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'0'</td><td>�޼�</td></tr>
	* <tr><td>'1'</td><td>�м�</td></tr>
	* <tr><td>'2'</td><td>���ż�</td></tr>
	* <tr><td>'3'</td><td>�м�ֹ��</td></tr>
	* <tr><td>'4'</td><td>�м�ֹӮ</td></tr>
	* <tr><td>'5'</td><td>�޼�ֹ��</td></tr>
	* <tr><td>'6'</td><td>�޼�ֹӮ</td></tr>
	* <tr><td>'7'</td><td>���ż�(���ַ����ţ�</td></tr>
	* <tr><td>'8'</td><td>�м������嵵</td></tr>
	* <tr><td>'K'</td><td>�м�תʣ���޼�</td></tr>
	* </table>
	* @see TMarketOrderFlagType
	* \par 14. Ͷ���ױ���־(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'0'</td><td>Ͷ��/�Ǳ���</td></tr>
	* <tr><td>'1'</td><td>��ֵ/����</td></tr>
	* <tr><td>'2'</td><td>����</td></tr>
	* </table>
	* @see THedgingFlag
	* \par 15. �������(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'0'</td><td>��ͨ���</td></tr>
	* <tr><td>'1'</td><td>�������</td></tr>
	* </table>
	* @see TCombType
	* \par 16. ��ϲ��Դ���(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>SP</td><td>�ڻ�����</td></tr>
	* <tr><td>SPC</td><td>�ڻ���Ʒ��</td></tr>
	* <tr><td>SPX</td><td>�ڻ�ѹե</td></tr>
	* <tr><td>SPD</td><td>�ڻ�����</td></tr>
	* <tr><td>IPS</td><td>�ڻ���Ʒ��</td></tr>
	* </table>
	* @see TStrategyCode
	* \par 17. ί�е�����(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>A</td><td>��ͨί��</td></tr>
	* <tr><td>B</td><td>����ί��</td></tr>
	* <tr><td>D</td><td>Ԥ����ͨί��</td></tr>
	* <tr><td>E</td><td>Ԥ������ί��</td></tr>
	* <tr><td>J</td><td>���ί��</td></tr>
	* <tr><td>K</td><td>�������ί��</td></tr>
	* <tr><td>L</td><td>Ԥ�����ί��</td></tr>
	* <tr><td>M</td><td>Ԥ���������ί��</td></tr>
	* <tr><td>1</td><td>��Ȩί��</td></tr>
	* </table>
	* @see TFutOrderType
	* \par 18. �ɽ�����(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'0'</td><td>GFD������Ч</td></tr>
	* <tr><td>'1'</td><td>FOKȫ�ɻ�ȫ��</td></tr>
	* <tr><td>'2'</td><td>FAKʣ�༴����</td></tr>
	* <tr><td>'3'</td><td>IOC��ʱ�ɽ�ʣ���Զ�����</td></tr>
	* <tr><td>'4'</td><td>JFOKʣ�༴����</td></tr>
	* <tr><td>'5'</td><td>ʣ��ת�޼�</td></tr>
	* </table>
	* @see TDoneProp
	* \par 19. ί��״̬(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>0</td><td>δ��</td></tr>
	* <tr><td>1</td><td>����</td></tr>
	* <tr><td>2</td><td>�ѱ�</td></tr>
	* <tr><td>3</td><td>�ѱ�����</td></tr>
	* <tr><td>4</td><td>���ɴ���</td></tr>
	* <tr><td>5</td><td>����</td></tr>
	* <tr><td>6</td><td>���⳷��</td></tr>
	* <tr><td>7</td><td>����</td></tr>
	* <tr><td>8</td><td>�ѳ�</td></tr>
	* <tr><td>9</td><td>�ϵ�</td></tr>
	* </table>
	* @see TOrderStatusType
	* \par 20. ��Լ״̬����(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'0'</td><td>����</td></tr>
	* <tr><td>'1'</td><td>ֻ��ƽ��</td></tr>
	* <tr><td>'2'</td><td>ͣ��</td></tr>
	* </table>
	* @see TContractStatus
	* \par 21. ��������(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'0'</td><td>֤ȯ</td></tr>
	* <tr><td>'1'</td><td>�ڻ�</td></tr>
	* <tr><td>'2'</td><td>�ڻ���Ȩ</td></tr>
	* <tr><td>'3'</td><td>������Ȩ</td></tr>
	* </table>
	* @see TTradeType
	* \par 22. ִ������(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'0'</td><td>��ʽ</td></tr>
	* <tr><td>'1'</td><td>ŷʽ</td></tr>
	* </table>
	* @see TStrikeModeType
	* \par 23. ��Ȩ����(�ڻ�)
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'C'</td><td>����</td></tr>
	* <tr><td>'P'</td><td>����</td></tr>
	* </table>
	* @see TOptionsTypeType
	* \par 24. ������������
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'3'</td><td>����</td></tr>
	* <tr><td>'4'</td><td>����</td></tr>
	* </table>
	* @see TLockType
	* \par 25. ��������
	* <table>
	* <tr><td>����</td><td>����</td></tr>
	* <tr><td>'1'</td><td>�ɽ��׵ı���</td></tr>
	* </table>
	* @see TQuoteType
	*/

#ifdef __cplusplus
}
#endif

#endif //__KFTS_TRADE_TYPE_H__

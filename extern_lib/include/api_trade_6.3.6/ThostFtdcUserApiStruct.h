/////////////////////////////////////////////////////////////////////////
///@system ��һ��������ϵͳ
///@company �Ϻ��ڻ���Ϣ�������޹�˾
///@file ThostFtdcUserApiStruct.h
///@brief �����˿ͻ��˽ӿ�ʹ�õ�ҵ�����ݽṹ
///@history 
///20060106	�Ժ��		�������ļ�
/////////////////////////////////////////////////////////////////////////

#if !defined(THOST_FTDCSTRUCT_H)
#define THOST_FTDCSTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThostFtdcUserApiDataType.h"

///��Ϣ�ַ�
struct CThostFtdcDisseminationField
{
	///����ϵ�к�
	TThostFtdcSequenceSeriesType	SequenceSeries;
	///���к�
	int	SequenceNo;
};

///�û���¼����
struct CThostFtdcReqUserLoginField
{
	///������
	TThostFtdcDateType	TradingDay;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����
	TThostFtdcPasswordType	Password;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///�ӿڶ˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	InterfaceProductInfo;
	///Э����Ϣ
	TThostFtdcProtocolInfoType	ProtocolInfo;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
	///��̬����
	TThostFtdcPasswordType	OneTimePassword;
	///�ն�IP��ַ
	TThostFtdcIPAddressType	ClientIPAddress;
	///��¼��ע
	TThostFtdcLoginRemarkType	LoginRemark;
};

///�û���¼Ӧ��
struct CThostFtdcRspUserLoginField
{
	///������
	TThostFtdcDateType	TradingDay;
	///��¼�ɹ�ʱ��
	TThostFtdcTimeType	LoginTime;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����ϵͳ����
	TThostFtdcSystemNameType	SystemName;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///��󱨵�����
	TThostFtdcOrderRefType	MaxOrderRef;
	///������ʱ��
	TThostFtdcTimeType	SHFETime;
	///������ʱ��
	TThostFtdcTimeType	DCETime;
	///֣����ʱ��
	TThostFtdcTimeType	CZCETime;
	///�н���ʱ��
	TThostFtdcTimeType	FFEXTime;
	///��Դ����ʱ��
	TThostFtdcTimeType	INETime;
};

///�û��ǳ�����
struct CThostFtdcUserLogoutField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
};

///ǿ�ƽ���Ա�˳�
struct CThostFtdcForceUserLogoutField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
};

///�ͻ�����֤����
struct CThostFtdcReqAuthenticateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///��֤��
	TThostFtdcAuthCodeType	AuthCode;
};

///�ͻ�����֤��Ӧ
struct CThostFtdcRspAuthenticateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
};

///�ͻ�����֤��Ϣ
struct CThostFtdcAuthenticationInfoField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///��֤��Ϣ
	TThostFtdcAuthInfoType	AuthInfo;
	///�Ƿ�Ϊ��֤���
	int	IsResult;
};

///����ת�ʱ���ͷ
struct CThostFtdcTransferHeaderField
{
	///�汾�ţ�������1.0
	TThostFtdcVersionType	Version;
	///���״��룬����
	TThostFtdcTradeCodeType	TradeCode;
	///�������ڣ������ʽ��yyyymmdd
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ�䣬�����ʽ��hhmmss
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ�ţ�N/A
	TThostFtdcTradeSerialType	TradeSerial;
	///�ڻ���˾���룬����
	TThostFtdcFutureIDType	FutureID;
	///���д��룬���ݲ�ѯ���еõ�������
	TThostFtdcBankIDType	BankID;
	///���з����Ĵ��룬���ݲ�ѯ���еõ�������
	TThostFtdcBankBrchIDType	BankBrchID;
	///����Ա��N/A
	TThostFtdcOperNoType	OperNo;
	///�����豸���ͣ�N/A
	TThostFtdcDeviceIDType	DeviceID;
	///��¼����N/A
	TThostFtdcRecordNumType	RecordNum;
	///�Ự��ţ�N/A
	int	SessionID;
	///�����ţ�N/A
	int	RequestID;
};

///�����ʽ�ת�ڻ�����TradeCode=202001
struct CThostFtdcTransferBankToFutureReqField
{
	///�ڻ��ʽ��˻�
	TThostFtdcAccountIDType	FutureAccount;
	///�����־
	char	FuturePwdFlag;
	///����
	TThostFtdcFutureAccPwdType	FutureAccPwd;
	///ת�˽��
	double	TradeAmt;
	///�ͻ�������
	double	CustFee;
	///���֣�RMB-����� USD-��Բ HKD-��Ԫ
	TThostFtdcCurrencyCodeType	CurrencyCode;
};

///�����ʽ�ת�ڻ�������Ӧ
struct CThostFtdcTransferBankToFutureRspField
{
	///��Ӧ����
	TThostFtdcRetCodeType	RetCode;
	///��Ӧ��Ϣ
	TThostFtdcRetInfoType	RetInfo;
	///�ʽ��˻�
	TThostFtdcAccountIDType	FutureAccount;
	///ת�ʽ��
	double	TradeAmt;
	///Ӧ�տͻ�������
	double	CustFee;
	///����
	TThostFtdcCurrencyCodeType	CurrencyCode;
};

///�ڻ��ʽ�ת��������TradeCode=202002
struct CThostFtdcTransferFutureToBankReqField
{
	///�ڻ��ʽ��˻�
	TThostFtdcAccountIDType	FutureAccount;
	///�����־
	char	FuturePwdFlag;
	///����
	TThostFtdcFutureAccPwdType	FutureAccPwd;
	///ת�˽��
	double	TradeAmt;
	///�ͻ�������
	double	CustFee;
	///���֣�RMB-����� USD-��Բ HKD-��Ԫ
	TThostFtdcCurrencyCodeType	CurrencyCode;
};

///�ڻ��ʽ�ת����������Ӧ
struct CThostFtdcTransferFutureToBankRspField
{
	///��Ӧ����
	TThostFtdcRetCodeType	RetCode;
	///��Ӧ��Ϣ
	TThostFtdcRetInfoType	RetInfo;
	///�ʽ��˻�
	TThostFtdcAccountIDType	FutureAccount;
	///ת�ʽ��
	double	TradeAmt;
	///Ӧ�տͻ�������
	double	CustFee;
	///����
	TThostFtdcCurrencyCodeType	CurrencyCode;
};

///��ѯ�����ʽ�����TradeCode=204002
struct CThostFtdcTransferQryBankReqField
{
	///�ڻ��ʽ��˻�
	TThostFtdcAccountIDType	FutureAccount;
	///�����־
	char	FuturePwdFlag;
	///����
	TThostFtdcFutureAccPwdType	FutureAccPwd;
	///���֣�RMB-����� USD-��Բ HKD-��Ԫ
	TThostFtdcCurrencyCodeType	CurrencyCode;
};

///��ѯ�����ʽ�������Ӧ
struct CThostFtdcTransferQryBankRspField
{
	///��Ӧ����
	TThostFtdcRetCodeType	RetCode;
	///��Ӧ��Ϣ
	TThostFtdcRetInfoType	RetInfo;
	///�ʽ��˻�
	TThostFtdcAccountIDType	FutureAccount;
	///�������
	double	TradeAmt;
	///���п������
	double	UseAmt;
	///���п�ȡ���
	double	FetchAmt;
	///����
	TThostFtdcCurrencyCodeType	CurrencyCode;
};

///��ѯ���н�����ϸ����TradeCode=204999
struct CThostFtdcTransferQryDetailReqField
{
	///�ڻ��ʽ��˻�
	TThostFtdcAccountIDType	FutureAccount;
};

///��ѯ���н�����ϸ������Ӧ
struct CThostFtdcTransferQryDetailRspField
{
	///��������
	TThostFtdcDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///���״���
	TThostFtdcTradeCodeType	TradeCode;
	///�ڻ���ˮ��
	int	FutureSerial;
	///�ڻ���˾����
	TThostFtdcFutureIDType	FutureID;
	///�ʽ��ʺ�
	TThostFtdcFutureAccountType	FutureAccount;
	///������ˮ��
	int	BankSerial;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з����Ĵ���
	TThostFtdcBankBrchIDType	BankBrchID;
	///�����˺�
	TThostFtdcBankAccountType	BankAccount;
	///֤������
	TThostFtdcCertCodeType	CertCode;
	///���Ҵ���
	TThostFtdcCurrencyCodeType	CurrencyCode;
	///�������
	double	TxAmount;
	///��Ч��־
	char	Flag;
};

///��Ӧ��Ϣ
struct CThostFtdcRspInfoField
{
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///������
struct CThostFtdcExchangeField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������
	TThostFtdcExchangeNameType	ExchangeName;
	///����������
	char	ExchangeProperty;
};

///��Ʒ
struct CThostFtdcProductField
{
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
	///��Ʒ����
	TThostFtdcProductNameType	ProductName;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ʒ����
	char	ProductClass;
	///��Լ��������
	int	VolumeMultiple;
	///��С�䶯��λ
	double	PriceTick;
	///�м۵�����µ���
	int	MaxMarketOrderVolume;
	///�м۵���С�µ���
	int	MinMarketOrderVolume;
	///�޼۵�����µ���
	int	MaxLimitOrderVolume;
	///�޼۵���С�µ���
	int	MinLimitOrderVolume;
	///�ֲ�����
	char	PositionType;
	///�ֲ���������
	char	PositionDateType;
	///ƽ�ִ�������
	char	CloseDealType;
	///���ױ�������
	TThostFtdcCurrencyIDType	TradeCurrencyID;
	///��Ѻ�ʽ���÷�Χ
	char	MortgageFundUseRange;
	///��������Ʒ����
	TThostFtdcInstrumentIDType	ExchangeProductID;
	///��Լ������Ʒ����
	double	UnderlyingMultiple;
};

///��Լ
struct CThostFtdcInstrumentField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ����
	TThostFtdcInstrumentNameType	InstrumentName;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
	///��Ʒ����
	char	ProductClass;
	///�������
	int	DeliveryYear;
	///������
	int	DeliveryMonth;
	///�м۵�����µ���
	int	MaxMarketOrderVolume;
	///�м۵���С�µ���
	int	MinMarketOrderVolume;
	///�޼۵�����µ���
	int	MaxLimitOrderVolume;
	///�޼۵���С�µ���
	int	MinLimitOrderVolume;
	///��Լ��������
	int	VolumeMultiple;
	///��С�䶯��λ
	double	PriceTick;
	///������
	TThostFtdcDateType	CreateDate;
	///������
	TThostFtdcDateType	OpenDate;
	///������
	TThostFtdcDateType	ExpireDate;
	///��ʼ������
	TThostFtdcDateType	StartDelivDate;
	///����������
	TThostFtdcDateType	EndDelivDate;
	///��Լ��������״̬
	char	InstLifePhase;
	///��ǰ�Ƿ���
	int	IsTrading;
	///�ֲ�����
	char	PositionType;
	///�ֲ���������
	char	PositionDateType;
	///��ͷ��֤����
	double	LongMarginRatio;
	///��ͷ��֤����
	double	ShortMarginRatio;
	///�Ƿ�ʹ�ô��߱�֤���㷨
	char	MaxMarginSideAlgorithm;
	///������Ʒ����
	TThostFtdcInstrumentIDType	UnderlyingInstrID;
	///ִ�м�
	double	StrikePrice;
	///��Ȩ����
	char	OptionsType;
	///��Լ������Ʒ����
	double	UnderlyingMultiple;
	///�������
	char	CombinationType;
};

///���͹�˾
struct CThostFtdcBrokerField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///���͹�˾���
	TThostFtdcBrokerAbbrType	BrokerAbbr;
	///���͹�˾����
	TThostFtdcBrokerNameType	BrokerName;
	///�Ƿ��Ծ
	int	IsActive;
};

///����������Ա
struct CThostFtdcTraderField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///����
	TThostFtdcPasswordType	Password;
	///��װ����
	int	InstallCount;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
};

///Ͷ����
struct CThostFtdcInvestorField
{
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���߷������
	TThostFtdcInvestorIDType	InvestorGroupID;
	///Ͷ��������
	TThostFtdcPartyNameType	InvestorName;
	///֤������
	char	IdentifiedCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�Ƿ��Ծ
	int	IsActive;
	///��ϵ�绰
	TThostFtdcTelephoneType	Telephone;
	///ͨѶ��ַ
	TThostFtdcAddressType	Address;
	///��������
	TThostFtdcDateType	OpenDate;
	///�ֻ�
	TThostFtdcMobileType	Mobile;
	///��������ģ�����
	TThostFtdcInvestorIDType	CommModelID;
	///��֤����ģ�����
	TThostFtdcInvestorIDType	MarginModelID;
};

///���ױ���
struct CThostFtdcTradingCodeField
{
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///�Ƿ��Ծ
	int	IsActive;
	///���ױ�������
	char	ClientIDType;
};

///��Ա����;��͹�˾������ձ�
struct CThostFtdcPartBrokerField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�Ƿ��Ծ
	int	IsActive;
};

///�����û�
struct CThostFtdcSuperUserField
{
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�û�����
	TThostFtdcUserNameType	UserName;
	///����
	TThostFtdcPasswordType	Password;
	///�Ƿ��Ծ
	int	IsActive;
};

///�����û�����Ȩ��
struct CThostFtdcSuperUserFunctionField
{
	///�û�����
	TThostFtdcUserIDType	UserID;
	///���ܴ���
	char	FunctionCode;
};

///Ͷ������
struct CThostFtdcInvestorGroupField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���߷������
	TThostFtdcInvestorIDType	InvestorGroupID;
	///Ͷ���߷�������
	TThostFtdcInvestorGroupNameType	InvestorGroupName;
};

///�ʽ��˻�
struct CThostFtdcTradingAccountField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ϴ���Ѻ���
	double	PreMortgage;
	///�ϴ����ö��
	double	PreCredit;
	///�ϴδ���
	double	PreDeposit;
	///�ϴν���׼����
	double	PreBalance;
	///�ϴ�ռ�õı�֤��
	double	PreMargin;
	///��Ϣ����
	double	InterestBase;
	///��Ϣ����
	double	Interest;
	///�����
	double	Deposit;
	///������
	double	Withdraw;
	///����ı�֤��
	double	FrozenMargin;
	///������ʽ�
	double	FrozenCash;
	///�����������
	double	FrozenCommission;
	///��ǰ��֤���ܶ�
	double	CurrMargin;
	///�ʽ���
	double	CashIn;
	///������
	double	Commission;
	///ƽ��ӯ��
	double	CloseProfit;
	///�ֲ�ӯ��
	double	PositionProfit;
	///�ڻ�����׼����
	double	Balance;
	///�����ʽ�
	double	Available;
	///��ȡ�ʽ�
	double	WithdrawQuota;
	///����׼����
	double	Reserve;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///���ö��
	double	Credit;
	///��Ѻ���
	double	Mortgage;
	///��������֤��
	double	ExchangeMargin;
	///Ͷ���߽��֤��
	double	DeliveryMargin;
	///���������֤��
	double	ExchangeDeliveryMargin;
	///�����ڻ�����׼����
	double	ReserveBalance;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///�ϴλ���������
	double	PreFundMortgageIn;
	///�ϴλ����ʳ����
	double	PreFundMortgageOut;
	///����������
	double	FundMortgageIn;
	///�����ʳ����
	double	FundMortgageOut;
	///������Ѻ���
	double	FundMortgageAvailable;
	///����Ѻ���ҽ��
	double	MortgageableFund;
	///�����Ʒռ�ñ�֤��
	double	SpecProductMargin;
	///�����Ʒ���ᱣ֤��
	double	SpecProductFrozenMargin;
	///�����Ʒ������
	double	SpecProductCommission;
	///�����Ʒ����������
	double	SpecProductFrozenCommission;
	///�����Ʒ�ֲ�ӯ��
	double	SpecProductPositionProfit;
	///�����Ʒƽ��ӯ��
	double	SpecProductCloseProfit;
	///���ݳֲ�ӯ���㷨����������Ʒ�ֲ�ӯ��
	double	SpecProductPositionProfitByAlg;
	///�����Ʒ��������֤��
	double	SpecProductExchangeMargin;
};

///Ͷ���ֲ߳�
struct CThostFtdcInvestorPositionField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///�ֲֶ�շ���
	char	PosiDirection;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///�ֲ�����
	char	PositionDate;
	///���ճֲ�
	int	YdPosition;
	///���ճֲ�
	int	Position;
	///��ͷ����
	int	LongFrozen;
	///��ͷ����
	int	ShortFrozen;
	///���ֶ�����
	double	LongFrozenAmount;
	///���ֶ�����
	double	ShortFrozenAmount;
	///������
	int	OpenVolume;
	///ƽ����
	int	CloseVolume;
	///���ֽ��
	double	OpenAmount;
	///ƽ�ֽ��
	double	CloseAmount;
	///�ֲֳɱ�
	double	PositionCost;
	///�ϴ�ռ�õı�֤��
	double	PreMargin;
	///ռ�õı�֤��
	double	UseMargin;
	///����ı�֤��
	double	FrozenMargin;
	///������ʽ�
	double	FrozenCash;
	///�����������
	double	FrozenCommission;
	///�ʽ���
	double	CashIn;
	///������
	double	Commission;
	///ƽ��ӯ��
	double	CloseProfit;
	///�ֲ�ӯ��
	double	PositionProfit;
	///�ϴν����
	double	PreSettlementPrice;
	///���ν����
	double	SettlementPrice;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///���ֳɱ�
	double	OpenCost;
	///��������֤��
	double	ExchangeMargin;
	///��ϳɽ��γɵĳֲ�
	int	CombPosition;
	///��϶�ͷ����
	int	CombLongFrozen;
	///��Ͽ�ͷ����
	int	CombShortFrozen;
	///���ն���ƽ��ӯ��
	double	CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	double	CloseProfitByTrade;
	///���ճֲ�
	int	TodayPosition;
	///��֤����
	double	MarginRateByMoney;
	///��֤����(������)
	double	MarginRateByVolume;
	///ִ�ж���
	int	StrikeFrozen;
	///ִ�ж�����
	double	StrikeFrozenAmount;
	///����ִ�ж���
	int	AbandonFrozen;
};

///��Լ��֤����
struct CThostFtdcInstrumentMarginRateField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///��ͷ��֤����
	double	LongMarginRatioByMoney;
	///��ͷ��֤���
	double	LongMarginRatioByVolume;
	///��ͷ��֤����
	double	ShortMarginRatioByMoney;
	///��ͷ��֤���
	double	ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	int	IsRelative;
};

///��Լ��������
struct CThostFtdcInstrumentCommissionRateField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	double	OpenRatioByMoney;
	///����������
	double	OpenRatioByVolume;
	///ƽ����������
	double	CloseRatioByMoney;
	///ƽ��������
	double	CloseRatioByVolume;
	///ƽ����������
	double	CloseTodayRatioByMoney;
	///ƽ��������
	double	CloseTodayRatioByVolume;
};

///�������
struct CThostFtdcDepthMarketDataField
{
	///������
	TThostFtdcDateType	TradingDay;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///���¼�
	double	LastPrice;
	///�ϴν����
	double	PreSettlementPrice;
	///������
	double	PreClosePrice;
	///��ֲ���
	double	PreOpenInterest;
	///����
	double	OpenPrice;
	///��߼�
	double	HighestPrice;
	///��ͼ�
	double	LowestPrice;
	///����
	int	Volume;
	///�ɽ����
	double	Turnover;
	///�ֲ���
	double	OpenInterest;
	///������
	double	ClosePrice;
	///���ν����
	double	SettlementPrice;
	///��ͣ���
	double	UpperLimitPrice;
	///��ͣ���
	double	LowerLimitPrice;
	///����ʵ��
	double	PreDelta;
	///����ʵ��
	double	CurrDelta;
	///����޸�ʱ��
	TThostFtdcTimeType	UpdateTime;
	///����޸ĺ���
	int	UpdateMillisec;
	///�����һ
	double	BidPrice1;
	///������һ
	int	BidVolume1;
	///������һ
	double	AskPrice1;
	///������һ
	int	AskVolume1;
	///����۶�
	double	BidPrice2;
	///��������
	int	BidVolume2;
	///�����۶�
	double	AskPrice2;
	///��������
	int	AskVolume2;
	///�������
	double	BidPrice3;
	///��������
	int	BidVolume3;
	///��������
	double	AskPrice3;
	///��������
	int	AskVolume3;
	///�������
	double	BidPrice4;
	///��������
	int	BidVolume4;
	///��������
	double	AskPrice4;
	///��������
	int	AskVolume4;
	///�������
	double	BidPrice5;
	///��������
	int	BidVolume5;
	///��������
	double	AskPrice5;
	///��������
	int	AskVolume5;
	///���վ���
	double	AveragePrice;
	///ҵ������
	TThostFtdcDateType	ActionDay;
};

///Ͷ���ߺ�Լ����Ȩ��
struct CThostFtdcInstrumentTradingRightField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����Ȩ��
	char	TradingRight;
};

///���͹�˾�û�
struct CThostFtdcBrokerUserField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�û�����
	TThostFtdcUserNameType	UserName;
	///�û�����
	char	UserType;
	///�Ƿ��Ծ
	int	IsActive;
	///�Ƿ�ʹ������
	int	IsUsingOTP;
};

///���͹�˾�û�����
struct CThostFtdcBrokerUserPasswordField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����
	TThostFtdcPasswordType	Password;
};

///���͹�˾�û�����Ȩ��
struct CThostFtdcBrokerUserFunctionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///���͹�˾���ܴ���
	char	BrokerFunctionCode;
};

///����������Ա���̻�
struct CThostFtdcTraderOfferField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///����
	TThostFtdcPasswordType	Password;
	///��װ���
	int	InstallID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///����������Ա����״̬
	char	TraderConnectStatus;
	///�����������������
	TThostFtdcDateType	ConnectRequestDate;
	///�������������ʱ��
	TThostFtdcTimeType	ConnectRequestTime;
	///�ϴα�������
	TThostFtdcDateType	LastReportDate;
	///�ϴα���ʱ��
	TThostFtdcTimeType	LastReportTime;
	///�����������
	TThostFtdcDateType	ConnectDate;
	///�������ʱ��
	TThostFtdcTimeType	ConnectTime;
	///��������
	TThostFtdcDateType	StartDate;
	///����ʱ��
	TThostFtdcTimeType	StartTime;
	///������
	TThostFtdcDateType	TradingDay;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��ϯλ���ɽ����
	TThostFtdcTradeIDType	MaxTradeID;
	///��ϯλ��󱨵�����
	TThostFtdcReturnCodeType	MaxOrderMessageReference;
};

///Ͷ���߽�����
struct CThostFtdcSettlementInfoField
{
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///���
	int	SequenceNo;
	///��Ϣ����
	TThostFtdcContentType	Content;
};

///��Լ��֤���ʵ���
struct CThostFtdcInstrumentMarginRateAdjustField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///��ͷ��֤����
	double	LongMarginRatioByMoney;
	///��ͷ��֤���
	double	LongMarginRatioByVolume;
	///��ͷ��֤����
	double	ShortMarginRatioByMoney;
	///��ͷ��֤���
	double	ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	int	IsRelative;
};

///��������֤����
struct CThostFtdcExchangeMarginRateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///��ͷ��֤����
	double	LongMarginRatioByMoney;
	///��ͷ��֤���
	double	LongMarginRatioByVolume;
	///��ͷ��֤����
	double	ShortMarginRatioByMoney;
	///��ͷ��֤���
	double	ShortMarginRatioByVolume;
};

///��������֤���ʵ���
struct CThostFtdcExchangeMarginRateAdjustField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///���潻����Ͷ���߶�ͷ��֤����
	double	LongMarginRatioByMoney;
	///���潻����Ͷ���߶�ͷ��֤���
	double	LongMarginRatioByVolume;
	///���潻����Ͷ���߿�ͷ��֤����
	double	ShortMarginRatioByMoney;
	///���潻����Ͷ���߿�ͷ��֤���
	double	ShortMarginRatioByVolume;
	///��������ͷ��֤����
	double	ExchLongMarginRatioByMoney;
	///��������ͷ��֤���
	double	ExchLongMarginRatioByVolume;
	///��������ͷ��֤����
	double	ExchShortMarginRatioByMoney;
	///��������ͷ��֤���
	double	ExchShortMarginRatioByVolume;
	///�����潻����Ͷ���߶�ͷ��֤����
	double	NoLongMarginRatioByMoney;
	///�����潻����Ͷ���߶�ͷ��֤���
	double	NoLongMarginRatioByVolume;
	///�����潻����Ͷ���߿�ͷ��֤����
	double	NoShortMarginRatioByMoney;
	///�����潻����Ͷ���߿�ͷ��֤���
	double	NoShortMarginRatioByVolume;
};

///����
struct CThostFtdcExchangeRateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Դ����
	TThostFtdcCurrencyIDType	FromCurrencyID;
	///Դ���ֵ�λ����
	double	FromCurrencyUnit;
	///Ŀ�����
	TThostFtdcCurrencyIDType	ToCurrencyID;
	///����
	double	ExchangeRate;
};

///��������
struct CThostFtdcSettlementRefField
{
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
};

///��ǰʱ��
struct CThostFtdcCurrentTimeField
{
	///��ǰ����
	TThostFtdcDateType	CurrDate;
	///��ǰʱ��
	TThostFtdcTimeType	CurrTime;
	///��ǰʱ�䣨���룩
	int	CurrMillisec;
	///ҵ������
	TThostFtdcDateType	ActionDay;
};

///ͨѶ�׶�
struct CThostFtdcCommPhaseField
{
	///������
	TThostFtdcDateType	TradingDay;
	///ͨѶʱ�α��
	TThostFtdcCommPhaseNoType	CommPhaseNo;
	///ϵͳ���
	TThostFtdcSystemIDType	SystemID;
};

///��¼��Ϣ
struct CThostFtdcLoginInfoField
{
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��¼����
	TThostFtdcDateType	LoginDate;
	///��¼ʱ��
	TThostFtdcTimeType	LoginTime;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///�ӿڶ˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	InterfaceProductInfo;
	///Э����Ϣ
	TThostFtdcProtocolInfoType	ProtocolInfo;
	///ϵͳ����
	TThostFtdcSystemNameType	SystemName;
	///����
	TThostFtdcPasswordType	Password;
	///��󱨵�����
	TThostFtdcOrderRefType	MaxOrderRef;
	///������ʱ��
	TThostFtdcTimeType	SHFETime;
	///������ʱ��
	TThostFtdcTimeType	DCETime;
	///֣����ʱ��
	TThostFtdcTimeType	CZCETime;
	///�н���ʱ��
	TThostFtdcTimeType	FFEXTime;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
	///��̬����
	TThostFtdcPasswordType	OneTimePassword;
	///��Դ����ʱ��
	TThostFtdcTimeType	INETime;
	///��ѯʱ�Ƿ���Ҫ����
	int	IsQryControl;
	///��¼��ע
	TThostFtdcLoginRemarkType	LoginRemark;
};

///��¼��Ϣ
struct CThostFtdcLogoutAllField
{
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///ϵͳ����
	TThostFtdcSystemNameType	SystemName;
};

///ǰ��״̬
struct CThostFtdcFrontStatusField
{
	///ǰ�ñ��
	int	FrontID;
	///�ϴα�������
	TThostFtdcDateType	LastReportDate;
	///�ϴα���ʱ��
	TThostFtdcTimeType	LastReportTime;
	///�Ƿ��Ծ
	int	IsActive;
};

///�û�������
struct CThostFtdcUserPasswordUpdateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///ԭ���Ŀ���
	TThostFtdcPasswordType	OldPassword;
	///�µĿ���
	TThostFtdcPasswordType	NewPassword;
};

///���뱨��
struct CThostFtdcInputOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�����۸�����
	char	OrderPriceType;
	///��������
	char	Direction;
	///��Ͽ�ƽ��־
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	double	LimitPrice;
	///����
	int	VolumeTotalOriginal;
	///��Ч������
	char	TimeCondition;
	///GTD����
	TThostFtdcDateType	GTDDate;
	///�ɽ�������
	char	VolumeCondition;
	///��С�ɽ���
	int	MinVolume;
	///��������
	char	ContingentCondition;
	///ֹ���
	double	StopPrice;
	///ǿƽԭ��
	char	ForceCloseReason;
	///�Զ������־
	int	IsAutoSuspend;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	int	RequestID;
	///�û�ǿ����־
	int	UserForceClose;
	///��������־
	int	IsSwapOrder;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///�ʽ��˺�
	TThostFtdcAccountIDType	AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///���ױ���
	TThostFtdcClientIDType	ClientID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///����
struct CThostFtdcOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�����۸�����
	char	OrderPriceType;
	///��������
	char	Direction;
	///��Ͽ�ƽ��־
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	double	LimitPrice;
	///����
	int	VolumeTotalOriginal;
	///��Ч������
	char	TimeCondition;
	///GTD����
	TThostFtdcDateType	GTDDate;
	///�ɽ�������
	char	VolumeCondition;
	///��С�ɽ���
	int	MinVolume;
	///��������
	char	ContingentCondition;
	///ֹ���
	double	StopPrice;
	///ǿƽԭ��
	char	ForceCloseReason;
	///�Զ������־
	int	IsAutoSuspend;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	int	RequestID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///�����ύ״̬
	char	OrderSubmitStatus;
	///������ʾ���
	int	NotifySequence;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������Դ
	char	OrderSource;
	///����״̬
	char	OrderStatus;
	///��������
	char	OrderType;
	///��ɽ�����
	int	VolumeTraded;
	///ʣ������
	int	VolumeTotal;
	///��������
	TThostFtdcDateType	InsertDate;
	///ί��ʱ��
	TThostFtdcTimeType	InsertTime;
	///����ʱ��
	TThostFtdcTimeType	ActiveTime;
	///����ʱ��
	TThostFtdcTimeType	SuspendTime;
	///����޸�ʱ��
	TThostFtdcTimeType	UpdateTime;
	///����ʱ��
	TThostFtdcTimeType	CancelTime;
	///����޸Ľ���������Ա����
	TThostFtdcTraderIDType	ActiveTraderID;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///���
	int	SequenceNo;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///�û�ǿ����־
	int	UserForceClose;
	///�����û�����
	TThostFtdcUserIDType	ActiveUserID;
	///���͹�˾�������
	int	BrokerOrderSeq;
	///��ر���
	TThostFtdcOrderSysIDType	RelativeOrderSysID;
	///֣�����ɽ�����
	int	ZCETotalTradedVolume;
	///��������־
	int	IsSwapOrder;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///�ʽ��˺�
	TThostFtdcAccountIDType	AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///����������
struct CThostFtdcExchangeOrderField
{
	///�����۸�����
	char	OrderPriceType;
	///��������
	char	Direction;
	///��Ͽ�ƽ��־
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	double	LimitPrice;
	///����
	int	VolumeTotalOriginal;
	///��Ч������
	char	TimeCondition;
	///GTD����
	TThostFtdcDateType	GTDDate;
	///�ɽ�������
	char	VolumeCondition;
	///��С�ɽ���
	int	MinVolume;
	///��������
	char	ContingentCondition;
	///ֹ���
	double	StopPrice;
	///ǿƽԭ��
	char	ForceCloseReason;
	///�Զ������־
	int	IsAutoSuspend;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	int	RequestID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///�����ύ״̬
	char	OrderSubmitStatus;
	///������ʾ���
	int	NotifySequence;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������Դ
	char	OrderSource;
	///����״̬
	char	OrderStatus;
	///��������
	char	OrderType;
	///��ɽ�����
	int	VolumeTraded;
	///ʣ������
	int	VolumeTotal;
	///��������
	TThostFtdcDateType	InsertDate;
	///ί��ʱ��
	TThostFtdcTimeType	InsertTime;
	///����ʱ��
	TThostFtdcTimeType	ActiveTime;
	///����ʱ��
	TThostFtdcTimeType	SuspendTime;
	///����޸�ʱ��
	TThostFtdcTimeType	UpdateTime;
	///����ʱ��
	TThostFtdcTimeType	CancelTime;
	///����޸Ľ���������Ա����
	TThostFtdcTraderIDType	ActiveTraderID;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///���
	int	SequenceNo;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///��������������ʧ��
struct CThostFtdcExchangeOrderInsertErrorField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///���뱨������
struct CThostFtdcInputOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	int	OrderActionRef;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	char	ActionFlag;
	///�۸�
	double	LimitPrice;
	///�����仯
	int	VolumeChange;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///��������
struct CThostFtdcOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	int	OrderActionRef;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	char	ActionFlag;
	///�۸�
	double	LimitPrice;
	///�����仯
	int	VolumeChange;
	///��������
	TThostFtdcDateType	ActionDate;
	///����ʱ��
	TThostFtdcTimeType	ActionTime;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��������״̬
	char	OrderActionStatus;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///��������������
struct CThostFtdcExchangeOrderActionField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	char	ActionFlag;
	///�۸�
	double	LimitPrice;
	///�����仯
	int	VolumeChange;
	///��������
	TThostFtdcDateType	ActionDate;
	///����ʱ��
	TThostFtdcTimeType	ActionTime;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��������״̬
	char	OrderActionStatus;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///��������������ʧ��
struct CThostFtdcExchangeOrderActionErrorField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///�������ɽ�
struct CThostFtdcExchangeTradeField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�ɽ����
	TThostFtdcTradeIDType	TradeID;
	///��������
	char	Direction;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///���׽�ɫ
	char	TradingRole;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///��ƽ��־
	char	OffsetFlag;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///�۸�
	double	Price;
	///����
	int	Volume;
	///�ɽ�ʱ��
	TThostFtdcDateType	TradeDate;
	///�ɽ�ʱ��
	TThostFtdcTimeType	TradeTime;
	///�ɽ�����
	char	TradeType;
	///�ɽ�����Դ
	char	PriceSource;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///���
	int	SequenceNo;
	///�ɽ���Դ
	char	TradeSource;
};

///�ɽ�
struct CThostFtdcTradeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�ɽ����
	TThostFtdcTradeIDType	TradeID;
	///��������
	char	Direction;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///���׽�ɫ
	char	TradingRole;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///��ƽ��־
	char	OffsetFlag;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///�۸�
	double	Price;
	///����
	int	Volume;
	///�ɽ�ʱ��
	TThostFtdcDateType	TradeDate;
	///�ɽ�ʱ��
	TThostFtdcTimeType	TradeTime;
	///�ɽ�����
	char	TradeType;
	///�ɽ�����Դ
	char	PriceSource;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///���
	int	SequenceNo;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///���͹�˾�������
	int	BrokerOrderSeq;
	///�ɽ���Դ
	char	TradeSource;
};

///�û��Ự
struct CThostFtdcUserSessionField
{
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��¼����
	TThostFtdcDateType	LoginDate;
	///��¼ʱ��
	TThostFtdcTimeType	LoginTime;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///�ӿڶ˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	InterfaceProductInfo;
	///Э����Ϣ
	TThostFtdcProtocolInfoType	ProtocolInfo;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
	///��¼��ע
	TThostFtdcLoginRemarkType	LoginRemark;
};

///��ѯ��󱨵�����
struct CThostFtdcQueryMaxOrderVolumeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	char	Direction;
	///��ƽ��־
	char	OffsetFlag;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///�������������
	int	MaxVolume;
};

///Ͷ���߽�����ȷ����Ϣ
struct CThostFtdcSettlementInfoConfirmField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///ȷ������
	TThostFtdcDateType	ConfirmDate;
	///ȷ��ʱ��
	TThostFtdcTimeType	ConfirmTime;
};

///�����ͬ��
struct CThostFtdcSyncDepositField
{
	///�������ˮ��
	TThostFtdcDepositSeqNoType	DepositSeqNo;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///�����
	double	Deposit;
	///�Ƿ�ǿ�ƽ���
	int	IsForce;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///������Ѻͬ��
struct CThostFtdcSyncFundMortgageField
{
	///������Ѻ��ˮ��
	TThostFtdcDepositSeqNoType	MortgageSeqNo;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Դ����
	TThostFtdcCurrencyIDType	FromCurrencyID;
	///��Ѻ���
	double	MortgageAmount;
	///Ŀ�����
	TThostFtdcCurrencyIDType	ToCurrencyID;
};

///���͹�˾ͬ��
struct CThostFtdcBrokerSyncField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
};

///����ͬ���е�Ͷ����
struct CThostFtdcSyncingInvestorField
{
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���߷������
	TThostFtdcInvestorIDType	InvestorGroupID;
	///Ͷ��������
	TThostFtdcPartyNameType	InvestorName;
	///֤������
	char	IdentifiedCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�Ƿ��Ծ
	int	IsActive;
	///��ϵ�绰
	TThostFtdcTelephoneType	Telephone;
	///ͨѶ��ַ
	TThostFtdcAddressType	Address;
	///��������
	TThostFtdcDateType	OpenDate;
	///�ֻ�
	TThostFtdcMobileType	Mobile;
	///��������ģ�����
	TThostFtdcInvestorIDType	CommModelID;
	///��֤����ģ�����
	TThostFtdcInvestorIDType	MarginModelID;
};

///����ͬ���еĽ��״���
struct CThostFtdcSyncingTradingCodeField
{
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///�Ƿ��Ծ
	int	IsActive;
	///���ױ�������
	char	ClientIDType;
};

///����ͬ���е�Ͷ���߷���
struct CThostFtdcSyncingInvestorGroupField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���߷������
	TThostFtdcInvestorIDType	InvestorGroupID;
	///Ͷ���߷�������
	TThostFtdcInvestorGroupNameType	InvestorGroupName;
};

///����ͬ���еĽ����˺�
struct CThostFtdcSyncingTradingAccountField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ϴ���Ѻ���
	double	PreMortgage;
	///�ϴ����ö��
	double	PreCredit;
	///�ϴδ���
	double	PreDeposit;
	///�ϴν���׼����
	double	PreBalance;
	///�ϴ�ռ�õı�֤��
	double	PreMargin;
	///��Ϣ����
	double	InterestBase;
	///��Ϣ����
	double	Interest;
	///�����
	double	Deposit;
	///������
	double	Withdraw;
	///����ı�֤��
	double	FrozenMargin;
	///������ʽ�
	double	FrozenCash;
	///�����������
	double	FrozenCommission;
	///��ǰ��֤���ܶ�
	double	CurrMargin;
	///�ʽ���
	double	CashIn;
	///������
	double	Commission;
	///ƽ��ӯ��
	double	CloseProfit;
	///�ֲ�ӯ��
	double	PositionProfit;
	///�ڻ�����׼����
	double	Balance;
	///�����ʽ�
	double	Available;
	///��ȡ�ʽ�
	double	WithdrawQuota;
	///����׼����
	double	Reserve;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///���ö��
	double	Credit;
	///��Ѻ���
	double	Mortgage;
	///��������֤��
	double	ExchangeMargin;
	///Ͷ���߽��֤��
	double	DeliveryMargin;
	///���������֤��
	double	ExchangeDeliveryMargin;
	///�����ڻ�����׼����
	double	ReserveBalance;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///�ϴλ���������
	double	PreFundMortgageIn;
	///�ϴλ����ʳ����
	double	PreFundMortgageOut;
	///����������
	double	FundMortgageIn;
	///�����ʳ����
	double	FundMortgageOut;
	///������Ѻ���
	double	FundMortgageAvailable;
	///����Ѻ���ҽ��
	double	MortgageableFund;
	///�����Ʒռ�ñ�֤��
	double	SpecProductMargin;
	///�����Ʒ���ᱣ֤��
	double	SpecProductFrozenMargin;
	///�����Ʒ������
	double	SpecProductCommission;
	///�����Ʒ����������
	double	SpecProductFrozenCommission;
	///�����Ʒ�ֲ�ӯ��
	double	SpecProductPositionProfit;
	///�����Ʒƽ��ӯ��
	double	SpecProductCloseProfit;
	///���ݳֲ�ӯ���㷨����������Ʒ�ֲ�ӯ��
	double	SpecProductPositionProfitByAlg;
	///�����Ʒ��������֤��
	double	SpecProductExchangeMargin;
};

///����ͬ���е�Ͷ���ֲ߳�
struct CThostFtdcSyncingInvestorPositionField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///�ֲֶ�շ���
	char	PosiDirection;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///�ֲ�����
	char	PositionDate;
	///���ճֲ�
	int	YdPosition;
	///���ճֲ�
	int	Position;
	///��ͷ����
	int	LongFrozen;
	///��ͷ����
	int	ShortFrozen;
	///���ֶ�����
	double	LongFrozenAmount;
	///���ֶ�����
	double	ShortFrozenAmount;
	///������
	int	OpenVolume;
	///ƽ����
	int	CloseVolume;
	///���ֽ��
	double	OpenAmount;
	///ƽ�ֽ��
	double	CloseAmount;
	///�ֲֳɱ�
	double	PositionCost;
	///�ϴ�ռ�õı�֤��
	double	PreMargin;
	///ռ�õı�֤��
	double	UseMargin;
	///����ı�֤��
	double	FrozenMargin;
	///������ʽ�
	double	FrozenCash;
	///�����������
	double	FrozenCommission;
	///�ʽ���
	double	CashIn;
	///������
	double	Commission;
	///ƽ��ӯ��
	double	CloseProfit;
	///�ֲ�ӯ��
	double	PositionProfit;
	///�ϴν����
	double	PreSettlementPrice;
	///���ν����
	double	SettlementPrice;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///���ֳɱ�
	double	OpenCost;
	///��������֤��
	double	ExchangeMargin;
	///��ϳɽ��γɵĳֲ�
	int	CombPosition;
	///��϶�ͷ����
	int	CombLongFrozen;
	///��Ͽ�ͷ����
	int	CombShortFrozen;
	///���ն���ƽ��ӯ��
	double	CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	double	CloseProfitByTrade;
	///���ճֲ�
	int	TodayPosition;
	///��֤����
	double	MarginRateByMoney;
	///��֤����(������)
	double	MarginRateByVolume;
	///ִ�ж���
	int	StrikeFrozen;
	///ִ�ж�����
	double	StrikeFrozenAmount;
	///����ִ�ж���
	int	AbandonFrozen;
};

///����ͬ���еĺ�Լ��֤����
struct CThostFtdcSyncingInstrumentMarginRateField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///��ͷ��֤����
	double	LongMarginRatioByMoney;
	///��ͷ��֤���
	double	LongMarginRatioByVolume;
	///��ͷ��֤����
	double	ShortMarginRatioByMoney;
	///��ͷ��֤���
	double	ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	int	IsRelative;
};

///����ͬ���еĺ�Լ��������
struct CThostFtdcSyncingInstrumentCommissionRateField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	double	OpenRatioByMoney;
	///����������
	double	OpenRatioByVolume;
	///ƽ����������
	double	CloseRatioByMoney;
	///ƽ��������
	double	CloseRatioByVolume;
	///ƽ����������
	double	CloseTodayRatioByMoney;
	///ƽ��������
	double	CloseTodayRatioByVolume;
};

///����ͬ���еĺ�Լ����Ȩ��
struct CThostFtdcSyncingInstrumentTradingRightField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����Ȩ��
	char	TradingRight;
};

///��ѯ����
struct CThostFtdcQryOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///��ʼʱ��
	TThostFtdcTimeType	InsertTimeStart;
	///����ʱ��
	TThostFtdcTimeType	InsertTimeEnd;
};

///��ѯ�ɽ�
struct CThostFtdcQryTradeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�ɽ����
	TThostFtdcTradeIDType	TradeID;
	///��ʼʱ��
	TThostFtdcTimeType	TradeTimeStart;
	///����ʱ��
	TThostFtdcTimeType	TradeTimeEnd;
};

///��ѯͶ���ֲ߳�
struct CThostFtdcQryInvestorPositionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///��ѯ�ʽ��˻�
struct CThostFtdcQryTradingAccountField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///��ѯͶ����
struct CThostFtdcQryInvestorField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

///��ѯ���ױ���
struct CThostFtdcQryTradingCodeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///���ױ�������
	char	ClientIDType;
};

///��ѯͶ������
struct CThostFtdcQryInvestorGroupField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
};

///��ѯ��Լ��֤����
struct CThostFtdcQryInstrumentMarginRateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	char	HedgeFlag;
};

///��ѯ��������
struct CThostFtdcQryInstrumentCommissionRateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///��ѯ��Լ����Ȩ��
struct CThostFtdcQryInstrumentTradingRightField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///��ѯ���͹�˾
struct CThostFtdcQryBrokerField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
};

///��ѯ����Ա
struct CThostFtdcQryTraderField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///��ѯ�����û�����Ȩ��
struct CThostFtdcQrySuperUserFunctionField
{
	///�û�����
	TThostFtdcUserIDType	UserID;
};

///��ѯ�û��Ự
struct CThostFtdcQryUserSessionField
{
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
};

///��ѯ���͹�˾��Ա����
struct CThostFtdcQryPartBrokerField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
};

///��ѯǰ��״̬
struct CThostFtdcQryFrontStatusField
{
	///ǰ�ñ��
	int	FrontID;
};

///��ѯ����������
struct CThostFtdcQryExchangeOrderField
{
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///��ѯ��������
struct CThostFtdcQryOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///��ѯ��������������
struct CThostFtdcQryExchangeOrderActionField
{
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///��ѯ�����û�
struct CThostFtdcQrySuperUserField
{
	///�û�����
	TThostFtdcUserIDType	UserID;
};

///��ѯ������
struct CThostFtdcQryExchangeField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///��ѯ��Ʒ
struct CThostFtdcQryProductField
{
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
	///��Ʒ����
	char	ProductClass;
};

///��ѯ��Լ
struct CThostFtdcQryInstrumentField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
};

///��ѯ����
struct CThostFtdcQryDepthMarketDataField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///��ѯ���͹�˾�û�
struct CThostFtdcQryBrokerUserField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
};

///��ѯ���͹�˾�û�Ȩ��
struct CThostFtdcQryBrokerUserFunctionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
};

///��ѯ����Ա���̻�
struct CThostFtdcQryTraderOfferField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///��ѯ�������ˮ
struct CThostFtdcQrySyncDepositField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�������ˮ��
	TThostFtdcDepositSeqNoType	DepositSeqNo;
};

///��ѯͶ���߽�����
struct CThostFtdcQrySettlementInfoField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������
	TThostFtdcDateType	TradingDay;
};

///��ѯ��������֤����
struct CThostFtdcQryExchangeMarginRateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	char	HedgeFlag;
};

///��ѯ������������֤����
struct CThostFtdcQryExchangeMarginRateAdjustField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	char	HedgeFlag;
};

///��ѯ����
struct CThostFtdcQryExchangeRateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Դ����
	TThostFtdcCurrencyIDType	FromCurrencyID;
	///Ŀ�����
	TThostFtdcCurrencyIDType	ToCurrencyID;
};

///��ѯ������Ѻ��ˮ
struct CThostFtdcQrySyncFundMortgageField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///������Ѻ��ˮ��
	TThostFtdcDepositSeqNoType	MortgageSeqNo;
};

///��ѯ����
struct CThostFtdcQryHisOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///��ʼʱ��
	TThostFtdcTimeType	InsertTimeStart;
	///����ʱ��
	TThostFtdcTimeType	InsertTimeEnd;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
};

///��ǰ��Ȩ��Լ��С��֤��
struct CThostFtdcOptionInstrMiniMarginField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��λ���֣���Ȩ��Լ��С��֤��
	double	MinMargin;
	///ȡֵ��ʽ
	char	ValueMethod;
	///�Ƿ���潻������ȡ
	int	IsRelative;
};

///��ǰ��Ȩ��Լ��֤�����ϵ��
struct CThostFtdcOptionInstrMarginAdjustField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ͷ����ͷ��֤�����ϵ��
	double	SShortMarginRatioByMoney;
	///Ͷ����ͷ��֤�����ϵ��
	double	SShortMarginRatioByVolume;
	///��ֵ��ͷ��֤�����ϵ��
	double	HShortMarginRatioByMoney;
	///��ֵ��ͷ��֤�����ϵ��
	double	HShortMarginRatioByVolume;
	///������ͷ��֤�����ϵ��
	double	AShortMarginRatioByMoney;
	///������ͷ��֤�����ϵ��
	double	AShortMarginRatioByVolume;
	///�Ƿ���潻������ȡ
	int	IsRelative;
	///�����̿�ͷ��֤�����ϵ��
	double	MShortMarginRatioByMoney;
	///�����̿�ͷ��֤�����ϵ��
	double	MShortMarginRatioByVolume;
};

///��ǰ��Ȩ��Լ�����ѵ���ϸ����
struct CThostFtdcOptionInstrCommRateField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	double	OpenRatioByMoney;
	///����������
	double	OpenRatioByVolume;
	///ƽ����������
	double	CloseRatioByMoney;
	///ƽ��������
	double	CloseRatioByVolume;
	///ƽ����������
	double	CloseTodayRatioByMoney;
	///ƽ��������
	double	CloseTodayRatioByVolume;
	///ִ����������
	double	StrikeRatioByMoney;
	///ִ��������
	double	StrikeRatioByVolume;
};

///��Ȩ���׳ɱ�
struct CThostFtdcOptionInstrTradeCostField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///��Ȩ��Լ��֤�𲻱䲿��
	double	FixedMargin;
	///��Ȩ��Լ��С��֤��
	double	MiniMargin;
	///��Ȩ��ԼȨ����
	double	Royalty;
	///��������Ȩ��Լ��֤�𲻱䲿��
	double	ExchFixedMargin;
	///��������Ȩ��Լ��С��֤��
	double	ExchMiniMargin;
};

///��Ȩ���׳ɱ���ѯ
struct CThostFtdcQryOptionInstrTradeCostField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///��Ȩ��Լ����
	double	InputPrice;
	///��ļ۸�,��0����������
	double	UnderlyingPrice;
};

///��Ȩ�������ʲ�ѯ
struct CThostFtdcQryOptionInstrCommRateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///��ָ�ֻ�ָ��
struct CThostFtdcIndexPriceField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ָ���ֻ����̼�
	double	ClosePrice;
};

///�����ִ������
struct CThostFtdcInputExecOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ִ����������
	TThostFtdcOrderRefType	ExecOrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����
	int	Volume;
	///������
	int	RequestID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��ƽ��־
	char	OffsetFlag;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///ִ������
	char	ActionType;
	///����ͷ������ĳֲַ���
	char	PosiDirection;
	///��Ȩ��Ȩ���Ƿ����ڻ�ͷ��ı��
	char	ReservePositionFlag;
	///��Ȩ��Ȩ�����ɵ�ͷ���Ƿ��Զ�ƽ��
	char	CloseFlag;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///�ʽ��˺�
	TThostFtdcAccountIDType	AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///���ױ���
	TThostFtdcClientIDType	ClientID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///����ִ���������
struct CThostFtdcInputExecOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///ִ�������������
	int	ExecOrderActionRef;
	///ִ����������
	TThostFtdcOrderRefType	ExecOrderRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///ִ������������
	TThostFtdcExecOrderSysIDType	ExecOrderSysID;
	///������־
	char	ActionFlag;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///ִ������
struct CThostFtdcExecOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ִ����������
	TThostFtdcOrderRefType	ExecOrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����
	int	Volume;
	///������
	int	RequestID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��ƽ��־
	char	OffsetFlag;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///ִ������
	char	ActionType;
	///����ͷ������ĳֲַ���
	char	PosiDirection;
	///��Ȩ��Ȩ���Ƿ����ڻ�ͷ��ı��
	char	ReservePositionFlag;
	///��Ȩ��Ȩ�����ɵ�ͷ���Ƿ��Զ�ƽ��
	char	CloseFlag;
	///����ִ��������
	TThostFtdcOrderLocalIDType	ExecOrderLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///ִ�������ύ״̬
	char	OrderSubmitStatus;
	///������ʾ���
	int	NotifySequence;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///ִ��������
	TThostFtdcExecOrderSysIDType	ExecOrderSysID;
	///��������
	TThostFtdcDateType	InsertDate;
	///����ʱ��
	TThostFtdcTimeType	InsertTime;
	///����ʱ��
	TThostFtdcTimeType	CancelTime;
	///ִ�н��
	char	ExecResult;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///���
	int	SequenceNo;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///�����û�����
	TThostFtdcUserIDType	ActiveUserID;
	///���͹�˾�������
	int	BrokerExecOrderSeq;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///�ʽ��˺�
	TThostFtdcAccountIDType	AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///ִ���������
struct CThostFtdcExecOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///ִ�������������
	int	ExecOrderActionRef;
	///ִ����������
	TThostFtdcOrderRefType	ExecOrderRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///ִ������������
	TThostFtdcExecOrderSysIDType	ExecOrderSysID;
	///������־
	char	ActionFlag;
	///��������
	TThostFtdcDateType	ActionDate;
	///����ʱ��
	TThostFtdcTimeType	ActionTime;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///����ִ��������
	TThostFtdcOrderLocalIDType	ExecOrderLocalID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��������״̬
	char	OrderActionStatus;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///ִ������
	char	ActionType;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///ִ�������ѯ
struct CThostFtdcQryExecOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///ִ��������
	TThostFtdcExecOrderSysIDType	ExecOrderSysID;
	///��ʼʱ��
	TThostFtdcTimeType	InsertTimeStart;
	///����ʱ��
	TThostFtdcTimeType	InsertTimeEnd;
};

///������ִ��������Ϣ
struct CThostFtdcExchangeExecOrderField
{
	///����
	int	Volume;
	///������
	int	RequestID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��ƽ��־
	char	OffsetFlag;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///ִ������
	char	ActionType;
	///����ͷ������ĳֲַ���
	char	PosiDirection;
	///��Ȩ��Ȩ���Ƿ����ڻ�ͷ��ı��
	char	ReservePositionFlag;
	///��Ȩ��Ȩ�����ɵ�ͷ���Ƿ��Զ�ƽ��
	char	CloseFlag;
	///����ִ��������
	TThostFtdcOrderLocalIDType	ExecOrderLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///ִ�������ύ״̬
	char	OrderSubmitStatus;
	///������ʾ���
	int	NotifySequence;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///ִ��������
	TThostFtdcExecOrderSysIDType	ExecOrderSysID;
	///��������
	TThostFtdcDateType	InsertDate;
	///����ʱ��
	TThostFtdcTimeType	InsertTime;
	///����ʱ��
	TThostFtdcTimeType	CancelTime;
	///ִ�н��
	char	ExecResult;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///���
	int	SequenceNo;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///������ִ�������ѯ
struct CThostFtdcQryExchangeExecOrderField
{
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///ִ�����������ѯ
struct CThostFtdcQryExecOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///������ִ���������
struct CThostFtdcExchangeExecOrderActionField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///ִ������������
	TThostFtdcExecOrderSysIDType	ExecOrderSysID;
	///������־
	char	ActionFlag;
	///��������
	TThostFtdcDateType	ActionDate;
	///����ʱ��
	TThostFtdcTimeType	ActionTime;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///����ִ��������
	TThostFtdcOrderLocalIDType	ExecOrderLocalID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��������״̬
	char	OrderActionStatus;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///ִ������
	char	ActionType;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///������ִ�����������ѯ
struct CThostFtdcQryExchangeExecOrderActionField
{
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///����ִ������
struct CThostFtdcErrExecOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ִ����������
	TThostFtdcOrderRefType	ExecOrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����
	int	Volume;
	///������
	int	RequestID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��ƽ��־
	char	OffsetFlag;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///ִ������
	char	ActionType;
	///����ͷ������ĳֲַ���
	char	PosiDirection;
	///��Ȩ��Ȩ���Ƿ����ڻ�ͷ��ı��
	char	ReservePositionFlag;
	///��Ȩ��Ȩ�����ɵ�ͷ���Ƿ��Զ�ƽ��
	char	CloseFlag;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///�ʽ��˺�
	TThostFtdcAccountIDType	AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///���ױ���
	TThostFtdcClientIDType	ClientID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///��ѯ����ִ������
struct CThostFtdcQryErrExecOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

///����ִ���������
struct CThostFtdcErrExecOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///ִ�������������
	int	ExecOrderActionRef;
	///ִ����������
	TThostFtdcOrderRefType	ExecOrderRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///ִ������������
	TThostFtdcExecOrderSysIDType	ExecOrderSysID;
	///������־
	char	ActionFlag;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///��ѯ����ִ���������
struct CThostFtdcQryErrExecOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

///Ͷ������Ȩ��Լ����Ȩ��
struct CThostFtdcOptionInstrTradingRightField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��������
	char	Direction;
	///����Ȩ��
	char	TradingRight;
};

///��ѯ��Ȩ��Լ����Ȩ��
struct CThostFtdcQryOptionInstrTradingRightField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	char	Direction;
};

///�����ѯ��
struct CThostFtdcInputForQuoteField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ѯ������
	TThostFtdcOrderRefType	ForQuoteRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///ѯ��
struct CThostFtdcForQuoteField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ѯ������
	TThostFtdcOrderRefType	ForQuoteRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����ѯ�۱��
	TThostFtdcOrderLocalIDType	ForQuoteLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///��������
	TThostFtdcDateType	InsertDate;
	///����ʱ��
	TThostFtdcTimeType	InsertTime;
	///ѯ��״̬
	char	ForQuoteStatus;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///�����û�����
	TThostFtdcUserIDType	ActiveUserID;
	///���͹�˾ѯ�۱��
	int	BrokerForQutoSeq;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///ѯ�۲�ѯ
struct CThostFtdcQryForQuoteField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��ʼʱ��
	TThostFtdcTimeType	InsertTimeStart;
	///����ʱ��
	TThostFtdcTimeType	InsertTimeEnd;
};

///������ѯ����Ϣ
struct CThostFtdcExchangeForQuoteField
{
	///����ѯ�۱��
	TThostFtdcOrderLocalIDType	ForQuoteLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///��������
	TThostFtdcDateType	InsertDate;
	///����ʱ��
	TThostFtdcTimeType	InsertTime;
	///ѯ��״̬
	char	ForQuoteStatus;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///������ѯ�۲�ѯ
struct CThostFtdcQryExchangeForQuoteField
{
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///����ı���
struct CThostFtdcInputQuoteField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	QuoteRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///���۸�
	double	AskPrice;
	///��۸�
	double	BidPrice;
	///������
	int	AskVolume;
	///������
	int	BidVolume;
	///������
	int	RequestID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///����ƽ��־
	char	AskOffsetFlag;
	///��ƽ��־
	char	BidOffsetFlag;
	///��Ͷ���ױ���־
	char	AskHedgeFlag;
	///��Ͷ���ױ���־
	char	BidHedgeFlag;
	///��������������
	TThostFtdcOrderRefType	AskOrderRef;
	///�����򱨵�����
	TThostFtdcOrderRefType	BidOrderRef;
	///Ӧ�۱��
	TThostFtdcOrderSysIDType	ForQuoteSysID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///���ױ���
	TThostFtdcClientIDType	ClientID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///���뱨�۲���
struct CThostFtdcInputQuoteActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///���۲�������
	int	QuoteActionRef;
	///��������
	TThostFtdcOrderRefType	QuoteRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///���۲������
	TThostFtdcOrderSysIDType	QuoteSysID;
	///������־
	char	ActionFlag;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///���ױ���
	TThostFtdcClientIDType	ClientID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///����
struct CThostFtdcQuoteField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	QuoteRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///���۸�
	double	AskPrice;
	///��۸�
	double	BidPrice;
	///������
	int	AskVolume;
	///������
	int	BidVolume;
	///������
	int	RequestID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///����ƽ��־
	char	AskOffsetFlag;
	///��ƽ��־
	char	BidOffsetFlag;
	///��Ͷ���ױ���־
	char	AskHedgeFlag;
	///��Ͷ���ױ���־
	char	BidHedgeFlag;
	///���ر��۱��
	TThostFtdcOrderLocalIDType	QuoteLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///������ʾ���
	int	NotifySequence;
	///�����ύ״̬
	char	OrderSubmitStatus;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///���۱��
	TThostFtdcOrderSysIDType	QuoteSysID;
	///��������
	TThostFtdcDateType	InsertDate;
	///����ʱ��
	TThostFtdcTimeType	InsertTime;
	///����ʱ��
	TThostFtdcTimeType	CancelTime;
	///����״̬
	char	QuoteStatus;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///���
	int	SequenceNo;
	///�����������
	TThostFtdcOrderSysIDType	AskOrderSysID;
	///�򷽱������
	TThostFtdcOrderSysIDType	BidOrderSysID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///�����û�����
	TThostFtdcUserIDType	ActiveUserID;
	///���͹�˾���۱��
	int	BrokerQuoteSeq;
	///��������������
	TThostFtdcOrderRefType	AskOrderRef;
	///�����򱨵�����
	TThostFtdcOrderRefType	BidOrderRef;
	///Ӧ�۱��
	TThostFtdcOrderSysIDType	ForQuoteSysID;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///�ʽ��˺�
	TThostFtdcAccountIDType	AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///���۲���
struct CThostFtdcQuoteActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///���۲�������
	int	QuoteActionRef;
	///��������
	TThostFtdcOrderRefType	QuoteRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///���۲������
	TThostFtdcOrderSysIDType	QuoteSysID;
	///������־
	char	ActionFlag;
	///��������
	TThostFtdcDateType	ActionDate;
	///����ʱ��
	TThostFtdcTimeType	ActionTime;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///���ر��۱��
	TThostFtdcOrderLocalIDType	QuoteLocalID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��������״̬
	char	OrderActionStatus;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///���۲�ѯ
struct CThostFtdcQryQuoteField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///���۱��
	TThostFtdcOrderSysIDType	QuoteSysID;
	///��ʼʱ��
	TThostFtdcTimeType	InsertTimeStart;
	///����ʱ��
	TThostFtdcTimeType	InsertTimeEnd;
};

///������������Ϣ
struct CThostFtdcExchangeQuoteField
{
	///���۸�
	double	AskPrice;
	///��۸�
	double	BidPrice;
	///������
	int	AskVolume;
	///������
	int	BidVolume;
	///������
	int	RequestID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///����ƽ��־
	char	AskOffsetFlag;
	///��ƽ��־
	char	BidOffsetFlag;
	///��Ͷ���ױ���־
	char	AskHedgeFlag;
	///��Ͷ���ױ���־
	char	BidHedgeFlag;
	///���ر��۱��
	TThostFtdcOrderLocalIDType	QuoteLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///������ʾ���
	int	NotifySequence;
	///�����ύ״̬
	char	OrderSubmitStatus;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///���۱��
	TThostFtdcOrderSysIDType	QuoteSysID;
	///��������
	TThostFtdcDateType	InsertDate;
	///����ʱ��
	TThostFtdcTimeType	InsertTime;
	///����ʱ��
	TThostFtdcTimeType	CancelTime;
	///����״̬
	char	QuoteStatus;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///���
	int	SequenceNo;
	///�����������
	TThostFtdcOrderSysIDType	AskOrderSysID;
	///�򷽱������
	TThostFtdcOrderSysIDType	BidOrderSysID;
	///Ӧ�۱��
	TThostFtdcOrderSysIDType	ForQuoteSysID;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///���������۲�ѯ
struct CThostFtdcQryExchangeQuoteField
{
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///���۲�����ѯ
struct CThostFtdcQryQuoteActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///���������۲���
struct CThostFtdcExchangeQuoteActionField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///���۲������
	TThostFtdcOrderSysIDType	QuoteSysID;
	///������־
	char	ActionFlag;
	///��������
	TThostFtdcDateType	ActionDate;
	///����ʱ��
	TThostFtdcTimeType	ActionTime;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///���ر��۱��
	TThostFtdcOrderLocalIDType	QuoteLocalID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��������״̬
	char	OrderActionStatus;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///���������۲�����ѯ
struct CThostFtdcQryExchangeQuoteActionField
{
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///��Ȩ��Լdeltaֵ
struct CThostFtdcOptionInstrDeltaField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Deltaֵ
	double	Delta;
};

///���������̵�ѯ������
struct CThostFtdcForQuoteRspField
{
	///������
	TThostFtdcDateType	TradingDay;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ѯ�۱��
	TThostFtdcOrderSysIDType	ForQuoteSysID;
	///ѯ��ʱ��
	TThostFtdcTimeType	ForQuoteTime;
	///ҵ������
	TThostFtdcDateType	ActionDay;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///��ǰ��Ȩ��Լִ��ƫ��ֵ����ϸ����
struct CThostFtdcStrikeOffsetField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///ִ��ƫ��ֵ
	double	Offset;
	///ִ��ƫ������
	char	OffsetType;
};

///��Ȩִ��ƫ��ֵ��ѯ
struct CThostFtdcQryStrikeOffsetField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///����������������
struct CThostFtdcInputBatchOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	int	OrderActionRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///������������
struct CThostFtdcBatchOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	int	OrderActionRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��������
	TThostFtdcDateType	ActionDate;
	///����ʱ��
	TThostFtdcTimeType	ActionTime;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��������״̬
	char	OrderActionStatus;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///������������������
struct CThostFtdcExchangeBatchOrderActionField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��������
	TThostFtdcDateType	ActionDate;
	///����ʱ��
	TThostFtdcTimeType	ActionTime;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��������״̬
	char	OrderActionStatus;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///��ѯ������������
struct CThostFtdcQryBatchOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///��Ϻ�Լ��ȫϵ��
struct CThostFtdcCombInstrumentGuardField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///
	double	GuarantRatio;
};

///��Ϻ�Լ��ȫϵ����ѯ
struct CThostFtdcQryCombInstrumentGuardField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///������������
struct CThostFtdcInputCombActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///�������
	TThostFtdcOrderRefType	CombActionRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��������
	char	Direction;
	///����
	int	Volume;
	///���ָ���
	char	CombDirection;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///�������
struct CThostFtdcCombActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///�������
	TThostFtdcOrderRefType	CombActionRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��������
	char	Direction;
	///����
	int	Volume;
	///���ָ���
	char	CombDirection;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///����������ϱ��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///���״̬
	char	ActionStatus;
	///������ʾ���
	int	NotifySequence;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///���
	int	SequenceNo;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///������ϲ�ѯ
struct CThostFtdcQryCombActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///���������������Ϣ
struct CThostFtdcExchangeCombActionField
{
	///��������
	char	Direction;
	///����
	int	Volume;
	///���ָ���
	char	CombDirection;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///����������ϱ��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///���״̬
	char	ActionStatus;
	///������ʾ���
	int	NotifySequence;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///���
	int	SequenceNo;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///������������ϲ�ѯ
struct CThostFtdcQryExchangeCombActionField
{
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///��Ʒ���ۻ���
struct CThostFtdcProductExchRateField
{
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
	///���۱�������
	TThostFtdcCurrencyIDType	QuoteCurrencyID;
	///����
	double	ExchangeRate;
};

///��Ʒ���ۻ��ʲ�ѯ
struct CThostFtdcQryProductExchRateField
{
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
};

///��ѯѯ�ۼ۲����
struct CThostFtdcQryForQuoteParamField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///ѯ�ۼ۲����
struct CThostFtdcForQuoteParamField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///���¼�
	double	LastPrice;
	///�۲�
	double	PriceInterval;
};

///��ǰ��������Ȩ��Լ�����ѵ���ϸ����
struct CThostFtdcMMOptionInstrCommRateField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	double	OpenRatioByMoney;
	///����������
	double	OpenRatioByVolume;
	///ƽ����������
	double	CloseRatioByMoney;
	///ƽ��������
	double	CloseRatioByVolume;
	///ƽ����������
	double	CloseTodayRatioByMoney;
	///ƽ��������
	double	CloseTodayRatioByVolume;
	///ִ����������
	double	StrikeRatioByMoney;
	///ִ��������
	double	StrikeRatioByVolume;
};

///��������Ȩ�������ʲ�ѯ
struct CThostFtdcQryMMOptionInstrCommRateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///�����̺�Լ��������
struct CThostFtdcMMInstrumentCommissionRateField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	double	OpenRatioByMoney;
	///����������
	double	OpenRatioByVolume;
	///ƽ����������
	double	CloseRatioByMoney;
	///ƽ��������
	double	CloseRatioByVolume;
	///ƽ����������
	double	CloseTodayRatioByMoney;
	///ƽ��������
	double	CloseTodayRatioByVolume;
};

///��ѯ�����̺�Լ��������
struct CThostFtdcQryMMInstrumentCommissionRateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///��ǰ���������ѵ���ϸ����
struct CThostFtdcInstrumentOrderCommRateField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///����������
	double	OrderCommByVolume;
	///����������
	double	OrderActionCommByVolume;
};

///�����������ʲ�ѯ
struct CThostFtdcQryInstrumentOrderCommRateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///�г�����
struct CThostFtdcMarketDataField
{
	///������
	TThostFtdcDateType	TradingDay;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///���¼�
	double	LastPrice;
	///�ϴν����
	double	PreSettlementPrice;
	///������
	double	PreClosePrice;
	///��ֲ���
	double	PreOpenInterest;
	///����
	double	OpenPrice;
	///��߼�
	double	HighestPrice;
	///��ͼ�
	double	LowestPrice;
	///����
	int	Volume;
	///�ɽ����
	double	Turnover;
	///�ֲ���
	double	OpenInterest;
	///������
	double	ClosePrice;
	///���ν����
	double	SettlementPrice;
	///��ͣ���
	double	UpperLimitPrice;
	///��ͣ���
	double	LowerLimitPrice;
	///����ʵ��
	double	PreDelta;
	///����ʵ��
	double	CurrDelta;
	///����޸�ʱ��
	TThostFtdcTimeType	UpdateTime;
	///����޸ĺ���
	int	UpdateMillisec;
	///ҵ������
	TThostFtdcDateType	ActionDay;
};

///�����������
struct CThostFtdcMarketDataBaseField
{
	///������
	TThostFtdcDateType	TradingDay;
	///�ϴν����
	double	PreSettlementPrice;
	///������
	double	PreClosePrice;
	///��ֲ���
	double	PreOpenInterest;
	///����ʵ��
	double	PreDelta;
};

///���龲̬����
struct CThostFtdcMarketDataStaticField
{
	///����
	double	OpenPrice;
	///��߼�
	double	HighestPrice;
	///��ͼ�
	double	LowestPrice;
	///������
	double	ClosePrice;
	///��ͣ���
	double	UpperLimitPrice;
	///��ͣ���
	double	LowerLimitPrice;
	///���ν����
	double	SettlementPrice;
	///����ʵ��
	double	CurrDelta;
};

///�������³ɽ�����
struct CThostFtdcMarketDataLastMatchField
{
	///���¼�
	double	LastPrice;
	///����
	int	Volume;
	///�ɽ����
	double	Turnover;
	///�ֲ���
	double	OpenInterest;
};

///�������ż�����
struct CThostFtdcMarketDataBestPriceField
{
	///�����һ
	double	BidPrice1;
	///������һ
	int	BidVolume1;
	///������һ
	double	AskPrice1;
	///������һ
	int	AskVolume1;
};

///�����������������
struct CThostFtdcMarketDataBid23Field
{
	///����۶�
	double	BidPrice2;
	///��������
	int	BidVolume2;
	///�������
	double	BidPrice3;
	///��������
	int	BidVolume3;
};

///������������������
struct CThostFtdcMarketDataAsk23Field
{
	///�����۶�
	double	AskPrice2;
	///��������
	int	AskVolume2;
	///��������
	double	AskPrice3;
	///��������
	int	AskVolume3;
};

///���������ġ�������
struct CThostFtdcMarketDataBid45Field
{
	///�������
	double	BidPrice4;
	///��������
	int	BidVolume4;
	///�������
	double	BidPrice5;
	///��������
	int	BidVolume5;
};

///���������ġ�������
struct CThostFtdcMarketDataAsk45Field
{
	///��������
	double	AskPrice4;
	///��������
	int	AskVolume4;
	///��������
	double	AskPrice5;
	///��������
	int	AskVolume5;
};

///�������ʱ������
struct CThostFtdcMarketDataUpdateTimeField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����޸�ʱ��
	TThostFtdcTimeType	UpdateTime;
	///����޸ĺ���
	int	UpdateMillisec;
	///ҵ������
	TThostFtdcDateType	ActionDay;
};

///���齻������������
struct CThostFtdcMarketDataExchangeField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///ָ���ĺ�Լ
struct CThostFtdcSpecificInstrumentField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///��Լ״̬
struct CThostFtdcInstrumentStatusField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///���������
	TThostFtdcSettlementGroupIDType	SettlementGroupID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��Լ����״̬
	char	InstrumentStatus;
	///���׽׶α��
	int	TradingSegmentSN;
	///���뱾״̬ʱ��
	TThostFtdcTimeType	EnterTime;
	///���뱾״̬ԭ��
	char	EnterReason;
};

///��ѯ��Լ״̬
struct CThostFtdcQryInstrumentStatusField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
};

///Ͷ�����˻�
struct CThostFtdcInvestorAccountField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///����ӯ���㷨
struct CThostFtdcPositionProfitAlgorithmField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///ӯ���㷨
	char	Algorithm;
	///��ע
	TThostFtdcMemoType	Memo;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///��Ա�ʽ��ۿ�
struct CThostFtdcDiscountField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///�ʽ��ۿ۱���
	double	Discount;
};

///��ѯת������
struct CThostFtdcQryTransferBankField
{
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з����Ĵ���
	TThostFtdcBankBrchIDType	BankBrchID;
};

///ת������
struct CThostFtdcTransferBankField
{
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з����Ĵ���
	TThostFtdcBankBrchIDType	BankBrchID;
	///��������
	TThostFtdcBankNameType	BankName;
	///�Ƿ��Ծ
	int	IsActive;
};

///��ѯͶ���ֲ߳���ϸ
struct CThostFtdcQryInvestorPositionDetailField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///Ͷ���ֲ߳���ϸ
struct CThostFtdcInvestorPositionDetailField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///����
	char	Direction;
	///��������
	TThostFtdcDateType	OpenDate;
	///�ɽ����
	TThostFtdcTradeIDType	TradeID;
	///����
	int	Volume;
	///���ּ�
	double	OpenPrice;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///�ɽ�����
	char	TradeType;
	///��Ϻ�Լ����
	TThostFtdcInstrumentIDType	CombInstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///���ն���ƽ��ӯ��
	double	CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	double	CloseProfitByTrade;
	///���ն��гֲ�ӯ��
	double	PositionProfitByDate;
	///��ʶԳ�ֲ�ӯ��
	double	PositionProfitByTrade;
	///Ͷ���߱�֤��
	double	Margin;
	///��������֤��
	double	ExchMargin;
	///��֤����
	double	MarginRateByMoney;
	///��֤����(������)
	double	MarginRateByVolume;
	///������
	double	LastSettlementPrice;
	///�����
	double	SettlementPrice;
	///ƽ����
	int	CloseVolume;
	///ƽ�ֽ��
	double	CloseAmount;
};

///�ʽ��˻�������
struct CThostFtdcTradingAccountPasswordField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///����
	TThostFtdcPasswordType	Password;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///���������鱨�̻�
struct CThostFtdcMDTraderOfferField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///����
	TThostFtdcPasswordType	Password;
	///��װ���
	int	InstallID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///����������Ա����״̬
	char	TraderConnectStatus;
	///�����������������
	TThostFtdcDateType	ConnectRequestDate;
	///�������������ʱ��
	TThostFtdcTimeType	ConnectRequestTime;
	///�ϴα�������
	TThostFtdcDateType	LastReportDate;
	///�ϴα���ʱ��
	TThostFtdcTimeType	LastReportTime;
	///�����������
	TThostFtdcDateType	ConnectDate;
	///�������ʱ��
	TThostFtdcTimeType	ConnectTime;
	///��������
	TThostFtdcDateType	StartDate;
	///����ʱ��
	TThostFtdcTimeType	StartTime;
	///������
	TThostFtdcDateType	TradingDay;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��ϯλ���ɽ����
	TThostFtdcTradeIDType	MaxTradeID;
	///��ϯλ��󱨵�����
	TThostFtdcReturnCodeType	MaxOrderMessageReference;
};

///��ѯ���鱨�̻�
struct CThostFtdcQryMDTraderOfferField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
};

///��ѯ�ͻ�֪ͨ
struct CThostFtdcQryNoticeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
};

///�ͻ�֪ͨ
struct CThostFtdcNoticeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Ϣ����
	TThostFtdcContentType	Content;
	///���͹�˾֪ͨ�������к�
	TThostFtdcSequenceLabelType	SequenceLabel;
};

///�û�Ȩ��
struct CThostFtdcUserRightField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�ͻ�Ȩ������
	char	UserRightType;
	///�Ƿ��ֹ
	int	IsForbidden;
};

///��ѯ������Ϣȷ����
struct CThostFtdcQrySettlementInfoConfirmField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

///װ�ؽ�����Ϣ
struct CThostFtdcLoadSettlementInfoField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
};

///���͹�˾�����ʽ��㷨��
struct CThostFtdcBrokerWithdrawAlgorithmField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�����ʽ��㷨
	char	WithdrawAlgorithm;
	///�ʽ�ʹ����
	double	UsingRatio;
	///�����Ƿ����ƽ��ӯ��
	char	IncludeCloseProfit;
	///�����޲����޳ɽ��ͻ��Ƿ��ܿ����������
	char	AllWithoutTrade;
	///�����Ƿ����ƽ��ӯ��
	char	AvailIncludeCloseProfit;
	///�Ƿ������û��¼�
	int	IsBrokerUserEvent;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///������Ѻ����
	double	FundMortgageRatio;
	///Ȩ���㷨
	char	BalanceAlgorithm;
};

///�ʽ��˻���������
struct CThostFtdcTradingAccountPasswordUpdateV1Field
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///ԭ���Ŀ���
	TThostFtdcPasswordType	OldPassword;
	///�µĿ���
	TThostFtdcPasswordType	NewPassword;
};

///�ʽ��˻���������
struct CThostFtdcTradingAccountPasswordUpdateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///ԭ���Ŀ���
	TThostFtdcPasswordType	OldPassword;
	///�µĿ���
	TThostFtdcPasswordType	NewPassword;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///��ѯ��Ϻ�Լ����
struct CThostFtdcQryCombinationLegField
{
	///��Ϻ�Լ����
	TThostFtdcInstrumentIDType	CombInstrumentID;
	///���ȱ��
	int	LegID;
	///���Ⱥ�Լ����
	TThostFtdcInstrumentIDType	LegInstrumentID;
};

///��ѯ��Ϻ�Լ����
struct CThostFtdcQrySyncStatusField
{
	///������
	TThostFtdcDateType	TradingDay;
};

///��Ͻ��׺�Լ�ĵ���
struct CThostFtdcCombinationLegField
{
	///��Ϻ�Լ����
	TThostFtdcInstrumentIDType	CombInstrumentID;
	///���ȱ��
	int	LegID;
	///���Ⱥ�Լ����
	TThostFtdcInstrumentIDType	LegInstrumentID;
	///��������
	char	Direction;
	///���ȳ���
	int	LegMultiple;
	///��������
	int	ImplyLevel;
};

///����ͬ��״̬
struct CThostFtdcSyncStatusField
{
	///������
	TThostFtdcDateType	TradingDay;
	///����ͬ��״̬
	char	DataSyncStatus;
};

///��ѯ��ϵ��
struct CThostFtdcQryLinkManField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

///��ϵ��
struct CThostFtdcLinkManField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��ϵ������
	char	PersonType;
	///֤������
	char	IdentifiedCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///����
	TThostFtdcPartyNameType	PersonName;
	///��ϵ�绰
	TThostFtdcTelephoneType	Telephone;
	///ͨѶ��ַ
	TThostFtdcAddressType	Address;
	///��������
	TThostFtdcZipCodeType	ZipCode;
	///���ȼ�
	int	Priority;
	///������������
	TThostFtdcUOAZipCodeType	UOAZipCode;
	///ȫ��
	TThostFtdcInvestorFullNameType	PersonFullName;
};

///��ѯ���͹�˾�û��¼�
struct CThostFtdcQryBrokerUserEventField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�û��¼�����
	char	UserEventType;
};

///��ѯ���͹�˾�û��¼�
struct CThostFtdcBrokerUserEventField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�û��¼�����
	char	UserEventType;
	///�û��¼����
	int	EventSequenceNo;
	///�¼���������
	TThostFtdcDateType	EventDate;
	///�¼�����ʱ��
	TThostFtdcTimeType	EventTime;
	///�û��¼���Ϣ
	TThostFtdcUserEventInfoType	UserEventInfo;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///��ѯǩԼ��������
struct CThostFtdcQryContractBankField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з����Ĵ���
	TThostFtdcBankBrchIDType	BankBrchID;
};

///��ѯǩԼ������Ӧ
struct CThostFtdcContractBankField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з����Ĵ���
	TThostFtdcBankBrchIDType	BankBrchID;
	///��������
	TThostFtdcBankNameType	BankName;
};

///Ͷ������ϳֲ���ϸ
struct CThostFtdcInvestorPositionCombineDetailField
{
	///������
	TThostFtdcDateType	TradingDay;
	///��������
	TThostFtdcDateType	OpenDate;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///������
	int	SettlementID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��ϱ��
	TThostFtdcTradeIDType	ComTradeID;
	///��ϱ��
	TThostFtdcTradeIDType	TradeID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///����
	char	Direction;
	///�ֲ���
	int	TotalAmt;
	///Ͷ���߱�֤��
	double	Margin;
	///��������֤��
	double	ExchMargin;
	///��֤����
	double	MarginRateByMoney;
	///��֤����(������)
	double	MarginRateByVolume;
	///���ȱ��
	int	LegID;
	///���ȳ���
	int	LegMultiple;
	///��ϳֲֺ�Լ����
	TThostFtdcInstrumentIDType	CombInstrumentID;
	///�ɽ����
	int	TradeGroupID;
};

///Ԥ��
struct CThostFtdcParkedOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�����۸�����
	char	OrderPriceType;
	///��������
	char	Direction;
	///��Ͽ�ƽ��־
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	double	LimitPrice;
	///����
	int	VolumeTotalOriginal;
	///��Ч������
	char	TimeCondition;
	///GTD����
	TThostFtdcDateType	GTDDate;
	///�ɽ�������
	char	VolumeCondition;
	///��С�ɽ���
	int	MinVolume;
	///��������
	char	ContingentCondition;
	///ֹ���
	double	StopPrice;
	///ǿƽԭ��
	char	ForceCloseReason;
	///�Զ������־
	int	IsAutoSuspend;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	int	RequestID;
	///�û�ǿ����־
	int	UserForceClose;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///Ԥ�񱨵����
	TThostFtdcParkedOrderIDType	ParkedOrderID;
	///�û�����
	char	UserType;
	///Ԥ��״̬
	char	Status;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
	///��������־
	int	IsSwapOrder;
	///�ʽ��˺�
	TThostFtdcAccountIDType	AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///���ױ���
	TThostFtdcClientIDType	ClientID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///����Ԥ�񵥲���
struct CThostFtdcParkedOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	int	OrderActionRef;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	char	ActionFlag;
	///�۸�
	double	LimitPrice;
	///�����仯
	int	VolumeChange;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ԥ�񳷵������
	TThostFtdcParkedOrderActionIDType	ParkedOrderActionID;
	///�û�����
	char	UserType;
	///Ԥ�񳷵�״̬
	char	Status;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///��ѯԤ��
struct CThostFtdcQryParkedOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///��ѯԤ�񳷵�
struct CThostFtdcQryParkedOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///ɾ��Ԥ��
struct CThostFtdcRemoveParkedOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ԥ�񱨵����
	TThostFtdcParkedOrderIDType	ParkedOrderID;
};

///ɾ��Ԥ�񳷵�
struct CThostFtdcRemoveParkedOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ԥ�񳷵����
	TThostFtdcParkedOrderActionIDType	ParkedOrderActionID;
};

///���͹�˾�����ʽ��㷨��
struct CThostFtdcInvestorWithdrawAlgorithmField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///�����ʽ����
	double	UsingRatio;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///������Ѻ����
	double	FundMortgageRatio;
};

///��ѯ��ϳֲ���ϸ
struct CThostFtdcQryInvestorPositionCombineDetailField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��ϳֲֺ�Լ����
	TThostFtdcInstrumentIDType	CombInstrumentID;
};

///�ɽ�����
struct CThostFtdcMarketDataAveragePriceField
{
	///���վ���
	double	AveragePrice;
};

///У��Ͷ��������
struct CThostFtdcVerifyInvestorPasswordField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����
	TThostFtdcPasswordType	Password;
};

///�û�IP
struct CThostFtdcUserIPField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///IP��ַ����
	TThostFtdcIPAddressType	IPMask;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///�û��¼�֪ͨ��Ϣ
struct CThostFtdcTradingNoticeInfoField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����ʱ��
	TThostFtdcTimeType	SendTime;
	///��Ϣ����
	TThostFtdcContentType	FieldContent;
	///����ϵ�к�
	TThostFtdcSequenceSeriesType	SequenceSeries;
	///���к�
	int	SequenceNo;
};

///�û��¼�֪ͨ
struct CThostFtdcTradingNoticeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���߷�Χ
	char	InvestorRange;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����ϵ�к�
	TThostFtdcSequenceSeriesType	SequenceSeries;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����ʱ��
	TThostFtdcTimeType	SendTime;
	///���к�
	int	SequenceNo;
	///��Ϣ����
	TThostFtdcContentType	FieldContent;
};

///��ѯ�����¼�֪ͨ
struct CThostFtdcQryTradingNoticeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

///��ѯ���󱨵�
struct CThostFtdcQryErrOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

///���󱨵�
struct CThostFtdcErrOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�����۸�����
	char	OrderPriceType;
	///��������
	char	Direction;
	///��Ͽ�ƽ��־
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	double	LimitPrice;
	///����
	int	VolumeTotalOriginal;
	///��Ч������
	char	TimeCondition;
	///GTD����
	TThostFtdcDateType	GTDDate;
	///�ɽ�������
	char	VolumeCondition;
	///��С�ɽ���
	int	MinVolume;
	///��������
	char	ContingentCondition;
	///ֹ���
	double	StopPrice;
	///ǿƽԭ��
	char	ForceCloseReason;
	///�Զ������־
	int	IsAutoSuspend;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	int	RequestID;
	///�û�ǿ����־
	int	UserForceClose;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
	///��������־
	int	IsSwapOrder;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///�ʽ��˺�
	TThostFtdcAccountIDType	AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///���ױ���
	TThostFtdcClientIDType	ClientID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///��ѯ���󱨵�����
struct CThostFtdcErrorConditionalOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�����۸�����
	char	OrderPriceType;
	///��������
	char	Direction;
	///��Ͽ�ƽ��־
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	double	LimitPrice;
	///����
	int	VolumeTotalOriginal;
	///��Ч������
	char	TimeCondition;
	///GTD����
	TThostFtdcDateType	GTDDate;
	///�ɽ�������
	char	VolumeCondition;
	///��С�ɽ���
	int	MinVolume;
	///��������
	char	ContingentCondition;
	///ֹ���
	double	StopPrice;
	///ǿƽԭ��
	char	ForceCloseReason;
	///�Զ������־
	int	IsAutoSuspend;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	int	RequestID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///�����ύ״̬
	char	OrderSubmitStatus;
	///������ʾ���
	int	NotifySequence;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������Դ
	char	OrderSource;
	///����״̬
	char	OrderStatus;
	///��������
	char	OrderType;
	///��ɽ�����
	int	VolumeTraded;
	///ʣ������
	int	VolumeTotal;
	///��������
	TThostFtdcDateType	InsertDate;
	///ί��ʱ��
	TThostFtdcTimeType	InsertTime;
	///����ʱ��
	TThostFtdcTimeType	ActiveTime;
	///����ʱ��
	TThostFtdcTimeType	SuspendTime;
	///����޸�ʱ��
	TThostFtdcTimeType	UpdateTime;
	///����ʱ��
	TThostFtdcTimeType	CancelTime;
	///����޸Ľ���������Ա����
	TThostFtdcTraderIDType	ActiveTraderID;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///���
	int	SequenceNo;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///�û�ǿ����־
	int	UserForceClose;
	///�����û�����
	TThostFtdcUserIDType	ActiveUserID;
	///���͹�˾�������
	int	BrokerOrderSeq;
	///��ر���
	TThostFtdcOrderSysIDType	RelativeOrderSysID;
	///֣�����ɽ�����
	int	ZCETotalTradedVolume;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
	///��������־
	int	IsSwapOrder;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///�ʽ��˺�
	TThostFtdcAccountIDType	AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
};

///��ѯ���󱨵�����
struct CThostFtdcQryErrOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

///���󱨵�����
struct CThostFtdcErrOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	int	OrderActionRef;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	char	ActionFlag;
	///�۸�
	double	LimitPrice;
	///�����仯
	int	VolumeChange;
	///��������
	TThostFtdcDateType	ActionDate;
	///����ʱ��
	TThostFtdcTimeType	ActionTime;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	int	InstallID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��������״̬
	char	OrderActionStatus;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ӫҵ�����
	TThostFtdcBranchIDType	BranchID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType	InvestUnitID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType	MacAddress;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///��ѯ������״̬
struct CThostFtdcQryExchangeSequenceField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///������״̬
struct CThostFtdcExchangeSequenceField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///���
	int	SequenceNo;
	///��Լ����״̬
	char	MarketStatus;
};

///���ݼ۸��ѯ��󱨵�����
struct CThostFtdcQueryMaxOrderVolumeWithPriceField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	char	Direction;
	///��ƽ��־
	char	OffsetFlag;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///�������������
	int	MaxVolume;
	///�����۸�
	double	Price;
};

///��ѯ���͹�˾���ײ���
struct CThostFtdcQryBrokerTradingParamsField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///���͹�˾���ײ���
struct CThostFtdcBrokerTradingParamsField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��֤��۸�����
	char	MarginPriceType;
	///ӯ���㷨
	char	Algorithm;
	///�����Ƿ����ƽ��ӯ��
	char	AvailIncludeCloseProfit;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///��ȨȨ����۸�����
	char	OptionRoyaltyPriceType;
};

///��ѯ���͹�˾�����㷨
struct CThostFtdcQryBrokerTradingAlgosField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///���͹�˾�����㷨
struct CThostFtdcBrokerTradingAlgosField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///�ֲִ����㷨���
	char	HandlePositionAlgoID;
	///Ѱ�ұ�֤�����㷨���
	char	FindMarginRateAlgoID;
	///�ʽ����㷨���
	char	HandleTradingAccountAlgoID;
};

///��ѯ���͹�˾�ʽ�
struct CThostFtdcQueryBrokerDepositField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///���͹�˾�ʽ�
struct CThostFtdcBrokerDepositField
{
	///��������
	TThostFtdcTradeDateType	TradingDay;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�ϴν���׼����
	double	PreBalance;
	///��ǰ��֤���ܶ�
	double	CurrMargin;
	///ƽ��ӯ��
	double	CloseProfit;
	///�ڻ�����׼����
	double	Balance;
	///�����
	double	Deposit;
	///������
	double	Withdraw;
	///�����ʽ�
	double	Available;
	///����׼����
	double	Reserve;
	///����ı�֤��
	double	FrozenMargin;
};

///��ѯ��֤����ϵͳ���͹�˾��Կ
struct CThostFtdcQryCFMMCBrokerKeyField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
};

///��֤����ϵͳ���͹�˾��Կ
struct CThostFtdcCFMMCBrokerKeyField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///���͹�˾ͳһ����
	TThostFtdcParticipantIDType	ParticipantID;
	///��Կ��������
	TThostFtdcDateType	CreateDate;
	///��Կ����ʱ��
	TThostFtdcTimeType	CreateTime;
	///��Կ���
	int	KeyID;
	///��̬��Կ
	TThostFtdcCFMMCKeyType	CurrentKey;
	///��̬��Կ����
	char	KeyKind;
};

///��֤����ϵͳ���͹�˾�ʽ��˻���Կ
struct CThostFtdcCFMMCTradingAccountKeyField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///���͹�˾ͳһ����
	TThostFtdcParticipantIDType	ParticipantID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///��Կ���
	int	KeyID;
	///��̬��Կ
	TThostFtdcCFMMCKeyType	CurrentKey;
};

///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
struct CThostFtdcQryCFMMCTradingAccountKeyField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

///�û���̬���Ʋ���
struct CThostFtdcBrokerUserOTPParamField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��̬�����ṩ��
	TThostFtdcOTPVendorsIDType	OTPVendorsID;
	///��̬�������к�
	TThostFtdcSerialNumberType	SerialNumber;
	///������Կ
	TThostFtdcAuthKeyType	AuthKey;
	///Ư��ֵ
	int	LastDrift;
	///�ɹ�ֵ
	int	LastSuccess;
	///��̬��������
	char	OTPType;
};

///�ֹ�ͬ���û���̬����
struct CThostFtdcManualSyncBrokerUserOTPField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��̬��������
	char	OTPType;
	///��һ����̬����
	TThostFtdcPasswordType	FirstOTP;
	///�ڶ�����̬����
	TThostFtdcPasswordType	SecondOTP;
};

///Ͷ������������ģ��
struct CThostFtdcCommRateModelField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��������ģ�����
	TThostFtdcInvestorIDType	CommModelID;
	///ģ������
	TThostFtdcCommModelNameType	CommModelName;
};

///�����ѯͶ������������ģ��
struct CThostFtdcQryCommRateModelField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��������ģ�����
	TThostFtdcInvestorIDType	CommModelID;
};

///Ͷ���߱�֤����ģ��
struct CThostFtdcMarginModelField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��֤����ģ�����
	TThostFtdcInvestorIDType	MarginModelID;
	///ģ������
	TThostFtdcCommModelNameType	MarginModelName;
};

///�����ѯͶ���߱�֤����ģ��
struct CThostFtdcQryMarginModelField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///��֤����ģ�����
	TThostFtdcInvestorIDType	MarginModelID;
};

///�ֵ��۵���Ϣ
struct CThostFtdcEWarrantOffsetField
{
	///��������
	TThostFtdcTradeDateType	TradingDay;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	char	Direction;
	///Ͷ���ױ���־
	char	HedgeFlag;
	///����
	int	Volume;
};

///��ѯ�ֵ��۵���Ϣ
struct CThostFtdcQryEWarrantOffsetField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///��ѯͶ����Ʒ��/��Ʒ�ֱ�֤��
struct CThostFtdcQryInvestorProductGroupMarginField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ʒ��/��Ʒ�ֱ�ʾ
	TThostFtdcInstrumentIDType	ProductGroupID;
	///Ͷ���ױ���־
	char	HedgeFlag;
};

///Ͷ����Ʒ��/��Ʒ�ֱ�֤��
struct CThostFtdcInvestorProductGroupMarginField
{
	///Ʒ��/��Ʒ�ֱ�ʾ
	TThostFtdcInstrumentIDType	ProductGroupID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	SettlementID;
	///����ı�֤��
	double	FrozenMargin;
	///��ͷ����ı�֤��
	double	LongFrozenMargin;
	///��ͷ����ı�֤��
	double	ShortFrozenMargin;
	///ռ�õı�֤��
	double	UseMargin;
	///��ͷ��֤��
	double	LongUseMargin;
	///��ͷ��֤��
	double	ShortUseMargin;
	///��������֤��
	double	ExchMargin;
	///��������ͷ��֤��
	double	LongExchMargin;
	///��������ͷ��֤��
	double	ShortExchMargin;
	///ƽ��ӯ��
	double	CloseProfit;
	///�����������
	double	FrozenCommission;
	///������
	double	Commission;
	///������ʽ�
	double	FrozenCash;
	///�ʽ���
	double	CashIn;
	///�ֲ�ӯ��
	double	PositionProfit;
	///�۵��ܽ��
	double	OffsetAmount;
	///��ͷ�۵��ܽ��
	double	LongOffsetAmount;
	///��ͷ�۵��ܽ��
	double	ShortOffsetAmount;
	///�������۵��ܽ��
	double	ExchOffsetAmount;
	///��������ͷ�۵��ܽ��
	double	LongExchOffsetAmount;
	///��������ͷ�۵��ܽ��
	double	ShortExchOffsetAmount;
	///Ͷ���ױ���־
	char	HedgeFlag;
};

///��ѯ��������û�����
struct CThostFtdcQueryCFMMCTradingAccountTokenField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

///��������û�����
struct CThostFtdcCFMMCTradingAccountTokenField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///���͹�˾ͳһ����
	TThostFtdcParticipantIDType	ParticipantID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///��Կ���
	int	KeyID;
	///��̬����
	TThostFtdcCFMMCTokenType	Token;
};

///��ѯ��Ʒ��
struct CThostFtdcQryProductGroupField
{
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};

///Ͷ����Ʒ��/��Ʒ�ֱ�֤���Ʒ��
struct CThostFtdcProductGroupField
{
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ʒ�����
	TThostFtdcInstrumentIDType	ProductGroupID;
};

///����������
struct CThostFtdcBulletinField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	int	BulletinID;
	///���к�
	int	SequenceNo;
	///��������
	TThostFtdcNewsTypeType	NewsType;
	///�����̶�
	char	NewsUrgency;
	///����ʱ��
	TThostFtdcTimeType	SendTime;
	///��ϢժҪ
	TThostFtdcAbstractType	Abstract;
	///��Ϣ��Դ
	TThostFtdcComeFromType	ComeFrom;
	///��Ϣ����
	TThostFtdcContentType	Content;
	///WEB��ַ
	TThostFtdcURLLinkType	URLLink;
	///�г�����
	TThostFtdcMarketIDType	MarketID;
};

///��ѯ����������
struct CThostFtdcQryBulletinField
{
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///������
	int	BulletinID;
	///���к�
	int	SequenceNo;
	///��������
	TThostFtdcNewsTypeType	NewsType;
	///�����̶�
	char	NewsUrgency;
};

///ת�ʿ�������
struct CThostFtdcReqOpenAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�Ա�
	char	Gender;
	///���Ҵ���
	TThostFtdcCountryCodeType	CountryCode;
	///�ͻ�����
	char	CustType;
	///��ַ
	TThostFtdcAddressType	Address;
	///�ʱ�
	TThostFtdcZipCodeType	ZipCode;
	///�绰����
	TThostFtdcTelephoneType	Telephone;
	///�ֻ�
	TThostFtdcMobilePhoneType	MobilePhone;
	///����
	TThostFtdcFaxType	Fax;
	///�����ʼ�
	TThostFtdcEMailType	EMail;
	///�ʽ��˻�״̬
	char	MoneyAccountStatus;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///��װ���
	int	InstallID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///�㳮��־
	char	CashExchangeCode;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///����ID
	int	TID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
};

///ת����������
struct CThostFtdcReqCancelAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�Ա�
	char	Gender;
	///���Ҵ���
	TThostFtdcCountryCodeType	CountryCode;
	///�ͻ�����
	char	CustType;
	///��ַ
	TThostFtdcAddressType	Address;
	///�ʱ�
	TThostFtdcZipCodeType	ZipCode;
	///�绰����
	TThostFtdcTelephoneType	Telephone;
	///�ֻ�
	TThostFtdcMobilePhoneType	MobilePhone;
	///����
	TThostFtdcFaxType	Fax;
	///�����ʼ�
	TThostFtdcEMailType	EMail;
	///�ʽ��˻�״̬
	char	MoneyAccountStatus;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///��װ���
	int	InstallID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///�㳮��־
	char	CashExchangeCode;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///����ID
	int	TID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
};

///��������˻�����
struct CThostFtdcReqChangeAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�Ա�
	char	Gender;
	///���Ҵ���
	TThostFtdcCountryCodeType	CountryCode;
	///�ͻ�����
	char	CustType;
	///��ַ
	TThostFtdcAddressType	Address;
	///�ʱ�
	TThostFtdcZipCodeType	ZipCode;
	///�绰����
	TThostFtdcTelephoneType	Telephone;
	///�ֻ�
	TThostFtdcMobilePhoneType	MobilePhone;
	///����
	TThostFtdcFaxType	Fax;
	///�����ʼ�
	TThostFtdcEMailType	EMail;
	///�ʽ��˻�״̬
	char	MoneyAccountStatus;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///�������ʺ�
	TThostFtdcBankAccountType	NewBankAccount;
	///����������
	TThostFtdcPasswordType	NewBankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///�����ʺ�����
	char	BankAccType;
	///��װ���
	int	InstallID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///����ID
	int	TID;
	///ժҪ
	TThostFtdcDigestType	Digest;
};

///ת������
struct CThostFtdcReqTransferField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	char	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///��װ���
	int	InstallID;
	///�ڻ���˾��ˮ��
	int	FutureSerial;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ת�ʽ��
	double	TradeAmount;
	///�ڻ���ȡ���
	double	FutureFetchAmount;
	///����֧����־
	char	FeePayFlag;
	///Ӧ�տͻ�����
	double	CustFee;
	///Ӧ���ڻ���˾����
	double	BrokerFee;
	///���ͷ������շ�����Ϣ
	TThostFtdcAddInfoType	Message;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///ת�˽���״̬
	char	TransferStatus;
};

///���з��������ʽ�ת�ڻ���Ӧ
struct CThostFtdcRspTransferField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	char	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///��װ���
	int	InstallID;
	///�ڻ���˾��ˮ��
	int	FutureSerial;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ת�ʽ��
	double	TradeAmount;
	///�ڻ���ȡ���
	double	FutureFetchAmount;
	///����֧����־
	char	FeePayFlag;
	///Ӧ�տͻ�����
	double	CustFee;
	///Ӧ���ڻ���˾����
	double	BrokerFee;
	///���ͷ������շ�����Ϣ
	TThostFtdcAddInfoType	Message;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///ת�˽���״̬
	char	TransferStatus;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///��������
struct CThostFtdcReqRepealField
{
	///����ʱ����
	int	RepealTimeInterval;
	///�Ѿ���������
	int	RepealedTimes;
	///���г�����־
	char	BankRepealFlag;
	///���̳�����־
	char	BrokerRepealFlag;
	///������ƽ̨��ˮ��
	int	PlateRepealSerial;
	///������������ˮ��
	TThostFtdcBankSerialType	BankRepealSerial;
	///�������ڻ���ˮ��
	int	FutureRepealSerial;
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	char	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///��װ���
	int	InstallID;
	///�ڻ���˾��ˮ��
	int	FutureSerial;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ת�ʽ��
	double	TradeAmount;
	///�ڻ���ȡ���
	double	FutureFetchAmount;
	///����֧����־
	char	FeePayFlag;
	///Ӧ�տͻ�����
	double	CustFee;
	///Ӧ���ڻ���˾����
	double	BrokerFee;
	///���ͷ������շ�����Ϣ
	TThostFtdcAddInfoType	Message;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///ת�˽���״̬
	char	TransferStatus;
};

///������Ӧ
struct CThostFtdcRspRepealField
{
	///����ʱ����
	int	RepealTimeInterval;
	///�Ѿ���������
	int	RepealedTimes;
	///���г�����־
	char	BankRepealFlag;
	///���̳�����־
	char	BrokerRepealFlag;
	///������ƽ̨��ˮ��
	int	PlateRepealSerial;
	///������������ˮ��
	TThostFtdcBankSerialType	BankRepealSerial;
	///�������ڻ���ˮ��
	int	FutureRepealSerial;
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	char	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///��װ���
	int	InstallID;
	///�ڻ���˾��ˮ��
	int	FutureSerial;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ת�ʽ��
	double	TradeAmount;
	///�ڻ���ȡ���
	double	FutureFetchAmount;
	///����֧����־
	char	FeePayFlag;
	///Ӧ�տͻ�����
	double	CustFee;
	///Ӧ���ڻ���˾����
	double	BrokerFee;
	///���ͷ������շ�����Ϣ
	TThostFtdcAddInfoType	Message;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///ת�˽���״̬
	char	TransferStatus;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///��ѯ�˻���Ϣ����
struct CThostFtdcReqQueryAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	char	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///�ڻ���˾��ˮ��
	int	FutureSerial;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
};

///��ѯ�˻���Ϣ��Ӧ
struct CThostFtdcRspQueryAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	char	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///�ڻ���˾��ˮ��
	int	FutureSerial;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///���п��ý��
	double	BankUseAmount;
	///���п�ȡ���
	double	BankFetchAmount;
};

///����ǩ��ǩ��
struct CThostFtdcFutureSignIOField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
};

///����ǩ����Ӧ
struct CThostFtdcRspFutureSignInField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
	///PIN��Կ
	TThostFtdcPasswordKeyType	PinKey;
	///MAC��Կ
	TThostFtdcPasswordKeyType	MacKey;
};

///����ǩ������
struct CThostFtdcReqFutureSignOutField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
};

///����ǩ����Ӧ
struct CThostFtdcRspFutureSignOutField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///��ѯָ����ˮ�ŵĽ��׽������
struct CThostFtdcReqQueryTradeResultBySerialField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///��ˮ��
	int	Reference;
	///����ˮ�ŷ����ߵĻ�������
	char	RefrenceIssureType;
	///����ˮ�ŷ����߻�������
	TThostFtdcOrganCodeType	RefrenceIssure;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	char	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ת�ʽ��
	double	TradeAmount;
	///ժҪ
	TThostFtdcDigestType	Digest;
};

///��ѯָ����ˮ�ŵĽ��׽����Ӧ
struct CThostFtdcRspQueryTradeResultBySerialField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
	///��ˮ��
	int	Reference;
	///����ˮ�ŷ����ߵĻ�������
	char	RefrenceIssureType;
	///����ˮ�ŷ����߻�������
	TThostFtdcOrganCodeType	RefrenceIssure;
	///ԭʼ���ش���
	TThostFtdcReturnCodeType	OriginReturnCode;
	///ԭʼ����������
	TThostFtdcDescrInfoForReturnCodeType	OriginDescrInfoForReturnCode;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ת�ʽ��
	double	TradeAmount;
	///ժҪ
	TThostFtdcDigestType	Digest;
};

///�����ļ���������
struct CThostFtdcReqDayEndFileReadyField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ļ�ҵ����
	char	FileBusinessCode;
	///ժҪ
	TThostFtdcDigestType	Digest;
};

///���ؽ��
struct CThostFtdcReturnResultField
{
	///���ش���
	TThostFtdcReturnCodeType	ReturnCode;
	///����������
	TThostFtdcDescrInfoForReturnCodeType	DescrInfoForReturnCode;
};

///��֤�ڻ��ʽ�����
struct CThostFtdcVerifyFuturePasswordField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///��װ���
	int	InstallID;
	///����ID
	int	TID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///��֤�ͻ���Ϣ
struct CThostFtdcVerifyCustInfoField
{
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	char	CustType;
};

///��֤�ڻ��ʽ�����Ϳͻ���Ϣ
struct CThostFtdcVerifyFuturePasswordAndCustInfoField
{
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	char	CustType;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///��֤�ڻ��ʽ�����Ϳͻ���Ϣ
struct CThostFtdcDepositResultInformField
{
	///�������ˮ�ţ�����ˮ��Ϊ���ڱ��̷��ص���ˮ��
	TThostFtdcDepositSeqNoType	DepositSeqNo;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///�����
	double	Deposit;
	///������
	int	RequestID;
	///���ش���
	TThostFtdcReturnCodeType	ReturnCode;
	///����������
	TThostFtdcDescrInfoForReturnCodeType	DescrInfoForReturnCode;
};

///���׺��������ڱ��̷�����Կͬ������
struct CThostFtdcReqSyncKeyField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///���׺��ĸ����ڱ��̵���Ϣ
	TThostFtdcAddInfoType	Message;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
};

///���׺��������ڱ��̷�����Կͬ����Ӧ
struct CThostFtdcRspSyncKeyField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///���׺��ĸ����ڱ��̵���Ϣ
	TThostFtdcAddInfoType	Message;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///��ѯ�˻���Ϣ֪ͨ
struct CThostFtdcNotifyQueryAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	char	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///�ڻ���˾��ˮ��
	int	FutureSerial;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///���п��ý��
	double	BankUseAmount;
	///���п�ȡ���
	double	BankFetchAmount;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///����ת�˽�����ˮ��
struct CThostFtdcTransferSerialField
{
	///ƽ̨��ˮ��
	int	PlateSerial;
	///���׷�������
	TThostFtdcTradeDateType	TradeDate;
	///��������
	TThostFtdcDateType	TradingDay;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///���״���
	TThostFtdcTradeCodeType	TradeCode;
	///�Ự���
	int	SessionID;
	///���б���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///�����ʺ�����
	char	BankAccType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///�ڻ���˾����
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///�ڻ���˾�ʺ�����
	char	FutureAccType;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///�ڻ���˾��ˮ��
	int	FutureSerial;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///���׽��
	double	TradeAmount;
	///Ӧ�տͻ�����
	double	CustFee;
	///Ӧ���ڻ���˾����
	double	BrokerFee;
	///��Ч��־
	char	AvailabilityFlag;
	///����Ա
	TThostFtdcOperatorCodeType	OperatorCode;
	///�������ʺ�
	TThostFtdcBankAccountType	BankNewAccount;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///�����ѯת����ˮ
struct CThostFtdcQryTransferSerialField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///���б���
	TThostFtdcBankIDType	BankID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///����ǩ��֪ͨ
struct CThostFtdcNotifyFutureSignInField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
	///PIN��Կ
	TThostFtdcPasswordKeyType	PinKey;
	///MAC��Կ
	TThostFtdcPasswordKeyType	MacKey;
};

///����ǩ��֪ͨ
struct CThostFtdcNotifyFutureSignOutField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///���׺��������ڱ��̷�����Կͬ����������֪ͨ
struct CThostFtdcNotifySyncKeyField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///��װ���
	int	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///���׺��ĸ����ڱ��̵���Ϣ
	TThostFtdcAddInfoType	Message;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	int	RequestID;
	///����ID
	int	TID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///�����ѯ����ǩԼ��ϵ
struct CThostFtdcQryAccountregisterField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///���б���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///�ͻ���������Ϣ��
struct CThostFtdcAccountregisterField
{
	///��������
	TThostFtdcTradeDateType	TradeDay;
	///���б���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///�ڻ���˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�ڻ���˾��֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///���������
	char	OpenOrDestroy;
	///ǩԼ����
	TThostFtdcTradeDateType	RegDate;
	///��Լ����
	TThostFtdcTradeDateType	OutDate;
	///����ID
	int	TID;
	///�ͻ�����
	char	CustType;
	///�����ʺ�����
	char	BankAccType;
};

///���ڿ�����Ϣ
struct CThostFtdcOpenAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�Ա�
	char	Gender;
	///���Ҵ���
	TThostFtdcCountryCodeType	CountryCode;
	///�ͻ�����
	char	CustType;
	///��ַ
	TThostFtdcAddressType	Address;
	///�ʱ�
	TThostFtdcZipCodeType	ZipCode;
	///�绰����
	TThostFtdcTelephoneType	Telephone;
	///�ֻ�
	TThostFtdcMobilePhoneType	MobilePhone;
	///����
	TThostFtdcFaxType	Fax;
	///�����ʼ�
	TThostFtdcEMailType	EMail;
	///�ʽ��˻�״̬
	char	MoneyAccountStatus;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///��װ���
	int	InstallID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///�㳮��־
	char	CashExchangeCode;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///����ID
	int	TID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///����������Ϣ
struct CThostFtdcCancelAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�Ա�
	char	Gender;
	///���Ҵ���
	TThostFtdcCountryCodeType	CountryCode;
	///�ͻ�����
	char	CustType;
	///��ַ
	TThostFtdcAddressType	Address;
	///�ʱ�
	TThostFtdcZipCodeType	ZipCode;
	///�绰����
	TThostFtdcTelephoneType	Telephone;
	///�ֻ�
	TThostFtdcMobilePhoneType	MobilePhone;
	///����
	TThostFtdcFaxType	Fax;
	///�����ʼ�
	TThostFtdcEMailType	EMail;
	///�ʽ��˻�״̬
	char	MoneyAccountStatus;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///��װ���
	int	InstallID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///�㳮��־
	char	CashExchangeCode;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///����ID
	int	TID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///���ڱ�������˺���Ϣ
struct CThostFtdcChangeAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�Ա�
	char	Gender;
	///���Ҵ���
	TThostFtdcCountryCodeType	CountryCode;
	///�ͻ�����
	char	CustType;
	///��ַ
	TThostFtdcAddressType	Address;
	///�ʱ�
	TThostFtdcZipCodeType	ZipCode;
	///�绰����
	TThostFtdcTelephoneType	Telephone;
	///�ֻ�
	TThostFtdcMobilePhoneType	MobilePhone;
	///����
	TThostFtdcFaxType	Fax;
	///�����ʼ�
	TThostFtdcEMailType	EMail;
	///�ʽ��˻�״̬
	char	MoneyAccountStatus;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///�������ʺ�
	TThostFtdcBankAccountType	NewBankAccount;
	///����������
	TThostFtdcPasswordType	NewBankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///�����ʺ�����
	char	BankAccType;
	///��װ���
	int	InstallID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///����ID
	int	TID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///�����������Ա����Ȩ��
struct CThostFtdcSecAgentACIDMapField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�ʽ��˻�
	TThostFtdcAccountIDType	AccountID;
	///����
	TThostFtdcCurrencyIDType	CurrencyID;
	///�����н�����ʽ��ʺ�
	TThostFtdcAccountIDType	BrokerSecAgentID;
};

///�����������Ա����Ȩ�޲�ѯ
struct CThostFtdcQrySecAgentACIDMapField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�ʽ��˻�
	TThostFtdcAccountIDType	AccountID;
	///����
	TThostFtdcCurrencyIDType	CurrencyID;
};

///�ֱ����Ľ���Ȩ��
struct CThostFtdcUserRightsAssignField
{
	///Ӧ�õ�Ԫ����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�������Ĵ���
	int	DRIdentityID;
};

///���ù�˾�Ƿ����ڱ���ʾ�Ľ���Ȩ��
struct CThostFtdcBrokerUserRightAssignField
{
	///Ӧ�õ�Ԫ����
	TThostFtdcBrokerIDType	BrokerID;
	///�������Ĵ���
	int	DRIdentityID;
	///�ܷ���
	int	Tradeable;
};

///�ֱ�����ת������
struct CThostFtdcDRTransferField
{
	///ԭ�������Ĵ���
	int	OrigDRIdentityID;
	///Ŀ�꽻�����Ĵ���
	int	DestDRIdentityID;
	///ԭӦ�õ�Ԫ����
	TThostFtdcBrokerIDType	OrigBrokerID;
	///Ŀ�����õ�Ԫ����
	TThostFtdcBrokerIDType	DestBrokerID;
};

///Fens�û���Ϣ
struct CThostFtdcFensUserInfoField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��¼ģʽ
	char	LoginMode;
};

///��ǰ����������������
struct CThostFtdcCurrTransferIdentityField
{
	///�������Ĵ���
	int	IdentityID;
};

///��ֹ��¼�û�
struct CThostFtdcLoginForbiddenUserField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///IP��ַ
	TThostFtdcIPAddressType	IPAddress;
};

///��ѯ��ֹ��¼�û�
struct CThostFtdcQryLoginForbiddenUserField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
};

///UDP�鲥����Ϣ
struct CThostFtdcMulticastGroupInfoField
{
	///�鲥��IP��ַ
	TThostFtdcIPAddressType	GroupIP;
	///�鲥��IP�˿�
	int	GroupPort;
	///Դ��ַ
	TThostFtdcIPAddressType	SourceIP;
};

///�ʽ��˻�����׼����
struct CThostFtdcTradingAccountReserveField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///����׼����
	double	Reserve;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};

///����ԤԼ����ȷ������
struct CThostFtdcReserveOpenAccountConfirmField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�Ա�
	char	Gender;
	///���Ҵ���
	TThostFtdcCountryCodeType	CountryCode;
	///�ͻ�����
	char	CustType;
	///��ַ
	TThostFtdcAddressType	Address;
	///�ʱ�
	TThostFtdcZipCodeType	ZipCode;
	///�绰����
	TThostFtdcTelephoneType	Telephone;
	///�ֻ�
	TThostFtdcMobilePhoneType	MobilePhone;
	///����
	TThostFtdcFaxType	Fax;
	///�����ʼ�
	TThostFtdcEMailType	EMail;
	///�ʽ��˻�״̬
	char	MoneyAccountStatus;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///��װ���
	int	InstallID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///����ID
	int	TID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///ԤԼ����������ˮ��
	TThostFtdcBankSerialType	BankReserveOpenSeq;
	///ԤԼ��������
	TThostFtdcTradeDateType	BookDate;
	///ԤԼ������֤����
	TThostFtdcPasswordType	BookPsw;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};

///����ԤԼ����
struct CThostFtdcReserveOpenAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	char	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�Ա�
	char	Gender;
	///���Ҵ���
	TThostFtdcCountryCodeType	CountryCode;
	///�ͻ�����
	char	CustType;
	///��ַ
	TThostFtdcAddressType	Address;
	///�ʱ�
	TThostFtdcZipCodeType	ZipCode;
	///�绰����
	TThostFtdcTelephoneType	Telephone;
	///�ֻ�
	TThostFtdcMobilePhoneType	MobilePhone;
	///����
	TThostFtdcFaxType	Fax;
	///�����ʼ�
	TThostFtdcEMailType	EMail;
	///�ʽ��˻�״̬
	char	MoneyAccountStatus;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///��װ���
	int	InstallID;
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	char	BankAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///����ID
	int	TID;
	///ԤԼ����״̬
	char	ReserveOpenAccStas;
	///�������
	int	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};



#endif

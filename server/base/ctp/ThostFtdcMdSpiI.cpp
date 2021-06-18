/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 汇海基金
///@file ThostFtdcMdApiSpiI.cpp
///@brief 定义了客户端接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#include "ThostFtdcMdSpiI.h"
#include <stdlib.h>
#include "util.h"

string CThostFtdcMdSpiI::OnFrontDisconnected_Text(int nReason)
{
	switch(nReason)
	{
		case 0x1001:
			return "网络读失败";
		case 0x1002:
			return "网络写失败";
		case 0x2001:
			return "接收心跳超时";
		case 0x2002:
			return "发送心跳失败";
		case 0x2003:
			return "收到错误报文";
		default:
			return "";
	}
}


CThostFtdcMdSpiI::CThostFtdcMdSpiI(CThostFtdcMdApi* pMdApi, string BrokenID, string AccountID, string AccountPWD, set<string> InsIds, ProcessMdInfo fun)
{
	LOG_INFO("CThostFtdcMdSpiI init executed.");
	_Fun	= fun;
	_pMdApi = pMdApi;
	strncpy(userLoginField.BrokerID, BrokenID.c_str(), sizeof(userLoginField.BrokerID));
	strncpy(userLoginField.UserID, AccountID.c_str(), sizeof(userLoginField.UserID));
	strncpy(userLoginField.Password, AccountPWD.c_str(), sizeof(userLoginField.Password));

	LOG_DEBUG("BrokerID:" << userLoginField.BrokerID << " UserID:" << userLoginField.UserID << " Password:" << userLoginField.Password << " count:" << InsIds.size());

	insCount = InsIds.size();
	InstrumnetIds = new char*[insCount];
	typeof(InsIds.begin()) itr = InsIds.begin();
	for(int i=0; itr!=InsIds.end(); itr++, i++)
	{
		string id = *itr;
		int len = id.size();
		InstrumnetIds[i] = new char[len+1];
		memset(InstrumnetIds[i], 0, len+1);
		strncpy(InstrumnetIds[i], id.c_str(), len);
		
		LOG_DEBUG("InstrumnetIds[" << i << "]: " << InstrumnetIds[i]);
	}

}


CThostFtdcMdSpiI::~CThostFtdcMdSpiI()
{
	if(InstrumnetIds!=NULL) delete[] InstrumnetIds;
}


void CThostFtdcMdSpiI::ChangeSubcribe(set<string> InsIds)
{
	if(InstrumnetIds != NULL) delete[] InstrumnetIds;

	insCount = InsIds.size();
	InstrumnetIds = new char*[insCount];
	typeof(InsIds.begin()) itr = InsIds.begin();
	for(int i=0; itr!=InsIds.end(); itr++, i++)
	{
		string id = *itr;
		int len = id.size();
		InstrumnetIds[i] = new char[len+1];
		memset(InstrumnetIds[i], 0, len+1);
		strncpy(InstrumnetIds[i], id.c_str(), len);
		
		LOG_DEBUG("InstrumnetIds[" << i << "]: " << InstrumnetIds[i]);
	}
	int result = _pMdApi->SubscribeMarketData(InstrumnetIds, insCount);
	if(result < 0)		
		LOG_ERROR("SubscribeMarketData executed Error. result:" <<  result);
}


void CThostFtdcMdSpiI::OnFrontConnected()
{
	//********logon to ctp md server
	//登录请求结构体
	int loginid = getRequestId();
	ReqUserLogin(&userLoginField, loginid);
	//LOG_DEBUG("logon the CTP md server. loginid:" << loginid << " user=" << userLoginField.UserID);
}
		
void CThostFtdcMdSpiI::OnFrontDisconnected(int nReason)
{
	LOG_DEBUG("OnFrontDisconnected executed[" << nReason << "]" << OnFrontDisconnected_Text(nReason));
}

void CThostFtdcMdSpiI::OnHeartBeatWarning(int nTimeLapse)
{
	LOG_DEBUG("OnHeartBeatWarning executed timespan:" << nTimeLapse);
}

void CThostFtdcMdSpiI::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	struct tm tmp = {0};
	time_t now = time(NULL);
	localtime_r(&now,  &tmp);
	uint32_t local = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec;

	char ErrorMsg[243];
    GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
    char UserID[16];
    GBK2UTF8(pRspUserLogin->UserID, strlen(pRspUserLogin->UserID), UserID, sizeof(UserID));
	
	//判断登录是否成功 失败告警 （继续登录）
	if (pRspInfo->ErrorID == 0) 
	{
        LOG_DEBUG("OnRspUserLogin executed. nRequestID:" << nRequestID << ", UserID:"<< UserID << ", ErrorMsg:" << ErrorMsg);
        //sem_post(&sem);
    } 
	else 
	{
        LOG_ERROR("OnRspUserLogin executed Error. nRequestID:" << nRequestID << ", UserID:"<< UserID << ", ErrorMsg:" << ErrorMsg);
		//登录失败再次请求登录
		int loginid = getRequestId();
		ReqUserLogin(&userLoginField, loginid); 
		//LOG_DEBUG("retry logon the CTP md server. loginid:" << loginid);
		return;
    }

	LOG_INFO("SHFETime="	<<	pRspUserLogin->SHFETime
			<<" DCETime="	<<	pRspUserLogin->DCETime
			<<" CZCETime="	<<	pRspUserLogin->CZCETime
			<<" FFEXTime="	<<	pRspUserLogin->FFEXTime
			<<" TradingDay="<<	pRspUserLogin->TradingDay
			<<" LoginTime="	<<	pRspUserLogin->LoginTime
			<<" BrokerID="	<<	pRspUserLogin->BrokerID
			<<" UserID="	<<	pRspUserLogin->UserID
			<<" SystemName="<<	pRspUserLogin->SystemName
			<<" FrontID="	<<	pRspUserLogin->FrontID
			<<" SessionID="	<<	pRspUserLogin->SessionID
			<<" MaxOrderRef="<<	pRspUserLogin->MaxOrderRef
			<<" INETime="	<<	pRspUserLogin->INETime
			<<" ErrorMsg="	<<	ErrorMsg);

	memset(&tmp, 0, sizeof(tmp));
	strptime(pRspUserLogin->SHFETime , "%H:%M:%S", &tmp);
	DiffSHFETime = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec - local;

	memset(&tmp, 0, sizeof(tmp));
	strptime(pRspUserLogin->DCETime , "%H:%M:%S", &tmp);
	DiffDCETime = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec - local;

	memset(&tmp, 0, sizeof(tmp));
	strptime(pRspUserLogin->CZCETime , "%H:%M:%S", &tmp);
	DiffCZCETime = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec - local;

	memset(&tmp, 0, sizeof(tmp));
	strptime(pRspUserLogin->FFEXTime , "%H:%M:%S", &tmp);
	DiffFFEXTime = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec - local;

    if(DiffSHFETime + local == 0) DiffSHFETime = 0;
    if(DiffDCETime + local == 0) DiffDCETime = 0;
    if(DiffCZCETime + local == 0) DiffCZCETime = 0;
    if(DiffFFEXTime + local == 0) DiffFFEXTime = 0;
	
    LOG_INFO("DiffSHFE="<<DiffSHFETime<<" DiffDCE="<<DiffDCETime<<" DiffCZCE="<<DiffCZCETime<<" DiffFFEX="<<DiffFFEXTime<<" local:"<<local);

	//登录成功 订阅市场数据
	int result = _pMdApi->SubscribeMarketData(InstrumnetIds, insCount);
	LOG_INFO("SubscribeMarketData Count:" << insCount);
	if(result < 0)
	{
		LOG_ERROR("SubscribeMarketData executed Error. result:" <<  result);
	}

}

void CThostFtdcMdSpiI::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	char ErrorMsg[243];
    GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
    char UserID[16];
    GBK2UTF8(pUserLogout->UserID, strlen(pUserLogout->UserID), UserID, sizeof(UserID));

    LOG_DEBUG("OnRspUserLogout executed. nRequestID:" << nRequestID << ", UserID:"<< UserID << ", ErrorMsg:" << ErrorMsg);
    //sem_post(&sem);
}

void CThostFtdcMdSpiI::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	char ErrorMsg[243];
    GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
    LOG_ERROR("nRequestID  executed Error. nRequestID:" << nRequestID << "[" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg << "]");
    //sem_post(&sem);
}

void CThostFtdcMdSpiI::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	char ErrorMsg[243];
    GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
    char InstrumentID[16];
    GBK2UTF8(pSpecificInstrument->InstrumentID, strlen(pSpecificInstrument->InstrumentID), InstrumentID, sizeof(InstrumentID));

	if (pRspInfo->ErrorID == 0) 
	{
		LOG_DEBUG("OnRspSubMarketData executed. nRequestID:" << nRequestID << ", InstrumentID:"<< InstrumentID << ", ErrorMsg:" << ErrorMsg);
    }
	else
	{
		LOG_ERROR("OnRspSubMarketData executed. nRequestID:" << nRequestID << ", InstrumentID:"<< InstrumentID << ", ErrorMsg:" << ErrorMsg);
		//订阅失败在订阅
		int result = _pMdApi->SubscribeMarketData(InstrumnetIds, insCount);
		if(result < 0)
		{
			LOG_ERROR("SubscribeMarketData executed Error. result:" <<  result);
		}
	}

    //sem_post(&sem);
}

void CThostFtdcMdSpiI::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	char ErrorMsg[243];
    GBK2UTF8(pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg), ErrorMsg, sizeof(ErrorMsg));
    char InstrumentID[16];
    GBK2UTF8(pSpecificInstrument->InstrumentID, strlen(pSpecificInstrument->InstrumentID), InstrumentID, sizeof(InstrumentID));

	if (pRspInfo->ErrorID == 0) 
	{
		LOG_DEBUG("OnRspUnSubMarketData executed. nRequestID:" << nRequestID << ", InstrumentID:"<< InstrumentID << ", ErrorMsg:" << ErrorMsg);
    }
	else
	{
		LOG_ERROR("OnRspUnSubMarketData executed. nRequestID:" << nRequestID  << ", ErrorMsg:" << ErrorMsg);
	}

    //sem_post(&sem);
}

void CThostFtdcMdSpiI::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	//做市商用到的
}

void CThostFtdcMdSpiI::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	//做市商用到的
}

void CheckData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{

#define IsValidDouble(value) if((value)<0 || (value)>1000000000000) value = 0; //0-------10000亿
#define IsValidInt(value) if((value)<0 || (value)>2100000000) value = 0; //0-------21亿
		//pDepthMarketData->TradingDay          
		//pDepthMarketData->InstrumentID        
		//pDepthMarketData->ExchangeID          
		//pDepthMarketData->ExchangeInstID      
		IsValidDouble(pDepthMarketData->LastPrice);          
		IsValidDouble(pDepthMarketData->PreSettlementPrice);  
		IsValidDouble(pDepthMarketData->PreClosePrice);       
		IsValidDouble(pDepthMarketData->PreOpenInterest);     
		IsValidDouble(pDepthMarketData->OpenPrice);           
		IsValidDouble(pDepthMarketData->HighestPrice);        
		IsValidDouble(pDepthMarketData->LowestPrice);         
		IsValidInt(pDepthMarketData->Volume);              
		IsValidDouble(pDepthMarketData->Turnover);       
		IsValidDouble(pDepthMarketData->OpenInterest);        
		IsValidDouble(pDepthMarketData->ClosePrice);        
		IsValidDouble(pDepthMarketData->SettlementPrice);  
		IsValidDouble(pDepthMarketData->UpperLimitPrice);   
		IsValidDouble(pDepthMarketData->LowerLimitPrice);   
		IsValidDouble(pDepthMarketData->PreDelta);   
		IsValidDouble(pDepthMarketData->CurrDelta);          
		//pDepthMarketData->UpdateTime          
		//pDepthMarketData->UpdateMillisec      
		IsValidDouble(pDepthMarketData->BidPrice1);    
		IsValidInt(pDepthMarketData->BidVolume1);         
		IsValidDouble(pDepthMarketData->AskPrice1);        
		IsValidInt(pDepthMarketData->AskVolume1);         
		IsValidDouble(pDepthMarketData->BidPrice2 );          
		IsValidInt(pDepthMarketData->BidVolume2);          
		IsValidDouble(pDepthMarketData->AskPrice2 );          
		IsValidInt(pDepthMarketData->AskVolume2);          
		IsValidDouble(pDepthMarketData->BidPrice3 );          
		IsValidInt(pDepthMarketData->BidVolume3);          
		IsValidDouble(pDepthMarketData->AskPrice3 );          
		IsValidInt(pDepthMarketData->AskVolume3);          
		IsValidDouble(pDepthMarketData->BidPrice4 );          
		IsValidInt(pDepthMarketData->BidVolume4);          
		IsValidDouble(pDepthMarketData->AskPrice4 );          
		IsValidInt(pDepthMarketData->AskVolume4);          
		IsValidDouble(pDepthMarketData->BidPrice5 );          
		IsValidInt(pDepthMarketData->BidVolume5);          
		IsValidDouble(pDepthMarketData->AskPrice5 );          
		IsValidInt(pDepthMarketData->AskVolume5);          
		IsValidDouble(pDepthMarketData->AveragePrice);        
		//pDepthMarketData->ActionDay				
}

/*
def preprocess_daybar(d):
        if(d['close'] < 0.001): d['close'] = d['price']
        if(d['open'] > 0.001): return True
        p = 0
        if(d['preclose'] > 0.001):
            p = d['preclose']
        elif(d['presettlement'] > 0.001):
            p = d['presettlement']
        else:
            p = d['price']
        d['open']  = p
        d['high']  = p
        d['low']   = p
        return True
*/

void preprocess(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
    //printf("%s %f %f\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->ClosePrice);
    if(pDepthMarketData->LastPrice > 0.001) pDepthMarketData->ClosePrice = pDepthMarketData->LastPrice;
    if(pDepthMarketData->OpenPrice < 0.001)
    {
        double p = 0;
        if(pDepthMarketData->PreClosePrice > 0.001) 
            p = pDepthMarketData->PreClosePrice;
        else if(pDepthMarketData->PreSettlementPrice > 0.001) 
            p = pDepthMarketData->PreSettlementPrice;
        else 
            p = pDepthMarketData->LastPrice;
        pDepthMarketData->OpenPrice    = p;
        pDepthMarketData->HighestPrice = p;
        pDepthMarketData->LowestPrice  = p;
    }
}

void CThostFtdcMdSpiI::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) 
{
    // 如果有返回结果读取返回信息
    if ( pDepthMarketData  != NULL ) 
	{
		CheckData(pDepthMarketData);
        
        preprocess(pDepthMarketData);
		
        //LOG_MD_INFO(pDepthMarketData);
		
		static int num = 0;
		if(num++ % 1000 == 0) LOG_INFO("write pDepthMarketData.");
		
		if(_Fun != NULL)
		{
			//几家交易所的夜市 tradingday和actionday不一致 这里统一
			snprintf(pDepthMarketData->TradingDay,  sizeof(pDepthMarketData->TradingDay), "%s", GetTradingDay());
		
			string TradingDay = pDepthMarketData->TradingDay;
			string ActionDay = pDepthMarketData->ActionDay;

			//夜盘的ActionDay要自己确定 因为交易所的不对 09:51:19
			if(pDepthMarketData->UpdateTime[0] == '2') //20--24点以后夜市 ActionDay是当天。
				snprintf(pDepthMarketData->ActionDay, sizeof(pDepthMarketData->ActionDay), "%s", stamptostr(time(NULL)-3600 * 5, "%Y%m%d").c_str()); 
			else if(pDepthMarketData->UpdateTime[0] == '0' && pDepthMarketData->UpdateTime[1]>='0' && pDepthMarketData->UpdateTime[1]<'3') //0--3点之间
				snprintf(pDepthMarketData->ActionDay, sizeof(pDepthMarketData->ActionDay), "%s", stamptostr(time(NULL)+3600, "%Y%m%d").c_str());
			else	//其余时间
				snprintf(pDepthMarketData->ActionDay,  sizeof(pDepthMarketData->ActionDay), "%s", GetTradingDay());


            static int count =0;
            if(count++ % 10000 == 0)
				LOG_INFO("InstrumentID=" << pDepthMarketData->InstrumentID << " TradingDay=" << TradingDay << ":" << pDepthMarketData->TradingDay << " ActionDay=" << ActionDay << ":" << pDepthMarketData->ActionDay << " Time=" << pDepthMarketData->UpdateTime << " Millisec=" << pDepthMarketData->UpdateMillisec);

			(*_Fun)((char*)pDepthMarketData, (uint32_t)sizeof(CThostFtdcDepthMarketDataField));
		}
		else
		{
			LOG_ERROR("OnRtnDepthMarketData have no callback.");
			printf("[%s:%s:%d] OnRtnDepthMarketData have no callback.", __FILE__, __FUNCTION__, __LINE__);
			exit(-1);
		}
	}
	else
	{
		LOG_ERROR("pDepthMarketData is null. No data.");
	}
	//清空
//	memset(pDepthMarketData, 0, sizeof(CThostFtdcDepthMarketDataField));
}

void CThostFtdcMdSpiI::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) 
{
	//询价通知 做市商用到
}

void CThostFtdcMdSpiI::LOG_MD_INFO(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	LOG_DEBUG("pDepthMarketData\n\t" <<
		"TradingDay        : " << pDepthMarketData->TradingDay          << "\n\t" <<
		"InstrumentID      : " << pDepthMarketData->InstrumentID        << "\n\t" <<
		"ExchangeID        : " << pDepthMarketData->ExchangeID          << "\n\t" <<
		"ExchangeInstID    : " << pDepthMarketData->ExchangeInstID      << "\n\t" <<
		"LastPrice         : " << pDepthMarketData->LastPrice           << "\n\t" <<
		"PreSettlementPrice: " << pDepthMarketData->PreSettlementPrice  << "\n\t" <<
		"PreClosePrice     : " << pDepthMarketData->PreClosePrice       << "\n\t" <<
		"PreOpenInterest   : " << pDepthMarketData->PreOpenInterest     << "\n\t" <<
		"OpenPrice         : " << pDepthMarketData->OpenPrice           << "\n\t" <<
		"HighestPrice      : " << pDepthMarketData->HighestPrice        << "\n\t" <<
		"LowestPrice       : " << pDepthMarketData->LowestPrice         << "\n\t" <<
		"Volume            : " << pDepthMarketData->Volume              << "\n\t" <<
		"Turnover          : " << pDepthMarketData->Turnover            << "\n\t" <<
		"OpenInterest      : " << pDepthMarketData->OpenInterest        << "\n\t" <<
		"ClosePrice        : " << pDepthMarketData->ClosePrice          << "\n\t" <<
		"SettlementPrice   : " << pDepthMarketData->SettlementPrice     << "\n\t" <<
		"UpperLimitPrice   : " << pDepthMarketData->UpperLimitPrice     << "\n\t" <<
		"LowerLimitPrice   : " << pDepthMarketData->LowerLimitPrice     << "\n\t" <<
		"PreDelta          : " << pDepthMarketData->PreDelta            << "\n\t" <<
		"CurrDelta         : " << pDepthMarketData->CurrDelta           << "\n\t" <<
		"UpdateTime        : " << pDepthMarketData->UpdateTime          << "\n\t" <<
		"UpdateMillisec    : " << pDepthMarketData->UpdateMillisec      << "\n\t" <<
		"BidPrice1         : " << pDepthMarketData->BidPrice1           << "\n\t" <<
		"BidVolume1        : " << pDepthMarketData->BidVolume1          << "\n\t" <<
		"AskPrice1         : " << pDepthMarketData->AskPrice1           << "\n\t" <<
		"AskVolume1        : " << pDepthMarketData->AskVolume1          << "\n\t" <<
		"BidPrice2         : " << pDepthMarketData->BidPrice2           << "\n\t" <<
		"BidVolume2        : " << pDepthMarketData->BidVolume2          << "\n\t" <<
		"AskPrice2         : " << pDepthMarketData->AskPrice2           << "\n\t" <<
		"AskVolume2        : " << pDepthMarketData->AskVolume2          << "\n\t" <<
		"BidPrice3         : " << pDepthMarketData->BidPrice3           << "\n\t" <<
		"BidVolume3        : " << pDepthMarketData->BidVolume3          << "\n\t" <<
		"AskPrice3         : " << pDepthMarketData->AskPrice3           << "\n\t" <<
		"AskVolume3        : " << pDepthMarketData->AskVolume3          << "\n\t" <<
		"BidPrice4         : " << pDepthMarketData->BidPrice4           << "\n\t" <<
		"BidVolume4        : " << pDepthMarketData->BidVolume4          << "\n\t" <<
		"AskPrice4         : " << pDepthMarketData->AskPrice4           << "\n\t" <<
		"AskVolume4        : " << pDepthMarketData->AskVolume4          << "\n\t" <<
		"BidPrice5         : " << pDepthMarketData->BidPrice5           << "\n\t" <<
		"BidVolume5        : " << pDepthMarketData->BidVolume5          << "\n\t" <<
		"AskPrice5         : " << pDepthMarketData->AskPrice5           << "\n\t" <<
		"AskVolume5        : " << pDepthMarketData->AskVolume5          << "\n\t" <<
		"AveragePrice      : " << pDepthMarketData->AveragePrice        << "\n\t" <<
		"ActionDay         : " << pDepthMarketData->ActionDay);
}







////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 汇海基金
///@file ThostFtdcMdApiSpi.h
///@brief 定义了客户端接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __THOSTFTDCMDSPI_H__
#define __THOSTFTDCMDSPI_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <set>
#include <string>
#include <sys/time.h>
#include "versions.h"
#include "util.h"
#include "RollLog.h"
#include "convert.h"
#include "CtpJsonStruct.h"

using namespace std;


typedef int  (*ProcessMdInfo)(const char* pin, uint32_t len);

class CThostFtdcMdSpiI : public CThostFtdcMdSpi 
{
	private:
		string OnFrontDisconnected_Text(int nReason);
	    int getRequestId() //生成一个唯一登录id	
		{
			time_t tmp = time(NULL) +  3600*5; //时间前移5个小时 保证一个tradingday内单调递增
			
			int32_t hms = atoll(stamptostr(tmp, "1%H%M%S000").c_str());
			
			static int  id = 1; //1 --- 1000之间
			if(id > 999) id = 1;

			return (hms+id++);
		}

        int ReqUserLogin(CThostFtdcReqUserLoginField* puserLoginField, int loginid)
        {
            int ret = -1;
            while(ret < 0)
            {
                ret = _pMdApi->ReqUserLogin(puserLoginField, loginid);
		        LOG_DEBUG("logon the CTP td server. loginid:" << loginid << " ret=" << ret);
                if(ret < 0) sleep(3);
            }
            return 0;
        }


	public:
		CThostFtdcMdSpiI();
		~CThostFtdcMdSpiI();
		//初始化
		CThostFtdcMdSpiI(CThostFtdcMdApi* pMdApi, string BrokenID, string AccountID, string AccountPWD, set<string> InsIds, ProcessMdInfo fun=NULL);

		void ChangeSubcribe(set<string> InsIds);
		
		virtual void OnFrontConnected();
		
		///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
		///@param nReason 错误原因
		///        0x1001 网络读失败
		///        0x1002 网络写失败
		///        0x2001 接收心跳超时
		///        0x2002 发送心跳失败
		///        0x2003 收到错误报文
		virtual void OnFrontDisconnected(int nReason);
			
		///心跳超时警告。当长时间未收到报文时，该方法被调用。
		///@param nTimeLapse 距离上次接收报文的时间
		virtual void OnHeartBeatWarning(int nTimeLapse);
		

		///登录请求响应
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

		///登出请求响应
		virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

		///错误应答
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

		///订阅行情应答
		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

		///取消订阅行情应答
		virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

		///订阅询价应答
		virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

		///取消订阅询价应答
		virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

		///深度行情通知
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) ;

		///询价通知
		virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;

		//输出行情到日志里面
		void LOG_MD_INFO(CThostFtdcDepthMarketDataField* pDepthMarketData);

		const char* GetTradingDay()
		{
			return _pMdApi->GetTradingDay();
		}
		/*
		//CThostFtdcRspUserLoginField rep_logon;
		TThostFtdcDateType  TradingDay;
		 ///登录成功时间
		TThostFtdcTimeType  LoginTime;
		///经纪公司代码
		//TThostFtdcBrokerIDType  BrokerID;
		/////用户代码
		//TThostFtdcUserIDType    UserID;
		/////交易系统名称
		//TThostFtdcSystemNameType    SystemName;
		/////前置编号
		//int FrontID;
		/////会话编号
		//int SessionID;
		/////最大报单引用                                       
		//TThostFtdcOrderRefType  MaxOrderRef;
		/////上期所时间
		//TThostFtdcTimeType  SHFETime;
		/////大商所时间
		//TThostFtdcTimeType  DCETime;
		/////郑商所时间
		//TThostFtdcTimeType  CZCETime;
		/////中金所时间
		//TThostFtdcTimeType  FFEXTime;
		/////能源中心时间
		//TThostFtdcTimeType  INETime;
		*/
		uint32_t DiffSHFETime;
		uint32_t DiffDCETime;
		uint32_t DiffCZCETime;
		uint32_t DiffFFEXTime;
	

		uint32_t GetExchangeTime(string market, uint32_t t=time(NULL))
		{
			return t; //采用本地时间	
			
			//废弃 行情里面都是空的。 交易里面正常
			if(market == "SHFE") return t + DiffSHFETime;
			else if(market == "DCE") return t + DiffDCETime;
			else if(market == "CZCE") return t + DiffCZCETime;
			else if(market == "CFFEX") return t + DiffFFEXTime;
			else return t;
		}
	private:
		CThostFtdcMdApi* 	_pMdApi; 		//交易请求结构体
		CThostFtdcReqUserLoginField userLoginField; //登录结构体
		char**	InstrumnetIds;		//合约列表
		int		insCount;			//合约数量
		ProcessMdInfo _Fun;			//行情数据回调函数
};

#endif







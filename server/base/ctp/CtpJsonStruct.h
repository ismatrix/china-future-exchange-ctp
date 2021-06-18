/*************************************************************************
    > File Name: CtpJsonStruct.h
    > Created Time: Tue 31 May 2016 04:06:19 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#ifndef __CTP_JSON_STRUCT__
#define __CTP_JSON_STRUCT__

#include <iostream>
#include <string.h>

#include <json/json.h>
#include "RollLog.h"
#include "versions.h"


using namespace std;

//资金 struct--->json
Json::Value& get_CThostFtdcTradingAccountField(Json::Value& root, CThostFtdcTradingAccountField *p);
//持仓
Json::Value& get_CThostFtdcInvestorPositionField(Json::Value& root, CThostFtdcInvestorPositionField *p);
//持仓明细
Json::Value& get_CThostFtdcInvestorPositionDetailField(Json::Value& root, CThostFtdcInvestorPositionDetailField *p);
//成交
Json::Value& get_CThostFtdcTradeField(Json::Value& root, CThostFtdcTradeField *p);
//报单通知
Json::Value& get_CThostFtdcOrderField(Json::Value& root, CThostFtdcOrderField *p);
//合约状态通知
Json::Value& get_CThostFtdcInstrumentStatusField(Json::Value& root, CThostFtdcInstrumentStatusField *p);
//产品
Json::Value& get_CThostFtdcProductField(Json::Value& root, CThostFtdcProductField *p);
//合约
Json::Value& get_CThostFtdcInstrumentField(Json::Value& root, CThostFtdcInstrumentField *p);
//下单
Json::Value& get_CThostFtdcInputOrderField(Json::Value& root, CThostFtdcInputOrderField *p);
//撤单
Json::Value& get_CThostFtdcInputOrderActionField(Json::Value& root, CThostFtdcInputOrderActionField *p);

//资金 json---->struct
CThostFtdcTradingAccountField* get_CThostFtdcTradingAccountField_Struct(Json::Value& root, CThostFtdcTradingAccountField *p);
//持仓
CThostFtdcInvestorPositionField* get_CThostFtdcInvestorPositionField_Struct(Json::Value& root, CThostFtdcInvestorPositionField *p);
//持仓明细
CThostFtdcInvestorPositionDetailField* get_CThostFtdcInvestorPositionDetailField_Struct(Json::Value& root, CThostFtdcInvestorPositionDetailField *p);
//成交
CThostFtdcTradeField* get_CThostFtdcTradeField_Struct(Json::Value& root, CThostFtdcTradeField *p);
//合约状态通知
CThostFtdcInstrumentStatusField* get_CThostFtdcInstrumentStatusField_Struct(Json::Value& root, CThostFtdcInstrumentStatusField *p);
//报单通知
CThostFtdcOrderField* get_CThostFtdcOrderField_Struct(Json::Value& root, CThostFtdcOrderField *p);
//产品
CThostFtdcProductField* get_CThostFtdcProductField_Struct(Json::Value& root, CThostFtdcProductField *p);
//合约
CThostFtdcInstrumentField* get_CThostFtdcInstrumentField_Struct(Json::Value& root, CThostFtdcInstrumentField *p);
//下单
//CThostFtdcInputOrderField* get_CThostFtdcInputOrderField_Struct(Json::Value& root, CThostFtdcInputOrderField *p);
//撤单
//CThostFtdcInputOrderActionField* get_CThostFtdcInputOrderActionField_Struct(Json::Value& root, CThostFtdcInputOrderActionField *p);

Json::Value& get_CThostFtdcBulletinField(Json::Value& root, CThostFtdcBulletinField *p);
CThostFtdcBulletinField* get_CThostFtdcBulletinField_Struct(Json::Value& root, CThostFtdcBulletinField *p);


#endif


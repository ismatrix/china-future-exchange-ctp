/*************************************************************************
    > File Name: StJsonStruct.h
    > Created Time: Tue 31 May 2016 04:12:47 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#ifndef __ST_JSON_STRUCT__
#define __ST_JSON_STRUCT__

#include <iostream>
#include <string.h>
#include <json/json.h>

#include "util.h"
#include "RollLog.h"
#include "sungard/kfts_trade_type.h"
#include "sungard/kfts_trade_struct.h"


#if __SUNGARD__VERSION__ == 16 
	#include "sungard1.6/kfts_trade_type.h"
	#include "sungard1.6/kfts_trade_struct.h"
	#include "sungard1.6/kfts_trade_api.h"
	#warning "include sungard1.6"
#elif __SUNGARD__VERSION__ == 17
	#include "sungard1.7/kfts_trade_type.h"
	#include "sungard1.7/kfts_trade_struct.h"
	#include "sungard1.7/kfts_trade_api.h"
	#warning "include sungard1.7"
#elif __SUNGARD__VERSION__ == 18
	#include "sungard1.8/kfts_trade_type.h"
	#include "sungard1.8/kfts_trade_struct.h"
	#include "sungard1.8/kfts_trade_api.h"
	#warning "include sungard1.8"
#else
	#include "sungard/kfts_trade_type.h"
	#include "sungard/kfts_trade_struct.h"
	#include "sungard/kfts_trade_api.h"
#endif






using namespace std;

//账户信息 struct---->json
Json::Value& get_STAccountInfo(Json::Value& root, STAccountInfo* p);
//持仓信息
Json::Value& get_STPositionInfo(Json::Value& root, STPositionInfo* p);
//成交信息
Json::Value& get_STDoneInfo(Json::Value& root, STDoneInfo* p);
//委托信息
Json::Value& get_STOrderInfo(Json::Value& root, STOrderInfo* p);


//账户信息 json---->struct
STAccountInfo* get_STAccountInfo_Struct(Json::Value& root, STAccountInfo* p);
//持仓信息
STPositionInfo* get_STPositionInfo_Struct(Json::Value& root, STPositionInfo* p);
//成交信息
STDoneInfo* get_STDoneInfo_Struct(Json::Value& root, STDoneInfo* p);
//委托信息
STOrderInfo* get_STOrderInfo_Struct(Json::Value& root, STOrderInfo* p);
#endif




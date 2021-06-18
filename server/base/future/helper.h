/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file helper.h
///@brief 格式转换，数据库操作等的封装函数
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdlib.h>
//#include "ThostFtdcUserApiStruct.h"
#include "../ctp/ThostFtdcTraderSpiI.h"
#include "../sungard/KFTSTraderSpiI.h"
#include "../ice/Common.h"
#include "mongoc_json.h"
#include "util.h"

typedef CThostFtdcInstrumentField InstrumentType;	//合约的结构

//多空
#define  CONST_DIRECTION_L	"long"		//多
#define  CONST_DIRECTION_S	"short"		//空

//买卖
#define CONST_DIRECTION_BUY	 "buy"
#define CONST_DIRECTION_SELL "sell"

//开平仓
#define CONST_OFFSETFLAG_O			"open"				//开仓
#define CONST_OFFSETFLAG_C			"close"				//平仓
#define CONST_OFFSETFLAG_CT 		"closetoday"		//平今天
#define CONST_OFFSETFLAG_CY 		"closeyesterday"	//平昨天天
#define CONST_OFFSETFLAG_FORCE		"force"				//强平
#define CONST_OFFSETFLAG_FORCEOFF	"forceoff"			//强减
#define CONST_OFFSETFLAG_LFC		"localforceclose"	//本地强平`

//投机套保
#define CONST_HEDGEFLAG_TOUJI		"speculation"	//投机
#define CONST_HEDGEFLAG_BAOZHI		"hedge"			//套值
#define CONST_HEDGEFLAG_TAOLI		"arbitrage"		//套利

//市场对应定义
#define CONST_MARKET_DCE	"DCE"		///大连商品交易所
#define CONST_MARKET_CZCE	"CZCE"		///郑州商品交易所
#define CONST_MARKET_SHFE	"SHFE"		///上海商品交易所
#define CONST_MARKET_CFFEX	"CFFEX"		///中金所

//#define CONST_ORDERSTATUS_
#define CONST_ORDERSTATUS_AllTraded				"AllTraded"				//全部成交","ctp":"0","sungard
#define CONST_ORDERSTATUS_PartTradedQueueing	"PartTradedQueueing"	//部分成交还在队列中［部成］",
#define CONST_ORDERSTATUS_PartTradedNotQueueing	"PartTradedNotQueueing"	//部分成交不在队列中［部撤］",
#define CONST_ORDERSTATUS_NoTradeQueueing		"NoTradeQueueing"		//未成交还在队列中［已报待撤］
#define CONST_ORDERSTATUS_NoTradeNotQueueing	"NoTradeNotQueueing"	//未成交不在队列中［未报］","c
#define CONST_ORDERSTATUS_Canceled				"Canceled"				//撤单［场外撤单］","ctp":"5",
#define CONST_ORDERSTATUS_Unknown				"Unknown"				//未知","ctp":"a","sungard":"-
#define CONST_ORDERSTATUS_NotTouched			"NotTouched"			//尚未触发［未报］","ctp":"b",
#define CONST_ORDERSTATUS_Touched				"Touched"				//已触发［已报］","ctp":"c","s
#define CONST_ORDERSTATUS_Invalid				"Invalid"				//废单］","ctp":"-1","sungar

//标识 ctp / sungard
extern string account_type;

//char的 我们这里都要用string。 
//涉及到的转换ctp--->my   st---->my   my--->ctp  my--->st   所有的字段有这4个转变
string ctp_posidirection_to_my(char direction);
string ctp_direction_to_my(char direction);
string ctp_offsetflag_to_my(char offsetflag);
string ctp_hedgeflag_to_my(char  hedgeflag);

string st_posidirection_to_my(char direction);
string st_direction_to_my(char direction);
string st_offsetflag_to_my(char offsetflag);
string st_hedgeflag_to_my(char  hedgeflag);
string st_marketcode_to_my(char  marketcode);

uint8_t	my_posidirection_to_ctp(string direction);//持仓方向
uint8_t	my_direction_to_ctp(string direction);
uint8_t	my_offsetflag_to_ctp(string offsetflag);
uint8_t	my_hedgeflag_to_ctp(string hedgeflag);

uint8_t	my_posidirection_to_st(string direction);
uint8_t	my_direction_to_st(string direction);
uint8_t	my_offsetflag_to_st(string offsetflag);
uint8_t	my_hedgeflag_to_st(string hedgeflag);
uint8_t	my_marketcode_to_st(string marketcode);

string   ctp_orderstatus_to_my(char orderstatus);
uint8_t  my_orderstatus_to_ctp(string orderstatus);

string   st_orderstatus_to_my(char orderstatus);
uint8_t  my_orderstatus_to_st(string orderstatus);

//持仓均价计算，rohon和ctp不一样
double get_todayopenaverage_ctp(CThostFtdcInvestorPositionField *ctp_position);
double get_todayopenaverage_rohon(CThostFtdcInvestorPositionField *ctp_position);

//转换ctp帐号 持仓 成交信息
bool convert_ctp_account(struct ::CM::Account* account, CThostFtdcTradingAccountField *ctp_account);
bool convert_ctp_position(struct ::CM::Position* position, CThostFtdcInvestorPositionField *ctp_position, double todayopenaverage);
bool convert_ctp_done(struct ::CM::Done* done, CThostFtdcTradeField *ctp_done);
bool convert_ctp_order(struct ::CM::Order* order, CThostFtdcOrderField *ctp_order);

//转换金仕达帐号 持仓 成交信息
bool convert_st_account(struct ::CM::Account* account, STAccountInfo *st_account);
bool convert_st_position(struct ::CM::Position* position, STPositionInfo *st_position);
bool convert_st_done(struct ::CM::Done* done, STDoneInfo *st_done);
bool convert_st_order(struct ::CM::Order* order, STOrderInfo *st_order);

//转化撤单结构到订单
bool convert_ctp_cancleorder_order(struct ::CM::Order* order, CThostFtdcOrderActionField* ctp_order);
bool convert_ctp_cancleorder_order(struct ::CM::Order* order, CThostFtdcInputOrderActionField* ctp_order);
//转化下单结构到订单
bool convert_ctp_doorder_order(struct ::CM::Order* order, CThostFtdcInputOrderField* ctp_order);

//转换成交 持仓 账户结构 日k到json数据
Json::Value& get_json_done(::CM::Done* p, Json::Value& root);
Json::Value& get_json_position(::CM::Position* p, Json::Value& root);
Json::Value& get_json_account(::CM::Account* p, Json::Value& root);
Json::Value& get_json_order(::CM::Order* p, Json::Value& root);
Json::Value& get_json_daybar(::CM::DayBar* p, Json::Value& root);
Json::Value& get_json_doorder(::CM::DoOrder* p, Json::Value& root);
Json::Value& get_json_ticker(::CM::Ticker* p, Json::Value& root);
Json::Value& get_json_bar(::CM::Bar* p, Json::Value& root);

//bson的接口
bson_t* get_bson_daybar(::CM::DayBar* p, bson_t* b);

//将json转换为struct
::CM::Order*	get_struct_order(::CM::Order* p, Json::Value& root);
::CM::Done*		get_struct_done(::CM::Done* p, Json::Value& root);
::CM::Account*	get_struct_acount(::CM::Account* p, Json::Value& root);
::CM::Position* get_struct_position(::CM::Position* p, Json::Value& root);
::CM::DayBar*	get_struct_daybar(::CM::DayBar* p, Json::Value& root);
::CM::DoOrder*	get_struct_doorder(::CM::DoOrder* p, Json::Value& root);


//mongo更新成交 持仓 账户文档头部
void  db_update_order_header(::CM::Order* p);
void  db_update_done_header(::CM::Done* p);
void  db_update_account_header(::CM::Account* p);
//void  db_update_position_header(::CM::Position* p);
//void  db_update_position_water_header(string& tradeid, ::CM::Position* p);

//更新账户 持仓 成交数据
void db_update_account(::CM::Account* account);
void db_update_position(string fundid, uint32_t tradingday,  int reqestid, map<string, ::CM::Position>& position);
void db_update_order(map<string, ::CM::Order>& orderm);
void db_update_done(map<string, ::CM::Done>& donem);

void init(::CM::Done* done);
void init(::CM::Account* account);
void init(::CM::Position* position);
void init(::CM::Order* order);

//更新交易日表
int  db_get_tradingday(string fundid);
int	 db_set_tradingday(string fundid, int tradingday);
#endif



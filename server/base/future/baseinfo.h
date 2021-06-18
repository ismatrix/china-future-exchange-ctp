/**********************************************************************
    > File Name: baseinfo.cpp
    > Created Time: Tue 05 Jul 2016 04:32:51 PM CST
    > Author:  
    > description: 基础数据数据块更新模块 独立线程数据库更新  每隔1分钟一更新。 查询数据库时间戳日大于这边数据的更新进来。 
 **********************************************************************/

#ifndef __BASE_INFO__H__
#define __BASE_INFO__H__

#include<iostream>
#include <pthread.h>
#include <map>
#include <set>
#include <string>


#include "util.h"
#include "RollLog.h"
#include "mongoc_json.h"
#include "conf.h"
#include "api_trade_6.3.6/ThostFtdcUserApiDataType.h"
#include "api_trade_6.3.6/ThostFtdcUserApiStruct.h"
#include "../ctp/CtpJsonStruct.h"
using namespace std;


//确认主连  ---- 根据日k的最大持仓量确定
//计算指数  ---- 根据product对应instrument确定
//计算板块  ---- 根据plate对应的product对应的instrument确定

typedef CThostFtdcProductField 			CtpProduct;			
typedef CThostFtdcInstrumentField 		CtpInstrument;
typedef CThostFtdcDepthMarketDataField 	DataSnap;

struct Plate
{
	Plate()
	{	
		groupid = 0;
	}
	int groupid;
	string groupcode;
	string groupname;
	string updatedate;
	set<string> productlist;
};


//这个是基础合约
struct Instrument
{
	Instrument()
	{
		rank			=0;
		openinterest	=0;
		prerank			=0;
		preopeninterest	=0;
		memset(&ctp_instrument, 0, sizeof(ctp_instrument));
	}
	CtpInstrument ctp_instrument;
	uint32_t 	rank;
	uint32_t	openinterest;
	uint32_t	prerank;
	uint32_t    preopeninterest;
	string		updatetime;
};

struct Product
{
	CtpProduct ctp_product;
	string master_instrument;	//对应的主连
};


//每天自动数据库加载一次。 单独起一个线程做。
class BaseInfo
{
	public:	
		BaseInfo();
		//db  初始化
		
		int init(Conf& conf);
		
		int init_thread(Conf& conf);
		
		//数据库更新数据
		int update();
		
		map<string, Product> 	 get_products();
		map<string, Instrument>  get_instruments();
		map<string, Plate>		 get_plates();
		
		Product* 	 get_product(string productid);
		Instrument*  get_instrument(string instrumentid);
		Plate*		 get_plate(string plateid);
		
		void	set_products(Product& product);
		void	set_instruments(Instrument& instrument);
		void	set_plates(Plate& Plate);
		
		//功能函数 获取合约对应的product
		string get_instrument_product(string instrumentid);
		//获取product对应板块
		string get_product_plate(string productid);
		//获取合约对应板块
		string get_instrument_plate(string instrumentid);
	
		//获取板块对应product
		set<string> get_plate_product(string plateid);
		//获取板块对应合约
		set<string> get_plate_instrument(string plateid);
		//获取product对应合约
		set<string> get_product_instrument(string productid);
		
		int get_product_instrument_size(string instrument);

		//获取合约列表
		set<string> get_instrument_set();
	private:
		//产品表
		map<string, Product> products;
		//合约表
		map<string, Instrument> instruments;
		//板块信息
		map<string, Plate> plates;
		
		//product instrument 用于产品对应合约的检查的用
		map<string, set<string> > product_instrument_index;

		
		bool  isupdate;		//默认false true表示数据正在被更新
		
		uint32_t updatetime;//内存数据更新时间
		CMongodb db;	
		pthread_mutex_t _mutex_products;   
		pthread_mutex_t _mutex_instruments;   
		pthread_mutex_t _mutex_plates;   
		pthread_mutex_t _mutex_product_instrument_index;   
};
 
 
#endif

 
 

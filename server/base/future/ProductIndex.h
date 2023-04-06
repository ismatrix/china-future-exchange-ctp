/*************************************************************************
    > File Name: ProductIndex.cpp
    > Created Time: Tue 28 Jun 2016 02:49:11 PM CST
    > Author: 
    > description: 历史数据计算 生成对应的IDX行情文件 
 ************************************************************************/
#ifndef __PRODUCT_INDEX_H__
#define __PRODUCT_INDEX_H__

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <sys/stat.h>
#include <dirent.h>

#include "util.h"
#include "../ice/Common.h"
#include "RollLog.h"
#include "mongoc_json.h"
#include "json/json.h"
#include "helper.h"
#include "file.h"
#include "baseinfo.h"

using namespace std;

// 1. fill 2. calculate 

struct DayBarExtern
{
	::CM::DayBar daybar;
	string instrument;
};

struct BarExtern
{
	::CM::Bar bar;
	string instrument;
};

typedef void (*_callback_daybar)(const string& tradingday, const string& instrument, const ::CM::DayBar& daybar);
typedef void (*_callback_bar)(const string& tradingday, const string& instrument, const ::CM::Bar& bar);



typedef void (*_callback_daybar_master)(const string& tradingday, const string& instrument, const ::CM::DayBar& daybar, const string& master_instrument);

typedef void (*_callback_bar_master)(const string& tradingday, const string& instrument, const string& master_instrument);

typedef void (*_callback_instrument_rank)(const string& instrument, const int prerank, const int rank, double openinterest, double preopeninterest);




//写一般合约日k
void  write_daybar_db(const string& tradingday, const string& instrument, const ::CM::DayBar& daybar);

//写分k
void write_bar_file(const string& tradingday, const string& instrument, const ::CM::Bar& bar);
//写ticker 主连的在用
void write_master_ticker_file(const string& tradingday, const string& instrument, const ::CM::Ticker& ticker);
//写日k主连db
void  write_daybar_db_master(const string& tradingday, const string& instrument, const ::CM::DayBar& daybar, const string& master_instrument);
//写分k主连文件
void  write_bar_file_master(const string& tradingday, const string& instrument,  const string& master_instrument);
//写合约排名信息
void update_instrument_rank(const string& instrument, const int prerank, const int rank, double openinterest, double preopeninterest);

//数据来源 db file 实时
//生成数据 实时写文件  写db   实时push下游不定
class CProductIndex
{
	public:
	CProductIndex()
	{
		callback_daybar_master	= NULL;
		callback_bar_master		= NULL;
		callback_instrument_rank= NULL; 
        if(query_instrument()<0){
            LOG_ERROR("query_instrument error.")
        }
	}
	virtual ~CProductIndex(){}

	/////////////////////////////历史静态数据计算指数/////////////////////////////////////
	//日k计算 抛给这个对象一批数据  他要能计算这批数据 
	//productid    tradingday   daybar
	map<string, map<string, vector<DayBarExtern> > >  product_daybar;

	//分k计算  抛数据  然后计算
	//productid    tradingday   key        bar
	map<string, map<string, map<long, vector<BarExtern> > > >  product_bar;

	//文件目录的获取
	//productid    tradingday   path
	map<string, map<string, vector<string> > >  bar1_dir;

	//合约信息表
	//productid instrument   
	map<string, map<string, Instrument> > instrumenttable;

    map<string, string> instrument_products;

	void set__callbackd(_callback_daybar _call);
	void set__callback(_callback_bar _call);
	

	void set__callback_masterd( _callback_daybar_master     callback_daybar_master); 
	void set__callback_master( _callback_bar_master     callback_bar_master); 

	void set__callback_instrument_rank(_callback_instrument_rank callback_instrument_rank);

	virtual bool check_instrument_expires_tomorrow(string& tradingday, Instrument& ctp);

	string get_productid(string instrument);		//历史的只能平规则推算

	void set_product_daybar(const string& productid, const string& tradingday, const string& instrument,  const ::CM::DayBar& daybar);
	void set_product_bar(const string& productid, const string& tradingday, const string& instrument, const ::CM::Bar&  bar);
	
	//暂时没用
	int fill_data_product_daybar(vector<string>& productids);		//填充product
	int fill_data_product_bar(vector<string>& productids);		//填充product

	////////////////////////////历史数据填充////////////////////////////////////////////
	int query_daybar(string tradingdday="", string productid="");
	int query_instrument();
	
	int fill_daybar_tradingday(string tradingday);	//跑某一天的数据
	int fill_daybar_product(string productid);		//跑某一只的所有数据
	int fill_daybar();								//跑所有的数据

	//遍历所有目录  读取数据  只能按天 或者按产品结算
	int check_dir(string prefix,  string tradingday="", string productid=""); //遍历目录找到合适的
	int fill_bar_dir(string& path, string& tradingday, string& productid);	 //填充目录结构
	int fill_product_bar(string prefix);	//根据目录结构读取kline填充
	
	int fill_bar_key(string tradingday, string prefix="./hqdata");		//跑所有产品某一天的数据
	int fill_bar_product(string productid, string prefix="./hqdata");		//跑某只产品所有的分k
	int fill_bar(string prefix="./hqdata");								//跑所有的数据

	void reset_data();
	void reset_all();

	////////////////////////////历史数据填充////////////////////////////////////////////


	//计算指数  计算完毕的数据写到公共结构或者文件等。
	int calculate_daybar();			//计算日k指数数据并入库
	int calculate_bar();			//计算分k指数数据并写文件


	//主连信息分三步 以回调的方式
	//1. 更新日k历史主连信息
	//2. 更新分k 历史主连数据
	//3. 更新当前合约表中的主连 及合约排名信息
	//计算主连合约信息 并入库主连信息日k
	int calculate_master_instrument();			//计算日k指数数据并入库

	//实时数据计算部分 必须所有的合约分k都生成才能生成指数 这个可以放在mdlive生成。
	//1. 来一个新的tickr 检查对应指数有没有生成， 没有的话
	//2. 检查指数没有生成，检查合约的上一分钟是不是都已经ok了。 ok了则生成指数线。
	//3. 这里有个指数和合约的对饮映射
	//4. 日k生成同样的逻辑
	//主连一样

	private:

	void set_instrumenttable(Instrument* p);
	void set_openinterest(string& productid, string& instrument, double openinterest, double preopeninterest, int volume);

	int calculate_instrument(string&  tradingday);

	list<_callback_daybar> _callbackd;			//日k的回调
	list<_callback_bar>	 _callback;			//日k的回调
	
	_callback_daybar_master		callback_daybar_master;
	_callback_bar_master		callback_bar_master;
	_callback_instrument_rank	callback_instrument_rank;
	
	
	
};


#endif



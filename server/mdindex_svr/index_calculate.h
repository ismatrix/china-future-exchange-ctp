/*********************************************************************
    > File Name: index_calculate.h
    > Created Time: Tue 05 Jul 2016 04:32:51 PM CST
    > Author: 
    > description: 标准化的依赖Product Instrument Plate计算指数 主连 板块等信息 作为mdlive的上游， 包含mapast的功能
	
 **********************************************************************/
#ifndef __INDEX_CALCULATE__H__
#define __INDEX_CALCULATE__H__

//这里需要填充随便   计算的是按品种计算 
//基础的计算  最终会计算的到 bar daybar给mdlive  mdlive获取这些bar daybar直接push下游。 mdlive自身的存储其实是没有用到的


#include "future/ProductIndex.h"
#include "future/baseinfo.h"
#include "future/holidays.h"
// set_product_daybar(string& productid, string& tradingday, string& instrument,  ::CM::DayBar& daybar);
// set_product_bar(string& productid, string& tradingday, string& instrument, ::CM::Bar&  bar);

//mdlive那边会保证每分钟过3秒推送完毕上一分k
//指数计算延迟一分钟计算  或者 10s中没有数据更新

class BaseCalculate : public CProductIndex
{
	public:
		BaseCalculate()
		{
			hqtime	= 0;
			timer1 = 1501; //计算时间
			timer2 = 1516; //计算时间
		}
		int  init(Conf& conf);
		
		//检查所有数据
		int Run();
		
		//计算分k主连
		int calculate_bar_master_instrument(const string& productid,  const string& tradingday, const string& instrument, const CM::Bar& bar);
		
		//计算所有分k的指数
		int calculate_bar_index(const string& tradingday, const long key);
		
		//计算分k板块
		int calculate_bar_plate();
		
		//计算日k主连
		int calculate_daybar_master_instrument(const string& productid, const string& tradingday, const string& instrument, const CM::DayBar& daybar);
		
		//计算所有日k指数
		int calculate_daybar_index(const string& tradingday);
		
		//计算日k板块
		int calculate_daybar_plate();
		
		bool is_master_instrument(const string& instrument, string*  master_instrument=NULL);
		
		//这个是ice客户算线程调用
		void set_key_bar(const string& tradingday, const string& instrument, const CM::Bar& bar);
		void set_key_daybar(const string& productid, const string& instrument, const CM::DayBar& daybar );
		void set_key_daybar_index(string& productid, string& instrument, CM::DayBar& daybar );

		//onTick更新 行情时间
		void set_hqtime(const string& _tradingday, const string& instrument,  const ::CM::Ticker& ticker);

		//用来更新日k
		void run();

		//获取合约列表用于订阅
		set<string> get_instrument_set();

		//获取主力合约
		list<string> get_master_list();

		//检查合约是不是明天要过期
		virtual bool check_instrument_expires_tomorrow(string& tradingday, string& instrument);
	private:
	
		BaseInfo base;

		// key     productid  instrument   bar
		map<long, map<string, map<string, CM::Bar> > > key_bar;
		
		//存储收到的日k
 		// productid    instrument   daybar
		map<string, map<string, CM::DayBar> > key_daybar;

		int64_t hqtime;
		string tradingday;	//处理的交易日

		//日k定时计算时间
		uint32_t timer1;
		uint32_t timer2;
};



#endif






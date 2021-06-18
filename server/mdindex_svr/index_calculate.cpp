/**********************************************************************
    > File Name: index_calculate.cpp
    > Created Time: Tue 05 Jul 2016 04:32:51 PM CST
    > Author: 
    > description: 标准化的依赖Product Instrument Plate计算指数 主连 板块等信息 作为mdlive的上游， 包含mapast的功能
	
 **********************************************************************/
 

#include "mdlive_client.h"
#include "index_calculate.h"

//主连直接push回去  指数计算之后通过回调push
void push_ticker_run(const string& tradingday, const string& instrument, const ::CM::Ticker& ticker)
{
	//生成的指数bar 回调函数
	try{
		g_push__session->begin_sendTicker(tradingday, instrument, ticker);
	}catch(Ice::Exception& e){
		stringstream ss;
		ss << e ;
		LOG_ERROR(ss.str());
	}
}

void push_bar_run(const string& tradingday, const string& instrument, const ::CM::Bar& bar)
{
	//生成的指数bar 回调函数
	try{
		g_push__session->begin_sendBar(tradingday, instrument, bar);
	}catch(Ice::Exception& e){
		stringstream ss;
		ss << e ;
		LOG_ERROR(ss.str());
	}
}


void  push_daybar_run(const string& tradingday, const string& instrument, const ::CM::DayBar& daybar)
{
	//生成的日k回调函数
	try{
		g_push__session->begin_sendDayBar(tradingday, instrument, daybar);
	}catch(Ice::Exception& e){
		stringstream ss;
		ss << e ;
		LOG_ERROR(ss.str());
	}
}

//检查计算数据
void*  thread_run(void* arg)
{
	BaseCalculate* p = (BaseCalculate*)arg;
	
	while(true)
	{
		p->run();

		sleep(10);
	}
	return NULL;
}


int  BaseCalculate::init(Conf& conf)
{
	base.init_thread(conf);

	set__callback(write_bar_file);		//写分k
	set__callbackd(write_daybar_db);	//写日k
	
	set__callback(push_bar_run);		//push分k
	set__callbackd(push_daybar_run);	//push日k

	//合约表中 排名更新
	set__callback_instrument_rank(update_instrument_rank);

	pthread_t tid;
	pthread_attr_t attr;
	
	if(pthread_attr_init(&attr) != 0)
	{
		LOG_ERROR("pthread_attr_init fail.");
		exit(-1);
	}
	
	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
	{
		LOG_ERROR("pthread_attr_setdetachstate fail.");
		exit(-1);
	}
	
	if(pthread_create(&tid, &attr, thread_run, this) != 0)
	{
		LOG_ERROR("pthread_create fail.");
		exit(-1);
	}
	return 0;
}


void BaseCalculate::set_key_bar(const string& tradingday, const string& instrument, const CM::Bar&  bar)
{
	string productid = base.get_instrument_product(instrument);
	typeof(key_bar.begin()) itr = key_bar.find(bar.Timestamp);
	if(itr != key_bar.end())
	{
		map<string, map<string, CM::Bar> >& pid = itr->second;
		typeof(pid.begin()) it = pid.find(productid);
		if(it != pid.end())
		{
			it->second.insert(make_pair(instrument, bar));
		}
		else
		{
			map<string, CM::Bar>  ins;
			ins[instrument] = bar;
			pid[productid] = ins;
		}
	}
	else
	{
		map<string, CM::Bar>  ins;
		ins[instrument] = bar;
		map<string, map<string, CM::Bar> > pid;
		pid[productid] = ins;
		key_bar[bar.Timestamp] = pid;
	}
	
	calculate_bar_master_instrument(productid, tradingday, instrument, bar);
	
	//calculate_bar_index(tradingday, bar.Timestamp);
	
	LOG_DEBUG("productid=" << productid << "  bar size=" << key_bar.find(bar.Timestamp)->second.find(productid)->second.size() << " product size=" << base.get_product_instrument_size(productid));
}

void BaseCalculate::set_key_daybar(const string& tradingday, const string& instrument, const CM::DayBar& daybar)
{

	//map<string, map<string, CM::DayBar> > key_daybar;
	string productid = base.get_instrument_product(instrument);
	typeof(key_daybar.begin()) itr = key_daybar.find(productid);
	if(itr != key_daybar.end())
	{
		itr->second.insert(make_pair(instrument, daybar));
	}
	else
	{
		map<string, CM::DayBar> sd;
		sd[instrument] = daybar;
		key_daybar[productid] = sd;
	}
	
	calculate_daybar_master_instrument(productid, tradingday, instrument, daybar);
	
	//calculate_daybar_index(productid, tradingday, instrument, daybar);
	
	LOG_DEBUG("productid=" << productid << "  bar size=" << key_daybar.find(productid)->second.size() << " product size=" << base.get_product_instrument_size(productid));
}



//计算所有kline的指数
int BaseCalculate::calculate_bar_index(const string& tradingday,  const long key)
{
	LOG_DEBUG("run bar calculate_bar_index key=" << key);
	
	reset_data();   //清空   
	typeof(key_bar.begin()) itr = key_bar.find(key);
	if(itr != key_bar.end())
	{
		map<string, map<string, CM::Bar> >& pid = itr->second;
		map<string, map<string, CM::Bar> >::iterator it = pid.begin();
		for(; it!=pid.end(); it++)
		{
			map<string, CM::Bar>& sb = it->second;
		
			LOG_DEBUG("index bar productid=" << it->first  << " tradingday="<<tradingday << " size="<< sb.size() << " key=" << key);
			
			typeof(sb.begin()) itrsb = sb.begin();
			for(; itrsb!=sb.end(); itrsb++)
			{
				set_product_bar(it->first, tradingday, itrsb->first, itrsb->second);	//填充数据
			}
		}
	}
	calculate_bar();    //计算     
	return 0;
}


//计算更新主连
int BaseCalculate::calculate_bar_master_instrument(const string& productid,  const string& tradingday, const string& instrument, const CM::Bar& bar)
{
	if(is_master_instrument(instrument))
	{
		string minstrument = productid + "0001";
	
		LOG_DEBUG("master bar productid=" << productid << " instrument="<<instrument << " tradingday="<<tradingday << " bar=" << bar.Timestamp);
		
		//处理主连
		write_bar_file(tradingday, minstrument, bar);
		
		//直接push下游
		push_bar_run(tradingday,  minstrument,  bar);
	}
	return 0;
}


//计算分k板块
int BaseCalculate::calculate_bar_plate()
{

	return 0;
}


//检查是不是主连合约
bool BaseCalculate::is_master_instrument(const string& instrument, string* master_instrument)
{
	string productid = base.get_instrument_product(instrument);
	Product* p = base.get_product(productid);
	if(p->master_instrument  == instrument)
	{
		if(master_instrument != NULL)  *master_instrument = productid + "0001";
		return true;
	}
	return false;
}

//计算日k主连
int BaseCalculate::calculate_daybar_master_instrument(const string& productid, const string& tradingday, const string& instrument, const CM::DayBar& daybar)
{
	if(is_master_instrument(instrument))
	{
		string minstrument = productid + "0001";
	
		LOG_DEBUG("master daybar productid=" << productid << " instrument="<<instrument << " tradingday="<<tradingday);

		//数据库操作
		write_daybar_db_master(tradingday,  minstrument,  daybar, instrument);
		
		//直接push下游
		push_daybar_run(tradingday,  minstrument,  daybar);
	}
	return 0;
}
		


int BaseCalculate::calculate_daybar_index(const string& tradingday)
{
	reset_data();	//清空
	map<string, map<string, CM::DayBar> >::iterator itrp = key_daybar.begin();
	for(; itrp!=key_daybar.end(); itrp++)
	{
		map<string, CM::DayBar>& sb =itrp->second;

		LOG_DEBUG("index daybar productid=" << itrp->first  << " tradingday="<<tradingday << " size="<< sb.size());
		
		typeof(sb.begin()) itr = sb.begin();
		for(; itr!=sb.end(); itr++)
		{
			set_product_daybar(itrp->first, tradingday, itr->first, itr->second);	//填充数据
		}
	}
	calculate_daybar();	//计算指数 

		
	return 0;
}

set<string> BaseCalculate::get_instrument_set()
{
	return base.get_instrument_set();
}

list<string> BaseCalculate::get_master_list()
{
	list<string> master;
	
	map<string, Instrument> sp = base.get_instruments();
	map<string, Instrument>::iterator itr = sp.begin();
	for(; itr!=sp.end(); itr++)
	{
		if(itr->second.rank == 1)
			master.push_back(itr->second.ctp_instrument.InstrumentID);
	}
	return master;
}


//计算日k板块
int BaseCalculate::calculate_daybar_plate()
{
	

	return 0;
}
	

//onTick触发 执行指数计算
void  BaseCalculate::set_hqtime(const string& _tradingday, const string& instrument,  const ::CM::Ticker& ticker)
{
	//ticker来了判断主连然后直接推送回去
	string master_instrument;
	if(is_master_instrument(instrument, &master_instrument))
	{
		write_master_ticker_file(tradingday, master_instrument, ticker);
		push_ticker_run(tradingday, master_instrument, ticker);	
	}

	//计算指数kline
	this->tradingday = _tradingday;
	if(ticker.Timestamp > hqtime*1000) hqtime = ticker.Timestamp/1000;

	static bool flag = false;

	struct tm tmp = {0};         
	localtime_r(&hqtime, &tmp);
	if(tmp.tm_sec >= 5)
	{
		if(flag)
		{
			flag = false;
			//计算上一分钟的
			calculate_bar_index(tradingday, (hqtime / 60 * 60 - 60)*1000);	
		}
	}
	else
	{
		flag = true;
	}

	//LOG_DEBUG("tradingday=" << tradingday << " hqtime=" << hqtime);
}


//线程里面定时计算 15.01  和 15.30
void BaseCalculate::run()
{
	//如果周末 或者节假日不开盘 直接返回。 定时任务在15点多 tradingday==actionday
	if(!g_holidays.IsTrading(time(NULL)))
		return;
	
	//计算时间点
	time_t t = time(NULL);
	uint32_t tt = atoll(stamptostr(t, "%H%M").c_str());

	//LOG_DEBUG("timer1=" << timer1 << " timer2=" << timer2 << " tt=" << tt);

	static bool flag1 = false;
	if(tt > timer1)
	{
		if(flag1)
		{
			LOG_INFO("1501 run tradingday=" << tradingday << " size=" << key_daybar.size());
			flag1 = false;
			
			calculate_daybar_index(tradingday);
			key_daybar.clear();
		}
	}
	else
	{
		flag1 = true;
	}

	static bool flag2 = false;
	if(tt > timer2)
	{
		
		if(flag2)
		{
			//刷15.14分的分k
			uint32_t rt = hqtime/60 * 60;
			uint32_t lt = get_ymd(hqtime) + 15*3600 + 14*60;
			
			LOG_INFO("1516 run rt=" << rt << " lt=" << lt << " tradingday=" << tradingday << " size="<< key_daybar.size() << " key_bar.size=" << key_bar.size());
			flag2 = false;
			
			while(rt <= lt)
			{
				calculate_bar_index(tradingday, rt);
				rt += 60;
			}

			calculate_daybar_index(tradingday);
			
			//数据库查寻日k 查询合约表 在计算
			CProductIndex tmp;
			tmp.fill_daybar_tradingday(tradingday);
			tmp.set__callback_instrument_rank(update_instrument_rank);
			tmp.calculate_master_instrument();//计算主连

			key_bar.clear();
			key_daybar.clear();
		}
	}
	else
	{
		flag2 = true;
	}
	
	LOG_DEBUG("key_bar.size=" << key_bar.size() << " key_daybar.size=" << key_daybar.size() << " hqtime=" << hqtime << " tradingday=" << tradingday);
}


bool BaseCalculate::check_instrument_expires_tomorrow(string& tradingday, string& instrument)
{
	Instrument*  ctp = base.get_instrument(instrument);	
	if(ctp == NULL)
	{
		LOG_ERROR("instrument=" << instrument << " not found.");
		return false;
	}

	if(atoll(tradingday.c_str()) >= atoll(ctp->ctp_instrument.ExpireDate))
	{
		return true; //表示要过期	
	}
	
	return false; //不过期
}










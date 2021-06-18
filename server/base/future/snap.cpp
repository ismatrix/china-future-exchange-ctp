/*************************************************************************
  > File Name: snap.cpp
  > Created Time: Tue 26 Jul 2016 01:35:59 PM CST
  > Author: 
  > description: 
 ************************************************************************/

#include "snap.h"


map<string, DayBarEx > CDayBar::_map_snap_tmp;

//回调函数
void  update_daybar_index(const string& tradingday, const string& instrument, const ::CM::DayBar& daybar)
{
	DayBarEx d;
	d.t				= time(NULL);
	d.instrumentid  = instrument;
	d.tradingday	= tradingday;
	d.daybar		=  daybar;
	
	CDayBar::_map_snap_tmp[instrument] = d;
}

void* snap_thread_run(void* p)
{
	CDayBar * st = (CDayBar*)p;
	while(true)
	{
		st->run();
	}
	return NULL;
}


void CDayBar::init(Conf& conf)
{
	pthread_mutex_init(&_mutex,  NULL);

	//指数快照计算后更新回写
	set__callbackd(update_daybar_index);    //回调更新数据到tmp中
	
	//初始化数据库
	string uri, dbname, collection;
	conf.get<string>("MONGO", "uri", uri, "mongodb://127.0.0.1");
	conf.get<string>("MONGO", "db", dbname, "smartwin");
	//conf.get<string>("MONGO", "collection", collection, "DAYBAR");
	collection = "DAYBAR";
	db.Init(uri.c_str(), dbname.c_str(), collection.c_str());

	conf.get<uint32_t>("DAY", "interval", interval, "3");

	LOG_INFO("uri=" << uri << " db=" << dbname << " collection=" << collection << " interval=" << interval);

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

	if(pthread_create(&tid, &attr, snap_thread_run, this) != 0)
	{
		LOG_ERROR("pthread_create fail.");
		exit(-1);
	}
}


void CDayBar::run()
{
	//全刷一次
	pthread_mutex_lock(&_mutex);
	_map_snap_tmp = _map_snap;
	pthread_mutex_unlock(&_mutex);


	reset_data();
	//计算指数快照
	map<string, DayBarEx >::iterator itr = _map_snap_tmp.begin();
	for(; itr!= _map_snap_tmp.end(); itr++)
	{
		set_product_daybar(itr->second.productid, itr->second.tradingday, itr->second.instrumentid,  itr->second.daybar);  
		LOG_DEBUG("set_product_daybar " << itr->second.productid << ", " << itr->second.tradingday << ", " << itr->second.instrumentid);
	}	
	calculate_daybar();

	itr = _map_snap_tmp.begin();
	for(uint32_t i=1; itr!= _map_snap_tmp.end(); itr++,i++)
	{
		//bupdate
		bson_t* q = bson_new();
		bson_t* d = bson_new();
		bson_t* b = bson_new();
		BSON_APPEND_UTF8(q, "instrument",  itr->second.instrumentid.c_str());
		BSON_APPEND_UTF8(q, "tradingday",  itr->second.tradingday.c_str());

		BSON_APPEND_UTF8(d, "instrument",  itr->second.instrumentid.c_str());
		BSON_APPEND_UTF8(d, "tradingday",  itr->second.tradingday.c_str());
		get_bson_daybar(&itr->second.daybar, d);
	
		BSON_APPEND_DOCUMENT(b, "$set", d);
	
		//db.bupdate_one(q, b, true);
		db.bupdate(q, b, true);
		bson_destroy(q);
		bson_destroy(d);
		bson_destroy(b);

		if(i%50 == 0 || i==_map_snap_tmp.size())
		{
			//bexce
			 string replay;
			int ret = db.bexecute(replay);
			if(ret < 0)
			{
				LOG_ERROR("run item=" << i << " size=" << _map_snap_tmp.size() << " error=" << db.GetLastErr() << " replay=" << replay << " ret=" << ret);
			}
			else
			{
				LOG_DEBUG("run item=" << i << " size=" << _map_snap_tmp.size() << " error=" << db.GetLastErr() << " replay=" << replay << " ret=" << ret);
			}
			db.binit();
		}
	}

	sleep(interval);
}


//query and update
void CDayBar::flush(DayBarEx& daybarextern)
{
	Json::Value s, q;
	get_json_daybar((::CM::DayBar*)&daybarextern.daybar, s);
	s["instrument"] = daybarextern.instrumentid;
	s["tradingday"] = daybarextern.tradingday;

	q["instrument"] = daybarextern.instrumentid;
	q["tradingday"] = daybarextern.tradingday;

	int ret = db.Update(s.toStyledString(), q.toStyledString(), true);
	if(ret < 0)
	{
		LOG_ERROR("db fail DayBar:" << db.GetLastErr() << " sql=" << s.toStyledString());
	}
	static uint32_t count = 0;
	if(count++ % 1000 == 0)
		LOG_INFO("daybar flush db times=" << count);
}

void CDayBar::set(string instrumentid, string tradingday, string productid, ::CM::DayBar& daybar)
{
	DayBarEx d;
	d.t				= time(NULL);
	d.instrumentid  = instrumentid;
	d.tradingday	= tradingday;
	d.daybar		=  daybar;
	d.productid     =  productid;

	/*
	if(interval == 0) return;
	{
		flush(d);
		return;
	}
*/
	pthread_mutex_lock(&_mutex);
	_map_snap[instrumentid] = d;
	pthread_mutex_unlock(&_mutex);

	/*
	   if(pthread_mutex_trylock(&_mutex) == 0)
	   {
	   _map_snap[key] = d;
	   pthread_mutex_unlock(&_mutex);
	   }*/
}


void CDayBar::clear()
{
	pthread_mutex_lock(&_mutex);
	_map_snap.clear();
	_map_snap_tmp.clear();
	reset_data();
	pthread_mutex_unlock(&_mutex);
}



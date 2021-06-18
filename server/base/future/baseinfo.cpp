/**********************************************************************
    > File Name: baseinfo.cpp
    > Created Time: Tue 05 Jul 2016 04:32:51 PM CST
    > Author: 
    > description: 基础数据数据块更新模块 独立线程数据库更新
 **********************************************************************/
#include "baseinfo.h"


void* update_base_data(void* arg)
{
	BaseInfo* p = (BaseInfo*)arg;
	while(true)
	{
		p->update();
		sleep(60*5);		//5分钟查询一次
	}
	return NULL;
}

BaseInfo::BaseInfo()
{
	isupdate = false;
	updatetime = 0;

	pthread_mutex_init(&_mutex_products, NULL);
	pthread_mutex_init(&_mutex_instruments, NULL);
	pthread_mutex_init(&_mutex_plates, NULL);
	pthread_mutex_init(&_mutex_product_instrument_index, NULL);
}

int BaseInfo::init(Conf& conf)
{
	//数据库初始化。
	string uri, dbname, collection;
	conf.get<string>("MONGO", "uri", uri, "mongodb://127.0.0.1");
	conf.get<string>("MONGO", "db", dbname, "smartwin");
	conf.get<string>("MONGO", "collection", collection, "DAYBAR");
	db.Init(uri.c_str(), dbname.c_str(), collection.c_str());
	
	int ret = update();
	if(ret < 0) exit(-1);
	
	return 0;
}

int BaseInfo::init_thread(Conf& conf)
{
	 init(conf);
	
	//起独立线程进行数据库更新
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
	
	if(pthread_create(&tid, &attr, update_base_data, this) != 0)
	{
		LOG_ERROR("pthread_create fail.");
		exit(-1);
	}
	return 0;
}
		
int BaseInfo::update()	
{
	isupdate = false;
	
	//查询时间大于原始日期的
	Json::Value q,gt;
	gt["$gt"] = stamp_to_isodate(updatetime); //stamptostr(updatetime);
	q["updatedate"]  = gt;
	
	 //更新plate
	 db.ChangeCollection("PRODUCTGROUP");
	 if(db.Query(q.toStyledString()) < 0)
	 {
		 LOG_ERROR("mongo query fail. err:" << db.GetLastErr());
		 return -1;;
	 }
	 while(db.Next())
	 {
		 try
		 {
			 char* result = db.GetResult();
			 Json::Reader reader;
			 Json::Value val;
			 if(!reader.parse(result, val))
			 {

				LOG_ERROR("reader.parse error. result:" << result);
				return -1;
			 }
			Plate p;
			if(!val["groupid"].isNull()) 		p.groupid 		= 	val["groupid"].asInt();
			if(!val["groupcode"].isNull()) 		p.groupcode 	= 	val["groupcode"].asString();
			if(!val["groupname"].isNull()) 		p.groupname 	= 	val["groupname"].asString();
			if(!val["updatedate"].isNull()) 	p.updatedate 	= 	stamptostr(isodate_to_stamp(val["updatedate"]));
			
			//遍历数组
			Json::Value productlist = val["productlist"];
			int size = productlist.size();  
			for(int i=0; i<size; i++)
			{  
				p.productlist.insert(productlist[i].asString());
			}  
			set_plates(p);
			LOG_DEBUG("insert plateid=" << p.groupid <<  " platecode=" << p.groupcode << " platename=" << p.groupname);
		 }
		 catch(Json::Exception& e)
		 {
			 LOG_ERROR(e.what());
			 return -1;
		 }
		 catch(...)
		 {
			 LOG_ERROR("db.Next() fail.");
			 return -1;
		 }
	 }
	 LOG_INFO("PRODUCTGROUP query plates size=" << plates.size() << q.toStyledString());
	

	db.ChangeCollection("PRODUCT");
	if(db.Query(q.toStyledString()) < 0)
	{
		LOG_ERROR("mongo query fail. err:" << db.GetLastErr());
		return -1;;
	}
	while(db.Next())
	{
		try
		{
			char* result = db.GetResult();
			Json::Reader reader;
			Json::Value val;
			if(!reader.parse(result, val))
			{
					LOG_ERROR("reader.parse error. result:" << result);
					return -1;
			}
			//更新内存的product 
			Product p;
			get_CThostFtdcProductField_Struct(val, &p.ctp_product); 
			set_products(p);
			LOG_DEBUG("insert productid=" << p.ctp_product.ProductID << " productname=" << p.ctp_product.ProductName);
		}
		catch(Json::Exception& e)
		{
			LOG_ERROR(e.what());
			return -1;
		}
		catch(...)
		{
			LOG_ERROR("db.Next() fail.");
			return -1;
		}
	}
	LOG_INFO("PRODUCT query products size=" << products.size() << q.toStyledString());
	
	db.ChangeCollection("INSTRUMENT");
    q["productclass"] =  "1";        //只处理为1的
	q["istrading"] =  1;  
	if(db.Query(q.toStyledString()) < 0)
	{
	 LOG_ERROR("mongo query fail. err:" << db.GetLastErr());
	 return -1;;
	}
	while(db.Next())
	{
		try
		{
			char* result = db.GetResult();
			Json::Reader reader;
			Json::Value val;
			if(!reader.parse(result, val))
			{
	
				LOG_ERROR("reader.parse error. result:" << result);
				return -1;
			}
			Instrument p;
			get_CThostFtdcInstrumentField_Struct(val, &p.ctp_instrument);
			if(!val["rank"].isNull()) 				p.rank 				= 	val["rank"].asInt();
			if(!val["openinterest"].isNull()) 		p.openinterest 		= 	val["openinterest"].asInt();
			if(!val["prerank"].isNull()) 			p.prerank 			= 	val["prerank"].asInt();
			if(!val["preopeninterest"].isNull()) 	p.preopeninterest 	= 	val["preopeninterest"].asInt();
			if(!val["updatetime"].isNull()) 		p.updatetime 	= 	val["updatetime"].asInt();
			set_instruments(p);
			
			//更新主连
			if(p.rank == 1)
			{
				string productid = get_instrument_product(p.ctp_instrument.InstrumentID);
				Product* pp = get_product(productid);
				if(pp != NULL)
					pp->master_instrument = p.ctp_instrument.InstrumentID;
			}
			
			//更新反向索引
			pthread_mutex_lock(&_mutex_product_instrument_index);
			typeof(product_instrument_index.begin()) itr = product_instrument_index.find(p.ctp_instrument.ProductID);
			if(itr != product_instrument_index.end())
			{
				itr->second.insert(p.ctp_instrument.InstrumentID);
			}
			else
			{
				set<string> pt;
				pt.insert(p.ctp_instrument.InstrumentID);
				product_instrument_index[p.ctp_instrument.ProductID] = pt;
			}
			pthread_mutex_unlock(&_mutex_product_instrument_index);
			
			LOG_DEBUG("insert instrument=" << p.ctp_instrument.InstrumentID << " instrumentname=" << p.ctp_instrument.InstrumentName);
		}
		catch(Json::Exception& e)
		{
			LOG_ERROR(e.what());
			return -1;
		}
		catch(...)
		{
			LOG_ERROR("db.Next() fail.");
			return -1;
		}
	}
	
	LOG_INFO("PRODUCT query products size=" << products.size());
	LOG_INFO("PRODUCTGROUP query plates size=" << plates.size());
	LOG_INFO("INSTRUMENT query instruments size=" << instruments.size());
	LOG_INFO("index size product_instrument_index=" << product_instrument_index.size());
	 
	isupdate = true;
	updatetime = get_ymd(time(NULL) + 86400);
	 
	return 0;
}

map<string, Product> 	 BaseInfo::get_products()
{
	map<string, Product> tmp;
	pthread_mutex_lock(&_mutex_products);       
	tmp = products;
	pthread_mutex_unlock(&_mutex_products);       
	return tmp;
}
map<string, Instrument> BaseInfo::get_instruments()
{
	map<string, Instrument> tmp;
	pthread_mutex_lock(&_mutex_instruments);
	tmp = instruments;
	pthread_mutex_unlock(&_mutex_instruments);
	return tmp;
}
map<string, Plate>		 BaseInfo::get_plates()
{
	map<string, Plate> tmp;
	pthread_mutex_lock(&_mutex_plates);            
	tmp = plates;
	pthread_mutex_unlock(&_mutex_plates);            
	return tmp;
}

Product* 	 BaseInfo::get_product(string productid)
{
	Product* p = NULL;
	
	pthread_mutex_lock(&_mutex_products);
	if(products.find(productid) != products.end())
		p =  &products[productid];
	pthread_mutex_unlock(&_mutex_products);
	
	return p;
}
Instrument*  BaseInfo::get_instrument(string instrumentid)
{
	Instrument* p = NULL;
	
	pthread_mutex_lock(&_mutex_instruments);
	if(instruments.find(instrumentid) != instruments.end())
		p =  &instruments[instrumentid];
	pthread_mutex_unlock(&_mutex_instruments);
	
	return p;
}
Plate*		 BaseInfo::get_plate(string plateid)
{
	Plate* p = NULL;
	
	pthread_mutex_lock(&_mutex_plates);
	if(plates.find(plateid) != plates.end())
		p = &plates[plateid];
	pthread_mutex_unlock(&_mutex_plates);
	
	return p;
}

void	BaseInfo::set_products(Product& product)
{
	pthread_mutex_lock(&_mutex_products);
	products[product.ctp_product.ProductID] = product;
	pthread_mutex_unlock(&_mutex_products);
}
void	BaseInfo::set_instruments(Instrument& instrument)
{
	pthread_mutex_lock(&_mutex_instruments);
	instruments[instrument.ctp_instrument.InstrumentID] = instrument;
	pthread_mutex_unlock(&_mutex_instruments);
}
void	BaseInfo::set_plates(Plate& Plate)
{
	pthread_mutex_lock(&_mutex_plates);
	plates[Plate.groupcode] = Plate;
	pthread_mutex_unlock(&_mutex_plates);
}


//功能函数 获取合约对应的product
string BaseInfo::get_instrument_product(string instrumentid)
{
	Instrument* ins = get_instrument(instrumentid);
	if(ins != NULL)
	{
		return ins->ctp_instrument.ProductID;
	}
	return "";
}
//获取product对应板块
string BaseInfo::get_product_plate(string productid)
{
	string plate="";
	pthread_mutex_lock(&_mutex_plates);
	typeof(plates.begin()) itr = plates.begin();
	for(; itr!=plates.end(); itr++)
	{
		if(itr->second.productlist.find(productid) != itr->second.productlist.end())
		{
			plate = itr->first;
			break;
		}
	}
	pthread_mutex_unlock(&_mutex_plates);
	return plate;
}
//获取合约对应板块
string BaseInfo::get_instrument_plate(string instrumentid)
{
	string productid = get_instrument_product(instrumentid);
	if(productid != "")
		return get_product_plate(productid);
	return "";
}

//获取板块对应product
set<string> BaseInfo::get_plate_product(string plateid)
{
	set<string> tmp;
	
	pthread_mutex_lock(&_mutex_plates);
	typeof(plates.begin()) itr = plates.begin();
	if(itr != plates.end())
		tmp =  itr->second.productlist;
	pthread_mutex_unlock(&_mutex_plates);
	
	return tmp;
}

//获取板块对应合约-----
set<string> BaseInfo::get_plate_instrument(string plateid)
{
	set<string> setins;
	set<string> products = get_plate_product(plateid);
	
	pthread_mutex_lock(&_mutex_products);
	set<string>::iterator itr = products.begin();
	for(; itr!=products.end(); itr++)
	{
		set<string> tmp = get_product_instrument(*itr);
		setins.insert(tmp.begin(), tmp.end());
	}
	pthread_mutex_unlock(&_mutex_products);
	
	return setins;
}

//获取product对应合约 遍历合约
set<string> BaseInfo::get_product_instrument(string productid)
{
	set<string>  tmp;
	
	pthread_mutex_lock(&_mutex_instruments);
	typeof(instruments.begin()) itr = instruments.begin();
	for(; itr!=instruments.end(); itr++)
	{
		if(productid == itr->second.ctp_instrument.ProductID)
		{
			tmp.insert(itr->first);
		}
	}
	pthread_mutex_unlock(&_mutex_instruments);
	
	return tmp;
}

int BaseInfo::get_product_instrument_size(string instrument)
{
	string productid = get_instrument_product(instrument);
	if(productid == "")  productid = instrument;

	int size = 0;
	pthread_mutex_lock(&_mutex_product_instrument_index);
	typeof(product_instrument_index.begin()) itr = product_instrument_index.find(productid);
	if(itr != product_instrument_index.end())
		size = itr->second.size();
	pthread_mutex_unlock(&_mutex_product_instrument_index);
	return size;
}



set<string> BaseInfo::get_instrument_set()
{
	set<string> l;
	pthread_mutex_lock(&_mutex_instruments);
	typeof(instruments.begin()) itr = instruments.begin();
	for(; itr!=instruments.end(); itr++)
	{
		l.insert(itr->second.ctp_instrument.InstrumentID);
	}
	pthread_mutex_unlock(&_mutex_instruments);
	return l;
}

















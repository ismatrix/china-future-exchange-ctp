/*************************************************************************
  > File Name: ProductIndex.h
  > Created Time: Tue 28 Jun 2016 02:39:59 PM CST
  > Author: 
  > description:	1. 日库数据库查询等 和计算逻辑
  2. 包含了商品指数计算逻辑 
 ************************************************************************/

#include "ProductIndex.h"




//日k写db
void  write_daybar_db(const string& tradingday, const string& instrument, const ::CM::DayBar& daybar)
{
	//if(daybar.Volume < 0.0001) return;
	
	CMongodb db = CMongodb::Instance();
	db.ChangeCollection("DAYBAR");
	Json::Value r, s,q;
	get_json_daybar((::CM::DayBar*)&daybar, s);	
	s["instrument"] = instrument;
	s["tradingday"] = tradingday;
	s["updatedate"] = stamp_to_isodate(time(NULL));
	//if(master_instrument != "") s["master_instrument"] = master_instrument;
	//r["$set"] = s;

	q["instrument"] = instrument;
	q["tradingday"] = tradingday;
	
	Json::FastWriter writer;  
	std::string ss = writer.write(s);

	int ret = db.Update(ss, q.toStyledString(), true);
	if(ret <0)
	{
		LOG_ERROR("db fail DayBar:" << db.GetLastErr() << " sql=" << ss);
	}
	LOG_DEBUG(ss << " " << db.GetLastErr());
}

//用来写主连合约日k
void  write_daybar_db_master(const string& tradingday, const string& instrument, const ::CM::DayBar& daybar, const string& master_instrument)
{
	//if(daybar.Volume < 0.0001) return;
	
	CMongodb db = CMongodb::Instance();
	db.ChangeCollection("DAYBAR");
	Json::Value s,q, r;
	get_json_daybar((::CM::DayBar*)&daybar, s);	
	s["instrument"] = instrument;
	s["tradingday"] = tradingday;
	s["master_instrument"] = master_instrument;
	s["updatedate"] = stamp_to_isodate(time(NULL));
	//r["$set"]	= s;

	q["instrument"] = instrument;
	q["tradingday"] = tradingday;
	
	Json::FastWriter writer;  
	std::string ss = writer.write(s);

	int ret = db.Update(ss, q.toStyledString(), true);
	if(ret <0)
	{
		LOG_ERROR("db fail DayBar:" << db.GetLastErr() << " sql=" << ss);
	}
	LOG_DEBUG(ss << " " << db.GetLastErr());
}


//分k写文件
void write_bar_file(const string& tradingday, const string& instrument, const ::CM::Bar& bar)
{
	CFile f("./hqdata");
	list< ::CM::Bar> bars;
	bars.push_back(bar);
	f.WriteTxt(bars, tradingday, instrument);
	LOG_DEBUG("tradingday=" << tradingday << " instrument=" << instrument);
}

//读取主力合约的tick和分k写主连合约 
void  write_bar_file_master(const string& tradingday, const string& instrument, const string& master_instrument)
{
	CFile f("./hqdata");
	list< ::CM::Bar> bars;
	f.ReadTxt(bars, tradingday, master_instrument);
	if(bars.size() > 0) f.WriteTxt(bars, tradingday, instrument);
	LOG_DEBUG("tradingday=" << tradingday << " instrument=" << instrument << " size=" << bars.size());
	
	//写ticker
	list< ::CM::Ticker> ticker;
	f.ReadTxt(ticker, tradingday, master_instrument);
	if(ticker.size() > 0) f.WriteTxt(ticker, tradingday, instrument);
	LOG_DEBUG("tradingday=" << tradingday << " instrument=" << instrument << " size=" << bars.size());
	
}

//ticker写文件
void write_master_ticker_file(const string& tradingday, const string& instrument, const ::CM::Ticker& ticker)
{
    CFile f("./hqdata");
    list< ::CM::Ticker> tl;
    tl.push_back(ticker); 
    f.WriteTxt(tl, tradingday, instrument);
    //LOG_DEBUG("tradingday=" << tradingday << " instrument=" << instrument);
}

//更新合约表合约排名到数据库中
void  update_instrument_rank(const string& instrument, const int prerank, const int rank, double preopeninterest, double openinterest, const int volume)
{
	Json::Value s, r;
	//get_CThostFtdcInstrumentField(root, &ins.ctp_instrument);
	
	s["update_rank_date"]	= stamptostr();
	s["prerank"]			= prerank;
	s["preopeninterest"]	= preopeninterest;
	s["rank"]				= rank;
	s["openinterest"]		= openinterest;
	s["volume"]				= volume;
	s["instrumentid"]		= instrument;
	r["$set"]	= s;

	Json::Value q;                                                                                          
	q["instrumentid"] = instrument;  

	CMongodb _db =  CMongodb::Instance();
	_db.ChangeCollection("INSTRUMENT");
	int ret = _db.Update(r.toStyledString(), q.toStyledString(), true);
	if(ret < 0)
    {
	    LOG_ERROR("update fail. " << instrument << " " << s.toStyledString());
    }
}

void CProductIndex::reset_data()
{
	product_bar.clear();
	product_daybar.clear();
	bar1_dir.clear();
}

void CProductIndex::reset_all()
{
	reset_data();
	_callbackd.clear();
	_callback.clear();
}

void CProductIndex::set_product_daybar(const string& productid, const string& tradingday, const string& instrument,  const ::CM::DayBar& daybar)
{

	DayBarExtern item;
	item.instrument = instrument;
	item.daybar = daybar;

	typeof(product_daybar.begin()) itr = product_daybar.find(productid);
	if(itr != product_daybar.end())
	{
		map<string, vector<DayBarExtern> >& td = itr->second;
		typeof(td.begin()) it = td.find(tradingday);
		if(it != td.end())
		{
			it->second.push_back(item);
		}
		else
		{
			td[tradingday] = vector<DayBarExtern>(1, item);
		}
	}
	else
	{
		map<string, vector<DayBarExtern> > td;
		td[tradingday] = vector<DayBarExtern>(1, item);
		product_daybar[productid] = td;
	}
}


void CProductIndex::set_product_bar(const string& productid, const string& tradingday, const string& instrument, const ::CM::Bar&  bar)
{
	BarExtern item;
	item.bar = bar;
	item.instrument = instrument; 

	typeof(product_bar.begin()) itr1 = product_bar.find(productid);
	if(itr1 != product_bar.end())
	{
		map<string, map<long, vector<BarExtern> > > & day = itr1->second;
		typeof(day.begin()) ditr = day.find(tradingday);
		if(ditr != day.end())
		{
			map<long, vector<BarExtern> >& td = ditr->second;
			typeof(td.begin()) it1 = td.find((long)item.bar.Timestamp);
			if(it1 != td.end())
			{
				it1->second.push_back(item);
			}
			else
			{
				td[(long)item.bar.Timestamp] = vector<BarExtern>(1, item);
			}
		}
		else
		{
			map<long, vector<BarExtern> > td;
			td[(long)item.bar.Timestamp] = vector<BarExtern>(1, item);
			day[tradingday] = td;
		}
	}
	else
	{
		map<long, vector<BarExtern> > td;
		td[(long)item.bar.Timestamp] = vector<BarExtern>(1, item);
		map<string, map<long, vector<BarExtern> > >  day;
		day[tradingday] = td;
		product_bar[productid] = day;
	}
}

int CProductIndex::fill_data_product_daybar(vector<string>& productids)
{
	product_daybar.clear();
	map<string, vector<DayBarExtern> > dat;
	for(uint32_t i=0; i<productids.size(); i++)
	{
		product_daybar[productids[i]] = dat;
	}
	return 0;
}

int CProductIndex::fill_data_product_bar(vector<string>& productids)
{
	product_bar.clear();
	map<string, map<long, vector<BarExtern> > >  dat;
	for(uint32_t i=0; i<productids.size(); i++)
	{
		product_bar[productids[i]] = dat;
	}
	return 0;
}

string  CProductIndex::get_productid(string instrument)
{
	//过滤主连  指数
	if(instrument.find("0001") != string::npos || instrument.find("IDX") != string::npos)
		return "";
    return instrument_products[instrument];

	string product;
	uint32_t size = instrument.size();
	for(uint32_t i=0; i<size; i++)
	{
		if(!((instrument[i] >= '0' && instrument[i] <= '9')     || 
					(instrument[i] >= 'A' && instrument[i] <= 'Z') ||
					(instrument[i] >= 'a' && instrument[i] <= 'z')))
		{
			LOG_ERROR(instrument << " not conform to the rule.");
			return "";
		}

		if(instrument[i] >='0'  && instrument[i] <= '9')
		{
			return instrument.substr(0, i);
		}
	}

	LOG_ERROR(instrument << " not conform to the rule.");
	return "";
}

//查询所有的
int CProductIndex::query_daybar(string _tradingdday, string _productid)
{
	product_daybar.clear();
    
    LOG_DEBUG("tradingday=" << _tradingdday << " _productid="<< _productid)

	CMongodb db = CMongodb::Instance();
	db.ChangeCollection("DAYBAR");

	Json::Value regex;
	regex["$regex"] = _productid;
	Json::Value q, gt;

	//成交量大于0
	//gt["$gt"] = 0;
	//q["volume"] = gt;

	string ss="{}";
	if(_tradingdday != "") 
	{
		q["tradingday"]  = _tradingdday;
		ss = q.toStyledString();
	}
	if(_productid != "")
	{
		q["instrument"]   = regex;
		ss = q.toStyledString();
	}	

	if(db.Query(ss) < 0)
	{
		LOG_ERROR("mongo query fail. err:" << db.GetLastErr());
		return -1;;
	}
    
    LOG_DEBUG("query daybar condition:" << ss)

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
				continue;
			}
			string instrument = val["instrument"].asString();
			string tradingday = val["tradingday"].asString();
			string productid = get_productid(instrument);

			LOG_DEBUG("db daybar tradingday=" << tradingday << " instrument=" << instrument << " productid=" << productid);

			if(productid == "")
			{
                LOG_ERROR("instrument=" <<  instrument << " productid None");
				continue;
			}

			if(_productid!="" && productid != _productid)
			{
				LOG_DEBUG("similar product. db productid=" << productid << " query productid=" << _productid);
				continue;
			}


			::CM::DayBar daybar;
			get_struct_daybar(&daybar, val);
			set_product_daybar(productid, tradingday, instrument,  daybar);
		}
		catch(Json::Exception& e)
		{
			LOG_ERROR(e.what());
		}
		catch(...)
		{
			LOG_ERROR("db.Next() fail.");
		}
	}
	return 0;
}

int CProductIndex::fill_daybar()
{
	return query_daybar();
}

//查询tradingday的
int CProductIndex::fill_daybar_tradingday(string tradingday)
{
	return query_daybar(tradingday);
}

int CProductIndex::fill_daybar_product(string productid)
{
	return query_daybar("", productid);
}

//fill path对应表
int CProductIndex::fill_bar_dir(string& path, string& tradingday, string& productid)
{
	string _instrument,_tradingday;
	int len = path.size();

	// /M1_cs1703_20160330.txt
	_tradingday  = path.substr(len-12, 8);
	std::size_t pos = path.rfind('/');
	if(pos != std::string::npos)
		_instrument = path.substr(pos+4, len-pos-17);

	string _productid = get_productid(_instrument);
	//LOG_DEBUG(path << " _tradingday=" << _tradingday << " _instrument=" << _instrument << " _productid="<<_productid);


	if(_productid == "") 
	{
		//LOG_DEBUG("_productid=" << _productid);
		return -1;
	}
	if(tradingday != "" && tradingday != _tradingday)
	{
		//LOG_DEBUG("tradingday=" << tradingday << " _tradingday=" << _tradingday <<  "_tradingday!=tradingday = " << (tradingday != _tradingday));
		return -1;
	}
	if(productid != "" && productid != _productid)
	{
		//LOG_DEBUG("productid=" << productid << " _productid=" << _productid << " _productid!=productid=" << (productid!=_productid));
		return -1;
	}


	typeof(bar1_dir.begin()) itr = bar1_dir.find(_productid);
	if(itr != bar1_dir.end())
	{
		map<string, vector<string> >& pt = itr->second;
		typeof(pt.begin()) it = pt.find(_tradingday);
		if(it != pt.end())
		{
			it->second.push_back(_instrument);
		}
		else
		{
			pt[_tradingday] = vector<string>(1, _instrument);
		}
	}
	else
	{
		map<string, vector<string> > pt;
		pt[_tradingday] = vector<string>(1, _instrument);  
		bar1_dir[_productid] = pt;
	}
	LOG_DEBUG(path << " _tradingday=" << _tradingday << " _instrument=" << _instrument << " _productid="<<_productid << " bar1_dir.size=" << bar1_dir.size());
	return 0;
}

//遍历目录
int CProductIndex::check_dir(string prefix, string tradingday, string productid)
{


	DIR *dirp;
	struct dirent *direntp;

	if((dirp = opendir(prefix.c_str())) == NULL)
	{
		LOG_ERROR("opendir " << prefix << " fail");
		return -1;
	}

	struct stat statbuf;  
	while((direntp = readdir(dirp)) != NULL) 
	{
		if(strncmp(direntp->d_name, ".",  1) == 0) continue;

		string path = prefix + "/" + direntp->d_name;
		lstat(path.c_str(), &statbuf);
		if (S_ISDIR(statbuf.st_mode))
		{
			check_dir(path, tradingday, productid);
			continue;
		}
		else if(S_ISREG(statbuf.st_mode))
		{
			if(strncmp(direntp->d_name, "M1_", 3)==0)
			{
				fill_bar_dir(path, tradingday, productid);				
				//	LOG_DEBUG(path <<  " bar1_dir.size=" << bar1_dir.size());
			}
		}
	}

	closedir(dirp);
	return 0;
}


//根据bar1_dir  读取分k填充 product_bar
int CProductIndex::fill_product_bar(string prefix)
{
	LOG_DEBUG("bar1_dir.size=" << bar1_dir.size());
	typeof(bar1_dir.begin()) itr = bar1_dir.begin();
	for(; itr!=bar1_dir.end(); itr++)
	{
		string productid = itr->first;
		map<string, vector<string> >& pt = itr->second;

		typeof(pt.begin()) it = pt.begin();
		for(; it!=pt.end(); it++)
		{
			string _tradingday = it->first;
			vector<string> _instruments = it->second;

			for(uint32_t i=0; i<_instruments.size(); i++)
			{
				list< ::CM::Bar> bars;
				CFile f(prefix);
				f.ReadTxt(bars, _tradingday, _instruments[i]);

				LOG_DEBUG("tradingday=" << _tradingday << "productid=" << productid << " instrument=" << _instruments[i] << " size="<<bars.size());

				list< ::CM::Bar>::iterator ltr  = bars.begin();
				for(; ltr!=bars.end(); ltr++)
				{
					set_product_bar(productid, _tradingday, _instruments[i], *ltr);
				}
			}
		}
	}
	return 0;
}

//查询产品的
int CProductIndex::fill_bar(string prefix)
{
	product_bar.clear();
	bar1_dir.clear();
	check_dir(prefix);
	return 	fill_product_bar(prefix);
}

//读取文件中的
int CProductIndex::fill_bar_key(string tradingday, string prefix)
{
	product_bar.clear();
	bar1_dir.clear();
	check_dir(prefix, tradingday);
	return fill_product_bar(prefix);
}

int CProductIndex::fill_bar_product(string productid, string prefix)
{
	product_bar.clear();
	bar1_dir.clear();
	check_dir(prefix, "", productid);
	return fill_product_bar(prefix);
}



int CProductIndex::calculate_daybar()			//计算日k指数数据并入库
{
	LOG_DEBUG("calculate size=" << product_daybar.size());

	typeof(product_daybar.begin()) itr = product_daybar.begin();
	for(; itr!=product_daybar.end(); itr++)
	{
		string productid = itr->first;
		string instrument = productid + "IDX";
		map<string, vector<DayBarExtern> >& td = itr->second;

		LOG_DEBUG("productid=" << productid << " tradingday size=" << td.size());

		typeof(td.begin()) it = td.begin();
		for(; it!=td.end(); it++)
		{
			string tradingday = it->first;
			vector<DayBarExtern>& daybars = it->second;

			::CM::DayBar daybar = {0};
			for(uint32_t i=0; i<daybars.size(); i++)
			{
				daybar.Timestamp =  daybars[i].daybar.Timestamp;
				daybar.Volume	 += daybars[i].daybar.Volume;
				daybar.Turnover	 += daybars[i].daybar.Turnover;
				daybar.OpenInterest		+= daybars[i].daybar.OpenInterest;
				daybar.PreoOpenInterest	 += daybars[i].daybar.PreoOpenInterest;
			}
            
            if(daybar.OpenInterest == 0) daybar.OpenInterest = 1;
			for(uint32_t i=0; i<daybars.size(); i++)
			{
				daybar.High			+= daybars[i].daybar.High		* daybars[i].daybar.OpenInterest/daybar.OpenInterest; 
				daybar.Low			+= daybars[i].daybar.Low		* daybars[i].daybar.OpenInterest/daybar.OpenInterest; 
				daybar.Open			+= daybars[i].daybar.Open		* daybars[i].daybar.OpenInterest/daybar.OpenInterest; 
				daybar.Close		+= daybars[i].daybar.Close		* daybars[i].daybar.OpenInterest/daybar.OpenInterest; 
				daybar.Average		+= daybars[i].daybar.Average	* daybars[i].daybar.OpenInterest/daybar.OpenInterest; 
				daybar.Settlement	+= daybars[i].daybar.Settlement	* daybars[i].daybar.OpenInterest/daybar.OpenInterest; 
				daybar.UpperLimit	+= daybars[i].daybar.UpperLimit * daybars[i].daybar.OpenInterest/daybar.OpenInterest; 
				daybar.LowerLimit	+= daybars[i].daybar.LowerLimit * daybars[i].daybar.OpenInterest/daybar.OpenInterest; 

				daybar.PreSettlement+= daybars[i].daybar.PreSettlement	* daybars[i].daybar.PreoOpenInterest/daybar.PreoOpenInterest; 
				daybar.PreClose		+= daybars[i].daybar.PreClose		* daybars[i].daybar.PreoOpenInterest/daybar.PreoOpenInterest; 
			}
			
			daybar.Price = daybar.Close;

			LOG_DEBUG("instrument=" << instrument << " tradingday=" << tradingday << " daybars.size=" << daybars.size());

			//成交量为0的就是没有
			//if(daybar.Volume < 0.0001) continue;

			typeof(_callbackd.begin()) citr = _callbackd.begin();
			for(; citr!=_callbackd.end(); citr++)
			{
				(*citr)(tradingday, instrument, daybar);
			}
		}
	}
	return 0;
}

int CProductIndex::calculate_bar()
{
	LOG_DEBUG("calculate size=" << product_bar.size());
	typeof(product_bar.begin()) itr = product_bar.begin();
	for(; itr!=product_bar.end(); itr++)
	{
		string productid = itr->first;
		string instrument = productid + "IDX";
		map<string, map<long, vector<BarExtern> > >& day = itr->second;


		typeof(day.begin()) ditr = day.begin();
		for(; ditr!=day.end(); ditr++)
		{
			string tradingday = ditr->first;
			map<long, vector<BarExtern> >& td = ditr->second;

			LOG_DEBUG("productid=" << productid << " tradingday=" << tradingday << " size=" << td.size());

			typeof(td.begin()) it = td.begin();
			for(; it!=td.end(); it++)
			{
				long key = it->first;
				vector<BarExtern>& bars = it->second;

				::CM::Bar bar = {0};
				bar.Timestamp	=  key;
				for(uint32_t i=0; i<bars.size(); i++)
				{
					bar.Volume				+= bars[i].bar.Volume;
					bar.Turnover			+= bars[i].bar.Turnover;
					bar.OpenInterest		+= bars[i].bar.OpenInterest;
				}
				for(uint32_t i=0; i<bars.size(); i++)
				{
					bar.High	+= bars[i].bar.High * bars[i].bar.OpenInterest/bar.OpenInterest; 
					bar.Low		+= bars[i].bar.Low  * bars[i].bar.OpenInterest/bar.OpenInterest; 
					bar.Open	+= bars[i].bar.Open * bars[i].bar.OpenInterest/bar.OpenInterest; 
					bar.Close	+= bars[i].bar.Close* bars[i].bar.OpenInterest/bar.OpenInterest; 
				}

				//LOG_DEBUG("instrument=" << instrument << " tradingday=" << tradingday << " key=" << key << " bars.size=" << bars.size());

				//if(bar.Volume < 0.0001) continue;

				typeof(_callback.begin()) citr = _callback.begin();
				for(; citr!=_callback.end(); citr++)
				{
					(*citr)(tradingday, instrument, bar);
				}
			}
		}
	}
	return 0;
}

void CProductIndex::set__callbackd(_callback_daybar _call)
{
	_callbackd.push_back(_call);
}

void CProductIndex::set__callback(_callback_bar _call)
{
	_callback.push_back(_call);
}
	
void CProductIndex::set__callback_masterd( _callback_daybar_master     callback_daybar_master)
{
	this->callback_daybar_master = callback_daybar_master;
}

void CProductIndex::set__callback_master( _callback_bar_master     callback_bar_master) 
{
	this->callback_bar_master = callback_bar_master;
}

void CProductIndex::set__callback_instrument_rank(_callback_instrument_rank callback_instrument_rank)
{
	this->callback_instrument_rank = callback_instrument_rank;
}

bool CProductIndex::check_instrument_expires_tomorrow(string& tradingday, Instrument& ctp)
{
	if(atoll(tradingday.c_str()) >= atoll(ctp.ctp_instrument.ExpireDate))
	{
	    return true; //表示要过期   
	}

	return false; //不过期
}

//不能同时更新日k分k  和  合约排名只有当前交易日才能
int CProductIndex::calculate_master_instrument()			//计算日k指数数据并入库
{
	LOG_DEBUG("calculate size=" << product_daybar.size());

	//查询合约表信息
	if(callback_instrument_rank != NULL && query_instrument() < 0)
    {
        LOG_ERROR("query_instrument error.");
        return -1;
    }
	
    string tradingday = "";
	typeof(product_daybar.begin()) itr = product_daybar.begin();
	for(; itr!=product_daybar.end(); itr++)
	{
		string productid = itr->first;
		map<string, vector<DayBarExtern> >& td = itr->second;

		LOG_DEBUG("productid=" << productid << " tradingday size=" << td.size());

		//instrument rank
		string instrument = productid + "0001";
		typeof(td.begin()) it = td.begin();
		for(; it!=td.end(); it++)
		{
			tradingday = it->first;
			vector<DayBarExtern>& daybars = it->second;

			//OpenInterest  instrument
			map<int, DayBarExtern> maptmp; // yes_tmp;
			//对vector<DayBarExtern> 排名
			for(uint32_t i=0; i<daybars.size(); i++)
			{
				maptmp[daybars[i].daybar.OpenInterest]  = daybars[i];
				//yes_tmp[daybars[i].daybar.PreoOpenInterest]  = daybars[i];

				if(callback_instrument_rank!= NULL)
					set_openinterest(productid, daybars[i].instrument, (double)daybars[i].daybar.OpenInterest, (double)daybars[i].daybar.PreoOpenInterest, (int)daybars[i].daybar.Volume);
			}

			//更新主连日k 和  全天分k 如果设置回调的话
			if(maptmp.rbegin() != maptmp.rend())
			{
				if(callback_daybar_master != NULL) callback_daybar_master(tradingday,  instrument,  maptmp.rbegin()->second.daybar, maptmp.rbegin()->second.instrument);
				if(callback_bar_master != NULL) callback_bar_master(tradingday,  instrument, maptmp.rbegin()->second.instrument);
			}
    	}
    }
    //所有产品  只有当前交易日才会被调用到一次
	if(callback_instrument_rank != NULL && tradingday!="") calculate_instrument(tradingday);
	
	return 0;
}

//1. 加载所有的合约表
//2. 填充并且计算合约表数据
//3. 计算并梳理合约表
int  CProductIndex::query_instrument()
{
	instrumenttable.clear();
	Json::Value qp;
	CMongodb db = CMongodb::Instance();
	db.ChangeCollection("INSTRUMENT");
	qp["productclass"] =  "1";
	
    //Json::Value q, in, a;
    //a.append("1");
    //a.append("2");
    //in["$in"] = a; 
	//qp["productclass"] =  in;
    
    qp["istrading"] =  1;  
	if(db.Query(qp.toStyledString()) < 0)
	{
		 LOG_ERROR("mongo query fail. err:" << db.GetLastErr());
		 return -1;;
	}
	int i = 0;
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
			if(!val["rank"].isNull())               p.prerank              =   val["rank"].asInt();
			if(!val["openinterest"].isNull())       p.preopeninterest      =   val["openinterest"].asInt();
			if(!val["volume"].isNull())       		p.volume      			=   val["volume"].asInt();
			
			p.rank = 9999;
			p.openinterest = 0;
			p.volume = 0;
			
			
			//if(!val["prerank"].isNull())            p.prerank           =   val["prerank"].asInt();
			//if(!val["preopeninterest"].isNull())    p.preopeninterest   =   val["preopeninterest"].asInt();
			if(!val["updatetime"].isNull())         p.updatetime    =   val["updatetime"].asInt();
            set_instrumenttable(&p);
			i++;
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
    if(i < 10)
    {
	    LOG_ERROR("query db instrumeny size=" << i << " less number. not do something");
        return -1;
    }
	LOG_INFO("query db instrumeny size=" << i);
	return 0;
}

void CProductIndex::set_instrumenttable(Instrument* p)
{
	typeof(instrumenttable.begin()) itr = instrumenttable.find(p->ctp_instrument.ProductID);
	if(itr != instrumenttable.end())
	{
		map<string, Instrument>& ins = itr->second;
		ins[p->ctp_instrument.InstrumentID] = *p;
	}
	else
	{
		map<string, Instrument> ins;
		ins[p->ctp_instrument.InstrumentID] = *p;
		instrumenttable[p->ctp_instrument.ProductID] = ins;	
	}
    instrument_products[p->ctp_instrument.InstrumentID] = p->ctp_instrument.ProductID;
}

void CProductIndex::set_openinterest(string& productid, string& instrument, double openinterest, double  preopeninterest, const int volume)
{
    LOG_INFO(productid << " " << instrument << " " << openinterest << " " << preopeninterest);
	typeof(instrumenttable.begin()) itr = instrumenttable.find(productid);
	if(itr != instrumenttable.end())
	{
		map<string, Instrument>& ins = itr->second;
		map<string, Instrument>::iterator itr = ins.find(instrument);
		if(itr != ins.end())
		{
			//itr->second.rank
			//itr->second.prerank
			itr->second.openinterest		= openinterest;
			itr->second.preopeninterest     = preopeninterest;
			itr->second.volume     			= volume;
		}
		else
		{
			LOG_ERROR("InstrumentID=" << instrument << " not found.");
		}
	}
	else
	{
		LOG_ERROR("ProductID=" << productid << " not found." );
	}
}



int CProductIndex::calculate_instrument(string&  tradingday)
{
	//这个在计算合约表之前查询
	//query_instrument();	//查询数据库合约表
	//1.判断将要过期合约 更新istrading=0 and 更新rank=9999 并删除这条记录
	CMongodb _db = CMongodb::Instance();
	_db.ChangeCollection("INSTRUMENT");
	
	typeof(instrumenttable.begin()) itr = instrumenttable.begin();
	for(; itr!=instrumenttable.end(); itr++)
	{
		map<string, Instrument>& ins = itr->second;
		map<string, Instrument>::iterator it = ins.begin();
		for(; it!=ins.end(); )
		{
			if(check_instrument_expires_tomorrow(tradingday, it->second))
			{
				//更新数据库istrading rank
				Json::Value s, r;
				s["update_rank_date"]	= stamptostr();
				s["rank"]				= 999; //过期合约
				s["istrading"]			= 0;
				s["instrumentid"]		= it->first;  
				r["$set"]	= s;

				Json::Value q;                                                                                          
				q["instrumentid"] = it->first;  

				_db.Update(r.toStyledString(), q.toStyledString(), true);
				LOG_DEBUG(s.toStyledString());
							
				LOG_INFO("instrument=" << it->first << " tradingday=" << tradingday<< " expires filtered.");
				ins.erase(it++);
			}
			else
			{
				it++;
			}
		}
	}

	//2.遍历所有记录排序rank今天和昨天
	itr = instrumenttable.begin();
	for(; itr!=instrumenttable.end(); itr++) //这个是遍历产品
	{
		map<string, Instrument>& ins = itr->second;
	
		map<string, Instrument>::iterator it = ins.begin();
		for(; it!=ins.end(); it++)   //这个是遍历合约列表
		{
			int rank=1, prerank=1; //相同持仓按序站位
			map<string, Instrument>::iterator oit = ins.begin();
			for(; oit!=it; oit++)
			{
				if(oit->second.openinterest >= it->second.openinterest
					&& oit->second.volume >= it->second.volume
				) {	rank++;	} 
				else if(oit->second.openinterest >= it->second.openinterest) {	rank++;	} 
				
				if(oit->second.preopeninterest	>= it->second.preopeninterest) prerank++;
			}
			
			for(oit++; oit!=ins.end(); oit++)
			{
				if(oit->second.openinterest	> it->second.openinterest
					&& oit->second.volume > it->second.volume
				) { rank++; }
				else if (oit->second.openinterest	> it->second.openinterest) { rank++; }
				
				if(oit->second.preopeninterest	> it->second.preopeninterest) prerank++;
			}

			if(callback_instrument_rank != NULL)  callback_instrument_rank(it->first, prerank, rank, it->second.preopeninterest, it->second.openinterest, it->second.volume);

            LOG_INFO((it->first) << " prerank:" << prerank << " rank:" << rank << " preopeninterest:" << it->second.preopeninterest << " openinterest:" << it->second.openinterest);
		}
	}
	return 0;
}


/////////////////////////动态计算指数 每个单独快照过来的时候生成一个综合快照///////////////////













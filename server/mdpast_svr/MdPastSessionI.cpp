/////////////////////////////////////////////////////////////////////////
///@system 交易系统
///@company 慧网基金
///@file MdSessionI.cpp
///@brief 定义了客户端接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#include "MdPastSessionI.h"

using namespace std;

long time_now = 0;

::std::string MdSessionI::queryInstruments(const ::MdPast::StringList& symbols, const ::MdPast::StringList& products, const ::MdPast::StringList& exchanges, const ::MdPast::IntList& ranks, const ::MdPast::StringList& productclass, const ::MdPast::IntList& istrading, const ::Ice::Current& current)
{
    //查询数据库
    CMongodb db(CMongodb::Instance());
    db.ChangeCollection("INSTRUMENT");
    //db.INSTRUMENT.find({instrumentid: {$in: ['ag1706']}, productid: {$in: ['ag']}, productclass: {$in: ['1, 2']}, rank:{$in: [1,2]}, exchangeid:{$in:['SHFE']}, istrading:1}).pretty()
    
    string sq = "{}";
    if(!(symbols.size()==0 && products.size()==0     && exchanges.size()==0 && 
       ranks.size()==0   && productclass.size()==0 && istrading.size()==0))
    {
        Json::Value q;
        combine_query_condition("instrumentid", q, symbols);
        combine_query_condition("productid",    q, products);
        combine_query_condition("productclass", q, productclass);
        combine_query_condition("rank",         q, ranks);
        combine_query_condition("exchangeid",   q, exchanges);
        combine_query_condition("istrading",    q, istrading);
        sq = q.toStyledString();
    }

    string s = "[";
    long num = 0;
    if(db.Query(sq) < 0)
    {
        LOG_ERROR("mongo query fail. err:" << db.GetLastErr());
        return "-1";
    }
    try
    {
        while(db.Next())
        {
            char* result = db.GetResult();
            s += result;
            s += ",";
            num++;
        }
    }
    catch(...)
    {
        LOG_ERROR("db.Next() fail.");
        return "-1";
    }
    if(s.size() > 2) s[s.size()-1] = ']'; else s += "]";
    LOG_INFO("query:" << sq << "\nresult: " << num);

    return s;
}

::std::string MdSessionI::queryData(const Ice::Identity& ident, const ::std::string& DataType, const ::std::string& InstrumentID, const ::std::string& starttime, const ::std::string& endtime, const ::Ice::Current&  current)
{
    time_now = time(NULL);
    //Lock sync(*this);
	//创建发送的代理 Identity& ident
	MdSessionCallBackPrx client = MdSessionCallBackPrx::uncheckedCast(current.con->createProxy(ident));
	//获取连接的客户端信息
	::Ice::IPConnectionInfoPtr connectionPtr = ::Ice::IPConnectionInfoPtr::dynamicCast(current.con->getInfo());



    if(DataType == "M") *(string*)&DataType = "M1";

    //20160204 time()
	time_t st = strtostamp(starttime.c_str(), "%Y%m%d");
	time_t et = strtostamp(endtime.c_str(), "%Y%m%d");

   
    //最多超过当前时间的2天 
    if(et - time(NULL) > 86400 * 5) et = get_ymd(time(NULL)) + 86400 * 2;
	
    LOG_INFO("request[" << connectionPtr->remoteAddress << ":" << (uint32_t)connectionPtr->remotePort << "]" << InstrumentID << " " << starttime << "---------" << endtime << "(real " << stamptostr(et, "%Y%m%d") <<")");

	if(st > et)
	{
		LOG_ERROR("starttime=" << st << " < endtime=" << et);
		return "param error.";
	}

    if(DataType == "DM")
    {
		request_daybar(client,  InstrumentID, starttime, endtime) ;
	    return "0";
    }
	
    list< ::CM::DayBar> dbars;
    if(DataType.find("M") != string::npos)
    {
        time_t sn = g_holidays.GetPrevTradingDay(st);
        string startnew = stamptostr(sn, "%Y%m%d");
        if(get_daybar(dbars, InstrumentID, startnew, endtime) < 0)
            return "db query error.";
    }

	try
	{
		while(st <= et)
		{
			string tradingday = stamptostr(st, "%Y%m%d");

			if(DataType == "T")
			{
				request_ticker(client, InstrumentID, tradingday);
			}
			else if(DataType.find("M") != string::npos)
			{
                request_bar(client,  dbars, InstrumentID, st, atoll(DataType.substr(1).c_str()));
			}
            else
			{
				LOG_ERROR("DataType=" << DataType);
				return "DataType error";
			}
			
			st += 86400;
		}
	}
	catch(Ice::Exception& e)
	{
		stringstream ss;
		ss << e;
		LOG_ERROR(ss.str());
	}

	return "0";
}


int MdSessionI::request_ticker(MdSessionCallBackPrx& client, const string& InstrumentID, const string& tradingday)
{
	list< ::CM::Ticker>  tickers;
	CFile f("/mnt/hqdata");
	//int ret = f.ReadDat(tickers, tradingday, InstrumentID);
	int ret = f.ReadTxt(tickers, tradingday, InstrumentID);
	if(ret < 0 ) return ret; 

	list< ::CM::Ticker>::iterator itr = tickers.begin();
	for(; itr!=tickers.end(); itr++)
	{
		client->begin_onTick(tradingday, InstrumentID, *itr);
	//	current.adapter->getCommunicator()->onTick(tradingday, InstrumentID, *itr);
	}

	LOG_INFO("InstrumentID=" << InstrumentID << " tradingday=" << tradingday);

	return 0;
}

    
//获取1mk
::CM::Bar get_1minute_bar(int64_t t, list< ::CM::DayBar>&  dbars, int64_t tradingday, list< ::CM::Bar>& bars, list< ::CM::Bar>::iterator& it)
{

    Bar tb;
    memset(&tb, 0, sizeof(tb));

    while(it != bars.end() && (long)it->Timestamp < t) it++;
    
    if(it != bars.end() && (long)it->Timestamp == t) 
    {
        return *it;
    }

    double close = 0, io = 0;
    if(it == bars.begin()) 
    {
        list< ::CM::DayBar>::iterator itr = dbars.begin();
        for(; itr!=dbars.end(); itr++)
        {
            //LOG_DEBUG("itr->Timestamp =" << ((int64_t)itr->Timestamp)  << " tradingday=" << tradingday);
            if(itr->Timestamp >= tradingday) break;
        }
        if(itr == dbars.begin()) return tb; 
        if(itr->Timestamp > tradingday) itr--;
        //LOG_DEBUG("itr->Timestamp =" << ((int64_t)itr->Timestamp)  << " tradingday=" << tradingday);
        close = itr->PreClose;
        io = itr->OpenInterest;
        //LOG_INFO("tradingday=" << tradingday << "Timestamp=" << ((int64_t)itr->Timestamp) << " close=" << close);
    }
    else
    {
        if(it == bars.begin()) return tb; //没数据来补齐
        it--;
        close = it->Close;
        io = it->OpenInterest;
        //LOG_INFO("Timestamp=" << (int)(it->Timestamp) << " close=" << close);
    }
    
    tb.Timestamp     = t;   
    tb.High          = close;        
    tb.Low           = close;         
    tb.Open          = close;        
    tb.Close         = close;       
    tb.Volume        = 0;      
    tb.Turnover      = 0;    
    tb.OpenInterest  = io;
    //LOG_INFO("Timestamp=" << (int)(it->Timestamp) << " close=" << close << " open=" << tb.Open);
    return tb;
}

string  get_productid(string instrument)
{
    string product;
    uint32_t size = instrument.size();
    for(uint32_t i=0; i<size; i++)
    {
        if(instrument[i] >='0'  && instrument[i] <= '9')
        {
            return instrument.substr(0, i);
        }
    }

    int pos = instrument.find("IDX");
    if(pos != (int)string::npos)
    {
        return instrument.substr(0, pos);
    }
    return instrument;
}


int MdSessionI::request_bar(MdSessionCallBackPrx& client, list< ::CM::DayBar>&  dbars, const string& InstrumentID, time_t st, uint32_t period)
{
    long limit_time = (time_now - 5)*1000;
    string tradingday = stamptostr(st, "%Y%m%d");
    list< ::CM::Bar> bars;
    CFile f("/mnt/hqdata");
    //int ret = f.ReadDat(bars, tradingday, InstrumentID);
    int ret = f.ReadTxt(bars, tradingday, InstrumentID);
    if(ret < 0) return ret;

    string product = get_productid(InstrumentID);
    _type_timeserise* p = g_timeserise.GetSeriesByTradingday((char*)product.c_str(), st);
    if(p == NULL)
    {
        LOG_ERROR("tradingday=" << tradingday << " g_timeserise.GetSeriesByTradingday error."); 
        return -1;
    }
    _type_timeserise& data = *p;

    list< ::CM::Bar>::iterator it = bars.begin();
    ::CM::Bar bar;
    memset(&bar, 0, sizeof(bar));
    //遍历时间序列表 推送一天的数据
    int i = 0;
    for(; i<(int)data.size(); i++)
    {
        if(data[i].key * 1000 > limit_time) return 0;
        if(i %  period == 0)
        {
            //LOG_DEBUG("i=" << i << " i%period=" << (i%period) << " bar.Timestamp=" << bar.Timestamp  << " bar.High=" << bar.High);
            //推送上一根 
            if(bar.Timestamp > 0 && bar.High > 0.1) client->begin_onBar(tradingday, InstrumentID, bar, period);
            memset(&bar, 0, sizeof(bar));
            bar.Timestamp = data[i].key*1000;
        }
        
        if(data[i].type == 0) continue;

        //合成
        ::CM::Bar b = get_1minute_bar(data[i].key * 1000, dbars, st * 1000,  bars, it);
        if(bar.Open < 0.01) bar.Open = b.Open;
        if(b.High > bar.High) bar.High = b.High;
        if(b.Low > 0.1 && (b.Low < bar.Low || bar.Low < 0.1)) bar.Low = b.Low;
        if(b.Close > 0.1) bar.Close = b.Close;
        bar.Volume        += b.Volume;      
        bar.Turnover      += b.Turnover;    
        bar.OpenInterest  += b.OpenInterest;
        //LOG_DEBUG("i=" << i << " i%period=" << (i%period) << " bar.Timestamp=" << bar.Timestamp  << " bar.High=" << bar.High << " b.High=" << b.High);
    }
    //LOG_DEBUG("i=" << i << " i%period=" << (i%period) << " bar.Timestamp=" << bar.Timestamp  << " bar.High=" << bar.High << " bar,open=" << bar.Open);
    if(bar.Timestamp > 0 && bar.High > 0.1 && bar.Timestamp < limit_time) client->begin_onBar(tradingday, InstrumentID, bar, period);
    
    LOG_INFO("InstrumentID=" << InstrumentID << " period=" << period << " tradingday=" << tradingday << " dbarssize=" << dbars.size() << " kline=" << bars.size() << " timeserise="<< data.size());
    return 0;
}


int MdSessionI::request_daybar(MdSessionCallBackPrx& client, const string& InstrumentID, const string& start, const string& end)
{

    list< ::CM::DayBar> dbars;
    if(get_daybar(dbars, InstrumentID, start, end) < 0)
    {
        return -1;
    }
    for(typeof(dbars.begin()) itr=dbars.begin(); itr!=dbars.end(); itr++)
    {
        string tradingday = stamptostr(itr->Timestamp/1000, "%Y%m%d");
        client->begin_onDay(tradingday, InstrumentID, *itr);
    }
    return 0;
}


int  MdSessionI::get_daybar(list< ::CM::DayBar>& dbars , const string& InstrumentID, const string& start, const string& end)
{
    CMongodb db(CMongodb::Instance());
    db.ChangeCollection("DAYBAR");
    //db.DAYBAR.find({instrument:"ag1706", tradingday:{$gte: "20170111", $lte: "20170117"}}).
    Json::Value q, condition;
    q["instrument"] = InstrumentID;
    condition["$gte"] = start;
    condition["$lte"] = end;
    q["tradingday"] = condition;

    if(db.Query(q.toStyledString()) < 0)
    {
        LOG_ERROR("mongo query fail. err:" << db.GetLastErr());
        return -1;
    }
    try
    {
        while(db.Next())
        {
            char* result = db.GetResult();
            Json::Reader reader(Json::Features::strictMode());    
            Json::Value value;
            if(!reader.parse(result, value))
            {
                LOG_ERROR("reader.parse error. result:" << result);
                return -1;
            }
            ::CM::DayBar dbar;
            get_struct_daybar(&dbar, value);	
            dbars.push_back(dbar);
        }
    }
    catch(...)
    {
        LOG_ERROR("db.Next() fail.");
        return -1;
    }
    LOG_DEBUG(q.toStyledString()<< "=====>" << dbars.size());
    return 0;	
}







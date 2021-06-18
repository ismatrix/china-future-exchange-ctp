/*************************************************************************
    > File Name: timeseries.cpp
    > Created Time: jeu. 09 f茅vr. 2017 17:23:05 CST
    > Author: 
    > description: 
 ************************************************************************/

#include "timeseries.h"


TimeSerise g_timeserise(&g_holidays);

TimeSerise::TimeSerise(Holidays* h)
{
    m_Holidays = h;
}

TimeSerise::~TimeSerise()
{

}

int TimeSerise::Init(CMongodb& db)
{
    //加载数据库数据
    db.ChangeCollection("TIMETABLE");
	db.Query("{}"); //查询所有的节假日
	while(db.Next())
	{
		char* result = db.GetResult();
		Json::Reader reader(Json::Features::strictMode());    
		Json::Value value;
		if(!reader.parse(result, value))
		{
			 LOG_ERROR("reader.parse error. result:" << result);
			 exit(-1);
		}
        _type_timetables v;
		uint32_t id = value["timetableid"].asInt();
        Json::Value a = value["timetable"];
        for(uint32_t i=0; i<a.size(); i++)
        {
            timetable t;
            t.s = t2seconds(a[i][0].asString(), "%H:%M:%S");
            t.e = t2seconds(a[i][1].asString(), "%H:%M:%S");
            t.t = stoenum(a[i][2].asString());
            v.push_back(t);
            LOG_INFO("s="<<t.s<<"---"<<second2ts(t.s)<<" e="<<t.e<<"---"<<second2ts(t.e)<<" t="<<t.t<<"---"<<a[i][2].asString());
        }
	    m_MapTimeTables[id] = v;
    }

    db.ChangeCollection("TIMETABLEMAP");
	db.Query("{}"); //查询所有的节假日
	while(db.Next())
	{
		char* result = db.GetResult();
		Json::Reader reader(Json::Features::strictMode());    
		Json::Value value;
		if(!reader.parse(result, value))
		{
			 LOG_ERROR("reader.parse error. result:" << result);
			 exit(-1);
		}
		uint32_t id = value["timetableid"].asInt();
        Json::Value a = value["symbols"];
        for(uint32_t i=0; i<a.size(); i++)
        {
            uint64_t k = stoint64(a[i].asString().c_str());
            m_MapTimeTablesId[k] = id;
            LOG_INFO("k="<<k<<" v="<<id<<" symbol="<<string(int64tos(k)));
        }
    }
    return 0;
}

_type_timeserise* TimeSerise::GetSeriesByTradingday(char* symbol, time_t tradingday)
{
    if(!m_Holidays->IsOpenDay(tradingday)) return NULL;
    uint32_t id = GetTimeTablesId(symbol);
    if(id == 0) return NULL;
    map<uint64_t, _type_timeserise >::iterator itrt =  m_SeriseTables.find(id+tradingday);
    if(itrt != m_SeriseTables.end())
    {
        return &itrt->second; 
    }

    _type_timetables* tb = GetTimeTables(symbol);
    if(tb == NULL) return NULL;
    
    _type_timeserise  as;
    _type_timetables::iterator itr = tb->begin(); 
    for(; itr!=tb->end(); itr++)
    {
        if(itr->t != night)
        {
            if(itr->t == stop_kline_only)
                ProductSerise(as, tradingday, itr->s, itr->e, 0);
            else
                ProductSerise(as, tradingday, itr->s, itr->e);
            continue;  
        }
        time_t pretradingday = m_Holidays->GetPrevTradingDay(tradingday);
        if(!m_Holidays->IsOpenNight(pretradingday)) continue;
        if(itr->e > itr->s)
        {
           ProductSerise(as, pretradingday, itr->s, itr->e);
           continue;
        }
        ProductSerise(as, pretradingday, itr->s, 23*3600 + 59*60 + 59);
        ProductSerise(as, pretradingday+86400, 0, itr->e);
    }
    sort(as.begin(), as.end());
    m_SeriseTables[id+tradingday] = as;
    return &m_SeriseTables[id+tradingday];
}

bool  compare_klinekey (const klinekey&  t, const klinekey& k)
{
    return t.key < k.key;
}

//二分搜索
int32_t TimeSerise::GetPosition(_type_timeserise& data, time_t t)
{
    klinekey k;
    k.key =t;
    //小于第一个返回位置0   大于最后一个返回最后一个位置+1
    typeof(data.begin()) itr = lower_bound(data.begin(), data.end(), k, compare_klinekey);
    return itr - data.begin();  //返回0就是当天第一个 
}

//晚上来的数据 和 白天来的数据
time_t TimeSerise::GetSeriseKey(char* symbol, uint32_t period, time_t tradingday, time_t t, time_t& beforet)
{
    if(!m_Holidays->IsOpenDay(tradingday)) return 0;
    _type_timeserise* pdata = GetSeriesByTradingday(symbol, tradingday);
    if(pdata == NULL)
    {
        //ERROR
        return 0;
    }
    _type_timeserise& data = *pdata;

    uint32_t pos = GetPosition(data, t);
    //15点应该返回整点
    //返回最后一个最大的数据
    if(pos == data.size())
    {
        beforet = -1;
        //cout << "symbol=" << symbol << " period=" << period << " tradingday=" << tradingday  << " k=" << data[data.size()-1].key << " k2=" << data[data.size()-1].key +  period*60 << endl;
        //最后一个推送 14:59:59---15:00:00  在15:00:04的时候推送
        if(t >= data[pos-1].key + 60) //t>15:00:00 或者15:15:15的时候推送
        {
            uint32_t p = pos - pos % period;
            if(pos % period == 0) p = pos - period;
            //cout << "pos=" << pos << " pos%=" << pos % period << " key=" << data[p].key << " p=" << p << " size=" << data.size() << endl;
            beforet = data[p].key; 
            return  -1; //推送最后一个
        }
        return -1; 
    }
    //周期起出
    int i = (pos / period) * period;
    if(i == 0) return data[0].key;

    int b = i - period;

    beforet = data[b].key;
    
    //b ---> i-1
    if(data[b].type == 0 && data[i-1].type == 0) beforet = 0;   //不用推送的
   
    return data[i].key; 
}

_type_timeserise& TimeSerise::ProductSerise(_type_timeserise& as, time_t actionday, uint32_t s, uint32_t e, uint8_t t)
{
    time_t ts = actionday +  s;
    time_t te = actionday +  e;
    //printf("actionday=%u %s, s=%u %s, e=%u %s\n", actionday, stamptostr(actionday).c_str(), ts, stamptostr(ts).c_str(), te, stamptostr(te).c_str());
    while(ts < te)
    {
        klinekey k;
        k.key = ts;
        k.type = t;
        as.push_back(k);
        ts = ts+60;
    }
    return as;
}

_type_timeserise TimeSerise::GetPeriodSeriesByTradingday(char* symbol, uint32_t period, time_t tradingday)
{
    time_t ymd = get_ymd(tradingday);
    _type_timeserise ts;
    _type_timeserise* pdata = GetSeriesByTradingday(symbol, ymd);
    if(pdata == NULL) 
    {
        //ERROR
        return ts; 
    }
    _type_timeserise& data = *pdata;
    uint32_t i = 0;
    while(i < data.size())
    {
        ts.push_back(data[i]);
        i += period;
    }
    return ts;
}

uint32_t TimeSerise::GetTimeTablesId(char* symbol)
{
    uint64_t k =  stoint64(symbol);
    map<uint64_t, uint32_t>::iterator itr = m_MapTimeTablesId.find(k);
    if(itr == m_MapTimeTablesId.end())
    {
        return 0;
    }
    return itr->second;
}

_type_timetables* TimeSerise::GetTimeTables(char* symbol)
{
    uint32_t id = GetTimeTablesId(symbol);
    if(id == 0) return NULL;

    map<uint32_t, _type_timetables>::iterator itr = m_MapTimeTables.find(id);
    if(itr == m_MapTimeTables.end())
    {
        return NULL;
    }
    return &itr->second;
}


matkettype TimeSerise::stoenum(string s)
{
    if(s == "morning") return morning;
    if(s == "afternoon") return afternoon;
    if(s == "night") return night;
    if(s == "stop_kline_only") return stop_kline_only;
    return none;
}


bool TimeSerise::isTrading(char* product, time_t t, uint32_t close_delay)
{

    _type_timetables* p = GetTimeTables(product);
    if(p == NULL) 
    {
        LOG_ERROR("get tradetime table fail. product=" << product);
        return true;
    }
    
    uint32_t s = st2seconds(t);
    _type_timetables::iterator itr = (p->begin());
    for(; itr!=p->end(); itr++)
    {
        if(itr->t == night && itr->s > itr->e)
        {
            if(s >= itr->s || s < itr->e) return true;
        }

        //LOG_INFO("product=" << product << " s =" << itr->s << " e=" << itr->e << " s="<<s << " t=" << t);
       
        //下午收盘延迟5s收盘
        uint32_t e=itr->e;
        if(itr->t == afternoon) e = itr->e + close_delay;
        
        if(s >= itr->s && s < e)
        {
            if(itr->t == stop_kline_only) return false;
            return true;
        }
    }
    LOG_INFO(product << "  " << t);
    return false;
}


//消除每次开盘 收盘前的交易所行情时间不对的问题
uint32_t TimeSerise::modify_open_close_timestamp(char* product, uint32_t timestamp)
{
    _type_timetables* p = GetTimeTables(product);
    if(p == NULL) 
    {
        LOG_ERROR("get tradetime table fail. product=" << product);
        return timestamp;
    }
    
    uint32_t s = st2seconds(timestamp);
    _type_timetables::iterator itr = (p->begin());
    for(; itr!=p->end(); itr++)
    {
        if(itr->t == stop_kline_only) continue;
        uint32_t t = 0;
        if(itr->s-s <= 60 && itr->s-s > 0) t = timestamp + 60; //规整到开盘时间
        if(s-itr->e <= 60 && s-itr->e >= 0) t =  timestamp - 60; //规整到收盘时间
        if(t > 0)
        {
            LOG_INFO(product << " timestamp modify: " << timestamp << "-------->" << t);
            return t;
        }
    }
    return timestamp;
}





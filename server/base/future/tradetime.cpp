/*************************************************************************
    > File Name: tradetime.cpp
    > Created Time: Fri 18 Nov 2016 04:13:22 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#include "tradetime.h"

TradeTime g_tradetime;



int TradeTime::Init(CMongodb& db)	
{
	db.ChangeCollection("TRADETIME");
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
		string market	= value["market"].asString();
		string statusm	= value["statusm"].asString();
		uint32_t status	= value["status"].asUInt();
		string st		= value["starttime"].asString(); //120000
		string et		= value["endtime"].asString();   //120000

		TimeSection ts;
		ts.market    = market;
		ts.status    = status;
		ts.statusm	 = statusm;
		ts.starttime = t2seconds(st, "%H%M%S");
		ts.endtime   = t2seconds(et, "%H%M%S");
		if(ts.starttime > ts.endtime) ts.endtime = 86400;

		LOG_DEBUG("market=" << market << " st=" << st << " et=" << et << " status=" << status << " statusm:" << statusm << " ts.starttime=" << ts.starttime << " ts.endtime=" << ts.endtime);
		Add(ts);

		m_now_status[market] = GetMarketStatus(time(NULL), market);
	}
	Print();

	if(m_tradetimes.size() < 1)
	{
		printf("error m_tradetimes.size()=%u\n", (uint32_t)m_tradetimes.size());
		exit(-1);
	}

	return 0;
}

// 获取市场状态
uint32_t TradeTime::GetMarketStatusP(uint32_t second, string market)
{
	typeof(m_tradetimes.begin()) itr = m_tradetimes.begin();
	for(; itr!=m_tradetimes.end(); itr++)
	{
		/*LOG_DEBUG ("market=" << market
				<< " second=" << second
				<< " itr->market=" << itr->market
				<< " itr->starttime=" << itr->starttime
				<< " itr->endtime=" << itr->endtime
				<< " itr->status=" << itr->status);*/
		if(itr->market == market && itr->starttime <= second && itr->endtime > second)
		{
			return itr->status;
		}
	}
	return 10; //收盘时间
}

uint32_t TradeTime::GetMarketStatus(uint32_t t, string market)
{
	uint32_t second = st2seconds(t);     
	uint32_t status =  GetMarketStatusP(second, market);

	static uint16_t cou = 0; //65535
	if(cou++ % 30000 ==  0) LOG_DEBUG("status="<<status<<" time="<<second<<" t="<<t<<" market="<<market << " count="<<cou);

	return status;
}

uint32_t TradeTime::GetMarketStatus(string t, string market, string format)
{
	uint32_t second = t2seconds(t, format);
	
	return GetMarketStatusP(second, market);
}

bool TradeTime::IsStopRcvData(uint32_t t, string market)
{
	int status = GetMarketStatus(t, market);
	return (status==10 || status==3);
}

bool TradeTime::IsStopRcvData(uint32_t status)
{
	return (status==10 || status==3);
}

//事件切换函数
bool TradeTime::OnChangeMarketStatus(uint32_t t, string market)
{
	if(m_now_status.find(market) == m_now_status.end())
	{
		LOG_ERROR("market=" << market << " error.");
		return false;
	}

	uint32_t status = GetMarketStatus(t, market);
	uint32_t now_status = m_now_status[market];
	if(status != now_status)
	{
		LOG_INFO("makrket=" << market << " time=" << t << " old status=" << now_status << " new status=" << status);
		m_now_status[market] = status;
		return true;
	}
	return false;
}

bool TradeTime::OnChangeMarketStatus(string t, string market, string format)
{
	string s = string("20160101 ") + t;
	string f = string("%Y%m%d ") + format;

	uint32_t tmp = strtostamp(s.c_str(), f.c_str());

	return OnChangeMarketStatus(tmp, market);
}

bool TradeTime::OnChangeMarketStatus(string market, uint32_t status)
{
	uint32_t now_status = m_now_status[market];
	if(status != now_status)
	{
		LOG_INFO("makrket=" << market <<  " old status=" << now_status << " new status=" << status);
		m_now_status[market] = status;
		return true;
	}
	return false;
}

//收盘状态判断
bool TradeTime::IsCloseMarket(uint32_t t, string market)
{
	return GetMarketStatus(t, market) == 10;
}
//收盘状态判断
bool TradeTime::IsCloseMarket(uint32_t status)
{
	return status == 10;
}

bool TradeTime::IsBeforeOpenMarket(uint32_t status)
{
	return status == 1;
}

//这几个时间点需要刷内存数据到磁盘
bool  TradeTime::IsStopTrade(uint32_t t, string market)
{
	int status = GetMarketStatus(t, market);
	return status==7 || status==9 || status==3;
}

//这几个时间点需要刷内存数据到磁盘
bool  TradeTime::IsStopTrade(uint32_t status)
{
	return status==7 || status==9 || status==3;
}

//这几个时间点需要刷内存数据到磁盘
bool  TradeTime::IsSettlement(uint32_t status)
{
	return status==9;
}


void TradeTime::Add(TimeSection ts)
{
	
	typeof(m_tradetimes.begin()) itr = m_tradetimes.begin();
	for(; itr!=m_tradetimes.end(); itr++)
	{
		if(itr->market == ts.market && itr->starttime == ts.starttime)
		{
			itr->endtime = ts.endtime;
			return;
		}
	}
	m_tradetimes.push_back(ts);
}

void TradeTime::Print()
{
	typeof(m_tradetimes.begin()) itr = m_tradetimes.begin();
	for(; itr!=m_tradetimes.end(); itr++)
	{
		LOG_INFO("market=" << itr->market << " starttime=" << itr->starttime << ":" <<  second2ts(itr->starttime) << " endtime=" << itr->endtime << ":" << second2ts(itr->endtime) << " status=" << itr->status << " statusm=" << itr->statusm);
	}
}






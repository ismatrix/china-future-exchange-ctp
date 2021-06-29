/*************************************************************************
    > File Name: holidays.cpp
    > Created Time: Thu 02 Jun 2016 10:50:23 AM CST
    > Author:
    > description:
 ************************************************************************/

#include "holidays.h"


Holidays g_holidays;


int Holidays::Init(CMongodb& db)
{
	db.ChangeCollection("HOLIDAYS");
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
		string market = value["market"].asString();
		string st = value["timestamp"].asString(); //20160603

		time_t timestamp = strtostamp(st.c_str(), "%Y%m%d");

		// LOG_INFO("timestamp=" << timestamp << " st:" << st);
		Add(timestamp, market);
	}

	// Print();

	if(m_holidays.size() < 2)
	{
		printf("error m_holidays.size()=%u\n", (uint32_t)m_holidays.size());
		exit(-1);
	}

	return 0;
}

void Holidays::Add(time_t timestamp, string market, time_t next_open_time)
{
	Holiday h;
	h.market = market;
	h.timestamp = timestamp;
	h.next_open_time = next_open_time;

	list<Holiday>::iterator itr = m_holidays.begin();
	for(; itr!=m_holidays.end(); itr++)
	{
		if(itr->market == market && itr->timestamp == timestamp)
		{
			itr->next_open_time = next_open_time;
			return;
		}
	}
	m_holidays.push_back(h);
}

void Holidays::Print()
{
	LOG_INFO("Print holidays size:" << m_holidays.size());
	typeof(m_holidays.begin()) itr = m_holidays.begin();
	for(; itr!=m_holidays.end(); itr++)
	{
		LOG_INFO("market:" << (*itr).market << "  time:" << stamptostr((*itr).timestamp) << " next_open_time:" << stamptostr((*itr).next_open_time));
	}
}


//判断今天是不是节假日
bool Holidays::IsHoladay(time_t timestamp, string market)
{
	timestamp = get_ymd(timestamp);
	typeof(m_holidays.begin()) itr = m_holidays.begin();
	for(; itr!=m_holidays.end(); itr++)
	{
		if(itr->market == market && itr->timestamp == timestamp)
		{
			return true;
		}
	}
	return false;
}

bool Holidays::IsWeekend(time_t timestamp)
{
	int w = get_day_of_week(timestamp);//0---6
	if(w == 6 || w == 0)
	{
		return true;
	}
	return false;
}

//判断今天要不要开日市
bool Holidays::IsOpenDay(time_t timestamp, string market)
{
	if(IsHoladay(timestamp, market) || IsWeekend(timestamp))
	{
		return false;
	}
	return true;
}

//判断今天要不要开夜盘
bool Holidays::IsOpenNight(time_t timestamp, string market)
{
	//日市不开肯定没夜盘
	if(!IsOpenDay(timestamp, market)) return false;

	//明天是节假日 今天不开夜市
	time_t tomorrow = timestamp + 86400;

	if(IsHoladay(tomorrow, market)) return false;

	return true;
}

//获取上个交易日
time_t Holidays::GetPrevTradingDay(time_t timestamp, string market)
{
	int i = 1;
	bool o = IsOpenDay(timestamp - 86400*i, market);
	while(!o)
	{
		i++;
		o = IsOpenDay(timestamp - 86400*i, market);
	}
	return timestamp - 86400*i;
}


//获取下个交易日
time_t Holidays::GetNextTradingDay(time_t timestamp, string market)
{
	int i = 1;
	bool o = IsOpenDay(timestamp + 86400*i, market);
	while(!o)
	{
		i++;
		o = IsOpenDay(timestamp + 86400*i, market);
	}
	return timestamp + 86400*i;
}



//返回20160602
time_t Holidays::GetTradingDay(time_t timestamp, string market)
{
	//今天交易  有夜市   明天交易 18点分割                     -------- 周内

	//今天交易  有夜市   明天不交易 夜市就是下一个交易日日期   -------- 明天是周末

	//今天交易  无夜市   就直接是今天                          -------- 明天是节假日

	//今天不交易  上个交易日无夜市就是上个交易日               -------- 节假日

	//今天不交易  上个交易日有夜市   就是下个交易日的日期      -------- 周末

	time_t ymd  = get_ymd(timestamp); //圆整日期

	bool open		= IsOpenDay(timestamp, market);
	bool opennight  = IsOpenNight(timestamp, market);
	bool tomorrow   = IsOpenDay(timestamp+86400, market);

	struct tm ptm;
	localtime_r(&timestamp, &ptm);

	char buf[32] = {0};
	strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S", &ptm);
	LOG_DEBUG("open="<<open<<" opennight="<<opennight<<" tomorrow="<<tomorrow<<" buf="<<buf<<" ptm=" << mktime(&ptm) <<" timestamp="<<timestamp<<" ymd="<<ymd);
	if(open)
	{
		if(!opennight) return ymd;

		if(!tomorrow  && ptm.tm_hour>=21)  return GetNextTradingDay(ymd, market);

		if(ptm.tm_hour>=21) return ymd+86400;

		return ymd;
	}
	else
	{
		time_t prev = GetPrevTradingDay(ymd, market);
		bool prevnight = IsOpenNight(prev, market);

		if(prevnight) return GetNextTradingDay(ymd, market);

		return prev;
	}

}

bool Holidays::IsTrading(time_t timestamp, string market)
{
	//判断今天 昨天开没开夜盘  判断今天开不开日盘
	struct tm ptm;
	localtime_r(&timestamp, &ptm);
	if(ptm.tm_hour < 5)
	{
		if(IsOpenNight(timestamp - 3600 *6, market))
			return true;
		else
			return false;
	}
	else if(ptm.tm_hour < 16)
	{
		if(IsOpenDay(timestamp, market))
			return true;
		else
			return false;
	}
	else
	{
		if(IsOpenNight(timestamp, market))
			return true;
		else
			return false;
	}
	return true;
}

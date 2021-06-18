/*************************************************************************
  > File Name: timeseries.h
  > Created Time: jeu. 09 f茅vr. 2017 17:22:57 CST
  > Author: 
  > description: 
 ************************************************************************/
#ifndef __TIMESERISE_H__
#define __TIMESERISE_H__

#include <iostream>
#include "holidays.h"
using namespace std;


enum matkettype{none=0, morning, afternoon, night, stop_kline_only};

struct timetable
{
    uint32_t s;  //start time
    uint32_t e;  //end time
    matkettype t;  //type
};
typedef vector<timetable> _type_timetables;

struct klinekey
{
    time_t key;
    uint8_t type;    //1 表示生成kline   0 表示计入间隔但是不生成kline
};
typedef vector<klinekey> _type_timeserise;


inline  bool operator < (const time_t& t, const klinekey& k)
{
    return t < k.key;
}

inline  bool operator > (const time_t& t, const klinekey& k)
{
    return t > k.key;
}
inline  bool operator < (const klinekey& t, const klinekey& k)
{
    return t.key < k.key;
}

inline  bool operator > (const klinekey& t, const klinekey& k)
{
    return t.key > k.key;
}

class TimeSerise
{
    public :
    TimeSerise(Holidays* h);
    ~TimeSerise();
    int Init(CMongodb& db);
    //生成合约tradingday对应的1MK的key表
    _type_timeserise* GetSeriesByTradingday(char* symbol, time_t tradingday);

    //获取合约对应周期的key
    time_t GetSeriseKey(char* symbol, uint32_t period, time_t tradingday, time_t t, time_t& beforet); 

    //获取合约对应周期的key表
    _type_timeserise GetPeriodSeriesByTradingday(char* symbol, uint32_t period, time_t tradingday);

    //判断产品是不是交易
    bool isTrading(char* product, time_t t = time(NULL), uint32_t close_delay=5);

    uint32_t modify_open_close_timestamp(char* product, uint32_t timestamp);

    void clear(){ m_SeriseTables.clear(); };
   
    
    private:
    int32_t GetPosition(_type_timeserise& data, time_t t);
    uint32_t GetTimeTablesId(char* symbol);
    _type_timetables* GetTimeTables(char* symbol);
    _type_timeserise& ProductSerise(_type_timeserise& as, time_t actionday, uint32_t s, uint32_t e, uint8_t t=1);
    
    matkettype stoenum(string s);

    private:
    Holidays* m_Holidays;

    //区间表和合约id对应
    map<uint32_t, _type_timetables> m_MapTimeTables;

    //合约对应的timetables id
    map<uint64_t, uint32_t> m_MapTimeTablesId;       //key=reinterpret_cast<uint64_t>("IF")

    //序列表
    map<uint64_t, _type_timeserise > m_SeriseTables;//timetables_id=1+tradingday
};

extern TimeSerise g_timeserise;


#endif 





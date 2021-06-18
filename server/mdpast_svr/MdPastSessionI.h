/////////////////////////////////////////////////////////////////////////
///@system 交易系统
///@company 慧网基金
///@file MdSessionI.h
///@brief 定义了客户端接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef __SESSIONI_H__
#define __SESSIONI_H__

#include <Ice/Ice.h>
#include <map>
#include <string>
#include <stdint.h>
#include <vector>
#include "RollLog.h"
#include "future/file.h"
#include "util.h"
#include "ice/Common.h"

#include "future/helper.h"
#include "future/timeseries.h"
//连接管理的
#include "MdPastSession.h"


using namespace CM;
using namespace MdPast;
using namespace std;


//连接管理类
class MdSessionI : public  MdPast::MdSession, public IceUtil::Mutex
{
public:

	virtual ::std::string queryData(const Ice::Identity& ident, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current());
    virtual ::std::string queryInstruments(const ::MdPast::StringList&, const ::MdPast::StringList&, const ::MdPast::StringList&, const ::MdPast::IntList&, const ::MdPast::StringList&, const ::MdPast::IntList&, const ::Ice::Current& = ::Ice::Current());

private:
	int request_ticker(MdSessionCallBackPrx& client, const string& InstrumentID, const string& tradingday);
	int request_bar(MdSessionCallBackPrx& client, list<DayBar>&  dbars, const string& InstrumentID, time_t st, uint32_t period);
	int request_daybar(MdSessionCallBackPrx& client, const string& InstrumentID, const string& start, const string& end);


    int  get_daybar(list<DayBar>& dbars , const string& InstrumentID, const string& start, const string& end);
    
    template<typename T>
    inline int combine_query_condition(string k, Json::Value& r, const vector<T>& v)
    {
        if(v.size() <= 0) return 0;
        Json::Value a, in;
        for(int i=0; i<v.size(); i++) a.append(v[i]);
        in["$in"] = a;
        r[k] = in;
        return 0;
    }

};

typedef IceUtil::Handle<MdSessionI> MdSessionIPtr;
/*
1. 获取日k
2. 处理一天的获取上一根的收盘价格
3. 分周期获取的

特殊处理一天的开始
*/
#endif




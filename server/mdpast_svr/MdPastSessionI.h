/////////////////////////////////////////////////////////////////////////
///@system ����ϵͳ
///@company ��������
///@file MdSessionI.h
///@brief �����˿ͻ��˽ӿ�
///@history 
///20160326	dreamyzhang		�������ļ�
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
//���ӹ����
#include "MdPastSession.h"


using namespace CM;
using namespace MdPast;
using namespace std;


//���ӹ�����
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
1. ��ȡ��k
2. ����һ��Ļ�ȡ��һ�������̼۸�
3. �����ڻ�ȡ��

���⴦��һ��Ŀ�ʼ
*/
#endif




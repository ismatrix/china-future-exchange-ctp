
/*************************************************************************
    > File Name: mdtools.cpp
    > Created Time: Thu 26 May 2016 03:29:00 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <list>
#include <math.h>
#include <stdint.h>

#include "ice/Common.h"
#include "RollLog.h"
#include "future/file.h"
#include "future/hq.h"
#include "mongoc_json.h"
#include "ut_str.h"
#include "conf.h"
#include "future/Init.h"
#include "future/ProductIndex.h"
#include "future/tradetime.h"
#include "future/holidays.h"
#include "future/timeseries.h"


#include "encrypt/encrypt_password.h"

using namespace std;


#define  __CONFIG__  "mdtools_config.ini"

//读取二进制转化文本
//读取文本转化二进制
// tradingday	instrument
typedef void (*HandleFun)(int argc, char* argv[]);

string tradingday;
string instrument;
string data_type;
string inprefix;
string infile;
string outfile;
string outprefix;


void usage()
{
	//包含了修改txt dat文件
	puts("-intxt (M1|T) instrument tradingday inprefix a.txt -outtxt prefix	//-intxt T IF1606 20160526 ./ IF1606.txt -outtxt ./\n");
	puts("-intxt (M1|T) instrument tradingday inprefix a.txt -outdat prefix	//-intxt T IF1606 20160526 ./ IF1606.txt -outdat ./\n");
	puts("-transtxt (M1|T) file.txt                                         //-transtxt T file.dat\n");
	puts("-transdat (M1|T) file.dat                                         //-transdat T file.txt\n");
	
	//根据ticker生成M1 ticker
	puts("-rundata infile   outprefix                                //-rundata  ./a.txt /mnt/hqdata/\n");
	puts("-tradetime_status HHMMSS  market                          //-tradetime  160000 SHFE.   check market status on tradetime.");
	puts("-tradetime_status_change  HHMMSS HHMMSS market            //-tradetime  160000   210000 SHFE .check change market status.");

	puts("-pwd_enc	pwd                                             //-pwd_enc 12345       encrypt the password of account");
	puts("-pwd_dec	pwd                                             //-pwd_dec sadsfdgfgfg decrypt the string to correct password");
	puts("-ctd	ymdhms                                                         //-ctd '2016-01-01 00:00:00'  check trading day");
	puts("-copen now												//-copen 0|20160101 check if trading  today");
	puts("-istrading product now									//-istrading ag 0|2016-01-01 00:00:00 check if trading now");
	puts("-pkline  instrumentid tradingday							//-pkline ag1612 20161025   product kline by ticker, default ./hqdata");

	//指数生成相关的
	puts("-index -day-t     tradingday                              //-index -day-t 20160626");
	puts("-index -day-p	    product                                 //-index -day-p IF");
	puts("-index -day	    all										//-index -day  produce all day bar");
	
	//计算分k的同时 也要计算最后的日k 并入库
	puts("-index -minute-t  tradingday                              //-index -minute-t 20160626");
	puts("-index -minute-p  product                                 //-index -minute-p  IF");
	puts("-index -minute	all		                                //-index -minute produce all minute bar");
	
	puts("-index -query-p											//-index -query-p");

	//计算合约表中主连的排名 
	puts("-index -master-t  20160616								//-master tradingday 20160616");
	puts("-index -master-p  IF										//-master tradingday IF");
	puts("-index -master    all										//-master all");
	
	puts("-instrument_rank tradingday								//-instrument_rank 20160616");



}

int flush_data()
{
	typeof(g_mapInstrmentData.begin()) itr = g_mapInstrmentData.begin();
	for(; itr!=g_mapInstrmentData.end(); itr++)
	{
		InstrumentData* p = itr->second;
		list<Bar>* pk = GetKlineList<Bar>(p,  0, K1MINUTE);
		if(pk != NULL &&  pk->size() > 0)
			g_file->WriteTxt(*pk, p->snap.TradingDay, p->snap.InstrumentID);
				
		if(p->TickerItems.size() > 0)
			g_file->WriteTxt(p->TickerItems, p->snap.TradingDay, p->snap.InstrumentID);
	}
	return 0;
}



int Write_DayBar(DayBar& daybar, string instrument,string tradingday)
{
	CMongodb& db = CMongodb::Instance();
	db.ChangeCollection("DAYBAR");
	
	//json 不支持uint64  转为string
	Json::Value q, s;
	s["instrument"]			=      instrument;
	s["tradingday"]			=		tradingday;
	s["timestamp"]			=		toa(daybar.Timestamp);        
	s["high"]				=		daybar.High;               //最高价
	s["low"]				=		daybar.Low;                //最低价
	s["open"]				=		daybar.Open;               //开盘价
	s["close"]				=		daybar.Close;              //收盘价
	s["average"]			=		daybar.Turnover/daybar.Volume;            //日均价
	s["volume"]				=		daybar.Volume;             //成交量
	s["turnover"]			=		daybar.Turnover;           //成交额
	s["settlement"]			=		daybar.Settlement;         //结算价
	s["presettlement"]		=		daybar.PreSettlement;      //上次结算价
	s["preclose"]			=		daybar.PreClose;           //昨收盘
	s["preoopeninterest"]	=		daybar.PreoOpenInterest;   //昨持仓量
	s["openinterest"]		=		daybar.OpenInterest;   //昨持仓量
	s["price"]				=		daybar.Price;
	s["upperlimit"]			=		daybar.UpperLimit;
	s["lowerlimit"]			=		daybar.LowerLimit;
	s["updatedate"]         =		stamp_to_isodate(time(NULL));
	
	q["instrument"] = instrument;
	q["tradingday"] = tradingday;
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(s);
	
	int ret = db.Update(ss, q.toStyledString(), true);
	if(ret < 0)
	{
		LOG_ERROR("db fail DayBar:" << ss);
	}
	
	return 0;
}

void preprocess(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
    if(pDepthMarketData->LastPrice > 0.001) pDepthMarketData->ClosePrice = pDepthMarketData->LastPrice;
    if(pDepthMarketData->OpenPrice < 0.001)
    {
        double p = 0;
        if(pDepthMarketData->PreClosePrice > 0.001) 
            p = pDepthMarketData->PreClosePrice;
        else if(pDepthMarketData->PreSettlementPrice > 0.001) 
            p = pDepthMarketData->PreSettlementPrice;
        else 
            p = pDepthMarketData->LastPrice;
        pDepthMarketData->OpenPrice    = p;
        pDepthMarketData->HighestPrice = p;
        pDepthMarketData->LowestPrice  = p;
    }
}

//kline主逻辑 需要累积日k
int RunHq(const char* pin, uint32_t len, int wnum)
{
    map<string, InstrumentData*>& mapInstrmentData = g_mapInstrmentData;
    if(len < sizeof(CThostFtdcDepthMarketDataField))
    {
        LOG_ERROR("len=" << len << " <  sizeof(CThostFtdcDepthMarketDataField)=" << sizeof(CThostFtdcDepthMarketDataField));
        return -1;
    }

    CThostFtdcDepthMarketDataField* pnew = (CThostFtdcDepthMarketDataField*)pin;
    if(strlen(pnew->InstrumentID) == 0)
    {
        LOG_ERROR("InstrumentID:" << pnew->InstrumentID);
        return 0;
    }

    preprocess(pnew);

    InstrumentData* prev = NULL;	//上一个快照
    //对比mapInstrmentData snap和pnew
    map<string, InstrumentData*>::iterator itr =  mapInstrmentData.find(pnew->InstrumentID);
    if(itr == mapInstrmentData.end() || strlen(itr->second->snap.InstrumentID) < 1)
    {
        //第一个快照更新
        LOG_INFO("new InstrumentID:" << pnew->InstrumentID);
        prev = GetInstrumentData(pnew->InstrumentID, true);	//创建一个合约位置
        PreProcessData(prev, pnew, true); // TODO 需要检查这里的需要处理不 成交额
        memcpy(&prev->snap, pnew, sizeof(CThostFtdcDepthMarketDataField));
        return 0;
    }
    else
    {
        prev = (itr->second);
        PreProcessData(prev, pnew, true);//TODO 需要检查这里的需要处理不 成交额 成交均价 时间戳
    }

    char timebuf[32] = {0};
    snprintf(timebuf, sizeof(timebuf), "%s %s", pnew->ActionDay, pnew->UpdateTime);
    //struct tm _tm = strtotm(timebuf, "%Y%m%d %H:%M:%S"); //当前时间

    // printf("prev->snap.Volume:%d pnew->Volume:%d\n", prev->snap.Volume, pnew->Volume);
    //计算kline
    if(pnew->Volume > prev->snap.Volume && IsDValid(pnew->LastPrice) && pnew->Turnover > prev->snap.Turnover)
    {
        calculate_Kline(prev, timebuf, pnew->LastPrice, pnew->Volume - prev->snap.Volume, pnew->Turnover-prev->snap.Turnover, pnew->OpenInterest);
    }

    //计算ticker
    if((pnew->Volume > prev->snap.Volume && pnew->Turnover > prev->snap.Turnover) ||  
            (IsDValid(pnew->BidPrice1) & !IsEqDouble(pnew->BidPrice1, prev->snap.BidPrice1)) || 
            pnew->BidVolume1	!= prev->snap.BidVolume1 || 
            (IsDValid(pnew->AskPrice1) && !IsEqDouble(pnew->AskPrice1, prev->snap.AskPrice1))  || 
            pnew->AskVolume1	!= prev->snap.AskVolume1 )
    {
        calculate_Ticker(prev,  &prev->snap, pnew);
    }	

    Update_Snap(&prev->snap, pnew);

    return 0;
}




//读入txt写对应dat中
void Handle_intxt(int argc, char* argv[])
{
    if(argc < 8)
    {
        usage();
        return;
    }
    data_type = argv[2];
    instrument = argv[3];
    tradingday = argv[4];
    inprefix = argv[5];
    infile = argv[6];
    if(string(argv[7]) != "-outtxt")
    {
        usage();
        return;
    }
    outprefix = argv[8];

    if(data_type == "M1")
    {
        return;
    }
    else if(data_type == "T")
    {

    }
    else
    {
        usage();
        return ;
    }

    CFile f(outprefix);
    list< ::CM::Ticker> ticker;
    f.ReadTxt(ticker, tradingday, instrument);

    list< ::CM::Ticker> ticker_add;
    f.set__prefix(inprefix);
    f.ReadDat(ticker_add, tradingday, instrument);

    //顺序插入
    list< ::CM::Ticker>::iterator itr = ticker_add.begin();
    for(; itr!=ticker_add.end(); itr++)
    {
        list< ::CM::Ticker>::iterator it = ticker.begin();
        for(; it!=ticker.end(); it++)
        {
            if(itr->Timestamp > it->Timestamp)
            {
                ticker.insert(it, itr, itr);
                break;
            }
            else if(itr->Timestamp == it->Timestamp)
            {
                (*it) = *itr;
                break;
            }
        }
    }

    f.set__prefix(outprefix);
    f.WriteDat(ticker, tradingday, instrument);
}

void Handle_transtxt(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }
    data_type = argv[2];
    infile = argv[3];
    int pos = infile.find(".txt");
    outfile  = infile.substr(0, pos) + ".dat";

    cout << "infile=" << infile << " outfile:" << outfile << endl;

    CFile file;
    if(data_type == "M1")
        file.TransM1Txt(infile, outfile);
    else if(data_type == "T")
        file.TransTTxt(infile, outfile);
    else
        usage();
}


void Handle_transdat(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }

    data_type = argv[2];
    infile = argv[3];
    int pos = infile.find(".dat");
    outfile  = infile.substr(0, pos) + ".txt";

    cout << "infile=" << infile << " outfile:" << outfile << endl;

    CFile file;
    if(data_type == "M1")
        file.TransM1Dat(infile, outfile);
    else if(data_type == "T")
        file.TransTDat(infile, outfile);
    else
        usage();
}

void Handle_ctd(int argc, char* argv[])
{
    if(argc < 3)
    {
        usage();
        return;
    }

    time_t t = strtostamp(argv[2], "%Y-%m-%d %H:%M:%S");

    //查询节假日表mongo
    g_holidays.Init(CMongodb::Instance());
    uint32_t today_trade = atoll(stamptostr(g_holidays.GetTradingDay(t), "%Y%m%d").c_str());

    cout << "date=" << argv[2] << "  tradingday:" << today_trade << endl;

}

void Handle_copen(int argc, char* argv[])
{
    if(argc < 3)
    {
        usage();
        return;
    }

    time_t t = time(NULL);
    if(atoll(argv[2]) > 2016) t = strtostamp(argv[2], "%Y%m%d");

    //查询节假日表mongo
    g_holidays.Init(CMongodb::Instance());
    int flag  = g_holidays.IsOpenDay(t);

    cout << "date=" << t << "  iftrading:" << flag << endl;

}

void Handle_istrading(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }

    char* product = argv[2];
    
    time_t t = time(NULL);
    if(atoll(argv[3]) > 2016) t = strtostamp(argv[3], "%Y-%m-%d %H:%M:%S");

    int delay = 60;
    if(argc >= 5) delay = atoll(argv[4]);

    //查询节假日表mongo
    //g_tradetime.Init(CMongodb::Instance());
    g_holidays.Init(CMongodb::Instance());
    g_timeserise.Init(CMongodb::Instance());
    int flag1  = g_holidays.IsTrading(t);
    int flag2  = g_timeserise.isTrading(product, t, delay);
    
    //cout << "flag1=" << flag1 << " flag2=" << flag2 << " delay=" << delay << endl;
    if(flag1 && flag2)
    {
        cout << "true" << endl;
        return;
    }
    cout << "false" << endl; 

}


void Handle_rundata(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }

    infile = argv[2];
    outprefix = argv[3];

    int len = infile.size();
    if(len < 13 || infile.compare(len-4, 4, ".csv") != 0)
    {
        cout << "error infile=" << infile << " must end with .csv" << endl;
        return;
    }

    //./IC1505_20150508.csv 
    //分析url获取合约id和交易日 TF1612_20160509.csv
    tradingday = infile.substr(len-12, 8);
    std::size_t pos = infile.rfind('/');
    if(pos != std::string::npos) 
        instrument = infile.substr(pos+1, len-14-pos);

    //1 21
    //cout << pos << " "  << len << endl;
    if(instrument.length() < 2 || atoll(tradingday.c_str()) < 20000101 || atoll(tradingday.c_str()) > 21000101)
    {
        cout << "error infile=" << infile << " instrument=" << instrument << " tradingday:" << tradingday << endl;
        return;
    }

    g_file = new CFile(outprefix.c_str(), ".dat");       

    //日k
    struct tm _tm = strtotm(tradingday.c_str(), "%Y%m%d");
    DayBar daybar;
    memset(&daybar, 0, sizeof(daybar));
    daybar.Timestamp = GetKlineKey(&_tm, KDAY); 

    double Volume = 0;
    double Turnover = 0;
    char buf[1024] = {0};
    ifstream in;
    in.open(infile.c_str(), ios::in);
    CThostFtdcDepthMarketDataField tmp;
    memset(&tmp, 0, sizeof(tmp));
    int flag = 1;
    string foward_time="";
    while(in.getline(buf, 1024))
    {
        vector<string> v;
        ut_split(buf, v, ",");
        if(v.size() <16)
        {
            LOG_ERROR("size buf=" << buf);
        }

        if(flag==1) 
        {	//第一行表头
            flag++;
            continue;
        }
        else if(flag == 2)
        {
            //第二行昨收信息
            flag++;
            daybar.PreClose = atof(v[3].c_str());
            daybar.PreSettlement  = daybar.PreClose;
            daybar.PreoOpenInterest = atoll(v[4].c_str()); 

            //第二行开始的量 额为昨天的持仓量额  需要更新为0	
            v[7] = "";
            v[6] = "";
        }


        string time = v[2].substr(0, 19);
        uint32_t mill = atoll(v[2].substr(20).c_str());
        if(foward_time == v[2])
        {
            mill++; //因为买的那份数据中1秒内两个ticker的时间值是一样的。 毫秒全部都是000
        }
        foward_time = v[2];
        //time_t tm = strtostamp(time.c_str(), "%Y%m%d %H:%M:%S");
        //uint32_t tk = atoll(mill.c_str());

        Volume		= atoll(v[7].c_str());
        Turnover	= atoll(v[6].c_str());

        //非法数据
        if(Volume < 0 || Turnover < 0 || atof(v[3].c_str()) < 0.00001)
        {
            LOG_ERROR("volume buf=" << buf);
            continue;
        }
        //由快照生成相关数据

        memcpy(&tmp.TradingDay, tradingday.c_str(), sizeof(tmp.TradingDay));
        memcpy(&tmp.InstrumentID,instrument.c_str(), sizeof(tmp.InstrumentID));

        //cout << "tradingday=" << tradingday << " instrument=" << instrument << endl;
        //cout << "tmp.TradingDay=" << tmp.TradingDay << " tmp.InstrumentID=" << tmp.InstrumentID << endl;

        //tmp.ExchangeID         =            
        //tmp.ExchangeInstID     =            
        tmp.LastPrice          =  atof(v[3].c_str());          
        //tmp.PreSettlementPrice =            
        //tmp.PreClosePrice      =            
        //tmp.PreOpenInterest    =            
        //tmp.OpenPrice          =            
        //tmp.HighestPrice       =            
        //tmp.LowestPrice        =            
        tmp.Volume             +=  Volume;          
        tmp.Turnover           +=  Turnover;
        tmp.OpenInterest       =  atof(v[4].c_str());           
        //tmp.ClosePrice         =            
        //tmp.SettlementPrice    =            
        //tmp.UpperLimitPrice    =            
        //tmp.LowerLimitPrice    =            
        //tmp.PreDelta           =            
        //tmp.CurrDelta          =            



        //这里根据这三个计算key 2015-04-05 13:23:12
        snprintf((char*)&tmp.ActionDay, sizeof(tmp.ActionDay), "%s%s%s", time.substr(0, 4).c_str(), time.substr(5, 2).c_str(), time.substr(8, 2).c_str());
        snprintf((char*)&tmp.UpdateTime, sizeof(tmp.UpdateTime), "%s", time.substr(11, 8).c_str());
        tmp.UpdateMillisec = mill;

        //cout << "v[2]=" << v[2] << " time=" << time << " mill=" << mill << endl;
        //cout << "tmp.ActionDay=" << tmp.ActionDay << " tmp.UpdateTime=" << tmp.UpdateTime << " tmp.UpdateMillisec=" << tmp.UpdateMillisec << endl;

        tmp.BidPrice1          =  atof(v[12].c_str());  //买1              
        tmp.BidVolume1         =  atoll(v[14].c_str());            
        tmp.AskPrice1          =  atof(v[13].c_str());  //卖1              
        tmp.AskVolume1         =  atoll(v[15].c_str());   

        RunHq((const char*)&tmp, sizeof(CThostFtdcDepthMarketDataField), 1);


        //累积日k 昨收价格不能来给今天开盘 或者作为今天最高最低。。。。
        if(tmp.LastPrice > 0.00001 && flag > 2)
        {
            if(tmp.LastPrice > daybar.High) daybar.High = 	tmp.LastPrice;
            if(daybar.Low < 0.00001 || daybar.Low > tmp.LastPrice)  daybar.Low = tmp.LastPrice;
            if(daybar.Open < 0.00001) daybar.Open = tmp.LastPrice;
            daybar.Close = tmp.LastPrice;
            daybar.Price = tmp.LastPrice;

            daybar.Volume = tmp.Volume;
            daybar.Turnover = tmp.Turnover;
            daybar.Settlement = daybar.Close;
            daybar.OpenInterest = tmp.OpenInterest;
        }
    }

    //写日k
    if(daybar.Open > 0 && daybar.Volume > 0) Write_DayBar(daybar, instrument, tradingday);

    flush_data();
}


//根据ticker生成1分k
void Handle_pkline(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }

    instrument = argv[2];
    tradingday = argv[3];

    //cout << pos << " "  << len << endl;
    if(instrument.length() < 2 || atoll(tradingday.c_str()) < 20000101 || atoll(tradingday.c_str()) > 21000101)
    {
        cout << "error infile=" << infile << " instrument=" << instrument << " tradingday:" << tradingday << endl;
        return;
    }

    InstrumentData* prev = NULL;
    map<string, InstrumentData*>::iterator it =  g_mapInstrmentData.find(instrument);
    if(it == g_mapInstrmentData.end())
    {
        LOG_INFO("new InstrumentID:" << instrument);
        prev = GetInstrumentData(instrument, true);	//创建一个合约位置
    }
    else
    {
        prev = (it->second); //找到合约
    }

    g_file = new CFile("./hqdata", ".dat");       

    //读取所有的ticke
    list< ::CM::Ticker> ticker;
    g_file->ReadTxt(ticker, tradingday, instrument);

    //遍历ticker 生成1分k
    list< ::CM::Ticker>::iterator itr = ticker.begin();
    for(; itr!=ticker.end(); itr++)
    {
        if(itr->Volume > 0)
        {
            //t.Timestamp 计算timebuf
            string buf = stamptostr(itr->Timestamp / 1000, "%Y%m%d %H:%M:%S");
            calculate_Kline(prev, (char*)buf.c_str(),  itr->Price, itr->Volume, itr->Turnover, itr->OpenInterest);
        }
    }

    list<Bar>* pk = GetKlineList<Bar>(prev,  0, K1MINUTE);
    if(pk != NULL &&  pk->size() > 0)
        g_file->WriteTxt(*pk, tradingday, instrument);

    delete g_file;;
}

//加载tradingday所有合约日k 并计算生成指数的日k
void Handle_day_t(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }
    tradingday = argv[3];

    CProductIndex index;
    index.set__callbackd(write_daybar_db);
    index.fill_daybar_tradingday(tradingday);
    index.calculate_daybar();
}

//加载产品对应所有合约日k 并计算生成指数日k
void Handle_day_p(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }
    string productid= argv[3];

    CProductIndex index;
    index.set__callbackd(write_daybar_db);
    index.fill_daybar_product(productid);
    index.calculate_daybar();
}

//加载所有合约日k 并计算生成指数日k
void Handle_day(int argc, char* argv[])
{
    if(argc < 3)
    {
        usage();
        return;
    }
    CProductIndex index;
    index.set__callbackd(write_daybar_db);
    index.fill_daybar();
    index.calculate_daybar();
}

//加载tradingday所有分k 并生成对应指数分k
void Handle_minute_t(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }

    tradingday = argv[3];

    CProductIndex index;
    index.set__callback(write_bar_file);
    index.fill_bar_key(tradingday);
    index.calculate_bar();
}

//加载product对应所有分k 并生成指数对应分k
void Handle_minute_p(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }
    string productid = argv[3];

    CProductIndex index;
    index.set__callback(write_bar_file);
    index.fill_bar_product(productid);
    index.calculate_bar();
}

//加载所有的分k并生成对应指数分k
void Handle_minute(int argc, char* argv[])
{
    if(argc < 3)
    {
        usage();
        return;
    }
    CProductIndex index;
    index.set__callback(write_bar_file);
    index.fill_bar();
    index.calculate_bar();
}


//查询合约对应的productid
void Handle_query_p(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }

    CProductIndex index;
    cout << index.get_productid(argv[3]) << endl;
}

//根据持仓计算product的合约的排名 得出主连 写主连的日k和分k
void Handle_master_p(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }

    string product = argv[3];

    CProductIndex index;
    index.fill_daybar_product(product);

    index.set__callback_masterd(write_daybar_db_master);
    index.set__callback_master(write_bar_file_master);
    index.calculate_master_instrument();
}


//根据持仓计算tradingday的合约的排名 得出主连 写主连的日k和分k
void Handle_master_t(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }

    tradingday = argv[3];

    CProductIndex index;
    index.fill_daybar_tradingday(tradingday);

    index.set__callback_masterd(write_daybar_db_master);
    index.set__callback_master(write_bar_file_master);
    index.calculate_master_instrument();
}


//根据持仓计算所有的合约的排名 得出主连 写主连的日k和分k
void Handle_master(int argc, char* argv[])
{
    if(argc < 3)
    {
        usage();
        return;
    }

    CProductIndex index;
    index.fill_daybar ();

    index.set__callback_masterd(write_daybar_db_master);
    index.set__callback_master(write_bar_file_master);

    index.calculate_master_instrument();
}



void Handle_index(int argc, char* argv[])
{
    if(argc < 3) 
    {
        usage();
        return;
    }
    map<string, HandleFun>  handle;

    handle["-master-p"]		= Handle_master_p;
    handle["-master-t"]		= Handle_master_t;
    handle["-master"]		= Handle_master;
    handle["-day"]			= Handle_day;
    handle["-day-t"]		= Handle_day_t;
    handle["-day-p"]		= Handle_day_p;
    handle["-minute-t"]		= Handle_minute_t;
    handle["-minute-p"]		= Handle_minute_p;
    handle["-minute"]		= Handle_minute;
    handle["-query-p"]		= Handle_query_p;;

    map<string, HandleFun>::iterator itr = handle.find(argv[2]);
    if(itr != handle.end())
    {
        (itr->second)(argc, argv);
    }
}

//更新合约排名
void Handle_instrument_rank(int argc, char* argv[])
{
    if(argc < 3)
    {
        usage();
        return;
    }

    time_t t = strtostamp(argv[2], "%Y%m%d");

    g_holidays.Init(CMongodb::Instance());
    string today_trade = stamptostr(g_holidays.GetTradingDay(t), "%Y%m%d");


    cout << "today_trade=" << today_trade << endl;


    CProductIndex index;
    
    index.fill_daybar_tradingday(today_trade);

    index.set__callback_instrument_rank(update_instrument_rank);

    index.calculate_master_instrument();
}

//-tradetime 120000 SHFE
void Handle_tradetime_status(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }

    g_tradetime.Init(CMongodb::Instance());

    uint32_t status = g_tradetime.GetMarketStatus(argv[2], argv[3], "%H%M%S");

    cout << "status = " << status << endl;
}


//-tradetime 120000  200000 SHFE
void Handle_tradetime_status_change(int argc, char* argv[])
{
    if(argc < 4)
    {
        usage();
        return;
    }

    g_tradetime.Init(CMongodb::Instance());

    uint32_t status1 = g_tradetime.GetMarketStatus(argv[2], argv[4], "%H%M%S");
    g_tradetime.OnChangeMarketStatus(argv[2], argv[4], "%H%M%S");

    uint32_t status2 = g_tradetime.GetMarketStatus(argv[3], argv[4], "%H%M%S");
    bool flag1 = g_tradetime.OnChangeMarketStatus(argv[3], argv[4], "%H%M%S");
    bool flag2 = g_tradetime.OnChangeMarketStatus(argv[3], argv[4], "%H%M%S");
    bool flag3 = g_tradetime.OnChangeMarketStatus(argv[3], argv[4], "%H%M%S");


    cout << "status1=" << status1 << " status2=" << status2 << " change1=" << flag1 << " change2=" << flag2 << " change3=" << flag3 << endl;
}


void Handle_pwd_enc(int argc, char* argv[])
{
    if(argc < 3)
    {
        usage();
        return;
    }

    string pwd = argv[2];

    string enc = encrypt_account_password((const uint8_t*)pwd.c_str(), pwd.size());

    cout << "pwd : " << pwd << " encrypt : " << enc << endl;

}



void Handle_pwd_dec(int argc, char* argv[])
{
    if(argc < 3)
    {
        usage();
        return;
    }

    string pwd = argv[2];

    int size = pwd.size();
    string dec = decrypt_account_password((const uint8_t*)pwd.c_str(), size);

    cout << "pwd : " << pwd << " decrypt : " << dec << endl;
}



void Match_Handle(int argc, char* argv[])
{
    map<string, HandleFun>  handle;
    handle["-intxt"] = Handle_intxt;
    handle["-transtxt"] = Handle_transtxt;
    handle["-transdat"] = Handle_transdat;
    handle["-rundata"]  = Handle_rundata;
    handle["-tradetime_status"]  = Handle_tradetime_status;
    handle["-tradetime_status_change"]  = Handle_tradetime_status_change;
    handle["-pwd_enc"]  = Handle_pwd_enc;
    handle["-pwd_dec"]  = Handle_pwd_dec;
    handle["-ctd"]  = Handle_ctd;
    handle["-copen"]  = Handle_copen;
    handle["-istrading"]  = Handle_istrading;
    handle["-pkline"]  = Handle_pkline;
    handle["-index"]  = Handle_index;
    handle["-instrument_rank"]  = Handle_instrument_rank;

    map<string, HandleFun>::iterator itr = handle.find(argv[1]);
    if(itr != handle.end())
    {
        (itr->second)(argc, argv);
    }
}


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        usage();
        return 0;
    }

    InitAll(__CONFIG__);

    Match_Handle(argc, argv);

    return 0;
}

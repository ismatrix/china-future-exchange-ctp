#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "ThostFtdcMdSpiI.h"
#include "md_callback.h"
#include "util.h"
#include "../future/common.h"
using namespace std;

int callback(const char* pin, uint32_t len)
{
    LOG_DEBUG("callback len:" << len << ", " << pin);
    return 0;
}

void LOG_MD_INFO(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	LOG_DEBUG("TradingDay    : " << pDepthMarketData->TradingDay << 
              " ActionDay     : " << pDepthMarketData->ActionDay     <<    
		      " InstrumentID  :" << pDepthMarketData->InstrumentID  <<      
		      " ExchangeID    : " << pDepthMarketData->ExchangeID    <<      
		      " UpdateMillisec: " << pDepthMarketData->UpdateMillisec
        );

}


int  main(int argv, char* argc[])
{
    //set_daemon();

    LOG_INIT(__FILE__, 4, 102400000, 10);


    md_fifo_init(10240000);


    set<string> idlist;
    idlist.insert("ag1709");
    idlist.insert("T1709");
    idlist.insert("rb1709");
    //QueryMongoInstrument(g_conf, idlist);


    CThostFtdcMdApi* pMd = CThostFtdcMdApi::CreateFtdcMdApi();
    //CThostFtdcMdSpiI MdSpi(pMd, "9999", "068074", "123456", idlist, &md_fifo_push);
    CThostFtdcMdSpiI MdSpi(pMd, "4500", "8010800705", "J105828.", idlist, &md_fifo_push);

    pMd->RegisterSpi(&MdSpi);
    pMd->RegisterFront("tcp://180.166.45.116:41213");
    //pMd->RegisterFront("tcp://180.166.125.146:41213");
    //pMd->RegisterFront("tcp://180.168.146.187:10000");
    pMd->Init();

    if(argv > 1)
    {
        fstream in;
        in.open(argc[1], ios::binary|ios::in);
        CThostFtdcDepthMarketDataField tmp;
        while(!in.eof())
        {
            in.read((void*)&tmp, sizeof(tmp));
            LOG_INFO(tmp.InstrumentID << " " << tmp.LastPrice << " " << tmp.ClosePrice << " " << tmp.OpenInterest << " " << tmp.TradingDay << " " << tmp.ActionDay << " " << tmp.UpdateTime);
        }       
        return 0;
    }   
    
	fstream out;
	out.open("./md.dat", ios::binary|ios::out|ios::app);
	while(true)
	{
		CThostFtdcDepthMarketDataField tmp;
		if(md_fifo_pop((char*)&tmp, sizeof(CThostFtdcDepthMarketDataField)) == 0)
		{
            //if(strncmp(tmp.InstrumentID, "ag1709", 6) == 0 || strncmp(tmp.InstrumentID, "T1709", 5) == 0 || strncmp(tmp.InstrumentID, "rb1709", 6) == 0)
			//    out.write((char*)&tmp, sizeof(CThostFtdcDepthMarketDataField));
            LOG_MD_INFO(&tmp);
        }   
		else
		{
			LOG_DEBUG("No data.");
			out.flush();
            sleep(1);
		}   
		
	}
	return 0;
}




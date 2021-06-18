/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
//@file MdSessionI.h
///@description  server对应的头文件  主要是kline存储转化生成的事情
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#include <math.h> 

#include "../ice/Common.h"
#include "hq.h"
#include "timeseries.h"
map<string, InstrumentData*>  g_mapInstrmentData;	

 
InstrumentData* GetInstrumentData(string id,  bool create)
{
	map<string, InstrumentData*>::iterator itr = g_mapInstrmentData.find(id);
	if(itr != g_mapInstrmentData.end())
		return itr->second;
	if(create)
	{
		InstrumentData* p = new InstrumentData();
		p->Init();
		g_mapInstrmentData.insert(std::pair<string,InstrumentData* >(id,p));
		return p;
	}
	return NULL;
}

void  DelInstrumentData(string id)
{
	map<string, InstrumentData*>::iterator itr = g_mapInstrmentData.find(id);
	if(itr != g_mapInstrmentData.end())
		g_mapInstrmentData.erase(itr);
}


Ticker* GetTickerItem(InstrumentData* p, int64_t Timestamp, bool create)
{
	if(create)
	{
		Ticker tk;
		memset(&tk, 0, sizeof(tk));
		tk.Timestamp = Timestamp;
		p->TickerItems.push_back(tk); 
		return &(*(p->TickerItems.rbegin()));
	}
	
	//list里面查找Timestamp
	list<Ticker>::iterator itr  = p->TickerItems.begin();
	for(; itr!=p->TickerItems.end(); itr++)
	{
		if(itr->Timestamp == Timestamp)
			return &(*itr);
	}
	return NULL;
}

void DelTickerItem(InstrumentData* p, int64_t Timestamp)
{
	//list里面查找Timestamp
	list<Ticker>::iterator itr  = p->TickerItems.begin();
	for(; itr!=p->TickerItems.end(); itr++)
	{
		if(itr->Timestamp == Timestamp)
			p->TickerItems.erase(itr);
	}
	LOG_INFO("DelTickerItem Timestamp:" << Timestamp);
}

void InitInstrumentDatas(map<string, InstrumentData*>&  t)
{
	map<string, InstrumentData*>::iterator itr = t.begin();
	for(; itr!=t.end(); itr++)
	{
		memset(&itr->second->snap, 0, sizeof(itr->second->snap)); 	//快照全部清空 用来计算ticker kline的
		/*
		memset(&itr->st, 0 sizeof(itr->st)); 		//全部清空
		TickerItems.clear();		
		kline1_minute.clear();		
		kline3_minute.clear();
		kline5_minute.clear();
		kline10_minute.clear();
		kline15_minute.clear();
		kline30_minute.clear();
		kline60_minute.clear();
		kline_day.clear();			
		*/
	}
}


//生成kline的Timestamp
uint64_t  GetKlineKey(struct tm* pt, KlineType kline_type)
{
	struct tm tt = *pt;
	tt.tm_sec = 0;
	switch(kline_type)
	{
		case K1MINUTE:
			break;
		case K3MINUTE:
			tt.tm_min = (tt.tm_min / 3) * 3;
			break;
		case K5MINUTE:
			tt.tm_min = (tt.tm_min / 5) * 5;
			break;
		case K10MINUTE:
			tt.tm_min = (tt.tm_min / 10) * 10;
			break;
		case K15MINUTE:
			tt.tm_min = (tt.tm_min / 15) * 15;
			break;
		case K30MINUTE:
			tt.tm_min = (tt.tm_min / 30) * 30;
			break;
		case K60MINUTE:
			tt.tm_min		= 0;
			break;
		case KDAY:
			tt.tm_min		= 0;
			tt.tm_hour		= 0;
			break;
		case KMONTH:
			tt.tm_min		= 0;
			tt.tm_hour		= 0;
			tt.tm_mday	= 0;
			break;
		default:
			break;
	}
	return  mktime(&tt) * 1000;
}



KlineType  GetNextKlineType(KlineType ktype)
{
	if(ktype == K1MINUTE) return KNONE; 
	return (KlineType)((int)ktype+1);
}
 

//price	=	当前价
//volum =	已经减出来的
//turnover=	减出来的
//日k 月k处理呢
int  calculate_Kline(InstrumentData* p, char* _time, double price, double volume, double turnover, double openinterest)
{
	struct tm _tm = strtotm(_time, "%Y%m%d %H:%M:%S");
	KlineType ktype  = K1MINUTE;
	while(ktype!=KNONE)
	{
		uint64_t Timestamp = GetKlineKey(&_tm, ktype);
		if(ktype == K1MINUTE) //期货行情交易所时间不严格 这里规整一下
            Timestamp = g_timeserise.modify_open_close_timestamp(p->instrucment.ProductID, Timestamp/1000) * 1000l;

        Bar* k = GetBar<Bar>(p, Timestamp,  ktype, true); //获取当前 没有就创建

		if(k->Open < 0.0001)
		{
			k->Open = price;
		}
		k->Close = price;
		
		k->Volume 	+= volume;
		k->Turnover += turnover;
		k->OpenInterest = openinterest;		
		if(k->High < price)
		{
			k->High  =  price;
		}
		
		if(k->Low < 0.0001 || k->Low > price)
		{
			k->Low  =  price;
		}
		//获取下一个线形
		ktype = GetNextKlineType(ktype);

	}
	return 0;
}

int  Del_Kline(InstrumentData* p,  uint32_t num)
{
	KlineType ktype  = K1MINUTE;
	while(ktype!=KNONE)
	{
		list<Bar>* pk = GetKlineList<Bar>(p,  0, ktype);
		if(pk == NULL) continue;

		if(pk->size() > num)
		{
			int size = pk->size();
			while(pk->size() > 50 && pk->size() > 0)
			{
				pk->erase(pk->begin());
			}
			LOG_DEBUG("del kline(" << ktype << ":" << size << "--->" << pk->size() << ") InstrumentID=" << p->snap.InstrumentID);
		}

		ktype = GetNextKlineType(ktype);
	}
	return 0;
}

int Del_Ticker(InstrumentData* p, uint32_t num)
{
    
	if(p->TickerItems.size() > num)
	{
		int size = p->TickerItems.size();
		while(p->TickerItems.size() > 50 && p->TickerItems.size() > 0)
		{
			p->TickerItems.erase(p->TickerItems.begin());
		}
		LOG_DEBUG("del ticker "  << size << "--->" << p->TickerItems.size() << " InstrumentID=" << p->snap.InstrumentID);
	}
	
	return 0;
}


int   PreProcessData(InstrumentData* p, CThostFtdcDepthMarketDataField* psrc, bool updateMill)
{
	//经测试在摆盘的时候单个合约的数据一秒内会有3个快照出现 所以毫秒小于前一个的在前一个的基础上再加100ms
	//在后面收到的数据时间值小于前面的时候丢掉这个数据
	//修正毫秒数 连续两个相同的行情修正key增加500ms
	
	/*
	LOG_DEBUG("InstrumentID=" << psrc->InstrumentID
			<< " ExchangeID=" << p->instrucment.ExchangeID
			<< " Price="  << psrc->LastPrice
			<< " Volume="  << psrc->Volume
			<< " UpdateTime="  << psrc->UpdateTime 
			<< " UpdateMillisec=" << psrc->UpdateMillisec);
	*/		
	if(strncmp(psrc->UpdateTime, p->snap.UpdateTime, strlen(psrc->UpdateTime)) == 0)
	{
		if(psrc->UpdateMillisec <= p->snap.UpdateMillisec)
			psrc->UpdateMillisec = p->snap.UpdateMillisec + 1;
	
		/*
		LOG_DEBUG("InstrumentID=" << psrc->InstrumentID
			<< " ExchangeID=" << p->instrucment.ExchangeID
			<< " Price=" <<  p->snap.LastPrice << " ---> " << psrc->LastPrice
			<< " Volume=" << p->snap.Volume << " ---> " << psrc->Volume
			<< " UpdateTime=" << p->snap.UpdateTime << " ----> " << psrc->UpdateTime 
			<< " UpdateMillisec=" << p->snap.UpdateMillisec << " ---> " << psrc->UpdateMillisec);
		*/
	}

	//LOG_DEBUG("InstrumentID=" << psrc->InstrumentID << " VolumeMultiple=" << p->instrucment.VolumeMultiple << " UpdateTime=" << psrc->UpdateTime << " UpdateMillisec=" << psrc->UpdateMillisec);

	if(p->instrucment.VolumeMultiple > 0)
	{
		if(strncmp(p->instrucment.ExchangeID, "CZCE", 4) == 0)
		{
			//修正成交额
			psrc->Turnover *= p->instrucment.VolumeMultiple;
		}
		else
		{
			//修正成交均价
			psrc->AveragePrice /= p->instrucment.VolumeMultiple;
		}
	}
	else
	{
		LOG_ERROR("InstrumentID=" << psrc->InstrumentID << " VolumeMultiple=" << p->instrucment.VolumeMultiple << " UpdateTime=" << psrc->UpdateTime << " UpdateMillisec=" << psrc->UpdateMillisec);
	}

	static uint16_t count = 0;
	if(count++ % 30000 == 0)
		LOG_INFO("InstrumentID=" << psrc->InstrumentID
			<< " ExchangeID=" << p->instrucment.ExchangeID
			<< " Price=" << psrc->LastPrice
			<< " AveragePrice=" << psrc->AveragePrice
			<< " Volume=" << psrc->Volume
			<< " Turnover=" << psrc->Turnover
			<< " VolumeMultiple=" << p->instrucment.VolumeMultiple 
			<< " UpdateTime=" << psrc->UpdateTime 
			<< " UpdateMillisec=" << psrc->UpdateMillisec);
	return 0;
}


int  Snap_Update_Kline(InstrumentData* p, char* _time, double open, double close, double high, double low, double openinterest)
{
	struct tm _tm = strtotm(_time, "%Y%m%d %H:%M:%S");
	
	//获取当天日k 修改最高最低
	uint64_t Timestamp = GetKlineKey(&_tm, KDAY);
	Bar* kd = GetBar<Bar>(p, Timestamp,  KDAY); //查询到他
	if(kd == NULL)
	{	//日k还没有生成
		return -1;
	}
	if(high > 0.0001) kd->High = high;
	if(low > 0.0001) kd->Low = low;
	
	if(openinterest > 0.00001) kd->OpenInterest = openinterest;
	if(open > 0.00001)	//更新当天所有第一根k
	{
		kd->Open = open;
	}
	
	if(close > 0.00001)	////更新当天所有最后一根
	{
		kd->Close = close;
	}
	return 0;
}

Ticker*  calculate_Ticker(InstrumentData* p,  CThostFtdcDepthMarketDataField* prev, CThostFtdcDepthMarketDataField* pnew)
{
	//获取时间戳
	char timebuf[32] = {0};
	snprintf(timebuf, sizeof(timebuf), "%s %s", pnew->ActionDay, pnew->UpdateTime);
	uint64_t Timestamp =  MAKE_TICK_KEY(strtostamp(timebuf, "%Y%m%d %H:%M:%S"), pnew->UpdateMillisec);
	
	//生成一个ticker
	Ticker* k = GetTickerItem(p, Timestamp, true);
	k->Price	 	=	pnew->LastPrice;
	k->OpenInterest	=	pnew->OpenInterest;
	k->TotalVolume	=	pnew->Volume;
	k->TotalTurnover=	pnew->Turnover;
	k->Volume	 	=	pnew->Volume - prev->Volume;
	k->Turnover	 	=	pnew->Turnover - prev->Turnover;
	k->BidPrice1 	=	pnew->BidPrice1;
	k->AskPrice1 	=	pnew->AskPrice1;
	k->BidVolume1 	=	pnew->BidVolume1;
	k->AskVolume1 	=	pnew->AskVolume1;

	//LOG_DEBUG("TEST k->Volume:" << k->Volume << " k->Turnover:" << k->Turnover);
	//发送ticker数据
	//SessionM::Instance().send_ticker(pnew->TradingDay, pnew->InstrumentID, *k);

	//printf("push ticker\n");

	//LOG_DEBUG("ticker info:" << query_ticker("IF1604"));
	return k;
}

DayBar*  calculate_DayBar(InstrumentData* p, CThostFtdcDepthMarketDataField* prev, CThostFtdcDepthMarketDataField* pnew, uint64_t Timestamp)
{
	//printf("%s pnew->ClosePrice=%f\n", pnew->InstrumentID, pnew->ClosePrice);
	DayBar* kd = GetBar<DayBar>(p, Timestamp, KDAYSNAP, true); //查询到他
	kd->Timestamp			= Timestamp;		
	kd->High			    = pnew->HighestPrice;			
	kd->Low			        = pnew->LowestPrice;			
	kd->Open			    = pnew->OpenPrice;			
	kd->Close			    = pnew->ClosePrice;			
	kd->Average		        = pnew->AveragePrice;		
	kd->Volume			    = pnew->Volume;			
	kd->Turnover		    = pnew->Turnover;		
	kd->Settlement		    = pnew->SettlementPrice;	
	kd->OpenInterest		= pnew->OpenInterest;
	kd->PreSettlement	    = pnew->PreSettlementPrice;	
	kd->PreClose		    = pnew->PreClosePrice;		
	kd->PreoOpenInterest    = pnew->PreOpenInterest;
	kd->Price				= pnew->LastPrice;
	kd->UpperLimit			= pnew->UpperLimitPrice;
	kd->LowerLimit			= pnew->LowerLimitPrice;
	return kd;
}

void Update_Snap(CThostFtdcDepthMarketDataField* psnap, CThostFtdcDepthMarketDataField* pnew)
{             
	if(strncmp(pnew->TradingDay    , psnap->TradingDay    , sizeof(pnew->TradingDay    ))!=0)memcpy(psnap->TradingDay    ,pnew->TradingDay    , sizeof(pnew->TradingDay    ));  
	if(strncmp(pnew->InstrumentID  , psnap->InstrumentID  , sizeof(pnew->InstrumentID  ))!=0)memcpy(psnap->InstrumentID  ,pnew->InstrumentID  , sizeof(pnew->InstrumentID  ));  
	if(strncmp(pnew->ExchangeID    , psnap->ExchangeID    , sizeof(pnew->ExchangeID    ))!=0)memcpy(psnap->ExchangeID    ,pnew->ExchangeID    , sizeof(pnew->ExchangeID    ));  
	if(strncmp(pnew->ExchangeInstID, psnap->ExchangeInstID, sizeof(pnew->ExchangeInstID))!=0)memcpy(psnap->ExchangeInstID,pnew->ExchangeInstID, sizeof(pnew->ExchangeInstID));  
	if(strncmp(pnew->UpdateTime    , psnap->UpdateTime    , sizeof(pnew->UpdateTime    ))!=0)memcpy(psnap->UpdateTime    ,pnew->UpdateTime    , sizeof(pnew->UpdateTime    ));  
	if(strncmp(pnew->ActionDay     , psnap->ActionDay     , sizeof(pnew->ActionDay     ))!=0)memcpy(psnap->ActionDay     ,pnew->ActionDay     , sizeof(pnew->ActionDay     ));                                                                                                                                                                      
	if(fabs(pnew->UpdateMillisec    -  psnap->UpdateMillisec	  ) > 0.0001) psnap->UpdateMillisec    = pnew->UpdateMillisec;
	if(fabs(pnew->LastPrice         - psnap->LastPrice         ) > 0.0001) psnap->LastPrice         = pnew->LastPrice;      
	if(fabs(pnew->PreSettlementPrice- psnap->PreSettlementPrice) > 0.0001) psnap->PreSettlementPrice= pnew->PreSettlementPrice;
	if(fabs(pnew->PreClosePrice     - psnap->PreClosePrice     ) > 0.0001) psnap->PreClosePrice     = pnew->PreClosePrice;  
	if(fabs(pnew->PreOpenInterest   - psnap->PreOpenInterest   ) > 0.0001) psnap->PreOpenInterest   = pnew->PreOpenInterest;   
	if(fabs(pnew->OpenPrice         - psnap->OpenPrice         ) > 0.0001) psnap->OpenPrice         = pnew->OpenPrice;         
	if(fabs(pnew->HighestPrice      - psnap->HighestPrice      ) > 0.0001) psnap->HighestPrice      = pnew->HighestPrice;      
	if(fabs(pnew->LowestPrice       - psnap->LowestPrice       ) > 0.0001) psnap->LowestPrice       = pnew->LowestPrice;       
	if(fabs(pnew->Volume            - psnap->Volume            ) > 0.0001) psnap->Volume            = pnew->Volume;            
	if(fabs(pnew->Turnover          - psnap->Turnover          ) > 0.0001) psnap->Turnover          = pnew->Turnover;          
	if(fabs(pnew->OpenInterest      - psnap->OpenInterest      ) > 0.0001) psnap->OpenInterest      = pnew->OpenInterest;      
	if(fabs(pnew->ClosePrice        - psnap->ClosePrice        ) > 0.0001) psnap->ClosePrice        = pnew->ClosePrice;        
	if(fabs(pnew->SettlementPrice   - psnap->SettlementPrice   ) > 0.0001) psnap->SettlementPrice   = pnew->SettlementPrice;   
	if(fabs(pnew->UpperLimitPrice   - psnap->UpperLimitPrice   ) > 0.0001) psnap->UpperLimitPrice   = pnew->UpperLimitPrice;   
	if(fabs(pnew->LowerLimitPrice   - psnap->LowerLimitPrice   ) > 0.0001) psnap->LowerLimitPrice   = pnew->LowerLimitPrice ;  
	if(fabs(pnew->PreDelta          - psnap->PreDelta          ) > 0.0001) psnap->PreDelta          = pnew->PreDelta;          
	if(fabs(pnew->CurrDelta         - psnap->CurrDelta         ) > 0.0001) psnap->CurrDelta         = pnew->CurrDelta;         
	if(fabs(pnew->BidPrice1         - psnap->BidPrice1         ) > 0.0001) psnap->BidPrice1         = pnew->BidPrice1;         
	if(fabs(pnew->BidVolume1        - psnap->BidVolume1        ) > 0.0001) psnap->BidVolume1        = pnew->BidVolume1;        
	if(fabs(pnew->AskPrice1         - psnap->AskPrice1         ) > 0.0001) psnap->AskPrice1         = pnew->AskPrice1;       
	if(fabs(pnew->AskVolume1        - psnap->AskVolume1        ) > 0.0001) psnap->AskVolume1        = pnew->AskVolume1;        
	if(fabs(pnew->BidPrice2         - psnap->BidPrice2         ) > 0.0001) psnap->BidPrice2         = pnew->BidPrice2;         
	if(fabs(pnew->BidVolume2        - psnap->BidVolume2        ) > 0.0001) psnap->BidVolume2        = pnew->BidVolume2;        
	if(fabs(pnew->AskPrice2         - psnap->AskPrice2         ) > 0.0001) psnap->AskPrice2         = pnew->AskPrice2;         
	if(fabs(pnew->AskVolume2        - psnap->AskVolume2        ) > 0.0001) psnap->AskVolume2        = pnew->AskVolume2;        
	if(fabs(pnew->BidPrice3         - psnap->BidPrice3         ) > 0.0001) psnap->BidPrice3         = pnew->BidPrice3;         
	if(fabs(pnew->BidVolume3        - psnap->BidVolume3        ) > 0.0001) psnap->BidVolume3        = pnew->BidVolume3;        
	if(fabs(pnew->AskPrice3         - psnap->AskPrice3         ) > 0.0001) psnap->AskPrice3         = pnew->AskPrice3;         
	if(fabs(pnew->AskVolume3        - psnap->AskVolume3        ) > 0.0001) psnap->AskVolume3        = pnew->AskVolume3;        
	if(fabs(pnew->BidPrice4         - psnap->BidPrice4         ) > 0.0001) psnap->BidPrice4         = pnew->BidPrice4;         
	if(fabs(pnew->BidVolume4        - psnap->BidVolume4        ) > 0.0001) psnap->BidVolume4        = pnew->BidVolume4;        
	if(fabs(pnew->AskPrice4         - psnap->AskPrice4         ) > 0.0001) psnap->AskPrice4         = pnew->AskPrice4;         
	if(fabs(pnew->AskVolume4        - psnap->AskVolume4        ) > 0.0001) psnap->AskVolume4        = pnew->AskVolume4;        
	if(fabs(pnew->BidPrice5         - psnap->BidPrice5         ) > 0.0001) psnap->BidPrice5         = pnew->BidPrice5;         
	if(fabs(pnew->BidVolume5        - psnap->BidVolume5        ) > 0.0001) psnap->BidVolume5        = pnew->BidVolume5;        
	if(fabs(pnew->AskPrice5         - psnap->AskPrice5         ) > 0.0001) psnap->AskPrice5         = pnew->AskPrice5;         
	if(fabs(pnew->AskVolume5        - psnap->AskVolume5        ) > 0.0001) psnap->AskVolume5        = pnew->AskVolume5;        
	if(fabs(pnew->AveragePrice      - psnap->AveragePrice      ) > 0.0001) psnap->AveragePrice      = pnew->AveragePrice;           
}


string query_ticker(const string& insid)
{
	try{
		Json::Value root;
		if(insid == "")
		{
			mapInstrumentDataType::iterator itr = g_mapInstrmentData.begin();
			for(; itr!=g_mapInstrmentData.end(); itr++)
			{
				Json::Value item;
				item["insid"] = itr->first;
				item["size"]  = (uint32_t)itr->second->TickerItems.size();
				root.append(item);
			}
		}
		else
		{
			mapInstrumentDataType::iterator itr = g_mapInstrmentData.find(insid);
			if(itr != g_mapInstrmentData.end())
			{
				root["insid"] = insid;
				root["size"]  = (uint32_t)itr->second->TickerItems.size();

				Json::Value l;
				list<Ticker>::iterator itt =  itr->second->TickerItems.begin();
				for(itt=itr->second->TickerItems.begin(); itt!=itr->second->TickerItems.end(); itt++)
				{
					Json::Value item;
					item["Timestamp"]		  = (double)itt->Timestamp;       
					item["price"]	  = itt->Price;     
					item["volum"]	  = itt->Volume;     
					item["turnover"]  = itt->Turnover;  
					item["bidprice1"] = itt->BidPrice1;
					item["askprice1"] = itt->AskPrice1;
					item["bidvolum1"] = itt->BidVolume1;
					item["askvolum1"] = itt->AskVolume1;
					l.append(item);
				}
				root["ticker"] = l;
			}
		}
		return root.toStyledString();
	}
	catch(...)
	{
		return "";
	}
}

string query_kline(const string& insid)
{
	try{
		Json::Value root;
		if(insid == "")
		{
			mapInstrumentDataType::iterator itr = g_mapInstrmentData.begin();
			for(; itr!=g_mapInstrmentData.end(); itr++)
			{
				Json::Value item;
				item["insid"]		= itr->first;
				item["size_k1"]		= (uint32_t)itr->second->kline1_minute.size();
				item["size_k3"]		= (uint32_t)itr->second->kline3_minute.size();
				item["size_k5"]		= (uint32_t)itr->second->kline5_minute.size();
				item["size_k10"]	= (uint32_t)itr->second->kline10_minute.size();
				item["size_k15"]	= (uint32_t)itr->second->kline15_minute.size();
				item["size_k30"]	= (uint32_t)itr->second->kline30_minute.size();
				item["size_k60"]	= (uint32_t)itr->second->kline60_minute.size();
				item["size_kd"]		= (uint32_t)itr->second->kline_day.size();
				item["size_km"]		= (uint32_t)itr->second->kline_month.size();
				root.append(item);
			}

		}
		else
		{
			mapInstrumentDataType::iterator itr = g_mapInstrmentData.find(insid);
			if(itr != g_mapInstrmentData.end())
			{
				root["insid"]       = itr->first;                            
				root["size_k1"]     = (uint32_t)itr->second->kline1_minute.size();     
				root["size_k3"]     = (uint32_t)itr->second->kline3_minute.size();     
				root["size_k5"]     = (uint32_t)itr->second->kline5_minute.size();     
				root["size_k10"]    = (uint32_t)itr->second->kline10_minute.size();    
				root["size_k15"]    = (uint32_t)itr->second->kline15_minute.size();    
				root["size_k30"]    = (uint32_t)itr->second->kline30_minute.size();    
				root["size_k60"]    = (uint32_t)itr->second->kline60_minute.size();    
				root["size_kd"]     = (uint32_t)itr->second->kline_day.size();         
				root["size_km"]     = (uint32_t)itr->second->kline_month.size();   
		
	#define  TOJSON(type, Timestampz) do{\
				Json::Value l;\
				list<Bar>::iterator itl;\
				for(itl=type.begin(); itl!=type.end(); itl++)\
				{\
					Json::Value item;\
					item["Timestamp"]			= (double)itl->Timestamp;\
					item["high"]		= itl->High;\
					item["low"]			= itl->Low;\
					item["open"]		= itl->Open;\
					item["close"]		= itl->Close;\
					item["volume"]		= itl->Volume;\
					item["turnover"]	= itl->Turnover;\
					l.append(item);\
				}\
				root[Timestampz] = l;\
	}while(0);
				TOJSON(itr->second->kline_month, "kline_month");
				TOJSON(itr->second->kline_day, "kline_day");
				TOJSON(itr->second->kline60_minute, "kline60_minute");
				TOJSON(itr->second->kline30_minute, "kline30_minute");
				TOJSON(itr->second->kline15_minute, "kline15_minute");
				TOJSON(itr->second->kline10_minute, "kline10_minute");
				TOJSON(itr->second->kline5_minute, "kline5_minute");
				TOJSON(itr->second->kline3_minute, "kline3_minute");
				TOJSON(itr->second->kline1_minute, "kline1_minute");
			}
		}
		return root.toStyledString();
	}
	catch(...)
	{
		return "";
	}
}

string query_snap(const string& insid)
{
	try{
		Json::Value root;
		root["size"] = (uint32_t)g_mapInstrmentData.size();
		if(insid == "")
		{
			Json::Value l;
			mapInstrumentDataType::iterator itr = g_mapInstrmentData.begin();
			for(; itr!=g_mapInstrmentData.end(); itr++)
			{
				Json::Value item;
				//这里增加合约的详细信息	
				item["TradingDay"]			=  itr->second->snap.TradingDay;
				item["InstrumentID"]		=  itr->second->snap.InstrumentID;
				item["ExchangeID"]			=  itr->second->snap.ExchangeID;
				item["ExchangeInstID"]		=  itr->second->snap.ExchangeInstID;
				item["LastPrice"]			=  itr->second->snap.LastPrice;
				item["PreSettlementPrice"]  =  itr->second->snap.PreSettlementPrice;
				item["PreClosePrice"]		=  itr->second->snap.PreClosePrice;
				item["PreOpenInterest"]		=  itr->second->snap.PreOpenInterest;
				item["OpenPrice"]			=  itr->second->snap.OpenPrice;
				item["HighestPrice"]		=  itr->second->snap.HighestPrice;
				item["LowestPrice"]			=  itr->second->snap.LowestPrice;
				item["Volume"]				=  itr->second->snap.Volume;
				item["Turnover"]			=  itr->second->snap.Turnover;
				item["OpenInterest"]		=  itr->second->snap.OpenInterest;
				item["ClosePrice"]			=  itr->second->snap.ClosePrice;
				item["SettlementPrice"]		=  itr->second->snap.SettlementPrice;
				item["UpperLimitPrice"]		=  itr->second->snap.UpperLimitPrice;
				item["LowerLimitPrice"]		=  itr->second->snap.LowerLimitPrice;
				item["PreDelta"]			=  itr->second->snap.PreDelta;
				item["CurrDelta"]			=  itr->second->snap.CurrDelta;
				item["UpdateTime"]			=  itr->second->snap.UpdateTime;
				item["UpdateMillisec"]		=  itr->second->snap.UpdateMillisec;
				item["BidPrice1"]			=  itr->second->snap.BidPrice1;
				item["BidVolume1"]			=  itr->second->snap.BidVolume1;
				item["AskPrice1"]			=  itr->second->snap.AskPrice1;
				item["AskVolume1"]			=  itr->second->snap.AskVolume1;
				item["BidPrice2"]			=  itr->second->snap.BidPrice2;
				item["BidVolume2"]			=  itr->second->snap.BidVolume2;
				item["AskPrice2"]			=  itr->second->snap.AskPrice2;
				item["AskVolume2"]			=  itr->second->snap.AskVolume2;
				item["BidPrice3"]			=  itr->second->snap.BidPrice3;
				item["BidVolume3"]			=  itr->second->snap.BidVolume3;
				item["AskPrice3"]			=  itr->second->snap.AskPrice3;
				item["AskVolume3"]			=  itr->second->snap.AskVolume3;
				item["BidPrice4"]			=  itr->second->snap.BidPrice4;
				item["BidVolume4"]			=  itr->second->snap.BidVolume4;
				item["AskPrice4"]			=  itr->second->snap.AskPrice4;
				item["AskVolume4"]			=  itr->second->snap.AskVolume4;
				item["BidPrice5"]			=  itr->second->snap.BidPrice5;
				item["BidVolume5"]			=  itr->second->snap.BidVolume5;
				item["AskPrice5"]			=  itr->second->snap.AskPrice5;
				item["AskVolume5"]			=  itr->second->snap.AskVolume5;
				item["AveragePrice"]		=  itr->second->snap.AveragePrice;
				item["ActionDay"]			=  itr->second->snap.ActionDay;
				l.append(item);
			}
			root["list"] = l;
		}
		else
		{

			mapInstrumentDataType::iterator itr = g_mapInstrmentData.find(insid);
			if(itr != g_mapInstrmentData.end())
			{
				Json::Value item;
				item["TradingDay"]			= itr->second->snap.TradingDay;
				item["InstrumentID"]		= itr->second->snap.InstrumentID;
				item["ExchangeID"]			= itr->second->snap.ExchangeID;
				item["ExchangeInstID"]		= itr->second->snap.ExchangeInstID;
				item["LastPrice"]			= itr->second->snap.LastPrice;
				item["PreSettlementPrice"]  = itr->second->snap.PreSettlementPrice;
				item["PreClosePrice"]		= itr->second->snap.PreClosePrice;
				item["PreOpenInterest"]		= itr->second->snap.PreOpenInterest;
				item["OpenPrice"]			= itr->second->snap.OpenPrice;
				item["HighestPrice"]		= itr->second->snap.HighestPrice;
				item["LowestPrice"]			= itr->second->snap.LowestPrice;
				item["Volume"]				= itr->second->snap.Volume;
				item["Turnover"]			= itr->second->snap.Turnover;
				item["OpenInterest"]		= itr->second->snap.OpenInterest;
				item["ClosePrice"]			= itr->second->snap.ClosePrice;
				item["SettlementPrice"]		= itr->second->snap.SettlementPrice;
				item["UpperLimitPrice"]		= itr->second->snap.UpperLimitPrice;
				item["LowerLimitPrice"]		= itr->second->snap.LowerLimitPrice;
				item["PreDelta"]			= itr->second->snap.PreDelta;
				item["CurrDelta"]			= itr->second->snap.CurrDelta;
				item["UpdateTime"]			= itr->second->snap.UpdateTime;
				item["UpdateMillisec"]		= itr->second->snap.UpdateMillisec;
				item["BidPrice1"]			= itr->second->snap.BidPrice1;
				item["BidVolume1"]			= itr->second->snap.BidVolume1;
				item["AskPrice1"]			= itr->second->snap.AskPrice1;
				item["AskVolume1"]			= itr->second->snap.AskVolume1;
				item["BidPrice2"]			= itr->second->snap.BidPrice2;
				item["BidVolume2"]			= itr->second->snap.BidVolume2;
				item["AskPrice2"]			= itr->second->snap.AskPrice2;
				item["AskVolume2"]			= itr->second->snap.AskVolume2;
				item["BidPrice3"]			= itr->second->snap.BidPrice3;
				item["BidVolume3"]			= itr->second->snap.BidVolume3;
				item["AskPrice3"]			= itr->second->snap.AskPrice3;
				item["AskVolume3"]			= itr->second->snap.AskVolume3;
				item["BidPrice4"]			= itr->second->snap.BidPrice4;
				item["BidVolume4"]			= itr->second->snap.BidVolume4;
				item["AskPrice4"]			= itr->second->snap.AskPrice4;
				item["AskVolume4"]			= itr->second->snap.AskVolume4;
				item["BidPrice5"]			= itr->second->snap.BidPrice5;
				item["BidVolume5"]			= itr->second->snap.BidVolume5;
				item["AskPrice5"]			= itr->second->snap.AskPrice5;
				item["AskVolume5"]			= itr->second->snap.AskVolume5;
				item["AveragePrice"]		= itr->second->snap.AveragePrice;
				item["ActionDay"]			= itr->second->snap.ActionDay;
				root["snap"] = item;
			}
		}
		return root.toStyledString();
	}
	catch(...)
	{
		return "";
	}
}






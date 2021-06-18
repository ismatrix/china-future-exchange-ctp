/*************************************************************************
    > File Name: CtpJsonStruct.cpp
    > Created Time: Tue 31 May 2016 04:34:35 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#include "CtpJsonStruct.h"
#include "util.h"

//请求查询资金账户响应
Json::Value& get_CThostFtdcTradingAccountField(Json::Value& root, CThostFtdcTradingAccountField *p)
{
	if(p == NULL) return root;

	//root["brokerid"]						=	p->BrokerID;
	root["accountid"]                   	=	p->AccountID;
	root["premortgage"]                 	=	p->PreMortgage;
	root["precredit"]                   	=	p->PreCredit;
	root["predeposit"]                  	=	p->PreDeposit;
	root["prebalance"]                  	=	p->PreBalance;
	root["premargin"]                   	=	p->PreMargin;
	root["interestbase"]			    	=	p->InterestBase;
	root["interest"]                    	=	p->Interest;
	root["deposit"]                     	=	p->Deposit;
	root["withdraw"]                    	=	p->Withdraw;
	root["frozenmargin"]                	=	p->FrozenMargin;
	root["frozencash"]                  	=	p->FrozenCash;
	root["frozencommission"]            	=	p->FrozenCommission;
	root["currmargin"]                  	=	p->CurrMargin;
	//root["cashin"]                      	=	p->CashIn;
	root["commission"]                  	=	p->Commission;
	root["closeprofit"]                 	=	p->CloseProfit;
	root["positionprofit"]              	=	p->PositionProfit;
	root["balance"]                     	=	p->Balance;
	root["available"]                   	=	p->Available;
	root["withdrawquota"]               	=	p->WithdrawQuota;
	root["reserve"]                     	=	p->Reserve;
	root["tradingday"]                  	=	p->TradingDay;
	root["settlementid"]                	=	p->SettlementID;
	root["credit"]                      	=	p->Credit;
	root["mortgage"]                    	=	p->Mortgage;
	root["exchangemargin"]              	=	p->ExchangeMargin;
	root["deliverymargin"]              	=	p->DeliveryMargin;
	root["exchangedeliverymargin"]      	=	p->ExchangeDeliveryMargin;
	root["reservebalance"]              	=	p->ReserveBalance;
	root["currencyid"]                  	=	p->CurrencyID;
	root["prefundmortgagein"]           	=	p->PreFundMortgageIn;
	root["prefundmortgageout"]          	=	p->PreFundMortgageOut;
	root["fundmortgagein"]              	=	p->FundMortgageIn;
	root["fundmortgageout"]             	=	p->FundMortgageOut;
	root["fundmortgageavailable"]       	=	p->FundMortgageAvailable;
	root["mortgageablefund"]            	=	p->MortgageableFund;
	root["specproductmargin"]           	=	p->SpecProductMargin;
	root["specproductfrozenmargin"]     	=	p->SpecProductFrozenMargin;
	root["specproductcommission"]       	=	p->SpecProductCommission;
	root["specproductfrozencommission"] 	=	p->SpecProductFrozenCommission;
	root["specproductpositionprofit"]   	=	p->SpecProductPositionProfit;
	root["specproductcloseprofit"]      	=	p->SpecProductCloseProfit;
	root["specproductpositionprofitbyalg"]	=	p->SpecProductPositionProfitByAlg;
	root["specproductexchangemargin"]   	=	p->SpecProductExchangeMargin;
	return root;
}

CThostFtdcTradingAccountField* get_CThostFtdcTradingAccountField_Struct(Json::Value& root, CThostFtdcTradingAccountField *p)
{
	if(p == NULL) return NULL;
	//if(!root["brokerid"].isNull())  memcpy(&p->BrokerID ,  root["brokerid"].asString().c_str() , sizeof(p->BrokerID));
	if(!root["accountid"].isNull()) memcpy(&p->AccountID,  root["accountid"].asString().c_str(), sizeof(p->AccountID));
	if(!root["premortgage"].isNull())     p->PreMortgage     =root["premortgage"].asDouble()   ;
	if(!root["precredit"].isNull())       p->PreCredit       =root["precredit"].asDouble()     ;
	if(!root["predeposit"].isNull())      p->PreDeposit      =root["predeposit"].asDouble()    ;
	if(!root["prebalance"].isNull())      p->PreBalance      =root["prebalance"].asDouble()    ;
	if(!root["premargin"].isNull())       p->PreMargin       =root["premargin"].asDouble()     ;
	if(!root["interestbase"].isNull())    p->InterestBase    =root["interestbase"].asDouble()  ;
	if(!root["interest"].isNull())        p->Interest        =root["interest"].asDouble()      ;
	if(!root["deposit"].isNull())         p->Deposit         =root["deposit"].asDouble()       ;
	if(!root["withdraw"].isNull())        p->Withdraw        =root["withdraw"].asDouble()      ;
	if(!root["frozenmargin"].isNull())    p->FrozenMargin    =root["frozenmargin"].asDouble()  ;
	if(!root["frozencash"].isNull())      p->FrozenCash      =root["frozencash"].asDouble()    ;
	if(!root["frozencommission"].isNull())p->FrozenCommission=root["frozencommission"].asDouble();
	if(!root["currmargin"].isNull())      p->CurrMargin      =root["currmargin"].asDouble()		;
	//if(!root["cashin"].isNull())          p->CashIn          =root["cashin"].asDouble()        ;
	if(!root["commission"].isNull())      p->Commission      =root["commission"].asDouble()    ;
	if(!root["closeprofit"].isNull())     p->CloseProfit     =root["closeprofit"].asDouble()   ;
	if(!root["positionprofit"].isNull())  p->PositionProfit  =root["positionprofit"].asDouble();
	if(!root["balance"].isNull())         p->Balance         =root["balance"].asDouble()       ;
	if(!root["available"].isNull())       p->Available       =root["available"].asDouble()     ;
	if(!root["withdrawquota"].isNull())   p->WithdrawQuota   =root["withdrawquota"].asDouble() ;
	if(!root["reserve"].isNull())         p->Reserve         =root["reserve"].asDouble()       ;
	if(!root["tradingday"].isNull())    memcpy(&p->TradingDay ,  root["tradingday"].asString().c_str(), sizeof(p->TradingDay));
	if(!root["settlementid"].isNull())  p->SettlementID = root["settlementid"].asInt();
	if(!root["credit"].isNull())                 p->Credit                = root["credit"].asDouble();
	if(!root["mortgage"].isNull())               p->Mortgage              = root["mortgage"].asDouble();
	if(!root["exchangemargin"].isNull())         p->ExchangeMargin        = root["exchangemargin"].asDouble();
	if(!root["deliverymargin"].isNull())         p->DeliveryMargin        = root["deliverymargin"].asDouble();
	if(!root["exchangedeliverymargin"].isNull()) p->ExchangeDeliveryMargin= root["exchangedeliverymargin"].asDouble();
	if(!root["reservebalance"].isNull())         p->ReserveBalance        = root["reservebalance"].asDouble();
	if(!root["currencyid"].isNull())          memcpy(&p->CurrencyID,  root["currencyid"].asString().c_str(), sizeof(p->CurrencyID));
	if(!root["prefundmortgagein"].isNull())               p->PreFundMortgageIn             = root["prefundmortgagein"].asDouble();
	if(!root["prefundmortgageout"].isNull())              p->PreFundMortgageOut            = root["prefundmortgageout"].asDouble();
	if(!root["fundmortgagein"].isNull())                  p->FundMortgageIn                = root["fundmortgagein"].asDouble();
	if(!root["fundmortgageout"].isNull())                 p->FundMortgageOut               = root["fundmortgageout"].asDouble();
	if(!root["fundmortgageavailable"].isNull())           p->FundMortgageAvailable         = root["fundmortgageavailable"].asDouble();
	if(!root["mortgageablefund"].isNull())                p->MortgageableFund              = root["mortgageablefund"].asDouble();
	if(!root["specproductmargin"].isNull())               p->SpecProductMargin             = root["specproductmargin"].asDouble();
	if(!root["specproductfrozenmargin"].isNull())         p->SpecProductFrozenMargin       = root["specproductfrozenmargin"].asDouble();
	if(!root["specproductcommission"].isNull())           p->SpecProductCommission         = root["specproductcommission"].asDouble();
	if(!root["specproductfrozencommission"].isNull())     p->SpecProductFrozenCommission   = root["specproductfrozencommission"].asDouble();
	if(!root["specproductpositionprofit"].isNull())       p->SpecProductPositionProfit     = root["specproductpositionprofit"].asDouble();
	if(!root["specproductcloseprofit"].isNull())          p->SpecProductCloseProfit        = root["specproductcloseprofit"].asDouble();
	if(!root["specproductpositionprofitbyalg"].isNull())  p->SpecProductPositionProfitByAlg= root["specproductpositionprofitbyalg"].asDouble();
	if(!root["specproductexchangemargin"].isNull())       p->SpecProductExchangeMargin     = root["specproductexchangemargin"].asDouble();
	return p;
}


//持仓
Json::Value& get_CThostFtdcInvestorPositionField(Json::Value& root, CThostFtdcInvestorPositionField *p)
{
	if(p == NULL) return root;
	root["instrumentid"]		=	p->InstrumentID;
	//root["brokerid"]          	=	p->BrokerID;
	root["investorid"]        	=	p->InvestorID;
	root["posidirection"]     	=	string(1,p->PosiDirection);
	root["hedgeflag"]         	=	string(1,p->HedgeFlag);
	root["positiondate"]      	=	string(1,p->PositionDate);
	root["ydposition"]        	=	p->YdPosition;
	root["position"]          	=	p->Position;
	root["longfrozen"]        	=	p->LongFrozen;
	root["shortfrozen"]       	=	p->ShortFrozen;
	root["longfrozenamount"]  	=	p->LongFrozenAmount;
	root["shortfrozenamount"] 	=	p->ShortFrozenAmount;
	root["openvolume"]        	=	p->OpenVolume;
	root["closevolume"]       	=	p->CloseVolume;
	root["openamount"]        	=	p->OpenAmount;
	root["closeamount"]       	=	p->CloseAmount;
	root["positioncost"]      	=	p->PositionCost;
	root["premargin"]         	=	p->PreMargin;
	root["usemargin"]         	=	p->UseMargin;
	root["frozenmargin"]      	=	p->FrozenMargin;
	root["frozencash"]        	=	p->FrozenCash;
	root["frozencommission"]  	=	p->FrozenCommission;
	//root["cashin"]            	=	p->CashIn;
	root["commission"]        	=	p->Commission;
	root["closeprofit"]       	=	p->CloseProfit;
	root["positionprofit"]    	=	p->PositionProfit;
	root["presettlementprice"]	=	p->PreSettlementPrice;
	root["settlementprice"]   	=	p->SettlementPrice;
	root["tradingday"]        	=	p->TradingDay;
	root["settlementid"]      	=	p->SettlementID;
	root["opencost"]          	=	p->OpenCost;
	root["exchangemargin"]    	=	p->ExchangeMargin;
	root["combposition"]      	=	p->CombPosition;
	root["comblongfrozen"]    	=	p->CombLongFrozen;
	root["combshortfrozen"]   	=	p->CombShortFrozen;
	root["closeprofitbydate"] 	=	p->CloseProfitByDate;
	root["closeprofitbytrade"]	=	p->CloseProfitByTrade;
	root["todayposition"]     	=	p->TodayPosition;
	root["marginratebymoney"] 	=	p->MarginRateByMoney;
	root["marginratebyvolume"]	=	p->MarginRateByVolume;
	root["strikefrozen"]      	=	p->StrikeFrozen;
	root["strikefrozenamount"]	=	p->StrikeFrozenAmount;
	root["abandonfrozen"]     	=	p->AbandonFrozen;
	return root;
}

CThostFtdcInvestorPositionField*  get_CThostFtdcInvestorPositionField_Struct(Json::Value& root, CThostFtdcInvestorPositionField *p)
{
	if(p == NULL) return NULL;
	if(!root["instrumentid"].isNull())       memcpy(&p->InstrumentID	   ,  root["instrumentid"].asString().c_str()         , sizeof(p->InstrumentID		 ));
	//if(!root["brokerid"].isNull())           memcpy(&p->BrokerID           ,  root["brokerid"].asString().c_str()             , sizeof(p->BrokerID           ));
	if(!root["investorid"].isNull())         memcpy(&p->InvestorID         ,  root["investorid"].asString().c_str()           , sizeof(p->InvestorID         ));
	if(!root["posidirection"].isNull())      memcpy(&p->PosiDirection      , root["posidirection"].asString().c_str(), sizeof(p->PosiDirection));
	if(!root["hedgeflag"].isNull())          memcpy(&p->HedgeFlag          , root["hedgeflag"].asString().c_str(), sizeof(p->HedgeFlag));
	if(!root["positiondate"].isNull())       memcpy(&p->PositionDate       , root["positiondate"].asString().c_str(), sizeof(p->PositionDate));
	if(!root["ydposition"].isNull())         p->YdPosition        =  root["ydposition"].asInt();
	if(!root["position"].isNull())           p->Position          =  root["position"].asInt();
	if(!root["longfrozen"].isNull())         p->LongFrozen        =  root["longfrozen"].asInt();
	if(!root["shortfrozen"].isNull())        p->ShortFrozen       =  root["shortfrozen"].asInt();
	if(!root["longfrozenamount"].isNull())   p->LongFrozenAmount  =  root["longfrozenamount"].asDouble();
	if(!root["shortfrozenamount"].isNull())  p->ShortFrozenAmount =  root["shortfrozenamount"].asDouble();
	if(!root["openvolume"].isNull())         p->OpenVolume        =  root["openvolume"].asInt();
	if(!root["closevolume"].isNull())        p->CloseVolume       =  root["closevolume"].asInt();
	if(!root["openamount"].isNull())         p->OpenAmount        =  root["openamount"].asDouble();
	if(!root["closeamount"].isNull())        p->CloseAmount       =  root["closeamount"].asDouble();
	if(!root["positioncost"].isNull())       p->PositionCost      =  root["positioncost"].asDouble();
	if(!root["premargin"].isNull())          p->PreMargin         =  root["premargin"].asDouble();
	if(!root["usemargin"].isNull())          p->UseMargin         =  root["usemargin"].asDouble();
	if(!root["frozenmargin"].isNull())       p->FrozenMargin      =  root["frozenmargin"].asDouble();
	if(!root["frozencash"].isNull())         p->FrozenCash        =  root["frozencash"].asDouble();
	if(!root["frozencommission"].isNull())   p->FrozenCommission  =  root["frozencommission"].asDouble();
	//if(!root["cashin"].isNull())             p->CashIn            =  root["cashin"].asDouble();
	if(!root["commission"].isNull())         p->Commission        =  root["commission"].asDouble();
	if(!root["closeprofit"].isNull())        p->CloseProfit       =  root["closeprofit"].asDouble();
	if(!root["positionprofit"].isNull())     p->PositionProfit    =  root["positionprofit"].asDouble();
	if(!root["presettlementprice"].isNull()) p->PreSettlementPrice=  root["presettlementprice"].asDouble();
	if(!root["settlementprice"].isNull())    p->SettlementPrice   =  root["settlementprice"].asDouble();
	if(!root["tradingday"].isNull())         memcpy(&p->TradingDay         ,  root["tradingday"].asString().c_str()           , sizeof(p->TradingDay         ));
	if(!root["settlementid"].isNull())       p->SettlementID       =  root["settlementid"].asInt();
	if(!root["opencost"].isNull())           p->OpenCost           =  root["opencost"].asDouble();
	if(!root["exchangemargin"].isNull())     p->ExchangeMargin     =  root["exchangemargin"].asDouble();
	if(!root["combposition"].isNull())       p->CombPosition       =  root["combposition"].asInt();
	if(!root["comblongfrozen"].isNull())     p->CombLongFrozen     =  root["comblongfrozen"].asInt();
	if(!root["combshortfrozen"].isNull())    p->CombShortFrozen    =  root["combshortfrozen"].asInt();
	if(!root["closeprofitbydate"].isNull())  p->CloseProfitByDate  =  root["closeprofitbydate"].asDouble();
	if(!root["closeprofitbytrade"].isNull()) p->CloseProfitByTrade =  root["closeprofitbytrade"].asDouble();
	if(!root["todayposition"].isNull())      p->TodayPosition      =  root["todayposition"].asInt();
	if(!root["marginratebymoney"].isNull())  p->MarginRateByMoney  =  root["marginratebymoney"].asDouble();
	if(!root["marginratebyvolume"].isNull()) p->MarginRateByVolume =  root["marginratebyvolume"].asDouble();
	if(!root["strikefrozen"].isNull())       p->StrikeFrozen       =  root["strikefrozen"].asInt();
	if(!root["strikefrozenamount"].isNull()) p->StrikeFrozenAmount =  root["strikefrozenamount"].asDouble();
	if(!root["abandonfrozen"].isNull())      p->AbandonFrozen      =  root["abandonfrozen"].asInt();

	return p;	
}
//持仓明细
Json::Value& get_CThostFtdcInvestorPositionDetailField(Json::Value& root, CThostFtdcInvestorPositionDetailField *p)
{
	if(p == NULL) return root;
	root["instrumentid"]			=	p->InstrumentID;
	//root["brokerid"]                =	p->BrokerID;
	root["investorid"]              =	p->InvestorID;
	root["hedgeflag"]               =	ctos(p->HedgeFlag);
	root["direction"]               =	ctos(p->Direction);
	root["opendate"]                =	p->OpenDate;
	root["tradeid"]                 =	p->TradeID;
	root["volume"]                  =	p->Volume;
	root["openprice"]               =	p->OpenPrice;
	root["tradingday"]              =	p->TradingDay;
	root["settlementid"]            =	p->SettlementID;
	root["tradetype"]               =	ctos(p->TradeType);
	root["combinstrumentid"]        =	p->CombInstrumentID;
	root["exchangeid"]              =	p->ExchangeID;
	root["closeprofitbydate"]       =	p->CloseProfitByDate;
	root["closeprofitbytrade"]      =	p->CloseProfitByTrade;
	root["positionprofitbydate"]    =	p->PositionProfitByDate;
	root["positionprofitbytrade"]   =	p->PositionProfitByTrade;
	root["margin"]                  =	p->Margin;
	root["exchmargin"]              =	p->ExchMargin;
	root["marginratebymoney"]       =	p->MarginRateByMoney;
	root["marginratebyvolume"]      =	p->MarginRateByVolume;
	root["lastsettlementprice"]     =	p->LastSettlementPrice;
	root["settlementprice"]         =	p->SettlementPrice;
	root["closevolume"]             =	p->CloseVolume;
	root["closeamount"]             =	p->CloseAmount;
	return root;
}

CThostFtdcInvestorPositionDetailField*  get_CThostFtdcInvestorPositionDetailField_Struct(Json::Value& root, CThostFtdcInvestorPositionDetailField *p)
{
	if(p == NULL) return NULL;
	if(!root["instrumentid"].isNull())          memcpy(&p->InstrumentID			 ,  root["instrumentid"].asString().c_str()           , sizeof(p->InstrumentID		   ));
	//if(!root["brokerid"].isNull())              memcpy(&p->BrokerID              ,  root["brokerid"].asString().c_str()              , sizeof(p->BrokerID              ));
	if(!root["investorid"].isNull())            memcpy(&p->InvestorID            ,  root["investorid"].asString().c_str()            , sizeof(p->InvestorID            ));
	if(!root["hedgeflag"].isNull())             memcpy(&p->HedgeFlag             ,  root["hedgeflag"].asString().c_str(), sizeof(p->HedgeFlag));
	if(!root["direction"].isNull())             memcpy(&p->Direction             , root["direction"].asString().c_str(), sizeof(p->Direction));
	if(!root["opendate"].isNull())              memcpy(&p->OpenDate              ,  root["opendate"].asString().c_str()              , sizeof(p->OpenDate              ));
	if(!root["tradeid"].isNull())               memcpy(&p->TradeID               ,  root["tradeid"].asString().c_str()               , sizeof(p->TradeID               ));
	if(!root["volume"].isNull())                p->Volume                = root["volume"].asInt();
	if(!root["openprice"].isNull())             p->OpenPrice             =  root["openprice"].asDouble();
	if(!root["tradingday"].isNull())            memcpy(&p->TradingDay            ,  root["tradingday"].asString().c_str()            , sizeof(p->TradingDay            ));
	if(!root["settlementid"].isNull())          p->SettlementID          =  root["settlementid"].asInt();
	if(!root["tradetype"].isNull())             memcpy(&p->TradeType             ,  root["tradetype"].asString().c_str(), sizeof(p->TradeType));
	if(!root["combinstrumentid"].isNull())      memcpy(&p->CombInstrumentID      ,  root["combinstrumentid"].asString().c_str()      , sizeof(p->CombInstrumentID      ));
	if(!root["exchangeid"].isNull())            memcpy(&p->ExchangeID            ,  root["exchangeid"].asString().c_str()            , sizeof(p->ExchangeID            ));
	if(!root["closeprofitbydate"].isNull())     p->CloseProfitByDate     =  root["closeprofitbydate"].asDouble();
	if(!root["closeprofitbytrade"].isNull())    p->CloseProfitByTrade    =  root["closeprofitbytrade"].asDouble();
	if(!root["positionprofitbydate"].isNull())  p->PositionProfitByDate  =  root["positionprofitbydate"].asDouble();
	if(!root["positionprofitbytrade"].isNull()) p->PositionProfitByTrade =  root["positionprofitbytrade"].asDouble();
	if(!root["margin"].isNull())                p->Margin                =  root["margin"].asDouble();
	if(!root["exchmargin"].isNull())            p->ExchMargin            =  root["exchmargin"].asDouble();
	if(!root["marginratebymoney"].isNull())     p->MarginRateByMoney     =  root["marginratebymoney"].asDouble();
	if(!root["marginratebyvolume"].isNull())    p->MarginRateByVolume    =  root["marginratebyvolume"].asDouble();
	if(!root["lastsettlementprice"].isNull())   p->LastSettlementPrice   =  root["lastsettlementprice"].asDouble();
	if(!root["settlementprice"].isNull())       p->SettlementPrice       =  root["settlementprice"].asDouble();
	if(!root["closevolume"].isNull())           p->CloseVolume           =  root["closevolume"].asInt();
	if(!root["closeamount"].isNull())           p->CloseAmount           =  root["closeamount"].asDouble();
         
	return p;
}

//成交
Json::Value& get_CThostFtdcTradeField(Json::Value& root, CThostFtdcTradeField *p)
{
	if(p == NULL) return root;
	//root["brokerid"]		=	p->BrokerID;
	root["investorid"]		=	p->InvestorID;
	root["instrumentid"]	=	p->InstrumentID;
	root["orderref"]		=	p->OrderRef;
	root["userid"]			=	p->UserID;
	root["exchangeid"]		=	p->ExchangeID;
	root["tradeid"]			=	p->TradeID;
	root["direction"]		=	ctos(p->Direction);
	root["ordersysid"]		=	p->OrderSysID;
	root["participantid"]	=	p->ParticipantID;
	root["clientid"]		=	p->ClientID;
	root["tradingrole"]		=	ctos(p->TradingRole);
	root["exchangeinstid"]	=	p->ExchangeInstID;
	root["offsetflag"]		=	ctos(p->OffsetFlag);
	root["hedgeflag"]		=	ctos(p->HedgeFlag);
	root["price"]			=	p->Price;
	root["volume"]			=	p->Volume;
	root["tradedate"]		=	p->TradeDate;
	root["tradetime"]		=	p->TradeTime;
	root["tradetype"]		=	ctos(p->TradeType);
	root["pricesource"]		=	ctos(p->PriceSource);
	root["traderid"]		=	p->TraderID;
	root["orderlocalid"]	=	p->OrderLocalID;
	root["clearingpartid"]	=	p->ClearingPartID;
	root["businessunit"]	=	p->BusinessUnit;
	root["sequenceno"]		=	p->SequenceNo;
	root["tradingday"]		=	p->TradingDay;
	root["settlementid"]	=	p->SettlementID;
	root["brokerorderseq"]	=	p->BrokerOrderSeq;
	root["tradesource"]		=	ctos(p->TradeSource);
	return root;
}

CThostFtdcTradeField* get_CThostFtdcTradeField_Struct(Json::Value& root, CThostFtdcTradeField *p)
{
	if(p == NULL) return NULL;
	//if(!root["brokerid"].isNull())       memcpy(&p->BrokerID       ,  root["brokerid"].asString().c_str()      , sizeof(p->BrokerID       ));
	if(!root["investorid"].isNull())     memcpy(&p->InvestorID     ,  root["investorid"].asString().c_str()     , sizeof(p->InvestorID     ));
	if(!root["instrumentid"].isNull())   memcpy(&p->InstrumentID   ,  root["instrumentid"].asString().c_str()   , sizeof(p->InstrumentID   ));
	if(!root["orderref"].isNull())       memcpy(&p->OrderRef       ,  root["orderref"].asString().c_str()       , sizeof(p->OrderRef       ));
	if(!root["userid"].isNull())         memcpy(&p->UserID         ,  root["userid"].asString().c_str()         , sizeof(p->UserID         ));
	if(!root["exchangeid"].isNull())     memcpy(&p->ExchangeID     ,  root["exchangeid"].asString().c_str()     , sizeof(p->ExchangeID     ));
	if(!root["tradeid"].isNull())        memcpy(&p->TradeID        ,  root["tradeid"].asString().c_str()        , sizeof(p->TradeID        ));
	if(!root["direction"].isNull())      memcpy(&p->Direction      ,  root["direction"].asString().c_str()		, sizeof(p->Direction));
	if(!root["ordersysid"].isNull())     memcpy(&p->OrderSysID     ,  root["ordersysid"].asString().c_str()     , sizeof(p->OrderSysID     ));
	if(!root["participantid"].isNull())  memcpy(&p->ParticipantID  ,  root["participantid"].asString().c_str()  , sizeof(p->ParticipantID  ));
	if(!root["clientid"].isNull())       memcpy(&p->ClientID       ,  root["clientid"].asString().c_str()       , sizeof(p->ClientID       ));
	if(!root["tradingrole"].isNull())    memcpy(&p->TradingRole    ,  root["tradingrole"].asString().c_str()	, sizeof(p->TradingRole));
	if(!root["exchangeinstid"].isNull()) memcpy(&p->ExchangeInstID ,  root["exchangeinstid"].asString().c_str() , sizeof(p->ExchangeInstID ));
	if(!root["offsetflag"].isNull())     memcpy(&p->OffsetFlag     ,  root["offsetflag"].asString().c_str()		, sizeof(p->OffsetFlag));
	if(!root["hedgeflag"].isNull())      memcpy(&p->HedgeFlag      ,  root["hedgeflag"].asString().c_str()		, sizeof(p->HedgeFlag));
	if(!root["price"].isNull())          p->Price          =  root["price"].asDouble();
	if(!root["volume"].isNull())         p->Volume         =  root["volume"].asInt();
	if(!root["tradedate"].isNull())      memcpy(&p->TradeDate      ,  root["tradedate"].asString().c_str()      , sizeof(p->TradeDate      ));
	if(!root["tradetime"].isNull())      memcpy(&p->TradeTime      ,  root["tradetime"].asString().c_str()      , sizeof(p->TradeTime      ));
	if(!root["tradetype"].isNull())      memcpy(&p->TradeType      ,  root["tradetype"].asString().c_str()		, sizeof(p->TradeType));
	if(!root["pricesource"].isNull())    memcpy(&p->PriceSource    ,  root["pricesource"].asString().c_str()	, sizeof(p->PriceSource));
	if(!root["traderid"].isNull())       memcpy(&p->TraderID       ,  root["traderid"].asString().c_str()       , sizeof(p->TraderID       ));
	if(!root["orderlocalid"].isNull())   memcpy(&p->OrderLocalID   ,  root["orderlocalid"].asString().c_str()   , sizeof(p->OrderLocalID   ));
	if(!root["clearingpartid"].isNull()) memcpy(&p->ClearingPartID ,  root["clearingpartid"].asString().c_str() , sizeof(p->ClearingPartID ));
	if(!root["businessunit"].isNull())   memcpy(&p->BusinessUnit   ,  root["businessunit"].asString().c_str()   , sizeof(p->BusinessUnit   ));
	if(!root["sequenceno"].isNull())     p->SequenceNo     =  root["sequenceno"].asInt();
	if(!root["tradingday"].isNull())     memcpy(&p->TradingDay     ,  root["tradingday"].asString().c_str()     , sizeof(p->TradingDay     ));
	if(!root["settlementid"].isNull())   p->SettlementID   =  root["settlementid"].asInt();
	if(!root["brokerorderseq"].isNull()) p->BrokerOrderSeq =  root["brokerorderseq"].asInt();
	if(!root["tradesource"].isNull())    memcpy(&p->TradeSource    ,  root["tradesource"].asString().c_str()	, sizeof(p->TradeSource));
                                                                                                                                            
	return p;
}

//报单
Json::Value& get_CThostFtdcOrderField(Json::Value& root, CThostFtdcOrderField *p)
{
	if(p == NULL) return root;
	//root["BrokerID"]				= p->BrokerID;
	root["InvestorID"]				= p->InvestorID;
	root["InstrumentID"]			= p->InstrumentID;
	root["OrderRef"]				= p->OrderRef;
	root["UserID"]					= p->UserID;
	root["OrderPriceType"]			= ctos(p->OrderPriceType);
	root["Direction"]				= ctos(p->Direction);
	root["CombOffsetFlag"]			= p->CombOffsetFlag;
	root["CombHedgeFlag"]			= p->CombHedgeFlag;
	root["LimitPrice"]				= p->LimitPrice;
	root["VolumeTotalOriginal"]		= p->VolumeTotalOriginal;
	root["TimeCondition"]			= p->TimeCondition;
	root["GTDDate"]					= p->GTDDate;
	root["VolumeCondition"]			= ctos(p->VolumeCondition);
	root["MinVolume"]				= p->MinVolume;
	root["ContingentCondition"]		= ctos(p->ContingentCondition);
	root["StopPrice"]				= p->StopPrice;
	root["ForceCloseReason"]		= ctos(p->ForceCloseReason);
	root["IsAutoSuspend"]			= p->IsAutoSuspend;
	root["BusinessUnit"]			= p->BusinessUnit;
	root["RequestID"]				= p->RequestID;
	root["OrderLocalID"]			= p->OrderLocalID;
	root["ExchangeID"]				= p->ExchangeID;
	root["ParticipantID"]			= p->ParticipantID;
	root["ClientID"]				= p->ClientID;
	root["ExchangeInstID"]			= p->ExchangeInstID;
	root["TraderID"]				= p->TraderID;
	root["InstallID"]				= p->InstallID;
	root["OrderSubmitStatus"]		= ctos(p->OrderSubmitStatus);
	root["NotifySequence"]			= p->NotifySequence;
	root["TradingDay"]				= p->TradingDay;
	root["SettlementID"]			= p->SettlementID;
	root["OrderSysID"]				= p->OrderSysID;
	root["OrderSource"]				= ctos(p->OrderSource);
	root["OrderStatus"]				= ctos(p->OrderStatus);
	root["OrderType"]				= ctos(p->OrderType);
	root["VolumeTraded"]			= p->VolumeTraded;
	root["VolumeTotal"]				= p->VolumeTotal;
	root["InsertDate"]				= p->InsertDate;
	root["InsertTime"]				= p->InsertTime;
	root["ActiveTime"]				= p->ActiveTime;
	root["SuspendTime"]				= p->SuspendTime;
	root["UpdateTime"]				= p->UpdateTime;
	root["CancelTime"]				= p->CancelTime;
	root["ActiveTraderID"]			= p->ActiveTraderID;
	root["ClearingPartID"]			= p->ClearingPartID;
	root["SequenceNo"]				= p->SequenceNo;
	root["FrontID"]					= p->FrontID;
	root["SessionID"]				= p->SessionID;
	root["UserProductInfo"]			= p->UserProductInfo;
	root["StatusMsg"]				= p->StatusMsg;
	root["UserForceClose"]			= p->UserForceClose;
	root["ActiveUserID"]			= p->ActiveUserID;
	root["BrokerOrderSeq"]			= p->BrokerOrderSeq;
	root["RelativeOrderSysID"]		= p->RelativeOrderSysID;
	root["ZCETotalTradedVolume"]	= p->ZCETotalTradedVolume;
	root["IsSwapOrder"]				= p->IsSwapOrder;
	root["BranchID"]				= p->BranchID;
	root["InvestUnitID"]			= p->InvestUnitID;
	root["AccountID"]				= p->AccountID;
	root["CurrencyID"]				= p->CurrencyID;
	root["IPAddress"]				= p->IPAddress;
	root["MacAddress"]				= p->MacAddress;
	return root;
}

CThostFtdcOrderField* get_CThostFtdcOrderField_Struct(Json::Value& root, CThostFtdcOrderField *p)
{
	if(p == NULL) return NULL;
	//if(!root["BrokerID"].isNull())				memcpy(&p->BrokerID,			root["BrokerID"].asString().c_str(),				 sizeof(p->BrokerID			   )); 	 
	if(!root["InvestorID"].isNull())			memcpy(&p->InvestorID,          root["InvestorID"].asString().c_str(),               sizeof(p->InvestorID          )); 
	if(!root["InstrumentID"].isNull())			memcpy(&p->InstrumentID,        root["InstrumentID"].asString().c_str(),             sizeof(p->InstrumentID        )); 
	if(!root["OrderRef"].isNull())				memcpy(&p->OrderRef,            root["OrderRef"].asString().c_str(),                 sizeof(p->OrderRef            )); 
	if(!root["UserID"].isNull())				memcpy(&p->UserID,              root["UserID"].asString().c_str(),                   sizeof(p->UserID              )); 
	if(!root["OrderPriceType"].isNull())		memcpy(&p->OrderPriceType	,   root["OrderPriceType"].asString().c_str(), sizeof(p->OrderPriceType)); 
	if(!root["Direction"].isNull())				memcpy(&p->Direction		,   root["Direction"].asString().c_str(), sizeof(p->Direction)); 
	if(!root["CombOffsetFlag"].isNull())		memcpy(&p->CombOffsetFlag,      root["CombOffsetFlag"].asString().c_str(),           sizeof(p->CombOffsetFlag      )); 
	if(!root["CombHedgeFlag"].isNull())			memcpy(&p->CombHedgeFlag,       root["CombHedgeFlag"].asString().c_str(),            sizeof(p->CombHedgeFlag       )); 
	if(!root["LimitPrice"].isNull())			p->LimitPrice          =  root["LimitPrice"].asDouble(); 
	if(!root["VolumeTotalOriginal"].isNull())	p->VolumeTotalOriginal =  root["VolumeTotalOriginal"].asInt(); 
	if(!root["TimeCondition"].isNull())			p->TimeCondition       =  root["TimeCondition"].asInt(); 
	if(!root["GTDDate"].isNull())				memcpy(&p->GTDDate,             root["GTDDate"].asString().c_str(),                  sizeof(p->GTDDate             )); 
	if(!root["VolumeCondition"].isNull())		memcpy(&p->VolumeCondition    ,  root["VolumeCondition"].asString().c_str(), sizeof(p->VolumeCondition)); 
	if(!root["MinVolume"].isNull())				p->MinVolume          =  root["MinVolume"].asInt(); 
	if(!root["ContingentCondition"].isNull())	memcpy(&p->ContingentCondition,  root["ContingentCondition"].asString().c_str(), sizeof(p->ContingentCondition)); 
	if(!root["StopPrice"].isNull())				p->StopPrice          =  root["StopPrice"].asDouble(); 
	if(!root["ForceCloseReason"].isNull())		memcpy(&p->ForceCloseReason   ,  root["ForceCloseReason"].asString().c_str(), sizeof(p->ForceCloseReason)); 
	if(!root["IsAutoSuspend"].isNull())			p->IsAutoSuspend      =  root["IsAutoSuspend"].asInt(); 
	if(!root["BusinessUnit"].isNull())			memcpy(&p->BusinessUnit,        root["BusinessUnit"].asString().c_str(),             sizeof(p->BusinessUnit        )); 
	if(!root["RequestID"].isNull())				p->RequestID          =  root["RequestID"].asInt(); 
	if(!root["OrderLocalID"].isNull())			memcpy(&p->OrderLocalID,        root["OrderLocalID"].asString().c_str(),             sizeof(p->OrderLocalID        )); 
	if(!root["ExchangeID"].isNull())			memcpy(&p->ExchangeID,          root["ExchangeID"].asString().c_str(),               sizeof(p->ExchangeID          )); 
	if(!root["ParticipantID"].isNull())			memcpy(&p->ParticipantID,       root["ParticipantID"].asString().c_str(),            sizeof(p->ParticipantID       )); 
	if(!root["ClientID"].isNull())				memcpy(&p->ClientID,            root["ClientID"].asString().c_str(),                 sizeof(p->ClientID            )); 
	if(!root["ExchangeInstID"].isNull())		memcpy(&p->ExchangeInstID,      root["ExchangeInstID"].asString().c_str(),           sizeof(p->ExchangeInstID      )); 
	if(!root["TraderID"].isNull())				memcpy(&p->TraderID,            root["TraderID"].asString().c_str(),                 sizeof(p->TraderID            )); 
	if(!root["InstallID"].isNull())				p->InstallID          = root["InstallID"].asInt(); 
	if(!root["OrderSubmitStatus"].isNull())		memcpy(&p->OrderSubmitStatus  , root["OrderSubmitStatus"].asString().c_str(), sizeof(p->OrderSubmitStatus)); 
	if(!root["NotifySequence"].isNull())		p->NotifySequence     = root["NotifySequence"].asInt(); 
	if(!root["TradingDay"].isNull())			memcpy(&p->TradingDay,          root["TradingDay"].asString().c_str(),               sizeof(p->TradingDay          )); 
	if(!root["SettlementID"].isNull())			p->SettlementID       = root["SettlementID"].asInt(); 
	if(!root["OrderSysID"].isNull())			memcpy(&p->OrderSysID,          root["OrderSysID"].asString().c_str(),               sizeof(p->OrderSysID          )); 
	if(!root["OrderSource"].isNull())			memcpy(&p->OrderSource        ,  root["OrderSource"].asString().c_str(), sizeof(p->OrderSource)); 
	if(!root["OrderStatus"].isNull())			memcpy(&p->OrderStatus        ,  root["OrderStatus"].asString().c_str(), sizeof(p->OrderStatus)); 
	if(!root["OrderType"].isNull())				memcpy(&p->OrderType          ,  root["OrderType"].asString().c_str(), sizeof(p->OrderType)); 
	if(!root["VolumeTraded"].isNull())			p->VolumeTraded       =  root["VolumeTraded"].asInt(); 
	if(!root["VolumeTotal"].isNull())			p->VolumeTotal        =  root["VolumeTotal"].asInt(); 
	if(!root["InsertDate"].isNull())			memcpy(&p->InsertDate,          root["InsertDate"].asString().c_str(),               sizeof(p->InsertDate          )); 
	if(!root["InsertTime"].isNull())			memcpy(&p->InsertTime,          root["InsertTime"].asString().c_str(),               sizeof(p->InsertTime          )); 
	if(!root["ActiveTime"].isNull())			memcpy(&p->ActiveTime,          root["ActiveTime"].asString().c_str(),               sizeof(p->ActiveTime          )); 
	if(!root["SuspendTime"].isNull())			memcpy(&p->SuspendTime,         root["SuspendTime"].asString().c_str(),              sizeof(p->SuspendTime         )); 
	if(!root["UpdateTime"].isNull())			memcpy(&p->UpdateTime,          root["UpdateTime"].asString().c_str(),               sizeof(p->UpdateTime          )); 
	if(!root["CancelTime"].isNull())			memcpy(&p->CancelTime,          root["CancelTime"].asString().c_str(),               sizeof(p->CancelTime          )); 
	if(!root["ActiveTraderID"].isNull())		memcpy(&p->ActiveTraderID,      root["ActiveTraderID"].asString().c_str(),           sizeof(p->ActiveTraderID      )); 
	if(!root["ClearingPartID"].isNull())		memcpy(&p->ClearingPartID,      root["ClearingPartID"].asString().c_str(),           sizeof(p->ClearingPartID      )); 
	if(!root["SequenceNo"].isNull())			p->SequenceNo        =  root["SequenceNo"].asInt(); 
	if(!root["FrontID"].isNull())				p->FrontID           =  root["FrontID"].asInt(); 
	if(!root["SessionID"].isNull())				p->SessionID         =  root["SessionID"].asInt(); 
	if(!root["UserProductInfo"].isNull())		memcpy(&p->UserProductInfo,     root["UserProductInfo"].asString().c_str(),          sizeof(p->UserProductInfo     )); 
	if(!root["StatusMsg"].isNull())				memcpy(&p->StatusMsg,           root["StatusMsg"].asString().c_str(),                sizeof(p->StatusMsg           )); 
	if(!root["UserForceClose"].isNull())		p->UserForceClose    =   root["UserForceClose"].asInt(); 
	if(!root["ActiveUserID"].isNull())			memcpy(&p->ActiveUserID,        root["ActiveUserID"].asString().c_str(),             sizeof(p->ActiveUserID        )); 
	if(!root["BrokerOrderSeq"].isNull())		p->BrokerOrderSeq    =   root["BrokerOrderSeq"].asInt(); 
	if(!root["RelativeOrderSysID"].isNull())	memcpy(&p->RelativeOrderSysID,  root["RelativeOrderSysID"].asString().c_str(),       sizeof(p->RelativeOrderSysID  )); 
	if(!root["ZCETotalTradedVolume"].isNull())	p->ZCETotalTradedVolume = root["ZCETotalTradedVolume"].asInt(); 
	if(!root["IsSwapOrder"].isNull())			p->IsSwapOrder          = root["IsSwapOrder"].asInt(); 
	if(!root["BranchID"].isNull())				memcpy(&p->BranchID,            root["BranchID"].asString().c_str(),                 sizeof(p->BranchID            )); 
	if(!root["InvestUnitID"].isNull())			memcpy(&p->InvestUnitID,        root["InvestUnitID"].asString().c_str(),             sizeof(p->InvestUnitID        )); 
	if(!root["AccountID"].isNull())				memcpy(&p->AccountID,           root["AccountID"].asString().c_str(),                sizeof(p->AccountID           )); 
	if(!root["CurrencyID"].isNull())			memcpy(&p->CurrencyID,          root["CurrencyID"].asString().c_str(),               sizeof(p->CurrencyID          )); 
	if(!root["IPAddress"].isNull())				memcpy(&p->IPAddress,           root["IPAddress"].asString().c_str(),                sizeof(p->IPAddress           )); 
	if(!root["MacAddress"].isNull())			memcpy(&p->MacAddress,          root["MacAddress"].asString().c_str(),               sizeof(p->MacAddress          )); 
	return p;
}

//产品
Json::Value& get_CThostFtdcProductField(Json::Value& root, CThostFtdcProductField *p)
{
	if(p == NULL) return root;
	root["productid"]				=	p->ProductID;
	root["productname"]            	=	p->ProductName;
	root["exchangeid"]             	=	p->ExchangeID;
	root["productclass"]           	=	ctos(p->ProductClass);
	root["volumemultiple"]         	=	p->VolumeMultiple;
	root["pricetick"]              	=	p->PriceTick;
	root["maxmarketordervolume"]   	=	p->MaxMarketOrderVolume;
	root["minmarketordervolume"]   	=	p->MinMarketOrderVolume;
	root["maxlimitordervolume"]    	=	p->MaxLimitOrderVolume;
	root["minlimitordervolume"]    	=	p->MinLimitOrderVolume;
	root["positiontype"]           	=	ctos(p->PositionType);
	root["positiondatetype"]       	=	ctos(p->PositionDateType);
	root["closedealtype"]          	=	ctos(p->CloseDealType);
	root["tradecurrencyid"]        	=	p->TradeCurrencyID;
	root["mortgagefunduserange"]   	=	ctos(p->MortgageFundUseRange);
	root["exchangeproductid"]      	=	p->ExchangeProductID;
	root["underlyingmultiple"]     	=	p->UnderlyingMultiple;
	return root;
}


CThostFtdcProductField* get_CThostFtdcProductField_Struct(Json::Value& root, CThostFtdcProductField *p)
{
	if(p == NULL) return NULL;
	if(!root["productid"].isNull())					memcpy(&p->ProductID			, root["productid"].asString().c_str()            , sizeof(p->ProductID			 ));
	if(!root["productname"].isNull())		        memcpy(&p->ProductName          , root["productname"].asString().c_str()          , sizeof(p->ProductName         ));
	if(!root["exchangeid"].isNull())		        memcpy(&p->ExchangeID           , root["exchangeid"].asString().c_str()           , sizeof(p->ExchangeID          ));
	if(!root["productclass"].isNull())		        memcpy(&p->ProductClass         , root["productclass"].asString().c_str(), sizeof(p->ProductClass));
	if(!root["volumemultiple"].isNull())		    p->VolumeMultiple       = root["volumemultiple"].asInt();
	if(!root["pricetick"].isNull())					p->PriceTick            = root["pricetick"].asDouble();
	if(!root["maxmarketordervolume"].isNull())		p->MaxMarketOrderVolume = root["maxmarketordervolume"].asInt();
	if(!root["minmarketordervolume"].isNull())		p->MinMarketOrderVolume = root["minmarketordervolume"].asInt();
	if(!root["maxlimitordervolume"].isNull())		p->MaxLimitOrderVolume  = root["maxlimitordervolume"].asInt();
	if(!root["minlimitordervolume"].isNull())		p->MinLimitOrderVolume  = root["minlimitordervolume"].asInt();
	if(!root["positiontype"].isNull())		        memcpy(&p->PositionType         , root["positiontype"].asString().c_str(), sizeof(p->PositionType));
	if(!root["positiondatetype"].isNull())		    memcpy(&p->PositionDateType     , root["positiondatetype"].asString().c_str(), sizeof(p->PositionDateType));
	if(!root["closedealtype"].isNull())				memcpy(&p->CloseDealType        , root["closedealtype"].asString().c_str(), sizeof(p->CloseDealType));
	if(!root["mortgagefunduserange"].isNull())		memcpy(&p->MortgageFundUseRange , root["mortgagefunduserange"].asString().c_str(), sizeof(p->MortgageFundUseRange));
	if(!root["tradecurrencyid"].isNull())		    memcpy(&p->TradeCurrencyID      , root["tradecurrencyid"].asString().c_str()      , sizeof(p->TradeCurrencyID     ));

	if(!root["exchangeproductid"].isNull())			memcpy(&p->ExchangeProductID    , root["exchangeproductid"].asString().c_str()    , sizeof(p->ExchangeProductID   ));
	if(!root["underlyingmultiple"].isNull())		p->UnderlyingMultiple   = root["underlyingmultiple"].asDouble();  
	return p;
}
//合约
Json::Value& get_CThostFtdcInstrumentField(Json::Value& root, CThostFtdcInstrumentField *p)
{
	if(p == NULL) return root;
	root["instrumentid"]			=		p->InstrumentID;
	root["exchangeid"]          	=		p->ExchangeID;
	root["instrumentname"]      	=		p->InstrumentName;
	root["exchangeinstid"]      	=		p->ExchangeInstID;
	root["productid"]           	=		p->ProductID;
	root["productclass"]        	=		ctos(p->ProductClass); //char --- string
	root["deliveryyear"]        	=		p->DeliveryYear;
	root["deliverymonth"]       	=		p->DeliveryMonth;
	root["maxmarketordervolume"]	=		p->MaxMarketOrderVolume;
	root["minmarketordervolume"]	=		p->MinMarketOrderVolume;
	root["maxlimitordervolume"] 	=		p->MaxLimitOrderVolume;
	root["minlimitordervolume"] 	=		p->MinLimitOrderVolume;
	root["volumemultiple"]      	=		p->VolumeMultiple;
	root["pricetick"]           	=		p->PriceTick;
	root["createdate"]          	=		p->CreateDate;
	root["opendate"]            	=		p->OpenDate;
	root["expiredate"]          	=		p->ExpireDate;
	root["startdelivdate"]      	=		p->StartDelivDate;
	root["enddelivdate"]        	=		p->EndDelivDate;
	root["instlifephase"]       	=		ctos(p->InstLifePhase);
	root["istrading"]           	=		p->IsTrading;
	root["positiontype"]        	=		ctos(p->PositionType);
	root["positiondatetype"]    	=		ctos(p->PositionDateType);
	root["longmarginratio"]     	=		p->LongMarginRatio;
	root["shortmarginratio"]    	=		p->ShortMarginRatio;
	root["maxmarginsidealgorithm"]	=		ctos(p->MaxMarginSideAlgorithm);
	root["underlyinginstrid"]   	=		p->UnderlyingInstrID;
	root["strikeprice"]         	=		p->StrikePrice;
	root["optionstype"]         	=		ctos(p->OptionsType);
	root["underlyingmultiple"]  	=		p->UnderlyingMultiple;
	root["combinationtype"]     	=		ctos(p->CombinationType);
	return root;
}

CThostFtdcInstrumentField* get_CThostFtdcInstrumentField_Struct(Json::Value& root, CThostFtdcInstrumentField* p)
{
	if(p == NULL) return NULL;
	
	if(!root["instrumentid"].isNull())			 memcpy(&p->InstrumentID           , root["instrumentid"].asString().c_str()          , sizeof(p->InstrumentID           ));
	if(!root["exchangeid"].isNull())			 memcpy(&p->ExchangeID             , root["exchangeid"].asString().c_str()            , sizeof(p->ExchangeID             ));
	if(!root["instrumentname"].isNull())		 memcpy(&p->InstrumentName         , root["instrumentname"].asString().c_str()        , sizeof(p->InstrumentName         ));
	if(!root["exchangeinstid"].isNull())		 memcpy(&p->ExchangeInstID         , root["exchangeinstid"].asString().c_str()        , sizeof(p->ExchangeInstID         ));
	if(!root["productid"].isNull())				 memcpy(&p->ProductID              , root["productid"].asString().c_str()             , sizeof(p->ProductID              ));
	if(!root["productclass"].isNull())			 p->ProductClass           = root["productclass"].asString()[0];// 第一个字符
	if(!root["deliveryyear"].isNull())			 p->DeliveryYear           = root["deliveryyear"].asInt();
	if(!root["deliverymonth"].isNull())			 p->DeliveryMonth          = root["deliverymonth"].asInt();
	if(!root["maxmarketordervolume"].isNull())	 p->MaxMarketOrderVolume   = root["maxmarketordervolume"].asInt();
	if(!root["minmarketordervolume"].isNull())	 p->MinMarketOrderVolume   = root["minmarketordervolume"].asInt();
	if(!root["maxlimitordervolume"].isNull())	 p->MaxLimitOrderVolume    = root["maxlimitordervolume"].asInt();
	if(!root["minlimitordervolume"].isNull())	 p->MinLimitOrderVolume    = root["minlimitordervolume"].asInt();
	if(!root["volumemultiple"].isNull())		 p->VolumeMultiple         = root["volumemultiple"].asInt();
	if(!root["pricetick"].isNull())				 p->PriceTick              = root["pricetick"].asDouble();
	if(!root["createdate"].isNull())			 memcpy(&p->CreateDate             , root["createdate"].asString().c_str()            , sizeof(p->CreateDate             ));
	if(!root["opendate"].isNull())				 memcpy(&p->OpenDate               , root["opendate"].asString().c_str()              , sizeof(p->OpenDate               ));
	if(!root["expiredate"].isNull())			 memcpy(&p->ExpireDate             , root["expiredate"].asString().c_str()            , sizeof(p->ExpireDate             ));
	if(!root["startdelivdate"].isNull())		 memcpy(&p->StartDelivDate         , root["startdelivdate"].asString().c_str()        , sizeof(p->StartDelivDate         ));
	if(!root["enddelivdate"].isNull())			 memcpy(&p->EndDelivDate           , root["enddelivdate"].asString().c_str()          , sizeof(p->EndDelivDate           ));
	if(!root["instlifephase"].isNull())			 p->InstLifePhase          = root["instlifephase"].asString()[0];
	if(!root["istrading"].isNull())				 p->IsTrading              = root["istrading"].asInt();
	if(!root["positiontype"].isNull())			 p->PositionType           = root["positiontype"].asString()[0];
	if(!root["positiondatetype"].isNull())		 p->PositionDateType       = root["positiondatetype"].asString()[0];
	if(!root["longmarginratio"].isNull())		 p->LongMarginRatio        = root["longmarginratio"].asDouble();
	if(!root["shortmarginratio"].isNull())		 p->ShortMarginRatio       = root["shortmarginratio"].asDouble();
	if(!root["maxmarginsidealgorithm"].isNull()) p->MaxMarginSideAlgorithm = root["maxmarginsidealgorithm"].asString()[0];
	if(!root["underlyinginstrid"].isNull())		 memcpy(&p->UnderlyingInstrID      , root["underlyinginstrid"].asString().c_str()     , sizeof(p->UnderlyingInstrID      ));
	if(!root["strikeprice"].isNull())			 p->StrikePrice            = root["strikeprice"].asDouble();
	if(!root["optionstype"].isNull())			 p->OptionsType            = root["optionstype"].asString()[0];
	if(!root["underlyingmultiple"].isNull())	 p->UnderlyingMultiple     = root["underlyingmultiple"].asDouble();
	if(!root["combinationtype"].isNull())		 p->CombinationType        = root["combinationtype"].asString()[0];
	
	return p;

}

//合约状态通知
Json::Value& get_CThostFtdcInstrumentStatusField(Json::Value& root, CThostFtdcInstrumentStatusField *p)
{
	if(p==NULL) return root;
	root["ExchangeID"] 			= p->ExchangeID;
	root["ExchangeInstID"] 		= p->ExchangeInstID;
	root["SettlementGroupID"] 	= p->SettlementGroupID;
	root["InstrumentID"] 		= p->InstrumentID;
	root["InstrumentStatus"] 	= ctos(p->InstrumentStatus);
	root["TradingSegmentSN"] 	= p->TradingSegmentSN;
	root["EnterTime"] 			= p->EnterTime;
	root["EnterReason"] 		= ctos(p->EnterReason);
	return root;
}
//交易所通告通知
Json::Value& get_CThostFtdcBulletinField(Json::Value& root, CThostFtdcBulletinField *p)
{
	if(p==NULL) return root;
	root["ExchangeID"]	= p->ExchangeID;
	root["TradingDay"]	= p->TradingDay;
	root["BulletinID"]	= p->BulletinID;
	root["SequenceNo"]	= p->SequenceNo;
	root["NewsType"]	= p->NewsType;
	root["NewsUrgency"]	= ctos(p->NewsUrgency);
	root["SendTime"]	= p->SendTime;
	root["Abstract"]	= p->Abstract;
	root["ComeFrom"]	= p->ComeFrom;
	root["Content"]		= p->Content;
	root["URLLink"]		= p->URLLink;
	root["MarketID"]	= p->MarketID;
	return root;
}
CThostFtdcBulletinField* get_CThostFtdcBulletinField_Struct(Json::Value& root, CThostFtdcBulletinField *p)
{
	if(p==NULL) return p;
	if(!root["ExchangeID"].isNull())  	memcpy(&p->ExchangeID, root["ExchangeID"].asString().c_str()	, sizeof(p->ExchangeID));
	if(!root["TradingDay"].isNull())  	memcpy(&p->TradingDay, root["TradingDay"].asString().c_str()	, sizeof(p->TradingDay));
	if(!root["BulletinID"].isNull())  	p->BulletinID = root["BulletinID"].asInt();
	if(!root["SequenceNo"].isNull())  	p->SequenceNo = root["SequenceNo"].asInt();
	if(!root["NewsType"].isNull())  	memcpy(&p->NewsType,   root["NewsType"].asString().c_str()		, sizeof(p->NewsType));
	if(!root["NewsUrgency"].isNull())  	p->NewsUrgency	= root["NewsUrgency"].asString()[0];	
	if(!root["SendTime"].isNull())  	memcpy(&p->SendTime,   root["SendTime"].asString().c_str()		, sizeof(p->SendTime));
	if(!root["Abstract"].isNull())  	memcpy(&p->Abstract,   root["Abstract"].asString().c_str()		, sizeof(p->Abstract));
	if(!root["ComeFrom"].isNull())  	memcpy(&p->ComeFrom,   root["ComeFrom"].asString().c_str()		, sizeof(p->ComeFrom));
	if(!root["Content"].isNull())  		memcpy(&p->Content,    root["Content"].asString().c_str()		, sizeof(p->Content));	
	if(!root["URLLink"].isNull())  		memcpy(&p->URLLink,    root["URLLink"].asString().c_str()		, sizeof(p->URLLink));	
	if(!root["MarketID"].isNull())  	memcpy(&p->MarketID,   root["MarketID"].asString().c_str()		, sizeof(p->MarketID));
	return p;
}

CThostFtdcInstrumentStatusField* get_CThostFtdcInstrumentStatusField_Struct(Json::Value& root, CThostFtdcInstrumentStatusField *p)
{
	if(p==NULL) return p;                                                                            
	if(!root["ExchangeID"].isNull())		memcpy(&p->ExchangeID,			root["ExchangeID"].asString().c_str()		, sizeof(p->ExchangeID));
	if(!root["ExchangeInstID"].isNull())	memcpy(&p->ExchangeInstID,      root["ExchangeInstID"].asString().c_str()	, sizeof(p->ExchangeInstID));
	if(!root["SettlementGroupID"].isNull())	memcpy(&p->SettlementGroupID,   root["SettlementGroupID"].asString().c_str(), sizeof(p->SettlementGroupID));
	if(!root["InstrumentID"].isNull())		memcpy(&p->InstrumentID,        root["InstrumentID"].asString().c_str()		, sizeof(p->InstrumentID));
	if(!root["InstrumentStatus"].isNull())	p->InstrumentStatus  =  root["InstrumentStatus"].asString()[0];
	if(!root["TradingSegmentSN"].isNull())	p->TradingSegmentSN  =  root["TradingSegmentSN"].asInt();
	if(!root["EnterTime"].isNull())			memcpy(&p->EnterTime,           root["EnterTime"].asString().c_str()		, sizeof(p->EnterTime));
	if(!root["EnterReason"].isNull())		p->EnterReason       =  root["EnterReason"].asString()[0];
	return p;                                                                              
}
//下单
Json::Value& get_CThostFtdcInputOrderField(Json::Value& root, CThostFtdcInputOrderField *p)
{
	if(p==NULL) return root;
	//root["BrokerID"]			= p->BrokerID;
	root["InvestorID"]			= p->InvestorID;
	root["InstrumentID"]		= p->InstrumentID;
	root["OrderRef"]			= p->OrderRef;
	root["UserID"]				= p->UserID;
	root["OrderPriceType"]		= p->OrderPriceType;
	root["Direction"]			= p->Direction;
	root["CombOffsetFlag"]		= p->CombOffsetFlag;
	root["CombHedgeFlag"]		= p->CombHedgeFlag;
	root["LimitPrice"]			= p->LimitPrice;
	root["VolumeTotalOriginal"] = p->VolumeTotalOriginal;
	root["TimeCondition"]		= p->TimeCondition;
	root["GTDDate"]				= p->GTDDate;
	root["VolumeCondition"]		= p->VolumeCondition;
	root["MinVolume"]			= p->MinVolume;
	root["ContingentCondition"] = p->ContingentCondition;
	root["StopPrice"]			= p->StopPrice;
	root["ForceCloseReason"]	= p->ForceCloseReason;
	root["IsAutoSuspend"]		= p->IsAutoSuspend;
	root["BusinessUnit"]		= p->BusinessUnit;
	root["RequestID"]			= p->RequestID;
	root["UserForceClose"]		= p->UserForceClose;
	root["IsSwapOrder"]			= p->IsSwapOrder;
	root["ExchangeID"]			= p->ExchangeID;
	root["InvestUnitID"]		= p->InvestUnitID;
	root["AccountID"]			= p->AccountID;
	root["CurrencyID"]			= p->CurrencyID;
	root["ClientID"]			= p->ClientID;
	root["IPAddress"]			= p->IPAddress;
	root["MacAddress"]			= p->MacAddress;
	return root;
}

//撤单
Json::Value& get_CThostFtdcInputOrderActionField(Json::Value& root, CThostFtdcInputOrderActionField *p)
{
	if(p==NULL) return root;
	//root["BrokerID"]		=	p->BrokerID;
	root["InvestorID"]		=	p->InvestorID;
	root["OrderActionRef"]	=	p->OrderActionRef;
	root["OrderRef"]		=	p->OrderRef;
	root["RequestID"]		=	p->RequestID;
	root["FrontID"]			=	p->FrontID;
	root["SessionID"]		=	p->SessionID;
	root["ExchangeID"]		=	p->ExchangeID;
	root["OrderSysID"]		=	p->OrderSysID;
	root["ActionFlag"]		=	p->ActionFlag;
	root["LimitPrice"]		=	p->LimitPrice;
	root["VolumeChange"]	=	p->VolumeChange;
	root["UserID"]			=	p->UserID;
	root["InstrumentID"]	=	p->InstrumentID;
	root["InvestUnitID"]	=	p->InvestUnitID;
	root["IPAddress"]		=	p->IPAddress;
	root["MacAddress"]		=	p->MacAddress;
	return root;
}














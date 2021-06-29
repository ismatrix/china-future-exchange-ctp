/*************************************************************************
    > File Name: helper.cpp
    > Created Time: Thu 19 May 2016 08:52:58 PM CST
    > Author:
    > description:
 ************************************************************************/

#include "helper.h"

string account_type = "";

//持仓用
string ctp_posidirection_to_my(char direction)
{
	string tmp;
	switch(direction)
	{
		case THOST_FTDC_PD_Long:	//持仓用
			tmp = CONST_DIRECTION_L; break;
		case THOST_FTDC_PD_Short:
			tmp = CONST_DIRECTION_S; break;
	}
	//LOG_DEBUG("direction=" << direction << " my_direction=" << tmp);
	return tmp;
}

string ctp_direction_to_my(char direction)
{
	string tmp;
	switch(direction)
	{
		case THOST_FTDC_D_Buy:
			tmp = CONST_DIRECTION_BUY; break;
		case THOST_FTDC_D_Sell:
			tmp = CONST_DIRECTION_SELL; break;
	}
	//LOG_DEBUG("direction=" << direction << " my_direction=" << tmp);
	return tmp;
}
string ctp_offsetflag_to_my(char offsetflag)
{
	string tmp;
	switch(offsetflag)
	{
		case THOST_FTDC_OF_Open:
			tmp = CONST_OFFSETFLAG_O; break;
		case THOST_FTDC_OF_Close:
			tmp = CONST_OFFSETFLAG_C; break;
		case THOST_FTDC_OF_CloseToday:
			tmp = CONST_OFFSETFLAG_CT; break;
		case THOST_FTDC_OF_CloseYesterday:
			tmp = CONST_OFFSETFLAG_CY; break;
		case THOST_FTDC_OF_ForceClose:
			tmp = CONST_OFFSETFLAG_FORCE;break;
		case THOST_FTDC_OF_ForceOff:
			tmp = CONST_OFFSETFLAG_FORCEOFF; break;
		case THOST_FTDC_OF_LocalForceClose:
			tmp = CONST_OFFSETFLAG_LFC; break;
	}
	//LOG_DEBUG("offsetflag=" << offsetflag << " my_offsetflag=" << tmp);
	return tmp;
}

string ctp_hedgeflag_to_my(char  hedgeflag)
{
	string tmp;
	switch(hedgeflag)
	{
		case THOST_FTDC_CIDT_Speculation:
			tmp = CONST_HEDGEFLAG_TOUJI; break;
		case THOST_FTDC_BHF_Arbitrage:
			tmp = CONST_HEDGEFLAG_BAOZHI; break;
		case THOST_FTDC_BHF_Hedge:
			tmp = CONST_HEDGEFLAG_TAOLI; break;
	}
	//LOG_DEBUG("hedgeflag=" << hedgeflag << " my_hedgeflag=" << tmp);
	return tmp;
}
string ctp_orderstatus_to_my(char orderstatus)
{
	string tmp;
	switch(orderstatus)
	{
		case THOST_FTDC_OST_AllTraded:				tmp = CONST_ORDERSTATUS_AllTraded; break;				//全部成交
		case THOST_FTDC_OST_PartTradedQueueing:     tmp = CONST_ORDERSTATUS_PartTradedQueueing; break;      //部分成交还在队列中
		case THOST_FTDC_OST_PartTradedNotQueueing:  tmp = CONST_ORDERSTATUS_PartTradedNotQueueing; break;   //部分成交不在队列中
		case THOST_FTDC_OST_NoTradeQueueing:		tmp = CONST_ORDERSTATUS_NoTradeQueueing; break;			//未成交还在队列中
		case THOST_FTDC_OST_NoTradeNotQueueing:     tmp = CONST_ORDERSTATUS_NoTradeNotQueueing; break;      //未成交不在队列中
		case THOST_FTDC_OST_Canceled:				tmp = CONST_ORDERSTATUS_Canceled; break;				//撤单
		case THOST_FTDC_OST_Unknown:				tmp = CONST_ORDERSTATUS_Unknown; break;					//未知
		case THOST_FTDC_OST_NotTouched:             tmp = CONST_ORDERSTATUS_NotTouched; break;              //尚未触发
		case THOST_FTDC_OST_Touched:				tmp = CONST_ORDERSTATUS_Touched; break;					//已触发
		default:    tmp = CONST_ORDERSTATUS_Unknown; break;
	}
	LOG_DEBUG("orderstatus=" << (char)orderstatus << " my=" << tmp);
	return tmp;
}

//持仓和下单参数都一样  0  1
string st_posidirection_to_my(char direction)
{
	string tmp;
	switch(direction)
	{
		case '0':
			tmp = CONST_DIRECTION_L; break;
		case '1':
			tmp = CONST_DIRECTION_S; break;
	}
	return tmp;
}

//持仓和下单参数都一样  0  1
string st_direction_to_my(char direction)
{
	string tmp;
	switch(direction)
	{
		case '0':
			tmp = CONST_DIRECTION_BUY; break;
		case '1':
			tmp = CONST_DIRECTION_SELL; break;
	}
	return tmp;
}
string st_offsetflag_to_my(char offsetflag)
{
	string tmp;
	switch(offsetflag)
	{
		case '0':
			tmp = CONST_OFFSETFLAG_O; break;
		case '1':
			tmp = CONST_OFFSETFLAG_C; break;
		case '2':
			tmp = CONST_OFFSETFLAG_CT; break;
	}
	return tmp;
}
string st_hedgeflag_to_my(char  hedgeflag)
{
	string tmp;
	switch(hedgeflag)
	{
		case '0':
			tmp = CONST_HEDGEFLAG_TOUJI; break;
		case '1':
			tmp = CONST_HEDGEFLAG_BAOZHI; break;
		case '2':
			tmp = CONST_HEDGEFLAG_TAOLI; break;
	}
	return tmp;
}

string st_marketcode_to_my(char marketcode)
{
	string tmp;
	switch(marketcode)
	{
		case '3':
			tmp = CONST_MARKET_CFFEX; break;
		case '4':
			tmp = CONST_MARKET_SHFE; break;
		case '5':
			tmp = CONST_MARKET_CZCE; break;
		case '6':
			tmp = CONST_MARKET_DCE; break;
	}
	return tmp;
}

string   st_orderstatus_to_my(char orderstatus)
{
	string tmp;
	switch(orderstatus)
	{
		case		0: tmp = CONST_ORDERSTATUS_NotTouched; break;				//未报
		case		1: tmp = CONST_ORDERSTATUS_NotTouched; break;				//正报
		case		2: tmp = CONST_ORDERSTATUS_Touched; break;				//已报
		case		3: tmp = CONST_ORDERSTATUS_NoTradeQueueing; break;        //已报待撤
		case		4: tmp = CONST_ORDERSTATUS_NoTradeNotQueueing; break;     //部成待撤
		case		5: tmp = CONST_ORDERSTATUS_PartTradedNotQueueing; break;  //部撤
		case		6: tmp = CONST_ORDERSTATUS_Canceled; break;				//场外撤单
		case		7: tmp = CONST_ORDERSTATUS_PartTradedQueueing; break;     //部成
		case		8: tmp = CONST_ORDERSTATUS_AllTraded; break;              //已成
		case		9: tmp = CONST_ORDERSTATUS_Invalid; break;				//废单
		default		   : tmp = CONST_ORDERSTATUS_Unknown; break;				//未知
	}
	return tmp;
}

uint8_t	my_direction_to_ctp(string direction)
{
	uint8_t tmp;
	if(direction == CONST_DIRECTION_BUY) tmp = THOST_FTDC_D_Buy;
	if(direction == CONST_DIRECTION_SELL) tmp = THOST_FTDC_D_Sell;
	return tmp;
}

uint8_t	my_posidirection_to_ctp(string direction)
{
	uint8_t tmp;
	if(direction == CONST_DIRECTION_L) tmp = THOST_FTDC_PD_Long;
	if(direction == CONST_DIRECTION_S) tmp = THOST_FTDC_PD_Short;
	return tmp;
}


uint8_t	my_offsetflag_to_ctp(string offsetflag)
{
	uint8_t tmp;
	if(offsetflag == CONST_OFFSETFLAG_O) tmp = THOST_FTDC_OF_Open;
	if(offsetflag == CONST_OFFSETFLAG_C) tmp = THOST_FTDC_OF_Close;
	if(offsetflag == CONST_OFFSETFLAG_CT) tmp = THOST_FTDC_OF_CloseToday;
	if(offsetflag == CONST_OFFSETFLAG_CY) tmp = THOST_FTDC_OF_CloseYesterday;
	return tmp;
}
uint8_t	my_hedgeflag_to_ctp(string hedgeflag)
{
	uint8_t tmp;
	if(CONST_HEDGEFLAG_TOUJI == hedgeflag) tmp = THOST_FTDC_CIDT_Speculation;
	if(CONST_HEDGEFLAG_TAOLI == hedgeflag) tmp = THOST_FTDC_BHF_Arbitrage;
	if( CONST_HEDGEFLAG_BAOZHI == hedgeflag) tmp = THOST_FTDC_BHF_Hedge;
	return tmp;
}

uint8_t my_orderstatus_to_ctp(string orderstatus)
{
	uint8_t tmp;
	if(orderstatus.size() > 0) tmp = orderstatus[0];
	if(orderstatus == CONST_ORDERSTATUS_AllTraded				) tmp = THOST_FTDC_OST_AllTraded;
	if(orderstatus == CONST_ORDERSTATUS_PartTradedQueueing		) tmp = THOST_FTDC_OST_PartTradedQueueing;
	if(orderstatus == CONST_ORDERSTATUS_PartTradedNotQueueing	) tmp = THOST_FTDC_OST_PartTradedNotQueueing;
	if(orderstatus == CONST_ORDERSTATUS_NoTradeQueueing			) tmp = THOST_FTDC_OST_NoTradeQueueing;
	if(orderstatus == CONST_ORDERSTATUS_NoTradeNotQueueing		) tmp = THOST_FTDC_OST_NoTradeNotQueueing;
	if(orderstatus == CONST_ORDERSTATUS_Canceled				) tmp = THOST_FTDC_OST_Canceled;
	if(orderstatus == CONST_ORDERSTATUS_Unknown					) tmp = THOST_FTDC_OST_Unknown;
	if(orderstatus == CONST_ORDERSTATUS_NotTouched				) tmp = THOST_FTDC_OST_NotTouched;
	if(orderstatus == CONST_ORDERSTATUS_Touched					) tmp = THOST_FTDC_OST_Touched;
	return tmp;
}

uint8_t	my_posidirection_to_st(string direction)
{
	uint8_t tmp;
	if(direction == CONST_DIRECTION_L) tmp = '0';
	if(direction == CONST_DIRECTION_S) tmp = '1';
	return tmp;
}

uint8_t	my_direction_to_st(string direction)
{
	uint8_t tmp;
	if(direction == CONST_DIRECTION_BUY) tmp = '0';
	if(direction == CONST_DIRECTION_SELL) tmp = '1';
	return tmp;
}
uint8_t	my_offsetflag_to_st(string offsetflag)
{
	uint8_t tmp;
	if(offsetflag == CONST_OFFSETFLAG_O) tmp = '0';
	if(offsetflag == CONST_OFFSETFLAG_C) tmp = '1';
	if(offsetflag == CONST_OFFSETFLAG_CT) tmp = '2';
	return tmp;
}
uint8_t  my_hedgeflag_to_st(string hedgeflag)
{
	uint8_t tmp;
	if(CONST_HEDGEFLAG_TOUJI == hedgeflag) tmp ='0';
	if(CONST_HEDGEFLAG_BAOZHI == hedgeflag) tmp = '1';
	if(CONST_HEDGEFLAG_TAOLI == hedgeflag) tmp = '2';
	return tmp;
}

uint8_t  my_marketcode_to_st(string marketcode)
{
	uint8_t tmp;
	if(marketcode == CONST_MARKET_DCE) tmp = '6';
	if(marketcode == CONST_MARKET_CZCE) tmp = '5';
	if(marketcode == CONST_MARKET_SHFE) tmp = '4';
	if(marketcode == CONST_MARKET_CFFEX) tmp = '3';
	return tmp;
}

uint8_t  my_orderstatus_to_st(string orderstatus)
{
	uint8_t tmp;
	if(orderstatus.size() > 0) tmp = orderstatus[0];
	if(orderstatus == CONST_ORDERSTATUS_NotTouched			 )	tmp = 0;
	if(orderstatus == CONST_ORDERSTATUS_Touched				 )	tmp = 2;
	if(orderstatus == CONST_ORDERSTATUS_NoTradeQueueing      )	tmp = 3;
	if(orderstatus == CONST_ORDERSTATUS_NoTradeNotQueueing   )	tmp = 4;
	if(orderstatus == CONST_ORDERSTATUS_PartTradedNotQueueing)	tmp = 5;
	if(orderstatus == CONST_ORDERSTATUS_Canceled             )	tmp = 6;
	if(orderstatus == CONST_ORDERSTATUS_PartTradedQueueing   )	tmp = 7;
	if(orderstatus == CONST_ORDERSTATUS_AllTraded            )	tmp = 8;
	if(orderstatus == CONST_ORDERSTATUS_Invalid              )	tmp = 9;
	return tmp;
}
void init(::CM::Done* done)
{
	done->price = 0;
	done->volume = 0;
	//done->seq	= 0;
}

void init(::CM::Account* account)
{
	account->capital		= 0;
//	account->cashin		    = 0;
	account->prebalance     = 0;
    account->premargin      = 0;
    account->balance        = 0;
    account->available      = 0;
    account->margin         = 0;
    account->incap          = 0;
    account->outcap         = 0;
    account->commission = 0;
    account->closeprofit    = 0;
    account->positionprofit = 0;
}

void init(::CM::Position* position)
{
	//position->cashin		     = 0;
	position->preposition    = 0;
    position->preholdposition= 0;
    position->todayholdposition= 0;
    position->position       = 0;
    position->openvolume     = 0;
    position->closevolume    = 0;
    position->openamount     = 0;
    position->closeamount    = 0;
    position->opencost       = 0;
    position->closeprofitbytrade       = 0;
    position->todayopenaverage       = 0;
    position->positioncost   = 0;
    position->premargin      = 0;
    position->margin         = 0;
    //position->opencommission = 0;
    //position->closecommission= 0;
    position->commission= 0;
    position->closeprofit    = 0;
    position->positionprofit = 0;
    position->totalprofile   = 0;
    position->positiondate   = '1';
}

void init(::CM::Order* order)
{
	order->price		= 0;
	order->volume		= 0;
	order->volumetraded = 0;
	order->oerrno		= 0;
	//order->seq			= 0;
}


//done 里面的相关的都是string    convert时需要将char转换为string
//需要统一ctp sungard的char    到done中
//增加行情的json和struct之间转换

bool convert_ctp_account(struct ::CM::Account* account, CThostFtdcTradingAccountField *ctp_account)
{
	if(account == NULL || ctp_account == NULL) return false;
	init(account);
	account->tradingday      =  ctp_account->TradingDay      ;    //交易日              ctp.TradingDay
    account->fundid          =  ctp_account->AccountID       ;    //账户标识            ctp.AccountID/st.cell_id
    account->capital		 =	ctp_account->Balance-ctp_account->PositionProfit;	//	ctp.balance-ctp.positionprofit / st.deposite
	account->prebalance      =  ctp_account->PreBalance      ;    //昨日余额            ctp.PreBalance/st.dthis_bal
    account->premargin       =  ctp_account->PreMargin       ;    //上次占用的保证金    ctp.PreMargin/st.null
	//account->cashin		     =  ctp_account->CashIn          ;    //资金差额

    // account->requestid       =  ctp_account->requestid       ;    //接口请求标识
    account->balance         =  ctp_account->Balance         ;    //期货结算准备金      ctp.Balance/st.deposite
    account->available       =  ctp_account->Available       ;    //可用资金            ctp.Available/st.available
    account->margin          =  ctp_account->CurrMargin      ;    //当前保证金总额      ctp.CurrMargin/st.margin
    account->incap           =  ctp_account->Deposit         ;    //入金金额            ctp.Deposit/st.incap
    account->outcap          =  ctp_account->Withdraw        ;    //出金金额            ctp.Withdraw/st.outcap
    account->commission		 =  ctp_account->Commission  ;			//开仓手续费  这里给了一个总的手续费
    account->closeprofit     =  ctp_account->CloseProfit     ;    //盯日平仓盈亏合计
    account->positionprofit  =  ctp_account->PositionProfit  ;    //盯日持仓盈亏合计
    account->updatedate      =   stamptostr()                  ;    //信息更新日期
	return false;
}

bool convert_ctp_position(struct ::CM::Position* position, CThostFtdcInvestorPositionField *ctp_position, double todayopenaverage)
{
	if(position == NULL || ctp_position == NULL) return false;
	init(position);
	position->tradingday       =  ctp_position->TradingDay     ;    //交易日              ctp.TradingDay
    position->fundid           =  ctp_position->InvestorID     ;    //账户标识            ctp.InvestorID/st.cell_id
    //position->brokerid         =  ctp_position->BrokerID       ;    //期货经纪公司标识    ctp.BrokerID/st.cell_id
    // position->requestid        =  ctp_position->requestid      ;    //ctp接口请求标识
    // position->tradeid          =  ctp_position->tradeid        ;    //成交编号            ::CM::Done中的成交编号
    position->instrumentid     =  ctp_position->InstrumentID   ;    //合约代码            ctp.InstrumentID/st.sec_code
    position->direction        =  ctp_posidirection_to_my(ctp_position->PosiDirection)  ;    //多空方向            ctp.PosiDirection/st.bs
    position->hedgeflag        =  ctp_hedgeflag_to_my(ctp_position->HedgeFlag)      ;    //投机套保标志        ctp.HedgeFlag/st.hedge_flag
	//position->cashin		   =  ctp_position->CashIn          ;    //资金差额

    position->position         =  ctp_position->Position       ;    //今日总持仓          ctp.Position/st.avail_vol
	position->todayholdposition=  ctp_position->TodayPosition  ;	//今日余仓
	position->preposition      =  ctp_position->YdPosition     ;    //昨日总持仓          ctp.YdPosition/st.pre_total_vol
    position->preholdposition  =  ctp_position->Position - ctp_position->TodayPosition;    //昨日余仓 ctp.POSITION-ctp.TODAYPOSITION/st.PRE_REMAIN_VOL

	position->openvolume       =  ctp_position->OpenVolume     ;    //今开仓量            ctp.OpenVolume/st.buy_vol
    position->closevolume      =  ctp_position->CloseVolume    ;    //今平仓量            ctp.CloseVolume/st.sell_vol
    position->openamount       =  ctp_position->OpenAmount     ;    //今开仓金额          ctp.OpenAmount/st.buy_done_amt
    position->closeamount      =  ctp_position->CloseAmount    ;    //今平仓金额          ctp.CloseAmount/st.sell_done_amt
    position->positioncost     =  ctp_position->PositionCost   ;    //持仓成本            ctp.PositionCost/st.total_cost
    position->premargin        =  ctp_position->PreMargin      ;    //昨天占用的保证金    ctp.PreMargin/st.yd_usemargin
    position->margin           =  ctp_position->UseMargin      ;    //当前占用的保证金    ctp.UseMargin/st.total_margin
	position->commission	   =  ctp_position->Commission ;    //开仓手续费-TThostFtdcMoneyType-double   这里全部手续费
    position->closeprofit      =  ctp_position->CloseProfit    ;    //盯日平仓盈亏
    position->positionprofit   =  ctp_position->PositionProfit ;    //盯日持仓盈亏
    // position->totalprofile     =  ctp_position->totalprofile   ;    //累计实现盈亏
    position->updatedate       =  stamptostr()    ;    //最新更新时间
    position->positiondate     =  ctos(ctp_position->PositionDate)    ;    //最新更新时间

    position->opencost         = ctp_position->OpenCost;
    position->closeprofitbytrade= ctp_position->CloseProfitByTrade;
    position->todayopenaverage  = todayopenaverage;
    /*
	#ifndef __ROHON__
    #warn("-----------not rohon-------------------")
		if(ctp_position->OpenVolume > 0) position->opencost        =  ctp_position->OpenAmount / ctp_position->OpenVolume;    //今开仓均价  (ctp.OpenAmount/ctp.OpenVolume) / (st.buy_done_amt/st.buy_vol)
	#else
    #warn("-----------rohon-------------------")
		if(ctp_position->TodayPosition > 0) position->opencost        =  ctp_position->OpenAmount / ctp_position->TodayPosition;    //今开仓均价  (ctp.OpenAmount/ctp.OpenVolume) / (st.buy_done_amt/st.buy_vol)
	#endif
	*/

	return false;
}
double get_todayopenaverage_ctp(CThostFtdcInvestorPositionField *ctp_position)
{
    if(ctp_position->OpenVolume > 0) return  ctp_position->OpenAmount / ctp_position->OpenVolume;
    return 0;
}


double get_todayopenaverage_rohon(CThostFtdcInvestorPositionField *ctp_position)
{
	if(ctp_position->TodayPosition > 0) return   ctp_position->OpenAmount / ctp_position->TodayPosition;    //今开仓均价  (ctp.OpenAmount/ctp.OpenVolume) / (st.buy_done_amt/st.buy_vol)
    return 0;
}

bool convert_ctp_done(struct ::CM::Done* done, CThostFtdcTradeField *ctp_done)
{
	if(done == NULL || ctp_done == NULL) return false;
	init(done);
	done->tradingday     =  ctp_done->TradingDay   ;    //交易日             ctp.TradingDay/系统维护
    done->fundid         =  ctp_done->InvestorID   ;    //基金标识，         ctp.InvestorID/st.cell_id

	//下单时里面填写的是requestid 对应order done信息时用。 add附加信息时候查询数据库用到
	done->requestid      =  ctp_done->OrderRef    ;    //接口请求标识

	//done->brokerid       =  ctp_done->BrokerID     ;    //期货经纪公司标识   ctp.BrokerID/st.cell_id
    done->orderid        =  ctp_done->OrderSysID   ;    //委托报单编号       ctp.OrderSysID/st.order_no
    done->tradeid        =  ctp_done->TradeID      ;    //成交编号           ctp.TradeID/st.trade_id
    done->exchangeid     =  ctp_done->ExchangeID   ;    //交易所代码         ctp.ExchangeID/st.-1
    done->instrumentid   =  ctp_done->InstrumentID ;    //合约代码           ctp.InstrumentID/st.sec_code
    done->direction      =  ctp_direction_to_my(ctp_done->Direction)    ;    //买卖方向           ctp.Direction/st.bs
    done->offsetflag     =  ctp_offsetflag_to_my(ctp_done->OffsetFlag)   ;    //开平标志           ctp.OffsetFlag/st.offset_flag
    done->hedgeflag      =  ctp_hedgeflag_to_my(ctp_done->HedgeFlag)    ;    //投机套保标志       ctp.HedgeFlag/st.hedge_flag
    done->price          =  ctp_done->Price        ;    //成交价格           ctp.Price/st.done_price
    done->volume         =  ctp_done->Volume       ;    //成交数量           ctp.Volume/st.done_vol
    done->tradedate      =  ctp_done->TradeDate    ;    //成交时期yyyymmdd   ctp.TradeDate/st.done_date
    done->tradetime      =  ctp_done->TradeTime    ;    //成交时间hh24miss   ctp.TradeTime/st.done_time
	done->updatedate     =  stamptostr()             ;    //纪录最新更新时间   stamptostr()
	return false;
}


bool convert_ctp_order(struct ::CM::Order* order, CThostFtdcOrderField *ctp_order)
{
	if(order == NULL || ctp_order == NULL) return false;
	init(order);
	order->frontid		   =	toa(ctp_order->FrontID)			;
	order->sessionid	   =	toa(ctp_order->SessionID)		;
	order->exchangeid      =	ctp_order->ExchangeID			;
	order->privateno	   =	ctp_order->OrderLocalID			;
	order->orderid         =	ctp_order->OrderSysID			;

	char buf[32];
	memcpy(buf, ctp_order->OrderLocalID, sizeof(ctp_order->OrderLocalID));
	buf[sizeof(ctp_order->OrderLocalID)] = '\0';
	RemoveChar(buf, ' ');
	order->privateno = buf;

	order->tradingday      =	ctp_order->TradingDay			;
	order->fundid          =	ctp_order->InvestorID			;
	//order->brokerid        =	ctp_order->BrokerID				;
	order->requestid       =	toa(ctp_order->RequestID)		;
	order->instrumentid    =	ctp_order->InstrumentID			;
	order->direction       =	ctp_direction_to_my(ctp_order->Direction)		;
	order->offsetflag      =	ctp_offsetflag_to_my(ctp_order->CombOffsetFlag[0])	;
	order->hedgeflag       =	ctp_hedgeflag_to_my(ctp_order->CombHedgeFlag[0])	;
	order->price           =	ctp_order->LimitPrice			;
	order->volume          =	ctp_order->VolumeTotalOriginal	;
	order->ordertype       =	ctos(ctp_order->OrderType)		;
	order->orderstatus     =	ctp_orderstatus_to_my(ctp_order->OrderStatus)	;
	order->volumetraded    =	ctp_order->VolumeTraded			;
	order->orderdate	   =	ctp_order->InsertDate			;
	order->ordertime       =	ctp_order->InsertTime;
	order->oerrmsg		   =	ctp_order->StatusMsg			;

	order->donetype		   =	ctp_order->TimeCondition;
	order->pricetype		   =	ctp_order->OrderPriceType;
	order->canceltime	   =	ctp_order->CancelTime;

	return true;
}

//转化下单结构到订单
bool convert_ctp_doorder_order(struct ::CM::Order* order, CThostFtdcInputOrderField* ctp_order)
{
	if(order == NULL || ctp_order == NULL) return false;

	init(order);
	//order->frontid		   =
	//order->sessionid	   =
	order->exchangeid      =	ctp_order->ExchangeID;

	order->privateno	   =	-1; //ctp_order->OrderRef;
	//order->orderid         =	ctp_order->
	//order->tradingday      =
	order->fundid          =	ctp_order->InvestorID;
	//order->brokerid        =	ctp_order->BrokerID;
	order->requestid       =	toa(ctp_order->RequestID);
	order->instrumentid    =	ctp_order->InstrumentID;
	order->direction       =	ctp_direction_to_my(ctp_order->Direction);
	order->offsetflag      =	ctp_offsetflag_to_my(ctp_order->CombOffsetFlag[0]);
	order->hedgeflag       =	ctp_hedgeflag_to_my(ctp_order->CombHedgeFlag[0]);
	order->price           =	ctp_order->LimitPrice;
	order->volume          =	ctp_order->VolumeTotalOriginal;
	//order->ordertype       =	ctp_order->
	order->orderstatus     =	"报单中"; //正在报单
	//order->volumetraded    =
	//order->insertdatetime  =
	//order->ordertime       =
	//order->updatetime      =
	return true;
}

//转化撤单结构到订单
bool convert_ctp_cancleorder_order(struct ::CM::Order* order, CThostFtdcInputOrderActionField* ctp_order)
{
	if(order == NULL || ctp_order == NULL) return false;

	init(order);
	order->frontid		   =	ctp_order->FrontID;
	order->sessionid	   =	ctp_order->SessionID;
	order->exchangeid      =	ctp_order->ExchangeID;
	order->privateno	   =	-1; //ctp_order->OrderRef;
	//order->orderid         =	ctp_order->OrderSysID;
	//order->tradingday      =
	order->fundid          =	ctp_order->InvestorID;
	//order->brokerid        =	ctp_order->BrokerID;
	order->requestid       =	toa(ctp_order->RequestID);
	order->instrumentid    =	ctp_order->InstrumentID;
	//order->direction       =	ctp_direction_to_my(ctp_order->Direction);
	//order->offsetflag      =	ctp_offsetflag_to_my(ctp_order->CombOffsetFlag[0]);
	//order->hedgeflag       =	ctp_hedgeflag_to_my(ctp_order->CombHedgeFlag[0]);
	order->price           =	ctp_order->LimitPrice;
	//order->volume          =	ctp_order->VolumeTotalOriginal;
	//order->ordertype       =	ctp_order->
	order->orderstatus     =	"撤单中"; //场外撤单
	//order->volumetraded    =
	//order->insertdatetime  =
	//order->ordertime       =
	//order->updatetime      =
	//order->oerrmsg			=  ctp_order->StatusMsg;

	return true;
}


//转化撤单结构到订单
bool convert_ctp_cancleorder_order(struct ::CM::Order* order, CThostFtdcOrderActionField* ctp_order)
{
	if(order == NULL || ctp_order == NULL) return false;

	init(order);
	order->frontid		   =	ctp_order->FrontID;
	order->sessionid	   =	ctp_order->SessionID;
	order->exchangeid      =	ctp_order->ExchangeID;
	order->privateno	   =	-1; //ctp_order->OrderRef;
	order->orderid         =	ctp_order->OrderSysID;
	//order->tradingday      =
	order->fundid          =	ctp_order->InvestorID;
	//order->brokerid        =	ctp_order->BrokerID;
	order->requestid       =	toa(ctp_order->RequestID);
	order->instrumentid    =	ctp_order->InstrumentID;
	//order->direction       =	ctp_direction_to_my(ctp_order->Direction);
	//order->offsetflag      =	ctp_offsetflag_to_my(ctp_order->CombOffsetFlag[0]);
	//order->hedgeflag       =	ctp_hedgeflag_to_my(ctp_order->CombHedgeFlag[0]);
	order->price           =	ctp_order->LimitPrice;
	//order->volume          =	ctp_order->VolumeTotalOriginal;
	//order->ordertype       =	ctp_order->
	order->orderstatus     =	"撤单中"; //场外撤单
	//order->volumetraded    =
	//order->insertdatetime  =
	//order->ordertime       =
	//order->updatetime      =
	order->oerrmsg			=  ctp_order->StatusMsg;

	return true;
}


bool convert_st_account(struct ::CM::Account* account, STAccountInfo *st_account)
{
	if(account == NULL || st_account == NULL) return false;

	init(account);
	// account->tradingday      =  tradingday                 ;    //交易日              ctp.TradingDay
	account->fundid          =  toa(st_account->cell_id)        ;    //账户标识            ctp.AccountID/st.cell_id
	account->capital		 =	st_account->deposite + st_account->margin;				;
	//account->prebalance      =  st_account->dthis_bal      ;    //昨日余额            ctp.PreBalance/st.deposite + st.margin
	// account->premargin       =  st_account->               ;    //上次占用的保证金    ctp.PreMargin/st.null

	//更新昨天权益和保证金 切换交易日的时候更新
	//account->prebalance      =  st_account->deposite + st_account->margin + get_commission(fundid)      ;    //昨日余额            ctp.PreBalance/st.deposite + st.margin
	//account->premargin       =  st_account->margin				               ;    //上次占用的保证金    ctp.PreMargin/st.null

	//convert_st_account 之后更新
	//account->balance		 = st_account->deposite + st_account->margin; //st_account->deposite + st_account->margin + get_commission(fundid);

	// account->requestid       =  st_account->requestid      ;    //接口请求标识
	account->available       =  st_account->available      ;    //可用资金            ctp.Available/st.available
	account->margin          =  st_account->margin         ;    //当前保证金总额      ctp.CurrMargin/st.margin
	account->incap           =  st_account->incap          ;    //入金金额            ctp.Deposit/st.incap
	account->outcap          =  st_account->outcap         ;    //出金金额            ctp.Withdraw/st.outcap
	//这两个在持仓改变的时候计算更新
	// account->commission  =  st_account->commission ;    //开仓手续费
	// account->closeprofit     =  st_account->closeprofit    ;    //盯日平仓盈亏合计
	// account->positionprofit  =  st_account->positionprofit ;    //盯日持仓盈亏合计
	account->updatedate      =   stamptostr()                 ;    //信息更新日期
	return true;
}

bool convert_st_position(struct ::CM::Position* position, STPositionInfo *st_position)
{
	if(position == NULL || st_position == NULL) return false;
	init(position);
    // position->tradingday       =                                ;    //交易日              ctp.TradingDay
    position->fundid           =  toa(st_position->cell_id)         ;    //账户标识            ctp.InvestorID/st.cell_id
    //position->brokerid         =  toa(st_position->cell_id)         ;    //期货经纪公司标识    ctp.BrokerID/st.cell_id
    // position->requestid        =  st_position->requestid       ;    //ctp接口请求标识
    //position->tradeid          =  st_position->tradeid         ;    //成交编号            ::CM::Done中的成交编号
    position->instrumentid     =  st_position->sec_code        ;    //合约代码            ctp.InstrumentID/st.sec_code
    position->direction        =  st_posidirection_to_my(st_position->PosiDirection)   ;    //多空方向            ctp.PosiDirection/st.PosiDirection
    position->hedgeflag        =  st_hedgeflag_to_my(st_position->hedgingflag)	   ;    //投机套保标志        ctp.HedgeFlag/st.hedgingflag

	position->preposition      =  st_position->pre_total_vol   ;    //昨日总持仓          ctp.YdPosition/st.pre_total_vol
    position->preholdposition  =  st_position->pre_remain_vol  ;    //昨日余仓
    position->position         =  st_position->total_vol       ;    //今日总持仓          ctp.Position/st.total_vol
    position->todayholdposition=  st_position->total_vol - st_position->pre_remain_vol;	// ctp.TODAYPOSITION/st.total_vol-st.PRE_REMAIN_VOL

	position->openvolume       =  st_position->buy_vol         ;    //今开仓量            ctp.OpenVolume/st.buy_vol
    position->closevolume      =  st_position->sell_vol        ;    //今平仓量            ctp.CloseVolume/st.sell_vol
    position->openamount       =  st_position->buy_done_amt    ;    //今开仓金额          ctp.OpenAmount/st.buy_done_amt
    position->closeamount      =  st_position->sell_done_amt   ;    //今平仓金额          ctp.CloseAmount/st.sell_done_amt
    position->positioncost     =  st_position->total_cost      ;    //持仓成本            ctp.PositionCost/st.total_cost
    position->premargin        =  st_position->yd_usemargin    ;    //昨天占用的保证金    ctp.PreMargin/st.yd_usemargin
    position->margin           =  st_position->total_margin    ;    //当前占用的保证金    ctp.UseMargin/st.total_margin
    //position->opencommission   =  st_position->OpenCommission  ;    //开仓手续费-TThostFtdcMoneyType-double
    //position->closecommission  =  st_position->CloseCommission ;    //平仓手续费-TThostFtdcMoneyType-double
    position->commission	   =  st_position->commission ;    //平仓手续费-TThostFtdcMoneyType-double
    position->closeprofit      =  st_position->realized_profit     ;    //盯日平仓盈亏  也就是已实现盈亏吧
    // position->positionprofit   =  st_position->positionprofit  ;    //盯日持仓盈亏
    position->totalprofile     =  st_position->total_profit      ;    //累计实现盈亏
    position->updatedate       =  stamptostr()                   ;    //最新更新时间

	if(st_position->buy_vol>0) position->todayopenaverage         =  st_position->buy_done_amt/st_position->buy_vol   ;    //今开仓均价          (ctp.OpenAmount/ctp.OpenVolume) / (st.buy_done_amt/st.buy_vol)

	return true;
}

bool convert_st_done(struct ::CM::Done* done, STDoneInfo *st_done)
{
	if(done == NULL || st_done == NULL) return false;
	init(done);
	// done->tradingday     =  TradingDay             ;    //交易日             ctp.TradingDay/系统维护
    done->fundid         =  toa(st_done->cell_id)      ;    //基金标识，         ctp.InvestorID/st.cell_id

	//下单时的请求id
	done->requestid      =  toa(st_done->private_no)    ;    //接口请求标识

	//done->brokerid       =  toa(st_done->cell_id)      ;    //期货经纪公司标识   ctp.BrokerID/st.cell_id
    done->orderid        =  toa(st_done->order_no)     ;    //委托报单编号       ctp.OrderSysID/st.order_no
    done->tradeid        =  st_done->trade_id     ;    //成交编号           ctp.TradeID/st.trade_id
    done->exchangeid     =  st_marketcode_to_my(st_done->market_code)      ;    //交易所代码         ctp.ExchangeID/st.-1
    done->instrumentid   =  st_done->sec_code     ;    //合约代码           ctp.InstrumentID/st.sec_code
    done->direction      =  st_direction_to_my(st_done->bs)           ;    //买卖方向           ctp.Direction/st.bs
    done->offsetflag     =  st_offsetflag_to_my(st_done->offset_flag)  ;    //开平标志           ctp.OffsetFlag/st.offset_flag
    done->hedgeflag      =  st_hedgeflag_to_my(st_done->hedge_flag)   ;    //投机套保标志       ctp.HedgeFlag/st.hedge_flag
    done->price          =  st_done->done_price   ;    //成交价格           ctp.Price/st.done_price
    done->volume         =  st_done->done_vol     ;    //成交数量           ctp.Volume/st.done_vol
    done->tradedate      =  toa(st_done->done_date)    ;    //成交时期yyyymmdd   ctp.TradeDate/st.done_date

	char buf[32]={0};
	snprintf(buf, sizeof(buf), "%02d%02d%02d", st_done->done_time/10000, st_done->done_time/100%100, st_done->done_time%100);
	done->tradetime      =  buf    ;    //成交时间hh24miss   ctp.TradeTime/st.done_time

	done->updatedate     =  stamptostr()            ;    //纪录最新更新时间   stamptostr()
	return true;
}

bool convert_st_order(struct ::CM::Order* order, STOrderInfo *st_order)
{
	if(order == NULL || st_order == NULL) return false;
	init(order);

	//兼容其他平台
	if(st_order->order.private_no <=0 ) st_order->order.private_no = st_order->order_no;

	//下单时的请求id
	order->requestid       =	toa(st_order->order.private_no)				;

	order->sessionid	   =	toa(st_order->session_id)		;
	order->privateno	   =	toa(st_order->order.private_no)	;
	order->exchangeid      =	st_marketcode_to_my(st_order->order.market_code)		;
	order->orderid         =	toa(st_order->order_no)				;
	//order->tradingday      =	st_order->TradingDay			;
	order->fundid          =	toa(st_order->order.cell_id)	;
	order->brokerid        =	toa(st_order->order.cell_id)	;

	order->instrumentid    =	st_order->order.sec_code		;
	order->direction       =	st_direction_to_my(st_order->order.bs)				;
	order->offsetflag      =	st_offsetflag_to_my(st_order->order.offset_flag)		;
	order->hedgeflag       =	st_hedgeflag_to_my(st_order->order.hedge_flag)		;
	order->price           =	st_order->order.price			;
	order->volume          =	st_order->order.order_vol		;
	order->ordertype       =	ctos(st_order->order.OrderType)	;	//市价  限价等信息
	order->orderstatus     =	st_orderstatus_to_my(st_order->order_status)		;
	order->volumetraded    =	st_order->done_vol				;

	order->orderdate  =	toa(st_order->order_date)			;		//委托时间
	if(st_order->order_date < 20160922 || st_order->order_date > 21000000) order->orderdate = "";

	order->donetype         =    st_order->order.doneprop;
	order->pricetype        =    st_order->order.market_order_flag;
	order->canceltime       =    toa(st_order->cancel_time);
	if(st_order->cancel_time == 0) order->canceltime       =    "";

	order->ordertime = toa(st_order->order_time);
	/*
	string tmp = toa(st_order->order_time);
	for(uint32_t i=2; i<=tmp.size(); i=i+2)
	{
		if(i!=2)  order->ordertime += ":";
		order->ordertime += tmp.substr(i-2, 2);
	}
	*/
	if(st_order->error_no !=0 )
	{
		order->oerrmsg		   =	st_order->err_msg			;
		order->oerrno		   =	-1							;
	}
	if(st_order->cancel_error_no != 0 )
	{
		order->oerrmsg		   =	st_order->cancel_err_msg	;
		order->oerrno		   =	-2							;
	}
	return  true;
}

Json::Value& get_json_doorder(::CM::DoOrder* p, Json::Value& root)
{
	if(p == NULL) return root;
	root["fundid"]			=	p->fundid;
	root["brokerid"]		=	p->brokerid;
	root["instrumentid"]	=	p->instrumentid;
	root["ordertype"]		=	p->ordertype;
	root["direction"]		=	p->direction;
	root["offsetflag"]		=	p->offsetflag;
	root["hedgeflag"]		=	p->hedgeflag;
	root["price"]			=	p->price;
	root["volume"]			=	p->volume;
	root["donetype"]		=	p->donetype;

	//root["seq"]				=	p->seq;
	//root["parentid"]		=	p->parentid;
	root["strategyid"]		=	p->strategyid;
	root["userid"]			=	p->userid;
	root["signalname"]		=	p->signalname;

	root["requestid"]		=	p->requestid;
	root["tradingday"]		=	p->tradingday;
	return root;
}

::CM::DoOrder*	get_struct_doorder(::CM::DoOrder* p, Json::Value& root)
{
	if(p == NULL) return NULL;

	if(!root["fundid"]        .isNull())    p->fundid		 = root["fundid"].asString();
	if(!root["brokerid"]      .isNull())    p->brokerid      = root["brokerid"].asString();
	if(!root["instrumentid"]  .isNull())    p->instrumentid  = root["instrumentid"].asString();
	if(!root["ordertype"]     .isNull())    p->ordertype     = root["ordertype"].asString();
	if(!root["direction"]     .isNull())    p->direction     = root["direction"].asString();
	if(!root["offsetflag"]    .isNull())    p->offsetflag    = root["offsetflag"].asString();
	if(!root["hedgeflag"]     .isNull())    p->hedgeflag     = root["hedgeflag"].asString();
	if(!root["price"]         .isNull())    p->price         = root["price"].asDouble();
	if(!root["volume"]        .isNull())    p->volume        = root["volume"].asInt();
	if(!root["donetype"]      .isNull())    p->donetype      = root["donetype"].asString();
	//if(!root["parentid"]      .isNull())    p->parentid      = root["parentid"].asString();
	//if(!root["seq"]      .isNull())			p->seq      = root["seq"].asInt();
	if(!root["strategyid"]    .isNull())    p->strategyid    = root["strategyid"].asString();
	if(!root["userid"]        .isNull())    p->userid        = root["userid"].asString();
	if(!root["signalname"]    .isNull())    p->signalname    = root["signalname"].asString();
	if(!root["requestid"]     .isNull())    p->requestid     = root["requestid"].asString();
	if(!root["tradingday"]    .isNull())    p->tradingday    = root["tradingday"].asString();

    LOG_DEBUG("----signalname=" << p->signalname << " ---"<<root["signalname"].asString());
	return p;
}

Json::Value& get_json_order(::CM::Order* p, Json::Value& root)
{
	if(p == NULL) return root;
	root["frontid"]		   =	p->frontid;
	root["sessionid"]	   =	p->sessionid;
	root["privateno"]	   =	p->privateno;
	root["exchangeid"]	   =	p->exchangeid;
	root["orderid"]        =  p->orderid         ;
	//root["tradingday"]		=  p->tradingday      ;
	//root["fundid"]          =  p->fundid          ;
	root["brokerid"]        =  p->brokerid        ;
	root["requestid"]       =  p->requestid       ;
	root["instrumentid"]    =  p->instrumentid    ;
	root["direction"]       =  p->direction       ;
	root["offsetflag"]      =  p->offsetflag      ;
	root["hedgeflag"]       =  p->hedgeflag       ;
	root["price"]           =  p->price           ;
	root["volume"]          =  p->volume          ;
	root["ordertype"]       =  p->ordertype       ;
	root["orderstatus"]     =  p->orderstatus     ;
	root["donevol"]    =  p->volumetraded    ;
	root["orderdate"]		 =  p->orderdate  ;
	root["ordertime"]       =  p->ordertime       ;
	//root["oerrno"]			=  p->oerrno			  ;
	root["statusmsg"]			=  p->oerrmsg		  ;
	root["updatedate"]		= stamp_to_isodate(time(NULL))     ;    //纪录最新更新时间   stamptostr()
	//root["parentid"]		=	p->parentid;
	root["strategyid"]		=	p->strategyid;
	root["userid"]			=	p->userid;
	root["signalname"]		=	p->signalname;

	root["donetype"]		=   p->donetype;
	root["pricetype"]		=	p->pricetype;
	root["canceltime"]		=	p->canceltime;

	return root;
}

::CM::Order*	get_struct_order(::CM::Order* p, Json::Value& root)
{
	if(p == NULL) return NULL;
	if(!root["frontid"]			.isNull())  p->frontid      =   root["frontid"].asString();
	if(!root["sessionid"]		.isNull())  p->sessionid    =   root["sessionid"].asString();
	if(!root["privateno"]		.isNull())  p->privateno    =   root["privateno"].asString();
	if(!root["exchangeid"]		.isNull())  p->exchangeid   =   root["exchangeid"].asString();
	if(!root["orderid"]          .isNull())  p->orderid         =   root["orderid"].asString();

	if(!root["tradingday"]		 .isNull())  p->tradingday      =   root["tradingday"].asString();
	if(!root["fundid"]           .isNull())  p->fundid          =   root["fundid"].asString();
	if(!root["brokerid"]         .isNull())  p->brokerid        =   root["brokerid"].asString();
	if(!root["requestid"]        .isNull())  p->requestid       =   root["requestid"].asString();
	if(!root["instrumentid"]     .isNull())  p->instrumentid    =   root["instrumentid"].asString();
	if(!root["direction"]        .isNull())  p->direction       =   root["direction"].asString();
	if(!root["offsetflag"]       .isNull())  p->offsetflag      =   root["offsetflag"].asString();
	if(!root["hedgeflag"]        .isNull())  p->hedgeflag       =   root["hedgeflag"].asString();
	if(!root["price"]            .isNull())  p->price           =   root["price"].asDouble();
	if(!root["volume"]           .isNull())  p->volume          =   root["volume"].asInt();
	if(!root["ordertype"]        .isNull())  p->ordertype       =   root["ordertype"].asString();
	if(!root["orderstatus"]      .isNull())  p->orderstatus     =   root["orderstatus"].asString();
	if(!root["donevol"]     .isNull())  p->volumetraded    =   root["donevol"].asInt();
	if(!root["orderdate"]		 .isNull())  p->orderdate		=   root["orderdate"].asString();
	if(!root["ordertime"]        .isNull())  p->ordertime       =   root["ordertime"].asString();
	//if(!root["oerrno"]			 .isNull())  p->oerrno			=   root["oerrno"].asInt();
	if(!root["statusmsg"]		.isNull())  p->oerrmsg			=   root["statusmsg"].asString();
	//if(!root["updatetime"]       .isNull())  p->updatetime      =   isodate_to_stamp(root["updatedate"]); //root["updatetime"].asString();

	//if(!root["parentid"]      .isNull())    p->parentid      = root["parentid"].asString();
	if(!root["strategyid"]    .isNull())    p->strategyid    = root["strategyid"].asString();
	if(!root["userid"]        .isNull())    p->userid        = root["userid"].asString();
	if(!root["signalname"]    .isNull())    p->signalname    = root["signalname"].asString();

	return p;
}

Json::Value& get_json_done(::CM::Done* p, Json::Value& root)
{
	if(p == NULL) return root;
	//root["tradingday"]			= p->tradingday     ;    //交易日             ctp.TradingDay/系统维护
	//root["fundid"]				= p->fundid         ;    //基金标识，         ctp.InvestorID/st.cell_id
	//root["brokerid"]			= p->brokerid       ;    //期货经纪公司标识   ctp.BrokerID/st.cell_id
	root["privateno"]			= "";
	root["requestid"]			= p->requestid      ;    //接口请求标识
	root["orderid"]				= p->orderid        ;    //委托报单编号       ctp.OrderSysID/st.order_no
	root["tradeid"]				= p->tradeid        ;    //成交编号           ctp.TradeID/st.trade_id
	root["exchangeid"]			= p->exchangeid     ;    //交易所代码         ctp.ExchangeID/st.-1
	root["instrumentid"]		= p->instrumentid   ;    //合约代码           ctp.InstrumentID/st.sec_code
	root["direction"]			= p->direction      ;    //买卖方向           ctp.Direction/st.bs     ('B'买入 'S'卖出)
	root["offsetflag"]			= p->offsetflag     ;    //开平标志           ctp.OffsetFlag/st.offset_flag  ('O'开仓 'C'平仓)
	root["hedgeflag"]			= p->hedgeflag      ;    //投机套保标志       ctp.HedgeFlag/st.hedge_flag    '0' 投机/非备兑   '1' 保值/备兑   '2' 套利
	root["price"]				= p->price          ;    //成交价格           ctp.Price/st.done_price
	root["volume"]				= p->volume         ;    //成交数量           ctp.Volume/st.done_vol
	root["tradedate"]			= p->tradedate      ;    //成交时期yyyymmdd   ctp.TradeDate/st.done_date
	root["tradetime"]			= p->tradetime      ;    //成交时间hh24miss   ctp.TradeTime/st.done_time
	root["updatedate"]			= stamp_to_isodate(time(NULL))     ;    //纪录最新更新时间   stamptostr()

	//root["parentid"]		=	p->parentid;
	root["strategyid"]		=	p->strategyid;
	root["userid"]			=	p->userid;
	root["signalname"]		=	p->signalname;

	return root;
}

::CM::Done* get_struct_done(::CM::Done* p, Json::Value& root)
{
	if(p == NULL) return NULL;
	if(!root["tradingday"].isNull())		p->tradingday     =  root["tradingday"].asString();
	if(!root["fundid"].isNull())			p->fundid         =  root["fundid"].asString();
	if(!root["requestid"].isNull())		    p->requestid      =  root["requestid"].asString();
	if(!root["brokerid"].isNull())		    p->brokerid       =  root["brokerid"].asString();
	if(!root["orderid"].isNull())			p->orderid        =  root["orderid"].asString();
	if(!root["tradeid"].isNull())			p->tradeid        =  root["tradeid"].asString();
	if(!root["exchangeid"].isNull())		p->exchangeid     =  root["exchangeid"].asString();
	if(!root["instrumentid"].isNull())	    p->instrumentid   =  root["instrumentid"].asString();
	if(!root["direction"].isNull())		    p->direction      =  root["direction"].asString();
	if(!root["offsetflag"].isNull())		p->offsetflag     =  root["offsetflag"].asString();
	if(!root["hedgeflag"].isNull())		    p->hedgeflag      =  root["hedgeflag"].asString();
	if(!root["price"].isNull())			    p->price		  =	 root["price"].asDouble();
	if(!root["volume"].isNull())			p->volume		  =	 root["volume"].asInt();
	if(!root["tradedate"].isNull())		    p->tradedate      =  root["tradedate"].asString();
	if(!root["tradetime"].isNull())		    p->tradetime      =  root["tradetime"].asString();
	//if(!root["updatedate"].isNull())		p->updatedate     =  root["updatedate"].asString();

	//if(!root["parentid"]      .isNull())    p->parentid      = root["parentid"].asString();
	if(!root["strategyid"]    .isNull())    p->strategyid    = root["strategyid"].asString();
	if(!root["userid"]        .isNull())    p->userid        = root["userid"].asString();
	if(!root["signalname"]    .isNull())    p->signalname    = root["signalname"].asString();

	return p;
}

Json::Value& get_json_account(::CM::Account* p, Json::Value& root)
{
	if(p == NULL) return root;
	//root["tradingday"]      =  p->tradingday      ;    //交易日              ctp.TradingDay
	//root["fundid"]          =  p->fundid          ;    //账户标识            ctp.AccountID/st.cell_id
	//root["prebalance"]      =  p->prebalance      ;    //昨日余额            ctp.PreBalance/st.dthis_bal
	//root["premargin"]       =  p->premargin       ;    //上次占用的保证金    ctp.PreMargin/st.null

	//root["cashin"]          =  p->cashin          ;    //权利金 资金差额
	root["requestid"]       =  p->requestid       ;    //接口请求标识
	root["capital"]			=  p->capital		  ;	   //资金（不含当日持仓盈亏
	root["balance"]         =  p->balance         ;    //期货结算准备金      ctp.Balance/st.deposite
	root["available"]       =  p->available       ;    //可用资金            ctp.Available/st.available
	root["margin"]          =  p->margin          ;    //当前保证金总额      ctp.CurrMargin/st.margin
	root["incap"]          	=  p->incap           ;    //入金金额            ctp.Deposit/st.incap
	root["outcap"]          =  p->outcap          ;    //出金金额            ctp.Withdraw/st.outcap
	root["commission"]		=  p->commission  ;    //开仓手续费
	root["closeprofit"]		=  p->closeprofit     ;    //盯日平仓盈亏合计

	if(account_type != "sungard") root["positionprofit"]  =  p->positionprofit  ;    //盯日持仓盈亏合计

	root["updatedate"]      =  stamp_to_isodate(time(NULL))      ;    //信息更新日期
	return root;
}

::CM::Account* get_struct_acount(::CM::Account* p, Json::Value& root)
{
	if(p == NULL) return NULL;
	//if(!root["tradingday"].isNull())        p->tradingday      =  root["tradingday"].asString();
	//if(!root["fundid"].isNull())            p->fundid          =  root["fundid"].asString();
	//if(!root["prebalance"].isNull())        p->prebalance      =  root["prebalance"].asDouble();
	//if(!root["premargin"].isNull())         p->premargin       =  root["premargin"].asDouble() ;
	//if(!root["requestid"].isNull())         p->requestid       =  root["requestid"].asString();
	//if(!root["cashin"].isNull())            p->cashin          =  root["cashin"].asDouble()        ;
	if(!root["capital"].isNull())           p->capital         =  root["capital"].asDouble()        ;
	if(!root["balance"].isNull())           p->balance         =  root["balance"].asDouble()        ;
	if(!root["available"].isNull())         p->available       =  root["available"].asDouble()      ;
	if(!root["margin"].isNull())            p->margin          =  root["margin"].asDouble()         ;
	if(!root["incap"].isNull())          	p->incap           =  root["incap"].asDouble()          ;
	if(!root["outcap"].isNull())            p->outcap          =  root["outcap"].asDouble()         ;
	if(!root["commission"].isNull())		p->commission	   =  root["commission"].asDouble()	;
	if(!root["closeprofit"].isNull())		p->closeprofit     =  root["closeprofit"].asDouble()	;
	if(!root["positionprofit"].isNull())    p->positionprofit  =  root["positionprofit"].asDouble() ;
	//if(!root["updatedate"].isNull())        p->updatedate      =  root["updatedate"].asString();
	return p;
}

Json::Value& get_json_position(::CM::Position* p, Json::Value& root)
{
	if(p == NULL) return root;
	//root["tradingday"]			=  p->tradingday      ;    //交易日              ctp.TradingDay
	//root["fundid"]				=  p->fundid          ;    //账户标识            ctp.InvestorID/st.cell_id
	//root["brokerid"]			=  p->brokerid        ;    //期货经纪公司标识    ctp.BrokerID/st.cell_id
	//root["requestid"]			=  p->requestid       ;    //ctp接口请求标识
	//root["cashin"]              =  p->cashin          ;    //权利金 资金差额
	root["instrumentid"]		=  p->instrumentid    ;    //合约代码            ctp.InstrumentID/st.sec_code
	root["direction"]			=  p->direction       ;    //多空方向            ctp.PosiDirection/st.bs
	root["hedgeflag"]			=  p->hedgeflag       ;    //投机套保标志        ctp.HedgeFlag/st.hedge_flag
	root["preposition"]			=  p->preposition     ;    //昨日总持仓          ctp.YdPosition/st.pre_total_vol
	root["preholdposition"]		=  p->preholdposition ;    //昨日余仓
	root["todayholdposition"]	=  p->todayholdposition ;  //今日余仓
	root["position"]			=  p->position        ;    //今日总持仓          ctp.Position/st.avail_vol
	root["openvolume"]			=  p->openvolume      ;    //今开仓量            ctp.OpenVolume/st.buy_vol
	root["closevolume"]			=  p->closevolume     ;    //今平仓量            ctp.CloseVolume/st.sell_vol
	root["openamount"]			=  p->openamount      ;    //今开仓金额          ctp.OpenAmount/st.buy_done_amt
	root["closeamount"]			=  p->closeamount     ;    //今平仓金额          ctp.CloseAmount/st.sell_done_amt
	root["opencost"]			=  p->opencost        ;    //开仓成本          (ctp.OpenAmount/ctp.OpenVolume) /
	root["todayopenaverage"]	=  p->todayopenaverage;    //今开仓均价          (ctp.OpenAmount/ctp.OpenVolume) /
	root["closeprofitbytrade"]	=  p->closeprofitbytrade;  //逐笔平仓盈亏
	root["positioncost"]		=  p->positioncost    ;    //持仓成本            ctp.PositionCost/st.total_cost
	root["premargin"]			=  p->premargin       ;    //昨天占用的保证金    ctp.PreMargin/st.yd_usemargin
	root["margin"]				=  p->margin          ;    //当前占用的保证金    ctp.UseMargin/st.total_margin
	//root["opencommission"]		=  p->opencommission  ;    //开仓手续费-TThostFtdcMoneyType-double
	//root["closecommission"]		=  p->closecommission ;    //平仓手续费-TThostFtdcMoneyType-double
	root["commission"]			=  p->commission ;    //平仓手续费-TThostFtdcMoneyType-double
	root["closeprofit"]			=  p->closeprofit     ;    //盯日平仓盈亏
	root["positionprofit"]		=  p->positionprofit  ;    //盯日持仓盈亏
	root["totalprofile"]		=  p->totalprofile    ;    //累计实现盈亏
	//root["updatedate"]			=  stamp_to_isodate(time(NULL))     ;    //最新更新时间
	return root;
}

::CM::Position* get_struct_position(::CM::Position* p, Json::Value& root)
{
	if(p == NULL) return NULL;
	if(!root["tradingday"].isNull())		p->tradingday      =  root["tradingday"].asString();
	if(!root["fundid"].isNull())			p->fundid          =  root["fundid"].asString();
	//if(!root["brokerid"].isNull())		    p->brokerid        =  root["brokerid"].asString();
	//if(!root["cashin"].isNull())            p->cashin          =  root["cashin"].asDouble()        ;
	if(!root["requestid"].isNull())		    p->requestid       =  root["requestid"].asString();
	if(!root["instrumentid"].isNull())	    p->instrumentid    =  root["instrumentid"].asString();
	if(!root["direction"].isNull())		    p->direction       =  root["direction"].asString();
	if(!root["hedgeflag"].isNull())		    p->hedgeflag       =  root["hedgeflag"].asString();
	if(!root["preposition"].isNull())		p->preposition     =  root["preposition"].asInt()	 ;
	if(!root["preholdposition"].isNull())	p->preholdposition =  root["preholdposition"].asInt();
	if(!root["todayholdposition"].isNull())	p->todayholdposition =  root["todayholdposition"].asInt();
	if(!root["position"].isNull())		    p->position        =  root["position"].asInt()		 ;
	if(!root["openvolume"].isNull())		p->openvolume      =  root["openvolume"].asInt()	 ;
	if(!root["closevolume"].isNull())		p->closevolume     =  root["closevolume"].asInt()	 ;
	if(!root["openamount"].isNull())		p->openamount      =  root["openamount"].asDouble()		;
	if(!root["closeamount"].isNull())		p->closeamount     =  root["closeamount"].asDouble()	;
	if(!root["opencost"].isNull())		    p->opencost        =  root["opencost"].asDouble()		;
	if(!root["todayopenaverage"].isNull())	p->todayopenaverage=  root["todayopenaverage"].asDouble()		;
	if(!root["closeprofitbytrade"].isNull())p->closeprofitbytrade=  root["closeprofitbytrade"].asDouble()		;
	if(!root["positioncost"].isNull())	    p->positioncost    =  root["positioncost"].asDouble()	;
	if(!root["premargin"].isNull())		    p->premargin       =  root["premargin"].asDouble()		;
	if(!root["margin"].isNull())			p->margin          =  root["margin"].asDouble()			;
	//if(!root["opencommission"].isNull())	p->opencommission  =  root["opencommission"].asDouble()	;
	//if(!root["closecommission"].isNull())	p->closecommission =  root["closecommission"].asDouble();
	if(!root["commission"].isNull())		p->commission	   =  root["commission"].asDouble();
	if(!root["closeprofit"].isNull())		p->closeprofit     =  root["closeprofit"].asDouble()	;
	if(!root["positionprofit"].isNull())	p->positionprofit  =  root["positionprofit"].asDouble()	;
	if(!root["totalprofile"].isNull())	    p->totalprofile    =  root["totalprofile"].asDouble() 	;
	//if(!root["updatedate"].isNull())		p->updatedate      =  root["updatedate"].asString();
	return p;
}

bson_t* get_bson_daybar(::CM::DayBar* p, bson_t* b)
{
	if(p == NULL) return b;
	BSON_APPEND_UTF8(b,   "timestamp"		,toa(p->Timestamp).c_str()	);
	BSON_APPEND_DOUBLE(b, "high"			, p->High					);
	BSON_APPEND_DOUBLE(b, "low"             , p->Low					);
	BSON_APPEND_DOUBLE(b, "open"            , p->Open					);
	BSON_APPEND_DOUBLE(b, "close"           , p->Close					);
	BSON_APPEND_DOUBLE(b, "average"         , p->Average				);
	BSON_APPEND_INT32 (b, "volume"          , p->Volume					);
	BSON_APPEND_DOUBLE(b, "turnover"        , p->Turnover				);
	BSON_APPEND_DOUBLE(b, "settlement"      , p->Settlement				);
	BSON_APPEND_DOUBLE(b, "openinterest"	, p->OpenInterest			);
	BSON_APPEND_DOUBLE(b, "presettlement"   , p->PreSettlement			);
	BSON_APPEND_DOUBLE(b, "preclose"        , p->PreClose				);
	BSON_APPEND_DOUBLE(b, "preoopeninterest", p->PreoOpenInterest		);
	BSON_APPEND_DOUBLE(b, "price"			, p->Price					);
	BSON_APPEND_DOUBLE(b, "upperlimit"		, p->UpperLimit				);
	BSON_APPEND_DOUBLE(b, "lowerlimit"		, p->LowerLimit				);
	//BSON_APPEND_DOUBLE(b, "updatedate"		, stamp_to_isodate(time(NULL)));
	bson_append_now_utc(b, "updatedate"		, strlen("updatedate"));
	return b;
}


Json::Value& get_json_daybar(::CM::DayBar* p, Json::Value& root)
{
	if(p == NULL) return root;
	root["timestamp"]			= toa(p->Timestamp);
	root["high"]                = p->High;
	root["low"]                 = p->Low;
	root["open"]                = p->Open;
	root["close"]               = p->Close;
	root["average"]             = p->Average;
	root["volume"]              = p->Volume;
	root["turnover"]            = p->Turnover;
	root["settlement"]          = p->Settlement;
	root["openinterest"]		= p->OpenInterest;
	root["presettlement"]       = p->PreSettlement;
	root["preclose"]            = p->PreClose;
	root["preoopeninterest"]    = p->PreoOpenInterest;
	root["price"]				= p->Price;
	root["upperlimit"]			= p->UpperLimit;
	root["lowerlimit"]			= p->LowerLimit;
	root["updatedate"]			= stamp_to_isodate(time(NULL));
	return root;
}


::CM::DayBar* get_struct_daybar(::CM::DayBar* p, Json::Value& root)
{
	if(p == NULL) return NULL;
	if(!root["timestamp"].isNull())		  p->Timestamp		  =	atoll(root["timestamp"].asString().c_str());
	if(!root["high"].isNull())            p->High             =	root["high"].asDouble();
	if(!root["low"].isNull())             p->Low              =	root["low"].asDouble();
	if(!root["open"].isNull())            p->Open             =	root["open"].asDouble();
	if(!root["close"].isNull())           p->Close            =	root["close"].asDouble();
	if(!root["average"].isNull())         p->Average          =	root["average"].asDouble();
	if(!root["volume"].isNull())          p->Volume           =	root["volume"].asInt();
	if(!root["turnover"].isNull())        p->Turnover         =	root["turnover"].asDouble();
	if(!root["settlement"].isNull())      p->Settlement       =	root["settlement"].asDouble();
	if(!root["openinterest"].isNull())    p->OpenInterest     =	root["openinterest"].asDouble();
	if(!root["presettlement"].isNull())   p->PreSettlement    =	root["presettlement"].asDouble();
	if(!root["preclose"].isNull())        p->PreClose         =	root["preclose"].asDouble();
	if(!root["preoopeninterest"].isNull())p->PreoOpenInterest =	root["preoopeninterest"].asDouble();
	if(!root["price"].isNull())			  p->Price			  =	root["price"].asDouble();
	if(!root["upperlimit"].isNull())	  p->UpperLimit		  =	root["upperlimit"].asDouble();
	if(!root["lowerlimit"].isNull())	  p->LowerLimit		  =	root["lowerlimit"].asDouble();

	return p;
}

Json::Value& get_json_ticker(::CM::Ticker* p, Json::Value& root)
{
	if(p == NULL) return root;
	root["Timestamp"]    = toa(p->Timestamp);
	root["Price"]		 = p->Price;
	root["Volume"]		 = p->Volume;
	root["Turnover"]     = p->Turnover;
	root["OpenInterest"] = p->OpenInterest;
	root["TotalVolume"]  = p->TotalVolume;
	root["TotalTurnover"]= p->TotalTurnover;
	root["BidPrice1"]	 = p->BidPrice1;
	root["AskPrice1"]	 = p->AskPrice1;
	root["BidVolume1"]	 = p->BidVolume1;
	root["AskVolume1"]   = p->AskVolume1;
	return root;
}

Json::Value& get_json_bar(::CM::Bar* p, Json::Value& root)
{
	if(p == NULL) return root;
	root["Timestamp"]	= toa(p->Timestamp);
	root["High"]		= p->High;
	root["Low"]			= p->Low;
	root["Open"]		= p->Open;
	root["Close"]		= p->Close;
	root["Volume"]		= p->Volume;
	root["Turnover"]	= p->Turnover;
	root["OpenInterest"]= p->OpenInterest;
	return root;
}

//查询回来线更新头部
void  db_update_order_header(CMongodb& _db, ::CM::Order* p)
{
	//成交的头的 插入
	Json::Value q, s, r;
	q["tradingday"]	=	p->tradingday;
	q["fundid"]		=	p->fundid;
	//q["parentid"]	=	p->parentid;
	//q["seq"]		=	p->seq;

	s["tradingday"] = p->tradingday;
	s["fundid"]     = p->fundid;
	//s["parentid"]	= p->parentid;
	//s["seq"]		= p->seq;

	r["$set"] = s;

	_db.ChangeCollection("ORDER");
	if(_db.Update(r.toStyledString(), q.toStyledString(), true) < 0)
	{
		LOG_ERROR(q.toStyledString() << "\n" << r.toStyledString() << "\n" <<  _db.GetLastErr());
	}
}



//查询回来线更新头部
void  db_update_done_header(CMongodb& _db, ::CM::Done* p)
{
	//成交的头的 插入
	Json::Value q, s, r;
	q["tradingday"]	= p->tradingday;
	q["fundid"]		= p->fundid;
	//q["parentid"]	= p->parentid;
	//q["seq"]		= p->seq;

	s["tradingday"] = p->tradingday;
	s["fundid"]     = p->fundid;
	//s["parentid"]	= p->parentid;
	//s["seq"]		= p->seq;

	r["$set"] = s;
	_db.ChangeCollection("DONE");
	if(_db.Update(r.toStyledString(), q.toStyledString(), true) < 0)
	{
		LOG_ERROR(q.toStyledString() << "\n" << r.toStyledString() << "\n" <<  _db.GetLastErr());
	}

}

//查询回来线更新头部
void  db_update_account_header(CMongodb& _db, ::CM::Account* p)
{
	//账户头信息的插入
	Json::Value q, s, r;
	q["tradingday"]	=	p->tradingday;
	q["fundid"]		=	p->fundid;

	s["tradingday"] = p->tradingday;
	s["fundid"]     = p->fundid;

	s["prebalance"]   = p->prebalance;
	s["premargin"]    = p->premargin;
	r["$set"] = s;
	_db.ChangeCollection("ACCOUNT");
	if(_db.Update(r.toStyledString(), q.toStyledString(), true) < 0)
	{
		LOG_ERROR(q.toStyledString() << "\n" << r.toStyledString() << "\n" <<  _db.GetLastErr());
	}

}

//整个账户信息更新
void db_update_account(::CM::Account* account)
{
	CMongodb _db =  CMongodb::Instance();
	_db.ChangeCollection("ACCOUNT");

	//更新账户表头
	db_update_account_header(_db, account);

	Json::Value q,r, p,d;
	q["tradingday"]	=	account->tradingday;
	q["fundid"]		=	account->fundid;

	get_json_account(account, r);

	d["account"] = r;
	p["$push"] = d;


	if(_db.Update(p.toStyledString(), q.toStyledString(), true) <0)
	{
		LOG_ERROR(q.toStyledString() << "\n" << p.toStyledString() << "\n" <<  _db.GetLastErr());
	}
}

//整个持仓的更新
void db_update_position(string fundid, uint32_t tradingday, int reqestid, map<string, ::CM::Position>& position)
{
	CMongodb _db =  CMongodb::Instance();
	_db.ChangeCollection("POSITION");

	//更新持仓 ----- 一个个push把
	Json::Value q,p;
	q["tradingday"]	=	toa(tradingday);//转化字符串
	q["fundid"]		=	fundid;

	Json::Value positionarr;
	typeof(position.begin()) itr = position.begin();
	for(; itr!=position.end(); itr++)
	{
		Json::Value pitem;
		get_json_position(&itr->second, pitem);
		pitem["requestid"] = reqestid;
		pitem["updatedate"] = stamp_to_isodate(time(NULL));
		positionarr.append(pitem);
	}
	Json::Value positionslist;
	positionslist["positions"] = positionarr;
	if(position.size() == 0) positionslist["positions"].resize(0);

	p["$set"] = positionslist;

	if(_db.Update(p.toStyledString(), q.toStyledString(), true) < 0)
	{
		LOG_ERROR(q.toStyledString() << "\n" << p.toStyledString() << "\n" <<  _db.GetLastErr());
	}
	else
	{
		LOG_DEBUG(p.toStyledString() <<  q.toStyledString());
	}
}

void db_update_done(map<string, ::CM::Done>& donem)
{
	if(donem.size() <= 0)  return ;

	CMongodb _db =  CMongodb::Instance();
	_db.ChangeCollection("DONE");


	::CM::Done* done = &donem.begin()->second;
	db_update_done_header(_db, done);

	Json::Value q,r,d, s;
	q["tradingday"]	=	 done->tradingday;
	q["fundid"]		=	 done->fundid;
	//q["seq"]		=	 done->seq;

	Json::Value ar;
	map<string, ::CM::Done>::iterator itr = donem.begin();
	for(; itr!=donem.end(); itr++)
	{
		done = &itr->second;
		get_json_done(done, r);
		ar.append(r);
	}
	d["done"]	= ar;
	if(donem.size() == 0) d["done"].resize(0);
	s["$set"] = d;

	LOG_DEBUG(q.toStyledString() << "\n" << s.toStyledString());
	if(_db.Update(s.toStyledString(), q.toStyledString(), false) < 0)
	{
		LOG_ERROR(q.toStyledString() << "\n" << s.toStyledString() << "\n" <<  _db.GetLastErr());
	}
}

//order.localid + order.session
void db_update_order(map<string, ::CM::Order>& orderm)
{
	if(orderm.size() <= 0) return;

	CMongodb _db =  CMongodb::Instance();
	_db.ChangeCollection("ORDER");

	::CM::Order* order = &orderm.begin()->second;
	db_update_order_header(_db, order);

	Json::Value q,d,s;
	q["tradingday"]	=	 order->tradingday;
	q["fundid"]		=	 order->fundid;
	//q["seq"]		=	 order->seq;

	Json::Value ar;
	map<string, ::CM::Order>::iterator itr = orderm.begin();
	for(; itr!=orderm.end(); itr++)
	{
		Json::Value r;
		order = &itr->second;
		get_json_order(order, r);
		ar.append(r);
	}
	d["order"] = ar;
	if(orderm.size() == 0) d["order"].resize(0);
	s["$set"]  = d;

	// LOG_DEBUG(q.toStyledString() << "\n" << s.toStyledString());
	if(_db.Update(s.toStyledString(), q.toStyledString(), false) < 0)
	{
		LOG_ERROR(q.toStyledString() << "\n" << s.toStyledString() << "\n" <<  _db.GetLastErr());
	}
}

int  db_get_tradingday(string fundid)
{
	CMongodb _db =  CMongodb::Instance();
	_db.ChangeCollection("TRADINGDAY");

	Json::Value q;
	q["fundid"] = fundid;
	if(_db.Query(q.toStyledString()) < 0)
	{
		LOG_ERROR("query tradingday fail. " << _db.GetLastErr());
		return -1;
	}
	while(_db.Next())
	{
		try
		{
			char* result = _db.GetResult();
			Json::Reader reader;
			Json::Value val;
			if(!reader.parse(result, val))
			{
				LOG_ERROR("reader.parse error. result:" << result);
				continue;
			}
			return val["tradingday"].asInt();
		}
		catch(...)
		{
			LOG_ERROR("query tradingday parse fail. " << _db.GetLastErr());
		}
	}
	return 0;
}

int	 db_set_tradingday(string fundid, int tradingday)
{
	CMongodb _db =  CMongodb::Instance();
	_db.ChangeCollection("TRADINGDAY");

	Json::Value q,s;
	q["fundid"] = fundid;

	s["fundid"] = fundid;
	s["tradingday"] = tradingday;
	s["updatedate"] = stamp_to_isodate(time(NULL));

	if(_db.Update(s.toStyledString(), q.toStyledString(), true) <0)
	{
		LOG_ERROR(q.toStyledString() << "\n" << s.toStyledString() << "\n" <<  _db.GetLastErr());
		return -1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file SunGardTradeDataProcess.cpp
///@brief 处理交易数据  写db push下游
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#include "SunGardTradeDataProcess.h"

#define __CONFIG_FILE__ "config.ini"

void SunGardTradeDataProcess::OnErrOrder(void* p,  int req_no, int row_no)
{
	STOrderInfo *p_order_info = (STOrderInfo*)p;
	Json::Value root;
	root = get_STOrderInfo(root, p_order_info);
	root["requestid"]	= 0;	//推送默认0  查询则为查询id
	root["type"]		= req_no;	//委托单类型
	//root["tradingday"]	= row_no;	//委托单类型
	root["update_date"]	= stamptostr(time(NULL));	//委托单类型

	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);
	
}
//委托推送处理
void SunGardTradeDataProcess::OnOrder(void* p,  int req_no, int row_no)
{
	STOrderInfo *p_order_info = (STOrderInfo*)p;
	Json::Value root;
	root = get_STOrderInfo(root, p_order_info);
	root["requestid"]	= 0;	//推送默认0  查询则为查询id
	root["type"]		= req_no;	//委托单类型
	//root["tradingday"]	= row_no;	//委托单类型
	root["update_date"]	= stamptostr(time(NULL));	//委托单类型
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);
}
//成交推送
void SunGardTradeDataProcess::OnDone(void* p,  int req_no, int row_no)
{
	STDoneInfo *p_done_info = (STDoneInfo*)p;
	Json::Value root;
	root = get_STDoneInfo(root, p_done_info);
	root["requestid"]	= req_no;	//推送默认0  查询则为查询id
	//root["tradingday"]	= row_no;	//委托单类型
	root["update_date"]	= stamptostr(time(NULL));	//委托单类型
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);
}
//账户信息推送
void SunGardTradeDataProcess::OnAccount(void* p,  int req_no, int row_no)
{
	STAccountInfo *p_account_info = (STAccountInfo*)p;
	
	Json::Value root;
	root = get_STAccountInfo(root, p_account_info);
	root["requestid"]	= req_no;	//推送默认0  查询则为查询id
	//root["tradingday"]	= row_no;	//委托单类型
	root["update_date"]	= stamptostr(time(NULL));	//委托单类型
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);

	//< 资金账户类型(0-储蓄类,1-证券A股类,2-期货类,3-个股期权)
	if(p_account_info->acc_type != 2) return;
}
//推送持仓信息
void SunGardTradeDataProcess::OnPosition(void* p,  int req_no, int row_no)
{
	STPositionInfo *p_position_info = (STPositionInfo*)p;
	Json::Value root;
	root = get_STPositionInfo(root, p_position_info);
	root["requestid"]	= req_no;	//推送默认0  查询则为查询id
	//root["tradingday"]	= row_no;	//委托单类型
	root["update_date"]	= stamptostr(time(NULL));	//委托单类型
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);
}

/********************查询有req_no   row_no的概念***********************************/
void SunGardTradeDataProcess::OnQuerySecCodeRsp(void* p,  int req_no, int row_no)
{
	//STSecCodeInfo *p_seccode_rsp = (STSecCodeInfo*)p;
	
}
void SunGardTradeDataProcess::OnQueryOrder(void* p,  int req_no, int row_no)
{
	STOrderInfo *p_order_info = (STOrderInfo*)p;
	Json::Value root;
	root = get_STOrderInfo(root, p_order_info);
	root["requestid"]	= req_no;	//推送默认0  查询则为查询id
	//root["tradingday"]	= row_no;	//委托单类型
	root["update_date"]	= stamptostr(time(NULL));	//委托单类型
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);
}
void SunGardTradeDataProcess::OnQueryEtfBaseInfo(void* p,  int req_no, int row_no)
{
//	STEtfBaseInfo *p_etfbaseinfo_rsp = (STEtfBaseInfo*)p;
}
void SunGardTradeDataProcess::OnQueryEtfBasketInfo(void* p,  int req_no, int row_no)
{
//	STEtfBasketInfo *p_etfbasketinfo_rsp = (STEtfBasketInfo*)p;
}
void SunGardTradeDataProcess::OnQueryDone(void* p,  int req_no, int row_no)
{
	STDoneInfo *p_done_info = (STDoneInfo*)p;
	Json::Value root;
	root = get_STDoneInfo(root, p_done_info);
	root["requestid"]	= req_no;	//推送默认0  查询则为查询id
	//root["tradingday"]	= row_no;	//委托单类型
	root["update_date"]	= stamptostr(time(NULL));	//委托单类型
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);
}
void SunGardTradeDataProcess::OnQueryAccount(void* p,  int req_no, int row_no)
{
	STAccountInfo *p_account_info = (STAccountInfo*)p;
	Json::Value root;
	root = get_STAccountInfo(root, p_account_info);
	root["requestid"]	= req_no;	//推送默认0  查询则为查询id
	//root["tradingday"]	= row_no;	//委托单类型
	root["update_date"]	= stamptostr(time(NULL));	//委托单类型
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);
}
void SunGardTradeDataProcess::OnQueryPosition(void* p,  int req_no, int row_no)	 	
{
	STPositionInfo *p_position_info = (STPositionInfo*)p;
	Json::Value root;
	root = get_STPositionInfo(root, p_position_info);
	root["requestid"]	= req_no;	//推送默认0  查询则为查询id
	//root["tradingday"]	= row_no;	//委托单类型
	root["update_date"]	= stamptostr(time(NULL));	//委托单类型
	
	Json::FastWriter fast_writer;
	string ss = fast_writer.write(root);
	LOG_DEBUG(ss);
}

int main(int argc, char* argv[])
{
	LOG_INIT(__FILE__, 4, 102400000, 10);


	SunGardTradeDataProcess* sp =  new SunGardTradeDataProcess("50000037", "123456");
	sp->set_front(1, "tcp://jinhuicapital.uicp.net:7900");  //交易
	sp->Init();

	sleep(3);

	
	if((sp->DoQueryAccount()) < 0)
	{
	    LOG_ERROR("query settle account fail...\n");
	    return;
	}

	sleep(3);

	if(sp->DoQueryPosition() < 0)
	{
		LOG_ERROR("query settle position fail...\n");
	    return;
	}

	sleep(3);
	if(sp->DoQueryOrder() < 0)
	{
		LOG_ERROR("query settle position fail...\n");
	    return;
	}
	
	sleep(10);

/*
	if(requestid_trade = DoQueryTrade() < 0)
	{
		LOG_DEBUG("query trade fail.");
		return;
	}

	STOrder order;
	
	DoOrder(order, 1)
*/
	return 0;
}










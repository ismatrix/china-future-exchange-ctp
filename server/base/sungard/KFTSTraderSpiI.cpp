/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file KFTSTraderSpiI.cpp
///@brief 处理交易数据  写db push下游
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#include "KFTSTraderSpiI.h"


CKFTSTraderSpiI::mapTypeFun		 CKFTSTraderSpiI::_map_callback_fun;
map<APIHandle, CKFTSTraderSpiI*> CKFTSTraderSpiI::map_instence_APIHandle;
pthread_mutex_t					 CKFTSTraderSpiI::_mutex = PTHREAD_MUTEX_INITIALIZER;
uint32_t						 CKFTSTraderSpiI::tradingday;
SyncRequest<int>			 	 CKFTSTraderSpiI::syncreq;        
map<int, int>		 CKFTSTraderSpiI::m_fundid_cellid;
map<int, int>		 CKFTSTraderSpiI::m_cellid_fundid;
		
void*  kthread_run(void* arg)
{
    CKFTSTraderSpiI* p = (CKFTSTraderSpiI*)arg;

    while(true)
    {
        p->ThreadRun();
		sleep(10);
    }
    return NULL;
}


CKFTSTraderSpiI::CKFTSTraderSpiI(const char* user, const char* pwd)
{
	InitFlag = false;
	apiHandle	= NULL;
	connHandle	= NULL;
	memset(&login, 0, sizeof(login));
	memset(&err_msg, 0, sizeof(err_msg));
	snprintf(login.cust_no, sizeof(login.cust_no), "%s", user);
	snprintf(login.cust_pwd, sizeof(login.cust_pwd), "%s", pwd);
}

CKFTSTraderSpiI::~CKFTSTraderSpiI()
{
	UnInit();

	InitFlag = false;
	apiHandle	= NULL;
	connHandle	= NULL;
	memset(&login, 0, sizeof(login));
	memset(&err_msg, 0, sizeof(err_msg));
}

int CKFTSTraderSpiI::ThreadRun()
{
	while(!get_InitFlag() || tradingday < 20160101)
	{
		ReInit();
		sleep(30);
	}
	return 0;
}

int CKFTSTraderSpiI::Init_Thread()
{
	pthread_t tid;
	pthread_attr_t attr;

	if(pthread_attr_init(&attr) != 0)
	{
		LOG_ERROR("pthread_attr_init fail.");
		exit(-1);
	}

	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
	{
		LOG_ERROR("pthread_attr_setdetachstate fail.");
		exit(-1);
	}

	if(pthread_create(&tid, &attr, kthread_run, this) != 0)
	{
		LOG_ERROR("pthread_create fail.");
		exit(-1);
	}
	return 0;
}

//初始化句柄 连接等
int CKFTSTraderSpiI::Init()
{
	if (!KFTSAPI_Initialize(&err_msg))
    {
        LOG_ERROR("初始化失败 " << err_msg.error_no << ":" <<  GBK2UTF8(err_msg.msg));
        //exit(-1);
		return -1;
    }
	
    // 创建交易员API
    apiHandle = KFTSAPI_Create(&err_msg);
    if (!apiHandle)
    {
        LOG_ERROR("创建交易员API失败 " <<  err_msg.error_no << ":" <<  GBK2UTF8(err_msg.msg));
        //exit(-1);
		return -1;
    }
	//记录apiHandle this映射表
	set_map_instence_APIHandle(apiHandle, this);

    // 委托撤单默认超时时间(ms),选项值为int类型,缺省值为3000. 
    int iOptValue = 3000;
    if(!KFTSAPI_SetOpt(apiHandle, KFTS_OPT_DEFAULT_TIMEOUT, &iOptValue, 3000, &err_msg)) // 设置默认超时时间
	{
        LOG_ERROR("参数设置失败 " << err_msg.error_no << ":" <<  GBK2UTF8(err_msg.msg));
		//exit(-1);
		return -1;
	}
	//日志级别,选项值为KFTS_LOG_LEVEL 类型，缺省值为KFTS_LL_INFO. 
	iOptValue = KFTS_LL_INFO;
	memset(&err_msg, 0, sizeof(err_msg));
	if(!KFTSAPI_SetOpt(apiHandle, KFTS_OPT_LOG_LEVEL, &iOptValue, 100, &err_msg)) // 设置日志级别
	{
		LOG_ERROR("参数设置失败 " << err_msg.error_no << ":" << GBK2UTF8(err_msg.msg));
		//exit(-1);
		//return -1;
	}
	
	//这里需要确认一下  断线怎么处理 这里会影响仓位等信息。 
	// KFTS_RELOGIN_RESUME  从上次接收推送结束位置开始回调(缺省选项) 
	// KFTS_RELOGIN_RESTART  重新执行所有回调 
	// KFTS_RELOGIN_QUICK  仅对后续推送执行回调 
 	iOptValue = KFTS_RELOGIN_QUICK;
	memset(&err_msg, 0, sizeof(err_msg));
	if(!KFTSAPI_SetOpt(apiHandle, KFTS_OPT_RELOGIN_TYPE, &iOptValue, 100, &err_msg)) // 设置订阅模式
	{
		LOG_ERROR("参数设置失败 " <<  err_msg.error_no << ":" <<  GBK2UTF8(err_msg.msg));
		//exit(-1);
		return -1;
	}
	
	//注册回调
	if(RegistCallBack()<0) return -1;
	
	map<int, string>::iterator itr = front.begin();
	for(; itr!=front.end(); itr++)
	{
		// 注册前置
		if (!KFTSAPI_RegFront(apiHandle, itr->first, itr->second.c_str(), &err_msg))
		{
			LOG_ERROR("注册前置失败 " <<  err_msg.error_no << ":" <<  GBK2UTF8(err_msg.msg));
			//exit(-1);
			return -1;
		}
	}
	
    // 先与前置建立连接
    if (!KFTSAPI_StartLogin(apiHandle, 0, -1, &err_msg))
    {
        LOG_ERROR("与前置建立连接失败 " <<  err_msg.error_no << ":" <<  GBK2UTF8(err_msg.msg));
       // exit(-1);
	   return -1;
    }

	LOG_DEBUG("init sungard");
	return 0;
}

void CKFTSTraderSpiI::UnInit()
{
	//登出	
	if(apiHandle != NULL)KFTSAPI_Logout(apiHandle, 1000, &err_msg);
	//清除apiHandle 和 实例的映射
	remove_instence_by_APIHandle(apiHandle);
	// 关闭辅连接
    //KFTSAPI_Close(apiHandle, connHandle, &err_msg);  
	// 关闭交易员API
    if(apiHandle != NULL) KFTSAPI_Destroy(apiHandle, &err_msg);
	// 全局退出
    KFTSAPI_Finalize(&err_msg);
}

void CKFTSTraderSpiI::ReInit()
{
	UnInit();
	Init();
}

/*****************************主动查询操作函数************************************************/
// 查询账户资金信息
int CKFTSTraderSpiI:: DoQueryAccount()
{
	int reqid = getRequestId();
	syncreq.set(reqid, 0);	
	
	if (!KFTSAPI_QueryAccount(apiHandle, NULL, reqid, &err_msg))
    {
		LOG_ERROR("DoQueryAccount fail.[" << err_msg.error_no << ":" << GBK2UTF8(err_msg.msg) << "]");
		return -1;
	}
	LOG_INFO("DoQueryAccount reqid:" << reqid);
	
	return reqid;
}


// 查询持仓信息
int CKFTSTraderSpiI::DoQueryPosition()
{
	//STQueryPosition qry_position = {0};
	int reqid = getRequestId();
	syncreq.set(reqid, 0);	
	
	if (!KFTSAPI_QueryPosition(apiHandle, NULL, reqid, &err_msg))
	{
		LOG_ERROR("DoQueryPosition fail.[" << err_msg.error_no << ":" << GBK2UTF8(err_msg.msg) << "]");
		return -1;
	}
	LOG_INFO("DoQueryPosition reqid:" << reqid);

	return reqid;
}

// 查询委托信息
int CKFTSTraderSpiI::DoQueryOrder()
{
	//STQueryOrder qry_order = {0};
	int reqid = getRequestId();
	syncreq.set(reqid, 0);	
	
	if (!KFTSAPI_QueryOrder(apiHandle, NULL, reqid, &err_msg))
	{
		LOG_ERROR("DoQueryPosition fail.[" << err_msg.error_no << ":" << GBK2UTF8(err_msg.msg) << "]");
		return -1;
	}
	LOG_INFO("DoQueryPosition reqid:" << reqid);
	
	return reqid;
}


// 查询成交信息
int CKFTSTraderSpiI::DoQueryTrade()
{
	//STQueryDone qry_done = {0};
	int reqid = getRequestId();
	syncreq.set(reqid, 0);	
	
	if (!KFTSAPI_QueryDone(apiHandle, NULL, reqid, &err_msg))
	{
		LOG_ERROR("DoQueryTrade fail.[" << err_msg.error_no << ":" << GBK2UTF8(err_msg.msg) << "]");
		return -1;
	}
	LOG_INFO("DoQueryTrade reqid:" << reqid);

	return reqid;
}


int CKFTSTraderSpiI::WaitReq(int32_t requestid, int timeout)
{
	if(requestid < 0) return requestid;

	int count = 0, val = 10;
	while(syncreq.get(requestid) != 1 && count++ * val < timeout)
	{
		msleep(val); //等待10ms检查
	}

	if(syncreq.get(requestid) != 1) return -1;

	return requestid;
}

// 查询ETF基本信息
int CKFTSTraderSpiI::DoQueryEtfBaseInfo()
{
	STQueryEtfBaseInfo etf_base_info_query = {0};
    etf_base_info_query.market_code = 1;
	int reqid = getRequestId();
	syncreq.set(reqid, 0);	
    
	if (!KFTSAPI_QueryEtfBaseInfo(apiHandle, &etf_base_info_query, reqid, &err_msg))
    {
    	LOG_ERROR("fail.[" << err_msg.error_no << ":" << GBK2UTF8(err_msg.msg) << "]");
		return -1;
	}
	LOG_INFO("reqid:" << reqid);

	return reqid;
}
// 查询证券代码信息
int CKFTSTraderSpiI::DoQuerySecCode(const char* security_code)
{
	STQuerySecCode sec_code_query = {0};
    sec_code_query.market_code = 1;
	snprintf(sec_code_query.sec_code, sizeof(sec_code_query.sec_code), "%s", security_code);
	int reqid = getRequestId();
	syncreq.set(reqid, 0);	
    
	if (!KFTSAPI_QuerySecCode(apiHandle, &sec_code_query, reqid, &err_msg))
    {
    	LOG_ERROR("fail.[" << err_msg.error_no << ":" << GBK2UTF8(err_msg.msg) << "]");
		return -1;
	}
	LOG_INFO("reqid:" << reqid);

	return reqid;
}

/*************************************主动下单操作*********************************************************/
// 使用主连接下单
int CKFTSTraderSpiI::DoOrder(STOrder* orders, int count)
{
	orders->private_no = getRequestId();	//私有号

    // 使用主连接一次下多笔委托
    if (!KFTSAPI_Order(apiHandle, NULL, orders, count, &err_msg))
    {
        LOG_ERROR("DoOrder fail.[" << err_msg.error_no << ":" << GBK2UTF8(err_msg.msg) << "]");
		return -1;
	}
	LOG_INFO("DoOrder OK");
    return orders->private_no;
}

//// 按委托私有号撤单
int CKFTSTraderSpiI::DoOrderCancel(STInputOrderAction* cancel)
{
    // 按私有号一次撤多笔委托
    if (!KFTSAPI_ReqOrderAction(apiHandle, NULL, cancel, &err_msg))
    {
        LOG_ERROR("DoOrderCancel fail.[" << err_msg.error_no << ":" << GBK2UTF8(err_msg.msg) << "]");
		return -1;
	}
	LOG_INFO("DoOrderCancel OK");
    return 0;
}

//// 使用辅连接下单
int CKFTSTraderSpiI::DoOrderEx(STOrder* orders, int count)
{
    // 使用辅连接下委托，要求交易服务全部处理成功，否则取消
	int reqid = getRequestId();
	syncreq.set(reqid, 0);	
  
	int cellid = orders->cell_id;
	orders->cell_id = m_cellid_fundid[cellid]; 
	LOG_DEBUG("cellid : " << cellid << " ------> " << orders->cell_id);
	//投资组合id
	orders->portfolio_id = m_cellid_fundid[cellid] + 1;

	if (!KFTSAPI_OrderEx(apiHandle, connHandle, orders, count, reqid, 1, &err_msg))
    {
		LOG_ERROR("DoOrderEx fail.[" << err_msg.error_no << ":" << GBK2UTF8(err_msg.msg) << "]");
		return -1;
	}
	LOG_INFO("DoOrderEx OK");

    return 0;
}

/***************************回调结果处理**************************************/
// 与前置机建立连接结果回调
void CKFTSTraderSpiI::OnConnected(APIHandle handle, const STRspMsg *p_rsp_info)
{
	STRspMsg err_msg = {0}; 
	//登录
    if (p_rsp_info->error_no != 0) // 与前置机建立连接失败
    {
		LOG_ERROR("OnConnected fail.error_no:" << p_rsp_info->error_no << " status:" << KFTSAPI_GetApiStatus(handle, &err_msg));
		return;
    } 
	LOG_INFO("OnConnected success.error_no:" << p_rsp_info->error_no << " msg:" << GBK2UTF8(p_rsp_info->msg) << " status:" << KFTSAPI_GetApiStatus(handle, &err_msg));
	
	CKFTSTraderSpiI* p = get_instence_by_APIHandle(handle);
	if(p==NULL) return;

	// 发送登录请求
	if (!KFTSAPI_Login(handle, &p->login, &err_msg)) 
	{
		LOG_ERROR("KFTSAPI_Login fail["<< p->login.cust_no << ":" << p->login.cust_pwd << " ]error_no:" << err_msg.error_no << " msg:" << GBK2UTF8(err_msg.msg) <<  " status:" << KFTSAPI_GetApiStatus(handle, &err_msg));
	}
	else
	{
		LOG_INFO("KFTSAPI_Login success.["<< p->login.cust_no << ":" << p->login.cust_pwd << " ]error_no:" << err_msg.error_no << " msg:" << GBK2UTF8(err_msg.msg) << " status:" << KFTSAPI_GetApiStatus(handle, &err_msg));
	}
}
// 登录结果回调
void CKFTSTraderSpiI::OnLogined(APIHandle handle, const STTraderLoginRsp *p_login_rsp, const STRspMsg *p_rsp_info)
{
	STRspMsg err_msg = {0}; 

	if (p_rsp_info->error_no != 0) // 登录失败
	{
		LOG_ERROR("login fail.error_no:" << p_rsp_info->error_no << " msg:" << GBK2UTF8(p_rsp_info->msg));
		CKFTSTraderSpiI* p = get_instence_by_APIHandle(handle);
		if(p==NULL) 
		{
			LOG_ERROR("apihandle not exist.");
			return ;
		}
	
		//继续登陆
		if(!KFTSAPI_Login(handle, &p->login, &err_msg)) 
		{
		//	sleep(3);
			LOG_ERROR("KFTSAPI_Login fail.["<< p->login.cust_no << ":" << p->login.cust_pwd << " ]error_no:" << err_msg.error_no << " msg:" << GBK2UTF8(err_msg.msg));
		}
			
		//LOG_INFO("send KFTSAPI_Login success.["<< p->login.cust_no << ":" << p->login.cust_pwd << " ]error_no:" << err_msg.error_no << " msg:" << GBK2UTF8(err_msg.msg));
		return ;
	} 
	LOG_INFO("login success. conn_session_id:" << p_login_rsp->conn_session_id << " cust_name:" << GBK2UTF8(p_login_rsp->cust_name) << 
							" instance_id:" << p_login_rsp->instance_id << " private_no:" << p_login_rsp->max_private_no <<
							" session_id:" << p_login_rsp->session_id << " tx_date:" << p_login_rsp->tx_date);
	tradingday = p_login_rsp->tx_date;
}
// 订阅结果回调
void CKFTSTraderSpiI::OnSubscribeRtn(APIHandle handle, const STRspMsg* msg)
{
	LOG_INFO("OnSubscribeRtn  error_no:" << msg->error_no << " msg:" << GBK2UTF8(msg->msg));
	CKFTSTraderSpiI* p = get_instence_by_APIHandle(handle);
	if(p!=NULL) 
	{
		p->InitFlag = true;
	}
}
// 错误回调
void CKFTSTraderSpiI::OnError(APIHandle handle, const STRspMsg *p_rsp_info, int req_no)
{
	LOG_ERROR("OnError req_no:" << req_no << " error_no:" << p_rsp_info->error_no << " msg:" << GBK2UTF8(p_rsp_info->msg));
}
//连接断开重连回调
int CKFTSTraderSpiI::OnReconnect(APIHandle handle, const CONNHandle conn_handle, int front_id, const STRspMsg *p_rsp_info)
{
	CKFTSTraderSpiI* p = get_instence_by_APIHandle(handle);
	if(p!= NULL) p->InitFlag = false; //这里可以确保重连之后可以销毁实例重建 因为重连不登录无法更新交易日

	STRspMsg err_msg;
	// 返回0由系统选择前置重新连接 断开连接
	LOG_INFO("OnReconnect error_no:" << p_rsp_info->error_no << " msg:" << GBK2UTF8(p_rsp_info->msg) << " front_id:" << front_id << " status:" << KFTSAPI_GetApiStatus(handle, &err_msg));
    return 0;
}

//注册下单和撤单的报错回调函数
void CKFTSTraderSpiI::OnErrOrder(APIHandle handle, const STOrderInfo *p_order_info, int type)
{
	if(p_order_info != NULL)
	{
		int cellid = p_order_info->order.cell_id;
		((STOrderInfo*)p_order_info)->order.cell_id =	m_cellid_fundid[cellid];
		LOG_DEBUG("cellid : " << cellid << " ------> " << p_order_info->order.cell_id);
	}

	CallBack(handle, "OnErrOrder", (void*)p_order_info, type);
}
//委托推送回调
void CKFTSTraderSpiI::OnOrder(APIHandle handle, const STOrderInfo *p_order_info, int type)
{
	if(p_order_info != NULL)
	{
		int cellid = p_order_info->order.cell_id;
		((STOrderInfo*)p_order_info)->order.cell_id =	m_cellid_fundid[p_order_info->order.cell_id];
		LOG_DEBUG("cellid : " << cellid << " ------> " << p_order_info->order.cell_id);
	}

	CallBack(handle, "OnOrder", (void*)p_order_info, type);
}
// 成交推送回调
void CKFTSTraderSpiI::OnDone(APIHandle handle, const STDoneInfo *p_done_info)
{
	if(p_done_info != NULL)
	{
		int cellid = p_done_info->cell_id;
		((STDoneInfo*)p_done_info)->cell_id = m_cellid_fundid[cellid];
		LOG_DEBUG("cellid : " << cellid << " ------> " << p_done_info->cell_id);
	}

	CallBack(handle, "OnDone", (void*)p_done_info);
}
// 资金变动回调
void CKFTSTraderSpiI::OnAccount(APIHandle handle, const STAccountInfo* p_account_info)
{
	if(p_account_info != NULL)
	{
		int cellid = p_account_info->cell_id;
		((STAccountInfo*)p_account_info)->cell_id = m_cellid_fundid[cellid];
		LOG_DEBUG("cellid : " << cellid << " ------> " << p_account_info->cell_id);
	}

	CallBack(handle, "OnAccount", (void*)p_account_info);
}
// 持仓变动回调
void CKFTSTraderSpiI::OnPosition(APIHandle handle, const STPositionInfo* p_position_info)
{
	if(p_position_info != NULL)
	{
		int cellid = p_position_info->cell_id;
		((STPositionInfo*)p_position_info)->cell_id = m_cellid_fundid[cellid];
		LOG_DEBUG("cellid : " << cellid << " ------> " << p_position_info->cell_id);
	}
	
	CallBack(handle, "OnPosition", (void*)p_position_info);
}
//证券代码信息查询结果回调
void CKFTSTraderSpiI::OnQuerySecCodeRsp(APIHandle handle, const STSecCodeInfo *p_seccode_rsp, int req_no, int row_no)
{
	CallBack(handle, "OnQuerySecCodeRsp", (void*)p_seccode_rsp, req_no, row_no);
}
//委托查询结果回调
void CKFTSTraderSpiI::OnQueryOrder(APIHandle handle, const STOrderInfo *p_orderinfo_rsp, int req_no, int row_no)
{
	if(p_orderinfo_rsp != NULL)
	{
		int cellid = p_orderinfo_rsp->order.cell_id;
		((STOrderInfo*)p_orderinfo_rsp)->order.cell_id =	m_cellid_fundid[p_orderinfo_rsp->order.cell_id];
		LOG_DEBUG("cellid : " << cellid << " ------> " << p_orderinfo_rsp->order.cell_id);
	}

	CallBack(handle, "OnQueryOrder", (void*)p_orderinfo_rsp, req_no, row_no);
}
// ETF基本信息查询结果回调
void CKFTSTraderSpiI::OnQueryEtfBaseInfo(APIHandle handle, const STEtfBaseInfo *p_etfbaseinfo_rsp, int req_no, int row_no)
{
	CallBack(handle, "OnQueryEtfBaseInfo", (void*)p_etfbaseinfo_rsp, req_no, row_no);
}
// ETF篮子信息查询结果回调
void CKFTSTraderSpiI::OnQueryEtfBasketInfo(APIHandle handle, const STEtfBasketInfo *p_etfbasketinfo_rsp, int req_no, int row_no)
{
	CallBack(handle, "OnQueryEtfBasketInfo", (void*)p_etfbasketinfo_rsp, req_no, row_no);
}
// 成交查询结果回调
void CKFTSTraderSpiI::OnQueryDone(APIHandle handle, const STDoneInfo *p_doneinfo_rsp, int req_no, int row_no)
{
	if(p_doneinfo_rsp != NULL)
	{
		int cellid = p_doneinfo_rsp->cell_id;
		((STDoneInfo*)p_doneinfo_rsp)->cell_id = m_cellid_fundid[cellid];
		LOG_DEBUG("cellid : " << cellid << " ------> " << p_doneinfo_rsp->cell_id);
	}


	CallBack(handle, "OnQueryDone", (void*)p_doneinfo_rsp, req_no, row_no);
}
// 账户资金查询结果回调
void CKFTSTraderSpiI::OnQueryAccount(APIHandle handle, const STAccountInfo *p_account_rsp, int req_no, int row_no)
{
	if(p_account_rsp != NULL)
	{
		int cellid = p_account_rsp->cell_id;
		((STAccountInfo*)p_account_rsp)->cell_id = m_cellid_fundid[cellid];
		LOG_DEBUG("cellid : " << cellid << " ------> " << p_account_rsp->cell_id);
	}


	CallBack(handle, "OnQueryAccount", (void*)p_account_rsp, req_no, row_no);
}

//持仓查询结果回调
void CKFTSTraderSpiI::OnQueryPosition(APIHandle handle, const STPositionInfo *p_position_rsp, int req_no, int row_no)
{
	if(p_position_rsp != NULL)
	{
		int cellid = p_position_rsp->cell_id;
		((STPositionInfo*)p_position_rsp)->cell_id = m_cellid_fundid[cellid];
		LOG_DEBUG("cellid : " << cellid << " ------> " << p_position_rsp->cell_id);
	}

	CallBack(handle, "OnQueryPosition", (void*)p_position_rsp, req_no, row_no);
}




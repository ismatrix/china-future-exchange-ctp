/////////////////////////////////////////////////////////////////////////
///@system 交易
///@company 慧网基金
///@file tdlive_svr.cpp
///@description  交易主函数
///@history 
///20160326	dreamyzhang		创建该文件
//现在交易所的异步逻辑不能保证持仓 成交 账户信息同步变化
//来一个成交之后计算持仓  计算账户信息下发。  这样才是ok的。 但是计算逻辑呢？
/////////////////////////////////////////////////////////////////////////


#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <Ice/Ice.h>

#include "RollLog.h"
#include "conf.h"
#include "mongoc_json.h"
#include "util.h"
#include "singleton.h"
#include "CTPTradeDataProcess.h"
#include "TradeSessionI.h"
#include "CTPObjPools.h"
#include "encrypt/encrypt_password.h"

#define  __TRADER__
#include "future/Init.h"

using namespace std;

#define __CONFIG_FILE__ "tdctp_config.ini"


class MdServer : public Ice::Application
{
	public:
		virtual int run(int, char*[]);
	private:
};

int main(int argc, char* argv[])
{
	int ret = ::InitAll(__CONFIG_FILE__);
	if(ret < 0) 
	{
		printf("init error \n");
		exit(-1);
	}


	string pwd = decrypt_account_password(g_keyValye["password"].c_str(), g_keyValye["password"].size());
	string  auth, user, dir2, appid;

    g_conf.get<string>("ACCOUNT", "auth", auth, "");
    g_conf.get<string>("ACCOUNT", "user", user, "");
    g_conf.get<string>("ACCOUNT", "appid", appid, "");
    LOG_INFO("auth:" << auth << " user:" << user << " appid:" << appid)
    g_syncsp  = new CTPTradeDataProcess(g_keyValye["brokerid"],  g_keyValye["account"], pwd, dir2, auth, user, appid);

	g_conf.get<string>("global", "dir2", dir2, "./tmp");
	g_syncsp->set_DataDir2(dir2);
	if(access(dir2.c_str(), 0) != 0)
	{
		if(mkdir(dir2.c_str(),   0755)==-1) 
		{
			printf("mkdir error. %s", dir2.c_str());
			return -1;	
		}	
	
	}

	int font_number = atoll(g_keyValye["number"].c_str());
	for(int i=0; i<font_number; i++)
	{
		 stringstream ss;
		 ss << "server" << i;
		g_syncsp->set_front(g_keyValye[ss.str()].c_str());
	}
	//g_syncsp->Init(TERT_RESTART); //从上次收到的续传
	//g_syncsp->Init(TERT_RESUME); //从上次收到的续传
	g_syncsp->Init( TERT_QUICK); //从上次收到的续传

	Ice::InitializationData initData;
	stringstream ss;
	ss << "tcp -h " << g_keyValye["service.ip"] << " -p " << g_keyValye["service.port"];
	string nameserver = g_keyValye["service.key"] + ".service.Endpoints";
	initData.properties = Ice::createProperties(argc, argv);
	initData.properties->setProperty(nameserver.c_str(), ss.str().c_str());
	initData.properties->setProperty("Ice.Warn.Connections", "1");
	initData.properties->setProperty("Ice.Trace.Network", "0");
	initData.properties->setProperty("Ice.Trace.Protocol", "0");
	
	initData.properties->setProperty("Ice.ThreadPool.Server.Size", "5");
	initData.properties->setProperty("Ice.ThreadPool.Server.SizeMax", "100");
	
	initData.properties->setProperty("Ice.Default.InvocationTimeout", "5000");
	
	//initData.properties->setProperty("Ice.ACM.Close", "4");	//空闲和没有调用是关闭
	//initData.properties->setProperty("Ice.ACM.Heartbeat", "0"); //没有心跳
	//initData.properties->setProperty("Ice.ACM.Timeout", "15");	//超时时间

#ifdef ICE_STATIC_LIBS
	Ice::registerIceSSL();
#endif

	MdServer app;
	return app.main(argc, argv, initData);
}

int MdServer::run(int argc, char* argv[])
{
	try
	{
		//创建对象适配器
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter(g_keyValye["service.key"] + ".service");

		TdSessionIPtr sender = new TdSessionI(); 

		adapter->add(sender, communicator()->stringToIdentity("sender"));

		adapter->activate();

		communicator()->waitForShutdown();
	}
	catch(::Ice::Exception& e)
	{
		stringstream ss;
		ss << e;
		LOG_ERROR(ss.str());
	}
	catch(...)
	{
		LOG_ERROR("Exception .");
	}

	communicator()->destroy();

	return EXIT_SUCCESS;
}




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
#include "MdPastSessionI.h"

#include "future/Init.h"

using namespace std;

#define __CONFIG_FILE__   "./mdpast_config.ini"


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
	
	g_holidays.Init(CMongodb::Instance());
	g_timeserise.Init(CMongodb::Instance());

#ifdef ICE_STATIC_LIBS
	    Ice::registerIceSSL();
#endif

	MdServer app;
	return app.main(argc, argv, "config.server");
}

int MdServer::run(int argc, char* argv[])
{
	try
    {
		//创建对象适配器
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("MDPAST.Server");
		
		MdSessionIPtr sender = new MdSessionI(); 
		
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




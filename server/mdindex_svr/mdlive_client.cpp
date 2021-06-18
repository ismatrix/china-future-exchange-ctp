/*************************************************************************
    > File Name: mdlive_client.cpp
    > Created Time: Fri 08 Jul 2016 01:11:59 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#include "mdlive_client.h"

using namespace std;

MdSessionPrx g_push__session;


int run(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif

	//短线重连 长连接
	while(true)
	{
		try
		{
			MdliveClient app;
			app.main(argc, argv, "config.client");
		}
		catch(Ice::Exception& e)
		{
			stringstream ss;
			ss << e;
			LOG_ERROR(ss.str());
		}
		catch(...)
		{
			LOG_ERROR("unknow exception");
		}
		sleep(1);	//3秒重连
	}
}





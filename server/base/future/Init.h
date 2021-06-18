/*************************************************************************
    > File Name: Init.h
    > Created Time: Fri 17 Jun 2016 03:43:40 PM CST
    > Author: 
    > description: 
 ************************************************************************/
#ifndef __INIT__
#define __INIT__


#include "common.h"



int InitAll(const char*  conf)
{
	if(!g_conf.load(conf))
	{
	     printf("load file %s fail.",  conf);
	     return -1;
	}

	InitConf(g_conf);
    
    //InitMongo(CMongodb::Instance());

	#ifdef   __INIT_CFILE__
		LOG_DEBUG("init file conf");
		InitFile(g_conf, g_file);
	#endif
	
	#ifdef __INIT_FIFO__
		LOG_DEBUG("init fifo conf");
		InitFifo(g_conf);
	#endif
	
	#ifdef __TRADER__
		GetAccountInfo(g_conf);
	#endif
	return 0;
}

#endif



// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <Ice/Ice.h>
//#include <Glacier2/Glacier2.h>
#include <math.h>
#include "RollLog.h"
#include "conf.h"
#include "ice/Common.h"
#include "mongoc_json.h"


#include "future/tradetime.h"
#include "singleton.h"
//初始化模块
#define  __INIT_FIFO__
#include "future/Init.h"
#include "future/baseinfo.h"
#include "mdlive_client.h"
#include "index_calculate.h"

using namespace std;
using namespace MdLive;

#define __CONFIG_FILE__   "./mdindex_config.ini"



int main(int argc, char* argv[])
{

	int ret = ::InitAll(__CONFIG_FILE__);
	if(ret < 0) 
	{
		printf("init error \n");
		exit(-1);
	}

	g_tradetime.Init(CMongodb::Instance());
	g_holidays.Init(CMongodb::Instance());

	//线程定时数据库load数据
	Singleton<BaseCalculate>::Instance().init(g_conf);

	run(argc, argv);	//连接上游获取数据 开始计算
	return 0;
}


/*************************************************************************
    > File Name: mongoc_api.h
    > Created Time: Fri 17 Jun 2016 01:52:31 PM CST
    > Author: 
    > description: 
 ************************************************************************/
#ifndef __COMMON_BUSS__
#define __COMMON_BUSS__

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>

#include "../ctp/ThostFtdcMdSpiI.h"
#include "RollLog.h"
#include "conf.h"
#include "../ctp/md_callback.h"
#include "mongoc_json.h"
#include "file.h"
#include "util.h"
#include "hq.h"

using namespace std;

extern bool			g_debug;
extern CFile*		g_file;
extern Conf			g_conf;
extern map<string, string> g_keyValye;

//#ifdef   __INIT_CFILE__
int InitFile(Conf& conf, CFile* & f);
//#ifdef __INIT_FIFO__
int InitFifo(Conf& conf);


//默认都要
int InitConf(Conf& conf);

void InitMongo(CMongodb& db);

int  QueryMongoInstrument(Conf& conf, set<string>& idlist);
int QueryMongoProduct(list<string>& products);

//每天17点清空内存相关信息
//同时查询数据库的合约信息以更新订阅
void md_reset(set<string>& idlist);


//读取数据库账户信息的
int GetAccountInfo(Conf& conf);














#endif

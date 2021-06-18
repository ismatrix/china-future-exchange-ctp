/*************************************************************************
    > File Name: util.cpp
    > Created Time: Mon 11 Apr 2016 06:18:03 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
//#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <signal.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <typeinfo>
#include <string.h>

using namespace std;

//remove char(c) of buf
#define RemoveChar(buf, c) do{\
	if(buf==NULL) break;\
	char* pf = buf;\
	char* pb = pf;\
	while(*pb != '\0')\
	{\
		while(*pb != '\0' && *pb != c) *pf++ = *pb++; \
		while(*pb == c) pb++;\
	}\
	*pf='\0';\
}while(0)


uint32_t t2seconds(string t, string format);
uint32_t st2seconds(time_t  t);
string second2ts(uint32_t second);

//字符串格式转化时间戳
time_t strtostamp(const char*  date, const char* format = "%Y-%m-%d %H:%M:%S");
//日期转化tm格式
struct tm strtotm(const char*  date, const char* format = "%Y-%m-%d %H:%M:%S");
//时间戳格式化
string stamptostr(time_t stamp = time(NULL), const char* format="%Y-%m-%d %H:%M:%S");
uint32_t get_hour(time_t stamp=time(NULL));
//获取星期几
uint32_t get_day_of_week(time_t stamp);
//圆整日期到日
uint32_t get_ymd(time_t stamp);
//返回微秒记得时间戳
uint64_t get_usec_timestamp();
//char to string
string ctos(char c);

	template<typename T>
string toa(T a)
{
	stringstream ss;
	ss << a;
	return ss.str();
}


	template<typename T>
T stoi(const char* buff)
{
	stringstream ss;
	ss << buff;

	T tmp;
	ss >> tmp;
	return tmp;
}

//设置程序core 不进行限制 失败就是设置默认最大
int set_limit_core(uint32_t size);
//设置精灵进程
int  set_daemon();

inline void msleep(uint32_t t)
{
	usleep(1000*t);
}



uint64_t stoint64(const char* s);
string int64tos(uint64_t v);





#endif






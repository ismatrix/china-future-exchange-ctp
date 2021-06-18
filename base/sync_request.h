/*************************************************************************
    > File Name: sync_request.h
    > Created Time: Sun 31 Jul 2016 01:55:53 PM CST
    > Author: 
    > description: 请求id同步的文件
 ************************************************************************/

#ifndef __SYNC__REQUEST__
#define __SYNC__REQUEST__


#include<iostream>
#include <map>
#include <pthread.h>
using namespace std;


//value -1 0 1 2------------ key不存在-1   0表示发请求   1 表示请求接收完毕
template<typename T>
class SyncRequest
{
	public:
		SyncRequest()
		{
			pthread_mutex_init(&_mutex, NULL);
		}

		void set(T t, int value)
		{
			pthread_mutex_lock(&_mutex);
						
			syncrequest[t] = value;
			
			pthread_mutex_unlock(&_mutex);
		}
		
		int get(T t)
		{
			int tmp = -1;
			pthread_mutex_lock(&_mutex);
			
			if(syncrequest.find(t) != syncrequest .end())
				tmp = syncrequest[t];
			
			pthread_mutex_unlock(&_mutex);
			return tmp;
		}

		void clear()
		{
			pthread_mutex_lock(&_mutex);
			syncrequest.clear();
			pthread_mutex_unlock(&_mutex);
		}
		map<T, int> syncrequest;
		pthread_mutex_t _mutex;
};

#endif






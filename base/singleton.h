/*************************************************************************
    > File Name: singleton.h
    > Created Time: Mon 09 May 2016 10:45:35 AM CST
    > Author: 
    > description: 
 ************************************************************************/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <iostream>
using namespace std;


template<typename T>
class Singleton
{
	public:
		static T& Instance()
		{
			//static T* p = NULL;
			if(p == NULL) p = new T;
			return *p;
		}

	private:
		static T* p;
};

template<typename T>
T* Singleton<T>::p = NULL;


#endif




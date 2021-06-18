/*************************************************************************
    > File Name: mongoc_api.cpp
    > Created Time: Wed 06 Apr 2016 03:34:01 PM CST
    > Author: 
    > description: 封装mongodb 非线程安全 依赖jsoncpp的
 ************************************************************************/
#ifndef __MONGOC_JSON_H__
#define __MONGOC_JSON_H__

#include <iostream>
#include <string>
#include <bson.h>
//#include <libmongoc-1.0/mongoc-cursor.h>
#include <libmongoc-1.0/mongoc.h>
#include <json/json.h>
#include "RollLog.h"

using namespace std;

/*
-----查询一个库里面的不同表
-----查询不同库里面的表
using
CMongodb db;
db.Init(...);
db.query();
while(db.next())
{
	string s = db.GetResult();
}
db.Release();

CMongodb db;
db.Init(...);
db.Insert();
db.Release();
*/
class CMongodb
{
	public:
		CMongodb();
	
		CMongodb(const CMongodb& _db);
		
		CMongodb(const char* uri, const char* db, const char* collection);
		~CMongodb();
		static CMongodb& Instance();	//单例模式 需要Init
	
		//CMongodb& operator = (const CMongodb& _db);

		int 	Init(const char* uri, const char* db, const char* collection);	//初始化
	
		int		Insert(string json);									//插入
		int		Update(string json_set, string json_query, bool upsert=false);	//更新 upsert=true不存在就插入
		int		Delete(string json);									//删除
		int		Query(string json, uint32_t skip=0, uint32_t limit=0); 	//查询
		int		GetCount(string json);		//获取受影响的行
		bool	Next();			//移动游标到下一条
		void	Release();		//释放本次查询相关内存

		
		//mongo批量操作接口
		inline int binit(){if(bulk!=NULL)mongoc_bulk_operation_destroy (bulk); bulk=mongoc_collection_create_bulk_operation(m_collection, true, NULL); return 0;}
		inline int binsert(bson_t* doc){mongoc_bulk_operation_insert(bulk, doc);return 0;}
		inline int bremove(bson_t* query){mongoc_bulk_operation_remove (bulk, query); return 0;}
		inline int bremove_one(bson_t* query){mongoc_bulk_operation_remove_one(bulk, query);return 0;}
		inline int bupdate(bson_t* query, bson_t*  doc,  bool upsert=false){mongoc_bulk_operation_update (bulk, query, doc, upsert);return 0;}
		inline int bupdate_one(bson_t* query, bson_t*  doc,  bool upsert=false){mongoc_bulk_operation_update_one(bulk, query, doc, upsert);return 0;}
		inline void brelease(){Release();};	//必须调用 释放请求和返回
		inline int bexecute(string& rsp)
		{
			if(bulk==NULL) return -1;
			bson_t reply;
			int ret = mongoc_bulk_operation_execute (bulk, &reply, &m_error);
			char* str = bson_as_json (&reply, NULL);
			rsp = str;
			bson_free (str);
			bson_destroy(&reply);
			if (!ret) return -2; else return 0;
		}

#define BPROCESS(doc, op) do{\
	bson_t* b = bson_new();\
	if(!bson_init_from_json(b, doc.c_str(), doc.length(), &m_error))\
	{\
		LOG_ERROR("bson_init_from_json Failure:" << m_error.message);\
		return -1;\
	}\
	int ret = b##op(b);\
	bson_destroy(b);\
	return ret;\
}while(0);
		inline int binsert(string doc)
		{
			BPROCESS(doc, insert);
		}

		inline int bremove(string doc)
		{
			BPROCESS(doc, remove);
		}
		
		inline int bremove_one(string doc)
		{
			BPROCESS(doc, remove_one);
		}

#define BUPDATE(q, doc, upsert, op) do{\
	bson_t* bq = bson_new();\
	bson_t* b = bson_new();	\
	if(!bson_init_from_json(b, doc.c_str(), doc.length(), &m_error)||\
	   !bson_init_from_json(bq, q.c_str(), q.length(), &m_error))\
	{\
		if(bq!=NULL)bson_destroy(bq);\
		if(b!=NULL)bson_destroy(b);\
		LOG_ERROR("bson_init_from_json Failure:" << m_error.message);\
		return -1;\
	}\
	int ret = b##op(bq, b, upsert);\
	bson_destroy(bq);\
	bson_destroy(b);\
	return ret;	\
}while(0)
		inline int bupdate(string q, string doc, bool upsert)
		{
			BUPDATE(q, doc, upsert, update);
		}

		inline int bupdate_one(string q, string doc, bool upsert)
		{
			BUPDATE(q, doc, upsert, update_one);
		}


		const char*  GetLastErr();	//获取最后一条错误信息
		int  	GetLastCode();	//获取最后一条错误代码
		char*   GetResult();	//获取查询结果
		int 	UnInit();
		int 	ReInit(const char* uri, const char* db, const char* collection);//重新初始化
		int 	ReInit(const char* db, const char* collection);					//重新初始化
		int 	ReInit(const char* collection);					//重新初始化
		int		ChangeCollection(const char* collection);

		string					m_db;			//db名字
		string					m_uri;			//mongodb连接地址
		mongoc_collection_t*	m_collection;	//查询集合
	private:
		mongoc_bulk_operation_t *bulk;	//批量操作接口

		bool					m_connected;	//是否连接状态
		mongoc_client_t*		m_client;		//客户端
		mongoc_cursor_t*		m_cursor;		//查询时的游标
		
		bson_error_t 			m_error;			//错误信息存储
		static CMongodb*    m_instance;	
		
		//query之后的全局存储
		 bson_t*				m_result;		//bson_t query对象
		const bson_t*			m_query;		//bson_t query对象
		char* 					m_pjson;		//查询next之后的内存 
	
		int _retry;
};

//isodate 转化为string
uint32_t   isodate_to_stamp(Json::Value& r);

//string转化iso
Json::Value stamp_to_isodate(uint32_t t);


#endif



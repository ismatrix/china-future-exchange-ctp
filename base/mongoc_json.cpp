/*************************************************************************
    > File Name: mongoc_json.cpp
    > Created Time: Wed 06 Apr 2016 03:34:01 PM CST
    > Author: 
    > description: 封装mongodb 非线程安全 依赖jsoncpp的
 ************************************************************************/
#include "mongoc_json.h"



//isodate 转化为string
uint32_t   isodate_to_stamp(Json::Value& r)
{
	if(r.isObject() && !r["$date"].isNull() && r["$date"].isDouble())
	{
		return (uint32_t)(r["$date"].asDouble()/1000);
	}
	return 0;
}


//string转化iso
Json::Value stamp_to_isodate(uint32_t t)
{
	Json::Value d;
	d["$date"]	=	((double)t)*1000;
	return d;
}

// CMongodb db;
//db.Init(char* uri, char* db, char* collection=NULL);
//CMongodb db(char* uri, char* db, char* collection=NULL);

CMongodb*    CMongodb::m_instance = NULL;	

//设置mongodb的log回调
static void log_handler (mongoc_log_level_t  log_level, const char* log_domain, const char* message, void* user_data)
{
	switch(log_level)
	{
		case MONGOC_LOG_LEVEL_ERROR:
		case MONGOC_LOG_LEVEL_CRITICAL:
		case MONGOC_LOG_LEVEL_WARNING:
			LOG_ERROR("[mongo]" << log_domain << "]" << message);
		case MONGOC_LOG_LEVEL_MESSAGE:
		case MONGOC_LOG_LEVEL_INFO:
			LOG_INFO("[mongo]" << log_domain << "]" << message);
		case MONGOC_LOG_LEVEL_DEBUG:
		case MONGOC_LOG_LEVEL_TRACE:
			LOG_DEBUG("[mongo]" << log_domain << "]" << message);
		default:
			break;
	}
}


//初始化成员变量
CMongodb::CMongodb()
{
	m_uri		= "";
	m_db		= "";
	m_connected = false;
	m_query 	= NULL;
	m_client	= NULL;
	m_collection= NULL;
	m_cursor	= NULL;
	m_pjson 	= NULL;
	m_result	= NULL;
	_retry		= 2;
	bulk		= NULL;
}

//初始化连接相关的信息 和 成员变量
CMongodb::CMongodb(const char* uri, const char* db, const char* collection)
{
	CMongodb();
	Init(uri, db, collection);
}

CMongodb::CMongodb(const CMongodb& _db)
{
	m_uri       = "";
	m_db        = "";
	m_connected = false;
	m_query     = NULL;
	m_client    = NULL;
	m_collection= NULL;
	m_cursor    = NULL;
	m_pjson     = NULL;
	m_result    = NULL;
	_retry      = 2;
	bulk		= NULL;
	Init(_db.m_uri.c_str(), _db.m_db.c_str(), NULL);
}

CMongodb::~CMongodb()
{
	UnInit();
}

int CMongodb::UnInit()
{
	Release();
	if(m_collection != NULL)
	{
		mongoc_collection_destroy(m_collection);	
		m_collection = NULL;
	}
	
	if(m_client != NULL)
	{
		mongoc_client_destroy(m_client);	
		m_client = NULL;
	}
	mongoc_cleanup();
	return 0;
}

CMongodb& CMongodb::Instance()
{
	if(m_instance == NULL)
		m_instance = new CMongodb();
	return *m_instance;
}

int CMongodb::ReInit(const char* uri, const char* db, const char* collection)
{
	UnInit();
	return Init(uri, db, collection);
}

int CMongodb::ReInit(const char* db, const char* collection)
{
	UnInit();
	string uri = m_uri;
	return Init(uri.c_str(), db, collection);
}

int CMongodb::ReInit(const char* collection)
{
	UnInit();
	string uri = m_uri;
	string db  = m_db;
	return Init(uri.c_str(), db.c_str(), collection);
}

//初始化连接相关的信息
int CMongodb::Init(const char* uri, const char* db, const char* collection)
{
	m_uri	= uri;
	m_db	= db;
	//初始化mongo
	mongoc_init();
	//初始化日至回调
	mongoc_log_set_handler(log_handler, NULL);
	//连接
	m_client = mongoc_client_new(m_uri.c_str());
	m_connected = true;

	if(collection!=NULL) ChangeCollection(collection);
	return 0;
}

int CMongodb::ChangeCollection(const char* collection)
{
	if(m_collection != NULL) mongoc_collection_destroy(m_collection); 
	m_collection = mongoc_client_get_collection(m_client, m_db.c_str(), collection);
	
	if(bulk!=NULL)mongoc_bulk_operation_destroy (bulk);  
	bulk=mongoc_collection_create_bulk_operation(m_collection, true, NULL);
	
	return 0;
}

//json string插入 
int CMongodb::Insert(string json)
{
	//string out = json.toStyledString();	
	bool flag = true;
	bson_t* bson_insert = bson_new();
	
	//json字符串转化bson对象
	flag = bson_init_from_json(bson_insert, json.c_str(), json.length(), &m_error);
	if(!flag)
	{
		LOG_ERROR("bson_init_from_json Failure:" << m_error.message);
		return -1;
	}

	for(int i = 0; i < _retry; i++)
	{
		//插入数据
		flag = mongoc_collection_insert(m_collection, MONGOC_INSERT_NONE,  bson_insert,  NULL, &m_error);
		if (!flag)
		{
			LOG_ERROR("mongoc_collection_insert Failure:" << m_error.message);
		}
		else
		{
			break;
		}
	}
	bson_destroy(bson_insert);
	if(!flag) return -1;

	return 0;
}

 //查询并更新
int CMongodb::Update(string json_set, string json_query, bool upsert)
{
	bool flag = true;
	bson_t* bson_query = bson_new();
	bson_t* bson_set = bson_new();

	//json字符串转化bson对象
	if(!bson_init_from_json(bson_set, json_set.c_str(), json_set.length(), &m_error) ||
	   !bson_init_from_json(bson_query, json_query.c_str(), json_query.length(), &m_error))
	{
		if(bson_query!=NULL) bson_destroy(bson_query);
		if(bson_set!=NULL) bson_destroy(bson_set);
		LOG_ERROR("bson_init_from_json Failure:" << m_error.message);
		return -1;
	}
	
	for(int i = 0; i < _retry; i++)
	{
		//flag = mongoc_collection_update(m_collection, MONGOC_UPDATE_NONE, bson_query, bson_set, NULL, &m_error);
		flag = mongoc_collection_find_and_modify(m_collection, bson_query, NULL, bson_set, NULL, false, upsert, true, NULL, &m_error);
		if (!flag)
		{
			LOG_ERROR("mongoc_collection_update Failure:" << m_error.message);
		}
		else
		{
			break;
		}
	}	
	bson_destroy(bson_query);
	bson_destroy(bson_set);
	
	if(!flag) return -1;
	
	return 0;
}
    

int  CMongodb::Delete(string json)
{
	bson_t* query = bson_new();
	bool flag = true;
	
	//json字符串转化bson对象
	flag = bson_init_from_json(query, json.c_str(), json.length(), &m_error);
	if(!flag)
	{
		LOG_ERROR("bson_init_from_json Failure:" << m_error.message);
		return -1;
	}
	
	for(int i = 0; i < _retry; i++)
	{
		//flag = mongoc_collection_delete(m_collection, MONGOC_DELETE_NONE, query, NULL, &m_error);
		flag = mongoc_collection_remove(m_collection, MONGOC_REMOVE_NONE, query, NULL, &m_error);
		if (!flag)
		{
			LOG_ERROR("mongoc_collection_update Failure:" << m_error.message);
		}
		else
		{
			break;
		}
	}
	bson_destroy(query);
	if(!flag) return -1;
	
	return 0;
}


int  CMongodb::Query(string json, uint32_t skip, uint32_t limit)
{
	if(m_result != NULL) bson_destroy(m_result); 
	m_result = bson_new();
	bool flag = true;
	//json字符串转化bson对象
	flag = bson_init_from_json(m_result, json.c_str(), json.length(), &m_error);
	if(!flag)
	{
		LOG_ERROR("bson_init_from_json Failure:" << m_error.message);
		return -1;
	}
	
	//查询
	if(m_cursor != NULL) mongoc_cursor_destroy(m_cursor);  
	m_cursor = mongoc_collection_find (m_collection, MONGOC_QUERY_NONE, skip, limit, 0, m_result, NULL, NULL);

	return 0;
}


bool CMongodb::Next()
{
	if(m_cursor == NULL) 
	{
		LOG_ERROR("m_cursor is NULL. not query or init.");
		return false;
	}

	//移动游标
	bool ret = mongoc_cursor_next(m_cursor, &m_query);
	if(!ret || m_query == NULL)
	{
		mongoc_cursor_error (m_cursor, &m_error);
		return false;
	}
	
	if(m_pjson != NULL)
	{
		bson_free(m_pjson);
		m_pjson = NULL;
	}

	m_pjson = bson_as_json(m_query, NULL);
	
	return true;
}

int CMongodb::GetCount(string json)
{
	if(m_result != NULL) bson_destroy(m_result); 
	m_result = bson_new();
	bool flag = true;
	//json字符串转化bson对象
	flag = bson_init_from_json(m_result, json.c_str(), json.length(), &m_error);
	if(!flag)
	{
		LOG_ERROR("bson_init_from_json Failure:" << m_error.message);
		return -1;
	}
	
	int count = mongoc_collection_count (m_collection, MONGOC_QUERY_NONE, m_result, 0, 0, NULL, &m_error);
	if(count < 0)
	{
		LOG_ERROR("mongoc_collection_count error:" << GetLastErr());
	}
	bson_destroy(m_result);
	return count;
}

void CMongodb::Release()
{
	if(m_result != NULL)
	{
		bson_destroy(m_result);
		m_result = NULL;
	}
	if(m_pjson != NULL)
	{
		bson_free(m_pjson);
		m_pjson = NULL;
	}
	if(m_cursor != NULL)
	{
		mongoc_cursor_destroy(m_cursor);	
		m_cursor = NULL;
	}
	if(bulk != NULL)
	{
		mongoc_bulk_operation_destroy (bulk);
		bulk = NULL;
	}
}

const char*  CMongodb::GetLastErr()
{
	return m_error.message;
}
	
int  CMongodb::GetLastCode()
{
	return m_error.code;
}

char* CMongodb::GetResult()
{
	return m_pjson;
}














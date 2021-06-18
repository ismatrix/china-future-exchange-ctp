/*
 * ut_mysql.cpp
 *
 *  Created on: 2013-9-25
 *      Author: echo
 */
#include "ut_mysql.h"
#include <assert.h>

#define ATTR_CREATE 422448
#define ATTR_DESTROY 422449
#define ATTR_ON_CONNECT 422450
#define ATTR_ON_DISCONNECT 422451
#define ATTR_ON_CONNECT_FAIL 422452
#define ATTR_ON_QUERY_ERR 422453

const char ut_mysql::_null_string[1] = {'\0'};
inline static bool err_is_connect_lost(uint32_t err)
{
	//return err==ER_SERVER_SHUTDOWN || err==CR_SERVER_LOST || err==CR_SERVER_GONE_ERROR;
	return err==ER_SERVER_SHUTDOWN || (err>=CR_MIN_ERROR && err<=CR_MAX_ERROR);
}

ut_mysql::ut_mysql(void)
:_res(NULL), _field_len(NULL), _row_count(0), _field_count(0), _connected(false),
 _execute_result(false), _port(0), _cnnt_param(0), _cnnt_timeout(-1), _read_timeout(-1),
 _write_timeout(-1), _err_code(0), _on_connect_hdlr(NULL), _on_disconnect_hdlr(NULL),
 _on_mysql_options_hdlr(NULL)
{
	ut_attr_inc(ATTR_CREATE,1);
	memset(&_on_connect_cb, 0, sizeof(_on_connect_cb));
	memset(&_on_disconnect_cb, 0, sizeof(_on_disconnect_cb));
	memset(&_on_mysql_options_cb, 0, sizeof(_on_mysql_options_cb));
	memset(&_mysql, 0, sizeof(_mysql));
	memset(&_row, 0, sizeof(_row));
}
ut_mysql::~ut_mysql(void)
{
	ut_attr_inc(ATTR_DESTROY,1);
	if(_res!=NULL)
	{
		mysql_free_result(_res);
		_res = NULL;
	}

	if(_connected)
	{
		mysql_close(&_mysql);
	}
}
int ut_mysql::field_index(const std::string& name) const
{
	if(NULL==_res)
		return -1;
	if(NULL==_res->fields)
		return -1;
	uint32_t fc = field_count();
	for(uint32_t i=0; i<fc; ++i)
	{
		if(0==strcmp(_res->fields[i].name, name.c_str()))
			return i;
	}
	return -1;
}
bool ut_mysql::next()
{
	if(_res!=NULL && _row_count>0)
	{
		_row=mysql_fetch_row(_res);
		if(_row==NULL)
		{
			update_err();
			return false;
		}
		else
		{
			_field_len = mysql_fetch_lengths(_res);
			return true;
		}
	}
	return false;
}
bool ut_mysql::execute(const char* sql, uint32_t len)
{
	_execute_result = false;
	if(_res!=NULL)
	{
		mysql_free_result(_res);
		_res = NULL;
	}

	if(!ensure_connection())
		return false;
	int query_result = mysql_real_query(&_mysql, sql, len);
	if(query_result!=0)
	{
		update_err();
		return false;
	}
	_execute_result = true;
	return true;
}
bool ut_mysql::query(const char* sql, uint32_t len)
{
	if(execute(sql, len))
	{
		_res = mysql_store_result(&_mysql);
		if(_res==NULL)
		{
			_row_count=0;
			_field_count = mysql_field_count(&_mysql);
			if(0!=_field_count)
			{
				_execute_result = false;
				update_err();
				return false;
			}
		}
		else
		{
			_row_count = static_cast<uint64_t>(mysql_num_rows(_res));
			_field_count = mysql_num_fields(_res);
		}
		return true;
	}
	return false;
}
bool ut_mysql::execute(MYSQL_STMT* stmt, MYSQL_BIND* param_bind)
{
	_execute_result = false;
	mysql_stmt_free_result(stmt);

	if(!ensure_connection())
		return false;
	if(NULL!=param_bind)
	{
		if(0!=mysql_stmt_bind_param(stmt, param_bind))
		{
			update_err(stmt);
			return false;
		}
	}
	int execute_result = mysql_stmt_execute(stmt);
	if(0!=execute_result)
	{
		update_err(stmt);
		return false;
	}
	_execute_result = true;
	return true;
}
bool ut_mysql::query(MYSQL_STMT* stmt, MYSQL_BIND* param_bind, MYSQL_BIND* result_bind)
{
	if(execute(stmt, param_bind))
	{
		if(NULL!=result_bind)
		{
			_field_count = mysql_stmt_field_count(stmt);
			//assert(_field_count>0);//if you pass a non-NULL @result_bind, field count should be greater than zero.
			if(0!=mysql_stmt_bind_result(stmt, result_bind))
			{
				update_err(stmt);
				return false;
			}
			if(0!=mysql_stmt_store_result(stmt))
			{
				_row_count = 0;
				_execute_result = false;
				update_err(stmt);
				return false;
			}
			else
			{
				_row_count = static_cast<uint64_t>(mysql_stmt_num_rows(stmt));
			}
		}
	}
	return true;
}

MYSQL_STMT* ut_mysql::create_and_prepare_stmt(const char* sql, uint32_t len)
{
	if(!ensure_connection())
		return NULL;

	MYSQL_STMT* stmt = mysql_stmt_init(&_mysql);
	if(NULL==stmt)
		return NULL;
	int result = mysql_stmt_prepare(stmt, sql, len);
	if(0!=result)
	{
		update_err();
		mysql_stmt_close(stmt);
		return NULL;
	}
	return stmt;
}
bool ut_mysql::next(MYSQL_STMT* stmt)
{
	int ret = mysql_stmt_fetch(stmt);
	if(0==ret)
		return true;
	else if(MYSQL_DATA_TRUNCATED==ret)
	{
		_err_code = CR_UNKNOWN_ERROR;
		static char mdt[] = "Data truncated(result bind buffer too small)";
		static int len = strlen(mdt);
		memcpy(_err_msg, mdt, len);
		_err_msg[len] = '\0';
	}
	else
		update_err(stmt);
	return false;
}
void ut_mysql::set_database(const std::string& host, const unsigned short port, const std::string& db_name, const std::string& user_name, const std::string& password, const std::string& charset, int32_t cnnt_param, int32_t cnnt_timeout, int32_t read_timeout, int32_t write_timeout)
{
	if(_connected)
		disconnect();
	_host = host;
	_port = port;
	_db_name = db_name;
	_user_name = user_name;
	_password = password;
	_charset = charset;
	_cnnt_param = cnnt_param;
	_cnnt_timeout = cnnt_timeout;
	_read_timeout = read_timeout;
	_write_timeout = write_timeout;
}
bool ut_mysql::ping()
{
	if(ensure_connection())
	{
		int result = mysql_ping(&_mysql);
		if(0!=result)
		{
			update_err();
			return false;
		}
		return true;
	}
	else
		return false;
}
void ut_mysql::update_err()
{
	_err_code=mysql_errno(&_mysql);
	if(0==_err_code)
	{
		_err_msg[0] = '\0';
	}
	else
	{
		strncpy(_err_msg, mysql_error(&_mysql), _err_msg_max_len-1);
		_err_msg[_err_msg_max_len-1] = '\0';
		if(err_is_connect_lost(_err_code))
			disconnect();
		ut_attr_inc(ATTR_ON_QUERY_ERR,1);
	}
}
void ut_mysql::update_err(MYSQL_STMT* stmt)
{
	_err_code=mysql_stmt_errno(stmt);
	if(0==_err_code)
	{
		_err_msg[0] = '\0';
	}
	else
	{
		strncpy(_err_msg, mysql_stmt_error(stmt), _err_msg_max_len-1);
		_err_msg[_err_msg_max_len-1] = '\0';
		if(err_is_connect_lost(_err_code))
			disconnect();
		ut_attr_inc(ATTR_ON_QUERY_ERR,1);
	}
}
void ut_mysql::on_connect()
{
}
void ut_mysql::on_disconnect()
{
}
void ut_mysql::on_mysql_options()
{
}
void ut_mysql::disconnect()
{
	if(_res!=NULL)
	{
		mysql_free_result(_res);
		_res=NULL;
	}
	if(_connected)
	{
		ut_attr_inc(ATTR_ON_DISCONNECT,1);
		if(NULL!=_on_disconnect_cb._member_function || NULL!=_on_disconnect_cb._static_function)
		{
			if(NULL!=_on_disconnect_hdlr)
				(_on_disconnect_hdlr->*(_on_disconnect_cb._member_function))(this);
			else
				(*(_on_disconnect_cb._static_function))(this);
		}
		else
		{
			on_disconnect();
		}
		mysql_close(&_mysql);
	}
	_connected=false;
	_execute_result = false;
}
bool ut_mysql::connect()
{
	if(_connected)
		return true;
	_execute_result = false;

	memset(&_mysql, 0, sizeof(_mysql));
	memset(&_row, 0, sizeof(_row));
	mysql_init(&_mysql);
	if(_charset.length())
		mysql_options(&_mysql,MYSQL_SET_CHARSET_NAME, _charset.c_str());
	if(_cnnt_timeout>0)
		mysql_options(&_mysql,MYSQL_OPT_CONNECT_TIMEOUT, &_cnnt_timeout);
	if(_read_timeout>0)
		mysql_options(&_mysql,MYSQL_OPT_READ_TIMEOUT, &_read_timeout);
	if(_write_timeout>0)
		mysql_options(&_mysql,MYSQL_OPT_WRITE_TIMEOUT, &_write_timeout);
	my_bool recnnt = 1;
	mysql_options(&_mysql, MYSQL_OPT_RECONNECT, &recnnt);
	if(NULL!=_on_mysql_options_cb._member_function || NULL!=_on_mysql_options_cb._static_function)
	{
		if(NULL!=_on_mysql_options_hdlr)
			(_on_mysql_options_hdlr->*(_on_mysql_options_cb._member_function))(this);
		else
			(*(_on_mysql_options_cb._static_function))(this);
	}
	else
	{
		on_mysql_options();
	}
	MYSQL *pMySql=mysql_real_connect(&_mysql, _host.c_str(), _user_name.c_str(), _password.c_str(), _db_name.length()?_db_name.c_str():NULL, _port, NULL, _cnnt_param);
	if (pMySql== NULL)
	{
		ut_attr_inc(ATTR_ON_CONNECT_FAIL,1);
		update_err();
		return false;
	}
	_connected=true;
	ut_attr_inc(ATTR_ON_CONNECT,1);
	if(NULL!=_on_connect_cb._member_function || NULL!=_on_connect_cb._static_function)
	{
		if(NULL!=_on_connect_hdlr)
			(_on_connect_hdlr->*(_on_connect_cb._member_function))(this);
		else
			(*(_on_connect_cb._static_function))(this);
	}
	else
	{
		on_connect();
	}
	return true;
}

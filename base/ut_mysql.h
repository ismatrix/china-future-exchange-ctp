/*
 * ut_mysql.h
 *
 *  Created on: 2013-9-25
 *      Author: echo
 */

#ifndef UT_MYSQL_H_
#define UT_MYSQL_H_

#include <string.h>
#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include <mysql/mysqld_error.h>
#include <string>
#include <sstream>

class ut_mysql
{
	typedef ut_mysql self_t;
protected:
	//types and constants for internal use only
	class handler_t { };
	union callback_t
	{
		void (*_static_function)(self_t*);
		void (handler_t::*_member_function)(self_t*);
	};

public:
	ut_mysql(void);
	virtual ~ut_mysql(void);

	//set mysql_real_connect connect parameter
	inline void set_connect_param(int param) { _cnnt_param = param;}

	///escape string to sql.
	inline uint64_t escape(char*to, const char* from, uint64_t len) { return mysql_real_escape_string(&_mysql, to, from, len); }

	///get the field index by field name. return -1 if field not found.
	int field_index(const std::string& name) const;

	/**
	 * get the value of the @index-th field or the field whose name is @name in current record row.
	 * if @index is invalid or @name is not found, NULL is returned.
	 */
	inline const char* field(const uint32_t index) const { if(index<field_count()) return _row[index]; return NULL; }
	inline const char* field(const std::string& name) const { int idx = field_index(name); if(idx>=0) return field(idx); return NULL; }

	/**
	 * get the length of the @index-th field or the field whose name is @name in current record row.
	 * if @index is invalid or @name is not found, 0 is returned.
	 */
	inline const uint64_t field_len(const uint32_t index) const { if(index<field_count()) return _field_len[index]; return 0; }
	inline const uint64_t field_len(const std::string& name) const { return field_len(field_index(name)); }

	/**
	 * get the value of the @index-th field or the field whose name is @name in current record row.
	 * if @index is invalid or @name is not found, return false.
	 */
	/*template<typename T>
	bool field(const uint32_t index, T& value) const
	{
		const char* sz = field(index);
		if (NULL == sz)
			return false;
		std::stringstream ss;
		ss.str(sz);
		ss >> value;
		return ss.good();
	}
	template<typename T>
	inline bool field(const std::string& name, T& value) { return field((uint32_t)field_index(name), value); }
	inline bool field(const uint32_t index, std::string& value) const
	{
		const char* sz = field(index);
		if (NULL == sz)
			return false;
		value = sz;
		return true;
	}*/

	/**
	 * make the cursor to the next row.
	 * Note: used for sql only, if you use this after statement execution, it will always return false.
	 * Note: use this method only after a successful query(sql, len) call, execute(sql, len) will not buffer the result set. check mysql manual mysql_fetch_row() for more detail.
	 */
	bool next();

	///execute the sql, usually used for INSERT,UPDATE,DELETE,REPLACE
	bool execute(const char* sql, uint32_t len);

	///execute a sql, and store the result, usually used for SELECT
	bool query(const char* sql, uint32_t len);

	/**
	 * get affected rows after a success sql execute or query.
	 * Note: this can not be used for mysql prepared statement, see affected_rows(MYSQL_STMT*) instead.
	 */
	inline uint64_t affected_rows() { return mysql_affected_rows(&_mysql); }

	/**
	 * execute a statement, usually used for INSERT,UPDATE,DELETE,REPLACE
	 * @param_bind is used to give parameters to this execution.
	 */
	bool execute(MYSQL_STMT* stmt, MYSQL_BIND* param_bind);

	/**
	 * execute a statement, and store the result, usually used for SEELCT
	 * @param_bind is used to give parameters to the statement execution.
	 * @result_bind is used to store the result after a successful execution.
	 */
	bool query(MYSQL_STMT* stmt, MYSQL_BIND* param_bind, MYSQL_BIND* result_bind);

	///get affected rows after a success statement execute or query
	inline uint64_t affected_rows(MYSQL_STMT* stmt) { return mysql_stmt_affected_rows(stmt); }

	///whether success in last query/execute.
	inline bool success() const { return _execute_result; }

	///create statement
	MYSQL_STMT* create_and_prepare_stmt(const char* sql, uint32_t len);

	/**
	 * fetch next on statement, same as next().
	 * return true if a new row fetched. return false if no more data or error occurred(check error code for what error)
	 * Note: only use this method on a statement which was successfully executed by query(MYSQL_STMT*) method, or you are sure that this method require the @stmt result set was totally buffered, check mysql manual mysql_stmt_fetch() for more detail.
	 */
	bool next(MYSQL_STMT* stmt);
	inline bool stmt_next(MYSQL_STMT* stmt) { return next(stmt); }

	///close statement
	inline void close_stmt(MYSQL_STMT* stmt) { mysql_stmt_free_result(stmt); mysql_stmt_close(stmt); }

	///set the connection parameters.
	void set_database(const std::string& host, const unsigned short port, const std::string& db_name, const std::string& user_name, const std::string& password, const std::string& charset, int32_t cnnt_param = 0, int32_t cnnt_timeout=-1, int32_t read_timeout=-1, int32_t write_timeout=-1);
	inline void set_database(const ut_mysql_cfg_t& cfg)
	{
		set_database(cfg._host, cfg._port, cfg._dbname, cfg._user, cfg._pswd, cfg._charset, cfg._cnnt_param, cfg._cnnt_timeout, cfg._read_timeout, cfg._write_timeout);
	}

	///get error message.
	inline const char* error_msg() const { return _err_msg; }

	///get error code.
	inline uint32_t error_code() const { return _err_code;	}
	inline bool error_cr_server_lost() const { return is_err_cr_server_lost(_err_code); }
	inline bool error_key_dup() const { return is_err_key_dup(_err_code); }

	///get result's row count
	inline uint64_t row_count() const { return _execute_result? _row_count : 0ull; }

	///get insert id.
	inline uint64_t last_insert_id() { return static_cast<uint64_t>(mysql_insert_id(&_mysql)); }

	///get result's field count.
	inline uint32_t field_count() const { return _execute_result? _field_count : 0; }

	///check whether the connection is still alive. return true if alive
	bool ping();

	///call this function to make sure there is a connection to server
	inline bool ensure_connection() { if(!_connected) return connect(); return true; }

	///close the connection to server if there is any
	inline void close_connection() { disconnect(); }

	///check whether connected
	inline bool connected() const { return _connected; }

	///get the mysql connection id
	inline uint64_t connection_id() { return mysql_thread_id(&_mysql); }

	/**
	 * update error code and error message
	 * Note: call this function only when the error message has not been stored automatically.
	 */
	void update_err();
	void update_err(MYSQL_STMT* stmt);

	/**
	 * set error code and message manually
	 * this will overwrite the current error code and message.
	 */
	inline void set_err(uint32_t err_code, const char* err_msg)
	{
		_err_code = err_code;
		strncpy(_err_msg, err_msg, _err_msg_max_len);
	}

	///check whether the @code is "lost database connection during query"
	inline static bool is_err_cr_server_lost(uint32_t code)
	{
		return CR_SERVER_LOST==code;
	}
	///check whether the @code is "record's primary/unique key duplicated"
	inline static bool is_err_key_dup(uint32_t code)
	{
		return ER_DUP_KEY==code || ER_DUP_ENTRY==code || ER_DUP_UNIQUE==code
			|| ER_DUP_ENTRY_AUTOINCREMENT_CASE==code;
	}

	/**
	 * set callback functions for these events: "connect", "disconnect", "mysql_options before connecting"
	 * @param handler	the callback method will be called on which class object
	 * @param function	the callback method
	 */
	template<typename handler_type, typename function_type>
	inline void set_on_connect(handler_type* handler, function_type function)
	{
		real_set_on_connect((handler_t*)handler, reinterpret_cast< void (handler_t::*)(self_t*) >(function));
	}
	template<typename function_type>
	inline void set_on_connect(function_type function)
	{
		real_set_on_connect(reinterpret_cast< void (*)(self_t*) >(function));
	}
	template<typename handler_type, typename function_type>
	inline void set_on_disconnect(handler_type* handler, function_type function)
	{
		real_set_on_disconnect((handler_t*)handler, reinterpret_cast< void (handler_t::*)(self_t*) >(function));
	}
	template<typename function_type>
	inline void set_on_disconnect(function_type function)
	{
		real_set_on_disconnect(reinterpret_cast< void (*)(self_t*) >(function));
	}
	template<typename handler_type, typename function_type>
	inline void set_on_mysql_options(handler_type* handler, function_type function)
	{
		real_set_on_mysql_options((handler_t*)handler, reinterpret_cast< void (handler_t::*)(self_t*) >(function));
	}
	template<typename function_type>
	inline void set_on_mysql_options(function_type function)
	{
		real_set_on_mysql_options(reinterpret_cast< void (*)(self_t*) >(function));
	}

protected:
	/**
	 * the default callback when connection established
	 * currently do nothing, you can:
	 * 	1. override this method, to do more things. or,
	 * 	2. use @set_on_connect() to set another callback function instead of this, and this method will never be called.
	 */
	virtual void on_connect();

	/**
	 * the default callback when connection lost
	 * currently do nothing, you can:
	 * 	1. override this method, to do more things. or,
	 * 	2. use @set_on_connect() to set another callback function instead of this, and this method will never be called.
	 */
	virtual void on_disconnect();

	/**
	 * the default callback when we can set mysql options before real connecting happening
	 * currently do nothing
	 */
	virtual void on_mysql_options();

	/**
	 * getters of private members.
	 * this is for internal use only.
	 */
	inline MYSQL& __intra_mysql() { return _mysql; }
	inline MYSQL_RES*& __intra_res() { return _res; }
	inline MYSQL_ROW& __intra_row() { return _row; }
	inline unsigned long*& __intra_field_len() { return _field_len; }
	inline uint64_t& __intra_row_count() { return _row_count; }
	inline uint32_t& __intra_field_count() { return _field_count; }
	inline bool& __intra_connected() { return _connected; }
	inline bool& __intra_execute_result() { return _execute_result; }
	inline std::string& __intra_host() { return _host; }
	inline uint16_t& __intra_port() { return _port; }
	inline std::string& __intra_db_name() { return _db_name; }
	inline std::string& __intra_user_name() { return _user_name; }
	inline std::string& __intra_password() { return _password; }
	inline std::string& __intra_charset() { return _charset; }
	inline int32_t& __intra_cnnt_param() { return _cnnt_param; }
	inline int32_t& __intra_cnnt_timeout() { return _cnnt_timeout; }
	inline int32_t& __intra_read_timeout() { return _read_timeout; }
	inline int32_t& __intra_write_timeout() { return _write_timeout; }
	inline handler_t*& __intra_on_connect_hdlr() { return _on_connect_hdlr; }
	inline callback_t& __intra_on_connect_cb() { return _on_connect_cb; }
	inline handler_t*& __intra_on_disconnect_hdlr() { return _on_disconnect_hdlr; }
	inline callback_t& __intra_on_disconnect_cb() { return _on_disconnect_cb; }
	inline handler_t*& __intra_on_mysql_options_hdlr() { return _on_mysql_options_hdlr; }
	inline callback_t& __intra_on_mysql_options_cb() { return _on_mysql_options_cb; }

private:
	inline void real_set_on_connect(handler_t* handler, void(handler_t::*member_function)(self_t*))
	{
		_on_connect_hdlr = handler;
		_on_connect_cb._member_function = member_function;
	}
	inline void real_set_on_connect(void(*static_function)(self_t*))
	{
		_on_connect_hdlr = NULL;
		_on_connect_cb._static_function = static_function;
	}
	inline void real_set_on_disconnect(handler_t* handler, void(handler_t::*member_function)(self_t*))
	{
		_on_disconnect_hdlr = handler;
		_on_disconnect_cb._member_function = member_function;
	}
	inline void real_set_on_disconnect(void(*static_function)(self_t*))
	{
		_on_disconnect_hdlr = NULL;
		_on_disconnect_cb._static_function = static_function;
	}
	inline void real_set_on_mysql_options(handler_t* handler, void(handler_t::*member_function)(self_t*))
	{
		_on_mysql_options_hdlr = handler;
		_on_mysql_options_cb._member_function = member_function;
	}
	inline void real_set_on_mysql_options(void(*static_function)(self_t*))
	{
		_on_mysql_options_hdlr = NULL;
		_on_mysql_options_cb._static_function = static_function;
	}

private:
	//mysql data
	MYSQL _mysql;
	MYSQL_RES *_res;
	MYSQL_ROW _row;
	unsigned long* _field_len;
	uint64_t _row_count;
	uint32_t _field_count;

	//mysql running status
	bool _connected;
	bool _execute_result;

	//mysql client parameters
	std::string _host;
	uint16_t _port;
	std::string _db_name;
	std::string _user_name;
	std::string _password;
	std::string _charset;
	int32_t _cnnt_param;
	int32_t _cnnt_timeout;
	int32_t _read_timeout;
	int32_t _write_timeout;

	//error related data
	static const char _null_string[1];
	static const uint32_t _err_msg_max_len = 512;
	char _err_msg[_err_msg_max_len];
	uint32_t _err_code;

	//callback
	handler_t* _on_connect_hdlr;
	callback_t _on_connect_cb;
	handler_t* _on_disconnect_hdlr;
	callback_t _on_disconnect_cb;
	handler_t* _on_mysql_options_hdlr;
	callback_t _on_mysql_options_cb;

private:
	void disconnect();
	bool connect();

private:
	ut_mysql(const ut_mysql& rhs);
	ut_mysql& operator=(const ut_mysql& rhs);
};

//some useful macro for mysql statement bind
#define ut_mysql_bind_int8(bind, data) do { bind.buffer_type=MYSQL_TYPE_TINY; bind.buffer=const_cast<void*>((const void*)(&data)); bind.is_null=0; bind.length=NULL; } while(0);
#define ut_mysql_bind_uint8(bind, data) do { ut_mysql_bind_int8(bind,data) bind.is_unsigned=1; } while(0);
#define ut_mysql_bind_int16(bind, data) do { bind.buffer_type=MYSQL_TYPE_SHORT; bind.buffer=const_cast<void*>((const void*)(&data)); bind.is_null=0; bind.length=NULL; } while(0);
#define ut_mysql_bind_uint16(bind, data) do { ut_mysql_bind_int16(bind,data) bind.is_unsigned=1; } while(0);
#define ut_mysql_bind_int32(bind, data) do { bind.buffer_type=MYSQL_TYPE_LONG; bind.buffer=const_cast<void*>((const void*)(&data)); bind.is_null=0; bind.length=NULL; } while(0);
#define ut_mysql_bind_uint32(bind, data) do { ut_mysql_bind_int32(bind,data) bind.is_unsigned=1; } while(0);
#define ut_mysql_bind_int64(bind, data) do { bind.buffer_type=MYSQL_TYPE_LONGLONG; bind.buffer=const_cast<void*>((const void*)(&data)); bind.is_null=0; bind.length=NULL; } while(0);
#define ut_mysql_bind_uint64(bind, data) do { ut_mysql_bind_int64(bind,data) bind.is_unsigned=1; } while(0);
#define ut_mysql_bind_double(bind, data) do { bind.buffer_type=MYSQL_TYPE_DOUBLE; bind.buffer=const_cast<void*>((const void*)(&data)); bind.is_null=0; bind.length=NULL; } while(0);
///@var is a variable holding the data length. using @var_name_suffix to automatically create a local variable holding the data length.
#define ut_mysql_bind_str(bind, str, len, var) do { if(len>=0) var=len; bind.buffer_type=MYSQL_TYPE_STRING; bind.buffer=const_cast<void*>((const void*)(str)); bind.is_null=0; bind.buffer_length=len; bind.length=&var; } while(0);
#define ut_mysql_bind_str2(bind, str, len, var_name_suffix) unsigned long bind_len_##var_name_suffix = len; ut_mysql_bind_str(bind, str, len, bind_len_##var_name_suffix);
#define ut_mysql_bind_str_const(bind, str, var) do { bind.buffer_type=MYSQL_TYPE_STRING; bind.buffer=const_cast<void*>((const void*)(str)); bind.is_null=0; bind.buffer_length=var; bind.length=const_cast<unsigned long*>(&var); } while(0);
#define ut_mysql_bind_blob(bind, str, len, var) do { if(len>=0) var=len; bind.buffer_type=MYSQL_TYPE_BLOB; bind.buffer=const_cast<void*>((const void*)(str)); bind.is_null=0; bind.buffer_length=len; bind.length=&var; } while(0);
#define ut_mysql_bind_blob2(bind, str, len, var_name_suffix) unsigned long bind_len_##var_name_suffix = len; ut_mysql_bind_blob(bind, str, len, bind_len_##var_name_suffix);
#define ut_mysql_bind_blob_const(bind, str, var) do { bind.buffer_type=MYSQL_TYPE_BLOB; bind.buffer=const_cast<void*>((const void*)(str)); bind.is_null=0; bind.buffer_length=var; bind.length=const_cast<unsigned long*>(&var); } while(0);

#endif /* UT_MYSQL_H_ */

/*
 * nw_evt.h
 *
 *  Created on: 2016-09-27
 *      Author: dreamyzhang
 *       Brief: event interface for session_io/timer and so on.
 *       		the open source event library of libev will be wrapped into nw_ses_evt interface.
 *       		Currently, only libev is supported.
 */

#ifndef __NW_EVT_H__
#define __NW_EVT_H__

#include <string.h>
#include <ev.h>


///the basic constant definitions
struct nw_evt_def
{
	//event bit
	static const int _b_read = 0x1;
	static const int _b_write = 0x2;

	//type
	static const int _t_common = 0x0;	//common connections
	static const int _t_client = 0x1;	//for clients connecting to server
	static const int _t_server = 0x2;	//for server accepting connections

	//backend
	static const int _be_epoll = 0x1;
	static const int _be_select = 0x2;
	struct loop_param_t
	{
		int backend;
	};
};


struct nw_ses_libev
{
	ev_io _ev;

	inline nw_ses_libev() { memset(&_ev, 0, sizeof(_ev)); }
	inline bool evt_is_active() const { return ev_is_active(&_ev); }
	void evt_set(void* loop, int event, int type); //not recommended, evt_restart_if_needed() is more efficient
	void evt_restart_if_needed(void* loop, int event, int type);
	inline void evt_start(void* loop) { ev_io_start((struct ev_loop*)loop, &_ev); }
	inline void evt_stop(void* loop) { ev_io_stop((struct ev_loop*)loop, &_ev); }
};

struct nw_tm_libev
{
	ev_timer _ev;

	nw_tm_libev();
	inline bool evt_is_active() const { return ev_is_active(&_ev); }
	inline void evt_set(double timeval) { ev_timer_set(&_ev, timeval, timeval); }
	inline void evt_start(void* loop) { ev_timer_again((struct ev_loop*)loop, &_ev); }
	inline void evt_stop(void* loop) { ev_timer_stop((struct ev_loop*)loop, &(_ev)); }
	inline double timeval() const { return _ev.repeat; }
	inline void timeval(double tv) { _ev.repeat = tv; }
};

struct nw_wkr_libev
{
	ev_async _ev;

	nw_wkr_libev(bool iev);
	inline bool evt_is_active() const { return ev_is_active(&_ev); }
	inline void evt_start(void* loop) { ev_async_start((struct ev_loop*)loop, &_ev); }
	inline void evt_stop(void* loop) { ev_async_stop((struct ev_loop*)loop, &_ev); }
	inline void evt_notify(void* loop) { ev_async_send((struct ev_loop*)loop, &_ev); }
};

struct nw_async_libev
{
	typedef void (*callback_t)(nw_async_libev*, void*);
	ev_async _ev;
	callback_t _cb;
	void* _data;

	nw_async_libev(callback_t cb, void* data);
	inline bool evt_is_active() const { return ev_is_active(&_ev); }
	inline void evt_start(void* loop) { ev_async_start((struct ev_loop*)loop, &_ev); }
	inline void evt_stop(void* loop) { ev_async_stop((struct ev_loop*)loop, &_ev); }
	inline void evt_notify(void* loop) { ev_async_send((struct ev_loop*)loop, &_ev); }
};

struct nw_child_libev
{
	typedef void (*callback_t)(nw_child_libev*, int pid, int status);
	ev_child _ev;

	nw_child_libev();
	inline bool evt_is_active() const { return ev_is_active(&_ev); }
	inline void evt_set(int pid, int trace) { ev_child_set(&_ev, pid, trace); }
	inline void evt_start(void* loop) { ev_child_start((struct ev_loop*)loop, &_ev); }
	inline void evt_stop(void* loop) { ev_child_stop((struct ev_loop*)loop, &_ev); }
	inline int pid() const { return _ev.pid; }
};

struct nw_prepare_libev
{
	typedef void (*callback_t)(nw_prepare_libev*, void*);
	ev_prepare _ev;
	callback_t _cb;
	void* _data;

	nw_prepare_libev(callback_t cb, void* data);
	inline bool evt_is_active() const { return ev_is_active(&_ev); }
	inline void evt_start(void* loop) { ev_prepare_start((struct ev_loop*)loop, &_ev); }
	inline void evt_stop(void* loop) { ev_prepare_stop((struct ev_loop*)loop, &_ev); }
};

struct nw_check_libev
{
	typedef void (*callback_t)(nw_check_libev*, void*);
	ev_check _ev;
	callback_t _cb;
	void* _data;

	nw_check_libev(callback_t cb, void* data);
	inline bool evt_is_active() const { return ev_is_active(&_ev); }
	inline void evt_start(void* loop) { ev_check_start((struct ev_loop*)loop, &_ev); }
	inline void evt_stop(void* loop) { ev_check_stop((struct ev_loop*)loop, &_ev); }
};

struct nw_io_libev
{
	ev_io _ev;
	void* _data;
	typedef void (*func_t)(void* data, bool can_read, bool can_write);
	func_t _func;

	inline nw_io_libev():_data(NULL), _func(NULL) { memset(&_ev, 0, sizeof(_ev)); }

	template<typename function_type>
	inline void set_callback(function_type function, void* data) { _func = (func_t)function; _data = data; }

	inline bool evt_is_active() const { return ev_is_active(&_ev); }
	void evt_set(int fd, void* loop, int event); //not recommended, evt_restart_if_needed() is more efficient
	void evt_restart_if_needed(int fd, void* loop, int event);
	inline int evt_get_fd(void) const { return _ev.fd; }
	inline void evt_start(void* loop) { ev_io_start((struct ev_loop*)loop, &_ev); }
	inline void evt_stop(void* loop) { ev_io_stop((struct ev_loop*)loop, &_ev); }
};

typedef nw_ses_libev nw_ses_evt;
typedef nw_tm_libev nw_tm_evt;
typedef nw_wkr_libev nw_wkr_evt;
typedef nw_async_libev nw_async_evt;
typedef nw_child_libev nw_child_evt;
typedef nw_prepare_libev nw_prepare_evt;
typedef nw_check_libev nw_check_evt;
typedef nw_io_libev nw_io_evt;

///global loop function using default library
void* nw_loop_create(nw_evt_def::loop_param_t* param = NULL);
void nw_loop_run(void* loop);
void nw_loop_break(void* loop);
void nw_loop_destroy(void* loop);


#endif 







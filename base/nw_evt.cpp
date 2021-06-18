/*
 * nw_evt.cpp
 *
 *  Created on: 2013-9-27
 *      Author: echo
 */
#include "nw_evt.h"
#include <assert.h>

#ifdef _nw_event_support_libev

static const int libev_evt_ses_offset2 = ut_offsetof(nw_ses, _evt);
static const int libev_evt_ses_offset = libev_evt_ses_offset2 + ut_offsetof(nw_ses_libev, _ev);

#define libev_evt_ses_inst nw_ses* ses = (nw_ses*)( (uint8_t*)watcher-libev_evt_ses_offset )
static void libev_on_common_evt_msghdr(struct ev_loop *loop, ev_io *watcher, int events)
{
	libev_evt_ses_inst;
	if(events | EV_READ)
		return ses->__on_can_recvmsg();
}
static void libev_on_common_evt_stream(struct ev_loop *loop, ev_io *watcher, int events)
{
	libev_evt_ses_inst;
	if(events & EV_READ)
		ses->__on_can_read_stream();
	if(events & EV_WRITE)
		ses->__on_can_write_stream();
}
static void libev_on_common_evt_dgram(struct ev_loop *loop, ev_io *watcher, int events)
{
	libev_evt_ses_inst;
	if(events | EV_READ)
		return ses->__on_can_read_dgram();
}
static void libev_on_client_evt(struct ev_loop *loop, ev_io *watcher, int events)
{
	libev_evt_ses_inst;
	if(events | EV_WRITE)
		return ses->__on_can_connect();
}
static void libev_on_server_evt(struct ev_loop *loop, ev_io *watcher, int events)
{
	libev_evt_ses_inst;
	if(events | EV_READ)
		return ses->__on_can_accept();
}
#define libev_evt_ses_inst2 nw_ses* ses = (nw_ses*)( (uint8_t*)this-libev_evt_ses_offset2 )
void nw_ses_libev::evt_set(void* loop, int event, int type)
{
	libev_evt_ses_inst2;
	typedef void (*cb_t)(struct ev_loop *loop, ev_io *watcher, int events);
	cb_t cb = NULL;
	switch(type)
	{
	case nw_evt_def::_t_common:
		if(ses->_io->_msghdr)
		{
			cb = libev_on_common_evt_msghdr;
			event &= ~EV_WRITE;
		}
		else
		{
			if(ses->_io->_type==SOCK_STREAM || ses->_io->_type==SOCK_SEQPACKET)
				cb = libev_on_common_evt_stream;
			else if(ses->_io->_type==SOCK_DGRAM)
				cb = libev_on_common_evt_dgram;
			else
				assert(0);
		}
		break;
	case nw_evt_def::_t_client: cb = libev_on_client_evt; break;
	case nw_evt_def::_t_server: cb = libev_on_server_evt; break;
	default: assert(false); break;
	}
	int events = EV_NONE;
	if(event & nw_evt_def::_b_read)
		events |= EV_READ;
	if(event & nw_evt_def::_b_write)
		events |= EV_WRITE;
	ev_io_init(&_ev, cb, ses->_io->_s, events);
}
void nw_ses_libev::evt_restart_if_needed(void* loop, int event, int type)
{
	//check whether event bits changed
	int events = EV_NONE;
	if(event & nw_evt_def::_b_read)
		events |= EV_READ;
	if(event & nw_evt_def::_b_write)
		events |= EV_WRITE;
	if(events==_ev.events && ev_is_active(&_ev))
		return;
	//decide which nw_ses and callback will be used
	libev_evt_ses_inst2;
	typedef void (*cb_t)(struct ev_loop *loop, ev_io *watcher, int events);
	cb_t cb = NULL;
	switch(type)
	{
	case nw_evt_def::_t_common:
		if(ses->_io->_msghdr)
		{
			cb = libev_on_common_evt_msghdr;
			event &= ~EV_WRITE;
		}
		else
		{
			if(ses->_io->_type==SOCK_STREAM || ses->_io->_type==SOCK_SEQPACKET)
				cb = libev_on_common_evt_stream;
			else if(ses->_io->_type==SOCK_DGRAM)
				cb = libev_on_common_evt_dgram;
			else
				assert(0);
		}
		break;
	case nw_evt_def::_t_client: cb = libev_on_client_evt; break;
	case nw_evt_def::_t_server: cb = libev_on_server_evt; break;
	default: assert(false); break;
	}
	//modify the ev, and start it again. if ev is active, stop it first before modification
	if(ev_is_active(&_ev))
	{
		ev_io_stop((struct ev_loop*)loop, &_ev);
		ev_io_set(&_ev, ses->_io->_s, events);
		ev_set_cb(&_ev, cb);
	}
	else
	{
		ev_io_init(&_ev, cb, ses->_io->_s, events);
	}
	ev_io_start((struct ev_loop*)loop, &_ev);
}

static const int libev_evt_tm_offset = nw_timer::__evt_offset() + ut_offsetof(nw_tm_libev, _ev);
static void libev_on_timer(struct ev_loop *loop, ev_timer *watcher, int events)
{
	nw_timer* tm = (nw_timer*)( (uint8_t*)watcher-libev_evt_tm_offset );
	return tm->__call();
}
nw_tm_libev::nw_tm_libev()
{
	memset(&_ev, 0, sizeof(_ev));
	ev_init(&_ev, libev_on_timer);
}

static const int libev_ievt_wkr_offset = nw_worker::__ievt_offset() + ut_offsetof(nw_wkr_libev, _ev);
static const int libev_oevt_wkr_offset = nw_worker::__oevt_offset() + ut_offsetof(nw_wkr_libev, _ev);
static const int libev_evt_wkr_offset = ut_offsetof(nw_wkr_libev, _ev);
static void libev_on_inotify(struct ev_loop *loop, ev_async *watcher, int events)
{
	nw_worker* wkr = (nw_worker*)( (uint8_t*)watcher-libev_ievt_wkr_offset );
	nw_wkr_libev* eo = (nw_wkr_libev*)( (uint8_t*)watcher-libev_evt_wkr_offset );
	wkr->__on_notify(eo);
}
static void libev_on_onotify(struct ev_loop *loop, ev_async *watcher, int events)
{
	nw_worker* wkr = (nw_worker*)( (uint8_t*)watcher-libev_oevt_wkr_offset );
	nw_wkr_libev* eo = (nw_wkr_libev*)( (uint8_t*)watcher-libev_evt_wkr_offset );
	wkr->__on_notify(eo);
}
nw_wkr_libev::nw_wkr_libev(bool iev)
{
	memset(&_ev, 0, sizeof(_ev));
	if(iev)
		ev_async_init(&_ev, libev_on_inotify);
	else
		ev_async_init(&_ev, libev_on_onotify);
}

static void libev_on_async(struct ev_loop *loop, ev_async *watcher, int events)
{
	nw_async_libev* nw = (nw_async_libev*)( (uint8_t*)watcher - ut_offsetof(nw_async_libev, _ev) );
	(nw->_cb)(nw, nw->_data);
}
nw_async_libev::nw_async_libev(callback_t cb, void* data)
:_cb(cb), _data(data)
{
	memset(&_ev, 0, sizeof(_ev));
	ev_async_init(&_ev, libev_on_async);
}

static const int libev_evt_child_offset = nw_child::__evt_offset() + ut_offsetof(nw_child_libev, _ev);
static void libev_on_child(struct ev_loop *loop, ev_child *watcher, int events)
{
	nw_child_libev* nw = (nw_child_libev*)( (uint8_t*)watcher - ut_offsetof(nw_child_libev, _ev) );
	nw_child* cld = (nw_child*)( (uint8_t*)watcher-libev_evt_child_offset );
	cld->__call(nw->_ev.rpid, nw->_ev.rstatus);
}
nw_child_libev::nw_child_libev()
{
	memset(&_ev, 0, sizeof(_ev));
	ev_child_init(&_ev, libev_on_child, 0, 0);
}

static void libev_on_prepare(struct ev_loop *loop, ev_prepare *watcher, int events)
{
	nw_prepare_libev* nw = (nw_prepare_libev*)( (uint8_t*)watcher - ut_offsetof(nw_prepare_libev, _ev) );
	(nw->_cb)(nw, nw->_data);
}
nw_prepare_libev::nw_prepare_libev(callback_t cb, void* data)
:_cb(cb), _data(data)
{
	memset(&_ev, 0, sizeof(_ev));
	ev_prepare_init(&_ev, libev_on_prepare);
}

static void libev_on_check(struct ev_loop *loop, ev_check *watcher, int events)
{
	nw_check_libev* nw = (nw_check_libev*)( (uint8_t*)watcher - ut_offsetof(nw_check_libev, _ev) );
	(nw->_cb)(nw, nw->_data);
}
nw_check_libev::nw_check_libev(callback_t cb, void* data)
:_cb(cb), _data(data)
{
	memset(&_ev, 0, sizeof(_ev));
	ev_check_init(&_ev, libev_on_check);
}

static void libev_on_io_libev(struct ev_loop *loop, ev_io *watcher, int events)
{
	nw_io_libev* evt = (nw_io_libev*)watcher;
	(evt->_func)(evt->_data, events&EV_READ, events&EV_WRITE);
}
void nw_io_libev::evt_set(int fd, void* loop, int event)
{
	int events = EV_NONE;
	if(event & nw_evt_def::_b_read)
		events |= EV_READ;
	if(event & nw_evt_def::_b_write)
		events |= EV_WRITE;
	ev_io_init(&_ev, libev_on_io_libev, fd, events);
}
void nw_io_libev::evt_restart_if_needed(int fd, void* loop, int event)
{
	//check whether event bits changed
	int events = EV_NONE;
	if(event & nw_evt_def::_b_read)
		events |= EV_READ;
	if(event & nw_evt_def::_b_write)
		events |= EV_WRITE;
	if(events==_ev.events && fd==_ev.fd && ev_is_active(&_ev))
		return;
	//modify the ev, and start it again. if ev is active, stop it first before modification
	if(ev_is_active(&_ev))
	{
		ev_io_stop((struct ev_loop*)loop, &_ev);
		ev_io_set(&_ev, fd, events);
	}
	else
	{
		ev_io_init(&_ev, libev_on_io_libev, fd, events);
	}
	ev_io_start((struct ev_loop*)loop, &_ev);
}

void* nw_loop_create(nw_evt_def::loop_param_t* param)
{
	int flag = EVFLAG_AUTO;
	if(NULL!=param)
	{
		if(param->backend & nw_evt_def::_be_epoll)
			flag |= EVBACKEND_EPOLL;
		else if(param->backend & nw_evt_def::_be_select)
			flag |= EVBACKEND_SELECT;
	}
	else
		flag |= EVBACKEND_EPOLL;
	return ev_loop_new(flag);
}

void nw_loop_run(void* loop)
{
	struct ev_loop* lp = (struct ev_loop*)(loop);
#if EV_VERSION_MAJOR>3
	ev_run(lp, 0);
#else
	ev_loop(lp, 0);
#endif
}
void nw_loop_break(void* loop)
{
#if EV_VERSION_MAJOR>3
	ev_break((struct ev_loop*)loop, EVBREAK_ALL);
#else
	ev_unloop((struct ev_loop*)loop, EVUNLOOP_ALL);
#endif
}
void nw_loop_destroy(void* loop)
{
	ev_loop_destroy((struct ev_loop*)loop);
}

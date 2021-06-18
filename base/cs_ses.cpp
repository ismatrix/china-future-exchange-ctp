/*************************************************************************
    > File Name: nw_ses.cpp
    > Created Time: Tue 27 Sep 2016 05:05:25 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#include<iostream>
#include "cs_ses.h"

using namespace std;

int nw_ses::connect()
{
	on_connect();
	if(csocket::connect() > 0)	
	{
		on_connected();
		return	true;
	}
	else
	{
		__watch_connect();
	}
	return false;
}


int nw_ses::listen()
{
	if(listen()==0)
	{
		__watch_accept();
		return true;
	}
	return false;
}

int nw_ses::accept()
{
	csocket client_socket;
	if(accept(client_socket) == 0)
	{
		on_accept(true);
		__watch_read();
		return 0;
	}
	else
	{
		on_accept(false);
	}
}

int nw_ses::send()
{
	__watch_read_and_write();
}

void connect_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	//读事件	
}

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) 
{
	//创建一个客户端session
	struct sockaddr_in client_addr;  
    socklen_t client_len = sizeof(client_addr)

	struct ev_io *client_watcher = (struct ev_io*) malloc(sizeof(struct ev_io))
		
	ev_io_init(client_watcher, socket_read_callback, client_sd, EV_READ);  
	ev_io_start(loop, client_watcher);  
}

void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) 
{

}

void write_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) 
{

}



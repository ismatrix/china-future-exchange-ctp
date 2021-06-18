/*************************************************************************
    > File Name: server.cpp
    > Created Time: Tue 27 Sep 2016 04:31:03 PM CST
    > Author: 
    > description:   inherit csocket, for async callback function. 
 ************************************************************************/

#include "csocket.h"
#include "nw_evt.h"

using namespace std;

//http://blog.csdn.net/hepeng597/article/details/7803277


//这里还是不区分类型
class nw_ses : public csocket
{
	public:
		nw_ses(){_loop = NULL};
	
		bool  connect();
		bool  listen();
		void  accept();
		
		int recv();
		int send();
	protected:

		//the callback function on the corresponding operation, subclass can realize this operations.
		virtual void on_close();
		virtual void on_closeed();
		virtual void on_connect();
		virtual void on_connected();
		virtual void on_accept();
		virtual void on_recv();
		virtual void on_send();

	
	private:
		ev_io	  _ev;
		void*	  _loop;	//loop handle

		//restart libev event
		inline void	__watch_read();
		inline void	__watch_read_and_write();
		inline void	__watch_connect();
		inline void	__watch_accept();
};


void connect_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);  
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);  
void write_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);  




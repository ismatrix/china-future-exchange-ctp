/*************************************************************************
  > File Name: csocket.h
  > Created Time: Wed 24 Aug 2016 08:51:59 AM CST
  > Author: 
  > description: 
 ************************************************************************/
#ifndef __CSOCKET_H__
#define __CSOCKET_H__

#include <string>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string.h>
#include <net/if.h>
#include<sys/ioctl.h>
#include <netdb.h>


using namespace std;


int get_peer_addr(int _socket_fd, in_addr_t & peer_addr, uint16_t & peer_port); //获取sockfd对端信息
int  get_sock_addr(int _socket_fd, in_addr_t& socket_addr, uint16_t & socket_port);//获取sockfd本端信息
string  get_addr_str(in_addr_t addr);	//获取字符串ip
in_addr_t get_addr(char* ip);			//获取in_addr_t地址信息
int32_t get_local_ip_list(uint32_t* ip_list, uint32_t max_count);	//获取本机的ip列表
uint32_t get_host_by_name(const char* name);		//获取域名对应ip 返回第一个
int get_local_mac(const char *eth, char *mac);		//获取mac地址
int get_local_ip(const char *eth, char *ip);		//获取本机网口对应ip

class CSocket
{
	public:
		CSocket(){_socket_fd = -1;}
		int  create(int protocol_family = PF_INET, int socket_type = SOCK_STREAM);

		int bind(uint16_t port);
		int bind(in_addr_t addr, uint16_t port);
		int bind(string ip, uint16_t port);
		int listen(int backlog = 32);
		int connect(in_addr_t addr, uint16_t port);
		int connect(string ip, uint16_t port);
		int accept(CSocket& client_socket);
		int shutdown(int how = SHUT_RDWR);
		int close();

		int receive(void * buf, size_t len, int flag = 0);
		int send(const void * buf, size_t len, int flag = 0);

		int receive_from (void* buf, size_t len, in_addr_t& from_addr, uint16_t& from_port, int flags = 0);
		int send_to(const void* msg, size_t len, in_addr_t to_addr, uint16_t to_port, int flags = 0);

		int set_nonblock();
		int set_reuseaddr();
	
		int get_socket_fd(){return _socket_fd;};
		static void ignore_pipe_signal() {signal(SIGPIPE, SIG_IGN);}
		char*  get_errmsg(){return strerror(errno);};
	protected:
		int _socket_fd;
};



#endif




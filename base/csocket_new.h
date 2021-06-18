/*************************************************************************
  > File Name: csocket.h
  > Created Time: Wed 24 Aug 2016 08:51:59 AM CST
  > Author: 
  > description: simple ocket function. contain tcp4, tcp6, udp, unix support.
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
#include <sys/un.h>


using namespace std;

#define IPV4_LEN 16
#define IPV6_LEN 64

int get_peer_addr(int _s, in_addr_t & peer_addr, uint16_t & peer_port); //获取sockfd对端信息
int  get_sock_addr(int _s, in_addr_t& socket_addr, uint16_t & socket_port);//获取sockfd本端信息
string  get_addr_str(in_addr_t addr);	//获取字符串ip
in_addr_t get_addr(char* ip);			//获取in_addr_t地址信息
int32_t get_local_ip_list(uint32_t* ip_list, uint32_t max_count);	//获取本机的ip列表
uint32_t get_host_by_name(const char* name);		//获取域名对应ip 返回第一个
int get_local_mac(const char *eth, char *mac);		//获取mac地址
int get_local_ip(const char *eth, char *ip);		//获取本机网口对应ip

typedef class address  
{
	private:  
		short int sin_family; //support for the three address family AF_INET or AF_INET6 or AF_UNIX. 
		union  
		{
			struct sockaddr_un	_unixaddr_t;
			struct sockaddr_in6 _ipv6addr_t;
			struct sockaddr_in  _ipv4addr_t;
		}addr;  

	public:  
		address(){sin_family = -1;};  
		bool operator == (const address &dst) const {return (*this != dst? false : true );};  
		bool operator != (const address &dst) const {return (*this == dst? false : true );}; 
		int get_family() const {return sin_family;}  
		const char* get_binary_data() {return (char*)&addr;};
		
		int get_socket(int type/*SOCK_STREAM/SOCK_DGRAM*/)
		{
			if(sin_family == -1) return -1;
			return socket(sin_family, type, 0);
		};

		bool set_from_readable_address(const char* readable_address/*=127.0.0.1*/, uint32_t port=0)
		{
			if(NULL == readable_address) return false;
			if(port == 0)
			{
				sin_family = AF_UNIX;
				addr._unixaddr_t.sun_family = sin_family;
				int len = strlen(readable_address);
				memcpy(addr._unixaddr_t.sun_path, readable_address, len);
				addr._unixaddr_t.sun_path[len] = '\0';
				return true;
			}
			int cnt = 0;char* p=(char*)readable_address;
			for(; *p!='\0'; p++) if(*p == ':') cnt++;
			if(cnt>=1)
			{
				sin_family = AF_INET6;  
				if(inet_pton(sin_family, readable_address, &addr._ipv6addr_t.sin6_addr) <= 0)  return false;
				addr._ipv6addr_t.sin6_port = htonl(port);
				addr._ipv6addr_t.sin6_family = sin_family;
			}
			else
			{
				sin_family = AF_INET;  
				if( inet_pton(sin_family, readable_address, &addr._ipv4addr_t.sin_addr) <= 0) return false;  
				addr._ipv4addr_t.sin_port = htonl(port);
				addr._ipv4addr_t.sin_family = sin_family;
			}
		};  
		string get_readable_address()
		{
			if(AF_INET == sin_family)
			{
				char ip[INET_ADDRSTRLEN];
				if(NULL==inet_ntop(sin_family, &((struct sockaddr_in*)&addr)->sin_addr, ip, sizeof(ip))) return "";
				char s[INET_ADDRSTRLEN+10];
				snprintf(s, sizeof(s), "%s:%u", ip, ntohs(((struct sockaddr_in*)&addr)->sin_port));
				return s;
			}
			else if(AF_INET6 == sin_family)
			{
				char ip[INET6_ADDRSTRLEN];
				if(NULL==inet_ntop(sin_family, &((struct sockaddr_in6*)&addr)->sin6_addr, ip, sizeof(ip))) return "";
				char s[INET6_ADDRSTRLEN+10];
				snprintf(s, sizeof(s), "%s:%u", ip, ntohs(((struct sockaddr_in6*)&addr)->sin6_port));
				return s;
			}
			else if(AF_UNIX == sin_family)
			{
				return "unix" + std::string(":") + ((struct sockaddr_un *)&addr)->sun_path;
			}
			else
			{
				return "unknow sin_family.";
			}
		}

}_address;  


class csocket
{
	public:
		csocket(){_s = -1;}

		//automatic recgnise ipv4 or ipv6
		int  create_udp(const char* ip, uint16_t port);
		int  create_tcp(const char* ip, uint16_t port);
		int  create_tcp_unix(const char* unixpath);

		int bind();
		
		int listen(int backlog = 32);
	
		//connect to the socket addr
		virtual int connect();
		
		virtual int accept(csocket& client_socket);
	
		//force close the connection 
		int shutdown(int how = SHUT_RDWR);
		//close the connection
		int close();

		//using tcp socket for send or recv message.
		virtual int receive(void * buf, size_t len, int flag = 0);
		virtual int send(const void * buf, size_t len, int flag = 0);

		//using udp socket for send or recv message; 
		virtual int recvfrom (void* buf, size_t len, in_addr_t& from_addr, uint16_t& from_port, int flags = 0);
		virtual int sendto(const void* msg, size_t len, in_addr_t to_addr, uint16_t to_port, int flags = 0);

		//using unix socket for send handle
		virtual ssize_t sendmsg(const struct msghdr *msg, int flags);  
		virtual ssize_t recvmsg(struct msghdr *msg, int flags);  

		//set delay for the socket handle.
		inline int set_no_delay();
		inline int set_reuseaddr();
		inline int set_nonblock();
			
		int get_socket(){return _s;};
		static void ignore_pipe_signal() {signal(SIGPIPE, SIG_IGN);}
		char*  get_errmsg(){return strerror(errno);};
	
	protected:
		int _s;
		_address _addr;
	
	private:
		int  create(int type, string ip, uint16_t port);

};



#endif




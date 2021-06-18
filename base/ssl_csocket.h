/*************************************************************************
  > File Name: ssl_csocket.h
  > Created Time: Wed 24 Aug 2016 08:51:59 AM CST
  > Author:		dreamyzhang 
  > description: 
 ************************************************************************/

#ifndef __SSL_CSOCKET_H__
#define __SSL_CSOCKET_H__

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

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "csocket.h"

using namespace std;

//1 sock create
//2 connect
//3 ssl bind
//4 ssl send receive
//5 ssl close
//6 sock close

class CSocketSSL : public CSocket
{
	public:
		CSocketSSL(){ssl = NULL;}
		~CSocketSSL(){ssl_close();}
		
		int ssl_create_bind_socket();
		int ssl_send(const void * buf, size_t len);
		int ssl_receive(void * buf, size_t len);
		int ssl_acpect(CSocketSSL& client_socket);
		int ssl_close();

		void show_certs(SSL* ssl);
	private:
		SSL_CTX* ctx;
		SSL* ssl;
};

#endif



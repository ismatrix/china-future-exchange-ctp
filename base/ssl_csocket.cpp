/*************************************************************************
  > File Name: csocket.cpp
  > Created Time: Wed 24 Aug 2016 09:03:01 AM CST
  > Author: 
  > description: 
 ************************************************************************/

#include "ssl_csocket.h"


void show_certs(SSL* ssl)
{
	X509* cert = SSL_get_peer_certificate(ssl);		/* get the server's certificate */
	if( cert != NULL )
	{
		char* line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		free(line);      /* free the malloc'ed string */

		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		free(line);      /* free the malloc'ed string */
	}
	else
	{
		printf("No certificates.\n");
	}
	X509_free(cert);    /* free the malloc'ed certificate copy */
}


int CSocketSSL::ssl_create_bind_socket()
{
	SSL_library_init();
	OpenSSL_add_all_algorithms();				/* Load cryptos, et.al. */
	SSL_load_error_strings();					/* Bring in and register error messages */

	const SSL_METHOD* method = SSLv2_client_method(); /* Create new client-method instance */
	ctx = SSL_CTX_new(method);					/* Create new context */
	if( ctx == NULL )
	{
		return -1;
	}

	ssl = SSL_new(ctx);		/* create new SSL connection state */ 
	SSL_set_fd(ssl, _socket_fd);	/* attach the socket descriptor */  
	if(SSL_connect(ssl) < 0 )		/* perform the connection */  
	{
		return -2;
	};
	return 0;
}

int CSocketSSL::ssl_receive(void * buf, size_t len)
{
	int bytes = SSL_read(ssl, buf, len);
	if (bytes < 0)
	{
		return errno;
	}
	return bytes;
}

int CSocketSSL::ssl_send(const void * buf, size_t len)
{
	int bytes = SSL_write(ssl, buf, len);
	if(bytes < 0)
	{
		return errno;
	}
	return bytes;
}

int CSocketSSL::ssl_acpect(CSocketSSL& client_socket)
{
	client_socket.ssl = SSL_new(ctx);		//基于ctx产生一个ssl
	SSL_set_fd(ssl, client_socket._socket_fd);
	if (SSL_accept(ssl) == -1) 
	{
		return errno;
	}
	return 0;
}


int CSocketSSL::ssl_close()
{
	if(ssl != NULL) SSL_shutdown(ssl);
	if(ctx != NULL) SSL_CTX_free(ctx);
	if(ssl != NULL) SSL_free(ssl);  /* release SSL state */
	close();        /* close connection */

	ssl = NULL;
	ctx = NULL;
	return 0;
}









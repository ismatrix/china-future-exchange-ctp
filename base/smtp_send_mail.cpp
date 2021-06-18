/*
 * smtp_send_mail.cpp
 *
 *  Created on: 2013-2-3
 *      Author: dreamyzhang
 */
#include <sstream>
#include "smtp_send_mail.h"
#include "csocket.h"
#include "RollLog.h"
#include "util_base64.h"
#include "util.h"

static int ssend(CSocket& _sock, const char* data, int data_len)
{
	LOG_INFO( "\ts: " << std::string(data, data_len));
	int sent = 0;
	while(sent < data_len)
	{
		int ret = send(_sock.get_socket_fd(), data+sent, data_len-sent, MSG_NOSIGNAL);
		if(ret<0)
			return -1;
		sent += ret;
	}
	return sent;
}
static int srecv(CSocket& _sock, char* buf, int buf_len)
{
	int ret = recv(_sock.get_socket_fd(), buf, buf_len, 0);
	if(ret>0)
		LOG_INFO( "\tr: " << std::string(buf, ret));
	return ret;
}
static int send_cmd(CSocket& _sock, const std::string& cmd, const std::string& content)
{
	std::string line = cmd;
	line.append(content);
	line.append("\r\n");
	return ssend(_sock, line.c_str(), line.size());
}

uint32_t smtp_send_mail(const smtp_send_mail_login_t& login, const smtp_send_mail_t& email)
{
	CSocket _sock;
	uint32_t ip = get_host_by_name(login._host.c_str());
	if(0==ip)
	{
		LOG_ERROR( "\tn: dns lookup failed, host=" << login._host);
		return smtp_send_mail_err::dns_failed;
	}
	
	int ret = _sock.create();
	if(ret < 0)
	{
		LOG_ERROR( "\tn: socket create failed."  << " err:" << _sock.get_errmsg());
		return smtp_send_mail_err::sock_failed;
	}
	
	uint32_t err = smtp_send_mail_err::ok;
	do
	{
		char buf[512];
		int len;
		//set sock opt
		struct timeval timeout = {3,0};
		setsockopt(_sock.get_socket_fd(), SOL_SOCKET, SO_SNDTIMEO, (void*)&timeout, sizeof(timeout));
		setsockopt(_sock.get_socket_fd(), SOL_SOCKET, SO_RCVTIMEO, (void*)&timeout, sizeof(timeout));
		//connect
		if(_sock.connect(ip, login._port) < 0)
		{
			err = smtp_send_mail_err::connect_failed;
			break;
		}
		
		//helo
		if(send_cmd(_sock, "HELO ", "SERVER") < 0)
		{
			err = smtp_send_mail_err::send_helo_err;
			break;
		}
		if(srecv(_sock, buf, sizeof(buf)) <= 0)
		{
			err = smtp_send_mail_err::recv_helo_err;
			break;
		}
		//auth
		if(send_cmd(_sock, "AUTH LOGIN ", "") < 0)
		{
			err = smtp_send_mail_err::send_auth_err;
			break;
		}
		if(srecv(_sock, buf, sizeof(buf)) <= 0)
		{
			err = smtp_send_mail_err::recv_auth_err;
			break;
		}
	
		
		//½ÓÊÕ username:
		if(srecv(_sock, buf, sizeof(buf)) <= 0)
		{
			err = smtp_send_mail_err::recv_auth_err;
			break;
		}
		
		//login name
		len = util_base64_encode((uint8_t*)buf, (const uint8_t*)login._auth_user.c_str(), login._auth_user.size());
		if(send_cmd(_sock, "", std::string(buf, len)) < 0)
		//if(send_cmd(_sock, "", login._auth_user) < 0)
		{
			err = smtp_send_mail_err::send_name_err;
			break;
		}
		if(srecv(_sock, buf, sizeof(buf)) <= 0)
		{
			err = smtp_send_mail_err::recv_name_err;
			break;
		}

		//login pswd
		len = util_base64_encode((uint8_t*)buf, (const uint8_t*)login._auth_pswd.c_str(), login._auth_pswd.size());
		if(send_cmd(_sock, "", std::string(buf, len)) < 0)
		//if(send_cmd(_sock, "", login._auth_pswd) < 0)
		{
			err = smtp_send_mail_err::send_pswd_err;
			break;
		}
		if(srecv(_sock, buf, sizeof(buf)) <= 0)
		{
			err = smtp_send_mail_err::recv_pswd_err;
			break;
		}
		//from
		std::string eaddr = email._from;
		if(eaddr.size() && eaddr[0]!='<')
			eaddr = "<" + eaddr + ">";
		if(send_cmd(_sock, "MAIL FROM: ", eaddr) < 0)
		{
			err = smtp_send_mail_err::send_from_err;
			break;
		}
		if(srecv(_sock, buf, sizeof(buf)) <= 0)
		{
			err = smtp_send_mail_err::recv_from_err;
			break;
		}
		//to
		for(smtp_send_mail_t::rcpt_lst_t::const_iterator it = email._rcpt_lst.begin(); it!=email._rcpt_lst.end(); ++it)
		{
			eaddr = *it;
			if(eaddr.size() && eaddr[0]!='<')
				eaddr = "<" + eaddr + ">";
			if(send_cmd(_sock, "RCPT TO: ", eaddr) < 0)
			{
				err = smtp_send_mail_err::send_rcpt_err;
				break;
			}
			msleep(500);
			if(srecv(_sock, buf, sizeof(buf)) <= 0)
			{
				err = smtp_send_mail_err::recv_rcpt_err;
				break;
			}
		}
		//data
		if(send_cmd(_sock, "DATA", "") < 0)
		{
			err = smtp_send_mail_err::send_data_err;
			break;
		}
		if(srecv(_sock, buf, sizeof(buf)) <= 0)
		{
			err = smtp_send_mail_err::recv_data_err;
			break;
		}
		//data body
		if(send_cmd(_sock, "From: ", email._from) < 0)
		{
			err = smtp_send_mail_err::send_body_err;
			break;
		}
		/*if(send_cmd(_sock, "To: ", email.) < 0)
		{
			err = smtp_send_mail_err::recv_body_err;
			break;
		}*/
		if(send_cmd(_sock, "Subject: ", email._subject+"\r\n") < 0)
		{
			err = smtp_send_mail_err::send_body_err;
			break;
		}
		if(send_cmd(_sock, email._content, "\r\n.") < 0)
		{
			err = smtp_send_mail_err::send_body_err;
			break;
		}
		msleep(500);
		if(srecv(_sock, buf, sizeof(buf)) <= 0)
		{
			err = smtp_send_mail_err::recv_body_err;
			break;
		}
		LOG_INFO( "\tn: email send over\n");
	}while(false);
	_sock.close();
	LOG_INFO( "\tn: socket closed, err=" << err << "\n");
	return err;
}

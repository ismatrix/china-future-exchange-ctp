/*
 * smtp_send_mail.h
 *
 *  Created on: 2013-2-3
 *      Author: dreamyzhang
 */

#ifndef __SMTP_SEND_MAIL__
#define __SMTP_SEND_MAIL__

#include <string>
#include <vector>
#include <stdint.h>


//a structure maintaining the infomation which is used for login into a smtp server
struct smtp_send_mail_login_t
{
	std::string _host;			//smtp server host name, eg: smtp.163.com
	unsigned short _port;		//smtp server port, it is 25 in most case
	std::string _auth_user;		//login user name. for fatmck@163.com, the auth_user should be 'fatmck'
	std::string _auth_pswd;		//password
};
//a structure maintaining the data of an email
struct smtp_send_mail_t
{
	//from whom this email is sent, eg: fatmck@163.com
	std::string _from;
	//adress list which email will be send to.
	typedef std::vector<std::string> rcpt_lst_t;
	rcpt_lst_t _rcpt_lst;
	//the subject of this email
	std::string _subject;
	//the text content of this email.
	std::string _content;
};

//error code
struct smtp_send_mail_err
{
	static const uint32_t ok = 0;
	static const uint32_t dns_failed = 1;
	static const uint32_t sock_failed = 2;
	static const uint32_t connect_failed = 3;
	static const uint32_t send_helo_err = 4;
	static const uint32_t recv_helo_err = 5;
	static const uint32_t send_auth_err = 6;
	static const uint32_t recv_auth_err = 7;
	static const uint32_t send_name_err = 8;
	static const uint32_t recv_name_err = 9;
	static const uint32_t send_pswd_err = 10;
	static const uint32_t recv_pswd_err = 11;
	static const uint32_t send_from_err = 12;
	static const uint32_t recv_from_err = 13;
	static const uint32_t send_rcpt_err = 14;
	static const uint32_t recv_rcpt_err = 15;
	static const uint32_t send_data_err = 16;
	static const uint32_t recv_data_err = 17;
	static const uint32_t send_body_err = 18;
	static const uint32_t recv_body_err = 19;
};

/**
 * a sync-mode smtp client which support auth-login.
 * it can only send simple text content.
 * return 0 or smtp_send_mail_err::ok on success.
 */
uint32_t smtp_send_mail(const smtp_send_mail_login_t& login, const smtp_send_mail_t& email);

#endif /* __SMTP_SEND_MAIL__ */

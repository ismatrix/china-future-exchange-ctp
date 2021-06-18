/*************************************************************************
  > File Name: csocket.cpp
  > Created Time: Wed 24 Aug 2016 09:03:01 AM CST
  > Author: 
  > description: 
 ************************************************************************/

#include "csocket.h"


int get_peer_addr(int _socket_fd, in_addr_t & peer_addr, uint16_t & peer_port)
{
	struct sockaddr_in _addr;
	if (::getpeername(_socket_fd, (struct sockaddr*)&_addr, NULL) < 0)
		return errno;

	peer_addr = _addr.sin_addr.s_addr;
	peer_port = ntohs(_addr.sin_port);
	return 0;
}

int  get_sock_addr(int _socket_fd, in_addr_t& socket_addr, uint16_t & socket_port)
{
	struct sockaddr_in _addr;
	if (::getsockname(_socket_fd, (struct sockaddr*)&_addr, NULL) < 0)
		return errno;
	socket_addr = _addr.sin_addr.s_addr;
	socket_port = ntohs(_addr.sin_port); 
	return 0;
}


string  get_addr_str(in_addr_t addr)
{
	char buf[64]={0}; 
	return inet_ntop(AF_INET, &addr, buf, sizeof(buf));
};

in_addr_t get_addr(char* ip)
{
	struct in_addr sinaddr; 
	inet_pton(AF_INET, ip, &sinaddr); 
	return sinaddr.s_addr;
};


int32_t get_local_ip_list(uint32_t* ip_list, uint32_t max_count)
{
	int sfd, intr;
	struct ifreq buf[16];
	struct ifconf ifc;
	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd < 0)
		return -1;
	uint32_t count = 0;
	do
	{
		ifc.ifc_len = sizeof(buf);
		ifc.ifc_buf = (caddr_t)buf;
		if(ioctl(sfd, SIOCGIFCONF, (char *)&ifc))
			break;
		intr = ifc.ifc_len / sizeof(struct ifreq);
		while (count<max_count && intr-- > 0 && 0==ioctl(sfd, SIOCGIFADDR, (char *)&buf[intr]))
		{
			uint32_t ip = ((struct sockaddr_in*)(&buf[intr].ifr_addr))->sin_addr.s_addr;
			if(ip==0x100007F)//skip 127.0.0.1
				continue;
			ip_list[count] = ip;
			++count;
		}
		close(sfd);
		return count;
	}while(0);
	close(sfd);
	return -1;
}

uint32_t get_host_by_name(const char* name)
{
	struct addrinfo *answer, hint;
	uint32_t ret_ip = 0;
	bzero(&hint, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	int ret = getaddrinfo(name, NULL, &hint, &answer);
	if (ret != 0)
		return 0;

	struct addrinfo* cur;
	for(cur=answer; cur!=NULL; cur=cur->ai_next )
	{
		ret_ip = ((struct sockaddr_in *)(cur->ai_addr))->sin_addr.s_addr;
		break;	//返回第一个ip
	}
	
	freeaddrinfo(answer);
	return ret_ip;
}


int get_local_mac(const char *eth, string& mac)  
{

	struct ifreq ifr;  
	int sd;  

	bzero(&ifr, sizeof(struct ifreq));  
	if( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
	{
		printf("get %s mac address socket creat error\n", eth);  
		return -1;  
	}  

	strncpy(ifr.ifr_name, eth, sizeof(ifr.ifr_name) - 1);  

	if(ioctl(sd, SIOCGIFHWADDR, &ifr) < 0)  
	{
		printf("get %s mac address error\n", eth);  
		close(sd);  
		return -1;  
	}  

	char tmp[64]={0};
	snprintf(tmp, sizeof(tmp), "%02x:%02x:%02x:%02x:%02x:%02x",  
			(unsigned char)ifr.ifr_hwaddr.sa_data[0],   
			(unsigned char)ifr.ifr_hwaddr.sa_data[1],  
			(unsigned char)ifr.ifr_hwaddr.sa_data[2],   
			(unsigned char)ifr.ifr_hwaddr.sa_data[3],  
			(unsigned char)ifr.ifr_hwaddr.sa_data[4],  
			(unsigned char)ifr.ifr_hwaddr.sa_data[5]);  

	close(sd);  

	mac = tmp;

	return 0;  
}  


int get_local_ip(const char *eth, string& ip)  
{
	int sd;  
	struct sockaddr_in sin;  
	struct ifreq ifr;  

	sd = socket(AF_INET, SOCK_DGRAM, 0);  
	if (-1 == sd)  
	{
		printf("socket error: %s\n", strerror(errno));  
		return -1;        
	}  

	strncpy(ifr.ifr_name, eth, IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ - 1] = 0;  

	// if error: No such device  
	if (ioctl(sd, SIOCGIFADDR, &ifr) < 0)  
	{
		printf("ioctl error: %s\n", strerror(errno));  
		close(sd);  
		return -1;  
	}  

	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));  
	
	ip = inet_ntoa(sin.sin_addr);

	close(sd);  
	return 0;  
}

int  CSocket::create(int protocol_family /* = PF_INET */, int socket_type /* = SOCK_STREAM */)
{
	//	create and check
	_socket_fd = ::socket(protocol_family, socket_type, 0);
	if (_socket_fd < 0)
	{
		return -1;
	}
	return 0;	
}

int  CSocket::bind(uint16_t port)
{
	return bind(htonl(INADDR_ANY), port);
}


int  CSocket::bind(in_addr_t addr, uint16_t port)
{
	struct sockaddr_in _addr;
	_addr.sin_family = AF_INET;
	_addr.sin_port	 = htons(port);
	_addr.sin_addr.s_addr = addr;

	return ::bind(_socket_fd, (struct sockaddr *)&_addr, sizeof(_addr));
}

int  CSocket::bind(string ip, uint16_t port)
{
	struct in_addr sinaddr;
	inet_pton(AF_INET, ip.c_str(), &sinaddr);
	return bind(sinaddr.s_addr, port);
}

int  CSocket::listen(int backlog /*=32*/)
{
	return ::listen(_socket_fd, backlog);
}

int  CSocket::accept(CSocket & client_socket)
{
	client_socket.close();
	int client_fd = -1;
	client_fd =::accept(_socket_fd, NULL, NULL);
	if (client_fd < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return 1;	//重试
		else if (errno == EINTR)
			return 2;	//重试
		else
			return -1;
	}
	client_socket._socket_fd = client_fd;
	return 0;
}

int  CSocket::connect(in_addr_t addr, uint16_t port)
{
	struct sockaddr_in _addr;
	_addr.sin_family = AF_INET;
	_addr.sin_port	 = htons(port);;
	_addr.sin_addr.s_addr = addr;

	return ::connect(_socket_fd, (struct sockaddr*)&_addr, sizeof(_addr));
}

int CSocket::connect(string ip, uint16_t port)
{
	struct in_addr sinaddr;                   
	inet_pton(AF_INET, ip.c_str(), &sinaddr); 
	return connect(sinaddr.s_addr, port) ;
}

int CSocket::shutdown(int how /*=SHUT_WR*/)
{
	//强制关闭连接 SHUT_WR关闭写 SHUT_RD关闭度 SHUT_RDWR关闭读写
	return ::shutdown(_socket_fd, how);
}

int CSocket::close()
{
	return ::close(_socket_fd);
}

int CSocket::receive(void *buf, size_t len, int flag /*=0*/)
{
	int bytes =::recv(_socket_fd, buf, len, flag);
	if (bytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return errno;	//重试
		else if (errno == EINTR)
			return errno;	//中断 重试
		else
			return errno;	//错误
	}
	return bytes;
}

int CSocket::send(const void *buf, size_t len, int flag /*=0*/)
{
	int bytes = ::send(_socket_fd, buf, len, flag);
	if (bytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return errno;	//重试
		else if (errno == EINTR)
			return errno;	//中断
		else
			return errno;	//错误
	}
	return bytes;
}

int CSocket::receive_from(void * buf, size_t len, in_addr_t& from_addr, uint16_t& from_port, int flags/*=0*/)
{
	struct sockaddr_in _addr;
	int bytes =::recvfrom(_socket_fd, buf, len, flags, (struct sockaddr*)&_addr, NULL);
	if (bytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return errno;
		else if (errno == EINTR)
			return errno;
		else
			return errno;
	}
	from_addr = _addr.sin_addr.s_addr;;
	from_port =  ntohs(_addr.sin_port);
	return bytes;
}

int CSocket::send_to(const void *msg, size_t len, in_addr_t to_addr, uint16_t to_port, int flags /*=0*/)
{
	struct sockaddr_in _addr;
	_addr.sin_family = AF_INET;
	_addr.sin_port	 = htons(to_port);;
	_addr.sin_addr.s_addr = to_addr;
	int bytes =::sendto(_socket_fd, msg, len, flags, (struct sockaddr*)&_addr, sizeof(_addr));
	if (bytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return errno;
		else if (errno == EINTR)
			return errno;
		else
			return errno;
	}
	return bytes;
}

//SO_REUSEADDR是让端口释放后立即就可以被再次使用
int  CSocket::set_reuseaddr()
{
	int optval = 1;
	size_t optlen = sizeof(optval);
	if (::setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) < 0)
		return errno;
	return 0;
}

int CSocket::set_nonblock()
{
	int val = fcntl(_socket_fd, F_GETFL, 0);
	if (val == -1)
		return errno;
	if (val & O_NONBLOCK)
		return 0;
	if (fcntl(_socket_fd, F_SETFL, val | O_NONBLOCK | O_NDELAY) == -1)
		return errno;
	return 0;
}









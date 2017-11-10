#ifndef _SOCKETADDR
#define _SOCKETADDR

#pragma once

#include "Base.h"
#include <mstcpip.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class SocketAddr
{
public:
	SocketAddr()
		:_IsInit(false) { memset(&_sockAddr, 0x00, sizeof(_sockAddr)); };

	SocketAddr(unsigned long addr, unsigned short port)
		:_IsInit(false)
	{
		_sockAddr.sin_family = AF_INET;
		_sockAddr.sin_port = port;
		_sockAddr.sin_addr.s_addr = addr;
	}

	SocketAddr(const char * addr, unsigned short port)
		:_IsInit(false)
	{
		_sockAddr.sin_family = AF_INET;
		_sockAddr.sin_port = port;

		struct hostent *remoteHost;
		remoteHost = gethostbyname(addr);
		if (NULL == remoteHost)
		{
			_sockAddr.sin_addr.s_addr = inet_addr(addr);
		}
		else
		{
			_sockAddr.sin_addr.s_addr = *(u_long*)remoteHost->h_addr_list[0];
		}
	}

	SocketAddr(const SocketAddr & sockAddr)
		:_sockAddr(sockAddr._sockAddr), _IsInit(false) {}


	SocketAddr(const sockaddr & sockAddr)
		:_sockAddr(*(sockaddr_in*)&sockAddr), _IsInit(false) {}

	SocketAddr(const sockaddr_in & sockAddr)
		:_sockAddr(sockAddr), _IsInit(false) {}

	~SocketAddr() {};
	
	void SetSocketAddr(const char * addr, unsigned short port)
	{
		_sockAddr.sin_family = AF_INET;
		_sockAddr.sin_port = port;
		_sockAddr.sin_addr.s_addr = inet_addr(addr);
	}

	void Clear()
	{
		memset(&_sockAddr, '\0', sizeof(sockaddr_in));
		_IsInit = false;
		_strAddr.clear();
	}

	unsigned short GetPort() const
	{
		return ntohs(_sockAddr.sin_port);
	}

	void SetPort(const unsigned short port)
	{
		_sockAddr.sin_port = htons(port);
	}

	unsigned int GetAddr() const
	{
		return _sockAddr.sin_addr.s_addr;
	}

	const char * GetDottedAddr()
	{
		if (!_IsInit)
		{
			_strAddr = InetNtoA();
			_IsInit = true;
		}

		return _strAddr.c_str();
	}

	const char * InetNtoA()
	{
		return inet_ntoa(_sockAddr.sin_addr);
	}

	void SetAddr(const unsigned int addr)
	{
		_sockAddr.sin_addr.s_addr = htonl(addr);
	}

	SocketAddr & operator=(const SocketAddr & rhs)
	{
		if (this != &rhs)
		{
			_sockAddr = rhs._sockAddr;
			_IsInit = rhs._IsInit;
			_strAddr = rhs._strAddr;
		}

		return *this;
	}

	SocketAddr & operator=(sockaddr & sockAddr)
	{
		_sockAddr = *(sockaddr_in*)&sockAddr;
		_IsInit = false;
		_strAddr.clear();

		return *this;
	}

	SocketAddr & operator=(sockaddr_in & sockAddr)
	{
		_sockAddr = sockAddr;
		_IsInit = false;
		_strAddr.clear();

		return *this;
	}

	operator sockaddr() { return *((sockaddr *)&_sockAddr); }
	operator sockaddr_in() { return *((sockaddr_in *)&_sockAddr); }
	operator sockaddr*() { return (sockaddr *)&_sockAddr; }
	operator sockaddr_in*() { return (sockaddr_in *)&_sockAddr; }

private:
	sockaddr_in _sockAddr;
	std::string	_strAddr;
	bool _IsInit;
};

#endif

#ifndef _SOCKET
#define _SOCKET

#pragma once

#include "Base.h"
#include "GameString.h"
#include "SocketAddr.h"

enum SOCKET_ERROR_LIST
{
	SOCK_OK,
	SOCK_ERROR = -1
};

class Socket
{
public:
	Socket() {};
	~Socket() { Close(); };
	
	static int StartUp();
	static int CleanUp();

	int Create();
	int Close();
	int Bind(SocketAddr& rSockAddr);
	int Listen(int nBackLog = SOMAXCONN);
	int Shutdown(int how);
	int GetPeerName(GameString & rAddress, WORD & rPort);
	int GetLocalName(GameString & rAddress, WORD & rPort);
	int GetPeerAddr(SocketAddr & rAddr);
	int GetLocalAddr(SocketAddr & rAddr);
	int SetNonBlocking(BOOL bActive);
	int SetReuseAddr(BOOL bActive);
	int SetLinger(BOOL bActive, WORD wTime);
	int SetTCPNoDelay(BOOL bActive);
	int SetKeepAlive(BOOL bActive);
	int SetKeepAlive(DWORD dwKeepAliveTime, DWORD dwKeepAliveInterval);
	int SetConditionalAccept(BOOL bActive);
	int GetCurReadSocketBuffer();
	int Connect(struct sockaddr_in * sockaddr);
	int SendStream(unsigned char *pSendBuffer, int nSendSize, bool bSendOut);
	int RecvStream(BYTE * pRecvBuffer, int nRecvSize);
	int AcceptEx(Socket &rAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped);
	int ConnectEx(const struct sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped);
	int DisconnectEx(LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved);
	void GetAcceptExSockaddrs(PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPSOCKADDR* LocalSockaddr, LPINT LocalSockaddrLength, LPSOCKADDR* RemoteSockaddr, LPINT RemoteSockaddrLength);
	int RecvEx(LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped);
	int SendEx(LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped);

	void Attach(SOCKET socket) { _socket = socket; }
	void Detach() { _socket = INVALID_SOCKET; }
	bool IsCreated() { return INVALID_SOCKET != _socket; }
	SOCKET GetRawSocket() { return _socket; }

	operator SOCKET() { return *((SOCKET *)&_socket); }
	Socket & operator=(const Socket & rhs);

protected:
	static int LoadExAPI();
	static int LoadExMethod(GUID functionID, LPVOID *pFunc);

	SOCKET _socket;
	static LPFN_ACCEPTEX _lpfnAcceptEx;
	static LPFN_CONNECTEX _lpfnConnectEx;
	static LPFN_DISCONNECTEX _lpfnDisconnectEx;
	static LPFN_GETACCEPTEXSOCKADDRS _lpfnGetAcceptExSockAddrs;
	static LPFN_TRANSMITFILE _lpfnTransmitFile;
};

inline int Socket::AcceptEx(Socket &rAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped)
{
	if (!_lpfnAcceptEx(_socket, rAcceptSocket.GetRawSocket(), lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, lpdwBytesReceived, lpOverlapped))
	{
		int rc = WSAGetLastError();
		if (ERROR_IO_PENDING != rc)
		{
			return rc;
		}
	}

	return SOCK_OK;
}

inline int Socket::ConnectEx(const struct sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped)
{
	if (!_lpfnConnectEx(_socket, name, namelen, lpSendBuffer, dwSendDataLength, lpdwBytesSent, lpOverlapped))
	{
		int rc = WSAGetLastError();
		if (ERROR_IO_PENDING != rc)
		{
			return rc;
		}
	}

	return SOCK_OK;
}

inline int Socket::DisconnectEx(LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved)
{
	if (!_lpfnDisconnectEx(_socket, lpOverlapped, dwFlags, reserved))
	{
		int rc = WSAGetLastError();
		if (ERROR_IO_PENDING != rc)
		{
			return rc;
		}
	}

	return SOCK_OK;
}

inline void Socket::GetAcceptExSockaddrs(PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPSOCKADDR* LocalSockaddr, LPINT LocalSockaddrLength, LPSOCKADDR* RemoteSockaddr, LPINT RemoteSockaddrLength)
{
	_lpfnGetAcceptExSockAddrs(lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, LocalSockaddr, LocalSockaddrLength, RemoteSockaddr, RemoteSockaddrLength);
}

inline int Socket::RecvEx(LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped)
{
	if (0 != ::WSARecv(_socket, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, NULL))
	{
		int rc = WSAGetLastError();
		if (ERROR_IO_PENDING != rc)
		{
			return rc;
		}
	}

	return SOCK_OK;
}

inline int Socket::SendEx(LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped)
{
	if (0 != ::WSASend(_socket, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, NULL))
	{
		int rc = WSAGetLastError();
		if (ERROR_IO_PENDING != rc)
		{
			return rc;
		}
	}

	return SOCK_OK;
}

inline Socket & Socket::operator=(const Socket & rhs)
{
	_socket = rhs._socket;
	return *this;
}

#endif
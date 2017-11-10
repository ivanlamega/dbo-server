#include "stdafx.h"
#include "Socket.h"

// STATIC
LPFN_ACCEPTEX Socket::_lpfnAcceptEx = NULL;
LPFN_CONNECTEX Socket::_lpfnConnectEx = NULL;
LPFN_DISCONNECTEX Socket::_lpfnDisconnectEx = NULL;
LPFN_GETACCEPTEXSOCKADDRS Socket::_lpfnGetAcceptExSockAddrs = NULL;
LPFN_TRANSMITFILE Socket::_lpfnTransmitFile = NULL;

int Socket::StartUp()
{
	WSADATA wsaData;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
		return WSAGetLastError();

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return WSAGetLastError();
	}

	int nResult = LoadExAPI();
	if (SOCK_OK != nResult)
	{
		WSACleanup();
		return nResult;
	}

	return SOCK_OK;
}

int Socket::CleanUp()
{
	if (0 != WSACleanup())
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::LoadExAPI()
{
	int nResult = 0;

	// AcceptEx Load
	GUID funcAcceptEx = WSAID_ACCEPTEX;
	nResult = LoadExMethod(funcAcceptEx, (LPVOID*)&_lpfnAcceptEx);
	if (SOCK_OK != nResult)
		return nResult;

	// ConnectEx Load
	GUID funcConnectEx = WSAID_CONNECTEX;
	nResult = LoadExMethod(funcConnectEx, (LPVOID*)&_lpfnConnectEx);
	if (SOCK_OK != nResult)
		return nResult;

	// DisconnectEx
	GUID funcDisconnectEx = WSAID_DISCONNECTEX;
	nResult = LoadExMethod(funcDisconnectEx, (LPVOID*)&_lpfnDisconnectEx);
	if (SOCK_OK != nResult)
		return nResult;

	// GetAcceptExAddr
	GUID funcGetAcceptExAddr = WSAID_GETACCEPTEXSOCKADDRS;
	nResult = LoadExMethod(funcGetAcceptExAddr, (LPVOID*)&_lpfnGetAcceptExSockAddrs);
	if (SOCK_OK != nResult)
		return nResult;

	// TransmitFile
	GUID funcTransmitFile = WSAID_TRANSMITFILE;
	nResult = LoadExMethod(funcTransmitFile, (LPVOID*)&_lpfnTransmitFile);
	if (SOCK_OK != nResult)
		return nResult;

	return SOCK_OK;
}

int Socket::LoadExMethod(GUID functionID, LPVOID *pFunc)
{
	Socket socket;
	if (0 != socket.Create())
		return WSAGetLastError();

	DWORD dwBytes = 0;
	int nResult = WSAIoctl(socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&functionID,
		sizeof(GUID),
		pFunc,
		sizeof(LPVOID),
		&dwBytes,
		0,
		0);

	if (0 != nResult)
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::Create()
{
	_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == _socket)
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::Close()
{
	if (INVALID_SOCKET == _socket)
	{
		return SOCK_OK;
	}

	if (SOCKET_ERROR == closesocket(_socket))
	{
		return WSAGetLastError();
	}

	_socket = INVALID_SOCKET;

	return SOCK_OK;
}

int Socket::Bind(SocketAddr& rSockAddr)
{
	if (0 != bind(_socket, (struct sockaddr *) rSockAddr, sizeof(struct sockaddr)))
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::Listen(int nBackLog)
{
	if (0 != listen(_socket, nBackLog))
	{
		return WSAGetLastError();
	}

	return SOCK_OK;
}

int Socket::Shutdown(int how)
{
	if (INVALID_SOCKET == _socket)
	{
		return SOCK_OK;
	}

	if (SOCKET_ERROR == shutdown(_socket, how))
	{
		return WSAGetLastError();
	}

	return SOCK_OK;
}

int Socket::GetPeerName(GameString & rAddress, WORD & rPort)
{
	struct sockaddr_in sockAddr;
	int nSockAddrLen = sizeof(sockAddr);

	if (0 != getpeername(_socket, (struct sockaddr*) &sockAddr, &nSockAddrLen))
	{
		return WSAGetLastError();
	}

	rAddress = inet_ntoa(sockAddr.sin_addr);
	rPort = ntohs(sockAddr.sin_port);

	return SOCK_OK;
}

int Socket::GetLocalName(GameString & rAddress, WORD & rPort)
{
	struct sockaddr_in sockAddr;
	int nSockAddrLen = sizeof(sockAddr);

	if (0 != getsockname(_socket, (struct sockaddr*) &sockAddr, &nSockAddrLen))
	{
		return WSAGetLastError();
	}

	rAddress = inet_ntoa(sockAddr.sin_addr);
	rPort = ntohs(sockAddr.sin_port);

	return SOCK_OK;
}

int Socket::GetPeerAddr(SocketAddr & rAddr)
{
	struct sockaddr_in sockAddr;
	int nSockAddrLen = sizeof(sockAddr);

	if (0 != getpeername(_socket, (struct sockaddr*) &sockAddr, &nSockAddrLen))
	{
		return WSAGetLastError();
	}

	rAddr.SetSocketAddr(inet_ntoa(sockAddr.sin_addr), ntohs(sockAddr.sin_port));

	return SOCK_OK;
}

int Socket::GetLocalAddr(SocketAddr & rAddr)
{
	struct sockaddr_in sockAddr;
	int nSockAddrLen = sizeof(sockAddr);

	if (0 != getsockname(_socket, (struct sockaddr*) &sockAddr, &nSockAddrLen))
	{
		return WSAGetLastError();
	}

	rAddr.SetSocketAddr(inet_ntoa(sockAddr.sin_addr), ntohs(sockAddr.sin_port));

	return SOCK_OK;
}

int Socket::SetNonBlocking(BOOL bActive)
{
	unsigned long i = bActive;
	int result = ioctlsocket(_socket, FIONBIO, &i);

	if (SOCKET_ERROR == result)
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::SetReuseAddr(BOOL bActive)
{
	int result = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bActive, sizeof(bActive));

	if (SOCKET_ERROR == result)
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::SetLinger(BOOL bActive, WORD wTime)
{
	struct linger so_linger;

	so_linger.l_onoff = (u_short)bActive;
	so_linger.l_linger = wTime;

	int result = setsockopt(_socket, SOL_SOCKET, SO_LINGER, (char *)&so_linger, sizeof(so_linger));

	if (SOCKET_ERROR == result)
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::SetTCPNoDelay(BOOL bActive)
{
	int result = setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&bActive, sizeof(bActive));

	if (SOCKET_ERROR == result)
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::SetKeepAlive(BOOL bActive)
{
	int result = setsockopt(_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&bActive, sizeof(bActive));

	if (SOCKET_ERROR == result)
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::SetKeepAlive(DWORD dwKeepAliveTime, DWORD dwKeepAliveInterval)
{
	tcp_keepalive keepAlive = { TRUE, dwKeepAliveTime, dwKeepAliveInterval };

	DWORD dwBytesReturned;
	int result = WSAIoctl(_socket, SIO_KEEPALIVE_VALS, &keepAlive, sizeof(keepAlive), 0, 0, &dwBytesReturned, NULL, NULL);

	if (SOCKET_ERROR == result)
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::SetConditionalAccept(BOOL bActive)
{
	int result = setsockopt(_socket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&bActive, sizeof(bActive));

	if (SOCKET_ERROR == result)
		return WSAGetLastError();

	return SOCK_OK;
}

int Socket::GetCurReadSocketBuffer()
{
	unsigned long nRead = 0;

	ioctlsocket(_socket, FIONREAD, &nRead);

	return nRead;
}

int Socket::Connect(struct sockaddr_in * sockaddr)
{
	int rc = connect(_socket, (struct sockaddr *)sockaddr, sizeof(struct sockaddr_in));
	if (SOCKET_ERROR == rc)
	{
		rc = WSAGetLastError();
		return rc;
	}

	return SOCK_OK;
}

int Socket::SendStream(unsigned char *pSendBuffer, int nSendSize, bool bSendOut)
{
	int nResult = 0;
	bool bProcess = true;
	BYTE * pBuffer = pSendBuffer;

	if (bSendOut)
	{
		while (bProcess)
		{
			nResult = send(_socket, (const char *)pBuffer, nSendSize, 0);

			if (SOCKET_ERROR == nResult)
			{
				bProcess = false;
				return SOCKET_ERROR;
			}
			else if (nResult < nSendSize)
			{
				pBuffer += nResult;
				nSendSize -= nResult;

				bProcess = true;
			}
			else
			{
				bProcess = false;
			}
		}
	}
	else
	{
		nResult = send(_socket, (const char *)pBuffer, nSendSize, 0);

		if (SOCKET_ERROR == nResult)
		{
			return SOCKET_ERROR;
		}
	}

	return nResult;
}

int Socket::RecvStream(BYTE * pRecvBuffer, int nRecvSize)
{
	return recv(_socket, (char *)pRecvBuffer, nRecvSize, 0);
}
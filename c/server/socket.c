#include "socket.h"

/*
socket_t Socket(int domain, int type, int protocol)
{
#ifdef __WINDOWS__
	WORD    wVersionRequested;
	WSADATA wsaData;
	int     err;
	wVersionRequested = MAKEWORD(2, 2);//create 16bit data  
									   //(1)Load WinSock  
	err = WSAStartup(wVersionRequested, &wsaData); //load win socket  
	if (err != 0)
		return (socket_t)-1;
#endif
	return socket(domain, type, protocol);
}

int Bind(socket_t sock, const struct sockaddr * addr, socklen_t addrlen)
{
	return bind(sock, addr, addrlen);
}

int Listen(socket_t sock, int backlog)
{
	return listen(sock, backlog);
}

int Accept(socket_t sock, struct sockaddr * addr, socklen_t * addrlen)
{
	return accept(sock, addr, addrlen);
}

int Connect(socket_t sock, const struct sockaddr * addr, socklen_t addrlen)
{
	return connect(sock, addr, addrlen);
}

int Close(socket_t sock)
{
#ifdef __WINDOWS__
	return closesocket(sock);
#else
	return close(sock);
#endif
}
*/

int socket_init()
{
#ifdef __WINDOWS__
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData);
#else
	return 0;
#endif
}

int socket_create(socket_t *sock, int domain, int type, int protocol)
{
	int ret = 0;
	*sock = socket(domain, type, protocol);
#ifdef __WINDOWS__
	if (*sock == INVALID_SOCKET)
		ret = -1;
#else
	if (*sock == -1)
		ret = -1;
#endif
	return ret;
}

int socket_bind(socket_t sock, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret = bind(sock, addr, addrlen);
#ifdef __WINDOWS__
	if (ret == SOCKET_ERROR)
		return -1;
#else
	if (ret == -1)
		return -1;
#endif
	return ret;
}

int socket_listen(socket_t sock, int backlog)
{
	int ret = listen(sock, backlog);
#ifdef __WINDOWS__
	if (ret == SOCKET_ERROR)
		return -1;
#else
	if (ret == -1)
		return -1;
#endif
	return ret;
}

int socket_accept(socket_t *sock, socket_t listen, struct sockaddr *addr, socklen_t *addrlen)
{
	int ret = 0;
	*sock = accept(listen, addr, addrlen);
#ifdef __WINDOWS__
	if (*sock == INVALID_SOCKET)
		ret = -1;
#else
	if (*sock == -1)
		ret = -1;
#endif
	return ret;
}

int socket_connect(socket_t sock, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret = connect(sock, addr, addrlen);
#ifdef __WINDOWS__
	if (ret == SOCKET_ERROR)
		return -1;
#else
	if (ret == -1)
		return -1;
#endif
	return ret;
}

int socket_close(socket_t sock)
{
	int ret = 0;
#ifdef __WINDOWS__
	if (closesocket(sock) == SOCKET_ERROR)
		ret = -1;
#else
	ret = close(sock);
#endif
	return ret;
}

int socket_destroy()
{
#ifdef __WINDOWS__
	return WSACleanup();
#else
	return 0;
#endif
}

#ifdef __WINDOWS__
ssize_t read(socket_t sock, void * buf, size_t count)
{
	return (ssize_t)recv(sock, buf, count, 0);
}

ssize_t write(socket_t sock, const void * buf, size_t count)
{
	return (ssize_t)send(sock, buf, count, 0);
}

int close(socket_t sock)
{
	return closesocket(sock);
}
#endif
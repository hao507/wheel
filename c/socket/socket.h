#ifndef __SOCKET_H__
#define __SOCKET_H__

#ifdef __cplusplus
extern "C" {
#endif

// os
#if (defined _WIN32) || (defined _WIN64)
	#define __WINDOWS__
	#define __OS__ "windows"
#else


	#define __LINUX__
	#define __OS__ "linux"
#endif

// include
#ifdef __WINDOWS__
	#include <winsock2.h>
	#pragma comment(lib, "ws2_32.lib")
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
#include <unistd.h>
#endif

// type define
#ifdef __WINDOWS__
	typedef SOCKET			socket_t;
	typedef SOCKADDR_IN		sockaddr_in;
	typedef int				socklen_t;
	typedef struct sockaddr	sockaddr;
	typedef long int		ssize_t;

#else
	typedef int				socket_t;
#endif

//int socket_init();
//socket_t socket(int domain, int type, int protocol);
//int bind(socket_t sock, const struct sockaddr *addr, socklen_t addrlen);
//int listen(socket_t sock, int backlog);
//int accept(socket_t sock, struct sockaddr *addr, socklen_t *addrlen);
//int connect(socket_t sock, const struct sockaddr *addr, socklen_t addrlen);
//int close(socket_t sock);
//ssize_t read(socket_t sock, void *buf, size_t count);
//ssize_t write(socket_t sock, const void *buf, size_t count);

int socket_init();
#ifdef __WINDOWS__
int close(socket_t sock);
ssize_t read(socket_t sock, void *buf, size_t count);
ssize_t write(socket_t sock, const void *buf, size_t count);
#endif

#ifdef __cplusplus
}
#endif

#endif // !__SOCKET_H__
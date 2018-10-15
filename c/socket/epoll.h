#ifndef __EPOLL_H__
#define __EPOLL_H__

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
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
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

#define LISTEN_NODE 1
#define CLIENT_NODE 2

#ifdef __WINDOWS__
#define EPOLL_CTL_ADD 0
#define EPOLL_CTL_MOD 1
#define EPOLL_CTL_DEL 2


	struct epoll_event
	{
		int events;
		struct data
		{
			void* ptr;
		}data;
	};
#endif


enum EPOLL_ERROR
{
	EPOLL_SUCCESS = 0,
	EPOLL_ARGUMENT_ERROR,
	EPOLL_CREATE_ERROR,
	EPOLL_CLOSE_ERROR,
	EPOLL_MALLOC_ERROR,
	EPOLL_REPEAT_EVENT,
	EPOLL_DELETED_EVENT,
	EPOLL_GET_ERROR,
	EPOLL_SET_ERROR,
	EPOLL_ADD_ERROR,
	EPOLL_MOD_ERROR,
	EPOLL_DEL_ERROR,
	EPOLL_ACCEPT_ERROR,
	EPOLL_OUTOFMAX_ERROR,
	EPOLL_NOTEVENT_ERROR,
	EPOLL_NONBLOCK_ERROR,
	EPOLL_WAIT_ERROR,

};


// include
typedef struct epoll_t epoll_t;
typedef struct epoll_node
{
	int		fd;
	int		events;
	int		status;
	void	*user;
	long	last_active;
	int		(*event_call)(struct epoll_t *epoll, struct epoll_node *node);
}epoll_node;

typedef struct epoll_t
{
	int					epfd;
	int					size;
	epoll_node			*lnodes;
	epoll_node			*cnodes;
	struct epoll_event	*events;
}epoll_t;


int read_event(epoll_t *epoll, epoll_node *node);
int write_event(epoll_t *epoll, epoll_node *node);
int close_event(epoll_t *epoll, epoll_node *node);
int accept_event(epoll_t *epoll, epoll_node *node);

epoll_t* epoll_init(int size);
epoll_node* epoll_get(epoll_t *epoll, int notype);
int epoll_set(epoll_t *epoll, int notype, epoll_node *node);
int epoll_add(epoll_t *epoll, epoll_node *node);
int epoll_mod(epoll_t *epoll, epoll_node *node);
int epoll_del(epoll_t *epoll, epoll_node *node);
int epoll_run(epoll_t *epoll);
int epoll_destroy(epoll_t *epoll);

#ifdef __cplusplus
}
#endif

#endif // !__EPOLL_H__

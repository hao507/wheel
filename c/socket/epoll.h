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

#ifdef __SUPPORT_EPOLL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define NOTYPE_LISTEN 0x01
#define NOTYPE_CLIENT 0x02

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

// typedef
typedef struct epoll_t epoll_t;
typedef struct epoll_node
{
	int		fd;				// file descriptor
	int		events;			// listen events
	int		status;			// node status:true is on the tree, false is not on the tree.
	void	*user;			// user generic pointer
	long	last_active;	// last active time.
							// have event, callback function.
	int(*event_call)(struct epoll_t *epoll, struct epoll_node *node);
}epoll_node;
typedef struct epoll_t
{
	int					epfd;		// epoll fd.
	int					size;		// listen node num.
	epoll_node			*lnodes;	// listen node array.
	epoll_node			*cnodes;	// client node array.
	struct epoll_event	*events;	// listen return event array.
}epoll_t;

// example event callback.
int read_event(epoll_t *epoll, epoll_node *node);
int write_event(epoll_t *epoll, epoll_node *node);
int close_event(epoll_t *epoll, epoll_node *node);
int accept_event(epoll_t *epoll, epoll_node *node);

// init epoll env, set listen num.
epoll_t* epoll_init(int size);
// get a available node by type. 
epoll_node* epoll_get(epoll_t *epoll, int notype);
// set a node in epoll rbtree, don't need call epoll_add.
int epoll_set(epoll_t *epoll, int notype, epoll_node node);
// add node in the epoll rbtree.
int epoll_add(epoll_t *epoll, epoll_node *node);
// change node event in the rbtree.
int epoll_mod(epoll_t *epoll, epoll_node *node);
// delete node from in rbtree.
int epoll_del(epoll_t *epoll, epoll_node *node);
// epoll timeout run.
int epoll_run(epoll_t *epoll, int timeout);
// epoll env destroy.
int epoll_destroy(epoll_t *epoll);

#endif // __SUPPORT_EPOLL__


#ifdef __cplusplus
}
#endif

#endif // !__EPOLL_H__

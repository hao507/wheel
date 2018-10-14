#ifndef __EPOLL_H__
#define __EPOLL_H__

#ifdef __cplusplus
extern "C" {
#endif

// os
#if (defined _WIN32_) || (defined _WIN64)
#define __WINDOWS__
#define __OS__ "windows"
#else


#define __LINUX__
#define __OS__ "linux"
#endif

enum EPOLL_ERROR
{
	EPOLL_SUCCESS = 0,
	EPOLL_ARGUMENT_ERROR,
	EPOLL_CREATE_ERROR,
	EPOLL_CLOSE_ERROR,
	EPOLL_MALLOC_ERROR,
	EPOLL_REPEAT_EVENT,
	EPOLL_ADD_ERROR,
	EPOLL_MOD_ERROR,
	EPOLL_DEL_ERROR,
	EPOLL_ACCEPT_ERROR,
	EPOLL_OUTOFMAX_ERROR,
	EPOLL_NOTEVENT_ERROR,
	EPOLL_NONBLOCK_ERROR,

};



// include

typedef struct epoll_node
{
	int fd;
	int events;
	int status;
	void *user;
	void *func_arg;
	int (*event_func)(epoll_t *epoll, epoll_node *node, void *arg);
	long last_active;
}epoll_node;

typedef struct epoll_t
{
	int epfd;
	int size;
	int errno;
	epoll_node *nodes;
	struct epoll_event *events;
}epoll_t;


int read_event(epoll_t *epoll, epoll_node *node, void *arg);
int write_event(epoll_t *epoll, epoll_node *node, void *arg);
int close_event(epoll_t *epoll, epoll_node *node, void *arg);
int accept_event(epoll_t *epoll, epoll_node *node, void *arg);

const char* epoll_error(epoll_t *epoll);
int enode_init(epoll_node *node, int fd, int events, void(*event_func)(void *arg), void *arg);
int event_add(epoll_t *epoll , epoll_node *node);
int event_del(epoll_t *epoll, epoll_node *node);

int epoll_init(epoll_t *epoll, int lfd, int size);
int epoll_destroy(epoll_t *epoll);

#ifdef __cplusplus
}
#endif

#endif // !__EPOLL_H__

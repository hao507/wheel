#include "epoll.h"

#define NULL ((void*)0x0)



int read_event(epoll_t *epoll, epoll_node *node, void *arg)
{
}

int write_event(epoll_t *epoll, epoll_node *node, void *arg)
{
}

int close_event(epoll_t *epoll, epoll_node *node, void *arg)
{
}

int accept_event(epoll_t *epoll, epoll_node *node, void *arg)
{
	if (epoll == NULL || node == NULL)
		return -1;

	int cfd, i, ret;

	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	SIGNAL:
	cfd = accept(node->fd, (struct sockaddr*)&client_addr, &client_len);
	if (cfd < 0)
	{
		if (cfd == EINTR)
			goto SIGNAL;
		else if (cfd == EAGAIN)
			epoll->errno = EPOLL_NOTEVENT_ERROR;
		else
			epoll->errno = EPOLL_ACCEPT_ERROR;

		return -1;
	}


	for (i = 0; i < epoll->size; i++)
		if (epoll->nodes[i].status != 1)
			break;

	if (i == epoll->size)
	{
		epoll->errno = EPOLL_OUTOFMAX_ERROR;
		return -1;
	}

	int ret = 0;
	if ((ret = fcntl(cfd, F_SETFL, O_NONBLOCK)) < 0)
	{
		epoll->errno = EPOLL_NONBLOCK_ERROR;
		return -1;
	}

	ret = enode_init(epoll, node, cfd, );
}

const char * epoll_error(epoll_t *epoll)
{
	switch (epoll->errno)
	{
	case EPOLL_SUCCESS:
		return "success.";
		break;
	case EPOLL_ARGUMENT_ERROR:
		return "incoming parameters is incomplete.";
		break;
	case EPOLL_CREATE_ERROR:
		return "epoll create error.";
		break;
	case EPOLL_CLOSE_ERROR:
		return "epoll close error.";
		break;
	case EPOLL_MALLOC_ERROR:
		return "epoll malloc error.";
		break;
	case EPOLL_REPEAT_EVENT:
		return "epoll repeat event error.";
		break;
	case EPOLL_ADD_ERROR:
		return "epoll add event error.";
		break;
	case EPOLL_MOD_ERROR:
		return "epoll mod event error.";
		break;
	case EPOLL_DEL_ERROR:
		return "epoll del event error.";
		break;
	default:
		return "unknown error.";
		break;
	}

	return "unknown error.";
}

int enode_init(epoll_t *epoll, epoll_node *node, int fd, int events, void(*event_func)(void *arg), void *arg)
{
	if (node == NULL || fd < 0 || event_func == NULL)
	{
		return -1;
	}

	node->fd = fd;
	node->events = events;
	node->status = 0;
	node->func_arg = arg;
	node->event_func = event_func;
	node->last_active = time(NULL);

	return EPOLL_SUCCESS;
}

int event_add(epoll_t *epoll, epoll_node *node)
{
	if (epoll == NULL || node == NULL)
	{
		return -1;
	}

	struct epoll_event events = { 0 };
	
	if (node->status != 0)
	{
		epoll->errno = EPOLL_REPEAT_EVENT;
		return -1;
	}

	events.data.ptr = node;
	events.events = node->events;
	
	if (epoll_ctl(epoll->epfd, EPOLL_CTL_ADD, node->fd, &events) != 0)
	{
		epoll->errno = EPOLL_ADD_ERROR;
		return -1;
	}

	return EPOLL_SUCCESS;
}

int event_del(epoll_t *epoll, epoll_node *node)
{
	if (epoll == NULL || node == NULL)
	{
		return -1;
	}

	if (epoll_ctl(epoll->epfd, EPOLL_CTL_DEL, node->fd, NULL) != 0)
	{
		epoll->errno = EPOLL_DEL_ERROR;
		return -1;
	}

	return EPOLL_SUCCESS;
}

int epoll_init(epoll_t *epoll, int lfd, int size)
{
	if (epoll == NULL || size <= 0)
	{
		return -1;
	}

	epoll->size = size;

	epoll->epfd = epoll_create(size);
	if (epoll->epfd < 0)
	{
		epoll->errno = EPOLL_CREATE_ERROR;
		return -1;
	}

	epoll->events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * (size + 1));
	if (epoll->events == NULL)
	{
		if (close(epoll->epfd) != 0)
			epoll->errno = EPOLL_CLOSE_ERROR;
		else
			epoll->errno = EPOLL_MALLOC_ERROR;

		return -1;
	}
		
	epoll->nodes = (struct epoll_node *)malloc(sizeof(struct epoll_node) * (size + 1));
	if (epoll->nodes == NULL)
	{
		if (epoll->events != NULL)
			free(epoll->events);

		if (close(epoll->epfd) != 0)
			epoll->errno = EPOLL_CLOSE_ERROR;
		else
			epoll->errno = EPOLL_MALLOC_ERROR;

		return -1;
	}

	struct epoll_node node;

	if (enode_init(epoll, &node, lfd, EPOLLIN, accept_event, NULL) != 0)
	{
		if (epoll->events != NULL)
			free(epoll->events);

		if (epoll->nodes != NULL)
			free(epoll->nodes);

		if (close(epoll->epfd) != 0)
			epoll->errno = EPOLL_CLOSE_ERROR;
		else
			epoll->errno = EPOLL_MALLOC_ERROR;

		return -1;
	}

	if (event_add(epoll, &node) != 0)
	{
		if (epoll->events != NULL)
			free(epoll->events);

		if (epoll->nodes != NULL)
			free(epoll->nodes);

		if (close(epoll->epfd) != 0)
			epoll->errno = EPOLL_CLOSE_ERROR;
		else
			epoll->errno = EPOLL_MALLOC_ERROR;

		return -1;
	}

	return EPOLL_SUCCESS;
}

int epoll_destroy(epoll_t *epoll)
{
	if (epoll == NULL)
		return -1;

	if (epoll->events != NULL)
		free(epoll->events);

	if (epoll->nodes != NULL)
		free(epoll->nodes);

	if (close(epoll->epfd) != 0)
	{
		epoll->errno = EPOLL_CLOSE_ERROR;
		return -1;
	}

	epoll->epfd = -1;
	epoll->events = NULL;
	epoll->size = 0;

	return EPOLL_SUCCESS;
}
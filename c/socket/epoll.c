#include "epoll.h"


#ifdef __SUPPORT_EPOLL__

// example event callback.
int read_event(epoll_t *epoll, epoll_node *node)
{
	if (epoll == NULL || node == NULL)
		return EPOLL_ARGUMENT_ERROR;

	int i, len;
	char buf[1024] = { 0 };
SIGNAL:
	len = recv(node->fd, buf, 1024, 0);
	if (len < 0)
	{
		if (errno == EINTR)
			goto SIGNAL;
		else if (errno == EAGAIN)
			return EPOLL_NOTEVENT_ERROR;

		return EPOLL_ACCEPT_ERROR;
	}
	if (len == 0)
	{
		epoll_del(epoll, node);
		close(node->fd);
	}

	for (i = 0; i < len; i++)
		if (buf[i] >= 'a' && buf[i] <= 'z')
			buf[i] -= 'a' - 'A';

	send(node->fd, buf, len, 0);

	printf("%s\n", buf);

	return EPOLL_SUCCESS;
}
int write_event(epoll_t *epoll, epoll_node *node)
{
	if (epoll == NULL || node == NULL)
		return EPOLL_ARGUMENT_ERROR;

	return EPOLL_SUCCESS;
}
int close_event(epoll_t *epoll, epoll_node *node)
{
	if (epoll == NULL || node == NULL)
		return EPOLL_ARGUMENT_ERROR;

	return EPOLL_SUCCESS;
}
int accept_event(epoll_t *epoll, epoll_node *node)
{
	if (epoll == NULL || node == NULL)
		return EPOLL_ARGUMENT_ERROR;

	int cfd;

	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	SIGNAL:
	cfd = accept(node->fd, (struct sockaddr*)&client_addr, &client_len);
	if (cfd < 0)
	{
		if (errno == EINTR)
			goto SIGNAL;
		else if (errno == EAGAIN)
			return EPOLL_NOTEVENT_ERROR;
		
		return EPOLL_ACCEPT_ERROR;
	}

	epoll_node *lnode = epoll_get(epoll, NOTYPE_CLIENT);
	if (lnode == NULL)
		return EPOLL_GET_ERROR;

	if (fcntl(cfd, F_SETFL, O_NONBLOCK) < 0)
		return EPOLL_NONBLOCK_ERROR;

	lnode->events = EPOLLIN;
	lnode->event_call = read_event;
	lnode->fd = cfd;
	lnode->last_active = time(NULL);
	lnode->status = 0;
	lnode->user = NULL;

	if (epoll_add(epoll, lnode) != 0)
		return EPOLL_ADD_ERROR;

	return EPOLL_SUCCESS;
}

// init epoll env, set listen num.
epoll_t* epoll_init(int size)
{
	if (size <= 0)
		return NULL;

	int epfd = epoll_create(size);
	if (epfd < 0)
		return NULL;

	epoll_t* epoll = (epoll_t*)malloc(sizeof(epoll_t));
	if (epoll == NULL)
	{
		close(epfd);
		return NULL;
	}
	
	epoll->events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * size);
	if (epoll->events == NULL)
	{
		close(epfd);
		free(epoll);
		return NULL;
	}

	epoll->lnodes = (struct epoll_node *)malloc(sizeof(struct epoll_node) * size);
	if (epoll->lnodes == NULL)
	{
		close(epfd);
		free(epoll);
		free(epoll->events);
		return NULL;
	}

	epoll->cnodes = (struct epoll_node *)malloc(sizeof(struct epoll_node) * size);
	if (epoll->cnodes == NULL)
	{
		close(epfd);
		free(epoll);
		free(epoll->events);
		free(epoll->lnodes);
		return NULL;
	}

	memset(epoll->events, 0, sizeof(struct epoll_event) * size);
	memset(epoll->lnodes, 0, sizeof(struct epoll_node) * size);
	memset(epoll->cnodes, 0, sizeof(struct epoll_node) * size);

	epoll->epfd = epfd;
	epoll->size = size;

	return epoll;
}
// get a available node by type. 
epoll_node * epoll_get(epoll_t * epoll, int notype)
{
	if (epoll == NULL)
		return NULL;

	epoll_node *array = NULL;
	switch (notype)
	{
	case NOTYPE_LISTEN:
		array = epoll->lnodes;
		break;
	case NOTYPE_CLIENT:
		array = epoll->cnodes;
		break;
	default:
		return NULL;
	}

	int i = 0;
	for (i = 0; i < epoll->size; i++)
		if (array[i].status != 1)
			break;

	if (i == epoll->size)
		return NULL;

	return &array[i];
}
// set a node in epoll rbtree, don't need call epoll_add.
int epoll_set(epoll_t *epoll, int notype,epoll_node node)
{
	if (epoll == NULL)
		return EPOLL_ARGUMENT_ERROR;

	epoll_node * n = epoll_get(epoll, notype);
	if (epoll == NULL)
		return EPOLL_GET_ERROR;

	*n = node;

	return EPOLL_SUCCESS;
}
// add node in the epoll rbtree.
int epoll_add(epoll_t *epoll, epoll_node *node)
{
	if (epoll == NULL || node == NULL)
		return EPOLL_ARGUMENT_ERROR;

	if (node->status != 0)
		return EPOLL_REPEAT_EVENT;

	struct epoll_event events = { 0 };
	events.events = node->events;
	events.data.ptr = node;

	if (epoll_ctl(epoll->epfd, EPOLL_CTL_ADD, node->fd, &events) != 0)
		return EPOLL_ADD_ERROR;

	node->status = 1;

	return EPOLL_SUCCESS;
}
// change node event in the rbtree.
int epoll_mod(epoll_t *epoll, epoll_node *node)
{
	if (epoll == NULL || node == NULL)
		return EPOLL_ARGUMENT_ERROR;

	if (node->status == 0)
		return EPOLL_DELETED_EVENT;

	struct epoll_event events = { 0 };
	events.events = node->events;
	events.data.ptr = node;

	if (epoll_ctl(epoll->epfd, EPOLL_CTL_MOD, node->fd, &events) != 0)
		return EPOLL_MOD_ERROR;

	return EPOLL_SUCCESS;
}
// delete node from in rbtree.
int epoll_del(epoll_t *epoll, epoll_node *node)
{
	if (epoll == NULL || node == NULL)
		return EPOLL_ARGUMENT_ERROR;

	if (node->status == 0)
		return EPOLL_DELETED_EVENT;

	if (epoll_ctl(epoll->epfd, EPOLL_CTL_DEL, node->fd, NULL) != 0)
		return EPOLL_DEL_ERROR;

	node->status = 0;

	return EPOLL_SUCCESS;
}
// epoll timeout run.
int epoll_run(epoll_t *epoll, int timeout)
{
	if (epoll == NULL)
		return EPOLL_ARGUMENT_ERROR;

	int i, num;

	while (1)
	{
		num = epoll_wait(epoll->epfd, epoll->events, epoll->size, timeout);
		if (num < 0)
		{
			// signal to interrupt or non-block retutn.
			if (errno == EINTR || errno == EAGAIN)
				continue;
			
			return EPOLL_WAIT_ERROR;
		}
		for (i = 0; i < num; i++)
		{
			epoll_node *node = epoll->events[i].data.ptr;
			if (node->event_call != NULL)
				node->event_call(epoll, node);
			else
				if (epoll_del(epoll, node) != 0);
				// TODO epoll node delete fail.

		}
	}

	return EPOLL_SUCCESS;
}
// epoll env destroy.
int epoll_destroy(epoll_t *epoll)
{
	if (epoll == NULL)
		return EPOLL_ARGUMENT_ERROR;

	close(epoll->epfd);

	if (epoll->events != NULL)
		free(epoll->events);

	if (epoll->lnodes != NULL)
		free(epoll->lnodes);

	if (epoll->cnodes != NULL)
		free(epoll->cnodes);

	epoll->epfd = -1;
	epoll->events = NULL;
	epoll->size = 0;

	free(epoll);

	return EPOLL_SUCCESS;
}

#endif // __SUPPORT_EPOLL__
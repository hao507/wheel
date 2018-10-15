#define _CRT_SECURE_NO_WARNINGS
#include "epoll.h"
#include "socket.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

void pexit(const char *err)
{
	printf("error: %s\n", err);
	exit(1);
}

int main()
{
	socket_init();

	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd < 0)
		pexit("socket");

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

	listen(lfd, 128);

	epoll_t *epoll = epoll_init(1024);
	if (epoll == NULL)
		pexit("epoll_init");

	epoll_node *n = epoll_get(epoll, LISTEN_NODE);
	if (n == NULL)
		pexit("epoll_get");

	n->events = EPOLLIN;
	n->event_call = accept_event;
	n->fd = lfd;
	n->last_active = time(NULL);
	n->status = 0;
	n->user = NULL;

	if (epoll_add(epoll, n) != 0)
		pexit("epoll_add");

	int ret = epoll_run(epoll);
	if (ret != 0)
		pexit("epoll_run");

	system("pause");
	return EXIT_SUCCESS;
}

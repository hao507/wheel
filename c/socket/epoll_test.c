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
	printf("error: %s\n");
	exit(1);
}

int main()
{
	socket_init();

	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd < 0)
		pexit("socket");

	epoll_t epoll;
	if (epoll_init(&epoll, lfd, 1024) != 0)
		pexit("epoll_init");

	int ret = epoll_run(&epoll);
	if (ret != 0)
		pexit("epoll_run");

	system("pause");
	return EXIT_SUCCESS;
}

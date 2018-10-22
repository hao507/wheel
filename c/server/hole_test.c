#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "hole.h"

int server(int argc, const char* argv[])
{
	int ret;
	hole_t *hole = hole_init(argv[1], 1024);
	if (hole == NULL) {
		printf("hole init error\n");
		getchar();
		return 1;
	}
	printf("server success running, listen : %s\n", argv[1]);

	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	char buf[1024];

	while (1) {
		if ((ret = hole_wait(hole, buf, sizeof(buf), &addr, &len)) != 0)
			printf("hole wait error:%d\n", ret);
		//printf("wait finish...\n");
	}

	return 0;
}

int client(int argc, const char* argv[])
{
	struct sockaddr_in self, other;
	memset(&self, 0, sizeof(self));
	memset(&other, 0, sizeof(other));
	int ret = hole_register(&self, &other, argv[1], argv[2]);
	if (ret != 0) {
		printf("register error:%d\n", ret);
		return 1;
	}

	printf("self:tcp://%s:%d/%s\n", inet_ntoa(self.sin_addr), self.sin_port, argv[2]);
	printf("other:tcp://%s:%d/%s\n", inet_ntoa(other.sin_addr), other.sin_port, argv[2]);

	return 0;
}

int main(int argc, const char* argv[])
{
	if (argc == 2) {
		server(argc, argv);
	} else if (argc == 3) {
		client(argc, argv);
	}

	getchar();

	return 0;
}

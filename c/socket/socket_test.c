#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "socket.h"

void pexit(const char* err)
{
	printf("error: %s\n", err);
	exit(1);
}

void server()
{
	int ret = socket_init();
	if (ret != 0)
		pexit("socket_init");

	socket_t sock;
	ret = socket_create(&sock, AF_INET, SOCK_STREAM, 0);
	if (ret < 0)
		pexit("socket");

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = socket_bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	if (ret != 0)
		pexit("bind");

	ret = socket_listen(sock, 128);
	if (ret != 0)
		pexit("listen");

	socket_t csock;
	ret = socket_accept(&csock, sock, NULL, NULL);
	if (ret < 0)
		pexit("socket");
	

	char buf[BUFSIZ] = { 0 };

	while (1)
	{
		memset(buf, 0, BUFSIZ);
		int len = read(csock, buf, BUFSIZ);
		if (len == 0 || len < 0)
		{
			printf("len = %d\n", len);
			close(csock);
			break;
		}
		for (int i = 0; i < len; i++)
		{
			if (buf[i] >= 'a' && buf[i] <= 'z')
			{
				buf[i] -= 'a' - 'A';
			}
		}
		printf("%s", buf);
		write(csock, buf, len);
	}

	socket_destroy();
}

void client()
{
	int ret = socket_init();
	if (ret != 0)
		pexit("socket_init");

	socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		pexit("socket");

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	if (ret < 0)
		pexit("connect");

	char buf[BUFSIZ];

	while (1)
	{
		memset(buf, 0, BUFSIZ);
		scanf("%s",buf);
		if (strncmp("exit", buf, 4) == 0)
		{
			close(sock);
			break;
		}
		write(sock, buf, strlen(buf));
		write(sock, "\n", strlen("\n"));
		memset(buf, 0, BUFSIZ);
		read(sock, buf, BUFSIZ);
		printf("%s\n", buf);
	}
}

int main()
{
	//server();
	client();
}
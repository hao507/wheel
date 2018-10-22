#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include "hole.h"
#include "socket.h"
#include "thread.h"

#define STACK_SIZE 1996
#define BUFFER_SIZE 1024

#define LISTEN_MAX "Server listens to upper limit, please try again later"

#define HOLE_CONV	0xABCDEF16


//typedef struct hole_request
//{
//	int		conv;
//	cmd		action;
//	int		length;
//	char	token[16];
//	char	topic[64];
//	char	data[64];
//}hole_request;
//
//typedef struct hole_respond
//{
//	int		conv;
//	cmd		action;
//	int		status;
//	char	other_ip[32];
//	int		other_port;
//	char	self_ip[32];
//	int		self_port;
//}hole_respond;

static int hole_socket(hole_t *hole)
{
	if (hole == NULL)
		return HOLE_ARGS_ERROR;

	if (socket_init() != 0)
		return HOLE_SOCKET_ENV_ERROR;

	if (socket_create(&hole->sock, AF_INET, hole->protocol, 0) != 0)
		return HOLE_SOCKET_CREATE_ERROR;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(hole->port);
	addr.sin_addr.s_addr = inet_addr(hole->ip);
	if (socket_bind(hole->sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		socket_close(hole->sock);
		return HOLE_SOCKET_BIND_ERROR;
	}
	
	if (hole->protocol == SOCK_STREAM)
	{
		if (socket_listen(hole->sock, 128) != 0)
		{
			socket_close(hole->sock);
			return HOLE_SOCKET_LISTEN_ERROR;
		}
	}

	return HOLE_SUCCESS;
}

static int hole_convmsg(hole_request *req, void *buf)
{
	if (req == NULL || buf == NULL)
		return HOLE_ARGS_ERROR;

	memset(req, 0, sizeof(hole_request));
	memcpy(req, buf, sizeof(hole_request));

	return HOLE_SUCCESS;
}

static int hole_getnode(hole_t *hole)
{
	if (hole == NULL)
		return -1;

	int i;
	for (i = 0; i < hole->size; i++) {
		if (hole->hosts[i].status == 0) {
			break;
		}
	}
	if (i == hole->size) {
		return -1;
	}

	memset(&hole->hosts[i], 0, sizeof(hole->hosts[i]));


	return i;
}

static int hole_gettopic(hole_t *hole, const char* topic, int len)
{
	if (hole == NULL || topic == NULL)
		return -1;

	int i;
	for (i = 0; i < hole->size; i++)
		if (hole->hosts[i].status != 0 && strlen(hole->hosts[i].topic) == len && strncmp(hole->hosts[i].topic, topic, len) == 0)
			break;

	if (i >= hole->size)
		return -1;

	return i;
}

static int hole_checktopic(char *topic)
{
	if (topic == NULL)
		return HOLE_ARGS_ERROR;

	char *begin = topic;
	while (*begin != '\0')
	{
		if (*begin < 32 || *begin > 126)
		{
			*begin = '\0';
			break;
		}
		begin++;
	}

	// begin - topic = length

	return HOLE_SUCCESS;
}

static int hole_working(hole_t *hole, hole_request *req, socket_t client, struct sockaddr_in *addr)
{
	if (hole == NULL || req == NULL || addr == NULL)
		return HOLE_ARGS_ERROR;

	if (req->action == REGISTER)
	{
		int pos = -1;
		hole_checktopic(req->topic);
		pos = hole_gettopic(hole, req->topic, strlen(req->topic));

		// register
		if (pos < 0)
		{
			pos = hole_getnode(hole);

			if (pos < 0)
			{
				return HOLE_MAXIMUM_NODE;
			}

			hole->hosts[pos].addr = *addr;
			sprintf(hole->hosts[pos].ip, "%s\0", inet_ntoa(addr->sin_addr));
			hole->hosts[pos].port = ntohs(addr->sin_port);
			hole->hosts[pos].sock = client;
			hole->hosts[pos].status = 1;
			hole->hosts[pos].action = req->action;
			sprintf(hole->hosts[pos].topic, "%s\0", req->topic);

			char pro[12] = { 0 };

			if (hole->protocol == SOCK_STREAM)
				strcpy(pro, "tcp");
			else
				strcpy(pro, "udp");

			printf("register:[%s://%s:%d/%s]\n", pro, hole->hosts[pos].ip, hole->hosts[pos].port, req->topic);
		}
		// get register info
		else
		{
			const char *currip = inet_ntoa(addr->sin_addr);
			int currport = ntohs(addr->sin_port);

			hole_respond prev = { 0 };
			hole_respond curr = { 0 };

			prev.action = CONNECT;
			prev.conv = HOLE_CONV;
			prev.status = 0;
			sprintf(prev.self_ip, "%s\0", hole->hosts[pos].ip);
			prev.self_port = hole->hosts[pos].port;
			sprintf(prev.other_ip, "%s\0", currip);
			prev.other_port = currport;

			curr.action = CONNECT;
			curr.conv = HOLE_CONV;
			curr.status = 0;
			sprintf(curr.self_ip, "%s\0", currip);
			curr.self_port = currport;
			sprintf(curr.other_ip, "%s\0", hole->hosts[pos].ip);
			curr.other_port = hole->hosts[pos].port;

			char pro[12] = { 0 };

			if (hole->protocol == SOCK_STREAM)
			{
				write(hole->hosts[pos].sock, &prev, sizeof(prev));
				write(client, &curr, sizeof(curr));
				socket_close(hole->hosts[pos].sock);
				socket_close(client);
				hole->hosts[pos].status = 0;
				strcpy(pro, "tcp");
			}
			else if (hole->protocol == SOCK_DGRAM)
			{
				int ret = 0;
				ret = sendto(hole->hosts[pos].sock, (const char*)&prev, sizeof(prev), 0, (struct sockaddr*)&hole->hosts[pos].addr, sizeof(hole->hosts[pos].addr));
				ret = sendto(client, (const char*)&curr, sizeof(curr), 0, (struct sockaddr*)addr, sizeof(struct sockaddr_in));
				hole->hosts[pos].status = 0;
				strcpy(pro, "udp");
			}
			else
			{
				return HOLE_PROTOCOL_UNKNOWN;
			}

			printf("connectr:[%s://%s:%d/%s] -> [tcp://%s:%d/%s]\n",
				pro,
				curr.self_ip,
				curr.self_port,
				req->topic,
				hole->hosts[pos].ip,
				hole->hosts[pos].port,
				req->topic
			);
		}

	}
	else
	{
		return HOLE_UNKNOW_ACTION;
	}

	return HOLE_SUCCESS;
}

static int hole_tcp(hole_t *hole, void *buf, size_t len, struct sockaddr_in *addr, socklen_t *addrlen)
{
	if (hole == NULL || buf == NULL || len <= 0 || addr == NULL || addrlen == NULL)
		return HOLE_ARGS_ERROR;

	socket_t client;
	int rett;
	if ((rett = socket_accept(&client, hole->sock, (struct sockaddr*)addr, addrlen)) != 0)
		return HOLE_SOCKET_ACCEPT_ERROR;

	hole_request req;
	int ret = read(client, buf, len);
	if (ret <= 0 || hole_convmsg(&req, buf) != 0 || req.conv != HOLE_CONV)
	{
		socket_close(client);
		return HOLE_CONV_ERROR;
	}

	if (hole_working(hole, &req, client, addr) != 0)
	{
		socket_close(client);
		return HOLE_WORKING_ERROR;
	}

	return HOLE_SUCCESS;
}

static int hole_udp(hole_t *hole, void *buf, size_t len, struct sockaddr_in *addr, socklen_t *addrlen)
{
	if (hole == NULL || buf == NULL || len <= 0 || addr == NULL || addrlen == NULL)
		return HOLE_ARGS_ERROR;

	hole_request req;
	int ret = recvfrom(hole->sock, buf, len, 0, (struct sockaddr*)addr, addrlen);
	if (ret <= 0 || hole_convmsg(&req, buf) != 0 || req.conv != HOLE_CONV)
		return HOLE_CONV_ERROR;


	if (hole_working(hole, &req, hole->sock, addr) != 0)
		return HOLE_WORKING_ERROR;

	return HOLE_SUCCESS;
}

hole_t* hole_init(const char* host, int size)
{
	if (host == NULL || size <= 0)
		return NULL;

	hole_t *hole = (hole_t*)malloc(sizeof(hole_t));
	if (hole == NULL)
		return NULL;

	memset(hole, 0, sizeof(hole_t));

	char pro[12] = { 0 };
	int protocol = 0;
	sscanf(host, "%[^:]://%[^:]:%d", pro, hole->ip, &hole->port);
	if (strncmp(pro, "tcp", strlen("tcp")) == 0)
		protocol = SOCK_STREAM;
	else if (strncmp(pro, "udp", strlen("udp")) == 0)
		protocol = SOCK_DGRAM;
	else
		return NULL;

	hole->size = size;
	hole->protocol = protocol;

	if (hole_socket(hole) != 0)
	{
		free(hole);
		return NULL;
	}

	hole->hosts = (hostnode*)malloc(sizeof(hostnode) * size);
	if (hole->hosts == NULL)
	{
		free(hole);
		return NULL;
	}

	memset(hole->hosts, 0, sizeof(hostnode) * size);

	return hole;
}

int hole_wait(hole_t *hole, void *buf, int len, struct sockaddr_in *addr, socklen_t *addrlen)
{
	if (hole == NULL || buf == NULL || len <= 0 || addr == NULL || addrlen == NULL)
		return HOLE_ARGS_ERROR;

	switch (hole->protocol)
	{
	case SOCK_STREAM:
		return hole_tcp(hole, buf, len, addr, addrlen);
	case SOCK_DGRAM:
		return hole_udp(hole, buf, len, addr, addrlen);
	default:
		return HOLE_PROTOCOL_UNKNOWN;
	}

	return HOLE_PROTOCOL_UNKNOWN;
}

int hole_destroy(hole_t *hole)
{
	if (hole == NULL)
		return HOLE_ARGS_ERROR;

	close(hole->sock);
	socket_destroy();

	if (hole->hosts != NULL)
		free(hole->hosts);

	free(hole);

	return 0;
}



static int hole_convaddr(struct sockaddr_in *addr, const char *host)
{
	if (addr == NULL || host == NULL)
		return HOLE_ARGS_ERROR;

	char protocol[12] = { 0 };
	char ip[12] = { 0 };
	int port = 0;
	sscanf(host, "%[^:]://%[^:]:%d", protocol, ip, &port);

	memset(addr, 0, sizeof(struct sockaddr_in));

	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = inet_addr(ip);

	return HOLE_SUCCESS;
}

static int hole_commit(socket_t sock, struct hole_request *req, struct hole_respond *res, struct sockaddr_in *addr, int protocol)
{
	if (req == NULL || res == NULL || addr == NULL)
		return HOLE_ARGS_ERROR;

	char buf[1024] = { 0 };

	switch (protocol)
	{
	case SOCK_STREAM:
		if (socket_connect(sock, (struct sockaddr*)addr, sizeof(struct sockaddr_in)) != 0)
			return HOLE_SOCKET_CONNECT_ERROR;
		if (write(sock, req, sizeof(struct hole_respond)) <= 0)
			return HOLE_SOCKET_SEND_ERROR;
		if (read(sock, buf, sizeof(buf)) <= 0)
			return HOLE_SOCKET_RECV_ERROR;
			break;
	case SOCK_DGRAM:
		if (sendto(sock, (const char*)req, sizeof(struct hole_respond), 0, (struct sockaddr*)addr, sizeof(struct sockaddr_in)) <= 0)
			return HOLE_SOCKET_SEND_ERROR;
		if (recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL) <= 0)
			return HOLE_SOCKET_RECV_ERROR;
		break;
	default:
		return HOLE_PROTOCOL_UNKNOWN;
	}

	memcpy(res, buf, sizeof(struct hole_respond));
	
	return HOLE_SUCCESS;
}

int hole_register(struct sockaddr_in *self, struct sockaddr_in *other, const char *host, const char *topic)
{
	if (self == NULL || other == NULL || host == NULL || topic == NULL)
		return HOLE_ARGS_ERROR;

	char protocol[12] = { 0 };
	sscanf(host, "%[^:]", protocol);
	int pro = -1;
	if (strncmp(protocol, "tcp", strlen("tcp")) == 0)
		pro = SOCK_STREAM;
	else if (strncmp(protocol, "udp", strlen("udp")) == 0)
		pro = SOCK_DGRAM;
	else
		return HOLE_PROTOCOL_UNKNOWN;

	socket_t sock;
	if (socket_init() != 0 || socket_create(&sock, AF_INET, pro, 0) != 0)
		return HOLE_SOCKET_CREATE_ERROR;

	struct sockaddr_in addr;
	if (hole_convaddr(&addr, host) != 0)
		return HOLE_CONVADDR_ERROR;

	hole_request req;
	memset(&req, 0, sizeof(req));
	req.conv = HOLE_CONV;
	req.action = REGISTER;
	strncpy(req.topic, topic, strlen(topic));
	hole_checktopic(req.topic);
	req.length = strlen(req.topic);

	hole_respond res;
	memset(&res, 0, sizeof(res));

	int ret = hole_commit(sock, &req, &res, &addr, pro);
	socket_close(sock);
	socket_destroy();
	if (ret != 0)
		return HOLE_SOCKET_COMMIT_ERROR;

	if (res.conv != HOLE_CONV || res.action != CONNECT)
		return HOLE_CONV_ERROR;

	memset(self, 0, sizeof(struct sockaddr_in));
	memset(other, 0, sizeof(struct sockaddr_in));

	self->sin_family = AF_INET;
	self->sin_port = res.self_port;
	self->sin_addr.s_addr = inet_addr(res.self_ip);

	other->sin_family = AF_INET;
	other->sin_port = res.other_port;
	other->sin_addr.s_addr = inet_addr(res.other_ip);
	
	return HOLE_SUCCESS;
}


/*


socket_t create_tcp(int port)
{
socket_t sock;
int ret = socket_init();
if (ret != 0)
return ret;
sock = socket(AF_INET, SOCK_STREAM, 0);
if (sock < 0)
return sock;

struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(port);
addr.sin_addr.s_addr = htonl(INADDR_ANY);

ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
if (__OS__ == "windows" && ret == SOCKET_ERROR) {
return -1;
} else if (ret < 0) {
return ret;
}

ret = listen(sock, 128);
if (ret < 0)
return ret;

return sock;
}


static void* worker(void *arg)
{
int ret = -1;
struct thread_arg *targ = arg;
int pos = targ->pos;
int len = targ->len;
struct hostinfo* hosts = targ->hosts;
struct hostinfo* host = &hosts[pos];
free(arg);

char buf[BUFFER_SIZE];

while (1) {
memset(buf, 0, BUFFER_SIZE);
ret = read(host->sock, buf, BUFFER_SIZE);
if (ret <= 0 || ret >= BUFFER_SIZE) {
break;
}

hole_request req;
memcpy(&req, buf, sizeof(req));
if (req.conv != HOLE_CONV) {
break;
}

//inet_ntop(AF_INET, (void*)&host->addr.sin_addr, host->ip, sizeof(host->ip));
strncpy(host->ip, inet_ntoa(host->addr.sin_addr), sizeof(host->ip));
host->port = ntohs(host->addr.sin_port);
memcpy(host->topic, req.topic, sizeof(req.topic));

if (req.action = reg) {
int i;
printf("connecter: tcp://%s:%d/%s\n", host->ip, host->port, host->topic);
for (i = 0; i < len; i++) {
if (i != pos && hosts[i].status != 0 && (strncmp(hosts[i].topic, host->topic, strlen(host->topic)) == 0)) {
break;
}
}
if (i >= len) {
//write(host->sock, "not found topic!", strlen("not found topic!"));
//printf("is first:%d\n", len);
return NULL;
break;
}
//printf("connecter: tcp://%s:%d/%s\n", host->ip, host->port, host->topic);

//char shost[128] = { 0 };	// to -> connecter
//char chost[128] = { 0 };	// to -> register
//sprintf(shost, "%s:%d", hosts[i].ip, hosts[i].port);
//sprintf(chost, "%s:%d,%d", host->ip, host->port, hosts[i].port);

//int		conv;
//cmd		action;
//int		status;
//char	other_ip[32];
//int		other_port;
//char	self_ip[32];
//int		self_port;

hole_respond self, other = { 0 };
self.conv = HOLE_CONV;
self.action = conn;
self.status = 0;
self.self_port = host->port;
memcpy(self.self_ip, host->ip, strlen(host->ip));
self.other_port = hosts[i].port;
memcpy(self.other_ip, hosts[i].ip, strlen(hosts[i].ip));

other.conv = HOLE_CONV;
other.action = conn;
other.status = 0;
other.self_port = hosts[i].port;
memcpy(other.self_ip, hosts[i].ip, strlen(hosts[i].ip));
other.other_port = host->port;
memcpy(other.other_ip, host->ip, strlen(host->ip));

write(hosts[i].sock, &other, sizeof(other));
write(host->sock, &self, sizeof(self));
thread_sleep(5000);
close(hosts[i].sock);
host->status = 0;
hosts[i].status = 0;
break;
} else {
//write(host->sock, "request error!", strlen("request error!"));
printf("-----------------thread----------------\n");
break;
}
}

close(host->sock);
host->status = 0;

return NULL;
}

static void* timer(void *arg)
{
// TODO time out check.
return NULL;
}
int hole_run(int port, int size)
{
	socket_t listen, client;
	struct hostinfo* hosts = (struct hostinfo*)malloc(sizeof(struct hostinfo) * size);
	if (hosts == NULL) {
		return -1;
	}
	memset(hosts, 0, size * sizeof(hosts[0]));

	listen = create_tcp(port);
	if (listen < 0) {
		printf("create tcp server error: %s\n", strerror(errno));
		return -1;
	}

	struct sockaddr_in caddr;

	printf("server running...\n");

	while (1) {
		socklen_t clen = sizeof(caddr);
		client = accept(listen, (struct sockaddr*)&caddr, &clen);
		if (client < 0) {
			printf("accept error, try again...\n");
			continue;
		}
		int i;
		for (i = 0; i < size; i++) {
			if (hosts[i].status == 0) {
				break;
			}
		}
		if (i == size) {
			close(client);
			printf("connect number is max!\n");
			continue;
		}

		hosts[i].addr = caddr;
		hosts[i].sock = client;
		hosts[i].status = 1;

		thread_t thread;
		struct thread_arg *arg = malloc(sizeof(struct thread_arg));
		if (arg == NULL) {
			close(client);
			printf("malloc arg error!\n");
			continue;
		}

		arg->hosts = hosts;
		arg->len = size;
		arg->pos = i;

		thread_create(&thread, worker, (void*)arg, STACK_SIZE);
		thread_detach(thread);

		printf("new client is connected.\n");
	}

	printf("programme finish...\n");

	return 0;
}

int hole_register(struct sockaddr_in *self, struct sockaddr_in *other, const char* host, const char *topic)
{
	if (other == NULL || host == NULL || topic == NULL)
		return -1;

	// server host address info
	char ip[64] = { 0 };
	char protocal[64] = { 0 };
	int port = 0;
	sscanf(host, "%[^:]://%[^:]:%d", protocal, ip, &port);
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(ip);

	// init socket
	if (socket_init() != 0) {
		return -2;
	}
	socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
	if (__OS__ == "windows" && sock == INVALID_SOCKET) {
		return -3;
	}
	else if (__OS__ == "linux" && sock <= 0) {
		return -4;
	}
	if (connect(sock, (struct sockaddr*)&address, sizeof(address)) != 0) {
		return -5;
	}

	char buf[1024] = { 0 };

	// request register msg
	hole_request requese;
	requese.conv = HOLE_CONV;
	requese.action = reg;
	strncpy(requese.topic, topic, strlen(topic));
	if (write(sock, &requese, sizeof(requese)) <= 0) {
		close(sock);
		return -6;
	}

	if (read(sock, buf, sizeof(buf)) <= 0) {
		close(sock);
		return -7;
	}
	hole_respond respond;
	memcpy(&respond, buf, sizeof(respond));
	if (respond.conv != HOLE_CONV) {
		close(sock);
		return -8;
	}
	// other address info
	other->sin_family = AF_INET;
	other->sin_port = respond.other_port;
	other->sin_addr.s_addr = inet_addr(respond.other_ip);
	// self address info
	if (self != NULL) {
		self->sin_family = AF_INET;
		self->sin_port = respond.self_port;
		self->sin_addr.s_addr = inet_addr(respond.self_ip);
	}

	return 0;
}

*/
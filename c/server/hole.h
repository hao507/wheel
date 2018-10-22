#ifndef __HOLE_H__
#define __HOLE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "socket.h"

#define HOLE_TIMEOUT	300000

enum
{
	HOLE_SUCCESS = 0x00000000,
	HOLE_CONV_ERROR,
	HOLE_ARGS_ERROR,
	HOLE_MAXIMUM_NODE,
	HOLE_MALLOC_ERROR,
	HOLE_UNKNOW_ACTION,
	HOLE_WORKING_ERROR,
	HOLE_PORT_OCCUPIED,
	HOLE_CONVADDR_ERROR,
	HOLE_PROTOCOL_UNKNOWN,
	HOLE_SOCKET_ENV_ERROR,
	HOLE_SOCKET_CREATE_ERROR,
	HOLE_SOCKET_BIND_ERROR,
	HOLE_SOCKET_LISTEN_ERROR,
	HOLE_SOCKET_ACCEPT_ERROR,
	HOLE_SOCKET_COMMIT_ERROR,
	HOLE_SOCKET_CONNECT_ERROR,
	HOLE_SOCKET_RECV_ERROR,
	HOLE_SOCKET_SEND_ERROR,

};

enum
{
	REGISTER = 100, CONNECT, HEARTBEAT, CLOSE
};


typedef struct hostnode {
	socket_t	sock;
	int			action;
	int			status;
	int			port;
	char		ip[12];
	char		topic[128];
	struct sockaddr_in addr;
}hostnode;

typedef struct hole_t
{
	char        ip[12];
	int			port;
	int			size;
	int			protocol;
	socket_t	sock;
	hostnode	*hosts;
}hole_t;

typedef struct hole_request
{
	int		conv;
	int		action;
	int		length;
	char	token[16];
	char	topic[64];
	char	data[64];
}hole_request;

typedef struct hole_respond
{
	int		conv;
	int		action;
	int		status;
	char	other_ip[32];
	int		other_port;
	char	self_ip[32];
	int		self_port;
}hole_respond;

// server api
hole_t* hole_init(const char *host, int size);
int hole_wait(hole_t *hole, void *buf, int len, struct sockaddr_in *addr, socklen_t *addrlen);
int hole_destroy(hole_t *hole);

// client api
int hole_register(struct sockaddr_in *self, struct sockaddr_in *other, const char* host, const char *topic);


#ifdef __cplusplus
}
#endif

#endif // !__HOLE_H__

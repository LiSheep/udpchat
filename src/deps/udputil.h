#ifndef UDPUTIL_H
#define UDPUTIL_H

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/file.h>
#include <math.h>

#include "merror.h"

#define SERV_PORT 19560
#define BACKLOG 10
#define CONN_PORT 20000

#define TYPE_LOGIN 1
#define TYPE_LOGOUT 2
#define TYPE_SEND 3

#define MSG_BUFF 10240
#define SOCK_LEN 20 //存储sin_addr和sin_port


extern int CreateUdpSocket();

extern void Bind(int sockfd, const struct sockaddr_in *addr, socklen_t len);

extern void Connect(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen);

extern void Close(int sockfd);

extern ssize_t Recvfrom(int sockfd, void *msg, size_t length, int flags, struct sockaddr_in *from, socklen_t *addrlen);

extern ssize_t Sendto(int sockfd, const void *msg, size_t length,
		int flags, const struct sockaddr_in *to, socklen_t addrlen);

extern void Inet_pton(const char *strptr, void *addrptr);

extern const char *Inet_ntop(const void *addrptr, char *strptr, size_t len);

extern void set_reuse(int fd);

#endif

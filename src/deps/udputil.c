#include "udputil.h"

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

int CreateUdpSocket(){
	int n;
	if( (n = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
		QUIT("socket error");
	return n;
}

void Bind(int sockfd, const struct sockaddr_in *addr, socklen_t len){
	if(bind(sockfd, (struct sockaddr*)addr, len) < 0)
		err_quit("bind error");
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
	if(connect(sockfd, addr, addrlen) <0 )
		QUIT("connect error");
}

void Close(int sockfd){
	if(close(sockfd) < 0)
		QUIT("close error");
}

ssize_t Recvfrom(int sockfd, void *msg, size_t length, int flags, struct sockaddr_in *from, socklen_t *addrlen)
{
	ssize_t res;
	if((res = recvfrom(sockfd, msg, length, flags, (struct sockaddr*)from, addrlen)) < 0)
		err_quit("recvfrom error");
	return res;
}

ssize_t Sendto(int sockfd, const void *msg, size_t length,
		int flags, const struct sockaddr *to, socklen_t addrlen)
{
	ssize_t res;
	if((res = sendto(sockfd, msg, length, flags, to, addrlen)) < 0)
		err_quit("sendto error");
	return res;
}

void Inet_pton(const char *strptr, void *addrptr)
{
	if(inet_pton(AF_INET, strptr, addrptr) <= 0)
		err_quit("inet_pton error");
}

const char *Inet_ntop(const void *addrptr, char *strptr, size_t len)
{
	if(inet_ntop(AF_INET, addrptr, strptr, len) == NULL)
		err_quit("inet_ntop error");
	return strptr;
}

void set_reuse(int fd)
{
	int reuse0 = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse0, sizeof(reuse0)) == -1)
		err_quit("set reuse error");
}

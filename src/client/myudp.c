#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "udputil.h"
#define MAXLINE 1024

#define checkarray(L) luaL_checkudata(L, 1, "udpchat.udp");

typedef struct{
	int fd;
}Conn;

static int newsocket(lua_State *L){
	Conn *conn;
	conn = lua_newuserdata(L, sizeof(Conn));
	if(conn == NULL)
		luaL_error(L, "new userdata error");

	conn->fd = CreateUdpSocket();
	luaL_getmetatable(L, "udpchat.udp");
	lua_setmetatable(L, -2);
	return 1;
}

static int udpsendto(lua_State *L){
	struct sockaddr_in servaddr;
    Conn *conn = checkarray(L);
    char *ip = lua_tostring(L, 2); 
    int port = lua_tonumber(L, 3); 

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port=htons(port);
    Inet_pton(ip, &servaddr.sin_addr);

	char *msg = lua_tostring(L, 4); 
	int i = Sendto(conn->fd, msg, strlen(msg), 0, &servaddr, sizeof(servaddr));
	lua_pushnumber(L, i); 
	return 1;
}

//bind(udp, port)
static int udpbind(lua_State *L){
	struct sockaddr_in servaddr;
	Conn *conn = checkarray(L);
	int port = lua_tonumber(L, 2);
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port=htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(conn->fd, &servaddr, sizeof(servaddr));
	return 0;
}

//recvfrom(udp) return (msg, cliIp)
static int udprecvfrom(lua_State *L){
	struct sockaddr_in cliaddr;
	char msg[MAXLINE];
	Conn *conn = checkarray(L);

	bzero(msg, MAXLINE);
	socklen_t len = sizeof(cliaddr);
	Recvfrom(conn->fd, msg, MAXLINE, 0, &cliaddr, &len);
 	 
	lua_pushstring(L, msg);
	char ip[20];
	Inet_ntop(&cliaddr.sin_addr, ip, sizeof(ip));
	lua_pushstring(L, ip);
	return 2;
}

static const struct luaL_Reg libs_f[]= {
	{"new", newsocket},
	{NULL, NULL}
};

static const struct luaL_Reg libs_m[] = {
	{"sendto", udpsendto},
	{"listen", udpbind},
	{"recvfrom", udprecvfrom},
	{NULL, NULL}
};

int luaopen_myudp(lua_State *L){
	luaL_newmetatable(L, "udpchat.udp");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	luaL_register(L, NULL, libs_m);
	luaL_register(L, "myudp", libs_f);
	return 1;
}

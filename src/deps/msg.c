#include "msg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "client.h"
#include "udputil.h"


int msg_gettype(const char *msg){
	assert(msg);

	if(0 == strncmp(msg, "hello", 5)) {
		return MSG_HELLO;
	} else if(0 == strncmp(msg, "bye", 3)) {
		return MSG_BYE;	
	} else if(0 == strncmp(msg, "get", 3)) {
		return MSG_GET;
	}
	return MSG_NONE;
}

//"ltc\n,16777343,36550" -> data,ip,port
void msg_getclient(char *msg, Client cli){
	assert(msg);

	int i = 0;
	while(msg[i] != ',') {
		cli->name[i] = msg[i];
		i++;
	}
	cli->name[i] = '\0';
	i++;
	cli->haddr = atoi(msg+i);
	Inet_ntop(&cli->haddr, cli->addr, ADDR_LEN);

	while(msg[i] != ',') {
		i++;
	}
	i++;
	cli->hport = atoi(msg+i);
	cli->port = ntohs(cli->hport);
}



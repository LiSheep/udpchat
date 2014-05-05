#include "msg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "client.h"
#include "udputil.h"


int msg_gettype(char *msg){
	assert(msg);
	char* tmp = strdup(msg);
	char* type = strsep(&tmp, " ");
	if(strcmp(type, "hello") == 0){
		return MSG_HELLO;
	}
	if(strcmp(type, "bye") == 0){
		return MSG_BYE;	
	}
	if(strcmp(type, "get") == 0){
		return MSG_GET;
	}
	free(tmp);
	return MSG_NONE;
}

//"ltc\n,16777343,36550" -> data,ip,port
void msg_getclient(char *msg, Client cli){
	assert(msg);
	char *data = strsep(&msg, ",");
	unsigned int addr = atoi(strsep(&msg, ","));
	Inet_ntop(&addr, cli->addr, ADDR_LEN);

	cli->port = ntohs(atoi(strsep(&msg, ",")));

	strsep(&data, " ");
	char *name_tmp = strsep(&data, " ");
	char *name = strsep(&name_tmp, "\n");
	strcpy(cli->name, name);
}

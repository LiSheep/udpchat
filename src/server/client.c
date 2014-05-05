#include "client.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define T Client

void client_getSendStr(T client, char* str, size_t len){
	assert(client);
	//id,name,sockadd,socklen
	snprintf(str, len, "%d,%s,%d,%s", client->id, client->addr, client->port, client->name);
}

void client_transStr(char *str, T cli){
	assert(cli);
	assert(str);
	cli->id = atoi(strsep(&str, ","));
	strncpy(cli->addr, strsep(&str, ","), ADDR_LEN);
	cli->port = atoi(strsep(&str, ","));
	strncpy(cli->name,strsep(&str, ","), NAME_LEN); 
}

void client_getClient(char *key, T cli){
	assert(cli);
	assert(key);
}

void client_getKey(T cli, char *key){

}

#undef T

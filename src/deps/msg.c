#include "msg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <regex.h>

#include "client.h"
#include "udputil.h"


static regex_t reghello;
static regex_t regbye;
static regex_t regget;
static int regflags = REG_EXTENDED;
static int isinit = 0;

static char *helloP = "^hello (.+),([0-9]+),([0-9]+)$";
static char *byeP = "^bye ,([0-9]+),([0-9]+)";
static char *getP = "^get ,([0-9]+),([0-9]+)";

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

//"hello ltc\n,16777343,36550" -> data,ip,port
//return: success 0 , false 1
int msg_getclient(char *msg, Client cli){
	assert(msg);
	assert(cli);
	if(!isinit){
		regcomp(&reghello, helloP, regflags);
		regcomp(&regbye, byeP, regflags);
		regcomp(&regget, getP, regflags);
	}
	regmatch_t match[4];
	if(REG_NOMATCH != regexec(&reghello, msg, 4, match, 0)){
		int namelen = match[1].rm_eo - match[1].rm_so;
		strncpy(cli->name, msg + match[1].rm_so, namelen > NAME_LEN ? NAME_LEN : namelen);
		cli->haddr = atoi(msg + match[2].rm_so);
		Inet_ntop(&cli->haddr, cli->addr, ADDR_LEN);
		cli->hport = atoi(msg + match[3].rm_so);
		cli->port = ntohs(cli->hport);
		puts("match hello");
		return 0;
	}
	if(REG_NOMATCH != regexec(&regget, msg, 3, match, 0)){
		cli->haddr = atoi(msg + match[1].rm_so);
		Inet_ntop(&cli->haddr, cli->addr, ADDR_LEN);
		cli->hport = atoi(msg + match[2].rm_so);
		cli->port = ntohs(cli->hport);
		puts("match get");
		return 0;
	}
	if(REG_NOMATCH != regexec(&regbye, msg, 3, match, 0)){
		cli->haddr = atoi(msg + match[1].rm_so);
		Inet_ntop(&cli->haddr, cli->addr, ADDR_LEN);
		cli->hport = atoi(msg + match[2].rm_so);
		cli->port = ntohs(cli->hport);
		puts("match bye");
		return 0;
	}
	puts(msg);
	return 1;
}



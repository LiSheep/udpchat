#include <stdio.h>
#include <mcheck.h>

#include "udputil.h"
#include "threadutil.h"
#include "hash.h"
#include "stack.h"
#include "client.h"
#include "msg.h"


//static int iswork = 0;

int udpfd;

//thread
pthread_mutex_t pmutex;
pthread_t worker;
pthread_cond_t pcond;

Stack_T msg_stack;

static inline int init();

static inline void dispose();

static void *workerthread(void *arg);

static inline void handle_msg(char *msg);

int udpfd = 0;
int main(void){
	udpfd = init();
	while(1){	//loop to recv
		struct sockaddr_in cli_addr;
		socklen_t len = sizeof(cli_addr);
		char msg[MSG_BUFF];
		bzero(msg, MSG_BUFF);
		Recvfrom(udpfd, msg, MSG_BUFF - SOCK_LEN -1, 0, &cli_addr, &len);

		char sockinfo[SOCK_LEN];
		//msg+sockinfo
		sprintf(sockinfo, ",%u,%u", (unsigned int)cli_addr.sin_addr.s_addr,(unsigned int)cli_addr.sin_port);
		strncat(msg, sockinfo, strlen(sockinfo));

		Pthread_mutex_lock(&pmutex);
		Stack_push(msg_stack, msg, strlen(msg));
		pthread_cond_signal(&pcond);
		Pthread_mutex_unlock(&pmutex);
	}
}

static inline int init(){
	setvbuf(stdout, NULL, _IOLBF, 0);
	udpfd = CreateUdpSocket();
	struct sockaddr_in serv_addr;

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(udpfd, &serv_addr, sizeof(serv_addr));

	hash_init();
	msg_stack = Stack_new();

	//start worker thread
	Pthread_create(&worker, workerthread, NULL);
	Pthread_mutex_init(&pmutex);
	Pthread_cond_init(&pcond);

	return udpfd;
}

static inline void dispose(){
	hash_dispose();
	Pthread_mutex_destroy(&pmutex);
	Pthread_cond_destrory(&pcond);
	exit(0);	

}

static void *workerthread(void *arg){
	while(1){
		Pthread_mutex_lock(&pmutex);
		while(Stack_empty(msg_stack)){
			Pthread_cond_wait(&pcond, &pmutex);
		}
		char *msg = Stack_pop(msg_stack);
		Pthread_mutex_unlock(&pmutex);
		handle_msg(msg);
	}
	return NULL;
}

static char *buffer = NULL;	//发送报文buffer
static inline void handle_msg(char *msg){
	Client client = malloc(sizeof (*client));
	Client out;
	char *outKey;
	int currPeople;		//当前在线人数

	char tmpbuff[CLI_STR_LEN];
	int type = msg_gettype(msg);

	struct sockaddr_in cliaddr;
	bzero(&cliaddr, sizeof(cliaddr));
	int for_i = 0;
	int sprintf_long = 0;

	char sendmsg[NAME_LEN+strlen("ser{\"type\":\"hello\", \"name\":\"\"}")]; 	//ser{type="hello", name="name"}
	switch(type){
		case MSG_HELLO:
			msg_getclient(msg, client);
			cliaddr.sin_addr.s_addr = client->haddr;
			cliaddr.sin_port = client->hport;

			bzero(sendmsg, sizeof(sendmsg));
			snprintf(sendmsg, sizeof(sendmsg), "ser{\"type\":\"hello\", \"name\":\"%s\"}", client->name);
			Sendto(udpfd, sendmsg, strlen(sendmsg), 0, &cliaddr, sizeof(cliaddr));
			if(!hash_add(client->addr, client, sizeof (*client))) //该用户已上线
				return;
			if(buffer)
				free(buffer);

			currPeople = hash_count();
			buffer = malloc(currPeople * CLI_STR_LEN);
			bzero(buffer, sizeof(buffer));

			strcat(buffer, "ser{\"type\":\"get\", \"data\":[");
			sprintf_long = strlen("ser{\"type\":\"get\", \"data\":[");
			for(for_i = 0; for_i < currPeople; ++for_i){
				hash_list(&outKey, (void**)&out);
				bzero(tmpbuff, sizeof(tmpbuff));
				client_tojson(out, tmpbuff, sizeof(tmpbuff));
				if(for_i == 0){
					sprintf_long += sprintf(buffer + sprintf_long, "%s", tmpbuff);
				}else{
					sprintf_long += sprintf(buffer+sprintf_long, ",%s", tmpbuff);
				}
				if(for_i == currPeople - 1){
					strcat(buffer+sprintf_long, "]}");
				}
			}
			break;
		case MSG_BYE:
			msg_getclient(msg, client);
			cliaddr.sin_addr.s_addr = client->haddr;
			cliaddr.sin_port = client->hport;
			bzero(sendmsg, sizeof(sendmsg));
			snprintf(sendmsg, sizeof(sendmsg), "ser{\"type\":\"bye\"}");
			Sendto(udpfd, sendmsg, strlen(sendmsg), 0, &cliaddr, sizeof(cliaddr));
			hash_del(client->addr);
			break;
		case MSG_GET:
			msg_getclient(msg, client);
			cliaddr.sin_addr.s_addr = client->haddr;
			cliaddr.sin_port = client->hport;
			if(buffer != NULL)
				Sendto(udpfd, buffer, strlen(buffer), 0, &cliaddr, sizeof(cliaddr));
			break;
	}
}



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

int main(void){
	int udpfd = init();
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

static inline void handle_msg(char *msg){
	Client client = malloc(sizeof (*client));
	Client out;
	char *outKey;
	int currPeople;		//当前在线人数

	char *buffer;	//发送报文buffer
	char tmpbuff[CLI_STR_LEN];
	int type = msg_gettype(msg);
	switch(type){
		case MSG_HELLO:
			msg_getclient(msg, client);
			hash_add(client->addr, client, sizeof (*client));
			break;
		case MSG_BYE:
			msg_getclient(msg,client);
			hash_del(client->addr);
			break;
		case MSG_GET:
			currPeople = hash_count();
			buffer = malloc(currPeople * CLI_STR_LEN);
			bzero(buffer, sizeof(buffer));

			while(hash_list(&outKey, (void**)&out)){
				bzero(tmpbuff, sizeof(tmpbuff));
				client_getSendStr(out, tmpbuff, sizeof(tmpbuff));
				strncat(buffer, tmpbuff, sizeof(tmpbuff));
				strcat(buffer, "|");
			}
			printf("%s\n", buffer);
			break;
	}
}



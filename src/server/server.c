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

Hash *hash;

static inline int init();
static inline void dispose(int signo);
static void *workerthread(void *arg);
static inline void handle_msg(char *msg);
static inline void build_getmsg(char *msg);

int udpfd = 0;
int main(void){
	udpfd = init();
	while(1){	//loop to recv
		struct sockaddr_in cli_addr;
		socklen_t len = sizeof(cli_addr);
		char msg[MSG_BUFF];
		char recvmsg[MSG_BUFF - SOCK_LEN];

		memset(msg, 0, sizeof(msg));
		memset(recvmsg, 0, sizeof(recvmsg));
		Recvfrom(udpfd, recvmsg, sizeof(recvmsg), 0, &cli_addr, &len);

		//msg+sockinfo
		sprintf(msg, "%s,%u,%u", recvmsg, (unsigned int)cli_addr.sin_addr.s_addr,(unsigned int)cli_addr.sin_port);
		//strncat(msg, sockinfo, strlen(sockinfo)+1);
		//strcat(msg, sockinfo);

		Pthread_mutex_lock(&pmutex);
		Stack_push(msg_stack, msg, strlen(msg) + 1);
		pthread_cond_signal(&pcond);
		Pthread_mutex_unlock(&pmutex);
	}
}

static inline int init(){
	setvbuf(stdout, NULL, _IOLBF, 0);
	udpfd = CreateUdpSocket();
	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(udpfd, &serv_addr, sizeof(serv_addr));
	

	//hash_init();
	hash = hash_create();
	msg_init();
	msg_stack = Stack_new();

	signal(SIGINT, dispose);
	//start worker thread
	Pthread_create(&worker, workerthread, NULL);
	Pthread_mutex_init(&pmutex);
	Pthread_cond_init(&pcond);

	return udpfd;
}

static inline void dispose(int signo){
	printf("server stoping...");
	msg_dispose();
	hash_dispose();

	Stack_free(&msg_stack);
	//Pthread_mutex_destroy(&pmutex);
	//Pthread_cond_destrory(&pcond);
	exit(0);	
}

static void *workerthread(void *arg){
	Pthread_detach(pthread_self());
	while(1){
		Pthread_mutex_lock(&pmutex);
		while(Stack_empty(msg_stack)){
			Pthread_cond_wait(&pcond, &pmutex);
		}
		char *msg = Stack_pop(msg_stack);
		Pthread_mutex_unlock(&pmutex);
		handle_msg(msg);
		free(msg);
	}
	return NULL;
}

static char *buffer = NULL;	//发送报文buffer
static inline void handle_msg(char *msg){
	Client client = calloc(1, sizeof(struct Client));
	int currPeople;		//当前在线人数
	int type = msg_gettype(msg);

	struct sockaddr_in cliaddr;
	memset(&cliaddr, 0, sizeof(cliaddr));
	char sendmsg[NAME_LEN + strlen("ser{\"type\":\"hello\", \"name\":\"\"}")]; 	//ser{type="hello", name="name"}
	memset(sendmsg, 0, sizeof(sendmsg));
	if(msg_getclient(msg, client)){
		free(client);
		return;
	}
	switch(type){
		case MSG_HELLO:
			cliaddr.sin_addr.s_addr = client->haddr;
			cliaddr.sin_port = client->hport;
			memset(sendmsg, 0, sizeof(sendmsg));
			snprintf(sendmsg, sizeof(sendmsg), "ser{\"type\":\"hello\", \"name\":\"%s\"}", client->name);
			Sendto(udpfd, sendmsg, strlen(sendmsg), 0, &cliaddr, sizeof(cliaddr));
			//if(!hash_add(client->addr, client, sizeof (*client))) //该用户已上线
			if(!hash_add(hash, client->addr, client, sizeof (*client))) //该用户已上线
				return;
			if(buffer)
				free(buffer);

			currPeople = hash_count(hash);
			buffer = malloc(currPeople * CLI_STR_LEN);
			build_getmsg(buffer);
			break;
		case MSG_BYE:
			cliaddr.sin_addr.s_addr = client->haddr;
			cliaddr.sin_port = client->hport;
			memset(sendmsg, 0, sizeof(sendmsg));
			snprintf(sendmsg, sizeof(sendmsg), "ser{\"type\":\"bye\"}");
			Sendto(udpfd, sendmsg, strlen(sendmsg), 0, &cliaddr, sizeof(cliaddr));
			//hash_del(client->addr);
			hash_del(hash, client->addr);
			build_getmsg(buffer);
			break;
		case MSG_GET:
			cliaddr.sin_addr.s_addr = client->haddr;
			cliaddr.sin_port = client->hport;
			if(buffer != NULL)
				Sendto(udpfd, buffer, strlen(buffer), 0, &cliaddr, sizeof(cliaddr));
			break;
	}
	free(client);
}

static void build_getmsg(char *buffer){
	memset(buffer, 0, sizeof(buffer));
	char tmpbuff[CLI_STR_LEN];
	int sprintf_long = 0;
	int currPeople = hash_count(hash);
	int for_i = 0;
	Client out;
	char *outKey;
	strcat(buffer, "ser{\"type\":\"get\", \"data\":[");
	sprintf_long = strlen("ser{\"type\":\"get\", \"data\":[");

	hash_begin(hash);
	for(for_i = 0; for_i < currPeople && hash_next(hash, &outKey, (void**)&out); ++for_i){
		//hash_list(&outKey, (void**)&out);
		memset(tmpbuff, 0, sizeof(tmpbuff));
		client_tojson(out, tmpbuff, sizeof(tmpbuff));
		if(for_i == 0){
			sprintf_long += sprintf(buffer + sprintf_long, "%s", tmpbuff);
		}else{
			sprintf_long += sprintf(buffer+sprintf_long, ",%s", tmpbuff);
		}
	}
	strcat(buffer+sprintf_long, "]}");
}

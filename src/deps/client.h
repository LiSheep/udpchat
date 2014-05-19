#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#define T Client

#define ADDR_LEN 20
#define NAME_LEN 28

#define CLI_STR_LEN (50 + ADDR_LEN + NAME_LEN)

typedef struct T *T;

struct T{
	uint32_t id;
	int port;
	uint32_t haddr;
	uint32_t hport;
	char addr[ADDR_LEN];
	char name[NAME_LEN];
	struct timeval activeTime;
};


//将1个Client转换成json字符串
extern void client_tojson(T client, char* str, size_t len);

//将json字符串转换成Client 
extern void client_toobj(char *str, T cli);

//通过key：ip,port获取Client
extern void client_getClient(char *key, T cli);

//通过Client获取key
extern void client_getKey(T cli, char *key);


#undef T
#endif

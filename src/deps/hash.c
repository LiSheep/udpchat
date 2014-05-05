#ifndef HASH_H
#define HASH_H
#include <stdlib.h>
#include <talloc.h>
#include <string.h>
#include <assert.h>

#include "hash.h"

#define T Hash 

#define HASH_TABLE_LEN 254
#define NEW(p) p = talloc_zero(pool, T) 
#define NEW_STR(str) talloc_strdup(pool, str)
#define NEW_OBJ(obj, size) talloc_memdup(pool, obj, size)
#define FREE(p) talloc_free_children(p);


static int _count;

typedef struct T T;

struct T{
	char *key;
	void *data;
	T *next;
	T *prev;
};

static T *root[HASH_TABLE_LEN];
static TALLOC_CTX* pool;

static T *hash_get(const char *key);

//ip:127.0.0.1.1000
static int getELEM(const char* ip){
	char *tmp = malloc(strlen(ip));
	memcpy(tmp, ip, strlen(ip));
	strsep(&tmp, ".");//127
	strsep(&tmp, ".");//0
	strsep(&tmp, ".");//0
	int ret = atoi(strsep(&tmp, "."));//1
	strsep(&tmp, ".");//port
	free(tmp);
	return ret;
}

void hash_init(){
	assert(!pool);
	pool = talloc_pool(NULL, 10 * sizeof(T));
	assert(pool);
}

int hash_add(const char *key, void* data, size_t data_size){
	assert(pool);
	assert(*key);
	T *p;
	if((p = hash_get(key)) != NULL)
		return 0;

	++_count;
	unsigned int h = getELEM(key);
	NEW(p);
	p->key = NEW_STR(key);
	p->data = NEW_OBJ(data, data_size);

	p->next = root[h];
	if(root[h] != NULL){
		root[h]->prev = p;
	}
	p->prev = NULL;
	root[h] = p;
	return 1;
}

static T *hash_get(const char* key){
	assert(*key);
	unsigned int h = getELEM(key);
	T *p;
	for(p = root[h]; p; p = p->next){
		if(!strcmp(key, p->key)){	//same one
			return p;
		}
	}
	return NULL;
}

void *hash_getdata(const char* key){
	T *p = hash_get(key);
	if(!p)
		return NULL;
	else
		return p->data;
}

static T *curr = NULL;
static int _index = 0;
int hash_list(char** key, void** data){
	assert(data);
	assert(key);
	while(curr == NULL){
		curr = root[_index++];
		if(_index == 254){
			*key = NULL;
			*data = NULL;
			_index = 0;
			return 0;
		}
	}

	*key = curr->key;
	*data = curr->data;
	curr = curr->next;
	return 1;
}
	
void hash_del(const char* key){
	T *p = hash_get(key);
	assert(p);
	if(p->prev != NULL){	//not first one
		p->prev->next= p->next;
	}
	--_count;
	FREE(p);
}

int hash_count(){
	return _count;
}

void hash_dispose(){
	talloc_free(pool);
}

#undef T
#endif

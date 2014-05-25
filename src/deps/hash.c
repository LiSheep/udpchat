#include "hash.h"

#include <stdlib.h>
#include <talloc.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#ifdef T
#undef T
#endif

#define T Hash
#define NEW(size) talloc_zero_size(pool, size)
#define NEW_STR(ctx, str) talloc_strdup(ctx, str)
#define NEW_OBJ(ctx, size) talloc_zero_size(ctx, size)
#define FREE(ctx, p) talloc_unlink(p, ctx); talloc_free(p)
typedef struct listNode{
	char *key;
	struct listNode *next;
	void *data;
}listNode;

struct T{	//alloc the talloc in pool
	int count;	//count the hash
	listNode *head[HASH_TABLE_LEN + 1];
};


static TALLOC_CTX *pool;	//the talloc pool, only release when hash not needed anymore

int getIndex(const char *ip){ 
	char *tmp = malloc(strlen(ip) + 1);
	char *ptmp = tmp;
	strncpy(tmp, ip, strlen(ip) + 1);
	strsep(&tmp, ".");//127
	strsep(&tmp, ".");//0
	strsep(&tmp, ".");//0
	int ret = atoi(strsep(&tmp, "."));//1
	free(ptmp);
	return ret;
}

T *hash_create(){
	if(pool == NULL)
		pool = talloc_pool(NULL, HASH_TABLE_LEN* 10* sizeof(T));
	assert(pool);
	T *t = NEW(sizeof(T));
	for(int i = 0; i < HASH_TABLE_LEN + 1; ++i)
		t->head[i] = NEW_OBJ(t, sizeof(listNode));
	return t;
}

//return: 1 success; 0 falied
int hash_add(T *hash, const char *key, void* data, size_t data_size){
	assert(pool);
	assert(hash);
	assert(*key);

	int h = getIndex(key);
	if(hash_getdata(hash, key) != NULL)
		return 0;
	
	listNode *newnode;
	newnode = NEW_OBJ(hash, sizeof(listNode));
	newnode->key = NEW_STR(newnode, key);
	newnode->data = NEW_OBJ(newnode, data_size);
	memcpy(newnode->data, data, data_size);

	newnode->next = hash->head[h]->next;
	hash->head[h]->next = newnode;

	++hash->count;
	return 1;
}

//return: NULL if not find
void *hash_getdata(T *hash, const char *key){
	assert(pool);
	assert(hash);
	assert(key);
	int h = getIndex(key);
	listNode *currNode = hash->head[h]->next;

	for(; currNode != NULL && strcmp(currNode->key, key); currNode = currNode->next)
		continue;
	if(currNode == NULL)
		return NULL;
	else 
		return currNode->data;
}

static listNode *hash_getnode(T *hash, const char *key){
	assert(pool);
	assert(hash);
	assert(key);
	int h = getIndex(key);
	listNode *currNode = hash->head[h];
	if(currNode == NULL)
		return NULL;
	for(; currNode != NULL && strcmp(currNode->key, key); currNode = currNode->next)
		continue;
	return currNode;
}

void hash_del(T *hash, const char *key){
	assert(pool);
	assert(hash);
	assert(key);

	listNode *prev;
	listNode *curr;

	int h = getIndex(key);
	for(prev = hash->head[h], curr = prev->next; curr; 
			prev=curr, curr = curr->next){
		
		if(strcmp(curr->key, key) == 0){
			prev->next = curr->next;
			FREE(hash, curr);
			--hash->count;
			break;
		}
	}
}

static int _index = 0;
static listNode *nextNode;

void hash_begin(T *hash){
	_index = 0;
	nextNode = hash->head[_index]->next;
}

//遍历hash return 1 has next, return 0 no next
int hash_next(T* hash, char **pkey, void **pdata){
	assert(hash);
	if(NULL == nextNode && HASH_TABLE_LEN == _index){
		return 0;
	}
	if(nextNode == NULL){
		++_index;
		nextNode = hash->head[_index]->next;
		return hash_next(hash, pkey, pdata);
	}else{
		*pkey = nextNode->key;
		*pdata = nextNode->data;
		nextNode = nextNode->next;
		return 1;
	}
}

int hash_count(T *hash){
	return hash->count;
}

void hash_release(T *hash){
	FREE(pool, hash);
}

void hash_dispose(){
	talloc_free(pool);
}

#undef T

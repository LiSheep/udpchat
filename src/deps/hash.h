#ifndef HASH_H
#define HASH_H

#define Hash T
#include <stdlib.h>

typedef struct T T;

extern void hash_init();

//return: 1 success; 0 falied
extern int hash_add(const char *key, void* data, size_t data_size);

//return: NULL if not find
extern void* hash_getdata(const char *key);

extern void hash_del(const char *key);

//遍历hash
//return: 0 if 遍历完成， 1 if 遍历未完成
extern int hash_list(char** key, void** data);

extern int hash_count();

extern void hash_dispose();

#undef T
#endif

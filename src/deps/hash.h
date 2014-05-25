#ifndef HASH_H
#define HASH_H

#include <stdlib.h>

#define HASH_TABLE_LEN 254
#define T Hash 

typedef struct T T;

extern int getIndex(const char *ip);

extern T* hash_create();

//return: 1 success; 0 falied
extern int hash_add(T* hash, const char *key, void* data, size_t data_size);

//return: NULL if not find
extern void* hash_getdata(T* hash, const char *key);

extern void hash_del(T* hash, const char *key);

extern void hash_begin(T *hash);

//遍历hash return 1 has next, return 0 no next
extern int hash_next(T* hash, char **key, void **data);

//return 1 has, 0 not
extern int hash_hasnext(T *hash, char* key);

extern int hash_count(T* hash);

extern void hash_release(T* hash);

extern void hash_dispose();

#undef T
#endif

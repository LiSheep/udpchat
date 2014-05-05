#ifndef THREADUTIL_H
#define THREADUTIL_H

#include <pthread.h>
#include <stdlib.h>

#include "merror.h"


extern void Pthread_create(pthread_t *tidp, void *(*start_rtn)(void*), void *arg);

extern void Pthread_mutex_init(pthread_mutex_t *mutex);

extern void Pthread_mutex_destroy(pthread_mutex_t *mutex);

extern void Pthread_mutex_lock(pthread_mutex_t *mutex);

extern void Pthread_mutex_unlock(pthread_mutex_t *mutex);

extern void Pthread_cond_init(pthread_cond_t *cond);

extern void Pthread_cond_destrory(pthread_cond_t *cond);

extern void Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

extern void Pthread_cond_signal(pthread_cond_t *cond);

#endif

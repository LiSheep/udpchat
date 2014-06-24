#include "threadutil.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "merror.h"


void Pthread_create(pthread_t *tidp, void *(*start_rtn)(void*), void *arg){
	if(pthread_create(tidp, NULL, start_rtn, arg) != 0)
		QUIT("pthread create error");
}

void Pthread_detach(pthread_t tid){
	if(pthread_detach(tid) != 0)
		QUIT("pthread detach error");
}

void Pthread_cancel(pthread_t tid){
	if(pthread_cancel(tid) != 0)
		QUIT("pthread cancel error");
}
	
void Pthread_mutex_init(pthread_mutex_t *mutex){
	if(pthread_mutex_init(mutex, NULL) != 0)
		QUIT("pthread mutex init error");
}

void Pthread_mutex_destroy(pthread_mutex_t *mutex){
	int i = 0;
	if((i = pthread_mutex_destroy(mutex)) != 0){
		errno = i;
		QUIT("pthread mutex destory error");
	}
}

void Pthread_mutex_lock(pthread_mutex_t *mutex){
	if(pthread_mutex_lock(mutex) != 0)
		QUIT("pthread mutex lock error");
}

void Pthread_mutex_unlock(pthread_mutex_t *mutex){
	if(pthread_mutex_unlock(mutex) != 0)
		QUIT("pthread mutex unlock error");
}

void Pthread_cond_init(pthread_cond_t *cond){
	if(pthread_cond_init(cond, NULL) != 0)
		QUIT("pthread cond init error");
}

void Pthread_cond_destrory(pthread_cond_t *cond){
	if(pthread_cond_destroy(cond) != 0)
		QUIT("pthread cond destory error");
}

void Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex){
	if(pthread_cond_wait(cond, mutex) != 0)
		QUIT("pthread cond wait error");
}

void Pthread_cond_signal(pthread_cond_t *cond){
	if(pthread_cond_signal(cond) != 0){
		QUIT("pthread cond signal");
	}
}


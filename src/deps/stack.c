#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "stack.h"

#define T Stack_T

struct T{
	int count;
	struct elem{
		void *x;
		struct elem *link;
	}*head;
};

T Stack_new(void){
	T stk = malloc(sizeof *stk);
	stk->count = 0;
	stk->head = NULL;
	return stk;

}

int Stack_empty(T stk){
	assert(stk);
	return stk->count == 0;
}


void Stack_push(T stk , void *x, size_t len){
	assert(stk);

	struct elem *t = malloc(sizeof(*t));
	t->x = malloc(len);
	memcpy(t->x, x, len);
	t->link = stk->head;

	stk->head = t;
	stk->count++;
}

void *Stack_pop(T stk){

	assert(stk);
	assert(stk->count > 0);
	void *x;
	struct elem *t;
	t = stk->head;
	stk->head = t->link;
	stk->count--;
	x = t->x;
	free(t);
	return x;	//remember free x
}

void Stack_free(T* stk){
	assert(stk && *stk);
	struct elem *t, *u;

	for(t = (*stk)->head; t; t=u)
	{
		u = t->link;
		free(t);
	}
	free(*stk);

}

#undef T


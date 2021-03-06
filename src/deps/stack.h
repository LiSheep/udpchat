#ifndef STACK_H
#define STACK_H

#define T Stack_T 

#include <stdlib.h>

typedef struct T *T;

extern T Stack_new(void);
extern int Stack_empty(T stk);
extern void Stack_push(T stk, void *x, size_t len);
extern void *Stack_pop(T stk);
extern void Stack_free(T* stk);

#undef T

#endif

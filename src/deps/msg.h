#ifndef MSG_H
#define MSG_H

#include <string.h>

#include "client.h"

#define MSG_NONE	0
#define MSG_HELLO	1
#define MSG_GET		1<<1
#define MSG_BYE		1<<2

extern int msg_gettype(const char *msg);

//only effective in MSG_HELLO MSG_BYE
extern void msg_getclient(char *msg, Client cli);

#endif

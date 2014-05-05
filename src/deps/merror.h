#ifndef MERROR_H
#define MERROR_H

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define QUIT(msg) do{strerror(errno); puts("\n"); err_quit(msg);}while(1)

extern void err_quit(char * str);


#endif

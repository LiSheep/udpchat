#include "merror.h"

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

void err_quit(char * str)
{
	puts(str);
	puts(strerror(errno));
	exit(1);
}

#include <stdio.h>
#include <errno.h>
#include "helper.h"
#include <stdlib.h>

void error(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}


void flush_buffer(char* buf){
	for (int i = 0; i < MAX_STR_LEN; i++)
	{
		buf[i] = 0;
	}
}


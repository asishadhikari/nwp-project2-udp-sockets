#include <stdio.h>
#include <errno.h>

void error(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}



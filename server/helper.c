#include <stdio.h>
#include <errno.h>
#include "helper.h"
#include <stdlib.h>

void error(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}


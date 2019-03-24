#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "helper.h"

void error(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}



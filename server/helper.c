#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "helper.h"

void error(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}




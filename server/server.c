#include <sys/types.h>	
#include <sys/socket.h>
#include <stdio.h>
#include "helper.h"
#include <arpa/inet.h>


int main(int argc, char** argv){
	int list_s;
	short int port;
	struct sockaddr_in servaddr;
	char* endptr;

	//input sanitisation
	if (argc!=2){
		errno = EINVAL;
		error("usage: ./server PORT_NO\n");
	}
	else{
		port = strtol(argv[1], &endptr, 0);
		if (*endptr){
			errno = EINVAL;
			error("Invalid port number supplied!");
		}
	}


	if( (list_s = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
		error("Unable to create listening socket");
	}

	//assumption : successful creation of socket
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons();

	return 0;
}
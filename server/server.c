#include <sys/types.h>	
#include <sys/socket.h>
#include <stdio.h>
#include "helper.h"
#include <arpa/inet.h>


int main(){
	int list_s;
	struct sockaddr_in servaddr;

	if( (list_s = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
		error("Unable to create listening socket");
	}

	//assumption : successful creation of socket


	return 0;
}
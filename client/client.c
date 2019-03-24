#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "helper.h"

int main(int argc, char** argv){

	char user_input = 't';
	int soc;
	short int port;
	char *svIP, *svPort, *endPtr;
	char buffer[MAX_STR_LEN];

	struct sockaddr_in servaddr;

	if (argc!=3){
		errno = EINVAL;
		error("Usage: %s hostname port\n");	
	}

	svIP = argv[1];
	svPort = argv[2];

	port = strtol(svPort, &endPtr, 10);
	if(*endPtr)
		error("Invalid port num provided\n");

	if(soc = socket(AF_INET, SOCK_DGRAM, 0) < 0)
		error("Unable to create socket\n");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	if(!inet_aton(svIP, &servaddr.sin_addr)){
		error("Please provide a valid IPv4 address \n");
	}

	while(user_input!='q'){
		//flush buffer
		for(int i = 0; i< MAX_STR_LEN; i++)
			buffer[i] = '\0';
		printf("\n\tEnter 's' to send string, 'q' to quit... ");
		user_input = (char) getc(stdin);
		getc(stdin); //ignore newline char
		switch(user_input){
			case's':
				printf("string chosen\n");
				break;
			default:
				break;	

		}
		



	}



	return 0;
}
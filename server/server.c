#include <sys/types.h>	
#include <sys/socket.h>
#include <stdio.h>
#include "helper.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

int main(int argc, char** argv){
	int list_s;
	short int port;
	struct sockaddr_in servaddr;
	char* endptr = NULL;
	char buffer[MAX_STR_LEN];
	struct sockaddr_in activeCl;
	socklen_t addrlen = sizeof(sockaddr_in); //contains actual size on return
	time_t cur_time, prev_time;	 
	int msg_length;
	tracker ACTIVE_CLIENTS[MAX_CLIENTS];
	memset( ACTIVE_CLIENTS, 0, MAX_CLIENTS * sizeof(tracker));

	//input sanitisation
	if (argc!=2){
		errno = EINVAL;
		error("usage: ./server PORT_NO\n");
	}
	else{
		port = strtol(argv[1], &endptr, 0);
		if (endptr!=NULL && *endptr){
			errno = EINVAL;
			error("Invalid port number supplied!");
		}
	}

	if( (list_s = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
		error("Unable to create listening socket");
	}

	memset( &servaddr, 0, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if( ( bind(list_s, (struct sockaddr*) &servaddr, sizeof(servaddr)) ) < 0 ){
		error("Unable to bind name to given socket");
	}

	while(1){
		cur_time = time(0);
		flush_buffer(buffer);
		if( (msg_length = recvfrom(list_s, buffer, MAX_STR_LEN, 
			0, (struct sockaddr*) &activeCl, &addrlen) ) < 0 )
			printf("Reading from socket failed!!\n");
		printf("%s\n",buffer);
		update_active_clients(ACTIVE_CLIENTS, cur_time, &activeCl);

		//printf("Current time is %d\n",(int) (cur_time = time(0) ) );

/*		if ( (sendto(list_s, buffer, MAX_STR_LEN, 0,
			(struct sockaddr*) &activeCl, addrlen)) < 0) 
			error("Error sending message");
		printf("message sent\n");

*/	}

	return 0;
}
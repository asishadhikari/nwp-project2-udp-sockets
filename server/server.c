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
	int list_s;   									//listening socket
	short int port;									//supplied port num
	struct sockaddr_in servaddr;					//address for socket
	char* endptr = NULL;					
	char buffer[MAX_STR_LEN];						//buffer
	time_t cur_time, prev_time;	
	tracker ACTIVE_CLIENTS[MAX_CLIENTS];
	int already_added = 0;

	//initialise clients
	for (int i = 0; i < MAX_CLIENTS; i++){
		ACTIVE_CLIENTS[i].active = 0;
	}

	struct sockaddr_in activeCl;	 			//temp holder that gets overridden
	socklen_t addrlen = sizeof(struct sockaddr_in); //contains actual size on return
	int msg_length;
	tracker test_t;
	memset(&test_t, 0, sizeof(tracker));

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
		//flush buffer
		for (int i = 0; i < MAX_STR_LEN; i++)
			buffer[i] = 0;
		//receive message on socket
		if( (msg_length = recvfrom(list_s, buffer, MAX_STR_LEN, 
			0, (struct sockaddr*) &activeCl, &addrlen) ) < 0 )
			printf("Reading from socket failed!!\n");
		printf("%s\n",buffer);
		
		cur_time = time(0);		

		//update inactive clients
		for (int i = 0; i < MAX_CLIENTS; i++){
			if( ACTIVE_CLIENTS[i].active && (((int) cur_time - ACTIVE_CLIENTS[i].time_recvd) <= 15)  ){
				free(ACTIVE_CLIENTS[i].client);
				ACTIVE_CLIENTS[i].active = 0;
			}
		}


		// //check if already in the list
		// for (int i = 0; i < MAX_CLIENTS; i++){
		// 	if(ACTIVE_CLIENTS[i].active && bcmp()){

		// 	}	
		// }

		//add the new client
		for (int i = 0; i < MAX_CLIENTS; i++){
			if(!ACTIVE_CLIENTS[i].active){
				ACTIVE_CLIENTS[i].time_recvd = cur_time;
				ACTIVE_CLIENTS[i].client = (struct sockaddr*) malloc(sizeof(addrlen));
				memcpy(ACTIVE_CLIENTS[i].client, &activeCl, addrlen);
				ACTIVE_CLIENTS[i].active=1; 
				printf("client activated\n");
			}
		}



		int count =0;
		//write to all clients
		for (int i = 0; i < MAX_CLIENTS; i++){
			if( ACTIVE_CLIENTS[i].active ){
				if(sendto(list_s, buffer, MAX_STR_LEN, 0, (struct sockaddr*) &ACTIVE_CLIENTS[i], addrlen) < 0 ) 		
				perror("Writing to client failed");
				count++;
			}
		}

		printf("Currently %d clients active\n",count);

		//printf("Current time is %d\n",(int) (cur_time = time(0) ) );

/*		if ( (sendto(list_s, buffer, MAX_STR_LEN, 0,
			(struct sockaddr*) &activeCl, addrlen)) < 0) 
			error("Error sending message");
		printf("message sent\n");

*/	}

	return 0;
}
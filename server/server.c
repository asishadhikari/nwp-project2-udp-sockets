#include <sys/types.h>	
#include <sys/socket.h>
#include <stdio.h>
#include "helper.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>


int main(int 	argc, char** argv){
	int list_s;   									//listening socket
	short int port;									//supplied port num
	struct sockaddr_in servaddr;					//address for socket
	char* endptr = NULL;					
	char buffer[MAX_STR_LEN];						//buffer
	time_t cur_time, prev_time;	
	tracker ACTIVE_CLIENTS[MAX_CLIENTS];
	int already_added = 0, allocated, count;

	//initialise clients
	for (int i = 0; i < MAX_CLIENTS; i++){
		ACTIVE_CLIENTS[i].time_recvd = 0;
		ACTIVE_CLIENTS[i].client = NULL;
		ACTIVE_CLIENTS[i].active = 0;
	}

	struct sockaddr_in activeCl;	 			//temp holder that gets overridden
	socklen_t addrlen = sizeof(struct sockaddr_in); //contains actual size on return
	int msg_length;

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
		allocated = 0;
		printf("%s\n",buffer);
	
		cur_time = time(0);	

		//eliminate inactive clients
		for (int i = 0; i < MAX_CLIENTS; i++){
			if( ACTIVE_CLIENTS[i].active==1 && 
				( ((uint32_t) cur_time - (uint32_t)ACTIVE_CLIENTS[i].time_recvd) >= (uint32_t)SESSION_TIME) ){
				free(ACTIVE_CLIENTS[i].client);
				ACTIVE_CLIENTS[i].active = 0;
				printf("Client eliminated\n");

				printf("%d cur time \n", (int) cur_time);
				printf("%d initiated time \n",(int) ACTIVE_CLIENTS[i].time_recvd );
				printf("%d SESSION_TIME is \n",SESSION_TIME );
			}
		}


		// //check if already in the list
		// for (int i = 0; i < MAX_CLIENTS; i++){
		// 	if(ACTIVE_CLIENTS[i].active && bcmp()){

		// 	}	
		// }

		//add the new client
		int j = 0;
		while(!allocated && j< MAX_CLIENTS){
				if(ACTIVE_CLIENTS[j].active==0){
					ACTIVE_CLIENTS[j].time_recvd = cur_time;
					ACTIVE_CLIENTS[j].client = (struct sockaddr*) malloc(sizeof(addrlen));
					memcpy(ACTIVE_CLIENTS[j].client, &activeCl, addrlen);
					ACTIVE_CLIENTS[j].active=1; 
					printf("client activated\n");		
					allocated=1;
			}
			j++;
		}



		count = 0;
		//write to all active clients
		for (int i = 0; i < MAX_CLIENTS; i++){
			if( ACTIVE_CLIENTS[i].active==1 ){
				if(sendto(list_s, buffer, MAX_STR_LEN, 0, (struct sockaddr*) ACTIVE_CLIENTS[i].client, addrlen) < 0 ) 		
				perror("Writing to client failed");
				count++;
			}
		}

		printf("Currently %d clients\n",count);


/*		if ( (sendto(list_s, buffer, MAX_STR_LEN, 0,
			(struct sockaddr*) &activeCl, addrlen)) < 0) 
			error("Error sending message");
		printf("message sent\n");

*/	}

	return 0;
}
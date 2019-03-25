#include <sys/types.h>	
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "helper.h"


int main(int 	argc, char** argv){
	int list_s;   									//listening socket
	short int port;									//supplied port num
	struct sockaddr_in servaddr;					//address for socket
	char* endptr = NULL;					
	char buffer[MAX_STR_LEN];						//buffer
	time_t cur_time, prev_time;	
	tracker* ACTIVE_CLIENTS[MAX_CLIENTS];

	for (int i = 0; i < MAX_CLIENTS; i++){
		ACTIVE_CLIENTS[i]= (tracker*) malloc(sizeof(tracker));
		ACTIVE_CLIENTS[i]->time_recvd = 0;
		ACTIVE_CLIENTS[i]->client = NULL;
		ACTIVE_CLIENTS[i]->active = 0;
	}
	
	int already_added = 0, allocated, count;

	int num_clients = 0;

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
			buffer[i] = '\0';

		//receive message on socket
		if( (msg_length = recvfrom(list_s, buffer, MAX_STR_LEN, 
			0, (struct sockaddr*) &activeCl, &addrlen) ) < 0 )
			printf("Reading from socket failed!!\n");
		allocated = 0;
		//printf("Received from client: %s\n",buffer);
	
		cur_time = time(0);	

		//eliminate inactive clients
		for (int i = 0; i < MAX_CLIENTS; i++){
			if( ACTIVE_CLIENTS[i]->active==1 && 
				( ((uint32_t) cur_time - (uint32_t)ACTIVE_CLIENTS[i]->time_recvd) >= (uint32_t)SESSION_TIME) ){
				free(ACTIVE_CLIENTS[i]->client);
				ACTIVE_CLIENTS[i]->active = 0;
				num_clients-=1;
			}
		}

		already_added = 0;
		// //check if already in the list
		for (int i = 0; i < MAX_CLIENTS; i++){
			if(ACTIVE_CLIENTS[i]->active==1){	
				struct in_addr rec =  ((struct sockaddr_in*) ACTIVE_CLIENTS[i]->client) -> sin_addr;
				if(bcmp(&rec, &(activeCl.sin_addr),4 )==0){
					already_added =1;
					break;
				}

			}
		}

		//add the new clients
		int j = 0;
		while(!allocated && j< MAX_CLIENTS && !already_added){
				if(ACTIVE_CLIENTS[j]->active==0){
					ACTIVE_CLIENTS[j]->time_recvd = cur_time;
					ACTIVE_CLIENTS[j]->client = (struct sockaddr*) malloc(sizeof(addrlen));
					memcpy(ACTIVE_CLIENTS[j]->client, &activeCl, addrlen);
					ACTIVE_CLIENTS[j]->active=1; 
					printf("New Active Client added\n");		
					allocated=1;
					num_clients++;
				}
			j++;
		}

		//write to all active clients
		int count = 0;
		int len = strlen(buffer);

		num_clients = 0;
		if(strcmp(buffer,"\n")!=0)
			for (int i = 0; i < MAX_CLIENTS; i++){
				if( ACTIVE_CLIENTS[i]->active==1 ){
					num_clients++;
					if( (count = sendto(list_s, buffer, len, 0,
					 (struct sockaddr*) ACTIVE_CLIENTS[i]->client, addrlen)) <= 0 ){
						perror("Writing to client failed");							
					}else{
					printf("Writing to port %d \n",(int)ntohs( ((struct sockaddr_in*)ACTIVE_CLIENTS[i]->client)->sin_port));
					}//printf("Writing to IP %s \n", inet_ntoa(  ((struct sockaddr_in*)ACTIVE_CLIENTS[i].client)->sin_addr) );
				}
			}


		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			printf("%d\n",ACTIVE_CLIENTS[i]->active );
		}
	}

	return 0;
}
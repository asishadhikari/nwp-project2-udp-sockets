#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "helper.h"

void error(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}


void flush_buffer(char* buf){
	for (int i = 0; i < MAX_STR_LEN; i++)
	{
		buf[i] = 0;
	}
}

void update_active_clients(tracker* list, time_t cur_time, struct sockaddr_in* activeClp){
	
	//free inactive clients
	for (int i = 0; i < MAX_CLIENTS; i++){
		if( ((int)cur_time - list[i].time_recvd) <= 0 ){
			free(list[i].client);
			memset( &list[i], 0, sizeof(tracker) );
			list[i].active = 0;
		}
	}

	//add new client
	for (int i = 0; i < MAX_CLIENTS; i++){
		if (list[i].active==0){
			printf("Found empty spot\n");
			list[i].time_recvd = cur_time;
			list[i].client = (struct sockaddr_in*) malloc(sizeof(sockaddr_in));
			memcpy(&list[i].client, activeCl, sizeof(sockaddr_in));
		}

	}

}


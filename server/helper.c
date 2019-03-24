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



void update_active_clients(tracker* list, time_t cur_time, struct sockaddr_in* activeClp, socklen_t addrlen){
	tracker* test = (tracker*) malloc(sizeof(tracker));
	memset(test, 0, sizeof(tracker));

	//free inactive clients
	for (int i = 0; i < MAX_CLIENTS; i++){
		if( (memcmp(&list[i], test, sizeof(tracker)) ) && (((int) cur_time - list[i].time_recvd) <= 0)  ){
			free(list[i].client);
			memset( &list[i], 0, sizeof(tracker) );
			list[i].active = 0;
		}
	}

/*	//add new client
	//check if already present

	for (int i = 0; i < MAX_CLIENTS; i++){
		if( memcmp(&list[i], test, sizeof(tracker))  &&  (bcmp(activeClp->,,32))  )   {

		}

	}
*/
	for (int i = 0; i < MAX_CLIENTS; i++){
		if ( memcmp(&list[i], test, sizeof(tracker)) || list[i].active==0){
			printf("Found empty spot\n");
			list[i].time_recvd = cur_time;
			list[i].client = (struct sockaddr*) malloc((int) addrlen);
			memcpy(&list[i].client, activeClp, sizeof(addrlen));
			list[i].active = 1;
			break;
		}

	}

	free(test);

}


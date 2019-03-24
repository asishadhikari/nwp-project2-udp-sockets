#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "helper.h"

void* threadFunc(void *arg); //thread function 
void update_display();
void ask_user_input();
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
char user_input = 't';
int soc, soc1;
struct sockaddr_in servaddr;
char buffer[MAX_STR_LEN];



int main(int argc, char** argv){

	short int port;
	char *svIP, *svPort, *endPtr;


	if (argc!=3){
		errno = EINVAL;
		error("Usage: %s hostname port\n");	
	}

	svIP = argv[1];
	svPort = argv[2];

	port = strtol(svPort, &endPtr, 10);
	if(*endPtr)
		error("Invalid port num provided\n");

	if(  (soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		error("Unable to create socket\n");

	if(  (soc1 = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		error("Unable to create socket\n");


	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	if(!inet_aton(svIP, &servaddr.sin_addr)){
		error("Please provide a valid IPv4 address \n");
	}


	pthread_t t1, t2;
	int a = 1, b = 2;
	pthread_create(&t1, NULL, threadFunc, &a);
	pthread_create(&t2, NULL, threadFunc, &b);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}


void* threadFunc(void *arg){
	int t = *(int*)arg;
	if(t == 1)
		update_display();
	else if (t==2)
		ask_user_input();
	else
		error("Something wrong\n");
}



void update_display(){
	char *b = (char* )malloc(MAX_STR_LEN);
	int msg_length, s = sizeof(servaddr);
	time_t prev_time, cur_time;
	prev_time = cur_time = time(0);
	while (user_input!='q'){
		//flush buffer
		for(int i = 0; i < MAX_STR_LEN; i++)
			buffer[i]= '\0';
		cur_time = time(0);
		if(cur_time - prev_time >=5 ){
			prev_time = cur_time;	
			if (sendto(soc, "\n", 1, 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0 ) 		
				perror("Writing to server failed");
		}

		if( (msg_length = recvfrom(soc, b, MAX_STR_LEN, 
			MSG_DONTWAIT, (struct sockaddr*) &servaddr, &s) ) > 1)
			printf("buffer is %s ahaaa\n",buffer);

	}

	free(b);

}

void ask_user_input(){
	int len;
	while(user_input!='q'){
		//flush buffer
		for(int i = 0; i< MAX_STR_LEN; i++)
			buffer[i] = '\0';
		printf("\n\tEnter 's' to send string, 'q' to quit... \n");
		user_input = (char) getc(stdin);
		getc(stdin); //ignore newline char
		if(user_input=='s'){
			fgets(buffer, MAX_STR_LEN ,stdin);
			len = strlen(buffer);
			if (sendto(soc1, buffer, len, 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0 ) 		
				perror("Writing to server failed");
		}
	}


}
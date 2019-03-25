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
void keep_alive();
void ask_user_input();
void update_buffer();

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
char user_input = 't';
int soc, dirty = 0;
struct sockaddr_in servaddr;

char buffer[MAX_STR_LEN];
char wbuffer[MAX_STR_LEN];




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


	if( ( bind(soc, (struct sockaddr*) &servaddr, sizeof(servaddr)) ) < 0 ){
		error("Unable to bind name to given socket");
	}


	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	if(!inet_aton(svIP, &servaddr.sin_addr)){
		error("Please provide a valid IPv4 address \n");
	}


	pthread_t t1, t2,t3;
	int a = 1, b = 2, c= 3;
	pthread_create(&t1, NULL, threadFunc, &a);
	pthread_create(&t2, NULL, threadFunc, &b);
	pthread_create(&t3, NULL, threadFunc, &c);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3,NULL);

	return 0;
}


void* threadFunc(void *arg){
	int t = *(int*)arg;
	if(t == 1)
		keep_alive();
	else if (t==2)
		ask_user_input();
	else if (t==3)
		update_buffer();
}



void keep_alive(){
	int msg_length, s = sizeof(servaddr);
	time_t prev_time, cur_time;
	prev_time = cur_time = time(0);
	while (user_input!='q'){
		//flush buffer
		cur_time = time(0);
		//send '\n' every 5 s
		if(cur_time - prev_time >=5 ){
			prev_time = cur_time;
			pthread_mutex_lock(&mutex1);	
			if (sendto(soc, "\n", 1, 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0 ) 		
				perror("Writing to server failed");
			pthread_mutex_unlock(&mutex1);
		}
	}
	exit(EXIT_SUCCESS);
}

void update_buffer(){
	int s = sizeof(servaddr);
	while(user_input!='q'){
		//flush buffer
		for(int i = 0; i< MAX_STR_LEN; i++)
			buffer[i] = '\0';
		if(recvfrom(soc, buffer, MAX_STR_LEN, 
			0, (struct sockaddr*) &servaddr, &s)  > 1)
			printf("\n Received data from server:  %s\n",buffer);
	}
	exit(EXIT_SUCCESS);  //most likely this will not run as it is a blocking call
}


void ask_user_input(){
	int len;
	while(user_input!='q'){
		//flush buffer
		for(int i = 0; i< MAX_STR_LEN; i++)
			wbuffer[i] = '\0';
		printf("\n\tEnter 's' to send string, 'q' to quit... \n");
		user_input = (char) getc(stdin);
		getc(stdin); //ignore newline char
		if(user_input=='s'){
			fgets(wbuffer, MAX_STR_LEN ,stdin);
			len = strlen(wbuffer);
			pthread_mutex_lock(&mutex1);
			if (sendto(soc, wbuffer, len, 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0 ) 		
				perror("Writing to server failed");
			pthread_mutex_unlock(&mutex1);
		}		
	}
	exit(EXIT_SUCCESS);
}
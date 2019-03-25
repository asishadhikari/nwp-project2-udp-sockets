#ifndef HELPER
#define HELPER
#define MAX_CLIENTS 100
#define SESSION_TIME 15  //15 seconds till considered inactive
#define MAX_STR_LEN 10000

typedef struct tracker
{
	time_t time_recvd;
	struct sockaddr* client;
	int active;
	
}tracker;


typedef struct sockaddr sockaddr_t;


void error(char* msg);



#endif

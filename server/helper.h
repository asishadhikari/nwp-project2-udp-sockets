#ifndef HELPER
#define HELPER
#define MAX_CLIENTS 6
#define MAX_STR_LEN 100

typedef struct tracker
{
	time_t time_recvd;
	struct sockaddr* client;
	int active;
	
}tracker;


typedef struct sockaddr sockaddr_t;


void error(char* msg);



#endif

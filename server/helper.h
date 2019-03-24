#ifndef HELPER
#define HELPER
#define MAX_CLIENTS 5
#define MAX_STR_LEN 100

typedef struct tracker
{
	time_t time_recvd;
	struct sockaddr* client;
	int active;
	
}tracker;


typedef struct sockaddr_in sockaddr_in_t;


void error(char* msg);
void update_active_clients(tracker* list, time_t cur_time, sockaddr_in_t* activeCl, socklen_t addrlen);



#endif

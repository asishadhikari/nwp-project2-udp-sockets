#ifndef HELPER
#define HELPER
#define MAX_CLIENTS 2
#define MAX_STR_LEN 1

typedef struct tracker
{
	time_t time_recvd;
	struct sockaddr_in* client;
	int active;
	
}tracker;



void error(char* msg);
void flush_buffer(char* buf);
void update_active_clients(tracker* list, time_t cur_time, struct sockaddr_in* activeCl, socklen_t addrlen);



#endif

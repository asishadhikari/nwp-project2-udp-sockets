#ifndef HELPER
#define HELPER
#define MAX_CLIENTS 1000
#define MAX_STR_LEN 10000

typedef struct tracker
{
	time_t time_recvd;
	struct sockaddr_in* client;
	int active;
	
}tracker;

typedef struct sockaddr_in sockaddr_in;


void error(char* msg);
void flush_buffer(char* buf);
void update_active_clients(tracker* list, time_t cur_time, struct sockaddr_in* activeCl);



#endif

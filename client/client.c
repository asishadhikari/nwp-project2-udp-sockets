#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char** argv){

	char user_input = 't';

	if (argc!=3){
		errno = EINVAL;
		error("Usage: %s hostname port\n");	
	}


	return 0;
}
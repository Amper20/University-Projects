extern "C" {
        #include <stdio.h>
        #include <stdlib.h>
        #include <unistd.h>
        #include <string.h>
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <arpa/inet.h>
        #include <netdb.h>
}

#include <iostream>
#include "utils.hpp"

using namespace std;

class Server{
public:
    
	fd_set readFds; // general set
	fd_set tmpFds; //temporary set
	struct sockaddr_in servAddr; //server adress
	int setSize, listenSocket, portno, ret; //max set value
    char buff[BUFFER_LEN];

	void init( char * argv1 ){
		FD_ZERO(&readFds);
		FD_ZERO(&tmpFds);
		listenSocket = socket(AF_INET, SOCK_STREAM, 0);
		portno = atoi(argv1);

		memset((char *) &servAddr, 0, sizeof(servAddr));
		servAddr.sin_port = htons(portno);
		servAddr.sin_addr.s_addr = INADDR_ANY;
		servAddr.sin_family = AF_INET;
		
		bind(listenSocket, (struct sockaddr *) &servAddr, sizeof(struct sockaddr));
		listen(listenSocket, MAX_CLINETS_NUM);

		FD_SET(listenSocket, &readFds);
		setSize = listenSocket;
	}

	void updateConnections(){
		for(int i = 0; i <= setSize; i++){
			tmpFds = readFds; 
			ret = select(setSize + 1, &tmpFds, NULL, NULL, NULL);
		}
	}

};

int main(int argc, char *argv[]){
        
	Server server;
	server.init(argv[1]);
	while(1){
		server.updateConnections();
	}

    return 0;
}
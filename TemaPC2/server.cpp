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
		tmpFds = readFds; 
		if (select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0)
				errorOccured("updateConnSelect", __LINE__);
		if (FD_ISSET(listenSocket, &tmpFds)){
			struct sockaddr_in newClientAddr;
			socklen_t newClientSize = sizeof(newClientAddr);
			int newClientSocket = accept(listenSocket, (struct sockaddr *) &newClientAddr, &newClientSize);
			if (newClientSocket < 0)
				errorOccured("updateSocketSelect", __LINE__);
			FD_SET(newClientSocket, &readFds);
			setSize = max(setSize, newClientSocket); 
			//NEW CONNECTION MESSAGE
			printf("Conn %d new\n", newClientSocket);
		}
	}
	
	void closeConnection(int delClientSocket){
		printf("Conn %d closed\n", delClientSocket);
		close(delClientSocket);
		FD_CLR(delClientSocket, &readFds);
	}

	void updateMessages(){
		tmpFds = readFds; 
		for(int i = 0; i <= setSize; i++){
			if(FD_ISSET(i, &tmpFds)){
				memset(buff, 0, BUFFER_LEN);
				int recvSize = recv(i, buff, sizeof(buff), 0);
				if (recvSize < 0) errorOccured("messageRecv", __LINE__);
				if (recvSize == 0) {closeConnection(i); continue;}
				printf ("Clinet %d sent  %s\n", i, buff);
			}
		}
	}

};

int main(int argc, char *argv[]){
        
	Server server;
	server.init(argv[1]);
	while(1){
		server.updateConnections();
		server.updateMessages();
	}

    return 0;
}
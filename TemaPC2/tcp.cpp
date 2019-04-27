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

class Client{
public:	
	int clientSocket, messageSize, setSize;
	struct sockaddr_in servAddr;
	char buff[BUFFER_LEN];
	fd_set readFds; // general set
	fd_set tmpFds; //temporary set


	void init(char *argv[]){
		clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if(clientSocket < 0) errorOccured("clientSocket", __LINE__);

		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(atoi(argv[2]));
		if(inet_aton(argv[1], &servAddr.sin_addr) < 0 ) errorOccured ("inetAton", __LINE__);
		if(connect(clientSocket, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0 ) errorOccured ("connect", __LINE__);

		FD_ZERO(&readFds);
		FD_ZERO(&tmpFds);
		FD_SET(clientSocket, &readFds);
		setSize = clientSocket;
		FD_SET(STDIN_FILENO, &readFds);
		setSize = max(clientSocket, STDIN_FILENO);	
	}

	void updateInput(){
		tmpFds = readFds;
		if(select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0) errorOccured("selectError", __LINE__);
		if(FD_ISSET(STDIN_FILENO, &tmpFds)){
			memset(buff, 0, BUFFER_LEN);
			fgets(buff, BUFFER_LEN - 1, stdin);

			if (strncmp(buff, "exit", 4) == 0) {
				//close client
				;
			}

			// se trimite mesaj la server
			cout << buff << " --";
			if(send(clientSocket, buff, strlen(buff), 0) < 0)
				errorOccured("sendError", __LINE__);
			
		}
	}
};

int main(int argc, char *argv[]){
	
	if (argc < 3)  errorOccured("fewArguments", __LINE__);

	Client client;
	client.init(argv);
	while(1){
		client.updateInput();
	}
}
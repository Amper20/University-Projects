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

//client class keeps relevant data for client
class Client{
public:	

	int clientSocket, messageSize, setSize;
	struct sockaddr_in servAddr;
	char buff[BUFFER_LEN];
	fd_set readFds; // general set
	fd_set tmpFds; //temporary set

	//inits client
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
		FD_SET(STDIN_FILENO, &readFds);
		
		setSize = clientSocket;
		setSize = max(clientSocket, STDIN_FILENO);	
	}

	//reads from stdin
	int updateInput(){
		
		tmpFds = readFds;
		if(select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0) errorOccured("selectError", __LINE__);
		
		if(FD_ISSET(STDIN_FILENO, &tmpFds)){
			memset(buff, 0, BUFFER_LEN);
			fgets(buff, BUFFER_LEN - 1, stdin);

			if (strncmp(buff, "exit", 4) == 0) {
				if(send(clientSocket, buff, sizeof(buff), 0) < 0)
					errorOccured("sendError", __LINE__);
				close(clientSocket);
				return 0;
			}

			// se trimite mesaj la server
			if(send(clientSocket, buff, sizeof(buff), 0) < 0) errorOccured("sendError", __LINE__);
		}
		return 1;
	}

	//receives messages if exit kill curent instance of program
	int receiveData(){
		
		tmpFds = readFds;
		if(select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0) errorOccured("selectError", __LINE__);

		if(FD_ISSET(clientSocket, &tmpFds)){
			
			memset(buff, 0, BUFFER_LEN);
			int recvSz = recv(clientSocket, buff, sizeof(buff), 0);
			
			if (strncmp(buff, "exit\n", 4) == 0 || !recvSz) {
				close(clientSocket);
				return 0;
			}
			printf("%s\n", buff);
		}
		return 1;
	}
};

int main(int argc, char *argv[]){
	
	if (argc < 3)  errorOccured("fewArguments", __LINE__);

	Client client;
	client.init(argv);

	while(1){
		if (!client.updateInput() || !client.receiveData())
			break;
	}
	return 0;
}
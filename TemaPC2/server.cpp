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
#include <vector>
#include <map>
#include "utils.hpp"

using namespace std;

class Client{
public:
	
	string id;
	int connected = 0, socket = -1;
	struct sockaddr_in clientAddr;

	map<string, int> SF;
	map<string, int> subscribed;
	map<string, vector<string>> clientMessages;

	Client (string id, int connected, int socket, struct sockaddr_in clientAddr){
		this->id = id;
		this->connected = connected;
		this->clientAddr = clientAddr;
		this->socket = socket;
		string ip =  getIP(&clientAddr);
		cout << "New client " << id << " connected from "<< ip << ":" << getPort(&clientAddr) << endl;
	}

};

class Server{
public:
    
	fd_set readFds; // general set
	fd_set tmpFds; //temporary set
	struct sockaddr_in servAddr; //server adress
	int setSize, listenSocket, portno, ret, udpDataSocket; //max set value
    char buff[BUFFER_LEN];
	map<int, Client*> clients;
	map <int, struct sockaddr_in> clientAddr;

	void init( char * argv1 ){
		FD_ZERO(&readFds);
		FD_ZERO(&tmpFds);
		listenSocket = socket(AF_INET, SOCK_STREAM, 0);
		udpDataSocket = socket(AF_INET, SOCK_DGRAM, 0);

		portno = atoi(argv1);

		memset((char *) &servAddr, 0, sizeof(servAddr));
		servAddr.sin_port = htons(portno);
		servAddr.sin_addr.s_addr = INADDR_ANY;
		servAddr.sin_family = AF_INET;
		
		bind(listenSocket, (struct sockaddr *) &servAddr, sizeof(struct sockaddr));
		bind(udpDataSocket, (struct sockaddr *) &servAddr, sizeof(struct sockaddr));

		listen(listenSocket, MAX_CLINETS_NUM);

		FD_SET(udpDataSocket, &readFds);
		FD_SET(listenSocket, &readFds);

		setSize = max(listenSocket, udpDataSocket);
	}

	void updateConnections(){
		tmpFds = readFds; 
		if (select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0)
				errorOccured("updateConnSelect", __LINE__);
		if (FD_ISSET(listenSocket, &tmpFds)){
			
			struct sockaddr_in newClientAddr;
			socklen_t newClientSize = sizeof(newClientAddr);
			int newClientSocket = accept(listenSocket, (struct sockaddr *) &newClientAddr, &newClientSize);
			
			clientAddr[newClientSocket] = newClientAddr;

			if (newClientSocket < 0)
				errorOccured("updateSocketSelect", __LINE__);
			
			FD_SET(newClientSocket, &readFds);
			setSize = max(setSize, newClientSocket); 

		}
	}

	void updateUDPMessages(){

		while (FD_ISSET(udpDataSocket, &tmpFds)){

			tmpFds = readFds; 
			if (select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0)
				errorOccured("updateConnSelect", __LINE__);	

			struct sockaddr_in udpAddr;
			socklen_t udpAddrSize = sizeof(udpAddr);
			int rec = recvfrom(udpDataSocket, buff, sizeof(buff), 0, (struct sockaddr *) &udpAddr, &udpAddrSize);
			
			printf("%s <- buff udp\n", buff);
			
			string topic = "";
			string message = generateMessage(topic, buff, rec,  (struct sockaddr_in *) &udpAddr); 
			
			cout << message << endl;

			tmpFds = readFds; 

			if (select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0)
				errorOccured("updateConnSelect", __LINE__);
		}
	}
	
	void closeConnection(int delClientSocket){
		string id = "";
		for(unsigned int i = 0 ; i < clients.size(); i++)
			if(clients[i]->socket == delClientSocket){
				id = clients[i]->id;
				clients.erase(i);
				break;
			}
		printf("Client %s disconnected\n", id.c_str());
		close(delClientSocket);
		FD_CLR(delClientSocket, &readFds);
	}

	void updateTCPMessages(){
		tmpFds = readFds; 
		if (select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0)
				errorOccured("updateConnSelect", __LINE__);

		for(int i = 0; i <= setSize; i++){
			if(FD_ISSET(i, &tmpFds) && i != listenSocket && i != udpDataSocket && i != STDIN_FILENO){
				memset(buff, 0, BUFFER_LEN);
				int recvSize = recv(i, buff, sizeof(buff), 0);
				if (recvSize < 0) errorOccured("messageRecv", __LINE__);
				if (recvSize == 0) {closeConnection(i); continue;}

				printf("%s <- buff tcp \n", buff);
				
				string buffAsString = buff;
				
				//gives clinet ID
				if(buffAsString.find("\"") != string::npos){
					for(unsigned int j = 0; j < buffAsString.length(); j++){
						if(buffAsString [j] == '\"'){
							string id = "";
							for(;j + 1 < buffAsString.length() && buffAsString[j + 1] != '\"'; j++, id += buffAsString[j]);
							if( id != "")
								clients[i] = new  Client(id, 1, i, clientAddr[i]);
						}
					}
					continue;
				}

				string topic = "";
				int sf = getSFandTopic(buffAsString, topic);
					
				//subscribed
				if (buffAsString.find("unsubscribe") != string::npos){
					if( topic != ""){
						clients[i]->subscribed[topic] = 1;	
						clients[i]->SF[topic] = sf; 
					}
				}else{
				//unsubscribe
					clients[i]->subscribed.erase(topic);
					clients[i]->SF.erase(topic);
				}
				


			}
		}
	}

};

int main(int argc, char *argv[]){
	setbuf(stdout, NULL);
	Server server;
	server.init(argv[1]);
	while(1){
		server.updateConnections();
		server.updateTCPMessages();
		server.updateUDPMessages();
	}

    return 0;
}
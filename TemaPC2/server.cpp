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
		#include <netinet/tcp.h>
		#include <netinet/in.h>
}

#include <iostream>
#include <vector>
#include <map>
#include "utils.hpp"

using namespace std;

// client Class contains clients data
class Client{
public:
	
	string id;
	int connected = 0, socket = -1;
	struct sockaddr_in clientAddr;

	map<string, int> SF;
	map<string, int> subscribed;
	vector<string> clientMessages;

	//constructor - initiates a client 
	Client (string id, int connected, int socket, struct sockaddr_in clientAddr){
		this->id = id;
		this->connected = connected;
		this->clientAddr = clientAddr;
		this->socket = socket;
		string ip =  getIP(&clientAddr);
		cout << "New client " << id << " connected from "<< ip << ":" << getPort(&clientAddr) << endl;
	}

	//acts like toString() - displays clients info
	void dispClient(){
		cout << id << " " << connected << "\n" << "messages:\n";
		for(unsigned int i = 0 ; i < clientMessages.size(); i++)
			cout << "-" << clientMessages[i] << "-";
		for(map<string,int>::iterator it = subscribed.begin(); it != subscribed.end(); it++){
			cout << it->first << ":" << it->second << "\n";
		}
	}

};

//class Server contains all data related to server 
//clients are stored into an map (key:clientSocket -> value:*Client)

class Server{
public:
    
	fd_set readFds; // general set
	fd_set tmpFds; //temporary set
	struct sockaddr_in servAddr; //server adress
	int setSize, listenSocket, portno, ret, udpDataSocket, dichi = 1; //max set value
    char buff[BUFFER_LEN];
	map<int, Client*> clients; //map that contains clients 
	map <int, struct sockaddr_in> clientAddr; // map that contains socket->addres data

	//function iterates over clients and displays them
	void dispClients(){
		for(map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++){
			it->second->dispClient();
		}
	}

	//function initializes all server related variables
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
		setsockopt(listenSocket, IPPROTO_TCP, TCP_NODELAY, &dichi, sizeof(int));

		listen(listenSocket, MAX_CLINETS_NUM);

		FD_SET(listenSocket, &readFds);
		FD_SET(udpDataSocket, &readFds);
		FD_SET(STDIN_FILENO, &readFds);

		setSize = max( STDIN_FILENO, max(listenSocket, udpDataSocket));
	}

	//function accepts new connections from tcp clients 
	void updateConnections(){
		tmpFds = readFds; 
		if (select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0)
				errorOccured("updateConnSelect", __LINE__);
		if (FD_ISSET(listenSocket, &tmpFds)){
			
			struct sockaddr_in newClientAddr;
			socklen_t newClientSize = sizeof(newClientAddr);
			int newClientSocket = accept(listenSocket, (struct sockaddr *) &newClientAddr, &newClientSize);
			setsockopt(newClientSocket, IPPROTO_TCP, TCP_NODELAY, &dichi, sizeof(int));
		
			clientAddr[newClientSocket] = newClientAddr;

			if (newClientSocket < 0)
				errorOccured("updateSocketSelect", __LINE__);
			
			FD_SET(newClientSocket, &readFds);
			setSize = max(setSize, newClientSocket); 

		}
	}

	//function receives messages from udp clients
	void updateUDPMessages(){

		tmpFds = readFds; 
		if (select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0)
			errorOccured("updateConnSelect", __LINE__);	

		if (FD_ISSET(udpDataSocket, &tmpFds)){
			struct sockaddr_in udpAddr;
			socklen_t udpAddrSize = sizeof(udpAddr);
			memset(buff, 0, sizeof(buff));
			int rec = recvfrom(udpDataSocket, buff, sizeof(buff), 0, (struct sockaddr *) &udpAddr, &udpAddrSize);
			if(rec > 0){
				//DEBUGprintf("%s <- buff udp\n", buff);
				string topic = "";
				string message = generateMessage(topic, buff, rec,  (struct sockaddr_in *) &udpAddr); 
				updateMap(message, topic);
			}
		}
	}

	//function updates messages that have to be sent to clients
	void updateMap(string message, string topic){
		//DEBUGcout << "update "  << message << " " << topic << "\n";
		int cnt = 0;
		for(map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++){
			if((it->second->subscribed[topic] == 1 && it->second->connected) || it->second->SF[topic] == 1){
				it->second->clientMessages.push_back(message);
				//DEBUGcout << it->second->id << " <-id \n";
				cnt++;
			}
		}
		//DEBUGcout << cnt << "!!!!!!!!" << endl;
	}

	//function terminates a connection with a cleint
	void closeConnection(int delClientSocket){
		string id = "";
		for(map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
			if(it->second->socket == delClientSocket){
				id = it->second->id;
				it->second->connected = 0;
				break;
			}
		printf("Client %s disconnected\n", id.c_str());
		close(delClientSocket);
		FD_CLR(delClientSocket, &readFds);
	}

	//function receives messages from tcp clients
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
				//DEBUGprintf("%s <- buff tcp \n", buff);
				string buffAsString = buff;
				
				//gives clinet ID
				if(buffAsString.find("\"") != string::npos){
					for(unsigned int j = 0; j < buffAsString.length(); j++){
						if(buffAsString [j] == '\"'){
							string id = "";
							for(;j + 1 < buffAsString.length() && buffAsString[j + 1] != '\"'; j++, id += buffAsString[j]);
							if( id != ""){
								int clientExists = 0;
								for(map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
									if(it->second->id == id){
										it->second->connected = 1;
										it->second->socket = i;
										it->second->clientAddr = clientAddr[i];
										clientExists = 1;
										sendMessages();
										break;
									}

								if(!clientExists) 
									clients[i] = new Client(id, 1, i, clientAddr[i]);
								//DEBUGcout <<"clntSZ" << clients.size() << "\n";
							}
						}
					}
					continue;
				}

				string topic = "";
				int sf = getSFandTopic(buffAsString, topic);
					
				//subscribed
				if (buffAsString.find("unsubscribe") == string::npos){
					if( topic != "" && buffAsString.find("subscribe") != string::npos){
						//DEBUGcout << "sub->" << topic  << "<- " << sf << endl;
						clients[i]->subscribed[topic] = 1;	
						clients[i]->SF[topic] = sf; 
					}
				}else{
				//unsubscribe
					//DEBUGcout << "unsub->" << topic << endl;
					clients[i]->subscribed[topic] = 0;
					clients[i]->SF[topic] = 0;
				}
				//DEBUGdispClients();
			}
		}
	}

	//function send messages to tcp clients
	void sendMessages(){
		for(map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
			if(it->second->connected && it->second->clientMessages.size() != 0)
				for (;!it->second->clientMessages.empty();it->second->clientMessages.pop_back()){
					string msg = it->second->clientMessages.back();
					sprintf(buff, "%s", msg.c_str());
					if(send(it->second->socket, buff, sizeof(buff), 0) < 0)
						errorOccured("sendError", __LINE__);
				}
	}

	//functions sends a messages that closes all clients
	void killAll(){
		for(map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++){
			string msg = "exit";
			sprintf(buff, "%s", msg.c_str());
			//DEBUGcout << "send" << msg << "\n";
			if(send(it->second->socket, buff, sizeof(buff), 0) < 0)
				errorOccured("sendError", __LINE__);
			delete it->second;
		}
	}

	//reads input from stdin
	int readInput(){
		tmpFds = readFds;
		if(select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0) errorOccured("selectError", __LINE__);
		if(FD_ISSET(STDIN_FILENO, &tmpFds)){
			memset(buff, 0, BUFFER_LEN);
			fgets(buff, BUFFER_LEN - 1, stdin);
			if (strncmp(buff, "exit", 4) == 0)
				return 0;
		}
		return 1;
	}
};

int main(int argc, char *argv[]){
	setbuf(stdout, NULL);
	Server server;
	server.init(argv[1]);
	while(1){
		if(!server.readInput()) 
			break;
		server.updateConnections();
		server.updateTCPMessages();
		server.updateUDPMessages();
		server.sendMessages();
	}
	server.killAll();
    return 0;
}
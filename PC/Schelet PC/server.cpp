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
	
	
	int socket = -1, acc = 0;
	struct sockaddr_in clientAddr;
        string ip;

        Client (int socket, struct sockaddr_in clientAddr){
		this->clientAddr = clientAddr;
		this->socket = socket;
		string ip =  getIP(&clientAddr);
		cout << "New client connected from "<< ip << ":" << getPort(&clientAddr) << endl;
                this->ip = ip + ":" + to_string(getPort(&clientAddr));
        }

	string dispClient(){
                if(socket == -1)
                        return "";
		return  "Socket " + to_string(socket) +  ": ACC = " + to_string(acc) + "\n";
	}
        void updateACC(char op, int val){
                if(op == '+')
                        acc += val;
                if(op == '-')
                        acc -= val;
                if(op == '*')
                        acc *= val;
        }
};

//class Server contains all data related to server 
//clients are stored into an map (key:clientSocket -> value:*Client)

class Server{
public:
    
	fd_set readFds; // general set
	fd_set tmpFds; //temporary set
	struct sockaddr_in servAddr; //server adress
	int setSize, listenSocket, portno, ret, dichi = 1; //max set value
    	char buff[BUFFER_LEN];
	vector<Client *> clients; //map that contains clients 
	map <int, struct sockaddr_in> clientAddr; // map that contains socket->addres data

	//function iterates over clients and displays them
	string dispClients(){
                string ret = "";
		for(unsigned int i = 0; i < clients.size(); i++){
			ret += clients[i]->dispClient();
		}
                return ret;
	}

	//function initializes all server related variables
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
		setsockopt(listenSocket, IPPROTO_TCP, TCP_NODELAY, &dichi, sizeof(int));

		listen(listenSocket, MAX_CLINETS_NUM);

		FD_SET(listenSocket, &readFds);
		FD_SET(STDIN_FILENO, &readFds);

		setSize = max( STDIN_FILENO, listenSocket);
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
                        Client * newClient = new Client(newClientSocket, newClientAddr);
                        clients.push_back(newClient);
			if (newClientSocket < 0)
				errorOccured("updateSocketSelect", __LINE__);
			
			FD_SET(newClientSocket, &readFds);
			setSize = max(setSize, newClientSocket); 

		}
	}

	//function terminates a connection with a cleint
	void closeConnection(int delClientSocket){
		for(unsigned int i = 0 ; i  < clients.size(); i++){
                        if(clients[i]->socket == delClientSocket){
                                if(clients[i]->socket != -1)
                                        cout << "Clinet " << clients[i]->ip << " disconected\n";
                                clients[i]->socket = -1;
                        }
                }
		close(delClientSocket);
		FD_CLR(delClientSocket, &readFds);
	}

	//function receives messages from tcp clients
	void updateTCPMessages(){
		tmpFds = readFds; 
		if (select(setSize + 1, &tmpFds, NULL, NULL, NULL) < 0)
				errorOccured("updateConnSelect", __LINE__);

		for(int i = 0; i <= setSize; i++){
			if(FD_ISSET(i, &tmpFds) && i != listenSocket && i != STDIN_FILENO){
				memset(buff, 0, BUFFER_LEN);
				int recvSize = recv(i, buff, sizeof(buff), 0);
				if (recvSize < 0) errorOccured("messageRecv", __LINE__);
				string buffAsString = buff;
                                if (recvSize == 0 || buffAsString.find("exit") != string::npos) {closeConnection(i); continue;}
                                if(buffAsString.find("show memory") != string::npos){
                                        string msg = dispClients();
                                        sprintf(buff, "%s", msg.c_str());
                                        if(send(i, buff, sizeof(buff), 0) < 0) errorOccured("sendError", __LINE__);
                                                
                                }else{
                                        char op = buffAsString[0];
                                        int val = atoi(buffAsString.substr(1,buffAsString.length()).c_str());
                                        cout << op << " " << val << "\n";
                                        for(unsigned int j = 0 ; j < clients.size(); j++){
                                                if(clients[j]->socket == i){
                                                        clients[j]->updateACC(op, val);
                                                }
                                        }
                                }


			}
		}
	}

	//functions sends a messages that closes all clients
	void killAll(){
		for(unsigned int i = 0 ; i < clients.size(); i++){
                        closeConnection(clients[i]->socket);
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
	}
	server.killAll();
    return 0;
}
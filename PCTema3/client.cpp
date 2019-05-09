extern "C" {
	#include <stdlib.h>
	#include <stdio.h>
	#include <unistd.h>
	#include <string.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include "parson.h"
}

#include "utils.hpp"
#include "request.hpp"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define LINE_SIZE 5000

// primeste ca parametru un string
//intaorce ip-ul asociat adresei 
string get_ip(string name){
	string ret;
	struct addrinfo hints, *info, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;  
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(name.c_str(), "http", &hints, &info);
	p = info;
	while(p != NULL){
		struct sockaddr_in* addr = (struct sockaddr_in*)p->ai_addr;
		string ip = inet_ntoa(addr->sin_addr);
		ret = ip;
		p = p->ai_next;

	}
	freeaddrinfo(info);
	return ret;
}



class Client{
public:
	
	
	int socket;
	
	//task1 response strings
	string dataTask1, type, url, method, body, urlParams, token;
	vector<string> coockies;
	string dataUrl, dataUrlParams, dataMethod;
	char hostCharVal[30] =  "185.118.200.35";
	Client(){};

	//extrage coockies din raspunsul dat de server
	vector<string> getCoockies(string response){
		vector<string> ret;
		while(response.find("Set-Cookie: ") != string::npos){
			int start = response.find("Set-Cookie");
			string rest = response.substr(start + 12, response.length());
			int end = rest.find("httponly");
			ret.push_back(rest.substr(0, end + 8));
			response = rest;
		}
		return ret;
	}

	void solveTask1(){
		//deschidem conexiunea si trimitem primul request obtinand primul raspuns
		socket = open_connection(hostCharVal, 8081, AF_INET, SOCK_STREAM, 0);
		string taskRequest = GET(hostCharVal, "/task1/start", "", "", coockies);
		send_to_server(socket, taskRequest.c_str());
		string response  = receive_from_server(socket);
		cout << "________________TASK1________________\n";
		cout << response << "\n\n";

		//extragem datele din rapsuns pentru a rezolva taskul urmator
		char jsonChar[LINE_SIZE];
		memcpy(jsonChar, (response.substr(response.find("\r\n\r\n")+4,response.length())).c_str(), response.length() - response.find("\r\n\r\n") - 4); 
		JSON_Value  *parser = json_parse_string(jsonChar);
		string userName = json_object_dotget_string(json_value_get_object(parser), "data.username");
		string userPassword = json_object_dotget_string(json_value_get_object(parser), "data.password");
		body = "username=" + userName + "&password=" + userPassword;
		
		//extragem datele din rapsuns pentru a rezolva taskul urmator
		type = json_object_get_string(json_value_get_object(parser), "type");
		url = json_object_get_string(json_value_get_object(parser), "url");
		method = json_object_get_string(json_value_get_object(parser), "method");
		coockies =  getCoockies(response);
	}

	void solveTask2(){
		if(method == "POST"){
			//deschidem conexiunea si trimitem requestul obtinand raspunsul 
			socket = open_connection(hostCharVal, 8081, AF_INET, SOCK_STREAM, 0);
			string taskRequest = POST(hostCharVal, url, body, type, method, coockies, "");
			send_to_server(socket, taskRequest.c_str());
			string response  = receive_from_server(socket);
			cout << "________________TASK2________________\n";
			cout << response << "\n\n";

			//extragem datele din rapsuns pentru a rezolva taskul urmator
			char jsonChar[LINE_SIZE];
			memcpy(jsonChar, (response.substr(response.find("\r\n\r\n")+4,response.length())).c_str(), response.length() - response.find("\r\n\r\n") - 4); 
			JSON_Value  *parser = json_parse_string(jsonChar);
			string id = json_object_dotget_string(json_value_get_object(parser), "data.queryParams.id");
			token = json_object_dotget_string(json_value_get_object(parser), "data.token");
			url = json_object_get_string(json_value_get_object(parser), "url");
			method = json_object_get_string(json_value_get_object(parser), "method");
			urlParams = "raspuns1=omul&raspuns2=numele&id="  + id;
			coockies =  getCoockies(response);
		}

	}
	void solveTask3(){
		if(method == "GET"){
			//deschidem conexiunea si trimitem requestul obtinand raspunsul
			socket = open_connection(hostCharVal, 8081, AF_INET, SOCK_STREAM, 0);
			string taskRequest = GET(hostCharVal, url, urlParams, token, coockies);
			cout << taskRequest << "\n";
			send_to_server(socket, taskRequest.c_str());
			string response  = receive_from_server(socket);
			cout << "________________TASK3________________\n";
			cout << response << "\n\n";
			char jsonChar[LINE_SIZE];
			memcpy(jsonChar, (response.substr(response.find("\r\n\r\n")+4,response.length())).c_str(), response.length() - response.find("\r\n\r\n") - 4); 
			
			//extragem datele din rapsuns pentru a rezolva taskul urmator
			JSON_Value  *parser = json_parse_string(jsonChar);
			url = json_object_get_string(json_value_get_object(parser), "url");
			method = json_object_get_string(json_value_get_object(parser), "method");
			coockies =  getCoockies(response);
		}
	}

	void solveTask4(){
		if(method == "GET"){
			//deschidem conexiunea si trimitem requestul obtinand raspunsul
			socket = open_connection(hostCharVal, 8081, AF_INET, SOCK_STREAM, 0);
			string taskRequest = GET(hostCharVal, url, "", token, coockies);
			send_to_server(socket, taskRequest.c_str());
			string response  = receive_from_server(socket);
			cout << "________________TASK4________________\n";
			cout << response << "\n\n";

			//extragem datele din rapsuns pentru a rezolva taskul urmator
			char jsonChar[LINE_SIZE];
			memcpy(jsonChar, (response.substr(response.find("\r\n\r\n")+4,response.length())).c_str(), response.length() - response.find("\r\n\r\n") - 4); 
			JSON_Value  *parser = json_parse_string(jsonChar);
			url = json_object_get_string(json_value_get_object(parser), "url");
			method = json_object_get_string(json_value_get_object(parser), "method");
			type = json_object_get_string(json_value_get_object(parser), "type");
			dataMethod = json_object_dotget_string(json_value_get_object(parser), "data.method");
			dataUrl = json_object_dotget_string(json_value_get_object(parser), "data.url");
			string q = json_object_dotget_string(json_value_get_object(parser), "data.queryParams.q");
			string appid = json_object_dotget_string(json_value_get_object(parser), "data.queryParams.APPID");
			dataUrlParams = "q=" + q + "&APPID=" + appid; 
			coockies =  getCoockies(response);
		}
	}
	void solveTask5(){
		if(dataMethod == "GET" && method == "POST"){
			// extragem ip-ul serverului pentru a trimite requestul pentru obtinerea starii vremii
			string host = get_ip(dataUrl.substr(0, dataUrl.find("/")));
			char hostChar[host.length() + 1]; 
    			strcpy(hostChar, host.c_str());
			vector<string> empt;
			string taskRequest = GET(host, dataUrl.substr(dataUrl.find("/"), dataUrl.length()), dataUrlParams, "", empt);
			socket = open_connection(hostChar, 80, AF_INET, SOCK_STREAM, 0);
			send_to_server(socket, taskRequest.c_str());
			string response  = receive_from_server(socket);
			cout << "________________TASK5________________\n";
			cout << response << "\n\n";
			body = (response.substr(response.find("\r\n\r\n")+4,response.length()));
			//expediem postul final
			socket = open_connection(hostCharVal, 8081, AF_INET, SOCK_STREAM, 0);
			taskRequest = POST(hostCharVal, url, body, type, method, coockies, token);
			send_to_server(socket, taskRequest.c_str());
			response  = receive_from_server(socket);
			response =  (response.substr(response.find("\r\n\r\n")+4,response.length()));
			cout << "----------------------------------------Yeah BOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOIIIIIIII----------------------------------------\n"<< response << "\n";
		}
	}
};

int main(){
	Client client;
	client.solveTask1();
	client.solveTask2();
	client.solveTask3();
	client.solveTask4();
	client.solveTask5();
	
}
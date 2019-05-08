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

class Client{
public:
	
	
	int socket;
	
	//task1 response strings
	string dataTask1, type, url, method, body, urlParams, token;
	vector<string> coockies;

	Client(){};

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
		socket = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
		string taskRequest = GET("185.118.200.35", "/task1/start", "", "", coockies);
		send_to_server(socket, taskRequest.c_str());
		
		string response  = receive_from_server(socket);
		cout << "________________TASK1________________\n";
		cout << response << "\n\n";
		char jsonChar[LINE_SIZE];
		memcpy(jsonChar, (response.substr(response.find("\r\n\r\n")+4,response.length())).c_str(), response.length() - response.find("\r\n\r\n") - 4); 

		JSON_Value  *parser = json_parse_string(jsonChar);
		
		string userName = json_object_dotget_string(json_value_get_object(parser), "data.username");
		string userPassword = json_object_dotget_string(json_value_get_object(parser), "data.password");
		body = "username=" + userName + "&password=" + userPassword;
		
		type = json_object_get_string(json_value_get_object(parser), "type");
		url = json_object_get_string(json_value_get_object(parser), "url");
		method = json_object_get_string(json_value_get_object(parser), "method");
		coockies =  getCoockies(response);
	}

	void solveTask2(){
		if(method == "POST"){
			socket = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
			string taskRequest = POST("185.118.200.35", url, body, type, method, coockies);
			send_to_server(socket, taskRequest.c_str());
			string response  = receive_from_server(socket);

			cout << "________________TASK2________________\n";
			cout << response << "\n\n";

			

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
			socket = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
			string taskRequest = GET("185.118.200.35", url, urlParams, token, coockies);
			send_to_server(socket, taskRequest.c_str());
			string response  = receive_from_server(socket);
			cout << "________________TASK3________________\n";
			cout << response << "\n\n";
			char jsonChar[LINE_SIZE];
			memcpy(jsonChar, (response.substr(response.find("\r\n\r\n")+4,response.length())).c_str(), response.length() - response.find("\r\n\r\n") - 4); 
			
			JSON_Value  *parser = json_parse_string(jsonChar);
			url = json_object_get_string(json_value_get_object(parser), "url");
			method = json_object_get_string(json_value_get_object(parser), "method");
			coockies =  getCoockies(response);
		}
	}

	void solveTask4(){
		if(method == "GET"){
			socket = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
			string taskRequest = GET("185.118.200.35", url, "", token, coockies);
			send_to_server(socket, taskRequest.c_str());
			string response  = receive_from_server(socket);
			cout << "________________TASK4________________\n";
			cout << response << "\n\n";
			char jsonChar[LINE_SIZE];
			memcpy(jsonChar, (response.substr(response.find("\r\n\r\n")+4,response.length())).c_str(), response.length() - response.find("\r\n\r\n") - 4); 
			
			// JSON_Value  *parser = json_parse_string(jsonChar);
			// url = json_object_get_string(json_value_get_object(parser), "url");
			// method = json_object_get_string(json_value_get_object(parser), "method");
			// coockies =  getCoockies(response);
		}
	}
};

int main(){
	Client client;
	client.solveTask1();
	client.solveTask2();
	client.solveTask3();
	client.solveTask4();
}
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

using namespace std;

class Client{
public:
	
	
	int socket;
	
	void print_commits_info(const char *username, const char *repo) {
		JSON_Value *root_value;
		JSON_Array *commits;
		JSON_Object *commit;
		size_t i;
		
		char curl_command[512];
		char cleanup_command[256];
		char output_filename[] = "commits.json";
		
		/* it ain't pretty, but it's not a libcurl tutorial */
		sprintf(curl_command, 
			"curl -s \"https://api.github.com/repos/%s/%s/commits\" > %s",
			username, repo, output_filename);
		sprintf(cleanup_command, "rm -f %s", output_filename);
		system(curl_command);
		
		/* parsing json and validating output */
		root_value = json_parse_file(output_filename);
		if (json_value_get_type(root_value) != JSONArray) {
			system(cleanup_command);
			return;
		}
		
		/* getting array from root value and printing commit info */
		commits = json_value_get_array(root_value);
		printf("%-10.10s %-10.10s %s\n", "Date", "SHA", "Author");
		for (i = 0; i < json_array_get_count(commits); i++) {
			commit = json_array_get_object(commits, i);
			printf("%.10s %.10s %s\n",
				json_object_dotget_string(commit, "commit.author.date"),
				json_object_get_string(commit, "sha"),
				json_object_dotget_string(commit, "commit.author.name"));
		}
		
		/* cleanup code */
		json_value_free(root_value);

	}
	
	Client(){
		socket = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
	};

	void solveTask1(){
		
		string taskRequest = GET("185.118.200.35", "/task1/start", "");
		send_to_server(socket, taskRequest.c_str());
		
		string response  = receive_from_server(socket);
		char buffer[2000];
		memcpy(buffer, (response.substr(response.find("\r\n\r\n")+4,response.length())).c_str(), response.length() - response.find("\r\n\r\n")-4); 
		printf("->%s<-", buffer);

		JSON_Value  *parser = json_parse_string(buffer);

		JSON_Object *obj = json_object(parser);
		printf("%s", json_object_dotget_string(obj, "data"));
		//string data = json_value_get_string(parser);
		//cout << data;
		// commits = json_value_get_array(parser);
		
		// for (i = 0; i < json_array_get_count(commits); i++) {
			
		// 	cout << "entry\n";

		// 	commit = json_array_get_object(commits, i);
			
		// 	printf("%.10s %.10s %s\n",
		// 		json_object_dotget_string(commit, "commit.author.date"),
		// 		json_object_get_string(commit, "sha"),
		// 		json_object_dotget_string(commit, "commit.author.name"));
		// }
		
		//print_commits_info("torvalds", "linux");
	}
};

int main(){
	Client client;
	client.solveTask1();
}
extern "C" {
        #include <stdlib.h>
        #include <stdio.h>
        #include <unistd.h>
        #include <string.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <netdb.h>
        #include <arpa/inet.h>
}

#include "utils.hpp"
#include "request.hpp"
#include<vector>
#include <string>

using namespace std;

string GET(string host, string url, string params, string auth, vector<string> coockies){
        string ret = "GET " + url + (params != "" ? "?" : "" ) + params + " HTTP/1.1\r\n";
        ret += "Host: " + host + "\r\n";
        if(auth != "")
                ret += "Authorization: Bearer " + auth + "\r\n";
        for(int i = 0 ; i < coockies.size(); i++){
                ret += "Cookie: " + coockies[i] + "\r\n";
        }
        ret += "\r\n"; // final line
        return ret;
}


string POST(string host, string url, string data, string type, string method, vector<string> coockies){
        string ret = method + " " + url + " HTTP/1.1\r\n";
        ret += "Host: " + host + "\r\n";
        for(int i = 0 ; i < coockies.size(); i++){
                ret += "Cookie: " + coockies[i] + "\r\n";
        }
        ret += "Content-Type: " + type + "\r\n";
        ret +=  "Content-Length: " + to_string(data.length()) + "\r\n\r\n";
        ret += data;
        ret += "\r\n"; // final line
        return ret;
}
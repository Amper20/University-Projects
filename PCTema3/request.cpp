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

#include <string>
using namespace std;

string GET(string host, string url, string params){
        string ret = "GET " + url + (params == "" ? "?" : "" ) + params + " HTTP/1.1\r\n";
        ret += "Host: " + host + "\r\n";
        
        ret += "\r\n"; // final line
        return ret;
}


string POST(string host, string url, string data, string type){
        string ret = "POST " + url + " HTTP/1.1\r\n";
        ret += "Host: " + host + "\r\n";
        ret += "Content-Type: " + type + "\r\n";
        ret +=  "Content-Length: " + to_string(data.length()) + "\r\n";

        ret += "\r\n"; // final line
        return ret;
}
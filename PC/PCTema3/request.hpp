#ifndef REQUEST_HPP
#define REQUEST_HPP

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

#include "request.hpp"
#include <vector>

using namespace std;

string GET(string host, string url, string params, string auth, vector<string> coockies);

string POST(string host, string url, string data, string type, string method, vector<string> coockies, string auth);

#endif
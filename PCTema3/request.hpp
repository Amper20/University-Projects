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

using namespace std;

string GET(string host, string url, string params);

string POST(string host, string url, string data);
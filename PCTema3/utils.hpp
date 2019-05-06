#ifndef UTILS_HPP
#define UTILS_HPP

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

#include <string>

int open_connection(char *hostIp, int portno, int ipType, int socketType, int flag);

void close_connection(int sockfd);

void error(std::string msg);

void send_to_server(int sockfd, const char *message);

char *receive_from_server(int sockfd);

#endif
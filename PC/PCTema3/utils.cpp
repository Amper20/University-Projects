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
#include <iostream>
#define BUFLEN 5000

void error(std::string msg){
    std::cout << (msg);
    exit(0);
}

int open_connection(char *hostIp, int portno, int ipType, int socketType, int flag){

    struct sockaddr_in serv_addr;
    int sockfd = socket(ipType, socketType, flag);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ipType;
    serv_addr.sin_port = htons(portno);
    inet_aton(hostIp, &serv_addr.sin_addr);
    
    if (sockfd < 0)
        error("ERROR opening socket");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}

void close_connection(int sockfd){
    close(sockfd);
}

void send_to_server(int sockfd, const char *message){
    int bytes, sent = 0;
    int total = strlen(message);
    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent += bytes;
    } while (sent < total);
}

char *receive_from_server(int sockfd){
    char *response = (char *)calloc(BUFLEN, sizeof(char));
    int total = BUFLEN;
    int received = 0;
    do
    {
        int bytes = read(sockfd, response + received, total - received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
        {
            break;
        }
        received += bytes;
    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket");

    return response;
}


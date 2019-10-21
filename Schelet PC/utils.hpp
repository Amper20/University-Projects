#ifndef FD_SET_HANDLER_HPP
#define FD_SET_HANDLER_HPP

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

// am pus mai mult, las sa fie
#define BUFFER_LEN 1700
#define MAX_CLINETS_NUM 100

void errorOccured(std::string message, int lineNum);

int getPort(struct sockaddr_in *sin);

char* getIP(struct sockaddr_in *sin);

int getSFandTopic(std::string buffAsString, std::string & topic);

std::string generateMessage(std::string &topic, char *buff, int rec, struct sockaddr_in * udpAddr);

#endif
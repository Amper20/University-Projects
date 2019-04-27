#ifndef FD_SET_HANDLER_HPP
#define FD_SET_HANDLER_HPP

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
 
#define BUFFER_LEN 1500
#define MAX_CLINETS_NUM 100

void errorOccured(std::string message, int lineNum);


#endif
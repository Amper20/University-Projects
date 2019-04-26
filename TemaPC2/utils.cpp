#include "utils.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void errorOccured(char *message, int lineNum){
	fprintf(stderr, "exited with error message: -->%s %d<-- \n", message, lineNum);
	exit(0);
}
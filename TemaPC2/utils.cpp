#include "utils.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

void errorOccured(std::string message, int lineNum){
	std::cout << "exited with error message: --> "<< message << " " << lineNum << " <-- \n";
	exit(0);
}
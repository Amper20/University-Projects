#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helpers.h"

void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, n, ret;
	struct sockaddr_in serv_addr;
	char buffer[BUFLEN];

	if (argc < 3) {
		usage(argv[0]);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	ret = inet_aton(argv[1], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");
	
	
	fd_set read_set;	// multimea de citire folosita in select()
	int fdmax;
	FD_ZERO(&read_set);

	FD_SET(sockfd, &read_set);
	fdmax = sockfd;
	FD_SET(STDIN_FILENO, &read_set);
	
	if(STDIN_FILENO > fdmax)
	    fdmax = STDIN_FILENO;  
	
	while (1) {

		fd_set tmp_fds = read_set; 
		
		int rc = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(rc < 0, "select");

		for (int i = 0; i <= fdmax; i++) {  // parcurs socketii din multime
			if (FD_ISSET(i, &tmp_fds)) { // pot citi de pe i
				if(STDIN_FILENO == i){
			  		// se citeste de la tastatura
					memset(buffer, 0, BUFLEN);
					fgets(buffer, BUFLEN - 1, stdin);

					if (strncmp(buffer, "exit", 4) == 0) {
						break;
					}

					// se trimite mesaj la server
					n = send(sockfd, buffer, strlen(buffer), 0);
					DIE(n < 0, "send");
				}else{
					memset(buffer, 0, BUFLEN);
					n = recv(i, buffer, sizeof(buffer), 0);
					DIE(n < 0, "recv");
					printf ("S-a primit de la clientul de pe socketul %d mesajul: %s\n", i, buffer);
				}
			}

		}
	}

	close(sockfd);

	return 0;
}

/*
 * Protocoale de comunicatii:
 * Laborator 6: UDP
 * mini-server de backup fisiere
 */
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helpers.h"


void usage(char*file)
{
	fprintf(stderr, "Usage: %s server_port file\n", file);
	exit(0);
}

/*
 * Utilizare: ./server server_port nume_fisier
 */
int main(int argc, char **argv)
{
	if (argc != 3)
		usage(argv[0]);

	int fd, sock;
	socklen_t socklen;
	struct sockaddr_in from_station;
	char buf[BUFLEN];
	int ret;

	/* TODO deschidere socket */

	/* TODO setare struct sockaddr_in pentru a specifica unde trimit
	 * datele */

	/* TODO legare proprietăți de socket */

	/* TODO deschidere fișier pentru scriere */

	/*
	 * TODO
	 * cat_timp  mai_pot_citi
	 *		citeste din socket
	 *		pune in fisier
	 */

	/* TODO închidere socket */

	/* TODO închidere fișier */

	return 0;
}

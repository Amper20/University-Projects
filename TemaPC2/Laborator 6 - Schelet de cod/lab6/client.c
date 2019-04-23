/*
 * Protocoale de comunicatii:
 * Laborator 6: UDP
 * client mini-server de backup fisiere
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


void usage(char *file)
{
	fprintf(stderr, "Usage: %s ip_server port_server file\n", file);
	exit(0);
}

/*
 * Utilizare: ./client ip_server port_server nume_fisier_trimis
 */
int main(int argc, char **argv)
{
	if (argc != 4)
		usage(argv[0]);

	int fd, sock;
	struct sockaddr_in to_station;
	char buf[BUFLEN];
	int ret;

	/* TODO deschidere socket */

	/* TODO setare struct sockaddr_in pentru a specifica unde trimit
	 * datele */

	/* TODO deschidere fișier pentru citire */

	/*
	 * TODO
	 * cat_timp  mai_pot_citi
	 *		citeste din fisier
	 *		trimite pe socket
	 */

	/* TODO închidere socket */

	/* TODO închidere fișier */

	return 0;
}

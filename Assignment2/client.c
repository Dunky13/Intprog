/*
 * client.c
 * Belonging to the second assignment of Internet Programming at Vrije Universiteit, Amsterdam
 * Marc Went and Ferry Avis
 * September and October 2015
 */

#define PORT 4444

#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv){    
    if(argc != 2){
    	fprintf(stderr, "Usage: %s hostname", argv[0]);
    	exit(-1);
    }

	struct sockaddr_in server_struct;
	struct hostent *resolv;
	struct in_addr *addr;
	int socketfd;
	int socket_option;
	uint32_t *buf;

	/* Get IP from hostname of server */
	if((resolv = gethostbyname(argv[1])) == NULL){
		printf("Address not found for %s\n", argv[1]);
		return -1;
	}

	addr = (struct in_addr*) resolv->h_addr_list[0];	/*already in network byte order*/

	/* Fill in struct */
	server_struct.sin_family = AF_INET;
	server_struct.sin_port = htons(PORT);
	server_struct.sin_addr = *addr;

	/* Buffer for receive */
	buf = malloc(sizeof(uint32_t));

	/* Create socket */
	if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		return -1;
	}

	/* Set socket option so we can quickly run again after termination */
	socket_option = 1;
	
	if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &socket_option, sizeof(int)) < 0){
		perror("setsockopt REUSEADDR");
		return -1;		
	}

	/* Connect to server*/
	if(connect(socketfd, (struct sockaddr *) &server_struct, sizeof(struct sockaddr_in)) != 0){
		perror("connect");
		return -1;
	}

	/* Wait for answer */
	if(read(socketfd, buf, sizeof(uint32_t)) < 0){
		perror("read");
		return -1;
	}

	/* Print result */
	printf("I received: %d\n", ntohl(*buf));

	if(close(socketfd) < 0){
		perror("Close socket");
		return -1;
	}

	free(buf);

	return 0;
}
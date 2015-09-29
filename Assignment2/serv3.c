#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 4444
#define BACKLOG 5
#define NB_PROC 10

//Slide 37
ssize_t writen(int fd, const void *vptr, size_t n){
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;
	
	ptr = vptr;
	nleft = n;
	
	while (nleft > 0) {
		if((nwritten = write(fd, ptr, nleft)) <= 0 ){
			if(errno == EINTR){
				nwritten = 0; /* and call write() again */
			}
			else{
				return -1;
			}
		}
		nleft -= nwritten;
		ptr += nwritten;
	}

	return n;
}

void sig_chld(int) {
	while(waitpid(0, NULL, WNOHANG) > 0){

	}

	signal(SIGCHLD,sig_chld);
}

void treat_request(int fd, uint32_t *counter){
	uint32_t send_counter = 0;

	//increase counter in shared memory
	(*counter)++;
	send_counter = htonl(*counter);
	writen(fd, &send_counter, sizeof(uint32_t));
}

void recv_requests(int fd) { /* An iterative server */
	int new_socket;
	
	/* Loop to serve requests */
	while(1){
		/* INSERT MUTEX (performance) */
		if((new_socket = accept(socketfd, (struct sockaddr *) &receive_struct, &addrlen)) < 0){
			perror("socket");
			return -1;
		}

		if(fork() == 0){
			treat_request(new_socket, &counter);
		}

		if(close(new_socket) < 0){
			perror("Close socket from accept");
			return -1;
		}
	}
}

int main(int argc, char **argv){    
    if(argc != 1){
    	fprintf(stderr, "Please call without parameters");
    	exit(-1);
    }

	struct sockaddr_in receive_struct;
    uint32_t counter;
    int socketfd, socket_option, i;
    socklen_t addrlen;
    void *buf;

    signal(SIGCHLD, sig_chld);

    counter = 0;
    buf = malloc(sizeof(uint32_t));

    /* Fill in structure */
   	receive_struct.sin_family = AF_INET;
	receive_struct.sin_port = htons(PORT);
	receive_struct.sin_addr.s_addr = htonl(INADDR_ANY);

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

	/* Bind and listen to address */
	if(bind(socketfd, (struct sockaddr *) &receive_struct, sizeof(struct sockaddr_in)) < 0){
		perror("bind");
		return -1;
	}

	if(listen(socketfd, BACKLOG) < 0){
		perror("listen");
		return -1;
	}

	addrlen = sizeof(struct sockaddr_in);

	/* Create NB_PROC children */
	for(i = 0; i < NB_PROC; i++){
		if(fork() == 0){
			recv_requests(socketfd);
		}
	}

	free(buf);

	if(close(socketfd) < 0){
		perror("Close socket");
		return -1;
	}

	return 0;
}
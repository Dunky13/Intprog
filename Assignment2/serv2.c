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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define PORT 4444
#define BACKLOG 5

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

/*void sig_chld(int error) { //NOG NIET GOED
	while(waitpid(0, NULL, WNOHANG) > 0){

	}

	signal(SIGCHLD,sig_chld);
}*/

void treat_request(int fd, uint32_t *counter, int counter_sem, struct sembuf *up, struct sembuf *down){
	uint32_t send_counter = 0;

	/* Obtain lock on shared memory */
    semop(counter_sem, down, 1);

	/* increase counter in shared memory */
	(*counter)++;
	send_counter = htonl(*counter);

	/* Free lock */	
	semop(counter_sem, up, 1);

	/* Send */
	writen(fd, &send_counter, sizeof(uint32_t));
}

int main(int argc, char **argv){    
    if(argc != 1){
    	fprintf(stderr, "Please call without parameters");
    	exit(-1);
    }

	struct sockaddr_in receive_struct;
    uint32_t *shm_counter;
    int shmid, counter_sem, socketfd, new_socket, socket_option;
    socklen_t addrlen;
    void *buf;

    struct sembuf up	= {0,  1, 0};
	struct sembuf down	= {0, -1, 0};

    //signal(SIGCHLD, sig_chld);

    /* Initialize shared counter and semaphore for counter */
    shmid = shmget(IPC_PRIVATE, sizeof(uint32_t), 0600);
    shm_counter = (uint32_t *) shmat(shmid, 0, 0);
    *shm_counter = 0;
    counter_sem = semget(IPC_PRIVATE, 1, 0600);
    semop(counter_sem, &up, 1);

    /* Allocate buffer */
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

	/* Loop to serve all requests */
	addrlen = sizeof(struct sockaddr_in);

	while(1){
		if((new_socket = accept(socketfd, (struct sockaddr *) &receive_struct, &addrlen)) < 0){
			perror("socket");
			return -1;
		}

		if(fork() == 0){
			treat_request(new_socket, shm_counter, counter_sem, &up, &down);
		}

		if(close(new_socket) < 0){
			perror("Close socket from accept");
			return -1;
		}
	}

	free(buf);
	shmdt((void *) shm_counter);
	/*shmctl(shmid, IPC_RMID, 0); and semctl(my_sem, 0, IPC_RMID); need to be called at server, but it will never reach these statements */

	if(close(socketfd) < 0){
		perror("Close socket");
		return -1;
	}

	return 0;
}
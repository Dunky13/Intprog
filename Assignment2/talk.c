#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include<sys/wait.h>

#define PORT 5555

int server(int sockfd){
	int bind_err, listen_err, close_err, accept_desc;

	struct sockaddr_in server_addr, client_addr;
	
	struct socklen_t server_addrlen, client_addrlen;

	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	server_addrlen = (socklen_t) sizeof(struct sockaddr_in);

	err = bind(sockfd, (struct sockaddr *) &server_addr, server_addrlen);
	if(err < 0){
		perror("Could not bind to socket");
		exit(1);
	}
	
	listen_err = listen(sockfd, 5);
	if(listen_err < 0){
		perror("Could not listen to socket");
		exit(1);
	}
	
	client_addrlen = (socklen_t) sizeof(struct sockaddr_in);
	accept_desc = accept(sockfd, (struct sockaddr *) client_addr, &client_addrlen);
	if(accept_desc < 0){
		perror("Could not accept from client");
		exit(1);
	}
	
	close_err = close(sockfd);
	if(close_err < 0){
		perror("Could not close socket");
		exit(1);
	}
}

int client(int sockfd, char* loc){
	struct hostent *resolv;
	struct in_addr *addr;
	struct sockaddr_in server_addr;
	
	int conn_err, close_err;
	
	resolv = gethostbyname(loc);
	if(resolv == NULL){
		printf("Address not found for %s\n", name);
		return -1;
	}
	addr = (struct in_addr*) resolv->h_addr_list[0];
	
	server_addr.sin_family       = AF_INET;
	server_addr.sin_port         = htons(PORT);
	server_addr.sin_addr.s_addr  = inet_addr(inet_ntoa(*addr));
	conn_err = connect(sockfd, (struct sockaddr *) &server_addr, (socklen_t) sizeof(struct sockaddr_in));
	if(conn_err < 0){
		perror("Could not connect");
		exit(1);
	}
	
	close_err = close(sockfd);
	if(close_err < 0){
		perror("Could not close socket");
		exit(1);
	}
	
}

int main(int argc, char **argv){
  int sockfd;

  /*check for correct input*/
  if(argc>2){
    fprintf(stderr,"Usage: talk [server address]");
    exit(1);
  }
  
  /*create socket*/
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd<0){
    perror("Error creating socket");
    exit(1);
  }
  
 
  /*decide whether process is client or server*/
  if(argc==1){ 
    server(sockfd);
  }else{
    client(sockfd, argv[1]);
  }
  return 0;
}

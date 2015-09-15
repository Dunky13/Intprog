/*
 * mysh1.sh
 * Belonging to the first assignment of Internet Programming at Vrije Universiteit, Amsterdam
 * Marc Went and Ferry Avis
 * September 2015
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 	100

void *trim(char *in, char *out){
	char *end;

	while(isspace(*in) != '\0'){
		in++;
	}

	if(*in != '\0'){ /*if true, the input is not empty*/
		end = in + strlen(in) - 1;	/*set pointer to end of string*/
		
		while(end > in && isspace(*end) != 0){
			end--;
		}

		*(end+1) = '\0'; /*end string*/
	}

	strncpy(out, in, strlen(in)+1);
}

void execute(char *command){
	pid_t pid;
	int status;

	pid = fork();

	if(pid < 0){
		perror("fork");
		exit(-1);
	}
	else if(pid == 0){	/*child*/
		execlp(command, command, NULL);
		perror("exec"); /*will not be reached in case of a success, as exec replaces the complete program*/
		exit(-1);
	}
	else{
		wait(&status);

		if(status < 0){
			perror("wait");
			exit(-1);
		}
	}
}
 
main(int argc, char **argv){
    
    if(argc != 1){
    	fprintf(stderr, "Usage: command\n%d parameters given", argc);
    	exit(-1);
    }

	char input[BUF_SIZE];
	char command[BUF_SIZE];

	printf("\nCommand: ");

	while(fgets(input, sizeof(input), stdin)){
		trim(input, command);

		if(strcmp("exit", command) == 0){
			printf("exiting...\n");
			exit(0);
		}

		execute(command);
		printf("\nCommand: ");		
	}

    return 0;
}
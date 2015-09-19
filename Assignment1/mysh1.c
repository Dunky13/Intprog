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
#include <ctype.h>
#include <sys/wait.h>

#define BUF_SIZE 	100
#define PROMPT_TEXT "$ "

/* Removes all white space at the beginning and end of the input */
void trim(char *in, char *out){
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

/* Function to execute requested program at child */
void child_code(char *command){
	execlp(command, command, NULL);

	perror("exec"); /*will not be reached in case of a success, as exec replaces the complete program*/
	exit(-1);
}

/* Function to wait for output of child at parent */
void parent_code(){
	int status;

	wait(&status);

	if(status < 0){
		perror("wait");
		exit(-1);
	}
}

/* Execute the given command. Does not support arguments */
void execute(char *input){
	pid_t pid;
	char command[BUF_SIZE];

	trim(input, command);

	if(strcmp("exit", command) == 0){
		printf("exiting...\n");
		exit(0);
	}
	else if(strcmp("", command) != 0){
		pid = fork();

		if(pid < 0){
			perror("fork");
			exit(-1);
		}
		else if(pid == 0){	/*child*/
			child_code(command);
		}
		else{				/* parent */
			parent_code();			
		}
	}
}
 
int main(int argc, char **argv){    
    if(argc != 1){
    	fprintf(stderr, "Please start without parameters");
    	exit(-1);
    }

	char input[BUF_SIZE];

	printf(PROMPT_TEXT);

	while(fgets(input, sizeof(input), stdin)){
		execute(input);
		printf(PROMPT_TEXT);		
	}

    return 0;
}
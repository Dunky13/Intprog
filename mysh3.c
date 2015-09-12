/*
 * mysh3.sh
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
#define OUTPUT_BUF_SIZE 10000

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

void str_to_array(char *str, const char *delimiter, char **array, int *n_commands){
	char *token;
	int i = 0;

	token = strtok(str, delimiter);
	array[i] = token;

	while((token = strtok(NULL, delimiter)) != NULL){
		i = i + 1;
		array[i] = token;
	}

	array[i+1] = NULL;
	*n_commands = i;
}

void execute(char **commands, int n_commands){
	pid_t pid;
	int status;
	int fd[2];
	char *command;
	char output_buffer[OUTPUT_BUF_SIZE];

	int pipe_in_command = n_commands > 1;

	if(pipe_in_command && pipe(fd) < 0){
		perror("pipe");
		exit(-1);
	}

	pid = fork();

	if(pid < 0){
		perror("fork");
		exit(-1);
	}
	else if(pid == 0){	/*child*/
		if(pipe_in_command){
			close(fd[0]);	/* close reader */
		}

		trim(commands[0], command);

		execvp(command, parameters);
		perror("exec"); /*will not be reached in case of a success, as exec replaces the complete program*/
		exit(-1);
	}
	else{		/* parent */
		if(pipe_in_command){
			close(fd[1]);	/* close writer */

			if(read(fd[0], output_buffer, OUTPUT_BUF_SIZE) < 0){
				perror("pipe read");
			}

			wait(&status);

			if(status < 0){
				perror("wait");
				exit(-1);
			}

			/*aanvullen*/
		}

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
	char *commands[BUF_SIZE/2+2]; //the parameters could consist of single characters seperated by a space. Add 1 for rounding and 1 for the null character to the array 
	const char delimiter[2] = "|";
	int n_commands;

	printf("\nCommand: ");

	while(fgets(input, sizeof(input), stdin)){
		trim(input, command);	/*necessary here too because of exit command */
		str_to_array(command, delimiter, commands, &n_commands);

		if(strcmp("exit", command) == 0){
			printf("exiting...\n");
			exit(0);
		}

		execute(commands, n_commands);
		printf("\nCommand: ");		
	}

    return 0;
}

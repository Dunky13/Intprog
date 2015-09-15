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

void execute(char **commands, int n_commands, int change_stdin);

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
	*n_commands = i+1;
}

void child_code(int *pipe_child_parent, int *pipe_parent_child, char *command, int use_pipe, int redir_stdin){
	char *parameters[BUF_SIZE/2+2]; //the parameters could consist of single characters seperated by a space. Add 1 for rounding and 1 for the null character to the array 
	const char delimiter[2] = " ";
	int n_parameters; /* only defined to pass as it is required in the line below, is not used further*/
	str_to_array(command, delimiter, parameters, &n_parameters);

	if(redir_stdin){
		close(0);
		dup(pipe_child_parent[0]); /* redirect stdin to read output of previous command (pipe) */
	}

	if(use_pipe){
		close(pipe_child_parent[0]); /* close reader */
		close(1);					/* close standard output */
		dup(pipe_child_parent[1]); /* redirect standard output to pipe */
	}

	execvp(command, parameters);

	perror("exec"); /*will not be reached in case of a success, as exec replaces the complete program*/
	exit(-1);
}

void parent_code(int *pipe_child_parent, char **commands, int n_commands, char output_buffer[OUTPUT_BUF_SIZE]){
	int status;

	if(n_commands > 1){	/* use pipe*/
		if(read(pipe_child_parent[0], output_buffer, OUTPUT_BUF_SIZE) < 0){
			perror("pipe read");
		}

		wait(&status);

		if(status < 0){
			perror("wait");
			exit(-1);
		}

		execute(commands++, n_commands - 1, 1);
	}

	wait(&status);

	if(status < 0){
		perror("wait");
		exit(-1);
	}
}

void execute(char **commands, int n_commands, int change_stdin){
	pid_t pid;
	int pipe_child_parent[2];
	int pipe_parent_child[2];
	char command[BUF_SIZE];
	char output_buffer[OUTPUT_BUF_SIZE];

	trim(commands[0], command);

	if(strcmp("exit", command) == 0){
		printf("exiting...\n");
		exit(0);
	}
	else if(strcmp("", command) != 0){
		int use_pipe = n_commands > 1 || change_stdin;

		/*open pipe if needed*/
		if(use_pipe && pipe(pipe_child_parent) < 0){
			perror("pipe");
			exit(-1);
		}

		/*if(change_stdin){
			if(pipe(pipe_parent_child) < 0){
				perror("pipe");
				exit(-1);
			}
		}*/

		pid = fork();

		if(pid < 0){
			perror("fork");
			exit(-1);
		}
		else if(pid == 0){	/*child*/
			child_code(pipe_child_parent, pipe_parent_child, command, use_pipe, change_stdin);
		}
		else{				/* parent */
			//if(change_stdin){
				//close(pipe_parent_child[0]); /* close reader */
				//write(pipe_parent_child[1], output_buffer, OUTPUT_BUF_SIZE);
				//close(pipe_parent_child[1]); /* close writer */
			//}*/

			parent_code(pipe_child_parent, commands, n_commands, output_buffer);
		}
	}
}
 
main(int argc, char **argv){    
    if(argc != 1){
    	fprintf(stderr, "Usage: command\n%d parameters given", argc);
    	exit(-1);
    }

	char input[BUF_SIZE];
	char *commands[BUF_SIZE/2+2]; //the commands could consist of single characters seperated by a |. Add 1 for rounding and 1 for the null character to the array 
	const char delimiter[2] = "|";
	int n_commands;

	printf("\nCommand: ");

	while(fgets(input, sizeof(input), stdin)){
		str_to_array(input, delimiter, commands, &n_commands); /* split entered commands by | symbol */
		execute(commands, n_commands, 0);
		printf("\nCommand: ");		
	}

    return 0;
}
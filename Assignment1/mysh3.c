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

void execute(int *fd, char **commands, int index_command);

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

/* splits a sstring on the delimiter and puts each resulting element in the array */
void str_to_array(char *str, const char *delimiter, char **array){
	char *token;
	int i = 0;

	token = strtok(str, delimiter);
	array[i] = token;

	while((token = strtok(NULL, delimiter)) != NULL){
		i = i + 1;
		array[i] = token;
	}

	array[i+1] = NULL;
}

/* executed at child */
void child_code(int *fd, char *command, int change_stdout, int change_stdin){
	char *parameters[BUF_SIZE/2+2]; //the parameters could consist of single characters seperated by a space. Add 1 for rounding and 1 for the null character to the array 
	const char delimiter[2] = " ";
	str_to_array(command, delimiter, parameters);

	if(change_stdin){
		close(fd[1]); 	/* close writer */

		if(dup2(fd[0], 0) < 0){		/* redirect stdin to read output of previous command (pipe) */
			perror("dup2 at redirecting pipe to stdin");
		}
	}
	else if(change_stdout){
		if(dup2(fd[1], 1) < 0){		/* redirect stout to pipe */
			perror("dup2 at redirecting stdout to pipe");
		}

		close(fd[0]);	/* close reader */
	}

	execvp(parameters[0], parameters);

	perror("exec"); /*will not be reached in case of a success, as exec replaces the complete program*/
	exit(-1);
}

/* executed at parent */
void parent_code(int *fd, char **commands, int index_command){
	int status;
	pid_t pid;
	char command[BUF_SIZE];

	wait(&status);

	if(status < 0){
		perror("wait");
		exit(-1);
	}

	if(commands[index_command+1] != NULL){
		pid = fork();

		if(pid < 0){
			perror("fork");
			exit(-1);
		}
		else if(pid == 0){	/*child*/
			trim(commands[1], command);

			child_code(fd, command, 0, 1);
		}
		else{				/* parent */
			close(fd[0]);
			close(fd[1]);

			wait(&status);

			if(status < 0){
				perror("wait");
				exit(-1);
			}
		}
	}			

	/*if(commands[index_command+1] != NULL){
		execute(fd, commands, index_command+1);
	}*/
}

void execute(int *fd, char **commands, int index_command){
	pid_t pid;
	char command[BUF_SIZE];
	int use_pipe;

	trim(commands[index_command], command);

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
			use_pipe = commands[index_command+1] != NULL;			
			child_code(fd, command, use_pipe, index_command > 0);
		}
		else{				/* parent */
			parent_code(fd, commands, index_command);			
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
	int fd[2];

	printf("\nCommand: ");

	while(fgets(input, sizeof(input), stdin)){
		if(pipe(fd) < 0){
			perror("pipe");
			exit(-1);
		}

		str_to_array(input, delimiter, commands); /* split entered commands by | symbol */
		execute(fd, commands, 0);
		printf("\nCommand: ");		
	}

    return 0;
}
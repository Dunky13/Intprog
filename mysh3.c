int fd[2];

if(pipe(fd) < 0){
	perror("pipe");
	exit(-1);
}


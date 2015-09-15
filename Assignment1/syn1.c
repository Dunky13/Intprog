#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/sem.h>
void display(char *str) {
    char *tmp;
    for (tmp=str;*tmp;tmp++) {
        write(1,tmp,1);
        usleep(100);
    }
}
int main() {
    int i;
	
	struct sembuf up 	= {0,  1, 0};
	struct sembuf down	= {0, -1, 0};
	int semaphore = semget(IPC_PRIVATE, 1, 0600);
	
	if(semaphore < 0) perror("Could not initialize semaphore");
	
	if(semop(semaphore,&up, 1) < 0) perror("Locking initial semaphore failed");
    if (fork()) 
	{
        for (i=0;i<10;i++)
		{ 
			if(semop(semaphore,&down, 1) >= 0)
			{ /* SEMOP Was succesful*/
				display("Hello world\n");
				if(semop(semaphore,&up, 1) < 0)
					perror("Unlocking semaphore failed");
			} else 
				perror("Locking semaphore failed");
		}
        wait(NULL);
		if(semctl(semaphore, 0, IPC_RMID) < 0)
			perror("Locking removing semaphore");
    }
    else
	{
        for (i=0;i<10;i++)
		{
			if(semop(semaphore,&down, 1) >= 0)
			{ /* SEMOP Was succesful*/
				display("Bonjour monde\n");
				if(semop(semaphore,&up, 1) < 0)
					perror("Unlocking semaphore failed");
			} else 
				perror("Locking semaphore failed");
		} 
    }
    return 0;
}
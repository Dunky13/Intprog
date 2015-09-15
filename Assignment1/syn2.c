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
    struct sembuf parent_up 	= {0,  1, 0};
	struct sembuf parent_down	= {0, -1, 0};
	struct sembuf child_up 		= {1,  1, 0};
	struct sembuf child_down	= {1, -1, 0};
	int semaphore = semget(IPC_PRIVATE, 2, 0600);
	
	if(semaphore < 0) perror("Could not initialize semaphore");
	
	if(semop(semaphore,&parent_up, 1) < 0) perror("Locking initial semaphore failed");
    if (fork()) 
	{
        for (i=0;i<10;i++)
		{ 
			if(semop(semaphore,&parent_down, 1) >= 0)
			{ /* SEMOP Was succesful*/
				display("ab");
				if(semop(semaphore,&child_up, 1) < 0)
					perror("Unlocking semaphore failed");
			} else 
				perror("Locking semaphore failed");
		}
        wait(NULL);
		if(semctl(semaphore, 1, IPC_RMID) < 0)
			perror("Removing parent semaphore");
		
    }
    else 
	{
        for (i=0;i<10;i++)
		{
			if(semop(semaphore,&child_down, 1) >= 0)
			{ /* SEMOP Was succesful*/
				display("cd\n");
				if(semop(semaphore,&parent_up, 1) < 0)
					perror("Unlocking semaphore failed");
			} else 
				perror("Locking semaphore failed");
		}		
    }
    return 0;
}
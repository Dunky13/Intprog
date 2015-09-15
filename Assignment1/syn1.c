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
  struct sembuf up = {0,1,0};
  struct sembuf down = {0,-1,0};
  int i, my_sem;

  my_sem = semget(IPC_PRIVATE, 1, 0600); /*create semaphore*/
  if(my_sem<0){
    perror("Error getting semaphore:");
  }
  
  if(semop(my_sem, &up, 1)){perror("error changing semaphore:");}; /*set to 1*/

  if (fork()) {
    for (i=0;i<10;i++) {
      if(semop(my_sem, &down, 1)){perror("error changing semaphore:");}; 
      display("Hello world\n");
      if(semop(my_sem, &up, 1)){perror("error changing semaphore:");}; 
    }
    wait(NULL);
    if(semctl(my_sem,0,IPC_RMID)){perror("error destroying semaphore:");}  /*destroy the semaphore*/
  }
  else {
    for (i=0;i<10;i++) {
      if(semop(my_sem, &down, 1)){perror("error changing semaphore:");};
      display("Bonjour monde\n");
      if(semop(my_sem, &up, 1)){perror("error changing semaphore:");}; 
    }
  }


  return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>


pthread_mutex_t cond_mutex1; /*The two mutex necessary for the conditional variables*/
pthread_mutex_t cond_mutex2;
pthread_cond_t cond_var1; /*And the two conditional variables*/
pthread_cond_t cond_var2;
int predicate1, predicate2; /*Two booleans used by the conditional variables */


/*Function to wait on a certain condition*/
void wait_condvar(pthread_mutex_t * cond_mutex, pthread_cond_t * cond_var, int *predicate){
    if(pthread_mutex_lock(cond_mutex)) {perror("Error with locking mutex:");};
    while(*predicate==0){
      pthread_cond_wait(cond_var,cond_mutex);
    }
    *predicate = 0;
    if(pthread_mutex_unlock(cond_mutex)) {perror("Error with unlocking mutex:");};
}

/*function to signal the conditional variable has changed*/
void signal_condvar(pthread_mutex_t * cond_mutex, pthread_cond_t * cond_var, int *predicate){
    if(pthread_mutex_lock(cond_mutex)) {perror("Error with locking mutex:");};
    *predicate = 1;
    pthread_cond_signal(cond_var);
    if(pthread_mutex_unlock(cond_mutex)) {perror("Error with unlocking mutex:");};
}

void display(char *str) {
  char *tmp;
  
  for (tmp=str;*tmp;tmp++) {
    write(1,tmp,1);
    usleep(100);
  }
}

void *thread_function(void *param){
  int i;
  char *string = (char *) param;

  for(i=0;i<10;i++){
    wait_condvar(&cond_mutex1, &cond_var1, &predicate1);
    display(string);
    signal_condvar(&cond_mutex2, &cond_var2, &predicate2);

  }
  return NULL;
}



int main() {
  int i;
  pthread_t id;
  pthread_attr_t attr;
  char *str1, *str2;
  
  str1 = "ab";
  str2 = "cd\n";

  /*initializing the conditional variables*/
  predicate1 = 1;
  predicate2 = 0;
  if(pthread_mutex_init(&cond_mutex1, NULL)){ perror("Error initializing mutex 1:");}
  if(pthread_mutex_init(&cond_mutex2, NULL)){ perror("Error initializing mutex 2:");}
  if(pthread_cond_init(&cond_var1, NULL)){ perror("Error initializing conditional variable 1:");} 
  if(pthread_cond_init(&cond_var2, NULL)){ perror("Error initializing conditional variable 1:");} 

  /*initializing the thread attributes*/
  if(pthread_attr_init(&attr)){perror("Error initializing thread attributes:");}
  
  /*launching thread*/
  if(pthread_create(&id, &attr, thread_function, (void *) str1)){perror("Error creating thread:");}

  for(i=0;i<10;i++){
    wait_condvar(&cond_mutex2, &cond_var2, &predicate2);
    display(str2);
    signal_condvar(&cond_mutex1, &cond_var1, &predicate1);
  }

  /*waiting for thread to finish*/
  if(pthread_join(id,NULL)){perror("Error joining thread:");}
  
  /*destroy mutexes and conditional variables*/
  if(pthread_mutex_destroy(&cond_mutex1)){perror("Error destroying mutex 1:");}
  if(pthread_mutex_destroy(&cond_mutex2)){perror("Error destroying mutex 2:");}
  if(pthread_cond_destroy(&cond_var1)){perror("Error destroying conditional variable 1:");}
  if(pthread_cond_destroy(&cond_var2)){perror("Error destroying conditional variable 2:");}

  
  return 0;
}
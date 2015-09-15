#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>

pthread_mutex_t parent_mutex;
pthread_mutex_t child_mutex;
pthread_cond_t parent_condition;
pthread_cond_t child_condition;

struct mutextCond{
	pthread_mutex_t *mutex;
	pthread_cond_t *condition;
	int *predicate;
};
struct threadArguments{
	struct mutextCond *comb1;
	struct mutextCond *comb2;
	char *string;
};

void display(char *str) {
    char *tmp;
    for (tmp=str;*tmp;tmp++) {
        write(1,tmp,1);
        usleep(100);
    }
}

void threadWait(struct mutextCond *args)
{
    if(pthread_mutex_lock(args->mutex))
		perror("Error with locking mutex:");
    while(args->predicate == 0)
      pthread_cond_wait(args->condition,args->mutex);
    *(args->predicate) = 0;
    if(pthread_mutex_unlock(args->mutex))
		perror("Error with unlocking mutex:");
}

void threadSignal(struct mutextCond *args)
{
    if(pthread_mutex_lock(args->mutex))
		perror("Error with locking mutex:");
    *(args->predicate) = 1;
    pthread_cond_signal(args->condition);
    if(pthread_mutex_unlock(args->mutex))
		perror("Error with unlocking mutex:");
}

void *printFunc(void *parm){
	int i;
	struct threadArguments *args = (struct threadArguments *)parm;
	for (i=0;i<10;i++)
	{ 
		threadWait(args->comb1);	
		display(args->string);
		threadSignal(args->comb2);
	}
	return 0;
}


int main() {
    pthread_t parent_id;
	pthread_t child_id;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	
	pthread_mutex_init(&parent_mutex, NULL);
	pthread_mutex_init(&child_mutex, NULL);
	
	pthread_cond_init(&parent_condition, NULL);
	pthread_cond_init(&child_condition, NULL);
	
	struct mutextCond parentCombo;
	parentCombo.mutex = &parent_mutex;
	parentCombo.condition = &parent_condition;
	*(parentCombo.predicate) = 1;
	
	struct mutextCond childCombo;
	childCombo.mutex = &child_mutex;
	childCombo.condition = &child_condition;
	*(childCombo.predicate) = 0;
	 
	struct threadArguments parentArgs;
	parentArgs.comb1 = &parentCombo;
	parentArgs.comb2 = &childCombo;
	parentArgs.string = "ab";
	
	struct threadArguments childArgs;
	childArgs.comb1 = &childCombo;
	childArgs.comb2 = &parentCombo;
	childArgs.string = "cd\n";
	
	
	
	pthread_create(&parent_id, &attr, printFunc, (void *)&parentArgs);
	pthread_create(&child_id, &attr, printFunc, (void *)&childArgs);
		
	pthread_join(parent_id, NULL);
	pthread_join(child_id, NULL);
	
	pthread_mutex_destroy(&parent_mutex);
	pthread_mutex_destroy(&child_mutex);
	
    return 0;
}
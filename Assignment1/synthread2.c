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

void display(char *str) {
    char *tmp;
    for (tmp=str;*tmp;tmp++) {
        write(1,tmp,1);
        usleep(100);
    }
}

void threadedWait(){
	
}

void *parent(){
	int i;
	for (i=0;i<10;i++)
	{ 
		pthread_mutex_lock(&parent_mutex);
		pthread_cond_wait(&parent_condition,&parent_mutex);
		display("ab");
		pthread_cond_signal(&child_condition);
		pthread_mutex_unlock(&parent_mutex);

	}
	return 0;
}

void *child(){
	int i;
	for (i=0;i<10;i++)
	{
		pthread_mutex_lock(&child_mutex);
		pthread_cond_wait(&child_condition,&child_mutex);
		display("cd\n");
		pthread_cond_signal(&parent_condition);
		pthread_mutex_unlock(&child_mutex);

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


	pthread_mutex_lock(&parent_mutex);
	pthread_cond_signal(&parent_condition);
	pthread_mutex_unlock(&parent_mutex);
	
	pthread_create(&parent_id, &attr, parent, NULL);
	pthread_create(&child_id, &attr, child, NULL);
	
	pthread_join(parent_id, NULL);
	pthread_join(child_id, NULL);
	
	pthread_mutex_destroy(&parent_mutex);
	pthread_mutex_destroy(&child_mutex);
	
    return 0;
}
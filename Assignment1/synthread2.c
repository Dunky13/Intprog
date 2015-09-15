#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t parent_condition;
pthread_cond_t child_condition;

void display(char *str) {
    char *tmp;
    for (tmp=str;*tmp;tmp++) {
        write(1,tmp,1);
        usleep(100);
    }
}

void *parent(){
	int i;
	for (i=0;i<10;i++)
	{ 
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&parent_condition,&mutex);
		display("ab");
		pthread_cond_signal(&child_condition);
		pthread_mutex_unlock(&mutex);

	}
	return 0;
}

void *child(){
	int i;
	for (i=0;i<10;i++)
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&child_condition,&mutex);
		display("cd\n");
		pthread_cond_signal(&parent_condition);
		pthread_mutex_unlock(&mutex);

	}
	return 0;
}

int main() {
    pthread_t parent_id;
	pthread_t child_id;
	pthread_attr_t attr;
	pthread_mutexattr_t mut_attr;

	pthread_attr_init(&attr);
	pthread_mutexattr_init(&mut_attr);
	pthread_mutex_init(&mutex, &mut_attr);
	
	pthread_cond_init(&parent_condition, NULL);
	pthread_cond_init(&child_condition, NULL);


	pthread_create(&parent_id, &attr, parent, NULL);
	pthread_create(&child_id, &attr, child, NULL);
	
	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&parent_condition);
	pthread_mutex_unlock(&mutex);
	
	pthread_join(parent_id, NULL);
	pthread_join(child_id, NULL);
	pthread_mutex_destroy(&mutex);
	
    return 0;
}
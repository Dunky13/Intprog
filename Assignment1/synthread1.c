#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <pthread.h>

pthread_mutex_t mutex;

void display(char *str) {
    char *tmp;
    for (tmp=str;*tmp;tmp++) {
        write(1,tmp,1);
        usleep(100);
    }
}

void parent(){
	int i;
	for (i=0;i<10;i++)
	{ 
		pthread_mutex_lock(&mutex);
		display("Hello world\n");
		pthread_mutex_unlock(&mutex);
	}
}

void *child(){
	int i;
	for (i=0;i<10;i++)
	{
		pthread_mutex_lock(&mutex);
		display("Bonjour monde\n");
		pthread_mutex_unlock(&mutex);
	}
	return 0;
}

int main() {
    pthread_t id;
	pthread_attr_t attr;
	pthread_mutexattr_t mut_attr;

	pthread_attr_init(&attr);
	pthread_mutexattr_init(&mut_attr);
	pthread_mutex_init(&mutex, &mut_attr);

	pthread_create(&id, &attr, child, NULL);
	parent();
	
	pthread_join(id, NULL);
	pthread_mutex_destroy(&mutex);
	
	
    return 0;
}
#include <pthread.h>
#include <stdio.h>

int thread1_done = 0; /* the condition variable */
pthread_cond_t convar;
pthread_mutex_t mutex;

void* printHello(){
	printf("hello ");

	pthread_mutex_lock(&mutex);
	thread1_done = 1;
	pthread_cond_signal(&convar);
	pthread_mutex_unlock(&mutex);
}

void* printWorld(){
	pthread_mutex_lock(&mutex);
	
	while(thread1_done==0){	
		pthread_cond_wait(&convar, &mutex);
	}

	printf("world\n");

	pthread_mutex_unlock(&mutex);
}

int main(){
	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, printHello, NULL);
	pthread_create(&thread2, NULL, printWorld, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}


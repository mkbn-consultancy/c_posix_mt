#include <pthread.h>
#include <stdio.h>

int thread1_done = 0; /* the condition variable */
pthread_cond_t convar;
pthread_mutex_t mutex;

void* printHello(void* p){
	printf("[hello tries to lock]");
	pthread_mutex_lock(&mutex);
	printf("[hello locked]");

	printf("hello ");

	thread1_done = 1;
	printf("[hello signals]");
	pthread_cond_signal(&convar);
	pthread_mutex_unlock(&mutex);
}

void* printWorld(void* p){
	printf("[world tries to lock]");
	pthread_mutex_lock(&mutex);
	printf("[world locked]");
	
	while(thread1_done==0){	
		printf("[world wait]");
		pthread_cond_wait(&convar, &mutex);
	}

	printf("world\n");

	pthread_mutex_unlock(&mutex);
}

int main(){
	pthread_t thread1, thread2;
	pthread_create(&thread2, NULL, printWorld, NULL);
	pthread_create(&thread1, NULL, printHello, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}


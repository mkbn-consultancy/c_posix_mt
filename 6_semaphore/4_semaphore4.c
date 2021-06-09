#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t g_semaphore;

void* print0(void* arg){
	printf("0-");
	sem_post(&g_semaphore); /*increase value of sem by 1*/
}


void* print1(void* arg){
	/*wait*/
	sem_wait(&g_semaphore); /*reduce sem in 1 if sem value > 0*/
	printf("1-");
}

int main(){
	sem_init(&g_semaphore, 0, 0); 

	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, print1, NULL);
	sleep(2);
	pthread_create(&thread2, NULL, print0, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	sem_destroy(&g_semaphore);

	printf("\n");

	return 0;
}


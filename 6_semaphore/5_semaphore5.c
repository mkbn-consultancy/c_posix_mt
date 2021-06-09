#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t g_semaphore1, g_semaphore2;

void* print0(void* arg){
	int i;
	for(i=0; i<10; ++i){
		/*signal*/
		sem_wait(&g_semaphore2); /*increase value of sem by 1*/
		printf("0-");
		sem_post(&g_semaphore1); /*increase value of sem by 1*/
		// sleep(1);
	}
}


void* print1(void* arg){
	int i;
	for(i=0; i<10; ++i){
		/*wait*/
		sem_wait(&g_semaphore1); /*reduce sem in 1 if sem value > 0*/
		printf("1-");
		sem_post(&g_semaphore2); /*reduce sem in 1 if sem value > 0*/
	}
}

int main(){
	sem_init(&g_semaphore1, 0, 0); 
	sem_init(&g_semaphore2, 0, 1); 
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, print0, NULL);
	pthread_create(&thread2, NULL, print1, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	sem_destroy(&g_semaphore1);
	sem_destroy(&g_semaphore2);

	printf("\n");

	return 0;
}


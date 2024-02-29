#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t g_semaphore;

void* printHello(void*){
	printf("Hello ");
	sem_post(&g_semaphore); /*increase value of sem by 1*/
	return NULL;
}


void* printWorld(void*){
	/*wait*/
	sem_wait(&g_semaphore); /*reduce sem in 1 if sem value > 0*/
	printf("World!\n");
	return NULL;
}

int main(){
	sem_init(&g_semaphore, 0, 0); 

	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, printWorld, NULL);
	sleep(2);
	pthread_create(&thread2, NULL, printHello, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	sem_destroy(&g_semaphore);

	printf("\n");

	return 0;
}


#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t g_semaphore1, g_semaphore2;

void* printHello(void*){

	return NULL;
}


void* printWorld(void*){

	return NULL;
}

int main(){
    /*create the necessary semaphores and init them*/

    /*creating 2 threads*/
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, printHello, NULL);
	pthread_create(&thread2, NULL, printWorld, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	sem_destroy(&g_semaphore1);
	sem_destroy(&g_semaphore2);

	printf("\n");

	return 0;
}
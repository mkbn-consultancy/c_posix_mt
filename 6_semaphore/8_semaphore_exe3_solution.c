#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h> /* for sleep */

const int free_space_in_store = 5; /* the shared resource */
const int NUM_THREADS = 10;

sem_t g_semaphore_enter;

void* store_handling(void*){
	sem_wait(&g_semaphore_enter); /*decrease value of sem by 1*/
    printf("---> customer enters the store...\n");
    sleep(1);
    sem_post(&g_semaphore_enter);
    printf("<--- customer exits the store... \n");
    return NULL;
}

int main(){

	sem_init(&g_semaphore_enter, 0, free_space_in_store); 

	pthread_t threads[10];  //declaring 10 threads
    int i;
	for(i=0; i<NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, store_handling, NULL);
	}
    for(i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i],NULL);
    }

    sem_destroy(&g_semaphore_enter);   
	return 0;
}

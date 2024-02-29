#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h> /* for sleep */

const int FREE_SPACE_IN_STORE = 5; 
const int NUM_THREADS = 10;

sem_t g_semaphore_enter;

void* store_handling(void*){
    /*add necessary code*/

    printf("---> customer enters the store...\n");

    sleep(1);

    printf("<--- customer exits the store... \n");
    return NULL;
}

int main(){

	sem_init(&g_semaphore_enter, 0, FREE_SPACE_IN_STORE); 

	pthread_t threads[NUM_THREADS];  //declaring 10 threads
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
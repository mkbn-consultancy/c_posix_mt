#include <pthread.h>
#include <stdio.h>

const int MAX_CUSTOMERS_IN_STORE = 5;
const int NUM_THREADS = 20;

int free_space_in_store = 5; /* the shared resource */

pthread_cond_t convar;
pthread_mutex_t mutex;

void* enter_store(void*){
	pthread_mutex_lock(&mutex);
	
	while(free_space_in_store==0){
        /* store is full, you cannot go it... */
		pthread_cond_wait(&convar, &mutex);
	}
    printf("---> customer enters the store...\n");
    free_space_in_store--;
	pthread_mutex_unlock(&mutex);

    return NULL;
}

void* exit_store(void*){
	pthread_mutex_lock(&mutex);
	
    while(free_space_in_store==5){
        /* store is empty, you cannot go out... */
 		pthread_cond_wait(&convar, &mutex);       
    }
    free_space_in_store++;
    printf("<--- customer exits the store...\n");
	pthread_mutex_unlock(&mutex);

	pthread_cond_signal(&convar);


    return NULL;
}

int main(){
	pthread_t threads[NUM_THREADS];  //declaring 10 threads
    int i;
    for(i=0; i<NUM_THREADS/2; i++){
        pthread_create(&threads[i], NULL, enter_store, NULL);
    }
    for(; i<NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, exit_store, NULL);
    }

    for(int i=0; i<NUM_THREADS; i++){
    	pthread_join(threads[i], NULL);
    }

	return 0;
}

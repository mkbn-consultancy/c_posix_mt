#include <pthread.h>
#include <stdio.h>

/* create the shared mutex and conditional variable */


/*what is the shared resource that we need to protect?
  define it here.
*/

const int MAX_CUSTOMERS_IN_STORE = 5;
const int NUM_THREADS = 20;

void* enter_store(void*){

    printf("---> customer enters the store...\n");

    return NULL;
}

void* exit_store(void*){

    printf("<--- customer exits the store...\n");


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

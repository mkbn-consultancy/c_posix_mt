#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

const int LOOP_SIZE = 6;

/**** solution 1: without any synchronization tools ****/

void* printNums_no_RC(void* x){
    pthread_exit(0);
}

void* printSep_no_RC(void* x){
    pthread_exit(0);
}

/**** solution 2: with mutex and conditional variable ****/

void* printNums_mutex(void* x){
    pthread_exit(0);
}

void* printSep_mutex(void* x){
    pthread_exit(0);
}

int main(){
    /*create 2 threads that run the above function,
    one starts from 1 and the other prints ',' between them */

    printf("\n*** Test without synchronization tools ***\n");

    printf("\n*** Test with mutex and conditional variable ***\n");

    return 0;
}

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

const int LOOP_SIZE = 6;

enum{NUM_TURN, SEP_TURN};
int turn = NUM_TURN;

/**** solution 1: without any synchronization tools ****/

void* printNums_no_RC(void* x){
    int num = (*(int*)x);
    /*print next 6 numbers starting at x,
    increasing it by 2 each iteration*/
    int i;
    for(i=0; i<LOOP_SIZE; ++i){
        while(turn==SEP_TURN){
            // usleep(6);
        }
        printf("%d",num);
        num++;
        turn=SEP_TURN;
    }
    pthread_exit(0);
}

void* printSep_no_RC(void* x){
    char sep = (*(char*)x);
    /*print next 6 numbers starting at x,
    increasing it by 2 each iteration*/
    int i;
    for(i=0; i<LOOP_SIZE; ++i){
        while(turn==NUM_TURN){
            // usleep(6);
        }
        printf("%c",sep);
        turn=NUM_TURN;
    }
    pthread_exit(0);
}

/**** solution 2: with mutex and conditional variable ****/

pthread_mutex_t mutex;
pthread_cond_t printNumsCond, printSepCond;

void* printNums_mutex(void* x){
    int num = (*(int*)x);
    /*print next 6 numbers starting at x,
    increasing it by 2 each iteration*/
    int i;
    for(i=0; i<LOOP_SIZE; ++i){
        pthread_mutex_lock(&mutex);
        while(turn==SEP_TURN){
            pthread_cond_wait(&printNumsCond, &mutex);
        }
        printf("%d",num);
        num++;
        turn=SEP_TURN;
        pthread_cond_signal(&printSepCond);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void* printSep_mutex(void* x){
   char sep = (*(char*)x);
    /*print next 6 numbers starting at x,
    increasing it by 2 each iteration*/
    int i;
    for(i=0; i<LOOP_SIZE; ++i){
        while(turn==NUM_TURN){
            pthread_cond_wait(&printSepCond, &mutex);
        }
        printf("%c",sep);
        turn=NUM_TURN;
        pthread_cond_signal(&printNumsCond);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

int main(){
	pthread_t thread1, thread2;
    /*create 2 threads that run the above function,
    one starts from 1 and the other starts from 2 */
    int start = 1;
    char sep = ',';

    printf("\n*** Test without synchronization tools ***\n");
	pthread_create(&thread1, NULL, printNums_no_RC, &start);
	pthread_create(&thread2, NULL, printSep_no_RC, &sep);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

    printf("\n*** Test with mutex and conditional variable ***\n");
	pthread_create(&thread1, NULL, printNums_mutex, &start);
	pthread_create(&thread2, NULL, printSep_mutex, &sep);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

    turn = NUM_TURN;
        
    return 0;
}

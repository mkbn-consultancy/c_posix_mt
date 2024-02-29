#include <pthread.h>
#include <stdio.h>
#include <unistd.h> /* for sleep */

void* printNumbers(void* param); /* thread start function */

int main(){
	pthread_t thread1, thread2;
	int thr1 = 1;
	int thr2 = 2;
	
	/* start the trheads */
	pthread_create(&thread1, NULL, printNumbers, (void*)&thr1);
	pthread_create(&thread2, NULL, printNumbers, (void*)&thr2);

	/* wait for threads to finish */
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	printf("\n");

	return 0;
}

void* printNumbers(void* param){
	int* pi = (int*)param;
	int i = *pi;
	int limit = i*10;
	while(i<limit){
		printf("%d - ", i);
		i+=2;
		sleep(1); /* just for forcing the OS to switch this thread */
	}
	return param;
}
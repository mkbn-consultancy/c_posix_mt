#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int g_counter = 0;

void* increment(){
	int i;
	for(i=0; i<100000; ++i){
		g_counter++;
	}
}

int main(){
	pthread_t thread1, thread2, thread3, thread4;

	pthread_create(&thread1, NULL, increment, NULL);
	pthread_create(&thread2, NULL, increment, NULL);
	pthread_create(&thread3, NULL, increment, NULL);
	pthread_create(&thread4, NULL, increment, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);

	printf("Counter=%d\n",g_counter);

	return 0;
}


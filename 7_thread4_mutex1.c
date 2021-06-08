#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int g_x = 5; 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* function(){

	pthread_mutex_lock(&lock);
	if(g_x < 6){
		++g_x;
		printf("x incremented\n");
	}
	else{
		printf("x is out of range\n");
	}
	pthread_mutex_unlock(&lock);
}

int main(){
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, function, NULL);
	pthread_create(&thread2, NULL, function, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}



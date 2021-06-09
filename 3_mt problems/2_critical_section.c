#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int g_x = 5; 

void* function(){
	if(g_x < 6){
//		sleep(5);
		++g_x;
		printf("x incremented\n");
	}
	else{
		printf("x is out of range\n");
	}
}

int main(){
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, function, NULL);
	pthread_create(&thread2, NULL, function, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("g_x=%d\n",g_x);

	return 0;
}



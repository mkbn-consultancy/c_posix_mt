#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
/*
In this example, 2 threads are being created, one 2 seconds after the first one.
But the first one will sleep for 4 seconds after acquiring the lock.
Thus the second thread will not enter immediately after it is called, but it will enter 4-2=2 seconds after it is called.
*/

sem_t g_semaphore;

void* thread(void*){
	int val;

	/*wait*/
	sem_getvalue(&g_semaphore, &val);
	printf("Before wait: value of semaphore = %d\n", val);
	sem_wait(&g_semaphore);
	sem_getvalue(&g_semaphore, &val);
	printf("After wait: value of semaphore = %d\n", val);

	sleep(4);

	/*signal*/
	sem_post(&g_semaphore);
	sem_getvalue(&g_semaphore, &val);
	printf("After post: value of semaphore = %d\n\n", val);

	return NULL;
}

int main(){
	sem_init(&g_semaphore, 0, 2); /*0 = semaphore is local.
					2 = non-binary semaphore.*/
	pthread_t thread1, thread2, thread3;

	pthread_create(&thread1, NULL, thread, NULL);
	pthread_create(&thread2, NULL, thread, NULL);
	pthread_create(&thread3, NULL, thread, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	sem_destroy(&g_semaphore);

	return 0;
}


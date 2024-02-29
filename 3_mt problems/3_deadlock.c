#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t res1, res2;

void aquire_resource(pthread_mutex_t* resource){
	pthread_mutex_lock(resource);
}
void release_resource(pthread_mutex_t* resource){
    pthread_mutex_unlock(resource);
}

void *some_function_1(){
	aquire_resource(&res1);

	printf("Job started in some_function_1..\n");
	sleep(2);

	printf("Trying to get resourc 2\n");
	aquire_resource(&res2);
	printf("Aquired resourc 2\n");
	
	release_resource(&res2);

	printf("Job finished in some_function_1..\n");

	release_resource(&res1);

	pthread_exit(NULL);
}

void *some_function_2(){
	aquire_resource(&res2);

	printf("Job started in some_function_2..\n");
	sleep(2);

	printf("Trying to get resourc 1\n");
	aquire_resource(&res1);
	printf("Aquired resourc 1\n");

	release_resource(&res1);

	printf("Job finished in some_function_2..\n");

	release_resource(&res2);

	pthread_exit(NULL);
}

int main() {
	pthread_mutex_init(&res1,NULL);
	pthread_mutex_init(&res2,NULL);

	pthread_t t1,t2;

	pthread_create(&t1,NULL,some_function_1,NULL);
	pthread_create(&t2,NULL,some_function_2,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	return 0;
}

//This program executes in order, have a look


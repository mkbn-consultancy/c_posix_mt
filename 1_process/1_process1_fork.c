#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){
	pid_t pid;
	/*fork another process*/
	pid = fork();
	if(pid<0){
		fprintf(stderr, "Fork Failed\n");
		_exit(-1);
	}
	else if(pid==0){
		printf("I'm the child process\n");
		sleep(30);
		execlp("/bin/ls","ls",NULL);
		printf("Child done...\n");
	}
	else{
		printf("I'm the parent process, waiting for the child process to end...\n");
		wait(NULL);
		printf("child complete\n");
		_exit(0);
	}
}


//https://onlinegdb.com/ftJG03W4S
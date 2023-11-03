#include <stdio.h>
#include <sys/types.h>

int main(){
	pid_t pid;
	int status;
	
	pid = fork();
	
	if(pid > 0){
		printf("wait for you...\n");
		wait(&status);
		printf("status is %d\n", status);
	}

	else if(pid == 0){
		sleep(1);
		printf("bye!\n");
		exit(1);
	}

}

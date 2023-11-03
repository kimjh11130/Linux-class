#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(void){
	sigset_t set;
	int result;
	
	sigemptyset(&set);

	result = sigismember(&set, SIGINT);
	printf("SIGINT is %s a memeber \n", result? " " : "not");

	sigaddset(&set, SIGINT);
	
	result = sigismember(&set, SIGINT);
	printf("SIGINT is %s a member \n", result ? " " : "not");

	sigfillset(&set);

	result = sigismember(&set, SIGINT);
	printf("SIGCHLD is %s a member \n", result ? "" : "not");

	sigdelset(&set, SIGCHLD);

	result = sigismember(&set, SIGCHLD);
	printf("SIGCHLD is %s a member \n", result ? "" : "not");
}

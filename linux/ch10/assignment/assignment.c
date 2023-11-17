#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

void int_handle(int);

int flag = 1;

int main(){
        static struct sigaction act;
        act.sa_handler = int_handle;
        sigfillset(&(act.sa_mask));
        sigaction(SIGINT, &act, NULL);

	while(flag){
		printf("ing..\n");
		sleep(1);
	}
}

void int_handle(int signum){
	char buf[] = "exit\n";
	int fd = open("a.txt", O_CREAT | O_EXCL, 0644);
	write(fd, buf, strlen(buf));
	
	close(fd);
	flag = 0;
}

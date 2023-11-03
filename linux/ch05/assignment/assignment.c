#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(void){
	pid_t pid;
	int n, a = 0;
	scanf("%d", &n);
	pid = fork();

	if(pid > 0){
		for(int i = 1; i <= n; i++) a += i;
		printf("%d\n", a);
	} else if(pid == 0){
		a = 1;
		for(int i = 1; i <= n; i++) a *= i;
		printf("%d\n", a);
	} else {
		printf("Fork failed\n");
	}
}

#include <stdio.h>
#include <unistd.h>

int main(){
	printf("before executing ls -l\n");
	execl("/biin.ls", "ls", "-'", (char *)0);
	printf("after executing ls -l\n");
}

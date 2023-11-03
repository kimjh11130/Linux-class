#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	putenv("APPLE=BANANA");
	printf("%s\n", getenv("APPLE"));
	
	execl("ex08-10", "ex08-11", (char *)0);
}

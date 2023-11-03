#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int main() {
	printf("%s\n", getenv("MYDATA"));
	while(*environ)
		printf("%s\n", *environ++);
}


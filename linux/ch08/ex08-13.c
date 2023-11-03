#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int main() {
	while(*environ)
		printf("%s\n", *environ++);
}

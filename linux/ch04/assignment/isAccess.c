#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
	if ( access(argv[1], F_OK) == -1 ) { 
		fprintf( stderr, "없음 %s \n",argv[1]);
		exit(1);
	}
	printf("%s 있음 \n", argv[1]);
}

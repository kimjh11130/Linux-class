#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
        int fdin, count = 0;
        ssize_t nread;
        char buffer[1024];

        fdin = open("text.txt", O_RDONLY);
        while((nread = read(fdin, buffer, 1024)) > 0){
                for(int cnt = 0; cnt < nread; cnt++){
                        if(buffer[cnt] >= 'a' && buffer[cnt] <= 'z')
				buffer[cnt] = buffer[cnt] - 'a' + 'A';
			printf("%c", buffer[cnt]);
                }
        }
}

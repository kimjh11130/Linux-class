#include <stdio.h>

int sum(){
        int sum = 0;
        for(int i = 1; i <= 100; i++) sum += i;
        printf("%d\n", sum);
        return 0;
}

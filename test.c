#include <stdio.h>

int main(){
    struct test
    {
        int x;
        int y;
    };

    printf("%lu\n",sizeof(struct test));
    
}
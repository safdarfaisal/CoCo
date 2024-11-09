#include <stdio.h>

int main(){
    struct test
    {
        in t x;
        int y;
    };

    printf("%lu\n",sizeof(struct test));
    
}
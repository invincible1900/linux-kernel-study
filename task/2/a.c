#include <stdio.h>

void spin(){
    int stack = 0;
    printf("stack: %p\n", &stack);
    while(1);
}

int main(){
    printf("spin: %p, main: %p\n", spin, main);
        spin();
        return 0;
}

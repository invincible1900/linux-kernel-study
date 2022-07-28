#include <stdio.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
       #include <unistd.h>

void spin(){
    int fd = -1;
    char buff[10];
    printf("stack: %p, %p\n", &fd, &buff);
    fd = open("/sys/xx_root/xx", O_RDONLY);
    read(fd, buff, 10);
    close(fd);
    return;
}

int main(){
        unsigned long r;
    printf("spin: %p, main: %p\n", spin, main);
        spin();

        return 0;
}

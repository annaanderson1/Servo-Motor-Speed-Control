#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "serialport.h"
#include "serialio.h"

#define STDOUT_FD 1
#define STDIN_FD 0
#define SIZE 100

int main(){

    char s[SIZE];
    char speed[SIZE];
    int fd;

    fd = serial_init("/dev/ttyS0", 0);

    /*  Takes input from terminal, sends over serialport and recieves
        from serialport after 1 s*/
    while(1){

        readFromFd(s, 1);
        writeToFd(s, fd);
        usleep((1000) * 1000);
        readFromFd(s, fd);
        extractSpeed(s, speed);
        printf("Read: %s", speed);
    }

    serial_cleanup(fd);
    return 0;
}
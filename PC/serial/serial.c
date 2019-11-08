#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "serialport.h"
#include "serialio.h"

#define STDOUT_FD 1
#define STDIN_FD 0
#define SIZE 10

int main(){

    char s[SIZE];
    int fd;

    fd = serial_init("/dev/ttyS0", 0);

    /*  Takes input from terminal, sends over serialport, recieves
        from serialport and prints to terminal*/
    while(1){

        readFromFd(s, STDOUT_FD);
        writeCharToFdWithDelay(s, fd);
        readCharsFromFd(s, fd);
        writeToFd(s, STDOUT_FD);
        writeToFd("\n", STDOUT_FD);
    }

    serial_cleanup(fd);
    return 0;
}
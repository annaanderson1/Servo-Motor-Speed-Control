#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "serialport.h"

#define SIZE 100


int writeToFd(char *s, int fd){

    char buf[200];
    size_t nbytes;
    ssize_t bytes_written;

    strncpy(buf, s, sizeof(buf));

    nbytes = strnlen(buf, sizeof(buf));
    bytes_written = write(fd, buf, nbytes);

    return bytes_written;

}

int readFromFd(char *s, int fd){

    char buf[SIZE+1];
    ssize_t bytes_read;

    bytes_read = read(fd, buf, SIZE);
    
    if(bytes_read > 0){
        strncpy(s, buf, bytes_read);
        strcpy(s + bytes_read,"\0");
        //printf("Read: %s", s);
    }
    return bytes_read;

}
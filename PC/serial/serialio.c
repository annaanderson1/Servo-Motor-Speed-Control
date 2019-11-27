#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "serialport.h"

#define SIZE 100
#define F_CPU 1000000UL

int writeToFd(char *s, int fd){

    char buf[200];
    size_t nbytes;
    ssize_t bytes_written;

    strncpy(buf, s, sizeof(buf));

    nbytes = strnlen(buf, sizeof(buf));
    bytes_written = write(fd, buf, nbytes);

    return bytes_written;

}

/* Writes 5 bytes to fd with 100 ms delay */
int writeCharToFdWithDelay(char *s, int fd){
    
    int i = 0;
    int j = 0;
    char buf[5];
    int s_len = strlen(s);

    strncpy(buf, s, s_len);

    // Pads input with trailing #
    while( (s_len + i) <= 5){
        int pos = s_len + i - 1;
        strcpy(buf + pos, " ");
        i++;
    }
    
    for(j = 0; j < 5; j++){
        write(fd, &buf[j], 1);
        usleep((1000) * 5);
    }
    strcpy(s, buf);

    return 1;
}

/* Reads available string from fd and copies to s*/ 
int readFromFd(char *s, int fd){

    char buf[SIZE + 1];
    ssize_t bytes_read = read(fd, buf, SIZE);

    if (bytes_read > 0) {
        strncpy(s, buf, bytes_read);
        strcpy(s + bytes_read, "\0");

    }

    return bytes_read;
}

/* Reads five chars from fd and copies to s */ 
int readCharsFromFd(char *s, int fd){

    int bytes_read = 0;
    int res;
    char buf[5];

    while(bytes_read < 5){
        char char_read;
        res = read(fd, &char_read, 1);
        if (res > 0){
            strncpy(buf+bytes_read, &char_read, 1);
            bytes_read++;
        }
    }
    
    strcpy(s, buf);
    return bytes_read;
}

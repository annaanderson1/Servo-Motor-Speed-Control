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

void readFromFd(char *s, int fd){

    int run = 1;
    //ssize_t bytes_read;
    while(run){
        char buf[SIZE + 1];
        ssize_t bytes_read = read(fd, buf, SIZE);

        if (bytes_read > 0) {
            strncpy(s, buf, bytes_read);
            strcpy(s + bytes_read, "\0");
            //printf("Read: %s", s);
        }
        else{
            run = 0;
        }
    }

    //return bytes_read;

}

void readFromFdMod(char *s, int fd){
    int run = 1;
    char buf[SIZE + 1];
    size_t pos = 0;

    while(run && pos < SIZE){
        ssize_t bytes_read = read(fd, buf+pos, SIZE-pos);
        if(res > 0){
            pos += res;
        }
        else{
            run = 0;
        }
    }
    buf[pos] = '\0';
    //printf("Buffer after read: %s\n", c);
    strpcy(s, buf);
}

void extractSpeed(char *s, char *speed){
    int front = 0;
    int rear = 0;
    bool extracted = false;
    char buf[SIZE];
    int pos = 0;

    while(s[pos] != '\0' && !extracted){

        if(s[pos] == '{'){
            front = pos;
        }
        else if(s[pos] == '}'){
            rear = pos;
        }
        if(s[front] == '{' && s[rear] == '}'){
            extracted = true;
            strncpy(buf, s[front], rear - front);
        }
    }
    strcpy(speed, buf);
}
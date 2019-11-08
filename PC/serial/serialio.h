#ifndef INPUT_H
#define INPUT_H

int writeToFd(char *s, int fd);
int writeCharToFdWithDelay(char *s, int fd);
int readFromFd(char *s, int fd);
int readCharsFromFd(char *s, int fd);

#endif
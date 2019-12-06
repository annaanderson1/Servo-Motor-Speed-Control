#ifndef INPUT_H
#define INPUT_H

/*
 * Writes the string s to fd, with maximum size of SIZE_COMMAND.
 * Returns number of bytes written.
*/
int write_to_fd(char *s, int fd);

/*
 * Writes 5 bytes to fd with 5 ms delay between them.
 * Pads with trailing " " if *s has < 5 elements.
 * Returns 1 if successful.
*/
int transmit_message(char *s, int fd);

/*
 * Reads available string from fd and copies to s.
 * Returns number of bytes read.
*/
int read_from_fd(char *s, int fd);

/*
 * Reads five bytes from fd and copies to s.
 * Returns number of bytes read.
*/
int recieve_message(char *s, int fd);

#endif
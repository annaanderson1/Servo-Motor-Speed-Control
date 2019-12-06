#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "serialport.h"
#include "serialio.h"

#define STDOUT_FD 1
#define SIZE 10
#define SIZE_COMMAND 300

int main(){

    char s[SIZE];
    char command_str[SIZE_COMMAND];
    int fd;

    //fd = serial_init("/dev/ttyS0", 0);

    strncpy(command_str, "\nCommands:\n0 - set rpm\n1 - get measured rpm avg\n2 - get fine tuning value\n3 - get control error\n4 - get pwm\n5 - get rpm set\n6 - get latest measured rpm\n7 - get clk_elapsed\n8 - get delta_time\n9 - reserved\n", SIZE_COMMAND);

    write_to_fd(command_str, STDOUT_FD);

    /*  Takes input from terminal, sends over serialport, recieves
        from serialport and prints to terminal*/
    while(1){

        read_from_fd(s, STDOUT_FD);
        transmit_message(s, fd);
        recieve_message(s, fd);
        write_to_fd(s, STDOUT_FD);
        write_to_fd("\n", STDOUT_FD);
    }

    serial_cleanup(fd);
    return 0;
}
#include <stdio.h>

void error(char* message, int line) {
    //(red) Line x: (white) message
    printf("\033[0;31mLine %d: \033[0m\033[0;37m%s\033[0m\n", line, message);
    exit(1);
}
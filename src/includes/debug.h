#ifndef DEBUG_H
#define DEBUG_H

#include <unistd.h>

struct g_program
{
    pid_t pid;
};

int attach_program(char *argv[]);

int handle_debug();

int debug(char *argv[]);

#endif /* !DEBUG_H */
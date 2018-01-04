#ifndef DEBUG_H
#define DEBUG_H

#include <unistd.h>

#include "list.h"

struct g_program
{
    pid_t pid;
    struct list *breakpoints;
};

void init_global(void);

int attach_program(char *argv[]);

int handle_debug();

int debug(char *argv[]);

#endif /* !DEBUG_H */
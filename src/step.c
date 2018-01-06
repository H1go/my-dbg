#include <stdio.h>
#include <sys/ptrace.h>

#include "debug.h"
#include "register.h"

struct g_program g_program;

int step(void *arg)
{
    arg = arg;
    uintptr_t next = register_read(rip);
    struct breakpoint *b = list_get(g_program.breakpoints, next);

    if (b)
        breakpoint_step(b);
    else {
        if (ptrace(PTRACE_SINGLESTEP, g_program.pid, 0, 0) < 0) {
            perror("Single step");
            return 0;
        }
    }
    return 1;
}
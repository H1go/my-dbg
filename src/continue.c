#include <stdio.h>
#include <sys/ptrace.h>

#include "debug.h"
#include "sig.h"

struct g_program g_program;

int continue_execution(void *arg)
{
    arg = arg;
    restore_context();
    if (ptrace(PTRACE_CONT, g_program.pid, 0, 0) < 0)
        return 0;

    handle_wait();
    return 1;
}

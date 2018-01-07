#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/ptrace.h>

#include "commands.h"
#include "debug.h"
#include "sig.h"

struct g_program g_program;

int attach_program(char *argv[])
{
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        perror("");
        return 0;
    }
    execvp(argv[1], argv + 1);
    exit(EXIT_FAILURE);
}

int handle_debug()
{
    handle_wait();

    while (1) {
        char *cmd = readline("dbg> ");
        add_history(cmd);
        dispatch_command(cmd);
        free(cmd);
    }
    return 1;
}

int debug(char *argv[])
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("");
        return 0;
    }
    else if (!pid)
    {
        return attach_program(argv);
    }
    else
    {
        g_program.pid = pid;
        g_program.breakpoints = list_init();
        if (!g_program.breakpoints)
            return 0;
        return handle_debug();
    }
}
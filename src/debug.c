#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <errno.h>

#include <sys/ptrace.h>
#include <sys/wait.h>

#include "commands.h"
#include "debug.h"

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
    int status;
    if (wait(&status) < 0) {
        perror("");
        return 0;
    }

    while (1) {
        char *cmd = readline("dbg> ");
        if (!strcmp(cmd, "quit")) {
            free(cmd);
            return 1;
        }
        dispatch_command(cmd);
        free(cmd);
    }
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
        return handle_debug();
    }
}
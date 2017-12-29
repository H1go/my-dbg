#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "debug.h"

int launch_debug(char *argv[])
{
    pid_t pid = fork();

    if (pid < 0)
        err(1, "Call to fork failed");
    else if (!pid)
    {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execvp(argv[1], argv + 1);
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        if (waitpid(pid, &status, 0) < 0)
            return 0;

        if (WIFEXITED(status))
            return 1;
    }
    return 0;
}
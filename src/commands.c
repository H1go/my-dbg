#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>

#include "commands.h"
#include "debug.h"
#include "register.h"

struct g_program g_program;

static struct command {
    const char *cmd;
    const char *usage;
    int (*callback)(void*);
} command[] = {
        {"info_memory", "display memory mappings", info_memory},
        {"info_regs", "display registers", info_regs},
        {"continue", "continue program being debugged", continue_execution},
        {"quit", "exit", quit},
        {"help", "display this help message", help},
};

int info_regs(void *arg)
{
    arg = arg;
    register_print();
    return 1;
}

int continue_execution(void *arg)
{
    arg = arg;
    printf("Continue execution\n");
    if (ptrace(PTRACE_CONT, g_program.pid, 0, 0) < 0)
        return 0;
    return 1;
}

int quit(void *arg)
{
    arg = arg;
    printf("Quit\n");
    return 1;
}

int help(void *arg)
{
    arg = arg;
    for (size_t i = 0; i < sizeof(command) / sizeof(struct command); ++i)
        printf("%s: %s\n", command[i].cmd, command[i].usage);
    return 1;
}

int dispatch_command(char *cmd)
{
    for (size_t i = 0; i < sizeof(command) / sizeof(struct command); ++i)
        if (!strcmp(cmd, command[i].cmd))
            return command[i].callback(NULL);

    printf("Unknown command\n");
    return 1;
}
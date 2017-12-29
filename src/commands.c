#include <stdio.h>
#include <string.h>

#include "commands.h"

static struct command {
    const char *cmd;
    const char *usage;
    int (*callback)(void*);
} command[] = {
        {"info_regs", "display registers", info_regs},
        {"continue", "continue program being debugged", continue_execution},
        {"quit", "exit", quit},
        {"help", "display this help message", help},
};

int info_regs(void *arg)
{
    arg = arg;
    printf("Info regs\n");
    return 1;
}

int continue_execution(void *arg)
{
    arg = arg;
    printf("Continue execution\n");
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
        printf("%s", command[i].usage);
    return 1;
}

int dispatch_command(char *cmd)
{
    for (size_t i = 0; i < sizeof(command) / sizeof(struct command); ++i)
        if (!strcmp(cmd, command[i].cmd))
            return command[i].callback(NULL);
    return 1;
}
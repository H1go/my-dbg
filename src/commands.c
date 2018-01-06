#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "breakpoint.h"
#include "commands.h"
#include "debug.h"
#include "register.h"

struct g_program g_program;

static struct command {
    const char *cmd;
    const char *usage;
    int (*callback)(void*);
} command[] = {
        {"backtrace", "Print call trace at current rip", backtrace},
        {"examine", "Read into the debugged program memory", examine},
        {"step_instr", "Allow to single step into your program", step},
        {"break_list", "Print a table of all breakpoints", break_list},
        {"break", "Set break at 0xaddr", breakpoint_set},
        {"info_memory", "display memory mappings", info_memory},
        {"info_regs", "display registers", register_print},
        {"continue", "continue program being debugged", continue_execution},
        {"quit", "exit", quit},
        {"help", "display this help message", help},
};

int help(void *arg)
{
    arg = arg;
    for (size_t i = 0; i < sizeof(command) / sizeof(struct command); ++i)
        printf("%s: %s\n", command[i].cmd, command[i].usage);
    return 1;
}

int break_list(void *arg)
{
    arg = arg;
    list_print(g_program.breakpoints);
    return 1;
}

int quit(void *arg)
{
    arg = arg;
    exit(0);
    return 1;
}

static char *get_arguments(char *cmd)
{
    for (size_t i = 0; i < strlen(cmd); ++i)
        if (cmd[i] == ' ')
            return cmd + i + 1;
    return NULL;
}

int dispatch_command(char *cmd)
{
    char *args = get_arguments(cmd);
    cmd = strtok(cmd, " ");
    if (!cmd)
        return 0;

    for (size_t i = 0; i < sizeof(command) / sizeof(struct command); ++i) {
        if (!strcmp(cmd, command[i].cmd))
            return command[i].callback(args);
    }
    printf("Unknown command\n");
    return 1;
}
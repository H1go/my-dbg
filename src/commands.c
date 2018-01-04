#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>

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
        {"break_list", "Print a table of all breakpoints", break_list},
        {"break", "Set break at 0xaddr", breakpoint},
        {"info_memory", "display memory mappings", info_memory},
        {"info_regs", "display registers", info_regs},
        {"continue", "continue program being debugged", continue_execution},
        {"quit", "exit", quit},
        {"help", "display this help message", help},
};

int break_list(void *arg)
{
    arg = arg;
    list_print(g_program.breakpoints);
    return 1;
}

int breakpoint(void *arg)
{
    char *addr = arg;
    if (!addr) {
        printf("Usage: break 0xaddr\n");
        return 0;
    }
    breakpoint_set_at_adress(addr);
    return 1;
}

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
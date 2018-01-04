#include <stdio.h>

#include "commands.h"
#include "debug.h"
#include "register.h"

struct g_program g_program;

int break_list(void *arg)
{
    arg = arg;
    list_print(g_program.breakpoints);
    return 1;
}

int info_regs(void *arg)
{
    arg = arg;
    register_print();
    return 1;
}

int quit(void *arg)
{
    arg = arg;
    printf("Quit\n");
    return 1;
}
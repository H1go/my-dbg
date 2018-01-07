#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif


#include <capstone/capstone.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <inttypes.h>

#include "debug.h"

struct g_program g_program;

static int get_arg(char *s, char *args[])
{
    char *p = strtok(s, " ");
    size_t i = 0;

    while (p != NULL)
    {
        args[i++] = p;
        p = strtok(NULL, " ");
    }
    return i == 3;
}

static void examine_format(char c, uintptr_t data)
{
    switch (c) {
        case 'd':
            printf("%d\n", (int)data);
            break;
        case 'x':
            printf("0x%lx\n", data);
            break;
        case 's':
            printf("\n");
            break;
        case 'i':
            printf("\n");
            break;
        default:
            break;
    }
}

int examine(void *arg)
{
    char *args[3];
    if (!get_arg(arg, args))
    {
        printf("Usage: examine format size start_addr\n");
        return 0;
    }

    printf("format: %s, size: %s, addr: 0x%s\n", args[0], args[1], args[2]);

    uintptr_t new_addr = strtol(args[2], 0, 16);

    int s = atoi(args[1]);
    long data;

    for (int i = 0; i < s; ++i)
    {
        data = ptrace(PTRACE_PEEKDATA, g_program.pid, (void *)new_addr, 0);

        if (data < 0 && errno) {
            perror("");
            return 0;
        }

        printf("0x%lx: ", new_addr);
        examine_format(*args[0], (uintptr_t )data);
        new_addr += 4;
    }
    return 1;
}
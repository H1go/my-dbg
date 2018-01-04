#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>

#include "breakpoint.h"
#include "debug.h"

struct g_program g_program;
static int g_count = 1;

struct breakpoint *breakpoint_init(char *addr)
{
    struct breakpoint *b = malloc(sizeof (struct breakpoint));
    if (!b)
        return NULL;
    b->id = g_count++;
    b->activated = 0;
    b->addr = strtol(addr, 0, 16);
    return b;
}

void breakpoint_activate(struct breakpoint *b)
{
    b->activated = 1;
    long data = ptrace(PTRACE_PEEKDATA, g_program.pid, (void *)b->addr, 0);
    if (data < 0 && errno)
        perror("Activate 1");

    b->data = (data & 0xFF);
    printf("Original data at 0x%lx: 0x%lx\n", b->addr, b->data);

    long int3 = ((data & ~0xFF) | 0xCC);
    data = ptrace(PTRACE_POKEDATA, g_program.pid, (void *)b->addr, int3);
    if (data < 0 && errno)
        perror("Activate 2");

    printf("Data at 0x%lx is now: 0x%lx\n", b->addr, data);
}

void breakpoint_deactivate(struct breakpoint *b)
{
    long data = ptrace(PTRACE_PEEKDATA, g_program.pid, (void *)b->addr, 0);
    if (data < 0 && errno)
        perror("");

    assert((data & 0xFF) == 0xCC);

    long save = ((data & ~0xFF) | b->data);
    if (ptrace(PTRACE_POKEDATA, g_program.pid, (void *)b->addr, save) < 0)
        if (errno)
            perror("");
    b->activated = 0;
}

void breakpoint_set_at_adress(char *addr)
{
    struct breakpoint *b = breakpoint_init(addr);
    if (!b)
        return;

    printf("Set breakpoint at adress 0x%lx\n", b->addr);
    breakpoint_activate(b);

    list_push(g_program.breakpoints, b);
}
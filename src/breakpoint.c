#include <assert.h>
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
    b->data = (data & 0xFF);
    printf("Original data at 0x%lx: 0x%lx", b->addr, b->data);

    long int3 = ((data & ~0xFF) | 0xCC);
    data = ptrace(PTRACE_POKEDATA, g_program.pid, (void *)b->addr, int3);

    printf("Data at 0x%lx is now: 0x%lx", b->addr, data);
}

void breakpoint_deactivate(struct breakpoint *b)
{
    long data = ptrace(PTRACE_PEEKDATA, g_program.pid, (void *)b->addr, 0);
    assert((data & 0xFF) == 0xCC);

    long save = ((data & ~0xFF) | b->data);
    ptrace(PTRACE_POKEDATA, g_program.pid, (void *)b->addr, save);
    b->activated = 0;
}

void breakpoint_set_at_adress(char *addr)
{
    struct breakpoint *b = breakpoint_init(addr);
    if (!b)
        return;

    printf("Set breakpoint at adress 0x%lx", b->addr);
    breakpoint_activate(b);
}
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>

#include "breakpoint.h"
#include "debug.h"
#include "register.h"
#include "sig.h"

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

    printf("Restored data is: 0x%lx\n", save);

    if (ptrace(PTRACE_POKEDATA, g_program.pid, (void *)b->addr, save) < 0)
        if (errno)
            perror("");
    b->activated = 0;
}

int breakpoint_set(void *arg)
{
    char *addr = arg;
    if (!addr) {
        printf("Usage: break 0xaddr\n");
        return 0;
    }

    struct breakpoint *b = breakpoint_init(addr);
    if (!b)
        return 0;

    printf("Set breakpoint at adress 0x%lx\n", b->addr);
    breakpoint_activate(b);

    list_push(g_program.breakpoints, b);
    return 1;
}

static struct breakpoint *breakpoint_get(uintptr_t addr)
{
    struct node *node = g_program.breakpoints->head;
    for (; node != NULL; node = node->next) {
        if (node->data->addr == addr)
            return node->data;
    }
    return NULL;
}

void restore_context(void)
{
    uintptr_t next = register_read(rip);

    struct breakpoint *b = breakpoint_get(next);
    if (!b)
        return;

    if (b->activated)
    {
        printf("Bp activated\n");
        breakpoint_deactivate(b);
        if (ptrace(PTRACE_SINGLESTEP, g_program.pid, 0, 0) < 0) {
            perror("Single step");
            return;
        }
        handle_wait();
        breakpoint_activate(b);
    }
}
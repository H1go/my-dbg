#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>

#include "breakpoint.h"
#include "debug.h"
#include "register.h"
#include "sig.h"

struct g_program g_program;

struct breakpoint *breakpoint_get(uintptr_t addr)
{
    struct node *node = g_program.breakpoints->head;
    for (; node != NULL; node = node->next) {
        if (node->data->addr == addr)
            return node->data;
    }
    return NULL;
}

static int breakpoint_remove(int id)
{
    struct node *node = g_program.breakpoints->head;

    if (node->data->id == id) {
        list_pop(g_program.breakpoints);
        return 1;
    }

    int found = 0;
    for (; node->next != NULL; node = node->next)
        if (node->next->data->id == id)
        {
            found = 1;
            break;
        }

    if (!found) {
        printf("No breakpoints found\n");
        return 0;
    }

    struct node *tmp = node->next;
    node->next = tmp->next;
    free(tmp);
    tmp = NULL;
    --g_program.breakpoints->size;

    return 1;
}

int breakpoint_delete(void *arg)
{
    if (!arg) {
        printf("Usage: break_del <number>");
        return 0;
    }

    int id = atoi(arg);

    if  (!g_program.breakpoints->size) {
        printf("No breakpoints found\n");
        return 0;
    }

    return breakpoint_remove(id);
}

void breakpoint_step(struct breakpoint *b)
{
    if (b->activated)
    {
        breakpoint_deactivate(b);
        if (ptrace(PTRACE_SINGLESTEP, g_program.pid, 0, 0) < 0) {
            perror("Single step");
            return;
        }
        handle_wait();
        if (b->type == PERM)
            breakpoint_activate(b);
    }
}

void restore_context(void)
{
    uintptr_t next = register_read(rip);

    struct breakpoint *b = breakpoint_get(next);
    if (!b)
        return;
    breakpoint_step(b);
}
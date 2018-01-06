#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <stdint.h>
#include <unistd.h>

struct breakpoint
{
    int id;
    uintptr_t addr;
    long data;
    int activated;
};

struct breakpoint *breakpoint_init(char *addr);

void breakpoint_activate(struct breakpoint *b);

void breakpoint_deactivate(struct breakpoint *b);

int breakpoint_set(void *arg);

void breakpoint_step(struct breakpoint *b);

void restore_context(void);

#endif /* !BREAKPOINT_H */

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

void breakpoint_set_at_adress(char *addr);

#endif /* !BREAKPOINT_H */

#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <stdint.h>
#include <unistd.h>

enum type
{
    PERM = 0,
    TEMP,
};

struct breakpoint
{
    int id;
    enum type type;
    uintptr_t addr;
    long data;
    int activated;

};

void breakpoint_activate(struct breakpoint *b);

void breakpoint_deactivate(struct breakpoint *b);

int tbreak_set(void *arg);

int breakpoint_set(void *arg);

int breakpoint_delete(void *arg);

struct breakpoint *breakpoint_get(uintptr_t addr);

void breakpoint_step(struct breakpoint *b);

void restore_context(void);

#endif /* !BREAKPOINT_H */

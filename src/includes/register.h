#include <stdint.h>
#include <unistd.h>

#ifndef REGISTER_H
#define REGISTER_H

enum name
{
    rip,
    rsp,
    rbp,
    eflags,
    orig_rax,
    rax,
    rbx,
    rcx,
    rdx,
    rdi,
    rsi,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    cs,
    ds,
    es,
    fs,
    gs,
    ss,
    fs_base,
    gs_base,
};

int register_print(void *arg);

uintptr_t register_read(enum name name);

void register_write(enum name name, uintptr_t value);

#endif /* !REGISTER_H */

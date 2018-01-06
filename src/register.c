#include <stdio.h>
#include <stdint.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>

#include "debug.h"
#include "register.h"

struct g_program g_program;

static struct registers
{
    enum name name;
    const char *string;
} registers[] = {
        {r15, "r15"},
        {r14, "r14"},
        {r13, "r13"},
        {r12, "r12"},
        {rbp, "rbp"},
        {rbx, "rbx"},
        {r11, "r11"},
        {r10, "r10"},
        {r9, "r9"},
        {r8, "r8"},
        {rax, "rax"},
        {rcx, "rcx"},
        {rdx, "rdx"},
        {rsi, "rsi"},
        {rdi, "rdi"},
        {orig_rax, "orig_rax"},
        {rip, "rip"},
        {cs, "cs"},
        {eflags, "eflags"},
        {rsp, "rsp"},
        {ss, "ss"},
        {fs_base, "fs_base"},
        {gs_base, "gs_base"},
        {ds, "ds"},
        {es, "es"},
        {fs, "fs"},
        {gs, "gs"},
};

int register_print(void *arg)
{
    arg = arg;
    for (size_t i = 0; i < sizeof(registers) / sizeof(struct registers); ++i) {
        uintptr_t addr = register_read(registers[i].name);
        printf("%s: 0x%lx\n", registers[i].string, addr);
    }
    return 1;
}

uintptr_t register_read(enum name name)
{
    struct user_regs_struct regs;
    if (ptrace(PTRACE_GETREGS, g_program.pid, 0, &regs) < 0) {
        perror("");
        return 0;
    }
    size_t field_size = sizeof(unsigned long long);
    size_t i = 0;
    for (; i < sizeof(registers) / sizeof(struct registers); ++i)
        if (registers[i].name == name)
            break;

    char *ptr = (char *)&regs;
    return *(unsigned long long *)(ptr + (i * field_size));
}

void register_write(enum name name, uintptr_t value)
{
    struct user_regs_struct regs;
    if (ptrace(PTRACE_GETREGS, g_program.pid, 0, &regs) < 0) {
        perror("");
        return;
    }
    size_t field_size = sizeof(unsigned long long);
    size_t i = 0;
    for (; i < sizeof(registers) / sizeof(struct registers); ++i)
        if (registers[i].name == name)
            break;

    char *ptr = (char *)&regs;
    *(unsigned long long *)(ptr + (i * field_size)) = value;
    if (ptrace(PTRACE_SETREGS, g_program.pid, 0, &regs) < 0) {
        perror("");
        return;
    }
}
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <bits/types/siginfo_t.h>

#include "debug.h"
#include "register.h"
#include "sig.h"

struct g_program g_program;

static void handle_call(siginfo_t sig)
{
    uint64_t next;
    printf("YOLO\n");

    switch (sig.si_code) {
        case SI_KERNEL:
        case TRAP_BRKPT:
            next = register_read(rip);
            register_write(rip, next - 1);
            printf("Reset pc\n");
            break;
        default:
            break;
    }
}

void handle_wait(void)
{
    int status;
    if (waitpid(g_program.pid, &status, 0) < 0) {
        perror("");
        return;
    }

    siginfo_t sig;
    if (ptrace(PTRACE_GETSIGINFO, g_program.pid, 0, &sig) < 0) {
        perror("");
        return;
    }

    switch (sig.si_signo) {
        case SIGTRAP:
            handle_call(sig);
            break;
        default:
            break;
    }
}
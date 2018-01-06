#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <stdio.h>

int backtrace(void *arg)
{
    arg = arg;
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip;
    unw_word_t sp;

    unw_getcontext(&uc);
    unw_init_local(&cursor, &uc);
    while (unw_step(&cursor) > 0) {
        unw_get_reg(&cursor, UNW_REG_IP, &ip);
        unw_get_reg(&cursor, UNW_REG_SP, &sp);
        printf("ip = 0x%lx, sp = 0x%lx\n", (long) ip, (long) sp);
    }
    return 1;
}
#ifndef COMMANDS_H
#define COMMANDS_H


int backtrace(void *arg);

int examine(void *arg);

int step(void *arg);

int break_list(void *arg);

int breakpoint(void *arg);

int info_memory(void *arg);

int info_regs(void *arg);

int continue_execution(void *arg);

int quit(void *arg);

int help(void *arg);

int dispatch_command(char *cmd);

#endif /* !COMMANDS_H */

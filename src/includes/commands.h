#ifndef COMMANDS_H
#define COMMANDS_H

int info_regs(void *arg);

int continue_execution(void *arg);

int quit(void *arg);

int help(void *arg);

int dispatch_command(char *cmd);

#endif /* !COMMANDS_H */

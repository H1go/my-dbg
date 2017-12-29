#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "commands.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        warnx("Usage: ./my-dbg /path/to/binary arguments...");
        return 1;
    }

    printf("Program being debugged is %s\n", argv[1]);
    while (1) {
        char *cmd = readline("dbg> ");
        if (!strcmp(cmd, "quit")) {
            free(cmd);
            return 0;
        }
        dispatch_command(cmd);
        free(cmd);
    }
}

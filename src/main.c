#include <err.h>
#include <stdio.h>

#include "debug.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        warnx("Usage: ./my-dbg /path/to/binary arguments...");
        return 1;
    }

    printf("Program being debugged is %s\n", argv[1]);
    if (!debug(argv))
        return 1;
    return 0;
}

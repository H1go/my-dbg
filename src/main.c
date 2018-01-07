#include <err.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "debug.h"

static int is_executable(char *filename)
{
    struct stat sb;
    return stat(filename, &sb) == 0 || sb.st_mode & S_IXUSR;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        warnx("Usage: ./my-dbg /path/to/binary arguments...");
        return 1;
    }

    if (!is_executable(argv[1]))
        return 1;

    printf("Program being debugged is %s\n", argv[1]);
    if (!debug(argv))
        return 1;
    return 0;
}

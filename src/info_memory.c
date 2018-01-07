#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

struct g_program g_program;

int info_memory(void *arg)
{
    arg = arg;
    char buffer[100] = {0};
    sprintf(buffer, "/%s/%d/%s", "proc", g_program.pid, "maps");

    FILE *stream = fopen(buffer, "r");
    if (!stream) {
        perror("Fopen");
        return 0;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&line, &len, stream)) > 0)
        fwrite(line, nread, 1, stdout);

    free(line);
    if (fclose(stream) < 0) {
        perror("");
        return 0;
    }
    return 1;
}
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

static char *allocate_buffer(void)
{
    static char *buffer = NULL;
    if (!buffer) {
        size_t size = snprintf(NULL, 0, "/%s/%d/%s", "proc", g_program.pid,
                               "maps");
        buffer = calloc(size + 2, sizeof(char));
        if (!buffer)
            return NULL;

        snprintf(buffer, size + 2, "/%s/%d/%s", "proc", g_program.pid, "maps");
    }
    return buffer;
}

int info_memory(void *arg)
{
    arg = arg;
    char *buffer = allocate_buffer();
    if (!buffer)
        return 0;

    FILE *stream = fopen(buffer, "r");
    if (!stream) {
        perror("Fopen");
        free(buffer);
        return 0;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&line, &len, stream)) > 0)
        fwrite(line, nread, 1, stdout);

    free(line);
    free(buffer);
    if (fclose(stream) < 0) {
        perror("");
        return 0;
    }
    return 1;
}
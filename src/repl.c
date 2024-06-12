#include "repl.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

void run_repl()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read_length = 0;
    printf("> ");
    read_length = getline(&line, &len, stdin);
    if (read_length == -1)
        return;

    printf("Entered: %s\n", line);

    free(line);
}
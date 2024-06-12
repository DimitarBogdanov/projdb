#include "repl.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include "sql/tokens.h"

void run_repl()
{
    char* line = NULL;
    size_t len = 0;
    ssize_t read_length = 0;
    printf("> ");
    read_length = getline(&line, &len, stdin);
    if (read_length == -1)
        return;

    printf("Entered: %s\n", line);
    tokenize(read_length, line);
    free(line);
}
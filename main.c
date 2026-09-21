/* #include "chunk.h" */
#include "common.h"
#include "vm.h"

/* #include <errno.h> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void repl();
static void runFile(const char *path);

int main(int argc, char *argv[])
{
    initVm();

    // rearrange the argument count and value.
    argc--;
    argv++;

    if (argc == 0) repl();
    else if (argc == 1) runFile(argv[0]);
    else
    { // wrong argument count.
        fprintf(stderr, "Usage: clox [path]\n");
        return EX_USAGE;
    }

    return EXIT_SUCCESS;
}

void repl()
{
    char line[1024];

    for (;;)
    {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

static char *readFile(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Can't open file %s.\n", path);
        exit(EX_IOERR);
    }

    fseek(fp, 0L, SEEK_END);
    size_t fileSize = ftell(fp);
    rewind(fp);

    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Failed to allocate %zu byte of memory.\n", fileSize);
        exit(EX_IOERR);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, fp);
    if (bytesRead < fileSize)
    {
        fprintf(stderr,
                "Error in reading into %s. bytes read [%zu] is different from size "
                "of the file [%zu].\n",
                path, bytesRead, fileSize);
        exit(EX_IOERR);
    }
    buffer[bytesRead] = '\0';

    fclose(fp);
    return buffer;
}

void runFile(const char *path)
{
    char *source = readFile(path);
    if (source != NULL)
    { // only if the reading was correct.
        InterpretResult result = interpret(source);
        free(source);

        if (result == INTERPRET_COMPILE_ERROR) exit(65);
        if (result == INTERPRET_RUNTIME_ERROR) exit(70);
    }
}

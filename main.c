#include "common.h"
#include "chunk.h"
#include "vm.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    initVm();

    Chunk chunk;
    initChunk(&chunk);

    writeConstant(&chunk, 5.5, 123);
    writeConstant(&chunk, 6.5, 123);
    writeChunk(&chunk, OP_ADD, 123);

    writeConstant(&chunk, 3.3, 123);
    writeChunk(&chunk, OP_SUB, 123);

    writeConstant(&chunk, 0.7, 123);
    writeChunk(&chunk, OP_SUB, 123);

    writeConstant(&chunk, 2, 123);
    writeChunk(&chunk, OP_DIV, 123);

    writeChunk(&chunk, OP_RETURN, 125);

    //disassembleChunk(&chunk, "test chunk");
    (void) interpret(&chunk);

    freeVm();
    freeChunk(&chunk);

    return EXIT_SUCCESS;
}

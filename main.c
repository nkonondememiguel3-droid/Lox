#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    initVm();

    Chunk chunk;
    initChunk(&chunk);

    for (size_t i = 0; i < 5; i++)
        writeConstant(&chunk, 5.5 + (float) i, 123 + (i % 100 == 0 ? 10 : 0));

    writeChunk(&chunk, OP_RETURN, 124);

    //disassembleChunk(&chunk, "test chunk");
    (void) interpret(&chunk);

    freeVm();
    freeChunk(&chunk);

    return EXIT_SUCCESS;
}

#include "debug.h"

#include <stdio.h>

static int simpleInstruction(const char *name, int offset);

static int constantInstruction(const char *name, const Chunk *chunk, int offset);

static int constantLongInstruction(const char *name, const Chunk *chunk, int offset);

static void printValue(Value value);

void disassembleChunk(const Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

int disassembleInstruction(const Chunk *chunk, const int offset) {
    printf("%04d ", offset);

    // print the source line.
    if (offset > 0 && getLine(chunk, offset) == getLine(chunk, offset - 1))
        printf("    | ");
    else
        printf(" %4llu ", getLine(chunk, offset));

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return constantLongInstruction("OP_CONSTANT_LONG", chunk, offset);
        default:
            fprintf(stderr, "Unknow opcode(operation code) %d\n", instruction);
            return offset + 1;
    }
}

static int simpleInstruction(const char *name, int offset) {
    printf("%s\n", name);

    return offset + 1;
}

static int constantInstruction(const char *name, const Chunk *chunk, int offset) {
    const uint8_t constIndex = chunk->code[offset + 1];

    printf("%-16s %4d '", name, constIndex);
    printValue(chunk->constants.values[constIndex]);
    printf("'\n");

    return offset + 2;
}

static void printValue(const Value value) {
    printf("%g", value);
}

static int constantLongInstruction(const char *name, const Chunk *chunk, const int offset) {
    /* 300
     * ( 1 << 8 ) = 256
     * ( 0 << 16 ) = 0
     * 44 | ( 1 << 8 ) = 300
     * 44 | ( 1 << 8 ) | ( 0 << 16 ) = 300
     *
     * 50, 000 = 0000 0000 1100 0011 0101 0000 ( 24 bits )
     * 0xff = 255 = 0000 0000 0000 0000 1111 1111 ( 24 bits )
     *
     * chunk->code[offset + 1] = 0000 0000 0000 0000 0101 0000 ( 80 in decimal )
     * chunk->code[offset + 2] = 0000 0000 0000 0000 1100 0011 ( 195 in decimal )
     * chunk->code[offset + 3] = 0000 0000 0000 0000 0000 0000 ( 0 in decimal )
     *
     * chunk->code[offset + 2] << 8 = 0000 0000 1100 0011 0000 0000 ( 49, 920 in decimal )
     * chunk->code[offset + 3] << 16 = 0000 0000 0000 0000 0000 0000 ( 0 in decimal )
     *
     * so now we have to join the three byte:
     * 0000 0000 0000 0000 0101 0000
     * |
     * 0000 0000 1100 0011 0000 0000
     * |
     * 0000 0000 0000 0000 0000 0000
     * =
     * 0000 0000 1100 0011 0101 0000
     * */
    const uint32_t constantIdx =
            (uint32_t) chunk->code[offset + 1] |
            ((uint32_t) chunk->code[offset + 2] << 8) |
            ((uint32_t) chunk->code[offset + 3] << 16);

    if (constantIdx >= (uint32_t) chunk->constants.count) {
        fprintf(stderr, "Invalid constant index %d. (constant count : %zu)\n", constantIdx, chunk->constants.count);
        return offset + 4;
    }

    printf("%-16s %4d '", name, constantIdx);
    printValue(chunk->constants.values[constantIdx]);
    printf("'\n");
    return offset + 4;
}

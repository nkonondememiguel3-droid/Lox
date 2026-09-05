#include "chunk.h"

static void initLines(Lines *lines) {
    lines->count = 0;
    lines->capacity = 0;

    lines->lineObj = NULL;
}

static void writeLinesLine(Lines *lines, size_t line) {

    if (lines->count > 0 &&
        lines->lineObj[lines->count - 1].line == line) {
        lines->lineObj[lines->count - 1].lineCount++;
    } else {
        /* grow the lines array if necessary. */
        if ( lines->capacity < lines->count + 1 ) {
            const size_t oldLinesCapacity = lines->capacity;
            lines->capacity = GROW_CAPACITY(oldLinesCapacity);
            lines->lineObj = GROW_ARRAY(LineObj, lines->lineObj, oldLinesCapacity, lines->capacity);
        }

        lines->lineObj[lines->count] = (LineObj){ .line = line, .lineCount = 1 };
        lines->count++;
    }

}

void initChunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;

    initLines(&chunk->lines);

    initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, const uint8_t byte, size_t line) {
    if (chunk->capacity < chunk->count + 1) {
        const size_t oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    writeLinesLine(&chunk->lines, line);
    chunk->count++;
}

void writeConstant(Chunk *chunk, const Value value, const size_t line) {
    const size_t index = addConstant(chunk, value);

    if (index < 256) { /* store constant in one byte. */
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, (uint8_t)index, line);
    } else { /* store the constant in three bytes. */
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        /* 300
         *
         * 300 & 0xff = 44
         * ( 300 >> 8 ) & 0xff = 1 & 0xff = 1
         * ( 300 >> 16 ) & 0xff = 0 & 0xff = 0
         *
         * 50, 000 = 0000 0000 1100 0011 0101 0000 ( 24 bits )
         * 0xff = 255 = 0000 0000 0000 0000 1111 1111 ( 24 bits )
         *
         * clearly, 50, 000 can not fit into one byte aka 8 bits.
         * so what we do is to divide it into three bytes and store
         * each byte separately.
         *
         * extract the lower first byte.
         * 50, 000 & 0xff = 0000 0000 1100 0011 0101 0000
         *                  &
         *                  0000 0000 0000 0000 1111 1111
         *                = 0000 0000 0000 0000 0101 0000 ( 80 in decimal )
         *
         * extract the second lower byte.
         * ( 50, 000 >> 8 ) & 0xff = 0000 0000 1100 0011 0101 0000
         *                           >>
         *                           8
         *                           &
         *                           0000 0000 0000 0000 1111 1111
         *                         =
         *                           0000 0000 0000 0000 1100 0011
         *                           &
         *                           0000 0000 0000 0000 1111 1111
         *                         =
         *                           0000 0000 0000 0000 1100 0011 ( 195 in decimal )
         * extract the third lower byte.
         * ( 50, 000 >> 16 ) & 0xff = 0000 0000 1100 0011 0101 0000
         *                            >>
         *                            16
         *                            &
         *                            0000 0000 0000 0000 1111 1111
         *                          =
         *                            0000 0000 0000 0000 0000 0000
         *                            &
         *                            0000 0000 0000 0000 1111 1111
         *                          =
         *                           0000 0000 0000 0000 0000 0000 ( 0 in decimal )
         *
         *                 24-bit index
                   ┌────────┬────────┬────────┐
                   │ byte 2 │ byte 1 │ byte 0 │
                   └────────┴────────┴────────┘
                      0x00     0xC3     0x50
                                          ↑
                                     written first

            bytecode:
            ┌──────────────────┬────────┬────────┬────────┐
            │ OP_CONSTANT_LONG │  0x50  │  0xC3  │  0x00  │
            └──────────────────┴────────┴────────┴────────┘
         */
        writeChunk(chunk, (uint8_t)(index & 0xff), line);
        writeChunk(chunk, (uint8_t)((index >> 8) & 0xff), line);
        writeChunk(chunk, (uint8_t)((index >> 16) & 0xff), line);
    }
}

size_t getLine(const Chunk *chunk, size_t instructionIndex) {

    size_t cumulative = 0;
    for ( size_t i = 0; i < chunk->lines.count; i++ ) {
        cumulative += chunk->lines.lineObj[i].lineCount;
        if (instructionIndex < cumulative) return chunk->lines.lineObj[i].line;
    }

    return 0; /* instructionIndex out of range. */
}

void freeChunk(Chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(LineObj, chunk->lines.lineObj, chunk->lines.capacity);
    freeValueArray(&chunk->constants);

    initChunk(chunk);
}

size_t addConstant(Chunk *chunk, const Value constant) {
    writeValueArray(&chunk->constants, constant);
    return chunk->constants.count - 1;
}

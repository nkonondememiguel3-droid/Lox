#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "memory.h"
#include "value.h"

/* a bytecode is a dense, linear sequence of binary instructions. */
/* each bytecode has a one-byte operation code shortened to 'opcode', it's used to control what kind of instruction
    we're dealing with. */
/* a chunk of bytecode is a list of bytecodes. */

/* operation code representation. */
typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_RETURN,
} OpCode;

/* line object. */
typedef struct {
    size_t line;
    size_t lineCount;
} LineObj;

typedef struct {
    size_t count;
    size_t capacity;

    LineObj *lineObj;
} Lines;

/* chunk data type representation. */
typedef struct {
    size_t count;
    size_t capacity; /* number of elements in the array we've allocated. */
    uint8_t *code; /* number of elements allocated already in used. */

    ValueArray constants; /* this represent the constant pool for a chunk. */

    Lines lines; /* line information for each bytecode in the chunk of bytecodes. */
} Chunk;

extern void initChunk(Chunk *chunk);

extern void writeChunk(Chunk *chunk, uint8_t byte, size_t line);

extern void writeConstant(Chunk *chunk, Value value, size_t line);

extern void freeChunk(Chunk *chunk);

/* write a constant into the constant pool of chunk and return the index of that constant in the constant pool.
op_constant 1 -> is going to write in the stream of bytecodes the opcode(op_constant) immediately followed by the
index where the constant(1) were written in the value array constant pool. */
extern size_t addConstant(Chunk *chunk, Value constant);

extern size_t getLine(const Chunk *chunk, size_t instructionIndex);


#endif // clox_chunk_h

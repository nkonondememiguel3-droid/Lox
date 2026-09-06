#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"

typedef struct {
    Chunk *chunk;
    uint8_t *ip; /* instruction pointer, used to keep track of the location of the currently executed instruction. */
} Vm;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

extern void initVm();
extern InterpretResult interpret(Chunk *chunk);
extern void freeVm();

#endif // clox_vm_h
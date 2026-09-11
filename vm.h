#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_LIMIT 256

typedef struct {
    Chunk *chunk;
    uint8_t *ip; /* instruction pointer, used to keep track of the location of the currently executed instruction. */

    /* TODO: will make this a dynamic stack. */
    Value stack[STACK_LIMIT];
    Value *stackTop;
} Vm;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

extern void initVm();
extern InterpretResult interpret(Chunk *chunk);
extern void freeVm();

extern void push( Value value );
extern Value pop();

#endif // clox_vm_h

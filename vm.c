#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"

#include <stdio.h>

Vm vm; /* just for learning purpose that we have a global variable. */

static InterpretResult run();

static void printValue(Value value);
static void resetStack() { // make the stackTop points to the next element to be added in the stack.
    vm.stackTop = vm.stack;
}

void initVm() {

    resetStack();
}

InterpretResult interpret(const char* source) {
    // vm.chunk = chunk;
    // vm.ip = vm.chunk->code; /* points to the next instruction that's about to be executed. */

    // return run();
    compile(source);
    
    return INTERPRET_OK;
}

void push( Value value ) {
    *vm.stackTop++ = value;
}

Value pop() {
    return *--vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG() ( \
    (Value)vm.chunk->constants.values[ \
        (uint32_t)READ_BYTE() | \
        ((uint32_t)READ_BYTE() << 8) | \
        ((uint32_t)READ_BYTE() << 16) \
    ] \
)
#define BINARY_OP(op) \
    do { \
        Value b = pop(); \
        Value a = pop(); \
        push( a op b ); \
    }while (0)

    while (true) {

#ifdef DEBUG

        printf("            ");
        for ( Value *slot = vm.stack; slot != vm.stackTop; slot++ ) {
            printf("[ ");
            printValue( *slot );
            printf(" ]");
        }
        printf("\n");

        disassembleInstruction( vm.chunk, (int)(vm.ip - vm.chunk->code) );
#endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_RETURN:
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;

            case OP_CONSTANT:
                Value constant = READ_CONSTANT();
                push(constant);
                break;

            case OP_CONSTANT_LONG:
                Value longConstant = READ_CONSTANT_LONG();
                push( longConstant );
                break;

            case OP_NEGATE: push(-pop()); break;
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUB: BINARY_OP(-); break;
            case OP_MUL: BINARY_OP(*); break;
            case OP_DIV: BINARY_OP(/); break;

            default:
                return INTERPRET_RUNTIME_ERROR;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
}

static void printValue(const Value value) {
    printf("%g", value);
}

void freeVm() {
}

#include "vm.h"
#include "debug.h"

Vm vm; /* just for learning purpose that we have a global variable. */

static InterpretResult run();

static void printValue(Value value);

void initVm() {
}

InterpretResult interpret(Chunk *chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code; /* points to the next instruction that's about to be executed. */

    return run();
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

    while (true) {
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_RETURN:
                return INTERPRET_OK;

            case OP_CONSTANT:
                Value constant = READ_CONSTANT();
                printValue(constant);
                printf("\n");
                break;

            case OP_CONSTANT_LONG:
                printf("long constatn\n");
                Value LongConstant = READ_CONSTANT_LONG();
                printValue(LongConstant);
                printf("\n");
                break;

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

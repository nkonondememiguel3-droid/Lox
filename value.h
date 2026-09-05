#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

/* this value array represent our constant pool. */
typedef struct {
    size_t capacity;
    size_t count;
    Value *values;
} ValueArray;

/* initialize the value array pool. */
extern void initValueArray(ValueArray *valueArray);

/* write a value into the value array pool. */
extern void writeValueArray(ValueArray *valueArray, Value value);

/* free the allocated memory for the value array pool. */
extern void freeValueArray(ValueArray *valueArray);

#endif // clox_value_h

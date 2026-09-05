#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

extern void disassembleChunk(const Chunk* chunk, const char* name);
extern int disassembleInstruction(const Chunk* chunk, int offset);

#endif // clox_debug_h
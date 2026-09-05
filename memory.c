#include "memory.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
	if (newSize == 0) {
		free(pointer);
		return NULL;
	}

	/*
	 - oldSize == 0 -> allocate a new block.
	 - oldSize > newSize -> shrink existing allocation block.
	 - oldSize < newSize -> grow the existing allocation block.
	*/
	void* result = realloc(pointer, newSize);
	if (result == NULL) {
		perror("realloc failed");
		exit(1);
	}
	
	return result;
}


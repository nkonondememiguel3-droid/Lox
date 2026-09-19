#ifndef clox_common_h
#define clox_common_h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DEBUG

#if defined(__unix__) || defined(__APPLE__)
	#include <sysexits.h>
#else
	#ifndef EX_IOERR
		#define EX_IOERR 74
	#endif
#endif



#endif // clox_common_h

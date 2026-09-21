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
		#define EX_IOERR 74			/* error occurred while opening a file. */
	#endif
	#ifndef EX_USAGE
		#define EX_USAGE 64				/* command line usage error *1/ */
	#endif /* ifndef EX_USAGE */
#endif



#endif // clox_common_h

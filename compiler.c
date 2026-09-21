#include "compiler.h"
#include "scanner.h"

#include <stdio.h>

void compile(const char* source)
{
	initScanner(source);

	int line = -1;
	for (;; )	// tempory code.
	{
		Token token = scanToken();
		if ( token.line != line )
		{
			printf("%4d ", token.line);
		}
		else
		{
			printf("	| ");
		}

		printf("%2d '%.*s'\n", token.type, token.length, token.start); // token.start - token.length

		if (token.type == TOKEN_EOF) break;
	}

}

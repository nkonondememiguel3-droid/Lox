#include "scanner.h"

#include <stdbool.h>

typedef struct
{
	const char* start;
	const char* current;
	int line;
} Scanner ;

Scanner scanner;	// on the stack.

void initScanner(const char* source)
{
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

static Token makeToken(TokenType tokenType);
static Token errorToken(const char* msg);
static bool isAtEnd();
static char advance();
static bool match(char c);
static void skipWhitespaces();
static char peek();
static char peekNext();

Token scanToken()
{
	skipWhitespaces();

	scanner.start = scanner.current;

	if (isAtEnd()) return makeToken(TOKEN_EOF);

	char c = advance();

	switch (c)
	{
		case '(': return makeToken(TOKEN_LEFT_PAREN);
		case ')': return makeToken(TOKEN_RIGHT_PAREN);
		case '{': return makeToken(TOKEN_LEFT_BRACE);
		case '}': return makeToken(TOKEN_RIGHT_BRACE);
		case ';': return makeToken(TOKEN_SEMICOLON);
		case ',': return makeToken(TOKEN_COMMA);
		case '.': return makeToken(TOKEN_DOT);
		case '-': return makeToken(TOKEN_MINUS);
		case '+': return makeToken(TOKEN_PLUS);
		case '/': return makeToken(TOKEN_SLASH);

		case '*': return makeToken(TOKEN_STAR);

		case '!':
			return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
		case '=':
			return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
		case '<':
			return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
		case '>':
			return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
	}

	return errorToken("Unexpected character.");
}

Token makeToken(TokenType tokenType)
{
	return (Token) {
		.type = tokenType,
			.start = scanner.start,
			.length = (int)(scanner.current - scanner.start),
			.line = scanner.line
	};
}

Token errorToken(const char* msg)
{
	return (Token) {
		.type = TOKEN_ERROR,
			.start = msg,
			.length = (int)strlen(msg),
			.line = scanner.line
	};
}

char peek()
{
	return *scanner.current;
}

char peekNext()
{
	if (isAtEnd()) return '\0';
	return scanner.current[1];
}

bool match(char c)
{
	if (isAtEnd()) return false;
	if (*scanner.current != c) return false;

	scanner.current++;
	return true;
}

void skipWhitespaces()
{
	for (;; )
	{
		char c = peek();

		switch (c)
		{
		case ' ':
		case '\r':
		case '\t':
			advance();
			break;

		case '\n':
			scanner.line++;
			advance();
			break;

		case '/':
			if (peekNext() == '/')
			{
				while (peek() != '\n' && !isAtEnd()) advance();
			}
			else {
				return ;
			}
			break;

		default:
			return;
		}
	}
}

char advance()
{
	scanner.current++;
	return scanner.current[-1];
}

bool isAtEnd()
{
	return *scanner.current == '\0';
}

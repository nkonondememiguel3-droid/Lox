#include "scanner.h"

#include <stdbool.h>
#include <string.h>

typedef struct
{
    const char *start;
    const char *current;
    int line;
} Scanner;

Scanner scanner; // on the stack.

void initScanner(const char *source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static Token makeToken(TokenType tokenType);
static Token errorToken(const char *msg);
static bool isAtEnd();
static char advance();
static bool match(char c);
static void skipWhitespaces();
static char peek();
static char peekNext();
static Token string();
static bool isDigit(char c);
static Token number();
static bool isAlpha(char c);
static bool isAlphaNumeric(char c);
static Token identifier();

Token scanToken()
{
    skipWhitespaces();

    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();

    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c)
    {
        case '(':
            return makeToken(TOKEN_LEFT_PAREN);
        case ')':
            return makeToken(TOKEN_RIGHT_PAREN);
        case '{':
            return makeToken(TOKEN_LEFT_BRACE);
        case '}':
            return makeToken(TOKEN_RIGHT_BRACE);
        case ';':
            return makeToken(TOKEN_SEMICOLON);
        case ',':
            return makeToken(TOKEN_COMMA);
        case '.':
            return makeToken(TOKEN_DOT);
        case '-':
            return makeToken(TOKEN_MINUS);
        case '+':
            return makeToken(TOKEN_PLUS);
        case '/':
            return makeToken(TOKEN_SLASH);

        case '*':
            return makeToken(TOKEN_STAR);

        case '!':
            return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

        case '"':
            return string(); // string literal.
    }

    return errorToken("Unexpected character.");
}

Token string()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n') scanner.line++; // support multi-lines string.
        advance();                          // advance the current char pointer.
    }

    if (isAtEnd()) errorToken("Unterminatd string.");

    advance(); // consume the last quote.
    return makeToken(TOKEN_STRING);
}

Token number()
{
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peek()))
    {
        advance();

        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static TokenType checkKeyword(int start, int length, const char *rest, TokenType type)
{
    if (scanner.current - scanner.start == start + length && memcmp(scanner.start + start, rest, length) == 0) return type;

    return TOKEN_IDENTIFIER;
}

static TokenType identifierType()
{
    switch (scanner.start[0])
    {
        case 'a':
            return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c':
            return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e':
            return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if ( scanner.current - scanner.start > 1 ) {
                switch ( scanner.start[1] ) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'i':
            return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n':
            return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o':
            return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p':
            return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r':
            return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's':
            return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't':
            if (scanner.current - scanner.start) {
                switch (scanner.start[1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v':
            return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w':
            return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_IDENTIFIER;
}

Token identifier()
{
    while (isAlphaNumeric(peek())) advance();

    return makeToken(identifierType());
}

Token makeToken(TokenType tokenType)
{
    return (Token){
        .type = tokenType,
        .start = scanner.start,
        .length = (int)(scanner.current - scanner.start),
        .line = scanner.line,
    };
}

Token errorToken(const char *msg)
{
    return (Token){
        .type = TOKEN_ERROR,
        .start = msg,
        .length = (int)strlen(msg),
        .line = scanner.line,
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
    for (;;)
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
                    while (peek() != '\n' && !isAtEnd()) advance();
                else return;
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

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

bool isAtEnd()
{
    return *scanner.current == '\0';
}

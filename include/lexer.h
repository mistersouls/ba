#ifndef BA_LEXER_H
#define BA_LEXER_H

#include <stdint.h>
#include <stdlib.h>

/**

id: [a-zA-Z_][a-zA-Z0-9_]*

type: 'string' | 'number' | 'boolean'

declaration: {id} ':' {type} ';'

number: [0-9]+

expression: {id} | 

assignment: {id} '=' {expression}

print: 'print' {expression} ';'

*/


typedef enum {
    TOKEN_ID,
    TOKEN_TYPE,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_VALUE,
    TOKEN_PRINT,

    TOKEN_UNKNOWN

} TokenType;


typedef struct {
    uint8_t *val;
    TokenType type;

} Token;


typedef struct Tokens {
    Token token;
    struct Tokens *prev;
} Tokens;


uint8_t *read_entry(char *filename);
Tokens *new_tokens();
void add_token(Tokens **tokens, uint8_t *val, size_t size_val);
void display_tokens(Tokens *tokens);

#endif // BA_LEXER_H


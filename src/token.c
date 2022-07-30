#include "../include/lexer.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>


Token init_token(uint8_t *val, size_t size_val) {
    Token token;
    token.val = malloc(sizeof(uint8_t) * (size_val + 1));
    strncpy(token.val, val, size_val);
    token.type = tokenof(token.val);

    return token;
}

TokenType tokenof(uint8_t *val) {
    if (isid(val))			            return TOKEN_ID;
    if (istype(val))			        return TOKEN_TYPE;
    if (strcmp(val, ":") == 0)		    return TOKEN_COLON;
    if (strcmp(val, ";") == 0)		    return TOKEN_SEMICOLON;
    if (strcmp(val, "=") == 0)		    return TOKEN_ASSIGNMENT;
    if (strcmp(val, "(") == 0)		    return TOKEN_LPARENTHESIS;
    if (strcmp(val, ")") == 0)		    return TOKEN_RPARENTHESIS;
    if (strcmp(val, "+") == 0)		    return TOKEN_PLUS;
    if (strcmp(val, "-") == 0)		    return TOKEN_MINUS;
    if (strcmp(val, "*") == 0)		    return TOKEN_MUL;
    if (strcmp(val, "/") == 0)		    return TOKEN_DIV;
    if (isnumber(val))			        return TOKEN_NUMBER;
    if (strcmp(val, "print") == 0)	    return TOKEN_PRINT;

    return TOKEN_UNKNOWN;
}

uint8_t *stringof_type(TokenType type) {
    switch (type) {
        case TOKEN_ID:			        return "ID";
        case TOKEN_TYPE:		        return "TYPE";
        case TOKEN_COLON:		        return "COLON";
        case TOKEN_SEMICOLON:		    return "SEMICOLON";
        case TOKEN_ASSIGNMENT:		    return "ASSIGNMENT";
        case TOKEN_LPARENTHESIS:        return "LEFT_PARENTHESIS";
        case TOKEN_RPARENTHESIS:        return "RIGHT_PARENTHESIS";
        case TOKEN_PLUS:                return "PLUS";
        case TOKEN_MINUS:               return "MINUS";
        case TOKEN_MUL:                 return "MUL";
        case TOKEN_DIV:                 return "DIV";
        case TOKEN_NUMBER:		        return "NUMBER";
        case TOKEN_PRINT:		        return "PRINT";
        default: 			            return "UNKNOWN";
    }
}

bool isid(uint8_t *val) {
    size_t i = 0;
   
    if (iskeyword(val)) {
	    return false;
    }

    if (val[i] == '_' || isalpha(val[i])) {
	    ++i;
    } else {
	    return false;
    }

    while (i < strlen(val) && (val[i] == '_' || isalnum(val[i]))) {
	    ++i;
    }

    return strlen(val) == i;
    
}

bool istype(uint8_t *val) {
    size_t size = sizeof(TYPES) / sizeof(uint8_t *);
    return strcontains(TYPES, size, val);
}

bool isnumber(uint8_t *val) {
    size_t i = 0;

    while (i < strlen(val) && isdigit(val[i])) {
	    ++i;
    }

    return i == strlen(val);
}

bool iskeyword(uint8_t *val) { 
    size_t size = sizeof(KEYWORDS) / sizeof(uint8_t *);
    return strcontains(KEYWORDS, size, val);
}

static bool strcontains(const uint8_t **strings, size_t size, uint8_t *s) {
    size_t i = 0;
    
    while (i < size && strcmp(strings[i], s) != 0) {
	    ++i;
    }

    return i != size;
}

bool isatomic(uint8_t seq) {
    size_t size = sizeof(ATOMICS) / sizeof(uint8_t);
    size_t i = 0;

    while (i < size && ATOMICS[i] != seq) {
	    ++i;
    }
    
    return i != size;
}


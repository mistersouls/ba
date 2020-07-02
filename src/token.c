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
    if (isid(val))			return TOKEN_ID;
    if (istype(val))			return TOKEN_TYPE;
    if (strcmp(val, ":") == 0)		return TOKEN_COLON;
    if (strcmp(val, ";") == 0)		return TOKEN_SEMICOLON;
    if (strcmp(val, "=") == 0)		return TOKEN_ASSIGNMENT;
    if (isvalue(val))			return TOKEN_VALUE;
    if (strcmp(val, "print") == 0)	return TOKEN_PRINT;

    return TOKEN_UNKNOWN;
}

uint8_t *stringof_type(TokenType type) {
    switch (type) {
	case TOKEN_ID:			return "ID";
	case TOKEN_TYPE:		return "TYPE";
	case TOKEN_COLON:		return "COLON";
	case TOKEN_SEMICOLON:		return "SEMICOLON";
	case TOKEN_ASSIGNMENT:		return "ASSIGNMENT";
	case TOKEN_VALUE:		return "VALUE";
	case TOKEN_PRINT:		return "PRINT";
	default: 			return "UNKNOWN";
    }
}

bool isid(uint8_t *val) {
    size_t i = 0;
    uint8_t *tmp = val; 
   
    if (iskeyword(val)) {
	return false;
    }

    if (*tmp == '_' || isalpha(*tmp)) {
	++i;
	++tmp;
    } else {
	return false;
    }

    while (i < strlen(val) && (*tmp == '_' || isalnum(*tmp))) {
	++i;
	++tmp;
    }

    return strlen(val) == i;
    
}

bool istype(uint8_t *val) {
    size_t size = sizeof(TYPES) / sizeof(uint8_t *);
    return strcontains(TYPES, size, val);
}

bool isvalue(uint8_t *val) {
    // TODO: check others value type like string and boolean
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


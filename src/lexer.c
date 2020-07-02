#include "../include/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

uint8_t *read_entry(char *filename) {
    FILE *f = fopen(filename, "r");
    // TODO: Handle size malloc
    uint8_t *content = malloc(sizeof(2048));
    uint8_t *tmp = content;
    char c;
    
    if (f != NULL) {
	while ((c = (char) fgetc(f)) != EOF) {
	    *tmp++ = c;
	}
    } else {
	printf("Unable to open file %s.\n", filename);
	exit(1);
    }

    *tmp = '\0';

    return content;
}

Tokens *new_tokens() {
    Tokens *tokens = malloc(sizeof(*tokens));
    if (tokens == NULL) {
	printf("Unable to allocate memory while initializing tokens.\n");
	exit(1);
    }

    tokens->prev = NULL;

    return tokens;
}

void add_token(Tokens **tokens, uint8_t *val, size_t size_val) {
    // TODO: handle val token since size_val
    Token token = init_token(val, size_val);
    Tokens *new = new_tokens();
    new->token = token;

    new->prev = *tokens;
    *tokens = new;
}

void display_tokens(Tokens *tokens) {
    Tokens *current = tokens;

    printf("<Tokens [");

    while (current->prev != NULL) {
	printf("{token = '%s', type = %s}",
	    current->token.val, stringof_type(current->token.type));
	current = current->prev;
	if (current->prev != NULL) {
	    printf(", ");
	}
    }

    printf("]>");
}



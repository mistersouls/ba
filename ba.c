#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"




void usage() {
    printf("usage: ba <filename>\n");
    exit(1);
}


int main(int argc, char **argv) {
    char *filename;
    uint8_t *program;

    if (argc != 2) {
	usage();
    }

    filename = argv[1];
    program = read_entry(filename);
    Tokens *tokens = new_tokens();
    tokenize(&tokens, program);

    /*add_token(&tokens, "x", 1);
    add_token(&tokens, ":", 1);
    add_token(&tokens, "number", 6);
    add_token(&tokens, ";", 1);
    add_token(&tokens, "x: number;", 1);
    add_token(&tokens, "12", 2);*/

    display_tokens(tokens);
    printf("\n");

    free(program);
    
    return 0;
}


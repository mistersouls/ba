#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"




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
    display_tokens(tokens);
    printf("\n");

    AST *ast = parse(&tokens);
    display_tree(ast);
    printf("\n");

    free(program);
    
    return 0;
}


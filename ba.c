#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/binder.h"




void usage() {
    printf("usage: ba <cmd> <filename>\n");
    printf("cmd option:\n\ttokenize: tokenize the given file.\n\t");
    printf("parse: Get the AST and check the grammar syntax.\n");
    exit(1);
}

void cmd_tokenize(char *filename) {
    uint8_t *program = read_entry(filename);
    Tokens *tokens = new_tokens();
    tokenize(&tokens, program);
    display_tokens(tokens);
    printf("\n");
}

void cmd_parse(char *filename) {
    uint8_t *program = read_entry(filename);
    Tokens *tokens = new_tokens();
    tokenize(&tokens, program);
    AST *ast = parse(&tokens);
    display_tree(ast);
    printf("\n");
}

int main(int argc, char **argv) {

    if (argc != 3) {
	usage();
    }

    char *cmd = argv[1];
    char *filename = argv[2];

    if (strcmp(cmd, "parse") == 0) {
	cmd_parse(filename);
    } else if (strcmp(cmd, "tokenize") == 0) {
	cmd_tokenize(filename);
    } else {
	usage();
    }
    
    return 0;
}


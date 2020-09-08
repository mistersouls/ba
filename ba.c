#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/binder.h"
#include "include/compiler.h"




void usage() {
    printf("usage: ba <cmd> <filename>\n");
    printf("cmd option:\n\ttokenize: tokenize the given file.\n\t");
    printf("parse: Get the AST and check the grammar syntax.\n\t");
    printf("bind: Get the stackframe and check the semantique syntax.\n\t");
    printf("disassemble: Disassemble the entry program.\n\t");
    printf("dump: Get bytecode onto the console by the given entry program.\n\t");
    printf("compile: Compile the entry program.\n");
    exit(1);
}

Tokens *__tokenize(char *filename) {
    uint8_t *program = read_entry(filename);
    Tokens *tokens = new_tokens();
    tokenize(&tokens, program);

    return tokens;
}

AST *__parse(char *filename) {
    Tokens *tokens = __tokenize(filename);
    AST *ast = parse(&tokens);

    return ast;
}

Stackframe *__bind(char *filename) {
    AST *ast = __parse(filename);
    Stackframe *sf = bind(ast);

    return sf;
}

Bytecode *__compile(char *filename) {
    Tokens *tokens = __tokenize(filename);
    AST *ast = parse(&tokens);
    Stackframe *sf = bind(ast);
    Bytecode *bytecode = compile(ast, sf);

    return bytecode;
}

char *outfile(char *filename) {
    size_t i = strlen(filename) - 1;

    if (!(filename[i] == 'a' && filename[i - 1] == 'b' && filename[i - 2] == '.')) {
	printf("Entry program must have *.ba format.\n");
	exit(1);
    }

    filename[i] = 'b';
    filename[i - 1] = 's';

    return filename;
}

int main(int argc, char **argv) {

    if (argc != 3) {
	usage();
    }

    char *cmd = argv[1];
    char *filename = argv[2];

    if (strcmp(cmd, "compile") == 0) {
	Bytecode *bytecode = __compile(filename);
	char *bytefile = outfile(filename);
	dump(bytecode, bytefile);
    }else if (strcmp(cmd, "dump") == 0) {
	Bytecode *bytecode = __compile(filename);
	dumps(bytecode);
	printf("\n");
    } else if (strcmp(cmd, "disassemble") == 0) {
	Bytecode *bytecode = __compile(filename);
	disassemble(bytecode);
	printf("\n");
    } else if (strcmp(cmd, "bind") == 0) {
	Stackframe *sf = __bind(filename);
	display_frame(sf);
	printf("\n");
    } else if (strcmp(cmd, "parse") == 0) {
	AST *ast = __parse(filename);
	display_tree(ast);
        printf("\n");
    } else if (strcmp(cmd, "tokenize") == 0) {
	Tokens *tokens = __tokenize(filename);
	display_tokens(tokens);
        printf("\n");	
    } else {
	usage();
    }
    
    return 0;
}


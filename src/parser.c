#include "../include/parser.h"
#include <stdio.h>


AST *parse(Tokens **tokens) {
    AST *ast;
    construct(&ast, tokens);
    return ast;
}

void display_tree(AST *ast) {
    //NOTE: just helping to debug declaration
    // TODO: implement display tree by using visitor pattern.

    switch(ast->type) {
	case INSTRUCTION_DECLARATION:
	    printf("AST {declaration {id = \"%s\", type = %d}}",
		ast->declaration->id->value, ast->declaration->type->value);
	default: break;

    }

}


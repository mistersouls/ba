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
	    break;
	case INSTRUCTION_ASSIGNMENT:
	    printf("AST {assignment {id = \"%s\", expression = ?}}",
		ast->assignment->id->value);
	    break;
	case INSTRUCTION_BFC:
	    printf("AST {bfc {bf = %d, expression = ?}}",
		ast->bfc->type);
	    break;
	default: break;

    }

}


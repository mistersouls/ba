#include "../include/parser.h"
#include <stdio.h>
#include "../include/tree_json.h"


AST *parse(Tokens **tokens) {
    AST *ast = new_ast();
    ast->next = NULL;
    construct(&ast, tokens);
    return ast;
}

void display_tree_old(AST *ast) {
    //NOTE: just helping to debug declaration
    // TODO: implement display tree by using visitor pattern.
    
    while (ast != NULL) {
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

    ast = ast->next;
    }
}

void display_tree(AST *ast) {
    Visitor visitor = TREE_JSON;
    tjvisit_ast(visitor, ast, 0, NULL);    
}


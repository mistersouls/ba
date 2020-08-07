#include "../include/tree_json.h"
#include <stdio.h>
#include "../include/parser.h"

void tjvisit_ast(Visitor self, AST *ast, uint8_t argc, void **argv) {
    printf("[");

    while (ast != NULL) {
	switch(ast->type) {
	    case INSTRUCTION_DECLARATION:
		printf("\"type\":\"DECLARATION\",\"declaration\":");
		ast
		 ->declaration
		 ->node
		 ->accept(ast->declaration, self, argc, argv);
		break;
	    case INSTRUCTION_ASSIGNMENT:
		break;
	    case INSTRUCTION_BFC:
		break;
	    default:
		printf("{\"type\":\"unknown\"}");
	}

	printf("}");

	if (ast->next != NULL) {
	    printf(",");
	}
	
	ast = ast->next;

    }

    printf("]");
}

void tjvisit_declaration(Visitor self, Declaration *declaration, uint8_t argc, void **argv) {

}

void tjvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv) {

}

void tjvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv) {

}

void tjvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv) {

}

void tjvisit_type(Visitor self, Type *type, uint8_t argc, void **argv) {

}

void tjvisit_id(Visitor self, Id *id, uint8_t argc, void **argv) {

}

void tjvisit_number(Visitor self, Number *number, uint8_t argc, void **argv) {

}


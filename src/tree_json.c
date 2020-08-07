#include "../include/tree_json.h"
#include <stdio.h>
#include "../include/parser.h"

void tjvisit_ast(Visitor self, AST *ast, uint8_t argc, void **argv) {
    printf("[");

    while (ast != NULL) {
	switch(ast->type) {
	    case INSTRUCTION_DECLARATION:
		printf("{\"type\":\"DECLARATION\",\"declaration\":");
		ast
		 ->declaration
		 ->node
		 ->accept(ast->declaration, self, argc, argv);
		break;
	    case INSTRUCTION_ASSIGNMENT:
		printf("{\"type\":\"ASSIGNMENT\",\"assignment\":");
		ast
		 ->assignment
		 ->node
		 ->accept(ast->assignment, self, argc, argv);
		break;
	    case INSTRUCTION_BFC:
		printf("{\"type\":\"BFC\",\"bfc\":");
		ast
		 ->bfc
		 ->node
		 ->accept(ast->bfc, self, argc, argv);
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
    printf("{\"id\":");
    declaration
	->id
	->node
	->accept(declaration->id, self, argc, argv);
    printf(",\"type\":");
    declaration
	->type
	->node
	->accept(declaration->type, self, argc, argv);
    printf("}");
}

void tjvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv) {
    printf("{\"id\":");
    assignment
	->id
	->node
	->accept(assignment->id, self, argc, argv);
    printf(",\"expression\":");
    assignment
	->expression
	->node
	->accept(assignment->expression, self, argc, argv);
    printf("}");
}

void tjvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv) {
    switch(bfc->type) {
	case BUILTIN_PRINT:
	    printf("{\"type\":\"print\"");
	    break;
	default:
	    printf("{\"type\":\"unknown\"}");
	    return;
    }

    printf(",\"expression\":");
    bfc
     ->expression
     ->node
     ->accept(bfc->expression, self, argc, argv);
    printf("}");
}

void tjvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv) {
    switch(expression->type) {
	case EXPRESSION_NUMBER:
	    printf("{\"type\":\"NUMBER\",\"number\":");
	    expression
		->number
		->node
		->accept(expression->number, self, argc, argv);
	    printf("}");
	    break;
	case EXPRESSION_ID:
	    printf("{\"type\":\"ID\",\"id\":");
	    expression
		->id
		->node
		->accept(expression->id, self, argc, argv);
	    printf("}");
	    break;
	default:
	    printf("{\"type\":\"unknown\"}");
    }
}

void tjvisit_type(Visitor self, Type *type, uint8_t argc, void **argv) {
    switch(type->value) {
	case TYPE_NUMBER:
	    printf("\"number\"");
	    break;
	case TYPE_STRING:
	    printf("\"string\"");
	    break;
	case TYPE_BOOLEAN:
	    printf("\"boolean\"");
	    break;
	default:
	    printf("\"unknown\"");
    }
}

void tjvisit_id(Visitor self, Id *id, uint8_t argc, void **argv) {
    printf("\"%s\"", id->value);
}

void tjvisit_number(Visitor self, Number *number, uint8_t argc, void **argv) {
    switch(number->type) {
	case NUMBER_INTEGER:
	    printf("%d", number->integer);
	    break;
	default:
	    printf("\"unknown\"");
	    break;
    }
}


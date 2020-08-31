#include "../include/compiler.h"
#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>


Bytecode *compile(AST *ast, Stackframe *sf) {
    Bytecode *bytecode = NULL;
    Visitor self = COMPILER;
    void *argv[] = {&bytecode, &sf};
    cvisit_ast(self, ast, 2, argv);

    /** This a test*/
    push(&bytecode, ILOAD_VAR, 0, true);
    push(&bytecode, ILOAD_CONST, 1, true);
    push(&bytecode, ISTORE, 0, true);
    push(&bytecode, ILOAD_VAR, 0, true);
    push(&bytecode, IECHO, -1, false);

    /* end test */

    return bytecode;
}

void disassemble(Bytecode *bytecode) {
    reverse(&bytecode);

    while (bytecode != NULL) {
	printf("%-16s", OPNAMES[bytecode->opcode]);
	if (bytecode->hasarg) {
	    printf(" %d", bytecode->oparg);
	}
	printf("\n");

	bytecode = bytecode->prev;
    }
}

void dumps(Bytecode *bytecode) {

}

void dump(Bytecode *bytecode, char *filename) {

}

Bytecode *new_bytecode(Opcode opcode, int32_t oparg, bool hasarg) {
    Bytecode *bytecode = malloc(sizeof(Bytecode));
    if (bytecode == NULL) {
	printf("Unable to allocate memory while initializing bytecode.\n");
	exit(1);
    }

    bytecode->opcode = opcode;
    bytecode->oparg = oparg;
    bytecode->hasarg = hasarg;
    bytecode->prev = NULL;

    return bytecode;
}

void push(Bytecode **bytecode, Opcode opcode, int32_t oparg, bool hasarg) {
    Bytecode *new = new_bytecode(opcode, oparg, hasarg);
    new->prev = *bytecode;
    *bytecode = new;
}

Bytecode pop(Bytecode **bytecode) {

}

static void reverse(Bytecode **bytecode) {
    Bytecode *current = *bytecode;
    Bytecode *next = NULL;
    Bytecode *prev = NULL;

    while (current != NULL) {
	prev = current->prev;
	current->prev = next;
	next = current;
	current = prev;
    }

    *bytecode = next;
}
       
void cvisit_ast(Visitor self, AST *ast, uint8_t argc, void **argv) {

}

void cvisit_declaration(Visitor self, Declaration *declaration, uint8_t argc, void **argv) {

}

void cvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv) {

}

void cvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv) {

}

void cvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv) {

}

void cvisit_type(Visitor self, Type *type, uint8_t argc, void **argv) {

}

void cvisit_id(Visitor self, Id *id, uint8_t argc, void **argv) {

}

void cvisit_number(Visitor self, Number *number, uint8_t argc, void **argv) {

}


#include "../include/compiler.h"
#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>


Bytecode *compile() {

}

void disassemble(Bytecode *bytecode) {

}

void dumps(Bytecode *bytecode) {

}

void dump(Bytecode *bytecode, char *filename) {

}

Bytecode *new_bytecode(Opcode opcode, int32_t oparg, bool hasarg) {

}

void push(Bytecode **bytecode, Opcode opcode, int32_t oparg, bool hasarg) {

}

Bytecode pop(Bytecode **bytecode) {

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


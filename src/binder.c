#include "../include/binder.h"
#include <stdlib.h>
#include <stdio.h>
#include "../include/parser.h"



Stackframe *bind(AST *ast) {
    Stackframe *sf = NULL;


    return sf;
}

void display_frame(Stackframe *sf) {

}

Stackframe *new_stackframe() {

    return NULL;
}

Dataframe *new_dataframe(uint8_t *id, uint8_t type, uint32_t addr) {
    return NULL;
}

void open_frame(Stackframe **sf) {

}

Dataframe *find_data(Stackframe *sf, uint8_t *id) {

    return NULL;
}

Dataframe *findall_data(Stackframe *sf, uint8_t *id) {
    return NULL;
}

void put_data(Stackframe **sf, uint8_t *id, uint8_t type, uint32_t addr) {

}

void bvisit_ast(Visitor self, AST *ast, uint8_t argc, void **argv) {

}

void bvisit_declaration(Visitor self, Declaration *declaration, uint8_t argc, void **argv) {

}

void bvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv) {

}

void bvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv) {

}

void bvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv) {

}

void bvisit_type(Visitor self, Type *type, uint8_t argc, void **argv) {

}

void bvisit_id(Visitor self, Id *id, uint8_t argc, void **argv) {

}

void bvisit_number(Visitor self, Number *number, uint8_t argc, void **argv) {

}


#ifndef BA_COMPILER_H
#define BA_COMPILER_H


#include <stdint.h>
#include <stdbool.h>
#include "visitor.h"
#include "binder.h"


typedef enum {
    NOP,
    ILOAD_CONST,
    ILOAD_VAR,
    ISTORE,
    IECHO,
    HALT

} Opcode;

typedef struct Bytecode {
    Opcode opcode;
    int32_t oparg;
    bool hasarg;

    struct Bytecode *prev;

} Bytecode;

static const char *OPNAMES[] = {
    [NOP] = "NOP",
    [ILOAD_CONST] = "ILOAD_CONST",
    [ILOAD_VAR] = "ILOAD_VAR",
    [ISTORE] = "ISTORE",
    [IECHO] = "IECHO",
    [HALT] = "HALT"
};


Bytecode *compile();
void disassemble(Bytecode *bytecode);
void dumps(Bytecode *bytecode);
void dump(Bytecode *bytecode, char *filename);
Bytecode *new_bytecode(Opcode opcode, int32_t oparg, bool hasarg);
void push(Bytecode **bytecode, Opcode opcode, int32_t oparg, bool hasarg);
Bytecode pop(Bytecode **bytecode);


void cvisit_ast(Visitor self, AST *ast, uint8_t argc, void **argv);
void cvisit_declaration(Visitor self, Declaration *declaration, uint8_t argc, void **argv);
void cvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv);
void cvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv);
void cvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv);
void cvisit_type(Visitor self, Type *type, uint8_t argc, void **argv);
void cvisit_id(Visitor self, Id *id, uint8_t argc, void **argv);
void cvisit_number(Visitor self, Number *number, uint8_t argc, void **argv);


#endif // BA_COMPILER_H

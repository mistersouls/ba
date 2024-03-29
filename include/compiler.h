#ifndef BA_COMPILER_H
#define BA_COMPILER_H


#include <stdint.h>
#include <stdbool.h>
#include "visitor.h"
#include "binder.h"


#define COMPILER {                                                             \
        .visit_ast              =       cvisit_ast,                            \
        .visit_declaration      =       cvisit_declaration,                    \
        .visit_assignment       =       cvisit_assignment,                     \
        .visit_expression       =       cvisit_expression,                     \
        .visit_bfc              =       cvisit_bfc,                            \
        .visit_type             =       cvisit_type,                           \
        .visit_id               =       cvisit_id,                             \
        .visit_number           =       cvisit_number,                         \
        .visit_factor		    =	    cvisit_factor,					       \
	    .visit_term			    =	    cvisit_term,					       \
	    .visit_operation	    =	    cvisit_operation				       \
}               




typedef enum {
    NOP,
    ILOAD_CONST,
    LOAD_M1,
    ILOAD_VAR,
    ISTORE,
    ADD,
    SUB,
    MUL,
    DIV,
    IECHO,
    HALT = 255

} Opcode;

typedef struct Bytecode {
    Opcode opcode;
    int32_t oparg;
    bool hasarg;

    struct Bytecode *prev;

} Bytecode;

static const char *OPNAMES[] = {
    [NOP]                       = "NOP",
    [ILOAD_CONST]               = "ILOAD_CONST",
    [LOAD_M1]                   = "LOAD_M1",
    [ILOAD_VAR]                 = "ILOAD_VAR",
    [ISTORE]                    = "ISTORE",
    [ADD]                       = "ADD",
    [SUB]                       = "SUB",
    [MUL]                       = "MUL",
    [DIV]                       = "DIV",
    [IECHO]                     = "IECHO",
    [HALT]                      = "HALT"
};


Bytecode *compile(AST *ast, Stackframe *sf);
void disassemble(Bytecode *bytecode);
void dumps(Bytecode *bytecode);
void dump(Bytecode *bytecode, char *filename);
Bytecode *new_bytecode(Opcode opcode, int32_t oparg, bool hasarg);
void push(Bytecode **bytecode, Opcode opcode, int32_t oparg, bool hasarg);
Bytecode pop(Bytecode **bytecode);

static void reverse(Bytecode **bytecode);
static int32_t compress_byte(int32_t i);


void cvisit_ast(Visitor self, AST *ast, uint8_t argc, void **argv);
void cvisit_declaration(Visitor self, Declaration *declaration, uint8_t argc, void **argv);
void cvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv);
void cvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv);
void cvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv);
void cvisit_type(Visitor self, Type *type, uint8_t argc, void **argv);
void cvisit_id(Visitor self, Id *id, uint8_t argc, void **argv);
void cvisit_number(Visitor self, Number *number, uint8_t argc, void **argv);
void cvisit_factor(Visitor self, Factor *factor, uint8_t argc, void **argv); 
void cvisit_term(Visitor self, Term *term, uint8_t argc, void **argv); 
void cvisit_operation(Visitor self, Operation *operation, uint8_t argc, void **argv);


#endif // BA_COMPILER_H

#ifndef BA_VISITABLE_H
#define BA_VISITABLE_H

#include "visitor.h"
#include <stdint.h>

typedef void (*accept)(void *ptr, Visitor visitor, uint8_t argc, void **argv);

void accept_ast(AST *ast, Visitor visitor, uint8_t argc, void **argv);
void accept_declaration(Declaration *declaration, Visitor visitor, uint8_t argc, void **argv);
void accept_assignment(Assignment *assignment, Visitor visitor, uint8_t argc, void **argv);
void accept_bfc(BuiltinFuncCall *bfc, Visitor visitor, uint8_t argc, void **argv);
void accept_expression(Expression *expression, Visitor visitor, uint8_t argc, void **argv);
void accept_type(Type *type, Visitor visitor, uint8_t argc, void **argv);
void accept_id(Id *id, Visitor visitor, uint8_t argc, void **argv);
void accept_number(Number *number, Visitor visitor, uint8_t argc, void **argv);
void accept_factor(Factor *factor, Visitor visitor, uint8_t argc, void **argv);
void accept_term(Term *term, Visitor visitor, uint8_t argc, void **argv);
void accept_operation(Operation *operation, Visitor visitor, uint8_t argc, void **argv);



#endif // BA_VISITABLE_H

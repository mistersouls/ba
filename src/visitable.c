#include "../include/visitable.h"

void accept_ast(AST *ast, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_ast(visitor, ast, argc, argv);
}

void accept_declaration(Declaration *declaration, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_declaration(visitor, declaration, argc, argv);
}

void accept_assignment(Assignment *assignment, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_assignment(visitor, assignment, argc, argv);
}

void accept_bfc(BuiltinFuncCall *bfc, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_bfc(visitor, bfc, argc, argv);
}

void accept_expression(Expression *expression, Visitor visitor, uint8_t argc, void **argv) { 
    visitor.visit_expression(visitor, expression, argc, argv);
}

void accept_type(Type *type, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_type(visitor, type, argc, argv);
}

void accept_id(Id *id, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_id(visitor, id, argc, argv);
}

void accept_number(Number *number, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_number(visitor, number, argc, argv);
}

void accept_factor(Factor *factor, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_factor(visitor, factor, argc, argv);
}

void accept_term(Term *term, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_term(visitor, term, argc, argv);
}

void accept_operation(Operation *operation, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_operation(visitor, operation, argc, argv);
}

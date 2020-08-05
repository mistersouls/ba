#include "../include/visitable.h"

void accept_ast(AST *ast, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_ast(visitor, ast, argc, argv);
}

void accept_declaration(Declaration *declaration, Visitor visitor, uint8_t argc, void **argv) {
    visitor.visit_declaration(visitor, declaration, argc, argv);
}


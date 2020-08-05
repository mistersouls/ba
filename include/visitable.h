#ifndef BA_VISITABLE_H
#define BA_VISITABLE_H

#include "visitor.h"
#include <stdint.h>

typedef void (*accept)(void *ptr, Visitor visitor, uint8_t argc, void **argv);

void accept_ast(AST *ast, Visitor visitor, uint8_t argc, void **argv);
void accept_declaration(Declaration *declaration, Visitor visitor, uint8_t argc, void **argv);


#endif // BA_VISITABLE_H

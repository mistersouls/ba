#ifndef BA_VISITOR_H
#define BA_VISITOR_H

#include <stdint.h>

typedef struct AST AST;
typedef struct Declaration Declaration;

typedef struct Visitor Visitor;
typedef void (*visit)(Visitor self, void *ptr, uint8_t argc, void **argv);


struct Visitor {
    void (*visit_ast)(Visitor self, AST *ast, uint8_t argc, void **argv);
    void (*visit_declaration)(Visitor self, Declaration *declaration, uint8_t argc, void **argv);
};


#endif // BA_VISITOR_H

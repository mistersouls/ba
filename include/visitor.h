#ifndef BA_VISITOR_H
#define BA_VISITOR_H

#include <stdint.h>

typedef struct AST AST;
typedef struct Declaration Declaration;
typedef struct Assignment Assignment;
typedef struct BuiltinFuncCall BuiltinFuncCall;
typedef struct Expression Expression;
typedef struct Id Id;
typedef struct Type Type;
typedef struct Number Number;
typedef struct Factor Factor;
typedef struct Term Term;
typedef struct Operation Operation;


typedef struct Visitor Visitor;
typedef void (*visit)(Visitor self, void *ptr, uint8_t argc, void **argv);


struct Visitor {
    void (*visit_ast)(Visitor self, AST *ast, uint8_t argc, void **argv);
    void (*visit_declaration)(Visitor self, Declaration *declaration, uint8_t argc, void **argv);
    void (*visit_assignment)(Visitor self, Assignment *assignment, uint8_t argc, void **argv);
    void (*visit_bfc)(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv);
    void (*visit_expression)(Visitor self, Expression *expression, uint8_t argc, void **argv);
    void (*visit_type)(Visitor self, Type *type, uint8_t argc, void **argv);
    void (*visit_id)(Visitor self, Id *id, uint8_t argc, void **argv);
    void (*visit_number)(Visitor self, Number *number, uint8_t argc, void **argv);
    void (*visit_factor)(Visitor self, Factor *factor, uint8_t argc, void **argv);
    void (*visit_term)(Visitor self, Term *term, uint8_t argc, void **argv);
    void (*visit_operation)(Visitor self, Operation *operation, uint8_t argc, void **argv);
};


#endif // BA_VISITOR_H

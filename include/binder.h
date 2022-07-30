#ifndef BA_BINDER_H
#define BA_BINDER_H

#include <stdint.h>
#include "visitor.h"

#define BINDER  {                                                    	       \
        .visit_ast              =       bvisit_ast,                            \
        .visit_declaration      =       bvisit_declaration,                    \
        .visit_assignment       =       bvisit_assignment,                     \
        .visit_expression       =       bvisit_expression,                     \
        .visit_bfc              =       bvisit_bfc,                            \
        .visit_type             =       bvisit_type,                           \
        .visit_id               =       bvisit_id,                             \
        .visit_number           =       bvisit_number,                         \
        .visit_factor		    =	    bvisit_factor,					       \
	    .visit_term			    =	    bvisit_term,					       \
	    .visit_operation	    =	    bvisit_operation				       \
}               


typedef struct Dataframe {
    uint8_t *id;
    uint8_t type;
    uint32_t addr;

    struct Dataframe *prev;

} Dataframe;

typedef struct Stackframe {
    Dataframe *df;

    struct Stackframe *prev;

} Stackframe;


Stackframe *bind(AST *ast);
void display_frame(Stackframe *sf);
Stackframe *new_stackframe();
Dataframe *new_dataframe(uint8_t *id, uint8_t type, uint32_t addr);
void open_frame(Stackframe **sf);
Dataframe *find_data(Stackframe *sf, uint8_t *id);
Dataframe *findall_data(Stackframe *sf, uint8_t *id);
void put_data(Stackframe **sf, uint8_t *id, uint8_t type, uint32_t addr);


void bvisit_ast(Visitor self, AST *ast, uint8_t argc, void **argv);
void bvisit_declaration(Visitor self, Declaration *declaration, uint8_t argc, void **argv);
void bvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv);
void bvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv);
void bvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv);
void bvisit_type(Visitor self, Type *type, uint8_t argc, void **argv);
void bvisit_id(Visitor self, Id *id, uint8_t argc, void **argv);
void bvisit_number(Visitor self, Number *number, uint8_t argc, void **argv);
void bvisit_factor(Visitor self, Factor *factor, uint8_t argc, void **argv); 
void bvisit_term(Visitor self, Term *term, uint8_t argc, void **argv); 
void bvisit_operation(Visitor self, Operation *operation, uint8_t argc, void **argv);


#endif // BA_BINDER_H

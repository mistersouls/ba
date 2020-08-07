#ifndef BA_TREE_JSON_H
#define BA_TREE_JSON_H

#include "visitor.h"

#define TREE_JSON  {								\
	.visit_ast		=	tjvisit_ast,				\
	.visit_declaration	=	tjvisit_declaration,			\
	.visit_assignment	=	tjvisit_assignment,			\
	.visit_expression	=	tjvisit_expression,			\
	.visit_bfc		=	tjvisit_bfc,				\
	.visit_type		=	tjvisit_type,				\
	.visit_id		=	tjvisit_id,				\
	.visit_number		=	tjvisit_number				\
}		


void tjvisit_ast(Visitor self, AST *ast, uint8_t argc, void **argv); 
void tjvisit_declaration(Visitor self, Declaration *declaration, uint8_t argc, void **argv); 
void tjvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv); 
void tjvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv); 
void tjvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv); 
void tjvisit_type(Visitor self, Type *type, uint8_t argc, void **argv); 
void tjvisit_id(Visitor self, Id *id, uint8_t argc, void **argv); 
void tjvisit_number(Visitor self, Number *number, uint8_t argc, void **argv); 

#endif // BA_TREE_JSON_H



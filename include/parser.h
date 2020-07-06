
#ifndef BA_PARSER_H
#define  BA_PARSER_H

#include <stdint.h>

typedef struct {

} Node;

typedef struct {
    Node *node;

    uint8_t *value;

} Id;

typedef struct {
    Node *node;

    enum {
	TYPE_NUMBER,
	TYPE_STRING,
	TYPE_BOOLEAN
    } value;
} Type;

typedef struct {
    Node *node;

    int32_t value;
} Number;

typedef struct {
    Node *node;
    
    enum {
	EXPRESSION_ID,
	EXPRESSION_NUMBER
    } type;


    union {
	Id *id;
	Number *number;
    };

} Expression;

typedef struct {
    Node *node;

    Id *id;
    Type *type;

} Declaration;

typedef struct {
    Node *node;

    Id *id;
    Expression *expression;
} Assignment;

typedef struct {
    Node *node;

    enum {
	BUILTIN_PRINT
    } type;

    Expression *expression;

} BuiltinFuncCall;

typedef struct AST {
    
    enum {
	INSTRUCTION_DECLARATION,
	INSTRUCTION_ASSIGNMENT,
	INSTRUCTION_BFC
    } type;

    union {
	Declaration *declaration;
	Assignment *assignment;
	BuiltinFuncCall *bfc;
    };

    struct AST *next;
} AST;

#endif // BA_PARSER_H

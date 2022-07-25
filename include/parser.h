
#ifndef BA_PARSER_H
#define  BA_PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include "lexer.h"
#include "visitable.h"

/**

id: [a-zA-Z_][a-zA-Z0-9_]*

type: 'string' | 'number' | 'boolean'

declaration: {id} ':' {type} ';'

number: [0-9]+

factor: ["-"] ({number} | {id}) | "(" {operation} ")"

term: {factor} ("*" | "/") {term} | {factor}

operation: {term} ("+" | "-") {operation} | {term}

expression: {string} | {boolean} | {operation}

assignment: {id} '=' {expression}

print: 'print' {expression} ';'

*/


typedef struct {
    accept accept;
} Node;

typedef struct Id {
    Node *node;

    uint8_t *value;

} Id;

typedef struct Type {
    Node *node;

    enum {
        TYPE_NUMBER,
        TYPE_STRING,
        TYPE_BOOLEAN
    } value;
} Type;

typedef struct Number {
    Node *node;

    enum  {
        NUMBER_INTEGER,
    //	NUMBER_REAL
    } type;

    union {
        int32_t integer;
    //	double real;
    };

} Number;

typedef struct Factor {
    Node *node;

    enum {
        FACTOR_NUMBER,
        FACTOR_ID,
        FACTOR_OPERATION
    } type;

    union {
        struct {
            bool minus;
            Number *this;
        } *number;

        struct {
            bool minus;
            Id *this;
        } *id;

        Operation *operation;
    };
} Factor;

typedef struct Term {
    Node *node;

    enum {
        TERM_FACTOR,
        TERM_THIS
    } type;

    union {
        Factor *factor;

        struct {
            Factor *factor;
            Token operator;
            Term *term;
        } *this;
    };
} Term;

typedef struct Operation {
    Node *node;

    enum {
        OPERATION_TERM,
        OPERATION_THIS
    } type;

    union {
        Term *term;

        struct {
            Term *term;
            Token operator;
            Operation *operation;
        } *this;
    };
} Operation;

typedef struct Expression {
    Node *node;
    
    enum {
        EXPRESSION_ID, /* @deprecated */
        EXPRESSION_NUMBER, /* @deprecated */
        EXPRESSION_OPERATION
    } type;


    union {
        Id *id; /* @deprecated */
        Number *number; /* @deprecated */
        Operation *operation;
    };

} Expression;

typedef struct Declaration {
    Node *node;

    Id *id;
    Type *type;

} Declaration;

typedef struct Assignment {
    Node *node;

    Id *id;
    Expression *expression;
} Assignment;

typedef struct BuiltinFuncCall {
    Node *node;

    enum {
	    BUILTIN_PRINT
    } type;

    Expression *expression;

} BuiltinFuncCall;

typedef struct AST {
    Node *node;

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

AST *parse(Tokens **tokens);
void display_tree(AST *ast);

void construct(AST **ast, Tokens **tokens);
void construct_declaration(Declaration **declaration, Tokens **tokens);
void construct_assignment(Assignment **assignment, Tokens **tokens);
void construct_bfc(BuiltinFuncCall **bfc, Tokens **tokens);
void construct_expression(Expression **expression, Tokens **tokens);
void construct_id(Id **id, Tokens **tokens);
void construct_type(Type **type, Tokens **tokens);
void construct_number(Number **number, Tokens **tokens);
void construct_factor(Factor **factor, Tokens **tokens);
void construct_term(Term **term, Tokens **tokens);
void construct_operation(Operation **operation, Tokens **tokens);


AST *new_ast();
Declaration *new_declaration();
Assignment *new_assignment();
BuiltinFuncCall *new_bfc();
Expression *new_expression();
Id *new_id();
Type *new_type();
Number *new_number();
Factor *new_factor();
Term *new_term();
Operation *new_operation();

static void *new_node(void *ptr, uint8_t *err);
static bool isfactor(Token token);
static bool isexpression(Token token);
static bool isbuiltin(Token token);

#endif // BA_PARSER_H

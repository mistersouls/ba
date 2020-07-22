#include "../include/parser.h"
#include <stdio.h>
#include <string.h>

void construct(AST **ast, Tokens **tokens) {
    *ast = new_ast();
    Token token = pop_token(tokens);

    if (token.type == TOKEN_SEMICOLON) {
	Token current = peek_token(*tokens);
	Token previous = peek_prev_token(*tokens);

	if (current.type == TOKEN_TYPE && previous.type == TOKEN_COLON) {
	    (*ast)->type = INSTRUCTION_DECLARATION;
	    construct_declaration(&(*ast)->declaration, tokens);
	} else {
	    printf("Syntax error: Unrecognized instruction.\n");
	}
    } else {
	printf("Syntax error: expected semicolon at the end of instruction.\n");
    }
}


void construct_declaration(Declaration **declaration, Tokens **tokens) {
    *declaration = new_declaration();
    construct_type(&(*declaration)->type, tokens);
    Token token = pop_token(tokens);

    if (token.type == TOKEN_COLON) {
	construct_id(&(*declaration)->id, tokens);
    } else {
	printf("Syntax error: expected colon onto the declaration.\n");
    }
}

void construct_id(Id **id, Tokens **tokens) {
    *id = new_id();
    Token token = pop_token(tokens);

    if (token.type == TOKEN_ID) {
	(*id)->value = token.val;
    } else {
	printf("Syntax error: expecte an id.\n");
    }
}

void construct_type(Type **type, Tokens **tokens) {
    *type = new_type();
    Token token = pop_token(tokens);
    
    if (token.type == TOKEN_TYPE) {
	if (strcmp(token.val, "number") == 0) {
	    (*type)->value = TYPE_NUMBER;
	} else if (strcmp(token.val, "string") == 0) {
            (*type)->value = TYPE_STRING;
	} else if (strcmp(token.val, "boolean") == 0) {
            (*type)->value = TYPE_BOOLEAN;
	} else {
	    printf("Unexpected error: a bug was raised while constructing type.\n");
	}
    } else {
	printf("Syntax error: expected a type.\n");
    }
}



AST *new_ast() {
    uint8_t *msg = "Unable to allocate memory while constructing ast.\n";
    AST *ast = new_node(ast, msg);
    return ast;
}

Declaration *new_declaration() {
    uint8_t *msg = "Unable to allocate memory while constructing declaration.\n";
    Declaration *declaration = new_node(declaration, msg);
    return declaration;
}

Id *new_id() {
    uint8_t *msg = "Unable to allocate memory while constructing id.\n";
    Id *id = new_node(id, msg);
    return id;
}

Type *new_type() {
    uint8_t *msg = "Unable to allocate memory while constructing type.\n";
    Type *type = new_node(type, msg);
    return type;
}

static void *new_node(void *ptr, uint8_t *err) {
    ptr = malloc(sizeof(*ptr));
    if (ptr == NULL) {
	printf(err);
	exit(1);
    }

    return ptr;
}

void construct_number(Number **number, Tokens **tokens) {
    *number = new_number();
    Token token = pop_token(tokens);

    if (token.type == TOKEN_NUMBER) {
	// TODO: check the type of number
	(*number)->type = NUMBER_INTEGER;
	(*number)->integer = atoi(token.val);
    } else {
	printf("Syntax error: expected a number.\n");
    }
}

void construct_assignment(Assignment **assignment, Tokens **tokens) {
    *assignment = new_assignment();
    construct_expression(&(*assignment)->expression, tokens);
    Token token = pop_token(tokens);

    if (token.type = TOKEN_ASSIGNMENT) {
	construct_id(&(*assignment)->id, tokens);
    } else {
	printf("Syntax error: expected equal onto the assignment.\n");
    }
}

void construct_expression(Expression **expression, Tokens **tokens) {
    *expression = new_expression();
    Token token = peek_token(*tokens);

    if (token.type == TOKEN_NUMBER) {
	(*expression)->type = EXPRESSION_NUMBER;
	construct_number(&(*expression)->number, tokens);
    } else if (token.type == TOKEN_ID) {
	(*expression)->type = EXPRESSION_ID;
	construct_id(&(*expression)->id, tokens);
    } else {
	printf("Syntax error: an expression must be a number or an id.\n");
    }
}

Number *new_number() {
    uint8_t *msg = "Unable to allocate memory while constructing number.\n";
    Number *number = new_node(number, msg);
    return number;
}

Assignment *new_assignment() {
    uint8_t *msg = "Unable to allocate memory while constructing assignment.\n";
    Assignment *assignment = new_node(assignment, msg);
    return assignment;
}

Expression *new_expression() {
    uint8_t *msg = "Unable to allocate memory while constructing expression.\n";
    Expression *expression = new_node(expression, msg);
    return expression;
}


#include "../include/parser.h"
#include <stdio.h>
#include <string.h>

void construct(AST **ast, Tokens **tokens) {
    Token token = pop_token(tokens);

    if (token.type == TOKEN_SEMICOLON) {
        Token current = peek_token(*tokens);
        Token previous = peek_prev_token(*tokens);

        if (current.type == TOKEN_TYPE && previous.type == TOKEN_COLON) {
            (*ast)->type = INSTRUCTION_DECLARATION;
            construct_declaration(&(*ast)->declaration, tokens);
        } else if (isexpression(current)) {
            Expression *expression = new_expression();
            construct_expression(&expression, tokens);
            token = peek_token(*tokens);

            if (token.type == TOKEN_ASSIGNMENT) {
                (*ast)->type = INSTRUCTION_ASSIGNMENT;
                construct_assignment(&(*ast)->assignment, tokens);
                (*ast)->assignment->expression = expression;
            } else if (isbuiltin(token)) {
                (*ast)->type = INSTRUCTION_BFC;
                construct_bfc(&(*ast)->bfc, tokens);
                (*ast)->bfc->expression = expression;
            } else {
                printf("Syntax error: expected assignment or call.\n");
            }
        } else {
            printf("Syntax error: Unrecognized instruction.\n");
        }
    } else {
	    printf("Syntax error: expected semicolon at the end of instruction.\n");
    }

    token = peek_token(*tokens);
    if (token.type != TOKEN_UNKNOWN) {
        AST *new = new_ast();
        new->next = *ast;
        construct(&new, tokens);
        *ast = new;
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
    AST *ast = malloc(sizeof(AST));
    if (ast == NULL) {
	printf("%s", msg);
	exit(1);
    }
    ast->node = new_node(ast->node, msg);
    ast->node->accept = (accept) accept_ast;

    return ast;
}

Declaration *new_declaration() {
    uint8_t *msg = "Unable to allocate memory while constructing declaration.\n";
    Declaration *declaration = new_node(declaration, msg);
    declaration->node = new_node(declaration->node, msg);
    declaration->node->accept = (accept) accept_declaration;
    return declaration;
}

Id *new_id() {
    uint8_t *msg = "Unable to allocate memory while constructing id.\n";
    Id *id = new_node(id, msg);
    id->node = new_node(id->node, msg);
    id->node->accept = (accept) accept_id;
    return id;
}

Type *new_type() {
    uint8_t *msg = "Unable to allocate memory while constructing type.\n";
    Type *type = new_node(type, msg);
    type->node = new_node(type->node, msg);
    type->node->accept = (accept) accept_type;
    return type;
}

static void *new_node(void *ptr, uint8_t *err) {
    ptr = malloc(sizeof(*ptr));
    if (ptr == NULL) {
	printf("%s", err);
	exit(1);
    }

    return ptr;
}

static bool isfactor(Token token) {
    return token.type == TOKEN_ID           ||
           token.type == TOKEN_NUMBER       ||
           token.type == TOKEN_RPARENTHESIS;
}

static bool isexpression(Token token) {
    return isfactor(token);
}

static bool isbuiltin(Token token) {
    return token.type == TOKEN_PRINT;
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

void construct_factor(Factor **factor, Tokens **tokens) {
    Token previous;
    Token token = peek_token(*tokens);
    *factor = new_factor();

    switch (token.type) {
        case TOKEN_RPARENTHESIS:
            (*factor)->type = FACTOR_OPERATION;
            pop_token(tokens);
            construct_operation(&(*factor)->operation, tokens);
            token = pop_token(tokens);
            if (token.type != TOKEN_LPARENTHESIS) {
                printf("Syntax error: expected token: '('\n");
            }
            break;
        case TOKEN_ID:
            (*factor)->type = FACTOR_ID;
            construct_id(&(*factor)->id, tokens);
            break;
        case TOKEN_NUMBER:
            (*factor)->type = FACTOR_NUMBER;
            construct_number(&(*factor)->number, tokens);
            break;
        default:
            printf("Syntax error: expected number, id or ).\n");
            break;
    }

    token = peek_token(*tokens);
    previous = peek_prev_token(*tokens);
    if (!isfactor(previous) && token.type == TOKEN_MINUS) {
        (*factor)->minus = true;
        pop_token(tokens);
    }

}

void construct_term(Term **term, Tokens **tokens) {
    Token token;
    Factor *factor = new_factor();
    *term = new_term();

    construct_factor(&factor, tokens);
    token = peek_token(*tokens);

    if (token.type == TOKEN_MUL || token.type == TOKEN_DIV) {
        (*term)->type = TERM_THIS;
        (*term)->this = malloc(sizeof((*term)->this));
        if ((*term)->this == NULL) {
            printf("Unable to allocate memory while constructing term.\n");
            exit(0);
        }
        (*term)->this->factor = factor;
        (*term)->this->operator = pop_token(tokens);
        construct_term(&(*term)->this->term, tokens);
    } else {
        (*term)->type = TERM_FACTOR;
        (*term)->factor = factor;
    }
}

void construct_operation(Operation **operation, Tokens **tokens) {
    /** {operation} ("+" | "-") {term} | {term} */
    Token token;
    *operation = new_operation();
    Term *term = new_term();

    construct_term(&term, tokens);
    token = peek_token(*tokens);

    if (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS) {
        (*operation)->type = OPERATION_THIS;
        (*operation)->this = malloc(sizeof((*operation)->this));
        if ((*operation)->this == NULL) {
            printf("Unable to allocate memory while constructing operation.\n");
            exit(0);
        }
        (*operation)->this->term = term;
        (*operation)->this->operator = pop_token(tokens);
        construct_operation(&(*operation)->this->operation, tokens);
    } else {
        (*operation)->type = OPERATION_TERM;
        (*operation)->term = term;
    }
}

void construct_assignment(Assignment **assignment, Tokens **tokens) {
    *assignment = new_assignment();
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

    if (isfactor(token)) {
        (*expression)->type = EXPRESSION_OPERATION;
        construct_operation(&(*expression)->operation, tokens);
    } else if (token.type == TOKEN_NUMBER) { /** @deprecated */
	    (*expression)->type = EXPRESSION_NUMBER;
	    construct_number(&(*expression)->number, tokens);
    } else if (token.type == TOKEN_ID) { /** @deprecated */
        (*expression)->type = EXPRESSION_ID;
        construct_id(&(*expression)->id, tokens);
    } else {
	    printf("Syntax error: an expression must be a number or an id.\n");
    }
}

Number *new_number() {
    uint8_t *msg = "Unable to allocate memory while constructing number.\n";
    Number *number = new_node(number, msg);
    number->node = new_node(number->node, msg);
    number->node->accept = (accept) accept_number;
    return number;
}

Factor *new_factor() {
    uint8_t *msg = "Unable to allocate memory while constructing factor.\n";
    Factor *factor = new_node(factor, msg);
    factor->node = new_node(factor->node, msg);
    factor->node->accept = (accept) accept_factor;
    return factor; 
}

Term *new_term() {
    uint8_t *msg = "Unable to allocate memory while constructing term.\n";
    Term *term = new_node(term, msg);
    term->node = new_node(term->node, msg);
    term->node->accept = (accept) accept_term;
    return term;
}

Operation *new_operation() {
    uint8_t *msg = "Unable to allocate memory while constructing operation.\n";
    Operation *operation = new_node(operation, msg);
    operation->node = new_node(operation->node, msg);
    operation->node->accept = (accept) accept_operation;
    return operation;
}

Assignment *new_assignment() {
    uint8_t *msg = "Unable to allocate memory while constructing assignment.\n";
    Assignment *assignment = new_node(assignment, msg);
    assignment->node = new_node(assignment->node, msg);
    assignment->node->accept = (accept) accept_assignment;
    return assignment;
}

Expression *new_expression() {
    uint8_t *msg = "Unable to allocate memory while constructing expression.\n";
    Expression *expression = new_node(expression, msg);
    expression->node = new_node(expression->node, msg);
    expression->node->accept = (accept) accept_expression;
    return expression;
}

void construct_bfc(BuiltinFuncCall **bfc, Tokens **tokens) {
    *bfc = new_bfc();
    Token token = pop_token(tokens);

    if (isbuiltin(token)) {
        // TODO: check condition by type bfc
        (*bfc)->type = BUILTIN_PRINT;
    } else {
	    printf("Syntax error: undeclared %s function.\n", token.val);
    }
}

BuiltinFuncCall *new_bfc() {
    uint8_t *msg = "Unable to allocate memory while constructing builtin call function.\n";
    BuiltinFuncCall *bfc = new_node(bfc, msg);
    bfc->node = new_node(bfc->node, msg);
    bfc->node->accept = (accept) accept_bfc;
    return bfc;

}

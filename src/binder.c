#include "../include/binder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/parser.h"



Stackframe *bind(AST *ast) {
    Stackframe *sf = NULL;
    void *argv[] = {
	&sf
    };
    Visitor self = BINDER;
    bvisit_ast(self, ast, 1, argv);

    return sf;
}

void display_frame(Stackframe *sf) {
    printf("[");
    
    while(sf != NULL) {
	Dataframe *df = sf->df;
	printf("[");

	while (df != NULL) {
	    printf("{\"id\":\"%s\",\"type\":%d,\"addr\":%d}", df->id, df->type, df->addr);
	    
	    if (df->prev != NULL) {
		printf(",");
	    }

	    df = df->prev;
	}

	printf("]");

	if (sf->prev != NULL) {
	    printf(",");
	}

	sf = sf->prev;
    }

    printf("]");
}

Stackframe *new_stackframe() {
    Stackframe *sf = malloc(sizeof(Stackframe));
    if (sf == NULL) {
	printf("Unable to allocate memory while intializing stack frame.\n");
	exit(1);
    }
    sf->df = NULL;
    sf->prev = NULL;

    return sf;
}

Dataframe *new_dataframe(uint8_t *id, uint8_t type, uint32_t addr) {
    Dataframe *df = malloc(sizeof(Dataframe));
    if (df == NULL) {
	printf("Unable to allocate memory while initializing data frame.\n");
	exit(1);
    }
    df->id = id;
    df->type = type;
    df->addr = addr;
    df->prev = NULL;

    return df;
}

void open_frame(Stackframe **sf) {
    Stackframe *new = new_stackframe();
    new->df = NULL;
    new->prev = *sf;
    *sf = new;
}

Dataframe *find_data(Stackframe *sf, uint8_t *id) {
    Dataframe *df = sf->df;
    
    while (df != NULL && df->id != NULL) {
	if (strcmp(id, df->id) == 0) {
	    return df;
	}

	df = df->prev;
    }

    return NULL;
}

Dataframe *findall_data(Stackframe *sf, uint8_t *id) {
    while (sf != NULL) {
	Dataframe *df = find_data(sf, id);
	if (df != NULL) {
	    return df;
	}
	
	sf = sf->prev;
    }

    return NULL;
}

void put_data(Stackframe **sf, uint8_t *id, uint8_t type, uint32_t addr) {
    Dataframe *df = new_dataframe(id, type, addr);
    df->prev = (*sf)->df;
    (*sf)->df = df;
}

void bvisit_ast(Visitor self, AST *ast, uint8_t argc, void **argv) {
    if (argc < 1) {
	printf("Assertion error: argument must contain at least 1.\n");
	exit(1);
    }

    Stackframe **sf = argv[0];
    open_frame(sf);

    while (ast != NULL) {
	switch (ast->type) {
	    case INSTRUCTION_DECLARATION:
		ast
		 ->declaration
		 ->node
		 ->accept(ast->declaration, self, argc, argv);
		break;
	    case INSTRUCTION_ASSIGNMENT:
		ast
		 ->assignment
		 ->node
		 ->accept(ast->assignment, self, argc, argv);
		break;
	    case INSTRUCTION_BFC:
		ast
		 ->bfc
		 ->node
		 ->accept(ast->bfc, self, argc, argv);
		break;
	    default:
		printf("Unexpected error: a bug was raised while visiting binder ast.\n");
		exit(1);
	}

	ast = ast->next;
    }
}

void bvisit_declaration(Visitor self, Declaration *declaration, uint8_t argc, void **argv) {
    if (argc < 1) {
	printf("Assertion error: argument must contain at least 1.\n");
	exit(1);
    }

    Stackframe **sf = argv[0];
    uint8_t *id = declaration->id->value;
    uint8_t type = declaration->type->value;
    uint32_t addr = (*sf)->df == NULL ? 0 : (*sf)->df->addr + 1;
    Dataframe *found = find_data(*sf, id);
    
    if (found == NULL) {
	put_data(sf, id, type, addr);
    } else {
	printf("Semantic error: %s is already declarared.\n", id);
    }

}

void bvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv) {
	uint8_t type;
	Stackframe **sf;
    Dataframe *found;
	uint8_t *id = assignment->id->value;

    if (argc < 1) {
		printf("Assertion error: argument must contain at least 1.\n");
		exit(1);
    }

	switch (assignment->expression->type) {
		case EXPRESSION_OPERATION:
			type = TYPE_NUMBER;
			break;
		default:
			type = 255;
			break;
	}
    
    assignment
		->expression
		->node
		->accept(assignment->expression, self, argc, argv);
	sf = argv[0];
	found = findall_data(*sf, id);

    if (found != NULL) {
		if (found->type != type) {
			printf("Semantic error: type mismatch, expected %s type.\n", id);
		}
    } else {
		printf("Semantic error: %s is undeclared.\n", id);
    }

}

void bvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv) {
    if (argc < 1) {
		printf("Assertion error: argument must contain at least 1.\n");
		exit(1);
    }

    bfc
    	->expression
     	->node
     	->accept(bfc->expression, self, argc, argv);
}

void bvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv) {
    if (argc < 1) {
		printf("Assertion error: argument must contain at least 1.\n");
		exit(1);
    }

    switch (expression->type) {
		case EXPRESSION_OPERATION:
			expression
				->operation
				->node
				->accept(expression->operation, self, argc, argv);
			break;
		default:
			printf("Unexpected error: a bug was raised while visiting binder expression.\n");
			exit(1);
    }

}

void bvisit_factor(Visitor self, Factor *factor, uint8_t argc, void **argv) {
	Dataframe *found;
	Stackframe **sf;
	uint8_t *id;

	if (argc < 1) {
		printf("Assertion error: argument must contain at least 1.\n");
		exit(1);
    }

	sf = argv[0];

	switch (factor->type) {
		case FACTOR_NUMBER:
			/* Nothing to do */
			break;
		case FACTOR_OPERATION:
			factor
				->operation
				->node
				->accept(factor->operation, self, argc, argv);
			break;
		case FACTOR_ID: {
			id = factor->id->value;
			found = findall_data(*sf, id);
			if (found != NULL) {
				if (found->type != TYPE_NUMBER) {
					printf("Semantic error: expected type number for '%s'.\n", id);
				}
			} else {
				printf("Semantic error: %s is undeclared.\n", id);
			}
			break;
		}
		default:
			printf("Unexpected error: a bug was raised while visiting binder expression.\n");
			exit(1);
	}
}

void bvisit_term(Visitor self, Term *term, uint8_t argc, void **argv) {
	if (argc < 1) {
		printf("Assertion error: argument must contain at least 1.\n");
		exit(1);
    }

	switch (term->type) {
		case TERM_FACTOR:
			term
				->factor
				->node
				->accept(term->factor, self, argc, argv);
			break;
		case TERM_THIS:
			term
				->this
				->factor
				->node
				->accept(term->this->factor, self, argc, argv);
			term
				->this
				->term
				->node
				->accept(term->this->term, self, argc, argv);
			break;
		default:
			printf("Unexpected error: a bug was raised while visiting binder expression.\n");
			exit(1);
	}
}

void bvisit_operation(Visitor self, Operation *operation, uint8_t argc, void **argv) {
	if (argc < 1) {
		printf("Assertion error: argument must contain at least 1.\n");
		exit(1);
    }

	switch (operation->type) {
		case OPERATION_TERM:
			operation
				->term
				->node
				->accept(operation->term, self, argc, argv);
			break;
		case OPERATION_THIS:
			operation
				->this
				->term
				->node
				->accept(operation->this->term, self, argc, argv);
			operation
				->this
				->operation
				->node
				->accept(operation->this->operation, self, argc, argv);
			break;
		default:
			printf("Unexpected error: a bug was raised while visiting binder expression.\n");
			exit(1);
	}
}

void bvisit_type(Visitor self, Type *type, uint8_t argc, void **argv) {
    /* Nothing to do */
}

void bvisit_id(Visitor self, Id *id, uint8_t argc, void **argv) {
    /* Nothing to do */
}

void bvisit_number(Visitor self, Number *number, uint8_t argc, void **argv) {
    /* Nothing to do */
}


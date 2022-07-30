#include "../include/compiler.h"
#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Bytecode *compile(AST *ast, Stackframe *sf) {
    Bytecode *bytecode = NULL;
    Visitor self = COMPILER;
    void *argv[] = {&bytecode, &sf};
    cvisit_ast(self, ast, 2, argv);
    reverse(&bytecode);

    return bytecode;
}

void disassemble(Bytecode *bytecode) {

    while (bytecode != NULL) {
	printf("%-16s", OPNAMES[bytecode->opcode]);
	if (bytecode->hasarg) {
	    printf(" %d", bytecode->oparg);
	}
	printf("\n");

	bytecode = bytecode->prev;
    }
}

void dumps(Bytecode *bytecode) {
    while (bytecode != NULL) {
	printf("%02x", bytecode->opcode);
	if (bytecode->hasarg) {
	    int8_t arg[16];
	    sprintf(arg, "%x", bytecode->oparg);
		unsigned int lenarg = strlen(arg);
	    printf("%02x%02x", lenarg, bytecode->oparg);
	} else {
	    printf("%02x", 0);
	}

	bytecode = bytecode->prev;
    }

}

void dump(Bytecode *bytecode, char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
	printf("Unable to open file %s.\n", filename);
	exit(1);
    }

    while (bytecode != NULL) {
	int32_t offset = 0;
	fwrite(&bytecode->opcode, sizeof(int8_t), 1, f);
	if (bytecode->hasarg) {
	    offset = compress_byte(bytecode->oparg);
	    fwrite(&offset, sizeof(int8_t), 1, f);
	    fwrite(&bytecode->oparg, sizeof(int8_t) * offset, 1, f);
	} else {
	    fwrite(&offset, sizeof(int8_t), 1, f);
	}

	bytecode = bytecode->prev;
    }
}

Bytecode *new_bytecode(Opcode opcode, int32_t oparg, bool hasarg) {
    Bytecode *bytecode = malloc(sizeof(Bytecode));
    if (bytecode == NULL) {
	printf("Unable to allocate memory while initializing bytecode.\n");
	exit(1);
    }

    bytecode->opcode = opcode;
    bytecode->oparg = oparg;
    bytecode->hasarg = hasarg;
    bytecode->prev = NULL;

    return bytecode;
}

void push(Bytecode **bytecode, Opcode opcode, int32_t oparg, bool hasarg) {
    Bytecode *new = new_bytecode(opcode, oparg, hasarg);
    new->prev = *bytecode;
    *bytecode = new;
}

Bytecode pop(Bytecode **bytecode) {

}

static void reverse(Bytecode **bytecode) {
    Bytecode *current = *bytecode;
    Bytecode *next = NULL;
    Bytecode *prev = NULL;

    while (current != NULL) {
	prev = current->prev;
	current->prev = next;
	next = current;
	current = prev;
    }

    *bytecode = next;
}

static int32_t compress_byte(int32_t i) {
    int32_t n = 0;

    do {
	i >>= 1;
	n++;
    } while (i);

    return n;
}

void cvisit_ast(Visitor self, AST *ast, uint8_t argc, void **argv) {
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
		printf("Internal compilation error (ICE): this statement is not supported.\n");
		exit(1);
	}

	ast = ast->next;
    }
}

void cvisit_declaration(Visitor self, Declaration *declaration, uint8_t argc, void **argv) {
    if (argc < 2) {
	printf("Asserrtion error: argument count must be greater than 1.\n");
	exit(1);
    }

    bool stored = false;
    void *new[] = {argv[0], argv[1], &stored};

    declaration
	->id
	->node
	->accept(declaration->id, self, 3, new);
}

void cvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv) {
    if (argc < 2) {
		printf("Asserrtion error: argument count must be greater than 1.\n");
		exit(1);
    }

    assignment
		->expression
		->node
		->accept(assignment->expression, self, argc, argv);
    
    bool stored = true;
    void *new[] = {argv[0], argv[1], &stored};

	
    assignment
	->id
	->node
	->accept(assignment->id, self, 3, new);
}

void cvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv) {
    Bytecode **bytecode;
	Bytecode *current;
	
	if (argc < 1) {
		printf("Asserrtion error: argument count must be greater than 0.\n");
		exit(1);
    }

    bfc 
    	->expression
    	->node
    	->accept(bfc->expression, self, argc, argv);
    bytecode = argv[0];
    current = *bytecode;

    if (current == NULL) {
		printf("Internal compilation error: unexpected error.\n");
		exit(1);
    }

    switch (bfc->type) {
		case BUILTIN_PRINT: {
			if (current->opcode == ILOAD_VAR || current->opcode == ILOAD_CONST) {
				push(bytecode, IECHO, -1, false);
			} else {
				printf("Internal compilation error: this statement is not supported.\n");
				exit(1);
			}
			break;
		}
		default:
			printf("Internal compilation error: this statement is not supported.\n");
			exit(1);
    }
}

void cvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv) {
    if (argc < 2) {
		printf("Asserrtion error: argument count must be greater than 1.\n");
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
			printf("Internal compilation error: this statement is not supported.\n");
			exit(1);
	}

}

void cvisit_type(Visitor self, Type *type, uint8_t argc, void **argv) {
    /* Nothing to do */
}

void cvisit_id(Visitor self, Id *id, uint8_t argc, void **argv) {
    if (argc < 3) {
		printf("Asserrtion error: argument count must be greater than 2.\n");
		exit(1);
    }

    Bytecode **bytecode = argv[0];
    Stackframe **sf = argv[1];
    bool *stored = argv[2];
    Dataframe *ref = find_data(*sf, id->value);

    if (ref != NULL) {
		switch (ref->type) {
			case TYPE_NUMBER: {
				Opcode opcode = *stored ? ISTORE : ILOAD_VAR;
				push(bytecode, opcode, ref->addr, true);
				break;
			}
			default:
				printf("Internal compilation error: this statement is not supported.\n");
				exit(1);
		}
    } else {
		printf("Internal compilation error: unable to process id %s.\n", id->value);
		exit(1);
    }

}

void cvisit_factor(Visitor self, Factor *factor, uint8_t argc, void **argv) {
	bool stored = false;
	Bytecode **bytecode;

	if (argc < 2) {
		printf("Asserrtion error: argument count must be greater than 1.\n");
		exit(1);
    }

	void *new[] = {argv[0], argv[1], &stored};
	bytecode = argv[0];

	switch (factor->type) {
		case FACTOR_NUMBER:
			factor
				->number
				->node
				->accept(factor->number, self, argc, argv);
			break;
		case FACTOR_ID:
			factor
				->id
				->node
				->accept(factor->id, self, 3, new);
			break;
		case FACTOR_OPERATION:
			factor
				->operation
				->node
				->accept(factor->operation, self, argc, argv);
			break;
		default:
			printf("Internal compilation error: this statement is not supported.\n");
			exit(1);
	}

	if (factor->minus) {
		push(bytecode, LOAD_M1, -1, false);
		push(bytecode, MUL, -1, false);
	}
}

void cvisit_term(Visitor self, Term *term, uint8_t argc, void **argv) {
	Opcode opcode;
	Bytecode **bytecode;

	if (argc < 2) {
		printf("Asserrtion error: argument count must be greater than 1.\n");
		exit(1);
    }

	bytecode = argv[0];

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
			opcode = term->this->operator.type == TOKEN_MUL ? MUL : DIV;
			push(bytecode, opcode, -1, false);
			break;
		default:
			printf("Internal compilation error: this statement is not supported.\n");
			exit(1);
	}
}

void cvisit_operation(Visitor self, Operation *operation, uint8_t argc, void **argv) {
	Opcode opcode;
	Bytecode **bytecode;
	
	if (argc < 2) {
		printf("Asserrtion error: argument count must be greater than 1.\n");
		exit(1);
    }

	bytecode = argv[0];

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
			opcode = operation->this->operator.type == TOKEN_PLUS ? ADD : SUB;
			push(bytecode, opcode, -1, false);
			break;
		default:
			printf("Internal compilation error: this statement is not supported.\n");
			exit(1);
	}
}

void cvisit_number(Visitor self, Number *number, uint8_t argc, void **argv) {
    if (argc < 1) {
	printf("Asserrtion error: argument count must be greater than 0.\n");
	exit(1);
    }

    Bytecode **bytecode = argv[0];

    switch (number->type) {
	case NUMBER_INTEGER:
	    push(bytecode, ILOAD_CONST, number->integer, true);
	    break;
	default:
	    printf("Internal compilation error: this statement is not supported.\n");
	    exit(1);

    }
}


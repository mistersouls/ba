#include "../include/compiler.h"
#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>


Bytecode *compile(AST *ast, Stackframe *sf) {
    Bytecode *bytecode = NULL;
    Visitor self = COMPILER;
    void *argv[] = {&bytecode, &sf};
    cvisit_ast(self, ast, 2, argv);

    return bytecode;
}

void disassemble(Bytecode *bytecode) {
    reverse(&bytecode);

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

}

void dump(Bytecode *bytecode, char *filename) {

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
    if (argc < 1) {
	printf("Asserrtion error: argument count must be greater than 0.\n");
	exit(1);
    }

    bfc 
     ->expression
     ->node
     ->accept(bfc->expression, self, argc, argv);

    Bytecode **bytecode = argv[0];
    Bytecode *current = *bytecode;

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

    bool stored = false;
    void *new[] = {argv[0], argv[1], &stored};

    switch (expression->type) {
	case EXPRESSION_NUMBER:
	    expression
		->number
		->node
		->accept(expression->number, self, argc, argv);
	    break;
	case EXPRESSION_ID:
	    expression
		->id
		->node
		->accept(expression->id, self, 3, new);
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


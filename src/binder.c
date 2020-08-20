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

    return NULL;
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

    return NULL;
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

}

void bvisit_declaration(Visitor self, Declaration *declaration, uint8_t argc, void **argv) {

}

void bvisit_assignment(Visitor self, Assignment *assignment, uint8_t argc, void **argv) {

}

void bvisit_bfc(Visitor self, BuiltinFuncCall *bfc, uint8_t argc, void **argv) {

}

void bvisit_expression(Visitor self, Expression *expression, uint8_t argc, void **argv) {

}

void bvisit_type(Visitor self, Type *type, uint8_t argc, void **argv) {

}

void bvisit_id(Visitor self, Id *id, uint8_t argc, void **argv) {

}

void bvisit_number(Visitor self, Number *number, uint8_t argc, void **argv) {

}


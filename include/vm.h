#ifndef BA_VM_H
#define BA_VM_H


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "compiler.h"


typedef struct {
    int32_t *code;
    int32_t *stack;
    int32_t *data;

    uint32_t ip;
    int32_t sp;
    uint32_t fp;

    bool running;

} VM;


typedef uint32_t (*operation)(VM *vm);

static inline uint32_t nop(VM *vm) {
    return ++vm->ip;
}

static inline uint32_t iload_const(VM *vm) {
    vm->stack[++vm->sp] = vm->code[++vm->ip];
    return ++vm->ip;
}

static inline uint32_t load_m1(VM *vm) {
    vm->stack[++vm->sp] = -1;
    return ++vm->ip;
}

static inline uint32_t iload_var(VM *vm) {
    int32_t addr = vm->code[++vm->ip];
    vm->stack[++vm->sp] = vm->data[addr];
    return ++vm->ip;
}

static inline uint32_t istore(VM *vm) {
    int32_t addr = vm->code[++vm->ip];
    vm->data[addr] = vm->stack[vm->sp--];
    return ++vm->ip;
}

static inline uint32_t add(VM *vm) {
    int32_t left = vm->stack[vm->sp--];
    int32_t right = vm->stack[vm->sp];
    vm->stack[vm->sp] = left + right;
    return ++vm->ip;
}

static inline uint32_t substitute(VM *vm) {
    int32_t left = vm->stack[vm->sp--];
    int32_t right = vm->stack[vm->sp];
    vm->stack[vm->sp] = left - right;
    return ++vm->ip;
}

static inline uint32_t multiply(VM *vm) {
    int32_t left = vm->stack[vm->sp--];
    int32_t right = vm->stack[vm->sp];
    vm->stack[vm->sp] = left * right;
    return ++vm->ip;
}

static inline uint32_t divide(VM *vm) {
    int32_t left = vm->stack[vm->sp--];
    int32_t right = vm->stack[vm->sp];
    vm->stack[vm->sp] = left / right;
    return ++vm->ip;
}

static inline uint32_t iecho(VM *vm) {
    printf("%d", vm->stack[vm->sp--]);
    return ++vm->ip;
}

static inline uint32_t halt(VM *vm) {
    vm->running = false;
    return ++vm->ip;
}

static const operation ops[] = {
    [NOP] 		            = nop,
    [ILOAD_CONST]	        = iload_const,
    [LOAD_M1]               = load_m1,
    [ILOAD_VAR]		        = iload_var,
    [ISTORE]		        = istore,
    [ADD]                   = add,
    [SUB]                   = substitute,
    [MUL]                   = multiply,
    [DIV]                   = divide,
    [IECHO] 		        = iecho,
    [HALT]		            = halt
};

static inline uint8_t fetch(VM vm) {
    return vm.code[vm.ip];
}

static inline operation decode(Opcode opcode) {
    return ops[opcode];
}

static inline uint32_t execute(VM *vm, operation op) {
    return op(vm);
}

void cpu(VM vm);


#endif // BA_VM_H

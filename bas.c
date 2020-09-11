#include "include/vm.h"

void cpu(VM vm) {
    vm.running = true;

    while(vm.running) {
	Opcode opcode = fetch(vm);
	operation op = decode(opcode);
	vm.ip = execute(&vm, op);
    }
}

int main(int argc, char **argv) {
    int32_t code[] = {
	2, 0,
	2, 1,
	1, 1,
	3, 1,
	2, 1,
	3, 0,
	2, 0,
	4,
	255
    };
    int32_t stack[1024];
    int32_t data[1024];

    VM vm = {
	.code = code,
	.stack = stack,
	.data = data,

	.ip = 0,
	.sp = -1,
	.fp = 0,

	.running = false
    };

    cpu(vm);

    return 0;
}


#include "include/vm.h"
#include <stdlib.h>
#include <stdarg.h>

void usage() {
	printf("Usage: bas <filename with *.sb format>.\n");
	exit(1);
}

void errorif(bool cond, const char *format, ...) {
	va_list arglist;
	if (cond) {
		va_start(arglist, format);
		vprintf(format, arglist);
		va_end(arglist);
		exit(1);
	}
}

int32_t *getcode(char *filename) {
	int32_t *code, *tmp;
	size_t size;
	uint8_t offset = 0;
	FILE *f = fopen(filename, "rb");

	errorif(f == NULL, "Unable to open file: %s\n", filename);
	errorif(fseek(f, 0, SEEK_END) == -1, "Unable to read file: %s\n", filename);
	size = ftell(f);
	errorif(fseek(f, 0, SEEK_SET) == -1, "Unable to read file: %s\n", filename);
	code = malloc(sizeof(int32_t) * size);
	tmp = code;

	while(fread(tmp++, sizeof(uint8_t), 1, f) > 0) {
		fread(&offset, sizeof(uint8_t), 1, f);
		if (offset > 0) {
			errorif(fread(tmp++, sizeof(int8_t) * offset, 1, f) <= 0, "Unable to decode bytecode from file: %s.\n", filename);
		}
	}

	*tmp = HALT;
	fclose(f);

	return code;
}



void cpu(VM vm) {
    vm.running = true;

    while(vm.running) {
	Opcode opcode = fetch(vm);
	operation op = decode(opcode);
	vm.ip = execute(&vm, op);
    }
}

int main(int argc, char **argv) {
	if (argc != 2) {
		usage();
	}
	char *filename = argv[1];
    // int32_t code[] = {
	// 2, 0,
	// 2, 1,
	// 1, 1,
	// 3, 1,
	// 2, 1,
	// 3, 0,
	// 2, 0,
	// 4,
	// 255
    // };
	int32_t *code = getcode(filename);
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


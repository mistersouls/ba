
CC = gcc
LIBFLAGS = -shared -fPIC
INCLIBFLAGS = -lba -Lbin -Wl,-rpath,bin

SRC = $(shell find src/ -name "*.c")
LIB_OBJ = $(SRC:src/%.c=bin/%.o)

bin/%.o: src/%.c
	@mkdir -p bin
	@echo "Compiling $< ..."
	@$(CC) $(LIBFLAGS) -c $< -o $@

bin/ba.o: ba.c
	@mkdir -p bin
	@echo "Compiling main target $< ..."
	@$(CC) -c $< -o $@

bin/libba.so: $(LIB_OBJ)
	@mkdir -p bin
	@echo "Linking lib target $@ ..."
	@$(CC) $(LIBFLAGS) -o $@ $^

bin/ba: bin/ba.o
	@mkdir -p bin
	@echo "Linking main target $@ ..."
	@$(CC) -o $@ $< $(INCLIBFLAGS)

clean:
	@rm -rf bin/

build: bin/libba.so bin/ba


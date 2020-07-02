
default:
	mkdir -p bin
	rm -rf bin/*
	gcc -c src/lexer.c -o bin/lexer.o
	gcc ba.c bin/lexer.o -o bin/ba


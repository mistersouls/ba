
default:
	mkdir -p bin
	rm -rf bin/*
	gcc -c src/token.c -o bin/token.o
	gcc -c src/lexer.c -o bin/lexer.o
	gcc ba.c bin/lexer.o bin/token.o -o bin/ba


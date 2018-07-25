
all: bison flex gcc
	@echo "Done."

bison: parser.y
	bison parser.y

flex: scanner.l
	flex scanner.l

gcc: ast.c interpreter.c tables.c scanner.c parser.c
	gcc -Wall -g -o trab5 ast.c interpreter.c tables.c scanner.c parser.c
bminor: scanner.o main.o parser.o helper.o symbol.o type.o expr.o stmt.o decl.o param_list.o hash_table.o scope.o scratch.o label.o codegen.o
	gcc scanner.o main.o parser.o helper.o symbol.o type.o expr.o stmt.o decl.o param_list.o hash_table.o scope.o scratch.o label.o codegen.o -o bminor

scanner.c: scanner.flex
	flex -o scanner.c scanner.flex

scanner.o: scanner.c
	gcc -g -c scanner.c

main.o: main.c helper.h
	gcc -g -c main.c

helper.o: helper.c tokens.h
	gcc -g -c helper.c

parser.o: parser.c
	gcc parser.c -c -o parser.o -g

parser.c parser.h: parser.bison
	bison --defines=tokens.h --output=parser.c -v parser.bison

symbol.o: symbol.c symbol.h
	gcc -g -c symbol.c

type.o: type.c type.h
	gcc -g -c type.c

expr.o: expr.c expr.h
	gcc -g -c expr.c

stmt.o: stmt.c stmt.h
	gcc -g -c stmt.c

decl.o: decl.c decl.h
	gcc -g -c decl.c 

param_list.o: param_list.c param_list.h 
	gcc -g -c param_list.c 

hash_table.o: hash_table.c hash_table.h
	gcc -g -c hash_table.c

scope.o: scope.c scope.h
	gcc -g -c scope.c 

scratch.o: scratch.c scratch.h
	gcc -g -c scratch.c

lable.o: label.c label.h
	gcc -g -c label.c

codegen.o: codegen.c codegen.h 
	gcc -g -c codegen.c 

all: bminor

clean:
	rm -f parser.[ch] parser.output scanner.c *.o bminor

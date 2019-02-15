all: lex.yy.o main.o y.tab.o
	gcc -o minic lex.yy.o main.o y.tab.o

y.tab.o: y.tab.c
	gcc -c y.tab.c y.tab.h
	
y.tab.c:
	bison --yacc --verbose -d GoLite.y
	
main.o:
	gcc -c main.c 
	
lex.yy.o: lex.yy.c y.tab.c
	gcc -c lex.yy.c

lex.yy.c:
	flex GoLite.l

clean:
	rm lex.yy.c
	rm y.tab.*
	rm y.output
	rm *.o
	rm *.gch
	rm minic
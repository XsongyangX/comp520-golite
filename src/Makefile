
all: lex.yy.o main.o y.tab.o tree.o pretty.h pretty.o weeder.o symbol.o
	gcc -o minic main.o lex.yy.o y.tab.o tree.o pretty.o weeder.o symbol.o -lfl 

symbol.o:
	gcc -c symbol.c symbol.h

weeder.o:
	gcc -c weeder.c weeder.h 

tree.o:
	gcc -c tree.c tree.h -g

pretty.o:
	gcc -c pretty.c pretty.h -g

y.tab.o: y.tab.c
	gcc -c y.tab.c y.tab.h
	
y.tab.c:
	bison --yacc --verbose -d GoLite.y
	
main.o: 
	gcc -c main.c -g

lex.yy.o: lex.yy.c y.tab.c
	gcc -c lex.yy.c 

lex.yy.c:
	flex GoLite.l

# pretty option for testing the pretty printer
pretty: lex.yy.o y.tab.o tree.o weeder.o pretty.o main.o
	gcc -o minic main.o lex.yy.o y.tab.o tree.o weeder.o pretty.o -lfl	

# scan option only uses bison to generate the token headers
scan: emptyParser.o
	gcc -o minic main.c y.tab.c lex.yy.c y.tab.h -lfl

emptyParser.o:
	bison --yacc -d --verbose empty.y
	flex GoLite.l

cleanScan:
	rm y.tab.*
	rm y.output
	rm lex.yy.c
	rm minic

clean:
	rm lex.yy.c
	rm y.tab.*
	rm y.output
	rm *.o
	rm *.gch
	rm minic

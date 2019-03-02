all: lex.yy.o main.o y.tab.o tree.o pretty.o
	gcc -o minic lex.yy.o tree.o pretty.o main.o y.tab.o -lfl

tree.o:
	gcc -c tree.c tree.h

pretty.o:
	gcc -c pretty.c pretty.h

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

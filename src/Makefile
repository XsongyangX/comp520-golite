
all: main.o lex.yy.o y.tab.o tree.o pretty.h pretty.o weeder.o symbol.o
	gcc -o minic main.o lex.yy.o y.tab.o tree.o pretty.o weeder.o symbol.o -lfl  

symbol.o:
	gcc -c symbol.c symbol.h -g

weeder.o:
	gcc -c weeder.c weeder.h -g

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
pretty: lex.yy.o y.tab.o tree.o weeder.o pretty.o mainPrettyTest
	gcc -o minic main.o lex.yy.o y.tab.o tree.o weeder.o pretty.o -lfl

mainPrettyTest:
	gcc -c main.c -DPRETTY_TEST

# scan option only uses bison to generate the token headers
scan: emptyParser.o mainScanTest
	gcc -o minic main.o y.tab.c lex.yy.c y.tab.h -lfl -DSCAN_TEST

mainScanTest:
	gcc -c main.c -DSCAN_TEST

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

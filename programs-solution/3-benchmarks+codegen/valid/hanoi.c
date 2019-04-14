#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
char *my_itoa(int a){ char *tmp=malloc(16); sprintf(tmp, "%d", a); return tmp;}
char *ftoa(double a){ char *tmp = malloc(32); sprintf(tmp, "%+1.6e", a); return tmp;}
char *ctoa(char a){ char *tmp = malloc(2); tmp[0] = a; tmp[1] = '\0'; return tmp;}
void CODEGEN_BINDING0towers(int CODEGEN_BINDING1num, char CODEGEN_BINDING2from, char CODEGEN_BINDING3to, char CODEGEN_BINDING4aux);
int CODEGEN_BINDING5count;
void CODEGEN_BINDING0towers(int CODEGEN_BINDING1num, char CODEGEN_BINDING2from, char CODEGEN_BINDING3to, char CODEGEN_BINDING4aux){
	{
		if (CODEGEN_BINDING1num==1){
			CODEGEN_BINDING5count++;
			return;
		}
	}
CODEGEN_BINDING0towers(CODEGEN_BINDING1num-1, CODEGEN_BINDING2from, CODEGEN_BINDING4aux, CODEGEN_BINDING3to);	CODEGEN_BINDING5count++;
CODEGEN_BINDING0towers(CODEGEN_BINDING1num-1, CODEGEN_BINDING4aux, CODEGEN_BINDING3to, CODEGEN_BINDING2from);}
void main(){
CODEGEN_BINDING5count = 0;
CODEGEN_BINDING0towers(28, 'A', 'C', 'B');	printf("%s", "Number of moves:"); 
	printf(" ");
	printf("%d",CODEGEN_BINDING5count); 
	printf("\n");
}

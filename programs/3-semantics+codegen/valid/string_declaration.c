#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
void main(){
char *CODEGEN_BINDING0a = malloc(128);
	char *CODEGEN_BINDING1b = malloc(128);
		char *CODEGEN_BINDING2 = malloc(128); strcpy(CODEGEN_BINDING2, "stringy");
	CODEGEN_BINDING0a = CODEGEN_BINDING2;
	char *CODEGEN_BINDING3 = malloc(128); strcpy(CODEGEN_BINDING3, CODEGEN_BINDING0a);
	CODEGEN_BINDING1b = CODEGEN_BINDING3;
	printf("%s", CODEGEN_BINDING1b); 
	printf("\n");
	char *CODEGEN_BINDING4 = malloc(128); strcpy(CODEGEN_BINDING4, "not a");
	CODEGEN_BINDING1b = CODEGEN_BINDING4;
	printf("%s", CODEGEN_BINDING0a); 
	printf("\n");
}

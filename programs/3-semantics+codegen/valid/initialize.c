#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
void main(){
bool CODEGEN_BINDING0b = 0;
	if(CODEGEN_BINDING0b){ printf("True");} else{ printf("False");}
	printf("\n");
int CODEGEN_BINDING1n = 0;
	printf("%d",CODEGEN_BINDING1n); 
	printf("\n");
float CODEGEN_BINDING2x = 0.0;
	printf("%f",CODEGEN_BINDING2x); 
	printf("\n");
char *CODEGEN_BINDING3s = malloc(128);
		printf("%s", CODEGEN_BINDING3s); 
	printf("\n");
char CODEGEN_BINDING4r = 0;
	printf("%d",CODEGEN_BINDING4r); 
	printf("\n");
	struct CODEGEN_BINDING6 {
		bool CODEGEN_BINDING7m;
	}CODEGEN_BINDING5xx;
	CODEGEN_BINDING5xx.CODEGEN_BINDING7m = 0; 	if(CODEGEN_BINDING5xx.CODEGEN_BINDING7m){ printf("True");} else{ printf("False");}
	printf("\n");
	struct CODEGEN_BINDING9 {
		int CODEGEN_BINDING10x;
		struct CODEGEN_BINDING12 {
			char *CODEGEN_BINDING13k = malloc(128);
			char CODEGEN_BINDING14l;
		}CODEGEN_BINDING11s;
	}CODEGEN_BINDING8ooo;
	CODEGEN_BINDING8ooo.CODEGEN_BINDING11s.CODEGEN_BINDING14l = 0; 	CODEGEN_BINDING8ooo.CODEGEN_BINDING11s.CODEGEN_BINDING13k = malloc(128); 	CODEGEN_BINDING8ooo.CODEGEN_BINDING10x = 0; 	printf("%d",CODEGEN_BINDING8ooo.CODEGEN_BINDING10x); 
	printf("\n");
	printf("%s", CODEGEN_BINDING8ooo.CODEGEN_BINDING11s.CODEGEN_BINDING13k); 
	printf("\n");
	printf("%d",CODEGEN_BINDING8ooo.CODEGEN_BINDING11s.CODEGEN_BINDING14l); 
	printf("\n");
}

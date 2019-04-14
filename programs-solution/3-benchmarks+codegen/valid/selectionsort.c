#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
char *my_itoa(int a){ char *tmp=malloc(16); sprintf(tmp, "%d", a); return tmp;}
char *ftoa(double a){ char *tmp = malloc(32); sprintf(tmp, "%+1.6e", a); return tmp;}
char *ctoa(char a){ char *tmp = malloc(2); tmp[0] = a; tmp[1] = '\0'; return tmp;}
struct CODEGEN_BINDING2 {
	int size;
	int cap;
	int *val;
};
void CODEGEN_BINDING0selection_sort(struct CODEGEN_BINDING2 CODEGEN_BINDING1array, int CODEGEN_BINDING3length);
void CODEGEN_BINDING0selection_sort(struct CODEGEN_BINDING2 CODEGEN_BINDING1array, int CODEGEN_BINDING3length){
	{
				int CODEGEN_BINDING4 = 0;
int CODEGEN_BINDING5i = CODEGEN_BINDING4;
		while (CODEGEN_BINDING5i<CODEGEN_BINDING3length){
			int CODEGEN_BINDING6 = CODEGEN_BINDING5i; if(CODEGEN_BINDING1array.size <= CODEGEN_BINDING6){fprintf(stderr, "Access out of range.\n"); exit(1);}
			int CODEGEN_BINDING7 = CODEGEN_BINDING1array.val[CODEGEN_BINDING6];
int CODEGEN_BINDING8min = CODEGEN_BINDING7;
			int CODEGEN_BINDING9 = CODEGEN_BINDING5i;
int CODEGEN_BINDING10min_index = CODEGEN_BINDING9;
			{
								int CODEGEN_BINDING11 = CODEGEN_BINDING5i+1;
int CODEGEN_BINDING12j = CODEGEN_BINDING11;
				while (CODEGEN_BINDING12j<CODEGEN_BINDING3length){
					{
						int CODEGEN_BINDING13 = CODEGEN_BINDING12j; if(CODEGEN_BINDING1array.size <= CODEGEN_BINDING13){fprintf(stderr, "Access out of range.\n"); exit(1);}
						if (CODEGEN_BINDING1array.val[CODEGEN_BINDING13]<CODEGEN_BINDING8min){
							int CODEGEN_BINDING14 = CODEGEN_BINDING12j; if(CODEGEN_BINDING1array.size <= CODEGEN_BINDING14){fprintf(stderr, "Access out of range.\n"); exit(1);}
							int CODEGEN_BINDING15 = CODEGEN_BINDING1array.val[CODEGEN_BINDING14];
							CODEGEN_BINDING8min = CODEGEN_BINDING15;
							int CODEGEN_BINDING16 = CODEGEN_BINDING12j;
							CODEGEN_BINDING10min_index = CODEGEN_BINDING16;
						}
					}
				CODEGEN_BINDING12j++;
					continue;
				CODEGEN_BINDING12j++;
				}
			}
			{
				if (CODEGEN_BINDING10min_index!=CODEGEN_BINDING5i){
					int CODEGEN_BINDING17 = CODEGEN_BINDING5i; if(CODEGEN_BINDING1array.size <= CODEGEN_BINDING17){fprintf(stderr, "Access out of range.\n"); exit(1);}
					int CODEGEN_BINDING18 = CODEGEN_BINDING10min_index; if(CODEGEN_BINDING1array.size <= CODEGEN_BINDING18){fprintf(stderr, "Access out of range.\n"); exit(1);}
					int CODEGEN_BINDING19 = CODEGEN_BINDING1array.val[CODEGEN_BINDING17];
					CODEGEN_BINDING1array.val[CODEGEN_BINDING18] = CODEGEN_BINDING19;
					int CODEGEN_BINDING20 = CODEGEN_BINDING5i; if(CODEGEN_BINDING1array.size <= CODEGEN_BINDING20){fprintf(stderr, "Access out of range.\n"); exit(1);}
					int CODEGEN_BINDING21 = CODEGEN_BINDING8min;
					CODEGEN_BINDING1array.val[CODEGEN_BINDING20] = CODEGEN_BINDING21;
				}
			}
		CODEGEN_BINDING5i++;
			continue;
		CODEGEN_BINDING5i++;
		}
	}
}
void main(){
	struct CODEGEN_BINDING2 CODEGEN_BINDING22array;
	CODEGEN_BINDING22array.size = 0; CODEGEN_BINDING22array.cap = 0;
	CODEGEN_BINDING22array.val = NULL; 
	int CODEGEN_BINDING23 = 50000;
int CODEGEN_BINDING24length = CODEGEN_BINDING23;
	{
				int CODEGEN_BINDING25 = 0;
int CODEGEN_BINDING26i = CODEGEN_BINDING25;
		while (CODEGEN_BINDING26i<CODEGEN_BINDING24length){
			struct CODEGEN_BINDING2 CODEGEN_BINDING27;
			CODEGEN_BINDING27.size = CODEGEN_BINDING22array.size + 1;
			if (CODEGEN_BINDING22array.size+1 > CODEGEN_BINDING22array.cap){
				if(CODEGEN_BINDING22array.cap == 0) CODEGEN_BINDING27.cap = 2;
				else CODEGEN_BINDING27.cap = CODEGEN_BINDING22array.cap*2;
			CODEGEN_BINDING27.val = malloc(sizeof(int)*CODEGEN_BINDING27.cap);
			}
			else{ CODEGEN_BINDING27.cap = CODEGEN_BINDING22array.cap; CODEGEN_BINDING27.val = CODEGEN_BINDING22array.val;}
			for( int codegen_i = 0; codegen_i < CODEGEN_BINDING22array.cap; codegen_i++){
				CODEGEN_BINDING27.val[codegen_i] = CODEGEN_BINDING22array.val[codegen_i];
			}
CODEGEN_BINDING27.val[CODEGEN_BINDING27.size-1] = CODEGEN_BINDING24length-CODEGEN_BINDING26i-1;
struct CODEGEN_BINDING2 CODEGEN_BINDING28 = CODEGEN_BINDING27;
			CODEGEN_BINDING22array.val = CODEGEN_BINDING28.val; 
			CODEGEN_BINDING22array.size = CODEGEN_BINDING28.size; 
			CODEGEN_BINDING22array.cap = CODEGEN_BINDING28.cap; 
		CODEGEN_BINDING26i++;
			continue;
		CODEGEN_BINDING26i++;
		}
	}
CODEGEN_BINDING0selection_sort(CODEGEN_BINDING22array, CODEGEN_BINDING24length);	{
				int CODEGEN_BINDING29 = 0;
int CODEGEN_BINDING30i = CODEGEN_BINDING29;
		while (CODEGEN_BINDING30i<10){
			int CODEGEN_BINDING31 = CODEGEN_BINDING30i; if(CODEGEN_BINDING22array.size <= CODEGEN_BINDING31){fprintf(stderr, "Access out of range.\n"); exit(1);}
			printf("%d",CODEGEN_BINDING22array.val[CODEGEN_BINDING31]); 
			printf("\n");
		CODEGEN_BINDING30i++;
			continue;
		CODEGEN_BINDING30i++;
		}
	}
}

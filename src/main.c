#include "stdio.h"
#include <string.h>
void yyparse();
int yylex();
int isToken;
STATEMENT *theprogram;
int typecheck;
char *currentFile;
int main(int argc, char** argv)
{
    theprogram = makeSTATEMENT_empty();
    currentFile = argv[2];
    if(argc < 2)
    {
        printf("Not enough arguments\n");
        return 0;

    }
    currentFile = argv[2];
    if(freopen(currentFile, "r", stdin) != NULL)
    {
        
        
        if(strncmp(*(argv+1), "scan", 4) == 0)
        {
            isToken = 0;
            typecheck = 0;
            while(yylex()){};
            printf("OK\n");
        }
        else if(strncmp(*(argv+1), "tokens", 6) == 0){
            isToken = 1;
            typecheck = 0;
            while(yylex()){};
        }
        // else if(strncmp(*(argv+1), "parse", 5) == 0)
        // {
        //     isToken = 0;
        //     typecheck = 0;
        //     yyparse();
        //     printf("OK\n");
        // }
        // else if (strncmp(*(argv+1), "pretty", 6) == 0){
        //     isToken = 0;
        //     typecheck = 0;
        //     yyparse();
        //     prettySTMT(theprogram, 0);
        // }
        // else if (strncmp(*(argv+1), "symbol", 6) == 0){
        //     isToken = 0;
        //     typecheck = 0;
        //     yyparse();
        //     symProg(theprogram);
        // }
        // else if (strncmp(*(argv+1), "typecheck", 11) == 0){
        //     isToken = 0;
        //     typecheck = 1;
        //     yyparse();
        //     symProg(theprogram);
        //     printf("OK\n");
        //     printf("%s", *(argv+2));
        // }
        // else if(strncmp(*(argv+1), "codegen", 7) == 0)
        // {
        //     char *tmp = malloc(sizeof(char)*(strlen(currentFile)+1));
        //     strcpy(tmp, currentFile);
        //     char *newFile = strtok(tmp, ".");
        //     newFile = strcat(newFile, ".c");
        //     isToken = 0;
        //     typecheck = 1;
        //     yyparse();
        //     symProg(theprogram);
        //     FILE *fp;
        //     fp = fopen(newFile, "w");
        //     genProgram(theprogram, fp);
        //     fclose(fp);
            
        // }
        return 0;
    }
    else{
        fprintf(stderr, "Trouble opening %s.\n", argv[2]);
        return 1;
    }
    
}
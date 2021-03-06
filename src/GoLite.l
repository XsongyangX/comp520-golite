
%{
#include "y.tab.h"
#include <string.h>
#include <stdlib.h>

// Stores the line number with each token sent to bison
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
extern int isToken;
int requiresIns = 0;

void rawStringHelper(char *tmp, char *s, int len)
{
	tmp[0] = '"';
	int offset = 0;
	for(int i = 1; i < len; i++){
		switch(s[i]){
			case '\0':
				tmp[i+offset] = '\\';
				tmp[i+offset+1] = '0';
				offset++;
				break;
			case '\n':
				tmp[i+offset] = '\\';
				tmp[i+offset+1] = 'n';
				offset++;
				break;
			case '\r':
				tmp[i+offset] = '\\';
				tmp[i+offset+1] = 'r';
				offset++;
				break;
			case '\t':
				tmp[i+offset] = '\\';
				tmp[i+offset+1] = 't';
				offset++;
				break;
			case '\b':
				tmp[i+offset] = '\\';
				tmp[i+offset+1] = 'b';
				offset++;
				break;
			case '\f':
				tmp[i+offset] = '\\';
				tmp[i+offset+1] = 'f';
				offset++;
				break;
			case '\v':
				tmp[i+offset] = '\\';
				tmp[i+offset+1] = 'v';
				offset++;
				break;
			default:
				tmp[i+offset] = s[i];
				break;
		}
	}
	tmp[len+offset-1] = '"';
	tmp[len+offset] = '\0';
}
%}

/* Scanner options, line number generation */
%option yylineno

/* Removes warnings about input/output functions */
%option noinput
%option nounput

/* Helper */
PUNCTUATION [?<>+-=,^$|&]
%s EOFCHECK
%x IN_COMMENT
%%
[ \t\r]+ 

"\n" {  BEGIN(EOFCHECK);
	if(requiresIns){
		requiresIns = 0;
		if(isToken)	printf("tSEMICOLON\n");
		
		return ';';
	}
}

"+" { // OPERATORS
	if(isToken){printf("tPLUS\n");} requiresIns = 0;return '+';}
"-" {if(isToken){printf("tMINUS\n");} requiresIns = 0;return '-';}
"*" {if(isToken){printf("tTIMES\n");} requiresIns = 0;return '*';}
"/" {if(isToken){printf("tDIV\n");} requiresIns = 0;return '/';}
"%" {if(isToken){printf("tMODULO\n");} requiresIns = 0;return '%';}
"&" {if(isToken){printf("tAMPERSAND\n");} requiresIns = 0;return '&';}
"|" {if(isToken){printf("tVERTBAR\n");} requiresIns = 0;return '|';}
"^" {if(isToken){printf("tPOWER\n");} requiresIns = 0;return '^';}
"<<" {if(isToken){printf("tLEFTSHIFT\n");} requiresIns = 0;return tLEFTSHIFT;}
">>" {if(isToken){printf("tRIGHTSHIFT\n");} requiresIns = 0;return tRIGHTSHIFT;}
"&^" {if(isToken){printf("tANDNOT\n");} requiresIns = 0;return tANDNOT;}
"+=" {if(isToken){printf("tPLUSASSIGN\n");} requiresIns = 0;return tPLUSASSIGN;}
"-=" {if(isToken){printf("tMINUSASSIGN\n");} requiresIns = 0;return tMINUSASSIGN;}
"*=" {if(isToken){printf("tTIMESASSIGN\n");} requiresIns = 0;return tTIMESASSIGN;}
"/=" {if(isToken){printf("tDIVASSIGN\n");} requiresIns = 0;return tDIVASSIGN;}
"%=" {if(isToken){printf("tMODASSIGN\n");} requiresIns = 0;return tMODASSIGN;}
"&=" {if(isToken){printf("tAMPASSIGN\n");} requiresIns = 0;return tANDASSIGN;}
"^=" {if(isToken){printf("tPOWASSIGN\n");} requiresIns = 0;return tPOWASSIGN;}
"<<=" {if(isToken){printf("tLSHIFTASSIGN\n");} requiresIns = 0;return tLSHIFTASSIGN;}
">>=" {if(isToken){printf("tRSHIFTASSIGN\n");} requiresIns = 0;return tRSHIFTASSIGN;}
"&^=" {if(isToken){printf("tAMPPOWASSIGN\n");} requiresIns = 0;return tAMPPOWASSIGN;}
"|=" {if(isToken){printf("tBARASSIGN\n");} requiresIns = 0;return tORASSIGN;}
"&&" {if(isToken){printf("tAND\n");} requiresIns = 0;return tAND;}
"||" {if(isToken){printf("tOR\n");} requiresIns = 0;return tOR;}
"<-" {if(isToken){printf("tLESSMINUS\n");} requiresIns = 0;return tLESSMINUS;}
"++" {if(isToken){printf("tINCREMENT\n");} requiresIns = 1;BEGIN(EOFCHECK); return tINCREMENT;}
"--" {if(isToken){printf("tDECREMENT\n");} requiresIns = 1;BEGIN(EOFCHECK); return tDECREMENT;}
"==" {if(isToken){printf("tEQ\n");} requiresIns = 0;return tEQ;} 
"<" {if(isToken){printf("tLESS\n");} requiresIns = 0;return '<';}
">" {if(isToken){printf("tGREATER\n");} requiresIns = 0;return '>';}
"=" {if(isToken){printf("tASSIGN\n");} requiresIns = 0;return '=';}
"!" {if(isToken){printf("tBANG\n");} requiresIns = 0;return '!';} 
"!=" {if(isToken){printf("tNEQ\n");} requiresIns = 0;return tNEQ;} 
"<=" {if(isToken){printf("tLEQ\n");} requiresIns = 0;return tLEQ;} 
">=" {if(isToken){printf("tGEQ\n");} requiresIns = 0;return tGEQ;} 
":=" {if(isToken){printf("tCOLONASSIGN\n");} requiresIns = 0;return tCOLONASSIGN;} 
"..." {if(isToken){printf("tDOTS\n");} requiresIns = 0;return tDOTS;} 
"(" {if(isToken){printf("tLEFTPAREN\n");} requiresIns = 0;return '(';}
")" {if(isToken){printf("tRIGHTPAREN\n");} requiresIns = 1;BEGIN(EOFCHECK); return ')';}
"{" {if(isToken){printf("tLEFTBRACE\n");} requiresIns = 0;return '{';}
"}" {if(isToken){printf("tRIGHTBRACE\n");} requiresIns = 1;BEGIN(EOFCHECK); return '}';}
"[" {if(isToken){printf("tLEFTBRACKET\n");} requiresIns = 0;return '[';}
"]" {if(isToken){printf("tRIGHTBRACKET\n");} requiresIns = 1;BEGIN(EOFCHECK); return ']';}
":" {if(isToken){printf("tCOLON\n");} requiresIns = 0;return ':';} 
";" {if(isToken){printf("tSEMICOLON\n");} requiresIns = 0; return ';';} 
"," {if(isToken){printf("tCOMMA\n");} requiresIns = 0;return ',';} 
"." {if(isToken){printf("tPERIOD\n");} requiresIns = 0;return '.';} 


"break" { // KEYWORDS
	if(isToken){printf("tBREAK\n");} requiresIns = 1; BEGIN(EOFCHECK); return tBREAK;}
"case" {if(isToken){printf("tCASE\n");} requiresIns = 0;return tCASE;}
"chan" {if(isToken){printf("tCHAN\n");} requiresIns = 0;return tCHAN;}
"const" {if(isToken){printf("tCONST\n");} requiresIns = 0;return tCONST;}
"continue" {if(isToken){printf("tCONTINUE\n");} requiresIns = 1; BEGIN(EOFCHECK); return tCONTINUE;}
"default" {if(isToken){printf("tDEFAULT\n");} requiresIns = 0;return tDEFAULT;}
"defer" {if(isToken){printf("tDEFER\n");} requiresIns = 0;return tDEFER;}
"else" {if(isToken){printf("tELSE\n");} requiresIns = 0;return tELSE;}
"fallthrough" {if(isToken){printf("tFALLTHROUGH\n");} requiresIns = 1; BEGIN(EOFCHECK); return tFALLTHROUGH;}
"for" {if(isToken){printf("tFOR\n");} requiresIns = 0;return tFOR;}
"func" {if(isToken){printf("tFUNC\n");} requiresIns = 0;return tFUNC;}
"go" {if(isToken){printf("tGO\n");} requiresIns = 0;return tGO;}
"goto" {if(isToken){printf("tGOTO\n");} requiresIns = 0;return tGOTO;}
"if" {if(isToken){printf("tIF\n");} requiresIns = 0;return tIF;}
"import" {if(isToken){printf("tIMPORT\n");} requiresIns = 0;return tIMPORT;}
"interface" {if(isToken){printf("tINTERFACE\n");} requiresIns = 0;return tINTERFACE;}
"map" {if(isToken){printf("tMAP\n");} requiresIns = 0;return tMAP;}
"package" {if(isToken){printf("tPACKAGE\n");} requiresIns = 0;return tPACKAGE;}
"range" {if(isToken){printf("tRANGE\n");} requiresIns = 0;return tRANGE;}
"return" {if(isToken){printf("tRETURN\n");} requiresIns = 1; BEGIN(EOFCHECK); return tRETURN;}
"select" {if(isToken){printf("tSELECT\n");} requiresIns = 0;return tSELECT;}
"struct" {if(isToken){printf("tSTRUCT\n");}
	yylval.identifier = malloc(10);
	strcpy(yylval.identifier, "struct");
	requiresIns = 0;return tSTRUCT;}
"switch" {if(isToken){printf("tSWITCH\n");} requiresIns = 0;return tSWITCH;}
"type" {if(isToken){printf("tTYPE\n");} requiresIns = 0;return tTYPE;}
"print" {if(isToken){printf("tPRINT\n");} requiresIns = 0;return tPRINT;}
"println" {if(isToken){printf("tPRINTLN\n");} requiresIns = 0;return tPRINTLN;}
"var" {if(isToken){printf("tVAR\n");} requiresIns = 0;return tVAR;}
"append" {if(isToken){printf("tAPPEND\n");} requiresIns = 0;return tAPPEND;}
"len" {if(isToken){printf("tLEN\n");} requiresIns = 0;return tLEN;}
"cap" {if(isToken){printf("tCAP\n");} requiresIns = 0;return tCAP;}

"//"[^\n]* {/*line comments*/}
	
"/*" {BEGIN(IN_COMMENT);}
<IN_COMMENT>"*/" {BEGIN(INITIAL);}
<IN_COMMENT><<EOF>> {fprintf(stderr, "Error: (line %d) unclosed block comment\n", yylineno); exit(1);}
<IN_COMMENT>\n {
	if (requiresIns) {
		if(isToken) printf("tSEMICOLON\n");
		requiresIns = 0;
		return ';';
	}
}
<IN_COMMENT>. 
<EOFCHECK><<EOF>> {BEGIN(INITIAL); if(requiresIns){ requiresIns = 0; if(isToken){ printf("tSEMICOLON\n"); } return ';';	} }
0|([1-9][0-9]*) { // INT
	BEGIN(EOFCHECK);
	//DECIMAL VALUE
    yylval.intval = atoi(yytext);
    if(isToken){
        fprintf(stdout, "tINTLITERAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tINTLITERAL;
}
0[0-7]* {//OCTAL VALUE
	BEGIN(EOFCHECK);
    yylval.intval = (int)strtol(yytext, NULL, 8);
    if(isToken){
        fprintf(stdout, "tINTLITERAL(%s = %ld)\n", yytext, strtol(yytext, NULL, 8));
    }
    requiresIns = 1; 
    return tINTLITERAL;
}
0(x|X)[0-9a-fA-F][0-9a-fA-F]* {//HEXADECIMAL VALUE
	BEGIN(EOFCHECK);
    yylval.intval = (int)strtol(yytext, NULL, 0);
    if(isToken){
        fprintf(stdout, "tINTLITERAL(%s = %ld)\n", yytext, strtol(yytext, NULL, 0));
    }
    requiresIns = 1; 
    return tINTLITERAL;
}


([0-9][0-9]*"."[0-9]*) { // FLOAT
	BEGIN(EOFCHECK);
	//LEFT of period
    yylval.floatval = atof(yytext);
    if(isToken){
        fprintf(stdout, "tFLOATLITERAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tFLOATLITERAL;
}
([1-9][0-9]*)*"."[0-9][0-9]* {//RIGHT of period
	BEGIN(EOFCHECK);
    yylval.floatval = atof(yytext);
    if(isToken){
        fprintf(stdout, "tFLOATLITERAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tFLOATLITERAL;
}


'([ a-zA-Z0-9~@#%*/_`.:;!]|{PUNCTUATION}|\\\\|\{|\}|\[|\]|\(|\)|(\\a)|(\\b)|(\\f)|(\\n)|(\\r)|(\\t)|(\\v)|(\")|(\\'))' {//RUNE (CHAR)
	BEGIN(EOFCHECK);
    yylval.runeval = strdup(yytext)[1];
    if(isToken){
        fprintf(stdout, "tRUNELITERAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tRUNELITERAL;
}


\"([ a-zA-Z0-9~@#%\*/'_`.:;!]|{PUNCTUATION}|\\\\|\{|\}|\[|\]|\(|\)|(\\a)|(\\b)|(\\f)|(\\n)|(\\r)|(\\t)|(\\v)|(\\\"))*\" {//STRING
	BEGIN(EOFCHECK);
    yylval.strval = strdup(yytext);
    if(isToken){
        fprintf(stdout, "tSTRINGLITERAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tSTRINGLITERAL;
}

`([ a-zA-Z0-9~@#%\*/'_.:;!]|{PUNCTUATION}|\\|\{|\}|\[|\]|\(|\)|(\"))*` {//RAW STRING
	BEGIN(EOFCHECK);
    yylval.strval = malloc(sizeof(char)*yyleng*2);
    rawStringHelper(yylval.strval, yytext, yyleng);
    if(isToken){
        fprintf(stdout, "tRAWSTRINGLITERAL(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tRAWSTRINGLITERAL;

}

[a-zA-Z_][a-zA-Z0-9_]* {//IDENTIFIERS
	BEGIN(EOFCHECK);
    yylval.identifier = strdup(yytext);
    if(isToken){
        fprintf(stdout, "tIDENTIFIER(%s)\n", yytext);
    }
    requiresIns = 1; 
    return tIDENTIFIER;
}

. {
    fprintf(stderr, "Error: (line %d) unexpected character '%s'\n", yylineno, yytext);
    exit(1);
}
%%

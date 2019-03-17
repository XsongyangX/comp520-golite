#include "codegen.h"

void codegenProgram(PROGRAM *p){

}

void codegenDeclaration(DECLARATION *d){
	
}

void codegenStatement(STATEMENT *s){
	
}

void codegenExpression(EXP *e, SYMTABLE *table){
	switch (e->kind){
		case emptyExp:
			break;
		case intExp:
			printf("%d", e->val->intLiteral);
			break; 
		case floatExp:
			printf("%f", e->val->floatLiteral);
			break;  
		case strExp: 
			printf("\"%s\"", e->val->strLiteral);
			break; 
		case rawstrExp:
			printf("\"%s\"", e->val->strLiteral);
			break; 
		case boolExp: 
			if (e->val->intLiteral == 1){
				printf("True");
			}
			else{
				printf("False");
			}
			break;
		case runeExp:
			printf("\'%c\'", e->val->runeLiteral);
			break;
		case plusExp:
			codegenExpression(e->val->binary->lhs);
			printf("+");
			codegenExpression(e->val->binary->rhs);
			break;
		case minusExp:
			codegenExpression(e->val->binary->lhs);
			printf("-");
			codegenExpression(e->val->binary->rhs);
			break;
		case timesExp:
			codegenExpression(e->val->binary->lhs);
			printf("*");
			codegenExpression(e->val->binary->rhs);
			break;
		case divExp:
			codegenExpression(e->val->binary->lhs);
			printf("/");
			codegenExpression(e->val->binary->rhs);
			break;
		case modExp:
			codegenExpression(e->val->binary->lhs);
			printf("\%");
			codegenExpression(e->val->binary->rhs);
			break;
		case notExp:
			printf("!");
			codegenExpression(e->val->binary->rhs);
			break;
		case posExp:
			/*We should handle non-int types in here IIRC
			I just can't think of a way to do it neatly at the moment*/
			printf("abs(");
			codegenExpression(e->val->binary->rhs);
			printf(")");
			break;
		case negExp:
			printf("-");
			codegenExpression(e->val->binary->rhs);
			break;
		case parExp:
			printf("(");
			codegenExpression(e->val->binary->rhs);
			printf(")");
			break;
		case eqExp:
			if (e->val->binary->lhs->kind == idExp){
				SYMBOL *tmpSymb1 = getSymbol(table, e->val->binary->lhs->val->identifier, structSymbol)
				if (tmpStruct != NULL){
					SYMBOL *tmpSymb2 = getSymbol(table, e->val->binary->rhs->val->identifier, structSymbol)
					eqExpStructs(tmpSymbol1, tmpSymbol2, 1, "");
				}
				else{
					codegenExpression(e->val->lhs);
					printf("==");
					codegenExpression(e->val->rhs);
				}
			}
			else{
				codegenExpression(e->val->lhs);
				printf("==");
				codegenExpression(e->val->rhs);
			}
			break;
		case neqExp:
			if (e->val->binary->lhs->kind == idExp){
				SYMBOL *tmpSymb1 = getSymbol(table, e->val->binary->lhs->val->identifier, structSymbol)
				if (tmpStruct != NULL){
					SYMBOL *tmpSymb2 = getSymbol(table, e->val->binary->rhs->val->identifier, structSymbol)
					eqExpStructs(tmpSymbol1, tmpSymbol2, 0, "");
				}
				else{
					codegenExpression(e->val->lhs);
					printf("!=");
					codegenExpression(e->val->rhs);
				}
			}
			else{
				codegenExpression(e->val->lhs);
				printf("!=");
				codegenExpression(e->val->rhs);
			}
			break;
		case geqExp:
			codegenExpression(e->val->binary->lhs);
			printf(">=");
			codegenExpression(e->val->binary->rhs);
			break;
		case leqExp:
			codegenExpression(e->val->binary->lhs);
			printf("<=");
			codegenExpression(e->val->binary->rhs);
			break;
		case gtExp:
			codegenExpression(e->val->binary->lhs);
			printf(">");
			codegenExpression(e->val->binary->rhs);
			break;
		case ltExp:
			codegenExpression(e->val->binary->lhs);
			printf("<");
			codegenExpression(e->val->binary->rhs);
			break;
		case orExp:
			codegenExpression(e->val->binary->lhs);
			printf("||");
			codegenExpression(e->val->binary->rhs);
			break;
		case andExp:
			codegenExpression(e->val->binary->lhs);
			printf("&&");
			codegenExpression(e->val->binary->rhs);
			break;
		case andnotExp:
			codegenExpression(e->val->binary->lhs);
			printf("& -(");
			codegenExpression(e->val->binary->rhs);
			printf(")");
			break;
		case idExp:
			printf("%s", e->val->identifier);
			break;
		/*
		-------------------------
		UNFINISHED
		-------------------------
		case idblockExp:
		case expblockExp:
		*/
		case bitAndExp:
			codegenExpression(e->val->binary->lhs);
			printf("&");
			codegenExpression(e->val->binary->rhs);
			break;
		case bitOrExp:
			codegenExpression(e->val->binary->lhs);
			printf("|");
			codegenExpression(e->val->binary->rhs);
			break;
		case xorExp:
			codegenExpression(e->val->binary->lhs);
			printf("^");
			codegenExpression(e->val->binary->rhs);
			break;
		case lshiftExp:
			codegenExpression(e->val->binary->lhs);
			printf("<<");
			codegenExpression(e->val->binary->rhs);
			break;
		case rshiftExp:
			codegenExpression(e->val->binary->lhs);
			printf(">>");
			codegenExpression(e->val->binary->rhs);
			break;
		case indexExp:
			codegenExpression(e->val->binary->rhs);
			break;
		case elementExp:
			codegenExpression(e->val->binary->lhs);
			printf("[");
			codegenExpression(e->val->binary->rhs);
			printf("]");
			break;
		case invocExp:
			codegenExpression(e->val->binary->lhs);
			printf(".");
			codegenExpression(e->val->binary->rhs);
			break;
		/*
		-------------------------
		UNFINISHED
		-------------------------
		case funcBlockExp:
		case appendExp:
		case lenExp:
		case capExp:
		*/
		case uxorExp:
			//As far as I can tell, 1111111... in C
			printf("2147483647^(")
			codegenExpression(e->val->binary->rhs);
			printf(")");
		case funcExp:
		default:
			printf("ERROR: Unknown expression type\n");
			exit(1);
	}
}


//Generates code for comparing structs.
//mode=1 for == expressions
//mode=0 for != expressions
//nameSoFar represents the path to nested structs since we don't have pointers to parents;
//e.g. struct.innerStruct.innerInnerStruct and the like
void eqExpStructs(SYMBOL *s1, SYMBOL *s2, int mode, char *nameSoFar){
	printf("(");
	SYMBOL *field1 = s1->val->structFields;
	SYMBOL *field2 = s2->val->structFields;
	while(field1 != NULL){
		if(*field1->kind != structSym){
			printf("%s%s.%s", nameSoFar, s1->name, field1->name);
			if (mode){
				printf("==");
			}
			else{
				printf("!=");
			}
			printf("%s%s.%s", nameSoFar, s2->name, field2->name);
			if(field1->next != NULL){
				if (mode){
					printf("&&");
				}
				else{
					printf("||");
				}
			}
		}
		else{
			eqExpStructs(field1, field2, mode, strcat(s1->name, "."));
		}
		field1 = field1->next;
		field2 = field2->next;
	}
	printf(")");
}


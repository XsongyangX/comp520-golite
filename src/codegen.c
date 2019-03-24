#include "codegen.h"
int bindingCounter = 0;

void codegenProgram(PROGRAM *p){

}
char *getNewBinding()
{
	char *tmp = malloc(32);
	strcat(tmp, "CODEGEN_BINDING");
	char tmpstr[32];
    sprintf(tmpstr, "[%d]", bindingCounter);
    strcat(tmp, tmpstr);
	bindingCounter++;
	return tmp;
}
/*general helper for slice and array structures*/
void genStructureHelper(char *tName, char *tMods, SYMBOL *sym, int depth, char *typeName, bool isTypeDef)
{
		
		if (*tMods != 0)
		{
			prettyTabs(depth);
			if(isTypeDef) printf("struct %s {\n", typeName);
			else printf("struct {\n");
			prettyTabs(depth+1);
				//slice case
			if(*(tMods+1) == ']')
			{
				printf("int size = 0;\n");
				prettyTabs(depth+1);
				tMods += 2;
				if(*tMods == 0)
				{
					if(strcmp(tName, "struct"))
					{
						printf("struct {\n");
						codegenStructHelper(sym->val.structFields, depth+2);
						prettyTabs(depth+1);
						printf("}val[32];\n");			
					}
					else if(strcmp(tName, "float64"))
					{
						printf("float val[32];\n");
					}
					else if(strcmp(tName, "rune"))
					{
						printf("char val[32];\n");
					}
					else if(strcmp(tName, "string"))
					{
						printf("char *val[32];\n");
					}
					else{
						printf("%s val[32];\n", tName);
					}
				}
				else{
					genStructureHelper(tName, tMods, sym, depth+1, "", false);
					prettyTabs(depth);
					printf("}val[32];\n");	
				}
			}
			else
			{
				int size = atoi(tMods+1);
				while(*tMods != ']')
				{
					tMods++;
				}
				tMods++;
				printf("int size = %d;\n", size);
				prettyTabs(depth+1);
				if(*tMods == 0)
				{
					if(strcmp(tName, "struct"))
					{
						printf("struct {\n");
						codegenStructHelper(sym->val.structFields, depth+2);
						prettyTabs(depth+1);
						printf("}val[%d];\n", size);			
					}
					else if(strcmp(tName, "float64"))
					{
						printf("float val[%d];\n", size);
					}
					else if(strcmp(tName, "rune"))
					{
						printf("char val[%d];\n", size);
					}
					else if(strcmp(tName, "string"))
					{
						printf("char *val[%d];\n", size);
					}
					else{
						printf("%s val[%d];\n", tName, size);
					}
				}
				else{
					genStructureHelper(tName, tMods, sym, depth+1, "", false);
					prettyTabs(depth);
					printf("}val[%d];\n", size);	
				}						
			}
		}
}
/*prints struct fields*/
void codegenStructHelper(SYMBOL *field, int depth)
{
	if(field == NULL) return;
	codegenStructHelper(field->next, depth);

	prettyTabs(depth);
	int curDepth = depth;
	char *tName = getTypeName(field);
	char *tMods = getTypeModifiers(field);
	if(*tMods == 0)
	{
		if(strcmp(tName, "struct"))
		{
			printf("struct {\n");
			codegenStructHelper(field->val.structFields, curDepth+1);
			prettyTabs(curDepth);
			printf("}%s;\n", field->name);			
		}
		else if(strcmp(tName, "float64"))
		{
			printf("float %s;\n", field->name);
		}
		else if(strcmp(tName, "rune"))
		{
			printf("char %s;\n", field->name);
		}
		else if(strcmp(tName, "string"))
		{
			printf("char *%s = malloc(128);\n", field->name);
		}
		else{
			printf("%s %s;\n", tName, field->name);
		}
	}
	else{
		genStructureHelper(tName, tMods, field, depth, "", false);
		prettyTabs(depth);
		printf("}%s;\n", field->name);
	}
	
	
}
SYMBOL *makeSymbolCopy(SYMBOL *sym)
{
	SYMBOL *copy = malloc(sizeof(SYMBOL));
	copy->name = sym->name;
	copy->kind = sym->kind;
	copy->t = sym->t;
	copy->val = sym->val;
	return copy;
}
char *findExistingBinding(SYMBOL *sym, symTable *table)
{
	SYMBOL *tmp = table->bindingsList;
	while(tmp != NULL)
	{
		if(MatchingTypes(sym, tmp, 0, false))
		{
			return tmp->bindingName;
		}
		tmp = tmp->next;
	}
	if(table->next != NULL)
	{
		return findExistingBinding(sym, table->next);
	}
	return "";
}
void codegenVarDecl(DECLARATION *decl, symTable *table, int depth)
{
	SYMBOL *tmp = lookupVar(table, decl->identifier, decl->lineno, false);
	/*avoid the use of reserved keywords in C*/
	if(strcmp(tmp->name, "int") == 0 || strcmp(tmp->name, "float") == 0 || strcmp(tmp->name, "bool") == 0 || strcmp(tmp->name, "char") == 0 || strcmp(tmp->name, "double") == 0)
	{
		tmp->bindingName = getNewBinding();
	}
	else{
		tmp->bindingName = tmp->name;
	}
	char *tName = findExistingBinding(tmp, table);
	if(strlen(tName) != 0)
	{
		printf("%s %s", tName, tmp->bindingName);
		if(decl->val.right != NULL)
		{
			printf(" = ");
			codegenExpression(decl->val.right, table);
		}
		printf(";\n");
	}
	else if(tmp->kind == varstructSym)//"inline" struct definition, can be grouped in an array structure or the like
	{
		/*make a custom type binding for every struct we define*/
		SYMBOL *copy = makeSymbolCopy(tmp);
		copy->bindingName = getNewBinding();
		copy->next = table->bindingsList;
		table->bindingsList = copy;
		table->bindingsSize++;
		char *tMods = getTypeModifiers(tmp);
		if(*tMods == 0)
		{
			prettyTabs(depth);
			printf("struct %s {\n", copy->bindingName);
			codegenStructHelper(tmp->val.structFields, depth+1);
			prettyTabs(depth);
			printf("}%s;\n", tmp->bindingName);
		}
		else{
			prettyTabs(depth);
			genStructureHelper("struct", tMods, tmp, depth, copy->bindingName, true);
			prettyTabs(depth);
			printf("}%s;\n", tmp->bindingName);
		}
	}
	else 
	{
		
		char *tName = getTypeName(tmp);
		char *tMods = getTypeModifiers(tmp);
		if(strlen(tMods) == 0)
		{
			if(strcmp(tName, "rune") == 0)
			{
				printf("char %s", tmp->bindingName);
			}
			else if(strcmp(tName, "string") == 0)
			{
				printf("char *%s", tName, tmp->bindingName);
			}
			else if (strcmp(tName, "float64") == 0){
				printf("float %s", tmp->bindingName);
			}
			else{//bool or int case
				printf("%s %s", tName, tmp->bindingName);
			}
			if(decl->val.right != NULL)
			{
				printf(" = ");
				codegenExpression(decl->val.right, table);
			}
			printf(";\n");
		}
		else{
			/*make a custom type binding for every struct we define*/
			SYMBOL *copy = makeSymbolCopy(tmp);
			copy->bindingName = getNewBinding();
			copy->next = table->bindingsList;
			table->bindingsList = copy;
			table->bindingsSize++;
			genStructureHelper(tName, tMods, tmp, depth, copy->bindingName, true);
			prettyTabs(depth);
			printf("}%s;\n", tmp->bindingName);
		}
	}
}
void codegenTypeDecl(DECLARATION *decl, symTable *table, int depth)
{
	SYMBOL *tmp = lookupType(table, decl->identifier, decl->lineno, false);
	/*avoid the use of reserved keywords in C*/
	if(strcmp(tmp->name, "int") == 0 || strcmp(tmp->name, "float") == 0 || strcmp(tmp->name, "bool") == 0 || strcmp(tmp->name, "char") == 0 || strcmp(tmp->name, "double") == 0)
	{
		tmp->bindingName = getNewBinding();
	}
	else{
		tmp->bindingName = tmp->name;
	}
	if(tmp->kind == structSym)
	{
		char *tMods = getTypeModifiers(tmp);
		if(*tMods == 0)
		{
			prettyTabs(depth);
			printf("struct %s {\n", tmp->bindingName);
			codegenStructHelper(tmp->val.structFields, depth+1);
			prettyTabs(depth);
			printf("};\n");
		}
		else{
			prettyTabs(depth);
			genStructureHelper("struct", tMods, tmp, depth, tmp->bindingName, true);
			prettyTabs(depth);
			printf("};\n");
		}
	}
	//TODO: May need to account for non struct type declarations...
}
/*Determines if any bindings need to be made for fn arguments and does so if needed*/
void codegenFuncDeclArgsCheck(SYMBOL *args, symTable *table)
{
	if(args == NULL)
	{
		return;
	}
	codegenFuncDeclArgsCheck(args->next, table);
	if(strcmp(args->name, "int") == 0 || strcmp(args->name, "float") == 0 || strcmp(args->name, "bool") == 0 || strcmp(args->name, "char") == 0 || strcmp(args->name, "double") == 0)
	{
		args->bindingName = getNewBinding();
	}
	else{
		args->bindingName = args->name;
	}
	char *tMods = getTypeModifiers(args);
	//No binding necessary
	if(strlen(tMods) == 0)
	{
		return;
	}
	char *tName = findExistingBinding(args, table);
	//binding already exists
	if(strlen(tName) != 0)
	{
		return;
	}
	//binding does not exist
	else
	{
		tName = getTypeName(args);
		SYMBOL *copy = makeSymbolCopy(args);
		copy->bindingName = getNewBinding();
		copy->next = table->bindingsList;
		table->bindingsList = copy;
		table->bindingsSize++;
		genStructureHelper(tName, tMods, args, 0, copy->bindingName, true);
		printf("};\n");
	}
	

}
/*codegens the fn declaration parameters*/
void codegenFuncDeclArgs(SYMBOL *args, symTable *table)
{
	if(args == NULL)
	{
		return;
	}
	codegenFuncDeclArgs(args->next, table);
	if(args->next != NULL) printf(", ");
	char *tName = findExistingBinding(args, table);
	//binding already exists
	if(strlen(tName) != 0)
	{
		printf("%s %s", tName, args->bindingName);
	}
	else
	{
		tName = getTypeName(args);
		printf("%s %s", tName, args->bindingName);
	}
	
}
void codegenFuncDecl(DECLARATION *decl, symTable *table, int depth)
{
	SYMBOL *tmp = lookupFunc(table, decl->val.f->identifier, decl->lineno, false);
	if(strcmp(tmp->name, "int") == 0 || strcmp(tmp->name, "float") == 0 || strcmp(tmp->name, "bool") == 0 || strcmp(tmp->name, "char") == 0 || strcmp(tmp->name, "double") == 0)
	{
		tmp->bindingName = getNewBinding();
	}
	else{
		tmp->bindingName = tmp->name;
	}
	codegenFuncDeclArgsCheck(tmp->val.func.funcParams, table);
	char *tName = findExistingBinding(tmp->val.func.returnSymRef, table);
	if(strlen(tName) != 0)
	{
		printf("%s %s(", tName, tmp->bindingName);
	}
	else
	{
		tName = getTypeName(tmp->val.func.returnSymRef);
		char *tMods = getTypeModifiers(tmp);
		SYMBOL *copy = makeSymbolCopy(tmp);
		copy->bindingName = getNewBinding();
		copy->next = table->bindingsList;
		table->bindingsList = copy;
		table->bindingsSize++;
		genStructureHelper(tName, tMods, tmp->val.func.returnSymRef, 0, copy->bindingName, true);
		printf("};\n");
		printf("%s %s(", tName, tmp->bindingName);
	}
	codegenFuncDeclArgs(tmp->val.func.funcParams, table);
	printf("{\n");
	codegenStatement(decl->val.f->body, depth+1);
	printf("}\n");
	

}
void codegenFuncCall(DECLARATION *decl, symTable *table)
{
	
}
void codegenDeclaration(DECLARATION *decl, symTable *table, int depth){
	if(decl == NULL) return;
	codegenDeclaration(decl->chain, table, depth);
	codegenDeclaration(decl->next, table, depth);
	switch(decl->d)
	{
		case varDecl:
			codegenVarDecl(decl, table, depth);
			break;
		case typeDecl:
			codegenTypeDecl(decl,table, depth);
			break;
		case structDecl:
			//should be unused
			fprintf(stderr, "StructDecl encountered.\n");
			exit(1);
		case funcDecl:
			codegenFuncDecl(decl, table, depth);
			break;
		case funcCall:
			codegenFuncCall(decl, table);
			break;
	}
}

void codegenStatement(STATEMENT *stmt, int depth){
	
}
/*do not use for varStructSym symbols*/
char *getTypeName(SYMBOL *tmp)
{
	if(tmp == NULL)
    {
        return " ";
    }
    if(tmp->kind == structSym)
                return "struct";
    char *typename = malloc(128);
    strcpy(typename, "");
    SYMBOL *cursym = tmp;
    TYPE *cur = tmp->t;
    if(cur == NULL || cur->gType == nilType)
    {
        strcat(typename, " ");//NULL type, i.e. void
        return typename;
    }
    while(1)
    {
        if(cur == NULL || cur->gType == nilType)
        {//move up hierarchy
            if(cursym->kind == funcSym)
            {
                cursym = cursym->val.func.returnSymRef;
                cur = cursym->t;
            }
            else{
                if(strcmp(cursym->val.parentSym->name, " ") == 0){
                    //next is the base type, end recursion now
                    strcat(typename, cursym->name);
                    return typename;
                }
                if(cursym->val.parentSym->kind == structSym){
                    strcat(typename, cursym->val.parentSym->name);
                    return typename;
                }
                cursym = cursym->val.parentSym;
                cur = cursym->t;
            }
            continue;
        }
        cur = cur->val.arg;//get next

    }
}
/*do not use for varStructSym symbols*/
char *getTypeModifiers(SYMBOL *tmp)
{
	if(tmp == NULL)
    {
        return "";
    }
    if(tmp->kind == structSym)
                return "";
    char *typename = malloc(128);
    strcpy(typename, "");
    SYMBOL *cursym = tmp;
    TYPE *cur = tmp->t;
    if(cur == NULL || cur->gType == nilType)
    {
        strcat(typename, " ");//NULL type, i.e. void
        return typename;
    }
    while(1)
    {
        if(cur == NULL || cur->gType == nilType)
        {//move up hierarchy
            if(cursym->kind == funcSym)
            {
                cursym = cursym->val.func.returnSymRef;
                cur = cursym->t;
            }
            else{
                if(strcmp(cursym->val.parentSym->name, " ") == 0){
                    //next is the base type, end recursion now
                    return typename;
                }
                if(cursym->val.parentSym->kind == structSym){
                    return typename;
                }
                cursym = cursym->val.parentSym;
                cur = cursym->t;
            }
            continue;
        }
        else if(cur->gType == arrayType){
            char tmpstr[32];
            sprintf(tmpstr, "[%d]", cur->size);
            strcat(typename, tmpstr);
        }
        else if(cur->gType == sliceType)
        {
            strcat(typename, "[]");
        }
        cur = cur->val.arg;//get next

    }
}
void codegenExpression(EXP *e, symTable *table){
	switch (e->kind){
		case emptyExp:
			break;
		case intExp:
			printf("%d", e->val.intLiteral);
			break; 
		case floatExp:
			printf("%f", e->val.floatLiteral);
			break;  
		case strExp: 
			printf("\"%s\"", e->val.strLiteral);
			break; 
		case rawstrExp:
			printf("\"%s\"", e->val.strLiteral);
			break; 
		case boolExp: 
			if (e->val.intLiteral != 0){
				printf("True");
			}
			else{
				printf("False");
			}
			break;
		case runeExp:
			printf("\'%c\'", e->val.runeLiteral);
			break;
		case plusExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("+");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case minusExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("-");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case timesExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("*");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case divExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("/");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case modExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("%%");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case notExp:
			printf("!");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case posExp:
			/*We should handle non-int types in here IIRC
			I just can't think of a way to do it neatly at the moment*/
			printf("abs(");
			codegenExpression(e->val.binary.rhs, table);
			printf(")");
			break;
		case negExp:
			printf("-");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case parExp:
			printf("(");
			codegenExpression(e->val.binary.rhs, table);
			printf(")");
			break;
		case eqExp:
			/*TODO: undeclared names, note that you cannot declare a 
			variable inside a case stmt. You should do so before the switch.*/
			if (e->val.binary.lhs->kind == idExp){
				SYMBOL *tmpSymb1 = getSymbol(table, e->val.binary.lhs->val.identifier, structSymbol);
				if (tmpStruct != NULL){
					SYMBOL *tmpSymb2 = getSymbol(table, e->val.binary.rhs->val.identifier, structSymbol);
					eqExpStructs(tmpSymbol1, tmpSymbol2, 1, "");
				}
				else{
					codegenExpression(e->val.binary.lhs, table);
					printf("==");
					codegenExpression(e->val.binary.rhs, table);
				}
			}
			else{
				codegenExpression(e->val.binary.lhs, table);
				printf("==");
				codegenExpression(e->val.binary.rhs, table);
			}
			break;
		case neqExp:
			/*TODO: undeclared names, note that you cannot declare a 
			variable inside a case stmt. You should do so before the switch.*/
			if (e->val.binary.lhs->kind == idExp){
				SYMBOL *tmpSymb1 = getSymbol(table, e->val.binary.lhs->val.identifier, structSymbol);
				if (tmpStruct != NULL){
					SYMBOL *tmpSymb2 = getSymbol(table, e->val.binary.rhs->val.identifier, structSymbol);
					eqExpStructs(tmpSymbol1, tmpSymbol2, 0, "");
				}
				else{
					codegenExpression(e->val.binary.lhs, table);
					printf("!=");
					codegenExpression(e->val.binary.rhs, table);
				}
			}
			else{
				codegenExpression(e->val.binary.lhs, table);
				printf("!=");
				codegenExpression(e->val.binary.rhs, table);
			}
			break;
		case geqExp:
			codegenExpression(e->val.binary.lhs, table);
			printf(">=");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case leqExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("<=");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case gtExp:
			codegenExpression(e->val.binary.lhs, table);
			printf(">");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case ltExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("<");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case orExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("||");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case andExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("&&");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case andnotExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("& -(");
			codegenExpression(e->val.binary.rhs, table);
			printf(")");
			break;
		case idExp:
			printf("%s", e->val.identifier);
			break;
		/*
		-------------------------
		UNFINISHED
		-------------------------
		case idblockExp:
		case expblockExp:
		*/
		case bitAndExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("&");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case bitOrExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("|");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case xorExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("^");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case lshiftExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("<<");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case rshiftExp:
			codegenExpression(e->val.binary.lhs, table);
			printf(">>");
			codegenExpression(e->val.binary.rhs, table);
			break;
		case indexExp:
			codegenExpression(e->val.binary.rhs, table);
			break;
		case elementExp:
			codegenExpression(e->val.binary.lhs, table);
			printf("[");
			codegenExpression(e->val.binary.rhs, table);
			printf("]");
			break;
		case invocExp:
			codegenExpression(e->val.binary.lhs, table);
			printf(".");
			codegenExpression(e->val.binary.rhs, table);
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
			printf("2147483647^(");
			codegenExpression(e->val.binary.rhs, table);
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
	SYMBOL *field1 = s1->val.structFields;
	SYMBOL *field2 = s2->val.structFields;
	while(field1 != NULL){
		if(field1->kind != structSym){
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


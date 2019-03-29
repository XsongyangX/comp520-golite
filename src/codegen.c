#include "codegen.h"
int bindingCounter;
int laggingCounter;
int forwardCounter;
continueLabel *continueList;
breakLabel *breakList;
char *getNewBinding()
{
	char *tmp = malloc(32);
	strcpy(tmp, "CODEGEN_BINDING");
	char tmpstr[32];
    sprintf(tmpstr, "%d", bindingCounter);
    strcat(tmp, tmpstr);
	bindingCounter++;
	return tmp;
}
void prettyTabular(int n)
{
	for(int i = 0; i < n; i++)
	{
		printf("\t");
	}
}
void lookForPlusString(EXP *e, int tabs)
{
	char *type = getTypeName(e->symTypeRef);
	switch (e->kind){
		case emptyExp:
		case intExp:
		case floatExp:  
		case strExp: 
		case rawstrExp:
		case boolExp: 
		case runeExp:
			return;
		case plusExp:
			if(strcmp(type, "string") == 0)
			{
				prettyTabular(tabs);
				printf("char *CODEGEN_BINDING_%d = malloc(128);\n", forwardCounter);
				forwardCounter++;
				lookForPlusString(e->val.binary.lhs, tabs);
				lookForPlusString(e->val.binary.rhs, tabs);
			}
			return;
		case minusExp:
		case timesExp:
		case divExp:
		case modExp:
			lookForPlusString(e->val.binary.lhs, tabs);
			lookForPlusString(e->val.binary.rhs, tabs);
			return;
		case notExp:
		case posExp:
		case negExp:
		case parExp:
			lookForPlusString(e->val.binary.rhs, tabs);
			return;
		case eqExp:
		case neqExp:
			lookForPlusString(e->val.binary.lhs, tabs);
			lookForPlusString(e->val.binary.rhs, tabs);
			return;
		case geqExp:
		case leqExp:
		case gtExp:
		case ltExp:
		case orExp:
		case andExp:
			lookForPlusString(e->val.binary.lhs, tabs);
			lookForPlusString(e->val.binary.rhs, tabs);
			return;
		case andnotExp:
			lookForPlusString(e->val.binary.lhs, tabs);
			lookForPlusString(e->val.binary.rhs, tabs);
			return;
		case idExp:
			return;
		case idblockExp:
			return;
		case expblockExp:
			if(e->val.expblock.next != NULL && e->val.expblock.value != NULL)
			{
				lookForPlusString(e->val.expblock.value, tabs);
				lookForPlusString(e->val.expblock.next, tabs);
				return;
			}
			else if(e->val.expblock.next != NULL)
			{
				lookForPlusString(e->val.expblock.next, tabs);
				return;
			}
			else if(e->val.expblock.value != NULL)
			{
				lookForPlusString(e->val.expblock.value, tabs);
				return; 
			}
			else return;
		case bitAndExp:
		case bitOrExp:
		case xorExp:
		case lshiftExp:
		case rshiftExp:
			lookForPlusString(e->val.binary.lhs, tabs);
			lookForPlusString(e->val.binary.rhs, tabs);
			return;
		case indexExp:
			lookForPlusString(e->val.binary.rhs, tabs);
			return;
		case elementExp:
		case invocExp:
		case appendExp:
			lookForPlusString(e->val.binary.lhs, tabs);
			lookForPlusString(e->val.binary.rhs, tabs);
			return;
		case lenExp:
		case capExp:
		case uxorExp:
			lookForPlusString(e->val.binary.rhs, tabs);
			return;
		case funcBlockExp:
			return lookForPlusString(e->val.fnblock.fn->params->val.fnCallBlock, tabs);
		default:
			printf("ERROR: Unknown expression type\n");
			exit(1);
	}
}

/*Goes through an expression tree to find appends and create bindings for them*/
void appendCheck(EXP *e, symTable *table, int depth)
{
	SYMBOL *parent;
	char *tName, *tMods;
	EXP *tmp;
	switch(e->kind)
	{
		case emptyExp:
		case intExp:
		case floatExp:  
		case strExp: 
		case rawstrExp:
		case boolExp: 
		case runeExp:
		case plusExp:
		case minusExp:
		case timesExp:
		case divExp:
		case modExp:
		case notExp:
		case posExp:
		case negExp:
			return;
		case parExp:
			appendCheck(e->val.binary.rhs, table, depth);
			return;
		case eqExp:
		case neqExp:
			return;
		case geqExp:
		case leqExp:
		case gtExp:
		case ltExp:
		case orExp:
		case andExp:
			return;
		case andnotExp:
			return;
		case idExp:
			return;
		case idblockExp:
			return;
		case expblockExp:
			return;
		case bitAndExp:
		case bitOrExp:
		case xorExp:
		case lshiftExp:
		case rshiftExp:
			return;
		case indexExp:
			return;
		case elementExp:
			appendCheck(e->val.binary.rhs, table, depth);
			return;
		case invocExp:
			return;
		case appendExp:
			appendCheck(e->val.binary.lhs, table, depth);
			appendCheck(e->val.binary.rhs, table, depth);
			sliceAppendHelper(e->val.binary.lhs, e->val.binary.rhs, table, depth);
			return;
		case lenExp:
		case capExp:
			appendCheck(e->val.binary.rhs, table, depth);
			return;
		case uxorExp:
			return;
		case funcBlockExp:
			tmp = e->val.fnblock.fn->params->val.fnCallBlock;
			while(tmp != NULL)
			{
				if(tmp->val.expblock.value != NULL)
				{
					appendCheck(tmp->val.expblock.value, table, depth);
				}
				tmp = tmp->val.expblock.next;
			}
			return;
		default:
			printf("ERROR: Unknown expression type\n");
			exit(1);
	}
}
/*general helper for slice and array structures
generates new symbols and binding mappings as it encounters new slice/array/struct types*/
void genStructureHelper(char *tName, char *tMods, SYMBOL *sym, int depth, char *typeName, bool isTypeDef, symTable *table)
{
		SYMBOL *copy = makeSymbol_Copy(sym);		
		SYMBOL *parent = getSymbol(table, tName, typeSym);
		if (*tMods != 0)
		{
			char *name1;
			prettyTabular(depth);
			if(isTypeDef) printf("struct %s {\n", typeName);
			else{
				name1 = findExistingBinding(copy, table);
				if(strlen(name1) == 0)
				{
					name1 = getNewBinding();
					copy->next = table->bindingsList;
					table->bindingsList = copy;
					copy->bindingName = name1;
				}
				printf("struct %s {\n", name1);
			}
			prettyTabular(depth+1);
				//slice case
			if(*(tMods+1) == ']')
			{
				while(copy->t->gType != sliceType)
				{
					copy = makeSymbol_Copy(copy->val.parentSym);
				}
				copy->t = copy->t->val.arg;
				printf("int size = 0;\n");
				prettyTabular(depth+1);
				printf("int cap = 0;\n");
				prettyTabular(depth+1);
				tMods += 2;
				if(*tMods == 0)
				{
					if(strcmp(tName, "struct") || parent != NULL && parent->kind == structSym)
					{
						char *name2;
						if(parent != NULL)
							name2 = findExistingBinding(parent, table);
						else
						{
							name2 = findExistingBinding(copy, table);
						}
						if(strlen(name2) == 0)
						{
							name2 = getNewBinding();
							copy->next = table->bindingsList;
							table->bindingsList = copy;
							copy->bindingName =  name2;
						}
						printf("struct %s {\n", name2);
						codegenStructHelper(sym->val.structFields, depth+2, table);
						prettyTabular(depth+1);
						printf("};\n");	
						prettyTabular(depth+1);
						printf("struct %s *val;\n", name2);		
					}
					else if(strcmp(tName, "float64"))
					{
						printf("float *val;\n");
					}
					else if(strcmp(tName, "rune"))
					{
						printf("char *val;\n");
					}
					else if(strcmp(tName, "string"))
					{
						printf("char **val;\n");
					}
					else{
						printf("%s *val;\n", tName);
					}
				}
				else{
					genStructureHelper(tName, tMods, sym, depth+1, "", false, table);
					prettyTabular(depth);
					printf("};\n");	
					prettyTabular(depth);
					printf("struct %s *val;\n", name1);
				}
			}
			else
			{
				while(copy->t->gType != arrayType)
				{
					copy = makeSymbol_Copy(copy->val.parentSym);
				}
				copy->t = copy->t->val.arg;
				int size = atoi(tMods+1);
				while(*tMods != ']')
				{
					tMods++;
				}
				tMods++;
				printf("int size = %d;\n", size);
				prettyTabular(depth+1);
				if(*tMods == 0)
				{
					if(strcmp(tName, "struct") || parent != NULL && parent->kind == structSym)
					{
						char *name2;
						if(parent != NULL)
							name2 = findExistingBinding(parent, table);
						else
						{
							name2 = findExistingBinding(copy, table);
						}
						
						if(strlen(name2) == 0)
						{
							name2 = getNewBinding();
							copy->next = table->bindingsList;
							table->bindingsList = copy;
							copy->bindingName = name2;
						}
						printf("struct %s {\n", name2);
						codegenStructHelper(sym->val.structFields, depth+2, table);
						prettyTabular(depth+1);
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
					genStructureHelper(tName, tMods, sym, depth+1, "", false, table);
					prettyTabular(depth);
					printf("}val[%d];\n", size);	
				}						
			}
		}
}
/*prints struct fields*/
void codegenStructHelper(SYMBOL *field, int depth, symTable *table)
{
	if(field == NULL) return;
	codegenStructHelper(field->next, depth, table);

	if(strcmp(field->name, "_") == 0)
	{
		return;
	}
	prettyTabular(depth);
	if(strcmp(field->name, "int") == 0 || strcmp(field->name, "float") == 0 || strcmp(field->name, "bool") == 0 || strcmp(field->name, "char") == 0 || strcmp(field->name, "double") == 0)
	{
		field->bindingName = getNewBinding();
	}
	else{
		field->bindingName = field->name;
	}
	int curDepth = depth;
	char *tName = getTypeName(field);
	char *tMods = getTypeModifiers(field);
	SYMBOL *parent = getSymbol(table, tName, typeSym);
	SYMBOL *copy = makeSymbol_Copy(field);
	if(*tMods == 0)
	{
		if(strcmp(tName, "struct") || parent != NULL && parent->kind == structSym)
		{
			char *name;
			if(parent != NULL)
				name = findExistingBinding(parent, table);
			else
			{
				name = findExistingBinding(copy, table);
			}
			if(strlen(name) == 0)
			{
				name = getNewBinding();
				copy->next = table->bindingsList;
				table->bindingsList = copy;
				copy->bindingName = name;
			}
			
			printf("struct %s {\n", name);
			codegenStructHelper(field->val.structFields, curDepth+1, table);
			prettyTabular(curDepth);
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
		genStructureHelper(tName, tMods, field, depth, "", false, table);
		prettyTabular(depth);
		printf("}%s;\n", field->name);
	}
	
	
}
SYMBOL *makeSymbol_Copy(SYMBOL *sym)
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
	char *tName1, *tMods1, *tName2, *tMods2;
	tName1 = getTypeName(sym);
	tMods1 = getTypeModifiers(sym);
	SYMBOL *tmp = table->bindingsList;
	while(tmp != NULL)
	{
		tName2 = getTypeName(tmp);
		tMods2 = getTypeModifiers(tmp);
		if(strcmp(tName1, tName2) == 0 && strcmp(tMods1, tMods2) == 0)
		{
			return tmp->bindingName;
		}
		tmp = tmp->next;
	}
	if(table->next != NULL)
	{
		return findExistingBinding(sym, table->next);
	}
	tName1 = malloc(1);
	strcpy(tName1, "");
	return tName1;
}
SYMBOL *findExistingBindingSym(SYMBOL *sym, symTable *table)
{
	char *tName1, *tMods1, *tName2, *tMods2;
	tName1 = getTypeName(sym);
	tMods1 = getTypeModifiers(sym);
	SYMBOL *tmp = table->bindingsList;
	while(tmp != NULL)
	{
		tName2 = getTypeName(tmp);
		tMods2 = getTypeModifiers(tmp);
		if(strcmp(tName1, tName2) == 0 && strcmp(tMods1, tMods2) == 0)
		{
			return tmp;
		}
		tmp = tmp->next;
	}
	if(table->next != NULL)
	{
		return findExistingBindingSym(sym, table->next);
	}
	return NULL;
}
void codegenVarDecl(DECLARATION *decl, symTable *table, int depth)
{
	SYMBOL *tmp = getSymbol(table, decl->identifier, varSym);
	if(decl->val.right != NULL)
	{
		lookForPlusString(decl->val.right, depth);
		appendCheck(decl->val.right, table, depth);
	}
	if(strcmp(decl->identifier, "_") == 0)
	{
		if(decl->val.right == NULL)
			return;
		else{
			tmp = makeSymbol_Copy(decl->val.right->symTypeRef);
			tmp->bindingName = getNewBinding();
		}
	}
	/*avoid the use of reserved keywords in C*/
	else{
		if(strcmp(tmp->name, "int") == 0 || strcmp(tmp->name, "float") == 0 || strcmp(tmp->name, "bool") == 0 || strcmp(tmp->name, "char") == 0 || strcmp(tmp->name, "double") == 0)
		{
			tmp->bindingName = getNewBinding();
		}
		else if(decl->val.right != NULL && (decl->val.right->kind == parExp || decl->val.right->kind == idExp))
		{
			EXP *right = decl->val.right;
			while(right->kind == parExp)
			{
				right = right->val.binary.rhs;
			}
			if(right->kind == idExp)
			{
				if(strcmp(right->val.identifier, decl->identifier) == 0)
				{
					tmp->bindingName = getNewBinding();
				}
			}
			else{
				tmp->bindingName = tmp->name;
			}
		}
		else{
			tmp->bindingName = tmp->name;
		}
	}
	if(decl->val.right != NULL)
	{
		lookForPlusString(decl->val.right, depth);
	}
	char *tMods = getTypeModifiers(tmp);
	SYMBOL *parent = findExistingBindingSym(tmp, table);
	if(parent != NULL)
	{
		prettyTabular(depth);
		printf("struct %s %s;\n", parent->bindingName, tmp->bindingName);
		if(decl->val.right != NULL)
		{
			if(strlen(tMods) == 0)
			{
				structAssignHelper(tmp, decl->val.right->symTypeRef, tmp->bindingName, getFullStr(decl->val.right, table), depth, table);
			}
			else if(*(tMods+1) == ']')
			{
				sliceAssignHelper(tmp->bindingName, getFullStr(decl->val.right, table), depth);
			}
			else{
				arrayAssignHelper(tmp, decl->val.right->symTypeRef, tMods, depth, tmp->bindingName, getFullStr(decl->val.right, table), table);
			}
		}
		else
		{
			if(strlen(tMods) == 0)
			{
				structAssignHelper(tmp, NULL, tmp->bindingName, NULL, depth, table);
			}
			else if(*(tMods+1) == ']')
			{
				sliceAssignHelper(tmp->bindingName, NULL, depth);
			}
			else{
				arrayAssignHelper(tmp, NULL, tMods, depth, tmp->bindingName, NULL, table);
			}
		}
		
	}
	else if(tmp->kind == varstructSym)//"inline" struct definition, can be grouped in an array structure or the like
	{
		/*make a custom type binding for every struct we define*/
		SYMBOL *copy = makeSymbol_Copy(tmp);
		copy->bindingName = getNewBinding();
		copy->next = table->bindingsList;
		table->bindingsList = copy;
		table->bindingsSize++;
		if(*tMods == 0)
		{
			prettyTabular(depth);
			printf("struct %s {\n", copy->bindingName);
			codegenStructHelper(tmp->val.structFields, depth+1, table);
			prettyTabular(depth);
			printf("}%s;\n", tmp->bindingName);
			structAssignHelper(copy, NULL, copy->bindingName, NULL, depth, table);
		}
		else{
			prettyTabular(depth);
			genStructureHelper("struct", tMods, tmp, depth, copy->bindingName, true, table);
			prettyTabular(depth);
			printf("}%s;\n", tmp->bindingName);
			if(*(tMods+1) == ']')
			{
				sliceAssignHelper(copy->bindingName, NULL, depth);
			}
			else{
				arrayAssignHelper(copy, NULL, tMods, depth, copy->bindingName, NULL, table);
			}
		}
	}
	else 
	{
		
		char *tName = getTypeName(tmp);
		if(strlen(tMods) == 0)
		{
			if(strcmp(tName, "rune") == 0)
			{
				printf("char %s = ", tmp->bindingName);
				if(decl->val.right != NULL)
				{
					codegenExpression(decl->val.right, table);
				}
				else{
					printf("0");
				}
				printf(";\n");
			}
			else if(strcmp(tName, "string") == 0)
			{
				printf("char *%s = malloc(128);\n", tmp->bindingName);
				prettyTabular(depth);
				
				if(decl->val.right != NULL)
				{
					printf("strcpy(%s, ", tmp->bindingName);
					codegenExpression(decl->val.right, table);
					printf(");\n");
				}
			}
			else if (strcmp(tName, "float64") == 0){
				printf("float %s = ", tmp->bindingName);
				if(decl->val.right != NULL)
				{
					codegenExpression(decl->val.right, table);
				}
				else{
					printf("0.0");
				}
				printf(";\n");
			}
			else{//bool or int case
				printf("%s %s = ", tName, tmp->bindingName);
				if(decl->val.right != NULL)
				{
					codegenExpression(decl->val.right, table);
				}
				else{
					printf("0");
				}
				printf(";\n");
			}
		}
		else{
			/*make a custom type binding for every struct we define*/
			SYMBOL *copy = makeSymbol_Copy(tmp);
			copy->bindingName = getNewBinding();
			copy->next = table->bindingsList;
			table->bindingsList = copy;
			table->bindingsSize++;
			genStructureHelper(tName, tMods, tmp, depth, copy->bindingName, true, table);
			prettyTabular(depth);
			printf("}%s;\n", tmp->bindingName);
			if(decl->val.right == NULL){
				if(*(tMods+1) == ']')
				{
					sliceAssignHelper(copy->bindingName, NULL, depth);
				}
				else{
					arrayAssignHelper(copy, NULL, tMods, depth, copy->bindingName, NULL, table);
				}
			}
			else{
				if(*(tMods+1) == ']')
				{
					sliceAssignHelper(copy->bindingName, getFullStr(decl->val.right,table), depth);
				}
				else{
					arrayAssignHelper(copy, decl->val.right->symTypeRef, tMods, depth, copy->bindingName, getFullStr(decl->val.right,table), table);
				}
			}
		}
	}
}
void codegenTypeDecl(DECLARATION *decl, symTable *table, int depth)
{
	if(strcmp(decl->identifier, "_") == 0)
	{
		return;
	}
	SYMBOL *tmp = getSymbol(table, decl->identifier, typeSym);
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
			prettyTabular(depth);
			printf("struct %s {\n", tmp->bindingName);
			codegenStructHelper(tmp->val.structFields, depth+1, table);
			prettyTabular(depth);
			printf("};\n");
		}
		else{
			prettyTabular(depth);
			genStructureHelper("struct", tMods, tmp, depth, tmp->bindingName, true, table);
			prettyTabular(depth);
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
	if(strcmp(args->name, "_") == 0)
	{
		args->bindingName = getNewBinding();
	}
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
		SYMBOL *copy = makeSymbol_Copy(args);
		copy->bindingName = getNewBinding();
		copy->next = table->bindingsList;
		table->bindingsList = copy;
		table->bindingsSize++;
		genStructureHelper(tName, tMods, args, 0, copy->bindingName, true, table);
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
	SYMBOL *tmp = getSymbol(table, decl->val.f->identifier, funcSym);
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
		SYMBOL *copy = makeSymbol_Copy(tmp);
		copy->bindingName = getNewBinding();
		copy->next = table->bindingsList;
		table->bindingsList = copy;
		table->bindingsSize++;
		genStructureHelper(tName, tMods, tmp->val.func.returnSymRef, 0, copy->bindingName, true, table);
		printf("};\n");
		printf("%s %s(", tName, tmp->bindingName);
	}
	codegenFuncDeclArgs(tmp->val.func.funcParams, table);
	printf("{\n");
	codegenStatement(decl->val.f->body, depth+1);
	printf("}\n");
	

}
/*set sym2 to NULL for imlicit initialization
nameSoFar should contain the name of the struct variable*/
void structAssignHelper(SYMBOL *sym1, SYMBOL *sym2, char *nameSoFar1, char *nameSoFar2, int depth, symTable *table)
{	
	char *tName, *tMods;
	SYMBOL *tmps1 = sym1, *tmps2 = sym2, *parent;
	if(sym2 == NULL)
	{
		
		while(tmps1->kind != structSym || tmps1->kind != varstructSym)
		{
			tmps1 = tmps1->val.parentSym;
		}
		
		SYMBOL *fields = tmps1->val.structFields;
		while(fields != NULL)
		{
			tMods = getTypeModifiers(fields);
			tName = getTypeName(fields);
			parent = getSymbol(table, tName, typeSym);
			if(*(tMods) == 0)
			{
				if(strcmp(tName, "string") == 0)
				{
					prettyTabular(depth);
					printf("%s.%s = malloc(128); ", nameSoFar1, fields->bindingName);
				}
				else if(strcmp(tName, "int") == 0 || strcmp(tName, "rune") == 0 || strcmp(tName, "bool") == 0)
				{
					prettyTabular(depth);
					printf("%s.%s = 0; ", nameSoFar1, fields->bindingName);
				}
				else if(strcmp(tName, "float64") == 0)
				{
					prettyTabular(depth);
					printf("%s.%s = 0.0; ", nameSoFar1, fields->bindingName);
				}
				else if(strcmp(tName, "struct") == 0 || parent != NULL && parent->kind == structSym)
				{
					char *nameSoFar = malloc(128);
					sprintf(nameSoFar, "%s.%s", nameSoFar1, fields->bindingName);
					structAssignHelper(fields, NULL, nameSoFar, NULL, depth, table);
				}
			}
			else{
				if(*(tMods+1) != ']')
				{
					char *tmp1, *tmp2;
					tmp1 = malloc(128);
					sprintf(tmp1, "%s.%s", nameSoFar1, fields->bindingName);
					arrayAssignHelper(fields, NULL, tMods, depth, tmp1, NULL, table);
				}
				else{
					char *tmp1, *tmp2;
					tmp1 = malloc(128);
					sprintf(tmp1, "%s.%s", nameSoFar1, fields->bindingName);
					sliceAssignHelper(tmp1, NULL, depth);
				}
			}
			fields = fields->next;
		}
	}
	else{
		
		while(tmps1->kind != structSym || tmps1->kind != varstructSym)
		{
			tmps1 = tmps1->val.parentSym;
			tmps2 = tmps2->val.parentSym;
		}
		
		SYMBOL *fields1, *fields2;
		fields1 = tmps1->val.structFields;
		fields2 = tmps2->val.structFields;
		while(fields1 != NULL)
		{
			tMods = getTypeModifiers(fields1);
			tName = getTypeName(fields1);
			parent = getSymbol(table, tName, typeSym);
			if(*(tMods) == 0)
			{
				if(strcmp(tName, "string") == 0)
				{
					prettyTabular(depth);
					printf("strcpy(%s.%s, %s.%s); \n", nameSoFar1, fields1->bindingName, nameSoFar2, fields2->bindingName);
				}
				else if(strcmp(tName, "struct") == 0 || parent != NULL && parent->kind == structSym)
				{
					char *tmp1, *tmp2;
					tmp1 = malloc(128);
					tmp2 = malloc(128);
					sprintf(tmp1, "%s.%s", nameSoFar1, fields1->bindingName);
					sprintf(tmp2, "%s.%s", nameSoFar2, fields2->bindingName);
					structAssignHelper(fields1, fields2, tmp1, tmp2, depth, table);
				}
				else{
					prettyTabular(depth);
					printf("%s.%s = %s.%s;\n ", nameSoFar1, fields1->bindingName, nameSoFar2, fields2->bindingName);
				}
			}
			else{
				if(*(tMods+1) != ']')
				{
					char *tmp1, *tmp2;
					tmp1 = malloc(128);
					tmp2 = malloc(128);
					sprintf(tmp1, "%s.%s", nameSoFar1, fields1->bindingName);
					sprintf(tmp2, "%s.%s", nameSoFar2, fields2->bindingName);
					arrayAssignHelper(fields1, fields2, tMods, depth, tmp1, tmp2, table);
				}
				else{
					char *tmp1, *tmp2;
					tmp1 = malloc(128);
					tmp2 = malloc(128);
					sprintf(tmp1, "%s.%s", nameSoFar1, fields1->bindingName);
					sprintf(tmp2, "%s.%s", nameSoFar2, fields2->bindingName);
					sliceAssignHelper(tmp1, tmp2, depth);
				}
			}
			fields1 = fields1->next;
			fields2 = fields2->next;
		}
	}
}
/*Helps with assignment to arrays.
nameSoFar should contain the name of the slice variable*/
void sliceAssignHelper(char *nameSoFar1, char *nameSoFar2, int depth)
{
	if(nameSoFar2 == NULL)
	{
		prettyTabular(depth);
		printf("%s.val = NULL; \n", nameSoFar1);
	}
	else {
		prettyTabular(depth);
		printf("%s.val = %s.val; \n", nameSoFar1, nameSoFar2);
		prettyTabular(depth);
		printf("%s.size = %s.size; \n", nameSoFar1, nameSoFar2);
		prettyTabular(depth);
		printf("%s.cap = %s.cap; \n", nameSoFar1, nameSoFar2);
	}
}
/*Helps with assignment to arrays.
sym1 is the symbol of the first array
sym2 is the symbol of the second array or NULL (for implicit initialization)
tMods is obtained through a call to getTypeModifiers
nameSoFar should contain the name of the array variable*/
void arrayAssignHelper(SYMBOL *sym1, SYMBOL *sym2, char *tMods, int depth, char *nameSoFar1, char *nameSoFar2, symTable *table)
{
	if(sym2 == NULL)
	{
		if(*(tMods) != 0)
		{
			if(*(tMods+1) == ']')
			{
				sliceAssignHelper(nameSoFar1, nameSoFar2, depth);
			}
			else{
				char *tmp = malloc(10);
				char *tmp1 = malloc(128);
				int size = atoi((tMods+1));
				int offset = 2;
				while(*(tMods+offset) != ']')
				{
					offset++;
				}
					offset++;
				for(int i = 0; i < size; i++)
				{
					sprintf(tmp1, "%s.val[%d]", nameSoFar1, i);
					arrayAssignHelper(sym1, sym2, tMods+offset, depth, tmp1, nameSoFar2, table);
				}

			}
		}
		else{
			char *tName = getTypeName(sym1);
			SYMBOL *parent = getSymbol(table, tName, typeSym);
			if(strcmp(tName, "string") == 0)
			{
				prettyTabular(depth);
				printf("%s = malloc(128);\n", nameSoFar1);
			}
			else if(strcmp(tName, "struct") == 0 || parent != NULL && parent->kind == structSym) 
			{
				structAssignHelper(sym1, sym2, nameSoFar1, nameSoFar2, depth, table);
			}
			else if(strcmp(tName, "float64"))
			{
				prettyTabular(depth);
				printf("%s = 0.0;\n", nameSoFar1);
			}
			else{
				prettyTabular(depth);
				printf("%s = 0;\n", nameSoFar1);
			}
		}
	}
	else{
		if(*(tMods) != 0)
		{
			if(*(tMods+1) == ']')
			{
				sliceAssignHelper(nameSoFar1, nameSoFar2, depth);
			}
			else{
				char *tmp = malloc(10);
				char *tmp1 = malloc(128);
				char *tmp2 = malloc(128);
				int size = atoi((tMods+1));
				int offset = 2;
				while(*(tMods+offset) != ']')
				{
					offset++;
				}
					offset++;
				for(int i = 0; i < size; i++)
				{
					sprintf(tmp1, "%s.val[%d]", nameSoFar1, i);
					sprintf(tmp2, "%s.val[%d]", nameSoFar2, i);
					arrayAssignHelper(sym1, sym2, tMods+offset, depth, tmp1, tmp2, table);
				}

			}
		}
		else{
			char *tName = getTypeName(sym1);
			SYMBOL *parent = getSymbol(table, tName, typeSym);
			if(strcmp(tName, "string") == 0)
			{
				prettyTabular(depth);
				printf("strcpy(%s, %s);\n", nameSoFar1, nameSoFar2);
			}
			else if(strcmp(tName, "struct") == 0 || parent != NULL && parent->kind == structSym) 
			{
				structAssignHelper(sym1, sym2, nameSoFar1, nameSoFar2, depth, table);
			}
			else{
				prettyTabular(depth);
				printf("%s = %s;\n", nameSoFar1, nameSoFar2);
			}
		}
	}
}
void sliceAppendHelper(EXP *target, EXP *value, symTable *table, int depth)
{
	char *name = getFullStr(target, table);
	char *newName = getNewBinding();
	target->symTypeRef->bindingName = newName;
	char *tName = findExistingBinding(target->symTypeRef, table);
	if(strlen(tName) == 0)
	{
		genStructureHelper(getTypeName(target->symTypeRef), getTypeModifiers(target->symTypeRef), target->symTypeRef, depth, "", false, table);
		prettyTabular(depth);
		printf("};\n");
		tName = findExistingBinding(target->symTypeRef, table);
	}
	SYMBOL *tmp = makeSymbol_Copy(target->symTypeRef);
	tmp->t = tmp->t->val.arg;
	
	prettyTabular(depth);
	printf("struct %s %s;\n", tName, newName);
	prettyTabular(depth);
	printf("%s.size = %s.size + 1;\n", newName, name);
	prettyTabular(depth);
	printf("if (%s.size+1 == %s.cap){\n", name, name);
	prettyTabular(depth+1);
	printf("%s.cap = %s.cap*2;\n", newName, name);
	
	prettyTabular(depth);
	printf("}\n");
	prettyTabular(depth);
	printf("else{ %s.cap = %s.cap; }\n", newName, name);
	prettyTabular(depth);
	printf("%s.val = malloc(sizeof(struct %s)*%s.cap);\n", newName, findExistingBinding(tmp, table), newName);
	prettyTabular(depth);
	printf("for( int codegen_i = 0; codegen_i < %s.cap; codegen_i++){\n", name);
	prettyTabular(depth+1);
	printf("%s.val[codegen_i] = %s.val[codegen_i];\n", newName, name);
	prettyTabular(depth);
	printf("}\n");
	printf("%s.val[%s.size-1] = ", newName, newName);
	codegenExpression(value, table);
	printf(";\n");
}
void codegenFuncCall(EXP *e, SYMBOL* sym, symTable *table)
{
	//type cast case
	if(sym->kind == typeSym)
	{
		codegenExpression(e->val.expblock.value, table);
		return;
	}
	//else
	if(e == NULL)
	{
		return;
	}
	if(e->val.expblock.next != NULL)
	{
		codegenFuncCall(e->val.expblock.next, sym->next, table);
	}
	if(e->val.expblock.value != NULL)
	{
		codegenExpression(e->val.expblock.value, table);	
	}
}
char *strFuncCall(EXP *e, SYMBOL* sym, symTable *table)
{
	//type cast case
	if(sym->kind == typeSym)
	{
		return getFullStr(e->val.expblock.value, table);
		
	}
	//else
	if(e == NULL)
	{
		return "";
	}
	char *returnVal = malloc(128);
	char *tmp;
	if(e->val.expblock.next != NULL)
	{
		tmp = strFuncCall(e->val.expblock.next, sym->next, table);
	}
	if(e->val.expblock.value != NULL)
	{
			returnVal = getFullStr(e->val.expblock.value, table);
			if(tmp != NULL)
			{
				return strcat(strcat(tmp, ", "), returnVal);
			}
			return returnVal;
		
	}
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
			//handle in exp case
			fprintf(stderr, "funcCall encountered.\n");
			exit(1);
	}
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

/*Assigns to a temporary variable first, in a left to right order if multiple assignments are present
codegenAssignLast assigns the temporary to the actual target*/
void codegenAssign(STATEMENT *stmt, int depth){
	if(stmt == NULL) return;
	codegenAssign(stmt->val.assignment.chain, depth);
	EXP *tmpExp= stmt->val.assignment.identifier;
	tmpExp->symTypeRef = makeSymbol_Copy(tmpExp->symTypeRef);
	char *tName, *tMods;
	tmpExp->symTypeRef->bindingName = getNewBinding();
	SYMBOL *parent = findExistingBindingSym(tmpExp->symTypeRef, stmt->localScope);
	tName = getTypeName(tmpExp->symTypeRef);
	tMods = getTypeModifiers(tmpExp->symTypeRef);
	if(parent == NULL)
	{
		if(strcmp(tName, "string") == 0)
		{
			prettyTabular(depth);
			printf("char *%s = malloc(128); strcpy(%s, ", tmpExp->symTypeRef->bindingName, tmpExp->symTypeRef->bindingName);
			codegenExpression(stmt->val.assignment.value, stmt->localScope);
			printf(");\n");
		}
		else if(strcmp(tName, "int") == 0)
		{
			prettyTabular(depth);
			printf("int %s = ", tmpExp->symTypeRef->bindingName);
			codegenExpression(stmt->val.assignment.value, stmt->localScope);
			printf(";\n");
		}
		else if(strcmp(tName, "rune") == 0)
		{
			prettyTabular(depth);
			printf("char %s = ", tmpExp->symTypeRef->bindingName);
			codegenExpression(stmt->val.assignment.value, stmt->localScope);
			printf(";\n");
		}
		else if(strcmp(tName, "float64") == 0)
		{
			prettyTabular(depth);
			printf("float %s = ", tmpExp->symTypeRef->bindingName);
			codegenExpression(stmt->val.assignment.value, stmt->localScope);
			printf(";\n");
		}
		else if(strcmp(tName, "bool") == 0)
		{
			prettyTabular(depth);
			printf("int %s = ", tmpExp->symTypeRef->bindingName);
			codegenExpression(stmt->val.assignment.value, stmt->localScope);
			printf(";\n");
		}
	}
	else{
		if(*(tMods) != 0)
		{
			if(*(tMods+1) == ']')
			{
				printf("struct %s %s;\n", parent->bindingName, tmpExp->symTypeRef->bindingName);
				sliceAssignHelper(tmpExp->symTypeRef->bindingName, NULL, depth);
				sliceAssignHelper(tmpExp->symTypeRef->bindingName, getFullStr(stmt->val.assignment.value, stmt->localScope), depth);
			}
			else{
				printf("struct %s %s;\n", parent->bindingName, tmpExp->symTypeRef->bindingName);
				arrayAssignHelper(tmpExp->symTypeRef, NULL, tMods, depth, tmpExp->symTypeRef->bindingName, NULL, stmt->localScope);
				arrayAssignHelper(tmpExp->symTypeRef, stmt->val.assignment.value->symTypeRef, tMods, depth, tmpExp->symTypeRef->bindingName, getFullStr(stmt->val.assignment.value, stmt->localScope), stmt->localScope);
			}
		}
		else if(parent->kind == structSym || parent->kind == varstructSym)
		{
			printf("struct %s %s;\n", parent->bindingName, tmpExp->symTypeRef->bindingName);
			structAssignHelper(tmpExp->symTypeRef, NULL, tmpExp->symTypeRef->bindingName, NULL, depth, stmt->localScope);
			structAssignHelper(tmpExp->symTypeRef, stmt->val.assignment.value->symTypeRef, tmpExp->symTypeRef->bindingName, getFullStr(stmt->val.assignment.value, stmt->localScope), depth, stmt->localScope);
		}
		else{
			if(strcmp(tName, "string") == 0)
			{
				prettyTabular(depth);
				printf("char *%s = malloc(128); strcpy(%s, ", tmpExp->symTypeRef->bindingName, tmpExp->symTypeRef->bindingName);
				codegenExpression(stmt->val.assignment.value, stmt->localScope);
				printf(");\n");
			}
			else if(strcmp(tName, "int") == 0)
			{
				prettyTabular(depth);
				printf("int %s = ", tmpExp->symTypeRef->bindingName);
				codegenExpression(stmt->val.assignment.value, stmt->localScope);
				printf(";\n");
			}
			else if(strcmp(tName, "rune") == 0)
			{
				prettyTabular(depth);
				printf("char %s = ", tmpExp->symTypeRef->bindingName);
				codegenExpression(stmt->val.assignment.value, stmt->localScope);
				printf(";\n");
			}
			else if(strcmp(tName, "float64") == 0)
			{
				prettyTabular(depth);
				printf("float %s = ", tmpExp->symTypeRef->bindingName);
				codegenExpression(stmt->val.assignment.value, stmt->localScope);
				printf(";\n");
			}
			else if(strcmp(tName, "bool") == 0)
			{
				prettyTabular(depth);
				printf("int %s = ", tmpExp->symTypeRef->bindingName);
				codegenExpression(stmt->val.assignment.value, stmt->localScope);
				printf(";\n");
			}
		}
	}
	
}
void codegenAssignLast(STATEMENT *stmt, int depth)
{
	if(stmt == NULL)
	{
		return;
	}
	codegenAssignLast(stmt->val.assignment.chain, depth);
	prettyTabular(depth);
	printf("%s = %s;\n", getFullStr(stmt->val.assignment.identifier, stmt->localScope), stmt->val.assignment.identifier->symTypeRef->bindingName);
}

/*Handles if statements*/
void codegenIf(STATEMENT *stmt, int depth){
	prettyTabular(depth);
	printf("{\n");
	if(stmt->val.conditional.optDecl != NULL)
		codegenStatement(stmt->val.conditional.optDecl, depth+1);
	prettyTabular(depth+1);
	printf("if (");
	codegenExpression(stmt->val.conditional.condition, stmt->localScope);
	printf("){\n");
	STATEMENT *tmpStmt = stmt->val.conditional.body;
	if(tmpStmt != NULL){
		codegenStatement(tmpStmt, depth+2);
	}
	prettyTabular(depth+1);
	printf("}\n");
	if(stmt->val.conditional.elif != NULL){
		codegenStatement(stmt->val.conditional.elif, depth+1);
	}
	prettyTabular(depth);
	printf("}\n");
}

/*Handles else statements*/ 
void codegenElse(STATEMENT *stmt, int depth){
	if(stmt->val.conditional.optDecl != NULL)
		codegenStatement(stmt->val.conditional.optDecl, depth);
	printf("else{\n");
	STATEMENT *tmpStmt = stmt->val.conditional.body;
	if(tmpStmt != NULL){
		codegenStatement(tmpStmt, depth+1);
	}
	prettyTabular(depth);
	printf("}\n");
}

/*Handles elif statements.
For scoping reasons, the else and if are separated onto their own lines
And quick declarations come inside the else but not the subsequent if.*/
void codegenElif(STATEMENT *stmt, int depth){
	
	prettyTabular(depth);
	printf("else{\n");
	if(stmt->val.conditional.optDecl != NULL)
		codegenStatement(stmt->val.conditional.optDecl, depth+1);
	prettyTabular(depth+1);
	printf("if(");
	codegenExpression(stmt->val.conditional.condition, stmt->localScope);
	printf("){\n");
	STATEMENT *tmpStmt = stmt->val.conditional.body;
	if(tmpStmt != NULL){
		codegenStatement(tmpStmt, depth+2);
	}
	prettyTabular(depth+1);
	printf("}\n");
	prettyTabular(depth);
	printf("}\n");
}

/*Handles quick declarations */
void codegenQuickDecl(STATEMENT *stmt, int depth){
	
	if(stmt == NULL)
	{
		return;
	}
	codegenQuickDecl(stmt->val.assignment.chain, depth);
	//left of short decl is always an identifier
	if(strlen(lookupVarLocal(stmt->localScope, stmt->val.assignment.identifier->val.identifier)) != 0)
	{
		prettyTabular(depth);
		printf("%s = %s;\n", getFullStr(stmt->val.assignment.identifier, stmt->localScope), stmt->val.assignment.identifier->symTypeRef->bindingName);
	}
	else{
		char *tName, *tMods;
		SYMBOL *parent;
		tName = findExistingBinding(stmt->val.assignment.identifier->symTypeRef, stmt->localScope);
		if(strlen(tName) > 0)
		{
			prettyTabular(depth);
			printf("struct %s %s = %s;\n", tName, getFullStr(stmt->val.assignment.identifier, stmt->localScope), stmt->val.assignment.identifier->symTypeRef->bindingName);
		}
		else{
			tName = getTypeName(stmt->val.assignment.identifier->symTypeRef);
			//tMods = getTypeModifiers(stmt->val.assignment.identifier->symTypeRef);
			printf("%s %s = %s",tName, getFullStr(stmt->val.assignment.identifier, stmt->localScope), stmt->val.assignment.identifier->symTypeRef->bindingName);
		}
	}
	
}


void codegenProgram(PROGRAM *p){
	laggingCounter = 0;
	forwardCounter = 0;
	bindingCounter = 0;
	breakList = NULL;
	continueList = NULL;
	printf("#include <stdio.h>\n");
	printf("#include <string.h>\n");
	printf("#include <stdlib.h>\n");
	printf("#include <math.h>\n");
	
	DECLARATION *tmpDecl = p->declList;
	codegenDeclaration(tmpDecl, p->globalScope, 0);
}


void codegenPrint(EXP *e, symTable *table, bool hasNewline, int depth)
{
	if(e == NULL)
	{
		return;
	}
	if(e->val.expblock.next != NULL)
	{
		codegenPrint(e->val.expblock.next, table, hasNewline, depth);
	}
	if(e->val.expblock.value == NULL) return;
	EXP *tmpe = e->val.expblock.value;
	tmpe->symTypeRef = makeSymbol_Copy(tmpe->symTypeRef);
	lookForPlusString(tmpe, depth);
	appendCheck(tmpe, table, depth);

	char *tName = getTypeName(tmpe->symTypeRef);
	if(strcmp(tName, "string") == 0)
	{
		prettyTabular(depth);
		printf("printf(\"%%s\", ");
		codegenExpression(tmpe, table);
		printf("); ");
	}
	else if(strcmp(tName, "float64") == 0)
	{
		prettyTabular(depth);
		printf("printf(\"%%f\",");
		codegenExpression(tmpe, table);
		printf("); ");
	}
	else if(strcmp(tName, "bool") == 0)
	{
		prettyTabular(depth);
		printf("if(");
		codegenExpression(tmpe, table);
		printf("){ printf(\"True\");} else{ printf(\"False\");}");
	}
	else{
		prettyTabular(depth);
		printf("printf(\"%%d\",");
		codegenExpression(tmpe, table);
		printf("); ");
	}
	if(hasNewline)
	{
		printf("printf(\" \");\n");
	}
	else{
		printf("\n");
	}
}

void codegenFor(STATEMENT *stmt, int depth)
{
	prettyTabular(depth);
	printf("{\n");
	prettyTabular(depth+1);
	codegenStatement(stmt->val.conditional.optDecl, depth+1);
	prettyTabular(depth+1);
	printf("while (");
	if(stmt->val.conditional.condition == NULL)
	{
		printf("1){\n");
	}
	else{
		codegenExpression(stmt->val.conditional.condition, stmt->localScope);
		printf("){\n");
	}
	int contId, breakId;
	if(continueList == NULL)
	{
		contId = -1;
	}
	else{
		contId = continueList->id;
	}
	if(breakList == NULL)
	{
		breakId = -1;
	}
	else{
		breakId = breakList->id;
	}
	codegenStatement(stmt->val.conditional.body, depth+2);
	while(continueList != NULL || continueList->id != contId)
	{
		prettyTabular(depth+2);
		printf("CONTINUE_LABEL%d:\n", continueList->id);
		continueList = continueList->next;
	}
	codegenStatement(stmt->val.conditional.elif, depth+1);
	prettyTabular(depth+2);
	printf("continue;\n");
	while(breakList != NULL && breakList->id != breakId)
	{
		prettyTabular(depth+2);
		printf("BREAK_LABEL%d:\n", breakList->id);
		breakList = breakList->next;
	}
	codegenStatement(stmt->val.conditional.elif, depth+1);
	prettyTabular(depth+2);
	printf("break;\n");
	prettyTabular(depth+1);
	printf("}\n");
	prettyTabular(depth);
	printf("}\n");
	
}

void codegenWhile(STATEMENT *stmt, int depth)
{
	prettyTabular(depth);
	printf("while(");
	if(stmt->val.conditional.condition == NULL)
	{
		printf("1){\n");
	}
	else{
		codegenExpression(stmt->val.conditional.condition, stmt->localScope);
		printf("){\n");
	}
	int contId, breakId;
	if(continueList == NULL)
	{
		contId = -1;
	}
	else{
		contId = continueList->id;
	}
	if(breakList == NULL)
	{
		breakId = -1;
	}
	else{
		breakId = breakList->id;
	}
	codegenStatement(stmt->val.conditional.body, depth+1);
	while(continueList != NULL && continueList->id != contId)
	{
		prettyTabular(depth+1);
		printf("CONTINUE_LABEL%d:\n", continueList->id);
		continueList = continueList->next;
	}
	codegenStatement(stmt->val.conditional.elif, depth+1);
	prettyTabular(depth+1);
	printf("continue;\n");
	while(breakList != NULL || breakList->id != breakId)
	{
		prettyTabular(depth+1);
		printf("BREAK_LABEL%d:\n", breakList->id);
		breakList = breakList->next;
	}
	codegenStatement(stmt->val.conditional.elif, depth+1);
	prettyTabular(depth+1);
	printf("break;\n");
	prettyTabular(depth);
	printf("}\n");

}

void codegenSwitch(STATEMENT *stmt, int depth)
{
	prettyTabular(depth);
	printf("{\n");
	prettyTabular(depth+1);
	codegenStatement(stmt->val.switchBody.optDecl, depth+1);
	int breakId;
	if(breakList == NULL)
	{
		breakId = -1;
	}
	else{
		breakId = breakList->id;
	}
	codegenCase(stmt->val.switchBody.cases, stmt->val.switchBody.condition, depth+1);
	//close the else branches that are still open
	STATEMENT *list = stmt->val.switchBody.cases;
	while(list != NULL)
	{
		if(list->val.caseBody.condition == NULL)
		{
			list = list->next;
			continue;
		}	
		prettyTabular(depth+1);
		printf("}\n");
		list = list->next;
	}
	while(breakList != NULL && breakList->id != breakId)
	{
		prettyTabular(depth+1);
		printf("BREAK_LABEL%d:\n", breakList->id);
		breakList = breakList->next;
	}
	prettyTabular(depth);
	printf("}\n");
}

void codegenCase(STATEMENT *stmt, EXP *ref, int depth)
{
	if(stmt == NULL) return;
	codegenCase(stmt->next, ref, depth);
	prettyTabular(depth);
	if(stmt->val.caseBody.condition != NULL)
	{
		printf("if ( ");
		EXP *condition;
		EXP *cur = stmt->val.caseBody.condition;
		while(cur != NULL)
		{
			if(cur->val.expblock.value == NULL)
			{
				cur = cur->val.expblock.next;
				continue;
			}
			if(ref == NULL)
			{
				condition = cur->val.expblock.value;
			}
			else{
				condition = makeEXP_eq(cur->val.expblock.value, ref);
			}
			cur = cur->val.expblock.next;
			
			codegenExpression(condition, stmt->localScope);
		}
		printf("){\n");
		codegenStatement(stmt->val.caseBody.body, depth+1);
		prettyTabular(depth);
		printf("}\n");
		prettyTabular(depth);
		printf("else{\n");

	}
	else{
		breakLabel *bl;
		codegenStatement(stmt->val.caseBody.body, depth);
		prettyTabular(depth);
		printf("goto BREAK_LABEL%d;\n", bindingCounter);
		bl = malloc(sizeof(breakLabel));
		bl->next = breakList;
		breakList = bl;
		bl->id = bindingCounter;
		bindingCounter++;
	}
}
void codegenStatement(STATEMENT *stmt, int depth){
	continueLabel *cl;
	breakLabel *bl;
	if(stmt == NULL)
	{
		return;
	}
	codegenStatement(stmt->next, depth);
	switch(stmt->kind){
		case emptyS:
			break; 
		case assignS:
			codegenAssign(stmt, depth);
			codegenAssignLast(stmt, depth);
			break;
		case quickDeclS:
			codegenAssign(stmt, depth);
			codegenQuickDecl(stmt, depth);
			break;
		case blockS:
			prettyTabular(depth);
			printf("{\n");
			codegenStatement(stmt->val.body, depth+1);
			prettyTabular(depth);
			printf("}\n");
			break;
		case ifS:
			codegenIf(stmt, depth);
			break;
		case elifS:
			codegenElif(stmt, depth);
			break;
		case elseS:
			codegenElse(stmt, depth);
			break;
		case forS:
			codegenFor(stmt, depth);
			break;
		case whileS:
			codegenWhile(stmt, depth);
			break;
		case printS:
			if(stmt->val.iostmt.hasNewLine == 0 && stmt->val.iostmt.value == NULL)
			{
				break;
			}
			else{
				codegenPrint(stmt->val.iostmt.value, stmt->localScope, stmt->val.iostmt.hasNewLine, depth);
				if(stmt->val.iostmt.hasNewLine)
				{
					prettyTabular(depth);
					printf("printf(\"\\n\");\n");
				}
			}
		case exprS:
			lookForPlusString(stmt->val.expression, depth);
			appendCheck(stmt->val.expression, stmt->localScope, depth);
			codegenExpression(stmt->val.expression, stmt->localScope);
			break;
		case returnS:
			if(stmt->val.expression != NULL)
			{
				lookForPlusString(stmt->val.expression, depth);
				appendCheck(stmt->val.expression, stmt->localScope, depth);
				prettyTabular(depth);
				printf(" return ");
				codegenExpression(stmt->val.expression, stmt->localScope);
				printf(";\n");
			}
			else{
				prettyTabular(depth);
				printf("return;\n");
			}
			break;
		case switchS:
			codegenSwitch(stmt, depth);
		case caseS:
			break;
		case breakS:
			prettyTabular(depth);
			printf("goto BREAK_LABEL%d;\n", bindingCounter);
			bl = malloc(sizeof(breakLabel));
			bl->next = breakList;
			breakList = bl;
			bl->id = bindingCounter;
			bindingCounter++;
			break;
		case continueS:
			prettyTabular(depth);
			printf("goto CONTINUE_LABEL%d;\n", bindingCounter);
			cl = malloc(sizeof(continueLabel));
			cl->next = continueList;
			continueList = cl;
			cl->id = bindingCounter;
			bindingCounter++;
			break;
		case declS:
			codegenDeclaration(stmt->val.declaration, stmt->localScope, depth);
			break;
		case incrementS:
			prettyTabular(depth);
			codegenExpression(stmt->val.expression, stmt->localScope);
			printf("++;\n");
			break;
		case decrementS:
			prettyTabular(depth);
			codegenExpression(stmt->val.expression, stmt->localScope);
			printf("--;\n");
			break;
	}
}

void codegenExpression(EXP *e, symTable *table){
	SYMBOL *parent;
	EXP *tmpe;
	char *tMods, *tName;
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
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "string") == 0)
			{
				laggingCounter++;
				printf("strcat(strcat(CODEGEN_BINDING%d, ", laggingCounter-1);
				codegenExpression(e->val.binary.lhs, table);
				printf("), ");
				codegenExpression(e->val.binary.rhs, table);
				printf(")");
			}
			else{
				codegenExpression(e->val.binary.lhs, table);
				printf("+");
				codegenExpression(e->val.binary.rhs, table);
			}
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
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "int") == 0 || strcmp(tName, "rune") == 0)
			{
				printf("abs(");
				codegenExpression(e->val.binary.rhs, table);
				printf(")");
				break;
			}
			else if(strcmp(tName, "float64") == 0)
			{
				printf("fabs(");
				codegenExpression(e->val.binary.rhs, table);
				printf(")");
				break;
			}
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
			tMods = getTypeModifiers(e->val.binary.lhs->symTypeRef);
			tName = getTypeName(e->val.binary.lhs->symTypeRef);
			parent = getSymbol(table, tName, typeSym);
			if(strlen(tMods) != 0)
			{
				eqExpArrays(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, tMods, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
			}
			else if(e->val.binary.lhs->symTypeRef->kind == varstructSym || e->val.binary.lhs->symTypeRef->kind == structSym || parent != NULL && parent->kind == structSym)
			{
				eqExpStructs(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
			}
			else if(strcmp(tName, "string") == 0)
			{
				printf("strcmp(");
				codegenExpression(e->val.binary.lhs, table);
				printf(", ");
				codegenExpression(e->val.binary.rhs, table);
				printf(") == 0");
			}
			else{
				codegenExpression(e->val.binary.lhs, table);
				printf("==");
				codegenExpression(e->val.binary.rhs, table);
			}
			break;
		case neqExp:
			tMods = getTypeModifiers(e->val.binary.lhs->symTypeRef);
			tName = getTypeName(e->val.binary.lhs->symTypeRef);
			parent = getSymbol(table, tName, typeSym);
			if(strlen(tMods) != 0)
			{
				printf("!(");
				eqExpArrays(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, tMods, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
				printf(")");
			}
			else if(e->val.binary.lhs->symTypeRef->kind == varstructSym || e->val.binary.lhs->symTypeRef->kind == structSym || parent != NULL && parent->kind == structSym)
			{
				printf("!(");
				eqExpStructs(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
				printf(")");
			}
			else if(strcmp(tName, "string") == 0)
			{
				printf("strcmp(");
				codegenExpression(e->val.binary.lhs, table);
				printf(", ");
				codegenExpression(e->val.binary.rhs, table);
				printf(") != 0");
			}
			else{
				codegenExpression(e->val.binary.lhs, table);
				printf("!=");
				codegenExpression(e->val.binary.rhs, table);
			}
			break;
		case geqExp:
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "string") == 0)
			{
				printf("strcmp(");
				codegenExpression(e->val.binary.lhs, table);
				printf(", ");
				codegenExpression(e->val.binary.rhs, table);
				printf(") >= 0");
			}
			else
			{
				codegenExpression(e->val.binary.lhs, table);
				printf(">=");
				codegenExpression(e->val.binary.rhs, table);
			}
			
			break;
		case leqExp:
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "string") == 0)
			{
				printf("strcmp(");
				codegenExpression(e->val.binary.lhs, table);
				printf(", ");
				codegenExpression(e->val.binary.rhs, table);
				printf(") <= 0");
			}
			else
			{
				codegenExpression(e->val.binary.lhs, table);
				printf("<=");
				codegenExpression(e->val.binary.rhs, table);
			}
			break;
		case gtExp:
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "string") == 0)
			{
				printf("strcmp(");
				codegenExpression(e->val.binary.lhs, table);
				printf(", ");
				codegenExpression(e->val.binary.rhs, table);
				printf(") > 0");
			}
			else
			{
				codegenExpression(e->val.binary.lhs, table);
				printf(">");
				codegenExpression(e->val.binary.rhs, table);
			}
			break;
		case ltExp:
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "string") == 0)
			{
				printf("strcmp(");
				codegenExpression(e->val.binary.lhs, table);
				printf(", ");
				codegenExpression(e->val.binary.rhs, table);
				printf(") < 0");
			}
			else
			{
				codegenExpression(e->val.binary.lhs, table);
				printf("<");
				codegenExpression(e->val.binary.rhs, table);
			}
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
			printf("&^(");
			codegenExpression(e->val.binary.rhs, table);
			printf(")");
			break;
		case idExp:
			parent = getSymbol(table, e->val.identifier, varSym);
			printf("%s", parent->bindingName);
			break;
		/*
		-------------------------
		UNFINISHED
		-------------------------
		case idblockExp:
		
		*/
		case expblockExp:
			if(e->val.expblock.value != NULL && e->val.expblock.next != NULL)
			{
				codegenExpression(e->val.expblock.next, table);
				printf(", ");
				codegenExpression(e->val.expblock.value, table);
			}
			else if(e->val.expblock.value != NULL)
			{
				codegenExpression(e->val.expblock.value, table);
			}
			else if(e->val.expblock.next != NULL)
			{
				codegenExpression(e->val.expblock.next, table);
			}
			break;
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
		case funcBlockExp:
			tmpe = e->val.fnblock.identifier;
			while(tmpe->kind != idExp)
			{
				tmpe = tmpe->val.binary.rhs;
			}
			parent = getSymbol(table, tmpe->val.identifier, funcSym);
			if(parent == NULL)
			{
				parent = getSymbol(table, tmpe->val.identifier, typeSym);
				tName = getTypeName(parent);
				if(strcmp(tName, "float64") == 0)
				{
					parent->bindingName = malloc(10);
					strcpy(parent->bindingName, "float");
				}
				else if(strcmp(tName, "rune") == 0)
				{
					parent->bindingName = malloc(10);
					strcpy(parent->bindingName, "char");
				}
				else if(strcmp(tName, "string") == 0)
				{
					parent->bindingName = malloc(10);
					strcpy(parent->bindingName, "char");
				}
				else if(strcmp(tName, "bool") == 0)
				{
					parent->bindingName = malloc(10);
					strcpy(parent->bindingName, "int");
				}
				else{
					parent->bindingName = tName;
				}
				printf("%s(", parent->bindingName);
				if(e->val.fnblock.fn->params != NULL && e->val.fnblock.fn->params->val.fnCallBlock != NULL)
				{
					codegenFuncCall(e->val.fnblock.fn->params->val.fnCallBlock, parent, table);
				}
			}
			else{
				printf("%s(", parent->bindingName);
				if(e->val.fnblock.fn->params != NULL && e->val.fnblock.fn->params->val.fnCallBlock != NULL)
				{
					codegenFuncCall(e->val.fnblock.fn->params->val.fnCallBlock, parent->val.func.funcParams, table);
				}
			}
			printf(")");
			break;
		case appendExp:
			printf("%s", e->symTypeRef->bindingName);
			break;
		case lenExp:
			codegenExpression(e->val.binary.rhs, table);
			printf(".size");
			break;
		case capExp:
			codegenExpression(e->val.binary.rhs, table);
			printf(".cap");
			break;
		case uxorExp:
			//As far as I can tell, 1111111... in C
			printf("2147483647^(");
			codegenExpression(e->val.binary.rhs, table);
			printf(")");
			break;
		case funcExp:
		default:
			printf("ERROR: Unknown expression type\n");
			exit(1);
	}
}
/*Behaves like codegenExpression, but returns a string instead*/
char *getFullStr(EXP *e, symTable *table)
{
	SYMBOL *ref;
	EXP *tmpe;
	char *tMods, *tName;
	char *tmp1, *tmp2;
	char *returnVal = malloc(32);
	switch(e->kind){
		case emptyExp:
			return "";
        case intExp:
			sprintf(returnVal, "%d", e->val.intLiteral);
			return returnVal;
        case floatExp:
			sprintf(returnVal, "%f", e->val.floatLiteral);
			return returnVal;
        case strExp:
			sprintf(returnVal, "%s", e->val.strLiteral);
			return returnVal;
        case rawstrExp:
			sprintf(returnVal, "%s", e->val.strLiteral);
			return returnVal;
        case boolExp:
			sprintf(returnVal, "%d", e->val.intLiteral);
			return returnVal;
        case runeExp:
			sprintf(returnVal, "%c", e->val.runeLiteral);
			return returnVal;
        case plusExp:
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "string") == 0)
			{
				laggingCounter++;
				sprintf(returnVal, "strcat(strcat(CODEGEN_BINDING_%d, %s), %s)", laggingCounter-1, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			}
			else{
				sprintf(returnVal, "%s+%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			}
			return returnVal;
		case minusExp:
			sprintf(returnVal, "%s-%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case timesExp:
			sprintf(returnVal, "%s*%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case divExp:
			sprintf(returnVal, "%s/%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case modExp:
			sprintf(returnVal, "%s%%%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case notExp:
			sprintf(returnVal, "!%s",getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case posExp:
			/*We should handle non-int types in here IIRC
			I just can't think of a way to do it neatly at the moment*/
			if(strcmp(tName, "int") == 0 || strcmp(tName, "rune") == 0)
			{
				sprintf(returnVal, "abs(%s)", getFullStr(e->val.binary.rhs, table));
			}
			else if(strcmp(tName, "float64") == 0)
			{
				sprintf(returnVal, "fabs(%s)", getFullStr(e->val.binary.rhs, table));
			}
			return returnVal;
		case negExp:
			sprintf(returnVal, "-%s", getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case parExp:
			sprintf(returnVal, "(%s)", getFullStr(e->val.binary.rhs, table));
			return returnVal;
		//The following CAN occur in the case of fn parameters
		case eqExp:
			tMods = getTypeModifiers(e->val.binary.lhs->symTypeRef);
			tName = getTypeName(e->val.binary.lhs->symTypeRef);
			ref = getSymbol(table, tName, typeSym);
			if(strlen(tMods) != 0)
			{
				returnVal = eqExpArraysStr(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, tMods, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
			}
			else if(e->val.binary.lhs->symTypeRef->kind == varstructSym || e->val.binary.lhs->symTypeRef->kind == structSym || ref != NULL && ref->kind == structSym)
			{
				returnVal = eqExpStructsStr(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
			}
			else if(strcmp(tName, "string") == 0)
			{
				sprintf(returnVal, "strcmp(");
				sprintf(returnVal, "%s %s, ", returnVal, getFullStr(e->val.binary.lhs, table));
				sprintf(returnVal, "%s %s ) == 0", returnVal, getFullStr(e->val.binary.rhs, table));
			}
			else{
				sprintf(returnVal, "%s == %s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			}
			return returnVal;
		case neqExp:
			tMods = getTypeModifiers(e->val.binary.lhs->symTypeRef);
			tName = getTypeName(e->val.binary.lhs->symTypeRef);
			ref = getSymbol(table, tName, typeSym);
			if(strlen(tMods) != 0)
			{
				returnVal = eqExpArraysStr(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, tMods, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
			}
			else if(e->val.binary.lhs->symTypeRef->kind == varstructSym || e->val.binary.lhs->symTypeRef->kind == structSym || ref != NULL && ref->kind == structSym)
			{
				returnVal = eqExpStructsStr(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
			}
			else if(strcmp(tName, "string") == 0)
			{
				sprintf(returnVal, "strcmp(");
				sprintf(returnVal, "%s %s, ", returnVal, getFullStr(e->val.binary.lhs, table));
				sprintf(returnVal, "%s %s ) == 0", returnVal, getFullStr(e->val.binary.rhs, table));
			}
			else{
				sprintf(returnVal, "%s == %s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			}
			sprintf(returnVal, "!(%s)", returnVal);
			return returnVal;
		case geqExp:
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "string") == 0)
			{
				sprintf(returnVal, "strcmp(");
				sprintf(returnVal, "%s %s,", returnVal, getFullStr(e->val.binary.lhs, table));
				sprintf(returnVal, "%s %s) >= 0", returnVal, getFullStr(e->val.binary.rhs, table));
			}
			else
			{
				sprintf(returnVal, "%s >= %s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			}
			return returnVal;
		case leqExp:
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "string") == 0)
			{
				sprintf(returnVal, "strcmp(");
				sprintf(returnVal, "%s %s,", returnVal, getFullStr(e->val.binary.lhs, table));
				sprintf(returnVal, "%s %s) <= 0", returnVal, getFullStr(e->val.binary.rhs, table));
			}
			else
			{
				sprintf(returnVal, "%s <= %s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			}
			return returnVal;
		case gtExp:
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "string") == 0)
			{
				sprintf(returnVal, "strcmp(");
				sprintf(returnVal, "%s %s,", returnVal, getFullStr(e->val.binary.lhs, table));
				sprintf(returnVal, "%s %s) > 0", returnVal, getFullStr(e->val.binary.rhs, table));
			}
			else
			{
				sprintf(returnVal, "%s > %s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			}
			return returnVal;
		case ltExp:
			tName = getTypeName(e->symTypeRef);
			if(strcmp(tName, "string") == 0)
			{
				sprintf(returnVal, "strcmp(");
				sprintf(returnVal, "%s %s,", returnVal, getFullStr(e->val.binary.lhs, table));
				sprintf(returnVal, "%s %s) < 0", returnVal, getFullStr(e->val.binary.rhs, table));
			}
			else
			{
				sprintf(returnVal, "%s < %s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			}
			return returnVal;
		case orExp:
			sprintf(returnVal, "%s || %s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case andExp:
			sprintf(returnVal, "%s && %s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case andnotExp:
			tmp1 = getFullStr(e->val.binary.lhs, table);
			tmp2 = getFullStr(e->val.binary.rhs, table);
			sprintf(returnVal, "%s&^%s", tmp1, tmp2);
			return returnVal;
		case idExp:
			ref = getSymbol(table, e->val.identifier, varSym);
			sprintf(returnVal, "%s", ref->bindingName);
			return returnVal;
		
		//case idblockExp:
		case expblockExp:
			fprintf(stderr, "Encountered expBlock in getFullStr.\n");
			exit(1);
			
		case bitAndExp:
			sprintf(returnVal, "%s&%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case bitOrExp:
			sprintf(returnVal, "%s|%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case xorExp:
			sprintf(returnVal, "%s^%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case lshiftExp:
			sprintf(returnVal, "%s<<%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case rshiftExp:
			sprintf(returnVal, "%s>>%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case indexExp:
			return getFullStr(e->val.binary.rhs, table);
		case elementExp:
			sprintf(returnVal, "%s[%s]", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case invocExp:
			sprintf(returnVal, "%s.%s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case funcBlockExp:
			tmpe = e->val.fnblock.identifier;
			while(tmpe->kind != idExp)
			{
				tmpe = tmpe->val.binary.rhs;
			}
			ref = getSymbol(table, tmpe->val.identifier, funcSym);
			if(ref == NULL)
			{
				ref = getSymbol(table, tmpe->val.identifier, typeSym);
				tName = getTypeName(ref);
				if(strcmp(tName, "float64") == 0)
				{
					ref->bindingName = malloc(10);
					strcpy(ref->bindingName, "float");
				}
				else if(strcmp(tName, "rune") == 0)
				{
					ref->bindingName = malloc(10);
					strcpy(ref->bindingName, "char");
				}
				else if(strcmp(tName, "string") == 0)
				{
					ref->bindingName = malloc(10);
					strcpy(ref->bindingName, "char");
				}
				else if(strcmp(tName, "bool") == 0)
				{
					ref->bindingName = malloc(10);
					strcpy(ref->bindingName, "int");
				}
				else{
					ref->bindingName = tName;
				}
				sprintf(returnVal, "%s(", ref->bindingName);
				if(e->val.fnblock.fn->params != NULL && e->val.fnblock.fn->params->val.fnCallBlock != NULL)
				{
					strFuncCall(e->val.fnblock.fn->params->val.fnCallBlock, ref, table);
				}
			}
			else{
				sprintf(returnVal, "%s(", ref->bindingName);
				if(e->val.fnblock.fn->params != NULL && e->val.fnblock.fn->params->val.fnCallBlock != NULL)
				{
					strFuncCall(e->val.fnblock.fn->params->val.fnCallBlock, ref->val.func.funcParams, table);
				}
			}
			sprintf(returnVal, "%s)", returnVal);
			break;
		case lenExp:
			tmp1 = getFullStr(e->val.binary.rhs, table);
			sprintf(returnVal, "%s.size", tmp1);
			return returnVal;
		case capExp:
			tmp1 = getFullStr(e->val.binary.rhs, table);
			sprintf(returnVal, "%s.cap", tmp1);
			return returnVal;
			
		case appendExp:
			return e->symTypeRef->bindingName;
		
		
		
		case uxorExp:
			//As far as I can tell, 1111111... in C
			sprintf(returnVal, "2147483647^(%s)", getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case funcExp:
		default:
			fprintf(stderr, "ERROR: Unknown expression type\n");
			exit(1);
	}
}
//Generates code for comparing structs.
//nameSoFar represents the path to nested structs since we don't have pointers to parents;
//e.g. struct.innerStruct.innerInnerStruct and the like
void eqExpStructs(SYMBOL *s1, SYMBOL *s2, char *nameSoFar1, char *nameSoFar2, symTable *table){
	printf("(");
	SYMBOL *field1 = s1->val.structFields;
	SYMBOL *field2 = s2->val.structFields;
	while(field1 != NULL){
		char *tMods = getTypeModifiers(field1);
		char *tName = getTypeName(field1);
		SYMBOL *parent = getSymbol(table, tName, typeSym);
		if(*(tMods) != 0)
		{
			char *tmp1 = malloc(128);
			char *tmp2 = malloc(128);
			strcpy(tmp1, nameSoFar1);
			strcpy(tmp2, nameSoFar2);
			eqExpArrays(field1, field2, tMods, strcat(strcat(tmp1, "."),  field1->name), strcat(strcat(tmp2, "."), field2->name), table);
			
		}
		else if(field1->kind != structSym && field1->kind != varstructSym && parent->kind != structSym){
			
			
				if(strcmp(tName, "string") == 0)
				{
					printf("strcmp(%s.%s, %s.%s) == 0", nameSoFar1, field1->name, nameSoFar2, field2->name);
				}
				else{
					printf("%s.%s", nameSoFar1, field1->name);
					printf("==");
					printf("%s.%s", nameSoFar2, field2->name);
				}
			
			
			
		}
		else if(parent->kind == structSym)
		{
			char *tmp1 = malloc(128);
			char *tmp2 = malloc(128);
			strcpy(tmp1, nameSoFar1);
			strcpy(tmp2, nameSoFar2);
			eqExpStructs(parent, parent, strcat(strcat(tmp1, "."), field1->name), strcat(strcat(tmp2, "."), field2->name), table);
		}
		else{
			char *tmp1 = malloc(128);
			char *tmp2 = malloc(128);
			strcpy(tmp1, nameSoFar1);
			strcpy(tmp2, nameSoFar2);
			eqExpStructs(field1, field2, strcat(strcat(tmp1, "."), field1->name), strcat(strcat(tmp2, "."), field2->name), table);
		}
		if(field1->next != NULL){
				printf("&&");
		}
		field1 = field1->next;
		field2 = field2->next;
	}
	printf(")");
}

void eqExpArrays(SYMBOL *s1, SYMBOL *s2, char *tMods, char *nameSoFar1, char *nameSoFar2, symTable *table)
{
	if(*tMods == '['){
		int size = atoi((tMods+1));
		int offset = 2;
		while(*(tMods+offset) != ']')
		{
			offset++;
		}
		offset++;
		printf("( ");
		for(int i = 0; i < size; i++)
		{
			char *tmp = malloc(10);
			sprintf(tmp, "[%d]", size);
			char *tmp1 = malloc(128), *tmp2 = malloc(128);
			strcpy(tmp1, nameSoFar1);
			strcpy(tmp2, nameSoFar2);
			strcat(strcat(tmp1,".val"), tmp);
			strcat(strcat(tmp2,".val"), tmp);
			eqExpArrays(s1, s2, tMods+offset, tmp1, tmp2, table);
			if(i != size - 1)
			{
				printf(" && ");
			}
		}
		printf(")");

	}
	else{
		char *tName = getTypeName(s1);
		SYMBOL *parent = getSymbol(table, tName, typeSym);
		if(s1->kind == varstructSym || s1->kind == structSym || parent->kind == structSym)
		{
			eqExpStructs(parent, parent, nameSoFar1, nameSoFar2, table);
		}
		else
		{
			if(strcmp(tName, "string") == 0)
			{
				printf("strcmp(%s, %s) == 0", nameSoFar1, nameSoFar2);
			}
			else
			{
				printf(" %s == %s ", nameSoFar1, nameSoFar2);
			}
			
		}
		
	}
}

//Generates code for comparing structs.
//nameSoFar represents the path to nested structs since we don't have pointers to parents;
//e.g. struct.innerStruct.innerInnerStruct and the like
char *eqExpStructsStr(SYMBOL *s1, SYMBOL *s2, char *nameSoFar1, char *nameSoFar2, symTable *table){
	char *returnVal = malloc(128);
	sprintf(returnVal, "(");
	SYMBOL *field1 = s1->val.structFields;
	SYMBOL *field2 = s2->val.structFields;
	while(field1 != NULL){
		char *tMods = getTypeModifiers(field1);
		char *tName = getTypeName(field1);
		SYMBOL *parent = getSymbol(table, tName, typeSym);
		if(strlen(tMods) == 0)
		{
			char *tmp1 = malloc(128);
			char *tmp2 = malloc(128);
			strcpy(tmp1, nameSoFar1);
			strcpy(tmp2, nameSoFar2);
			sprintf(returnVal, "%s %s", returnVal, eqExpArraysStr(field1, field2, tMods, strcat(strcat(tmp1, "."), field1->name), strcat(strcat(tmp2, "."), field2->name), table));
		}
		else if(field1->kind != structSym && field1->kind != varstructSym && parent->kind != structSym){
			
			if(strcmp(tName, "string") == 0)
			{
				sprintf(returnVal, "%s strcmp(%s.%s, %s.%s) == 0",returnVal, nameSoFar1, field1->name, nameSoFar2, field2->name);
			}
			else{
				sprintf(returnVal, "%s %s.%s",returnVal, nameSoFar1, field1->name);
				sprintf(returnVal, "%s==", returnVal);
				sprintf(returnVal, "%s%s.%s",returnVal, nameSoFar2, field2->name);
			}
			
		}
		else if(parent->kind == structSym)
		{
			char *tmp1 = malloc(128);
			char *tmp2 = malloc(128);
			strcpy(tmp1, nameSoFar1);
			strcpy(tmp2, nameSoFar2);
			sprintf(returnVal, "%s %s", returnVal, eqExpStructsStr(parent, parent, strcat(strcat(tmp1, "."), field1->name), strcat(strcat(tmp2, "."), field2->name), table));
		}
		else{
			char *tmp1 = malloc(128);
			char *tmp2 = malloc(128);
			strcpy(tmp1, nameSoFar1);
			strcpy(tmp2, nameSoFar2);
			sprintf(returnVal, "%s %s", returnVal, eqExpStructsStr(field1, field2, strcat(strcat(tmp1, "."), field1->name), strcat(strcat(tmp2, "."), field2->name), table));
		}
		if(field1->next != NULL){
				sprintf(returnVal, "%s&&", returnVal);
		}
		field1 = field1->next;
		field2 = field2->next;
	}
	sprintf(returnVal, "%s)", returnVal);
	return returnVal;
}

char *eqExpArraysStr(SYMBOL *s1, SYMBOL *s2, char *tMods, char *nameSoFar1, char *nameSoFar2, symTable *table)
{
	char *returnVal = malloc(128);
	if(*tMods == '['){
		int size = atoi((tMods+1));
		int offset = 2;
		while(*(tMods+offset) != ']')
		{
			offset++;
		}
		offset++;
		sprintf(returnVal, "( ");
		for(int i = 0; i < size; i++)
		{
			char *tmp = malloc(10);
			sprintf(tmp, "[%d]", size);
			char *tmp1 = malloc(128);
			char *tmp2 = malloc(128);
			strcpy(tmp1, nameSoFar1);
			strcpy(tmp2, nameSoFar2);
			strcat(strcat(tmp1,".val"), tmp);
			strcat(strcat(tmp2,".val"), tmp);
			sprintf(returnVal, "%s%s", returnVal, eqExpArraysStr(s1, s2, tMods+offset, tmp1, tmp2, table));
			if(i != size - 1)
			{
				sprintf(returnVal, "%s && ", returnVal);
			}
		}
		sprintf(returnVal, "%s)", returnVal);

	}
	else{
		char *tName = getTypeName(s1);
		SYMBOL *parent = getSymbol(table, tName, typeSym);
		if(s1->kind == varstructSym || s1->kind == structSym || parent->kind == structSym)
		{
			sprintf(returnVal, "%s", eqExpStructsStr(parent, parent, nameSoFar1, nameSoFar2, table));
		}
		else
		{
			if(strcmp(tName, "string") == 0)
			{
				sprintf(returnVal, "strcmp(%s, %s) == 0", nameSoFar1, nameSoFar2);
			}
			else
			{
				sprintf(returnVal, " %s == %s ", nameSoFar1, nameSoFar2);
			}
			
		}
		
	}
	return returnVal;
}
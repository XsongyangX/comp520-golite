#include "codegen.h"
int bindingCounter = 0;
int laggingCounter = 0;
int forwardCounter = 0;
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
				prettyTabs(tabs);
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
			appendCheck(e->val.binary.rhs, tabs, depth);
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
		SYMBOL *copy = makeSymbolCopy(sym);		
		SYMBOL *parent = getSymbol(table, tName, typeSym);
		if (*tMods != 0)
		{
			char *name1;
			prettyTabs(depth);
			if(isTypeDef) printf("struct %s {\n", typeName);
			else{
				name1 = findExistingBinding(copy, table);
				if(strlen(name1) == 0)
				{
					name1 = getNewBinding();
					copy->next = table->bindingsList;
					table->bindingList = copy;
					copy->bindingName = name1;
				}
				printf("struct %s {\n", name1);
			}
			prettyTabs(depth+1);
				//slice case
			if(*(tMods+1) == ']')
			{
				while(copy->t->gType != sliceType)
				{
					copy = makeSymbolCopy(copy->val.parentSym);
				}
				copy->t = copy->t->val.arg;
				printf("int size = 0;\n");
				prettyTabs(depth+1);
				printf("int cap = 0;\n");
				prettyTabs(depth+1);
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
							table->bindingList = copy;
							copy->bindingName =  name2;
						}
						printf("struct %s {\n", name2);
						codegenStructHelper(sym->val.structFields, depth+2, table);
						prettyTabs(depth+1);
						printf("};\n");	
						prettyTabs(depth+1);
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
					prettyTabs(depth);
					printf("};\n");	
					prettyTabs(depth);
					printf("struct %s *val;\n", name1)
				}
			}
			else
			{
				while(copy->t->gType != arrayType)
				{
					copy = makeSymbolCopy(copy->val.parentSym);
				}
				copy->t = copy->t->val.arg;
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
							table->bindingList = copy;
							copy->bindingName = name2;
						}
						printf("struct %s {\n", name2);
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
					genStructureHelper(tName, tMods, sym, depth+1, "", false, table);
					prettyTabs(depth);
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
	prettyTabs(depth);
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
	SYMBOl *parent = getSymbol(table, tName, typeSym);
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
				table->bindingList = copy;
				copy->bindingName = name;
			}
			
			printf("struct %s {\n", name);
			codegenStructHelper(field->val.structFields, curDepth+1, table);
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
		genStructureHelper(tName, tMods, field, depth, "", false, table);
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
	return "";
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
		return findExistingBinding(sym, table->next);
	}
	return NULL;
}
void codegenVarDecl(DECLARATION *decl, symTable *table, int depth)
{
	SYMBOL *tmp = lookupVar(table, decl->identifier, decl->lineno, false);
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
			tmp = makeSymbolCopy(decl->val.right->symTypeRef);
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
	SYMBOL *parent = findExistingBinding(tmp, table);
	if(parent != NULL)
	{
		prettyTabs(depth);
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
				arrayAssignHelper(tmp, decl->val.right->symTypeRef, tMods, depth, tmp->bindingName, getFullStr(decl->val.right, table));
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
				arrayAssignHelper(tmp, NULL, tMods, depth, tmp->bindingName, NULL);
			}
		}
		
	}
	else if(tmp->kind == varstructSym)//"inline" struct definition, can be grouped in an array structure or the like
	{
		/*make a custom type binding for every struct we define*/
		SYMBOL *copy = makeSymbolCopy(tmp);
		copy->bindingName = getNewBinding();
		copy->next = table->bindingsList;
		table->bindingsList = copy;
		table->bindingsSize++;
		if(*tMods == 0)
		{
			prettyTabs(depth);
			printf("struct %s {\n", copy->bindingName);
			codegenStructHelper(tmp->val.structFields, depth+1, table);
			prettyTabs(depth);
			printf("}%s;\n", tmp->bindingName);
			structAssignHelper(copy, NULL, copy->bindingName, NULL, depth, table);
		}
		else{
			prettyTabs(depth);
			genStructureHelper("struct", tMods, tmp, depth, copy->bindingName, true, table);
			prettyTabs(depth);
			printf("}%s;\n", tmp->bindingName);
			if(*(tMods+1) == ']')
			{
				sliceAssignHelper(copy->bindingName, NULL, depth);
			}
			else{
				arrayAssignHelper(copy, NULL, tMods, depth, copy->bindingName, NULL);
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
				printf("char *%s = malloc(128);\n", tName, tmp->bindingName);
				prettyTabs(depth);
				
				if(decl->val.right != NULL)
				{
					printf("strcpy(%s, ", tmp->bindingName);
					codegenExpression(decl->val.right, table);
					printf(");\n")
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
			SYMBOL *copy = makeSymbolCopy(tmp);
			copy->bindingName = getNewBinding();
			copy->next = table->bindingsList;
			table->bindingsList = copy;
			table->bindingsSize++;
			genStructureHelper(tName, tMods, tmp, depth, copy->bindingName, true, table);
			prettyTabs(depth);
			printf("}%s;\n", tmp->bindingName);
			if(decl->val.rght == NULL){
				if(*(tMods+1) == ']')
				{
					sliceAssignHelper(copy->bindingName, NULL, depth);
				}
				else{
					arrayAssignHelper(copy, NULL, tMods, depth, copy->bindingName, NULL);
				}
			}
			else{
				if(*(tMods+1) == ']')
				{
					sliceAssignHelper(copy->bindingName, getFullStr(decl->val.right->symTypeRef,table), depth);
				}
				else{
					arrayAssignHelper(copy, decl->val.right->symTypeRef, tMods, depth, copy->bindingName, getFullStr(decl->val.right->symTypeRef,table));
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
			codegenStructHelper(tmp->val.structFields, depth+1, table);
			prettyTabs(depth);
			printf("};\n");
		}
		else{
			prettyTabs(depth);
			genStructureHelper("struct", tMods, tmp, depth, tmp->bindingName, true, table);
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
	char *tMods = getTypeModifiers(args);)
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
		genStructureHelper(tName, tMods, tmp->val.func.returnSymRef, 0, copy->bindingName, true, table);
		printf("};\n");
		printf("%s %s(", tName, tmp->bindingName);
	}
	codegenFuncDeclArgs(tmp->val.func.funcParams, table);
	printf("{\n");
	codegenStatement(decl->val.f->body, table, depth+1);
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
					prettyTabs(depth);
					printf("%s.%s = malloc(128); ", nameSoFar1, fields->bindingName);
				}
				else if(strcmp(tName, "int") == 0 || strcmp(tName, "rune") == 0 || strcmp(tName, "bool") == 0)
				{
					prettyTabs(depth);
					printf("%s.%s = 0; ", nameSoFar1, fields->bindingName);
				}
				else if(strcmp(tName, "float64") == 0)
				{
					prettyTabs(depth);
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
					sprintf(tmp1, "%s.%s", nameSoFar1, fields1->bindingName);
					arrayAssignHelper(fields, NULL, tMods, depth, tmp1, NULL);
				}
				else{
					char *tmp1, *tmp2;
					tmp1 = malloc(128);
					sprintf(tmp1, "%s.%s", nameSoFar1, fields1->bindingName);
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
					prettyTabs(depth);
					printf("strcpy(%s.%s, %s.%s); \n", nameSoFar1, fields1->bindingName, nameSoFar2, fields2->bindingName);
				}
				else if(strcmp(tName, "struct") == 0 || parent != NULL && parent->kind = structSym)
				{
					char *tmp1, *tmp2;
					tmp1 = malloc(128);
					tmp2 = malloc(128);
					sprintf(tmp1, "%s.%s", nameSoFar1, fields1->bindingName);
					sprintf(tmp2, "%s.%s", nameSoFar2, fields2->bindingName);
					structAssignHelper(fields1, fields2, tmp1, tmp2, depth, table);
				}
				else{
					prettyTabs(depth);
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
					arrayAssignHelper(fields1, fields2, tMods, depth, tmp1, tmp2);
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
		prettyTabs(depth);
		printf("%s.val = NULL; \n", nameSoFar1);
	}
	else {
		prettyTabs(depth);
		printf("%s.val = %s.val; \n", nameSoFar1, nameSoFar2);
		prettyTabs(depth);
		printf("%s.size = %s.size; \n", nameSoFar1, nameSoFar2);
		prettyTabs(depth);
		printf("%s.cap = %s.cap; \n", nameSoFar1, nameSoFar2);
	}
}
/*Helps with assignment to arrays.
sym1 is the symbol of the first array
sym2 is the symbol of the second array or NULL (for implicit initialization)
tMods is obtained through a call to getTypeModifiers
nameSoFar should contain the name of the array variable*/
void arrayAssignHelper(SYMBOl *sym1, SYMBOL *sym2, char *tMods, int depth, char *nameSoFar1, char *nameSoFar2)
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
				int size = atoi(*(tMods+1));
				int offset = 2;
				while(*(tMods+offset) != ']')
				{
					offset++;
				}
					offset++;
				for(int i = 0; i < size; i++)
				{
					sprintf(tmp1, "%s.val[%d]", nameSoFar1, i);
					arrayAssignHelper(sym1, sym2, tMods+offset, depth, tmp1, nameSoFar2);
				}

			}
		}
		else{
			char *tName = getTypeName(sym1);
			SYMBOL *parent = getSymbol(table, tName, typeSym);
			if(strcmp(tName, "string") == 0)
			{
				prettyTabs(depth);
				printf("%s = malloc(128);\n")
			}
			else if(strcmp(tName, "struct") == 0 || parent != NULL && parent->kind == structSym) 
			{
				structAssignHelper(sym1, sym2, nameSoFar1, nameSoFar2, depth, table);
			}
			else if(strcmp(tName, "float64"))
			{
				prettyTabs(depth);
				printf("%s = 0.0;\n", nameSoFar1);
			}
			else{
				prettyTabs(depth);
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
				int size = atoi(*(tMods+1));
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
					arrayAssignHelper(sym1, sym2, tMods+offset, depth, tmp1, tmp2);
				}

			}
		}
		else{
			char *tName = getTypeName(sym1);
			SYMBOL *parent = getSymbol(table, tName, typeSym);
			if(strcmp(tName, "string") == 0)
			{
				prettyTabs(depth);
				printf("strcpy(%s, %s);\n", nameSoFar1, nameSoFar2);
			}
			else if(strcmp(tName, "struct") == 0 || parent != NULL && parent->kind == structSym) 
			{
				structAssignHelper(sym1, sym2, nameSoFar1, nameSoFar2, depth, table);
			}
			else{
				prettyTabs(depth);
				printf("%s = %s;\n", nameSoFar1, nameSoFar2);
			}
		}
	}
}
void sliceAppendHelper(EXP *target, EXP *value, symTable *table, int depth)
{
	char *name = getFullStr(target->symTypeRef, table);
	char *newName = getNewBinding();
	target->symTypeRef->bindingName = newName;
	char *tName = findExistingBinding(target->symTypeRef, table);
	if(strlen(tName) == 0)
	{
		genStructureHelper(getTypeName(target->symTypeRef), getTypeModifiers(target->symTypeRef), target->symTypeRef, depth, "", false, table);
		prettyTabs(depth);
		printf("};\n");
		tName = findExistingBinding(target->symTypeRef, table);
	}
	SYMBOL *tmp = makeSymbolCopy(target->symTypeRef);
	tmp->t = tmp->t->val.arg;
	
	prettyTabs(depth);
	printf("struct %s %s;\n", tName, newName);
	prettyTabs(depth);
	printf("%s.size = %s.size + 1;\n", newName, name);
	prettyTabs(depth);
	printf("if (%s.size+1 == %s.cap){\n", name, name);
	prettyTabs(depth+1);
	printf("%s.cap = %s.cap*2;\n", newName, name);
	
	prettyTabs(depth);
	printf("}\n");
	prettyTabs(depth);
	printf("else{ %s.cap = %s.cap; }\n", newName, name);
	prettyTabs(depth);
	printf("%s.val = malloc(sizeof(struct %s)*%s.cap);\n", newName, findExistingBinding(tmp, table), newName);
	prettyTabs(depth);
	printf("for( int codegen_i = 0; codegen_i < %s.cap; codegen_i++){\n", name);
	prettyTabs(depth+1);
	printf("%s.val[codegen_i] = %s.val[codegen_i];\n", newName, name);
	prettyTabs(depth);
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
		if(strcmp(sym->name, "_") == 0)
		{
			return;
		}
		else{
			codegenExpression(e->val.expblock.value, table);
		}
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
		return;
	}
	char *returnVal = malloc(128);
	char *tmp;
	if(e->val.expblock.next != NULL)
	{
		tmp = strFuncCall(e->val.expblock.next, sym->next, table);
	}
	if(e->val.expblock.value != NULL)
	{
		if(strcmp(sym->name, "_") == 0)
		{
			return;
		}
		else{
			returnVal = getFullStr(e->val.expblock.value, table);
			if(tmp != NULL)
			{
				return strcat(strcat(tmp, ", "), returnVal);
			}
			return returnVal;
		}
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

/*Only handles simple assignments to singleton vars for the moment,
i.e. things like a = 5 and not a.b = 5 or a[7] = 5 */
void codegenAssign(STATEMENT *stmt, symTable *table, int depth){
	EXP *tmpExp= stmt->val.assignment.identifier;
	if (tmpExp->kind == idExp){
		SYMBOL *tmp = lookupVar(table, tmpExp->val.identifier, stmt->lineno, false);
		//We need to handle strings in a special way since Go changes values not pointers
		if (strcmp(getTypeName(tmp), "str")==0){
			prettyTabs(depth);
			printf("strcpy(%s,", tmp->bindingName);
			codegenExpression(stmt->val.assignment.value, stmt->localScope);
			printf(");\n");
		}
		else{
			prettyTabs(depth);
			printf("%s", tmp->bindingName);
			printf(" = ");
			codegenExpression(stmt->val.assignment.value, stmt->localScope);
			printf(";\n");
		}
	} 
}

/*Handles if statements*/
void codegenIf(STATEMENT *stmt, symTable *table, int depth){
	if(stmt->val.conditional.optDecl != NULL)
		codegenStatement(stmt->val.conditional.optDecl, stmt->localScope, depth);
	prettyTabs(depth);
	printf("if (");
	codegenExpression(stmt->val.conditional.condition, stmt->localScope);
	printf("){\n");
	STATEMENT *tmpStmt = stmt->val.conditional.body;
	if(tmpStmt != NULL){
		codegenStatement(tmpStmt, stmt->localScope, depth+1);
		tmpStmt = tmpStmt->next;
	}
	printf("}\n");
	if(stmt->val.conditional.elif != NULL){
		codegenStatement(stmt->val.conditional.elif, stmt->localScope, depth);
	}
}

/*Handles else statements*/ 
void codegenElse(STATEMENT *stmt, symTable *table, int depth){
	if(stmt->val.conditional.optDecl != NULL)
		codegenStatement(stmt->val.conditional.optDecl, stmt->localScope, depth);
	printf("else{\n");
	STATEMENT *tmpStmt = stmt->val.conditional.body;
	if(tmpStmt != NULL){
		codegenStatement(tmpStmt, stmt->localScope, depth+1);
		tmpStmt = tmpStmt->next;
	}
	prettyTabs(depth);
	printf("}\n");
}

/*Handles elif statements.
For scoping reasons, the else and if are separated onto their own lines
And quick declarations come inside the else but not the subsequent if.*/
void codegenElif(STATEMENT *stmt, symTable *table, int depth){
	prettyTabs(depth);
	printf("else{\n");
	if(stmt->val.conditional.optDecl != NULL)
		codegenStatement(stmt->val.conditional.optDecl, stmt->localScope, depth+1);
	prettyTabs(depth+1);
	printf("if(");
	codegenExpression(stmt->val.conditional.condition);
	printf("){\n")
	STATEMENT *tmpStmt = stmt->val.conditional.body;
	if(tmpStmt != NULL){
		codegenStatement(tmpStmt, stmt->localScope, depth+2);
		tmpStmt = tmpStmt->next;
	}
	prettyTabs(depth+1);
	printf("}\n");
	prettyTabs(depth);
	printf("}\n");
}

/*Handles quick declarations */
void codegenQuickDecl(STATEMENT *stmt, symTable *table, int depth){
	//shouldn't need to do any symbol table manipulation in here
	//So this is delegated to other methods
	DECLARATION *dummyDecl = makeDECL_norhs(1, stmt->val.assignment.identifier, typecheckExp(stmt->val.assignment.value), 0);
	codegenDeclaration(dummyDecl, stmt->localScope, depth);
	STATEMENT *dummyStmt = makeSTMT_assmt(0, stmt->val.assignment.identifier, stmt->val.assignment.value);
	codegenStatement(dummyStmt, stmt->localScope, depth);
}


void codegenProgram(PROGRAM *p, symTable *table){
	DECL *tmpDecl = p->decList;
	while(tmpDecl != NULL){
		codegenDeclaration(tmpDecl);
		tmpDecl = tmpDecl->next;
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
			// codegenFuncCall(decl, table);
			// break;
			fprintf(stderr, "FuncCall encountered.\n");
			exit(1);
	}
}


void codegenStatement(STATEMENT *stmt, symTable *table, int depth){
	switch(stmt->kind){
		case emptyS:
			break; 
		case assignS:
			codegenAssign(stmt, table, depth);
			break;
		case quickDeclS:
			codegenQuickDecl(stmt, table, depth);
		case blockS:
		case ifS:
			codegenIf(stmt, table, depth);
			break;
		case elifS:
			codegenElif(stmt, table, depth);
			break;
		case elseS:
			codegenElse(stmt, table, depth);
			break;
		case forS:
		case whileS:
		case printS:
		case exprS:
		case returnS:
		case switchS:
		case caseS:
		case breakS:
		case continueS:
		case declS:
		case incrementS:
		case decrementS:
	}
}

void codegenExpression(EXP *e, symTable *table){
	SYMBOL *parent;
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
			if(strcmp(tName, "int") == 0 || strcmp(tname, "rune") == 0)
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
	SYMBOl *ref;
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
			sprintf(returnVal, "-%s"getFullStr(e->val.binary.rhs, table));
			return returnVal;
		case parExp:
			sprintf(returnVal, "(%s)"getFullStr(e->val.binary.rhs, table));
			return returnVal;
		//The following CAN occur in the case of fn parameters
		case eqExp:
			tMods = getTypeModifiers(e->val.binary.lhs->symTypeRef);
			tName = getTypeName(e->val.binary.lhs->symTypeRef);
			parent = getSymbol(table, tName, typeSym);
			if(strlen(tMods) != 0)
			{
				returnVal = eqExpArraysStr(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, tMods, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
			}
			else if(e->val.binary.lhs->symTypeRef->kind == varstructSym || e->val.binary.lhs->symTypeRef->kind == structSym || parent != NULL && parent->kind == structSym)
			{
				returnVal = eqExpStructsStr(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
			}
			else if(strcmp(tName, "string") == 0)
			{
				sprintf(returnVal, "strcmp(");
				sprintf(returnVal, "%s %s, ", returnVal, getFullStr(e->val.binary.lhs, table));
				sprintf(returnVal, "%s %s ) == 0", returnVal, codegenExpression(e->val.binary.rhs, table));
			}
			else{
				sprintf(returnVal, "%s == %s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			}
			return returnVal;
		case neqExp:
			tMods = getTypeModifiers(e->val.binary.lhs->symTypeRef);
			tName = getTypeName(e->val.binary.lhs->symTypeRef);
			parent = getSymbol(table, tName, typeSym);
			if(strlen(tMods) != 0)
			{
				returnVal = eqExpArraysStr(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, tMods, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
			}
			else if(e->val.binary.lhs->symTypeRef->kind == varstructSym || e->val.binary.lhs->symTypeRef->kind == structSym || parent != NULL && parent->kind == structSym)
			{
				returnVal = eqExpStructsStr(e->val.binary.lhs->symTypeRef, e->val.binary.rhs->symTypeRef, getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table), table);
			}
			else if(strcmp(tName, "string") == 0)
			{
				sprintf(returnVal, "strcmp(");
				sprintf(returnVal, "%s %s, ", returnVal, getFullStr(e->val.binary.lhs, table));
				sprintf(returnVal, "%s %s ) == 0", returnVal, codegenExpression(e->val.binary.rhs, table));
			}
			else{
				sprintf(returnVal, "%s == %s", getFullStr(e->val.binary.lhs, table), getFullStr(e->val.binary.rhs, table));
			}
			sprintf(returnVal, "!(%s)", returnVal);
			return returnVal;
		case geqExp:
			tname = getTypeName(e->symTypeRef);
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
			tname = getTypeName(e->symTypeRef);
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
			tname = getTypeName(e->symTypeRef);
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
			tname = getTypeName(e->symTypeRef);
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
				sprintf(returnval, "%s(", parent->bindingName);
				if(e->val.fnblock.fn->params != NULL && e->val.fnblock.fn->params->val.fnCallBlock != NULL)
				{
					strFuncCall(e->val.fnblock.fn->params->val.fnCallBlock, parent, table);
				}
			}
			else{
				sprintf(returnVal, "%s(", parent->bindingName);
				if(e->val.fnblock.fn->params != NULL && e->val.fnblock.fn->params->val.fnCallBlock != NULL)
				{
					strFuncCall(e->val.fnblock.fn->params->val.fnCallBlock, parent->val.func.funcParams, table);
				}
			}
			sprintf(returnVal, "%s)", returnval);
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
			return e->symTypeRef->bindingname;
		
		
		
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
		if(field1->kind != structSym && field1->kind != varStructSym && parent->kind != structSym){
			if(strlen(tMods) == 0)
			{
				if(strcmp(tName, "string") == 0)
				{
					printf("strcmp(%s.%s, %s.%s) == 0", nameSoFar1, field1->name, nameSoFar2, field2->name)
				}
				else{
					printf("%s.%s", nameSoFar1, field1->name);
					printf("==");
					printf("%s.%s", nameSoFar2, field2->name);
				}
			}
			else{
				char *tmp1 = malloc(128);
				char *tmp2 = malloc(128);
				strcpy(tmp1, nameSoFar1);
				strcpy(tmp2, nameSoFar2);
				eqExpArrays(field1, field2, strcat(strcat(tmp1, "."), field1->name), strcat(strcat(tmp2, "."), field2->name), table);
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
		int size = atoi(*(tMods+1));
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
		if(field1->kind != structSym && field1->kind != varStructSym && parent->kind != structSym){
			if(strlen(tMods) == 0)
			{
				if(strcmp(tName, "string") == 0)
				{
					sprintf(returnVal, "%s strcmp(%s.%s, %s.%s) == 0",returnVal, nameSoFar1, field1->name, nameSoFar2, field2->name)
				}
				else{
					sprintf(returnVal, "%s %s.%s",returnVal, nameSoFar1, field1->name);
					sprintf(returnVal, "%s==", returnVal);
					sprintf(returnVal, "%s%s.%s",returnVal, nameSoFar2, field2->name);
				}
			}
			else{
				char *tmp1 = malloc(128);
				char *tmp2 = malloc(128);
				strcpy(tmp1, nameSoFar1);
				strcpy(tmp2, nameSoFar2);
				sprintf(returnVal, "%s %s", returnVal, eqExpArrays(field1, field2, strcat(strcat(tmp1, "."), field1->name), strcat(strcat(tmp2, "."), field2->name), table));
			}
			
		}
		else if(parent->kind == structSym)
		{
			char *tmp1 = malloc(128);
			char *tmp2 = malloc(128);
			strcpy(tmp1, nameSoFar1);
			strcpy(tmp2, nameSoFar2);
			sprintf(returnVal, "%s %s", returnVal, eqExpStructs(parent, parent, strcat(strcat(tmp1, "."), field1->name), strcat(strcat(tmp2, "."), field2->name), table));
		}
		else{
			char *tmp1 = malloc(128);
			char *tmp2 = malloc(128);
			strcpy(tmp1, nameSoFar1);
			strcpy(tmp2, nameSoFar2);
			sprintf(returnVal, "%s %s", returnVal, eqExpStructs(field1, field2, strcat(strcat(tmp1, "."), field1->name), strcat(strcat(tmp2, "."), field2->name), table));
		}
		if(field1->next != NULL){
				sprintf(returnVal, "%s&&", returnVal);
		}
		field1 = field1->next;
		field2 = field2->next;
	}
	sprintf(returnVal, "%s)", returnVal);
	return returnval;
}

char *eqExpArraysStr(SYMBOL *s1, SYMBOL *s2, char *tMods, char *nameSoFar1, char *nameSoFar2, symTable *table)
{
	char *returnVal = malloc(128);
	if(*tMods == '['){
		int size = atoi(*(tMods+1));
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
			sprintf(returnVal, "%s%s", returnVal, eqExpArrays(s1, s2, tMods+offset, tmp1, tmp2, table));
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
			sprintf(returnVal, "%s", eqExpStructs(parent, parent, nameSoFar1, nameSoFar2, table));
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

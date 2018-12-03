/* IFJ Project 2018/19
 * Implementation of Intermediate code Generator
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: generator.c
 *  FILE AUTHOR: 
 */

/***SYSTEM FILES***/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/***LOCAL FILES***/
#include "parser.h"
#include "parser-semantic.h"
#include "generator.h"
#include "scanner.h"

/* Generates code from derivation tree into intermediate code
 * @tree derivation tree
 * Returns: Intermediate code as a string
 */



int ExpTmp = 0;
int IfLabel, IfTopLabel = 0;
int WhileLabel, WhileTopLabel = 0;

FILE *outputFile;



bool GeneratorAddExpression(ExL Ex, char *name, lexems lexem){
	Expr tmp;
	tmp = malloc(sizeof(struct ExprS));// + (strlen(name) * sizeof(char) + 1));
	
	if (!tmp)
		return false;

	tmp->name = malloc(strlen(name) * sizeof(char) + 1);	
	tmp->name = name;
	tmp->lexem = lexem;

	if (lexem == LESS ||
	    lexem == MORE ||
	    lexem == EQ ||
	    lexem == LESSEQ ||
            lexem == MOREEQ)
		tmp->code = GeneratorMatcher(lexem);

        if (lexem == IDENT)
        	tmp->code = GeneratorVariable(tmp->name); 

	if (lexem == INT ||
	    lexem == STR ||
	    lexem == FLOAT )
		tmp->code = GeneratorConstantDefine(lexem, tmp->name);
	
	if (lexem == PLUS ||
	    lexem == MINUS ||
	    lexem == DIVISION ||
	    lexem == MULTIPLY)
		tmp->code = GeneratorMathOperation(lexem);
	
	if (!Ex->Last){
		Ex->Last = tmp;
		Ex->First = tmp;
		tmp->Next = NULL;
		tmp->Before = NULL;
	} else {
		Ex->Last->Next = tmp;
		tmp->Before = Ex->Last;
		Ex->Last = tmp;
		tmp->Next = NULL;
	}
	return true;
}

void GeneratorDeleteExpression(ExL ExpL, Expr Delete){
	Expr tmp = ExpL->First;

	/* Looking for Delete item in List */
	while(tmp->Next != Delete){
		tmp = tmp->Next;
		if (!tmp->Next)
			return;
	}

	if (Delete->Before && Delete->Next){
		Delete->Before->Next = Delete->Next;
		Delete->Next->Before = Delete->Before;

	} else if (Delete->Before && !Delete->Next){
		Delete->Before->Next = NULL;
		ExpL->Last = Delete->Before;

	} else if (!Delete->Before && Delete->Next){
		Delete->Next->Before = NULL;
		ExpL->First = Delete->Next;

	} else {
		ExpL->First = NULL;
		ExpL->Last = NULL;

	}
	free(Delete);
}

StackGen CreateStack(){
	StackGen StackS = malloc(sizeof(struct StackGen));
	
	if(!StackS)
		return NULL;

	StackS->top = 0;
	return StackS;
}

bool PushStack(StackGen StackS, char *code){

	int sizeString = strlen(code) + 1;
	char *addcode = malloc(sizeof(char) * sizeString);

	if(!addcode)
		return false;

	strcpy(addcode, code);

	StackS->code[StackS->top++] = addcode;

	return true;
}

bool GenEmptyStack(StackGen StackS){
	if(StackS->top)
		return false;
	else 
		return true;
}

char *PopStack(StackGen StackS){
	if(!StackS->top)
		return NULL;	

	return StackS->code[--StackS->top];
}

void GeneratorAssign(char *name, bool defined){
	if (!defined){
		printf("DEFVAR LF@$%s\n", name);			
	} 
	char *code = malloc(sizeof(char) * (strlen(name) + 1 + (strlen("MOVE "))));
	strcpy(code, "MOVE LF@$");
	strcat(code, name);

	PushStack(StackAssign, code);	
	
}

char *GeneratorMathOperation(lexems lexem){
	char *code;
	switch (lexem){
		case PLUS:
			code = GeneratorCharAppend("ADD");
			break;
		case MINUS:
			code = GeneratorCharAppend("SUB");
			break;
		case MULTIPLY:
			code = GeneratorCharAppend("MUL");
			break;
		case DIVISION:
			code = GeneratorCharAppend("DIV");
			break;
		default:
			break;
	}
	return code;
}

char *GeneratorCharAppend(char *name){
	char *code = malloc(sizeof(char) * (strlen(name) + 1 ));
	sprintf(code,"%s",name);
	
	if (!code)
		return NULL;
	else 
		return code;
}

char *GeneratorMatcher(lexems lexem){
	char *code;
	switch (lexem){
		case LESS:
			code = GeneratorCharAppend("LT LF@$cond");
			break;
		case MORE:
			code = GeneratorCharAppend("GT LF@$cond");
			break;
		case EQ:
			code = GeneratorCharAppend("EQ LF@$cond");
			break;
		case LESSEQ:
			code = GeneratorCharAppend("-- LF@$cond");
			break;
		case MOREEQ:
			code = GeneratorCharAppend("-- LF@$cond");
			break;
		default:
			break;
	}

	return code;
}

char *GeneratorVariable(char *name){
	char *code = malloc(sizeof(char) * (strlen(name) + strlen("LF@$") + 1 ));
	sprintf(code,"LF@$%s ",name);

	return code;
}

char *GeneratorConstantDefine(lexems lexem, char *name){

	switch (lexem){
		case INT:
			printf("DEFVAR LF@$const%d\n", ExpTmp);
			printf("MOVE LF@$const%d int@%s\n", ExpTmp, name);
			break;
		case STR:
			printf("DEFVAR LF@$const%d\n", ExpTmp);
			printf("MOVE LF@$const%d string@%s\n", ExpTmp, name);
			break;
		case FLOAT:
			printf("DEFVAR LF@$const%d\n", ExpTmp);
			printf("MOVE LF@$const%d float@%s\n", ExpTmp, name);
			break;
		case NIL:
			printf("DEFVAR LF@$const%d\n", ExpTmp);
			printf("MOVE LF@$const%d nil@%s\n", ExpTmp, name);
			break;
		default:
			break;
	}

	char *code = malloc(sizeof(char) * (strlen(name) + strlen("LF@const") + 1 + sizeof(int)));
	sprintf(code,"LF@const%d",ExpTmp++);
	
	return code;
}

void GeneratorFunctionCall(char *name){
	char *code = malloc(sizeof(char) * (strlen(name) + strlen("CALL $") + 1));
	sprintf(code,"CALL $%s",name);
	
	PushStack(StackG, code);
}

void GeneratorParameterOut(int order, char *name, lexems lexem){
	switch(lexem){
		case STR:
			printf("DEFVAR LF@$const%d\n", ExpTmp);
			printf("MOVE LF@$const%d string@%s\n", ExpTmp, name);
			printf("MOVE TF@%%%d LF$const%d\n", order, ExpTmp++);
			break;
		case FLOAT:
			printf("DEFVAR LF@$const%d\n", ExpTmp);
			printf("MOVE LF@$const%d float@%s\n", ExpTmp, name);
			printf("MOVE TF@%%%d LF$const%d\n", order, ExpTmp++);
			break;
		case INT:
			printf("DEFVAR LF@$const%d\n", ExpTmp);
			printf("MOVE LF@$const%d int@%s\n", ExpTmp, name);
			printf("MOVE TF@%%%d LF$const%d\n", order, ExpTmp++);
			break;
                case IDENT:
			printf("MOVE TF@%%%d LF$%s\n", order, name);
                        break;
		default:
			break;
	}
}

void GeneratorFunctionDefinition(char *name){
	printf("#Start of function called: %s.\n", name);
	printf("LABEL $%s\n", name);
	printf("PUSHFRAME\n");
	printf("DEFVAR LF@%%retval\n");
	printf("MOVE LF@%%retval nil@nil\n");
	printf("#Parameters: \n");
}

void GeneratorParameterIn(int order, char *name){
	printf("DEFVAR LF@$%s\n", name);
	printf("MOVE LF@$%s LF@%%%d\n", name, order);
}

void GeneratorFunctionEnd(){
	printf("POPFRAME\n");
	printf("RETURN\n");
}

void GeneratorWhileStartLabel(){
	printf("LABEL $whileLabel\n");
}

void GeneratorWhileCondEvaluation(){
        printf("JUMPIFEQ $whileLabelEnd LF@$cond bool@false\n");
}

void GeneratorWhileEnd(){
	printf("JUMP $whileLabel\n");
        printf("LABEL $whileLabelEnd\n");
}

void GeneratorIfStart(){
	char *code = malloc(sizeof(char) * strlen("JUMPIFEQ $ifLabelEnd LF@$cond bool@false") + 1);
	sprintf(code,"JUMPIFEQ $ifLabelEnd LF@$cond bool@false");

}

void GeneratorAfterIf(){
	printf("LABEL $ifLabelEnd\n");
}

void Generator(FILE *file){
    StackG = CreateStack();	
    StackAssign = CreateStack();
    outputFile = file;   
    printf(".IFJcode2018\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@cond\n");
}



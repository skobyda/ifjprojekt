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

#define FUNCTION_LENGTH							\
	"# Built-in function Length\n"					\
	"LABEL $length\n"						\
	"PUSHFRAME\n"							\
	"DEFVAR LF@%retval\n"						\
	"STRLEN LF@%retval LF@%0\n"					\
	"POPFRAME\n"							\
	"RETURN\n"

// SubStr(s As String, i As Integer, n As Integer) As String
#define FUNCTION_SUBSTR					\
	"# Built-in function SubStr\n"			\
	"LABEL $substr\n"				\
	"PUSHFRAME\n"					\
	"DEFVAR LF@%retval\n"				\
	"MOVE LF@%retval string@\n"			\
	"DEFVAR LF@length_str\n"			\
	"CREATEFRAME\n"				\
	"DEFVAR TF@%0\n"				\
	"MOVE TF@%0 LF@%0\n"					\
	"CALL $length\n"					\
	"MOVE LF@length_str TF@%retval\n"			\
	"DEFVAR LF@ret_cond\n"					\
	"LT LF@ret_cond LF@length_str int@0\n"			\
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"	\
	"EQ LF@ret_cond LF@length_str int@0\n"			\
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"	\
	"LT LF@ret_cond LF@%1 int@0\n"				\
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"	\
	"EQ LF@ret_cond LF@%1 int@0\n"				\
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"	\
	"GT LF@ret_cond LF@%1 LF@length_str\n"			\
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"	\
	"EQ LF@ret_cond LF@%2 int@0\n"				\
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"	\
	"DEFVAR LF@max_n\n"					\
	"MOVE LF@max_n LF@length_str\n"			\
	"SUB LF@max_n LF@max_n LF@%1\n"			\
	"ADD LF@max_n LF@max_n int@1\n"			\
	"DEFVAR LF@edit_n_cond\n"				\
	"LT LF@edit_n_cond LF@%2 int@0\n"			\
	"JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true\n"	\
	"GT LF@edit_n_cond LF@%2 LF@max_n\n"			\
	"JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true\n"	\
	"JUMP $substr$process\n"				\
	"LABEL $substr$edit_n\n"				\
	"MOVE LF@%2 LF@max_n\n"				\
	"LABEL $substr$process\n"				\
	"DEFVAR LF@index\n"					\
	"MOVE LF@index LF@%1\n"				\
	"SUB LF@index LF@index int@1\n"			\
	"DEFVAR LF@char\n"					\
	"DEFVAR LF@process_loop_cond\n"					\
	"LABEL $substr$process_loop\n"						\
	"GETCHAR LF@char LF@%0 LF@index\n"					\
	"CONCAT LF@%retval LF@%retval LF@char\n"				\
	"ADD LF@index LF@index int@1\n"					\
	"SUB LF@%2 LF@%2 int@1\n"						\
	"GT LF@process_loop_cond LF@%2 int@0\n"				\
	"JUMPIFEQ $substr$process_loop LF@process_loop_cond bool@true\n"	\
	"LABEL $substr$return\n"						\
	"POPFRAME\n"								\
	"RETURN\n"

// Asc(s As String, i As Integer) As Integer
#define FUNCTION_ASC								\
	"# Built-in function Asc\n"						\
	"LABEL $asc\n"								\
	"PUSHFRAME\n"								\
	"DEFVAR LF@%retval\n"							\
	"MOVE LF@%retval int@0\n"						\
	"DEFVAR LF@cond_length\n"						\
	"LT LF@cond_length LF@%1 int@1\n"					\
	"JUMPIFEQ $asc$return LF@cond_length bool@true\n"			\
	"DEFVAR LF@length_str\n"						\
	"CREATEFRAME\n"							\
	"DEFVAR TF@%0\n"							\
	"MOVE TF@%0 LF@%0\n"							\
	"CALL $length\n"							\
	"MOVE LF@length_str TF@%retval\n"					\
	"GT LF@cond_length LF@%1 LF@length_str\n"				\
	"JUMPIFEQ $asc$return LF@cond_length bool@true\n"			\
	"SUB LF@%1 LF@%1 int@1\n"						\
	"STRI2INT LF@%retval LF@%0 LF@%1\n"					\
	"LABEL $asc$return\n"							\
	"POPFRAME\n"								\
	"RETURN\n"

// Chr(i As Integer) As String
#define FUNCTION_CHR						\
	"# Built-in function Chr\n"				\
	"LABEL $chr\n"						\
	"PUSHFRAME\n"						\
	"DEFVAR LF@%retval\n"					\
	"MOVE LF@%retval string@\n"				\
	"DEFVAR LF@cond_range\n"				\
	"LT LF@cond_range LF@%0 int@0\n"			\
	"JUMPIFEQ $chr$return LF@cond_range bool@true\n"	\
	"GT LF@cond_range LF@%0 int@255\n"			\
	"JUMPIFEQ $chr$return LF@cond_range bool@true\n"	\
	"INT2CHAR LF@%retval LF@%0\n"				\
	"LABEL $chr$return\n"					\
	"POPFRAME\n"						\
	"RETURN\n"

bool GeneratorAddExpression(ExL Ex, char *name, lexems lexem){
	Expr tmp;
	tmp = malloc(sizeof(struct ExprS) + 1 + (ownStrLen(name) * sizeof(char)));
	
	if (!tmp)
		return false;

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
	char *code = malloc(sizeof(char) * (ownStrLen(name) + ownStrLen("MOVE LF@$")) + 1);
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
	char *code = malloc(sizeof(char) * strlen(name) + 1 );
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

int ownStrLen(char *name){
	int i;
	for(i = 0; name[i] != '\0'; i++);
	return i;
}

char *GeneratorVariable(char *name){
	char *code = malloc(sizeof(char) * (ownStrLen(name) + ownStrLen("LF@$")) + 1 );

	strcpy(code, "LF@$");
	strcat(code, name);

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



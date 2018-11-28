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

/***LOCAL FILES***/
#include "parser.h"
#include "parser-semantic.h"

/* Generates code from derivation tree into intermediate code
 * @tree derivation tree
 * Returns: Intermediate code as a string
 */

#define FUNCT_LENGHT 	\
	"#Build-in function Lenght in IFJcode2018\n"	\
	"LABEL $Lenght\n"	\
	"PUSHFRAME\n"	\
	"DEFVAR LF@%retval\n"	\
	"STRLEN LF@%retval LF@$0\n"	\
	"POPFRAME\n"	\
	"RETURN\n"	

#define FUNCT_SUBSTR	\
	"#Build-in function SubStr in IFJcode2018\n"	\
	"PUSHFRAME\n"	\
	"DEFVAR LF@%retval\n"	\
	"MOVE LF@%retval string@\n"	\
	"DEFVAR LF@strLen\n"	\
	"CREATEFRAME\n"	\
	"DEFVAR TF@$0\n"	\
	"MOVE TF@$0 LF@strLen\n"	\
	"CALL $Lenght\n"	\
	"MOVE LF@strLen LF@%retval\n"	\
	"DEFVAR LF@lenght_cond\n"	\
	"LT LF@lenght_cond LF@strLen int@0\n"	\
	"JUMPIFEQ $substr_end LF@lenght_cond bool@true\n"	\
	"EQ LF@lenght_cond LF@strLen int@0\n"	\
	"JUMPIFEQ $substr_end LF@lenght_cond bool@true\n"	\
	"LT LF@lenght_cond LF@$1 int@0\n"	\
	"JUMPIFEQ $substr_end LF@lenght_cond bool@true\n"	\
	"EQ LF@lenght_cond LF@$1 int@0\n"	\
	"JUMPIFEQ $substr_end LF@lenght_cond bool@true\n"	\
	"\n"	\
	"\n"	\
	"\n"	\
	"\n"	\
	"\n"	\
	"\n"	\
	"LABEL $substr_end\n"	\
	"POPFRAME\n"	\
	"RETURN\n"	\

/* Generate program start */
bool programGenerator(){
	printf("#Start of the program.\n");
	printf(".IFJcode2018\n");

	return true;
}

/* Define variable in IFJcode2018 */
bool varDef(char *nameVar){
	printf("DEFVAR LF@_%s\n", nameVar);

	return true;
}

/* Assign to variable */
bool varAsign(char *nameVar){
	printf("MOVE LF@_%s ", nameVar);

	return true;
}

/* Prints only variable */
bool var(char *nameVar){
	printf("%s ", nameVar);
	
	return true;
}

/* Decides about operator in IFJcode2018 */
char *operation(ATreeNodePtr Tree){
	char *type = NULL;
	switch(Tree->oType){
		case OMUL:
			asprintf(type,"MUL ");	
			break;
		case OADD:
			asprintf(type,"ADD ");	
			break;
		case OSUB:
			asprintf(type,"SUB ");	
			break;
		case ODIV:
			asprintf(type,"DIV ");	
			break;
	}
	return type;
}

/*
char *typeOfVar(ATreeNodePtr Tree){
	char *type = NULL;

	switch (Tree->val) {
		case iVal:
			asprintf(type,"int@");	
			break;
		case fVal:
			asprintf(type,"float@");	
			break;
		case sVal:
			asprintf(type,"string@");	
			break;
		case bVal:
			asprintf(type,"bool@");	
			break;
	}
	return type;
}
*/

/* Decides about type of matcher */
char *typeOfMatcher(compType cmpType){
	char *type;

	switch (cmpType) {
		case LT:
			strcpy(type,"LT ");	
			break;
		case GT:
			strcpy(type,"GT ");	
			break;
		case EQU:
			strcpy(type,"EQ ");	
			break;
		case GOE:
			//strcpy(type,"LT ");	
			break;
		case NEQE:
			//strcpy(type,"LT ");	
			break;
		case LOE:
			//strcpy(type,"LT ");	
			break;
	}

	return type;
}

/* TODO waste of space */
bool compare(ATreeNodePtr Tree){
	char *matcherType = typeOfMatcher(Tree->cmpType);
	printf("%s", matcherType);
	
	return true;
}

/* Start of if statement only condition evaluation */
bool ifStart(int ifNumb){
	printf("#If statement start\n");
	printf("JUMPIFEQ $ifLabel%d LF@cond bool@false\n", ifNumb);

	return true;
}

/* End of if with only label declaration */
bool ifEnd(int ifNumb){
	printf("LABEL $ifLabel%d\n", ifNumb);
	printf("#End of If statement\n");

	return true;
}


bool functionGenerator(){
	printf("PUSHFRAME\n");
	printf("DEFVAR LF@_retval\n");
	printf("MOVE LF@_retval nil@nil\n");

	return true;
}

bool functionParam(int paramNumber){
	printf("DEFVAR LF@param%d\n", paramNumber);
	printf("MOVE LF@param%d LF@$%d\n", paramNumber, paramNumber);
	paramNumber++;

	return true;
}

bool functionEnd(int paramNumber){
	printf("RETURN\n");
	printf("#End of this function\n");
	paramNumber = 1;

	return true;
}

void treeTraversal(ATreeNodePtr Tree){
    if(!Tree)
        return;

    ATreeNodePtr tmp = Tree;

    if(tmp->left){
        generate(tmp->left);
        tmp = tmp->left;
    }
    if(tmp->right){
        generate(tmp->right);
        tmp = tmp->right;
    }

}


void generate(ATreeNodePtr Tree){
    
    //start programu
	
        switch(Tree->nType) {
            case WHILEST:
      
            case ENDWHILEST:

            case IFST:

            case ENDIFST:

            case BLOCKIF:

            case COMPARATOR:

            case FUNCTIONDEF:

            case FUNCTIONCALL:

            case ARGUMENT:

            case VARIABLEST:

            case CONSTANT:

            case OPER:

            case STATEMENT:

            case ASSIGNMENT:
        }
}

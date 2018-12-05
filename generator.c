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
#include <stdarg.h>

/***LOCAL FILES***/
#include "parser.h"
#include "parser-semantic.h"
#include "generator.h"
#include "scanner.h"

/* Generates code from derivation tree into intermediate code
 * @tree derivation tree
 * Returns: Intermediate code as a string
 */


int WhileEnd = 0;
int ExpTmp = 0;
int IfLabel = 0;
int WhileLabel = 0;
int FunctionLabel = 0;

/* Generates Expresion and Condition in If and While Statement */
bool GeneratorAddExpression(ExL Ex, char *name, lexems lexem){
	Expr tmp = NULL;
	tmp = malloc(sizeof(struct ExprS));

	if (!tmp)
		return false;

        tmp->name =  malloc(1 + (ownStrLen(name) * sizeof(char)));
	strcpy(tmp->name, name);
	tmp->lexem = lexem;
	
	if (lexem == LESS ||
	    lexem == MORE ||
	    lexem == EQ ||
	    lexem == LESSEQ ||
            lexem == MOREEQ ||
            lexem == NOTEQ)
		tmp->code = GeneratorMatcher(lexem);

        if (lexem == IDENT){
		tmp->code = GeneratorVariable(name); 
	}

	if (lexem == INT ||
	    lexem == STR ||
	    lexem == FLOAT ){
		tmp->code = GeneratorConstantDefine(lexem, name);
	}
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

	Ex->count++;
	return true;
}

/* Deletes one of the list element */
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
	ExpL->count--;
	free(Delete->name);
	free(Delete->code);
	free(Delete);
}

/* Create Stack */
StackGen CreateStack(){
	StackGen StackS = malloc(sizeof(struct StackGen));
	
	if(!StackS)
		return NULL;

	StackS->top = 0;
	return StackS;
}

/* Push to Stack */
bool PushStack(StackGen StackS, char *code){

	int sizeString = strlen(code) + 1;
	char *addcode = malloc(sizeof(char) * sizeString);

	if(!addcode)
		return false;

	strcpy(addcode, code);

	StackS->code[StackS->top++] = addcode;

	return true;
}

/* Figuring out if is stack Empty */
bool GenEmptyStack(StackGen StackS){
	if(StackS->top)
		return false;
	else 
		return true;
}


/* Popstack returns string on top position */
char *FrontStack(StackGen StackS){
	if(!StackS->top)
		return NULL;	

	char* ret = StackS->code[0];

	for (int i = 1; i < StackS->top; i++)
		StackS->code[i-1] = StackS->code[i]; 
	StackS->top--;

	return ret;
}

/* Popstack returns string on top position */
char *PopStack(StackGen StackS){
	if(!StackS->top)
		return NULL;	

	return StackS->code[--StackS->top];
}

/* Generates assignment in IFJcode2018 and stack it on stack */
void GeneratorAssign(char *name, bool defined){
	if (!defined){
		fprintf(stdout,"DEFVAR LF@$%s\n", name);			
	} 
	char *code = malloc(sizeof(char) * (ownStrLen(name) + ownStrLen("LF@$")) + 1);
	strcpy(code, "LF@$");
	strcat(code, name);

	PushStack(StackAssign, code);	
	
}

/* Print assignment from stack */
void GeneratorAssignPrint(){
	if (!GenEmptyStack(StackAssign)){
		char *assign = PopStack(StackAssign);
		char *code = PopStack(StackG);
		if (!code || !assign){
			if (!code && !assign)
				return;
			if (!code)
				free(assign);
			if (!assign)
				free(code);
			return;
		}	

		if(!strcmp(assign, code));
		else
			GeneratorPrintWrapper(0, "MOVE %s %s\n", assign, code);

		code = PopStack(StackG);
		free(code);
		free(assign);
	}
}

/* Generates code for concatenation in IFJcode2018 */
void GeneratorConcat(Expr Ex, char *symb1, char *symb2){
	if ((Ex->Before->lexem == STR || 
            Ex->Before->lexem == IDENT) &&
            (Ex->lexem == IDENT ||
            Ex->lexem == STR)){
		fprintf(stdout,"CONCAT %s %s %s\n", symb1, symb1, symb2);
	}
}

/* Generates and return code for operation */
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

/* Malloc and returns pointer to string */
char *GeneratorCharAppend(char *name){
	char *code = malloc(sizeof(char) * strlen(name) + 1 );
	strcpy(code, name);	

	if (!code){
		return NULL;}
	else 
		return code;
}

/* Generates matcher for condition in IFJcode2018 */
char *GeneratorMatcher(lexems lexem){
	char *code;
	switch (lexem){
		case LESS:
			code = GeneratorCharAppend("LT GF@$cond");
			break;
		case MORE:
			code = GeneratorCharAppend("GT GF@$cond");
			break;
		case EQ:
			code = GeneratorCharAppend("EQ GF@$cond");
			break;
		case LESSEQ:
			code = GeneratorCharAppend("-- GF@$cond");
			break;
		case MOREEQ:
			code = GeneratorCharAppend("-- GF@$cond");
			break;
		case NOTEQ:
			code = GeneratorCharAppend("EQ GF@$cond");
			
		default:
			break;
	}

			
	return code;
}

/* Own strlen(), original strlen returns some memory leak */
int ownStrLen(char *name){
	int i;
	for(i = 0; name[i] != '\0'; i++);
	return i;
}

/* Generate code for alredy initialized variable */
char *GeneratorVariable(char *name){
	char *code = malloc(sizeof(char) * (ownStrLen(name) + ownStrLen("LF@$")) + 1 );

	strcpy(code, "LF@$");
	strcat(code, name);

	return code;
}

/* Generate and define constant return pointer to them */
char *GeneratorConstantDefine(lexems lexem, char *name){
	char *code = NULL;
	switch (lexem){
		case INT:
			//code = malloc(sizeof(char) * (strlen(name) + strlen("int@") + 1));
			//sprintf(code,"int@%s", name);
			fprintf(stdout,"DEFVAR LF@$const%d\n", ExpTmp);
			GeneratorPrintWrapper(0,"MOVE LF@$const%d int@%s\n", ExpTmp, name);
			break;
		case STR:
			//code = malloc(sizeof(char) * (strlen(name) + strlen("string@") + 1 ));
			//sprintf(code,"string@%s", name);
			fprintf(stdout,"DEFVAR LF@$const%d\n", ExpTmp);
			GeneratorPrintWrapper(0,"MOVE LF@$const%d string@%s\n", ExpTmp, name);
			break;
		case FLOAT:
			//code = malloc(sizeof(char) * (strlen(name) + strlen("float@") + 1) + sizeof(float));
			//sprintf(code,"float@%a",atof(name));
			fprintf(stdout,"DEFVAR LF@$const%d\n", ExpTmp);
			GeneratorPrintWrapper(0,"MOVE LF@$const%d float@%a\n", ExpTmp, atof(name));
			break;
		case NIL:
			//code = malloc(sizeof(char) * (strlen(name) + strlen("nil@") + 1));
			//sprintf(code,"nil@%s", name);
			fprintf(stdout,"DEFVAR LF@$const%d\n", ExpTmp);
			GeneratorPrintWrapper(0,"MOVE LF@$const%d nil@%s\n", ExpTmp, name);
			break;
		default:
			break;
	}

	code = malloc(sizeof(char) * (strlen(name) + strlen("LF@$const") + 2) + sizeof(int));
	sprintf(code,"LF@$const%d",ExpTmp++);
	
	return code;
}

/* Generate function call code  */
void GeneratorFunctionCall(char *name){

	char *code;

	if (!strcmp(name, "print")){
		code = malloc(sizeof(char) * (ownStrLen("print") + 1));
		sprintf(code,"print");
                PushStack(StackG, code);

	}else if (!strcmp(name, "inputi") || 
                  !strcmp(name, "inputf") || 
                  !strcmp(name, "inputs")){
		code = malloc(sizeof(char) * (ownStrLen("input") + 2));
			if (!strcmp(name, "inputi")){
				code = PopStack(StackAssign);
				GeneratorPrintWrapper(0,"READ %s int\n", code);
			}
			if (!strcmp(name, "inputf")){
				code = PopStack(StackAssign);
				GeneratorPrintWrapper(0,"READ %s float\n", code);
			}
			if (!strcmp(name, "inputs")){
				code = PopStack(StackAssign);
				GeneratorPrintWrapper(0,"READ %s string\n", code);
			}
	} else {
		fprintf(stdout,"CREATEFRAME\n");
		code = malloc(sizeof(char) * (strlen(name) + strlen("CALL $") + 1));
		sprintf(code,"CALL $%s",name);
		PushStack(StackG, code);
	}
}

/* Generate parameter for function */
/* If function print of input generate code for them */
void GeneratorParameterOut(int order, char *name, lexems lexem){
	switch(lexem){
		case STR:
			fprintf(stdout,"DEFVAR LF@$const%d\n", ExpTmp);
			GeneratorPrintWrapper(0,"MOVE LF@$const%d string@%s\n", ExpTmp, name);
			break;
		case FLOAT:
			fprintf(stdout,"DEFVAR LF@$const%d\n", ExpTmp);
			GeneratorPrintWrapper(0,"MOVE LF@$const%d float@%s\n", ExpTmp, name);
			break;
		case INT:
			fprintf(stdout,"DEFVAR LF@$const%d\n", ExpTmp);
			GeneratorPrintWrapper(0,"MOVE LF@$const%d int@%s\n", ExpTmp, name);
			break;
                case IDENT:
			
                        break;
		default:
			break;
	}

	if (!strcmp(StackG->code[0], "print")){
		if (lexem == IDENT)
			GeneratorPrintWrapper(0,"WRITE LF@$%s\n", name);
		else
			GeneratorPrintWrapper(0,"WRITE LF@$const%d\n", ExpTmp++);
		char *code = PopStack(StackG);
		free(code);
	} else {
		GeneratorPrintWrapper(0,"DEFVAR TF@%%%d\n", order);
		GeneratorPrintWrapper(0,"MOVE TF@%%%d LF@$%s\n", order, name);
	}
}

/* Define start of function in IFJcode18 */
void GeneratorFunctionDefinition(char *name){
	fprintf(stdout,"#Start of function called: %s.\n", name);
	fprintf(stdout,"JUMP $$function%sEnd\n", name);
	PushStack(StackFunction, name);
	fprintf(stdout,"LABEL $%s\n", name);
	fprintf(stdout,"PUSHFRAME\n");
	fprintf(stdout,"DEFVAR LF@%%retval\n");
	fprintf(stdout,"MOVE LF@%%retval nil@nil\n");
	fprintf(stdout,"#Parameters: \n");
}

/* Parameter used in funct definicion */
void GeneratorParameterIn(int order, char *name){
	fprintf(stdout,"DEFVAR LF@$%s\n", name);
	fprintf(stdout,"MOVE LF@$%s LF@%%%d\n", name, order);
}

/* End of function */
void GeneratorFunctionEnd(){
	fprintf(stdout,"POPFRAME\n");
	fprintf(stdout,"RETURN\n");
	char *code = PopStack(StackFunction);
	fprintf(stdout,"LABEL $$function%sEnd\n", code);
	free(code);
}

/* Generate RETVAL for function */
void GeneratorRetValInFunction(char *name){
	GeneratorPrintWrapper(0,"MOVE LF@%%retval LF@$%s\n", name);
}

/* Generate */
void GeneratorRetValAssign(char *name){
	GeneratorPrintWrapper(0,"MOVE %s TF@%%retval \n", name);
}

void GeneratorPrintWrapper(int size, const char *format, ...) {
    if (!format)
        return;

    if (size == 0)
	size = 400;

    va_list args;
    va_start(args, format);

    if (!WhileEnd) {
        vprintf(format, args);
    } else {
	char *str = malloc(size);
	vsprintf(str, format, args);
	PushStack(StackWhileAll, str);
    }

    va_end(args);
}



/* Generate While statement in IFJcode18 */
void GeneratorWhile(){
	/* If While Generating */
	GeneratorPrintWrapper(0,"LABEL $WhileStartLabel%d\n", WhileLabel);
	char *codeWhileStart = malloc(sizeof(char) * ownStrLen("JUMPIFEQ $WhileEndLabel GF@$cond bool@false") + sizeof(int) + 1);
	sprintf(codeWhileStart,"JUMPIFEQ $WhileEndLabel%d GF@$cond bool@false", WhileLabel);	

	/* End While Generating */
	char *codeEndWhile1 = malloc(sizeof(char) * ownStrLen("JUMP $WhileStartLabel") + sizeof(int) + 1);
	char *codeEndWhile2 = malloc(sizeof(char) * ownStrLen("LABEL $WhileEndLabel") + sizeof(int) + 1);
	sprintf(codeEndWhile1,"JUMP $WhileStartLabel%d", WhileLabel);	
	sprintf(codeEndWhile2,"LABEL $WhileEndLabel%d", WhileLabel++);	

	/* Pushing On Stack */
	PushStack(StackWhile, codeEndWhile2);
	PushStack(StackWhile, codeEndWhile1);
	PushStack(StackWhile, codeWhileStart);

	free(codeEndWhile1);
	free(codeEndWhile2);
        free(codeWhileStart);
}

/* Generate If statement in IFJcode18 */
void GeneratorIf(){
	/* If Start Generating */
	char *codeIfStart = malloc(sizeof(char) * ownStrLen("JUMPIFEQ $IfElseLabel GF@$cond bool@false") + sizeof(int) + 1);
	sprintf(codeIfStart,"JUMPIFEQ $IfElseLabel%d GF@$cond bool@false", IfLabel);	

	/* Else Start Generating */
	char *codeElse1 = malloc(sizeof(char) * ownStrLen("JUMP $IfEndLabel") + sizeof(int) + 1);
	char *codeElse2 = malloc(sizeof(char) * ownStrLen("LABEL $IfElseLabel") + sizeof(int) + 1);
	sprintf(codeElse1,"JUMP $IfEndLabel%d", IfLabel);	
	sprintf(codeElse2,"LABEL $IfElseLabel%d", IfLabel);	

	/* End If Generating */
	char *codeEnd = malloc(sizeof(char) * ownStrLen("LABEL $IfEndLabel") + sizeof(int) + 1);
	sprintf(codeEnd,"LABEL $IfEndLabel%d", IfLabel++);	

	/* Pushing On Stack */
	PushStack(StackIf, codeEnd);
	PushStack(StackIf, codeElse2);
	PushStack(StackIf, codeElse1);
	PushStack(StackIf, codeIfStart);

	free(codeIfStart);
	free(codeElse1);
	free(codeElse2);
	free(codeEnd);
}

/* Prints from top of the stack */
void GeneratorStackPrint(StackGen StackG){
	char *code = PopStack(StackG);
	if (code){
		GeneratorPrintWrapper(0,"%s\n", code);
		free(code);
	}
}

/* Generate whole expression for condition and statement */
void GeneratorExpression(ExL Ex, bool floatOccur, bool intOccur){
int operandCount = 0;
        Expr tmp;
        if (Ex->First)
            tmp = Ex->First;

        if (Ex->count == 1){
                PushStack(StackG, Ex->Last->code);
                free(Ex->Last->name); 
                free(Ex->Last->code); 
                free(Ex->Last); 
        } else {
            while (Ex->First){
                if (floatOccur && intOccur){
                    if (tmp->lexem == INT){
                        tmp->lexem = FLOAT;
                        GeneratorPrintWrapper(0,"INT2FLOAT %s %s\n", tmp->code, tmp->code);
                    }
                }

                if (tmp->lexem == INT ||
                    tmp->lexem == STR ||
                    tmp->lexem == FLOAT ||
                    tmp->lexem == IDENT){
                    operandCount++;
                }

                if (tmp->lexem == PLUS ||
                    tmp->lexem == MINUS ||
                    tmp->lexem == MULTIPLY ||
                    tmp->lexem == DIVISION ||
                    tmp->lexem == LESS ||
                    tmp->lexem == MORE ||
                    tmp->lexem == EQ ||
                    tmp->lexem == LESSEQ ||
                    tmp->lexem == MOREEQ ||
                    tmp->lexem == NOTEQ){
                    operandCount = 0;
                }

                if (operandCount == 2){
                    char *sign = tmp->Before->Before->code;
                    char *symb1 = tmp->Before->code;
                    char *symb2 = tmp->code;
		    if (tmp->Before->Before->lexem == PLUS &&
                       (tmp->Before->lexem == STR ||
                        tmp->lexem == STR))
                        GeneratorConcat(tmp, symb1, symb2);
	            else {
                        if(tmp->Before->Before->lexem == LESS ||
                           tmp->Before->Before->lexem == MORE ||
                           tmp->Before->Before->lexem == LESSEQ ||
                           tmp->Before->Before->lexem == MOREEQ ||
                           tmp->Before->Before->lexem == EQ ||       
                           tmp->Before->Before->lexem == NOTEQ){   
                            	GeneratorPrintWrapper(0,"%s %s %s\n", sign, symb1, symb2);
		        	if (tmp->Before->Before->lexem == NOTEQ)
					GeneratorPrintWrapper(0,"NOT GF@$cond GF@$cond\n");    

                       } else {
			    if (!strcmp(tmp->Before->Before->code, "DIV") && 
			        tmp->Before->lexem == INT && tmp->lexem == INT)
				GeneratorPrintWrapper(0,"IDIV %s %s %s\n", symb1, symb1, symb2);
  	                    else
				GeneratorPrintWrapper(0,"%s %s %s %s\n", sign, symb1, symb1, symb2);
			}
		    }
                    GeneratorDeleteExpression(Ex, tmp->Before->Before);         
                    GeneratorDeleteExpression(Ex, tmp);          
                    operandCount = 0;
                }
		
                if (!tmp->Next){
                    tmp = Ex->First;
                }
                else
                    tmp = tmp->Next;
		

                if(Ex->count == 2){
                    if(Ex->First->lexem != LESS &&
                       Ex->First->lexem != MORE &&
                       Ex->First->lexem != LESSEQ &&
                       Ex->First->lexem != MOREEQ &&
                       Ex->First->lexem != EQ &&
                       Ex->First->lexem != NOTEQ)       
                        PushStack(StackG, Ex->Last->code);
		    free(Ex->First->code);
                    free(Ex->Last->code);
                    free(Ex->First->name);
                    free(Ex->Last->name);
                    free(Ex->First);
                    free(Ex->Last);
		    break;
                }
            }   
        } 
	free(Ex);	
}
/* Function used in main.c its just touch Generator create stack and start of IFJcode18 program*/
void Generator(){
    StackG = CreateStack();	
    StackAssign = CreateStack();
    StackIf = CreateStack();    
    StackFunction = CreateStack();
    StackWhile = CreateStack();
    StackWhileAll = CreateStack();

    fprintf(stdout,".IFJcode18\n");
    fprintf(stdout,"CREATEFRAME\n");
    fprintf(stdout,"PUSHFRAME\n");
    fprintf(stdout,"DEFVAR GF@$cond\n");

}




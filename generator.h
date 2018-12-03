/* IFJ Project 2018/19
 * Implementation of Intermediate code Generator
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: generator.h
 *  FILE AUTHOR:
 */

#ifndef GENERATOR_H
#define GENERATOR_H

/***LOCAL FILES***/
#include "parser.h"

/* Generates code from derivation tree into intermediate code
 * @tree derivation tree
 * Returns: Intermediate code as a string
 */

#define MAXSIZE 1000


typedef struct ExprS *Expr;
struct ExprS {
    lexems lexem;
    char *name;
    char *code;
    struct ExprS *Next;
    struct ExprS *Before;
};

typedef struct ExList *ExL;
struct ExList {
    struct ExprS *First;
    struct ExprS *Last;
};
    
typedef struct StackGen *StackGen;
struct StackGen {
    char *code[MAXSIZE];
    int top;
};

StackGen StackG;
StackGen StackAssign;

StackGen CreateStack();
bool PushStack(StackGen StackS, char *code);
bool GenEmptyStack(StackGen StackS);
char *PopStack(StackGen StackS);
void Generator(FILE *file);

bool GeneratorAddExpression(ExL Ex, char *name, lexems lexem);
void GeneratorDeleteExpression(ExL ExpL, Expr Delete);

void GeneratorAssign(char *name, bool defined);
char *GeneratorMathOperation(lexems lexem);
char *GeneratorVariable(char *name);
char *GeneratorConstantDefine(lexems lexem, char *name);
void GeneratorFunctionDefinition(char *name);
void GeneratorParameterIn(int order, char *name);
void GeneratorFunctionEnd();
void GeneratorFunctionCall(char *name);
void GeneratorParameterOut(int order, char *name, lexems lexem);
void GeneratorIfStart();
void GeneratorAfterIf();
void GeneratorWhileStartLabel();
void GeneratorWhileCondEvaluation();
void GeneratorWhileEnd();
char *GeneratorMatcher(lexems lexem);
char *GeneratorCharAppend(char *name);
#endif

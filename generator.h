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
    int count;
    struct ExprS *First;
    struct ExprS *Last;
};
    
typedef struct StackGen *StackGen;
struct StackGen {
    char *code[MAXSIZE];
    int top;
};

extern int WhileEnd;

StackGen StackG;
StackGen StackAssign;
StackGen StackIf;
StackGen StackFunction;
StackGen StackWhile;
StackGen StackWhileAll;

StackGen CreateStack();
bool PushStack(StackGen StackS, char *code);
bool GenEmptyStack(StackGen StackS);
char *PopStack(StackGen StackS);
char *FrontStack(StackGen StackS);
void Generator(FILE *file);

bool GeneratorAddExpression(ExL Ex, char *name, lexems lexem);
void GeneratorDeleteExpression(ExL ExpL, Expr Delete);
void GeneratorExpression(ExL Ex, bool floatOccur, bool intOccur);

int ownStrLen(char *name);
void GeneratorAssign(char *name, bool defined);
void GeneratorAssignPrint();
char *GeneratorMathOperation(lexems lexem);
char *GeneratorVariable(char *name);
char *GeneratorConstantDefine(lexems lexem, char *name);
void GeneratorFunctionDefinition(char *name);
void GeneratorParameterIn(int order, char *name);
void GeneratorFunctionEnd();
void GeneratorFunctionCall(char *name);
void GeneratorParameterOut(int order, char *name, lexems lexem);
void GeneratorRetValInFunction(char *name);
void GeneratorRetValAssign(char *name);
void GeneratorIf();
void GeneratorWhile();
char *GeneratorMatcher(lexems lexem);
char *GeneratorCharAppend(char *name);
void GeneratorConcat(Expr Ex, char *symb1, char *symb2);
void GeneratorStackPrint(StackGen StackG);
void GeneratorPrintWrapper(int size, const char *format, ...);

#endif

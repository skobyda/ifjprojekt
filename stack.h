/* IFJ Project 2018/19
 * Implementation of Stack of tokens
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: stack.h
 *  FILE AUTHOR: Simon Kobyda
 */

#ifndef STACK_H
#define STACK_H

/***LOCAL FILES***/
#include "scanner.h"

#define MAXSTACKSIZE 1000

//TODO define operation integers

typedef struct Stack *StackPtr;

struct Stack{
    TokenPtr arr[MAXSTACKSIZE];
    int top;
};

/* Runs Syntax analysis over code.
 */
StackPtr StackCreate();
void StackDestroy(StackPtr stack);
bool StackPush(StackPtr stack, TokenPtr token);
TokenPtr StackPop(StackPtr stack);

#endif

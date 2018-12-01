/* IFJ Project 2018/19
 * Implementation of Stack of tokens
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: stack.c
 *  FILE AUTHOR: Simon Kobyda
 */

/***SYSTEM FILES***/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/***LOCAL FILES***/
#include "scanner.h"
#include "stack.h"

StackPtr StackCreate() {
    StackPtr stack = malloc(sizeof(struct Stack));
    if (!stack)
        return NULL;

    stack->top = 0;

    return stack;
}

void StackDestroy(StackPtr stack) {
    if (!stack)
        return;

    //printf("STACK TOP JE %d\n", stack->top);
    for (int i = 0; i < stack->top; i++) {
        free(stack->arr[i]->name);
        free(stack->arr[i]);
    }

    free(stack);
}

bool StackPush(StackPtr stack, TokenPtr token) {
    TokenPtr newtoken = malloc(sizeof(Token));
    if (!newtoken)
        return false;
    newtoken->lexem = token->lexem;
    newtoken->line = token->line;
    if (token->name) {
        newtoken->name = malloc(sizeof(char) * (strlen(token->name) + 1));
        strcpy(newtoken->name, token->name);
    } else {
        token->name = NULL;
    }

    stack->arr[stack->top++] = newtoken;

    return true;
}

TokenPtr StackPop(StackPtr stack) {
    if(!stack->top)
        return NULL;

    return stack->arr[--stack->top];
}

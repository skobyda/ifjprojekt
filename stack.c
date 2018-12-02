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

    for (int i = 0; i < stack->top; i++) {
        free(stack->arr[i]->name);
        free(stack->arr[i]);
    }

    free(stack);
}

bool StackPush(StackPtr stack, TokenPtr token) {
    stack->arr[stack->top++] = token;

    return true;
}

TokenPtr StackPop(StackPtr stack) {
    if(!stack->top)
        return NULL;

    return stack->arr[--stack->top];
}

/* IFJ Project 2018/19
 * Implementation of Lexical analyser
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: scanner.c
 *  FILE AUTHOR: 
 */

/***SYSTEM FILES***/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/***LOCAL FILES***/
#include "scanner.h"
#include "ourstring.h"

/* Firstly, file with source code is passed to Scanner
 *
 * @sourceCode File with code written in IFJ18 language
 *
 * Returns: true in case of success, false in case of error
 */
bool
Scanner(FILE *sourceCode) {
    (void)(sourceCode); //REMOVE THIS LINE

    //TODO

    return true;
}

/* 
 * Returns: The next token.
 * If there are no more tokens, returns NULL
 */
TokenPtr
ScannerGetToken() {
    TokenPtr Token = NULL;

    //TODO

    return Token;
}

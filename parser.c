/* IFJ Project 2018/19
 * Implementation of Lexical analyser
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: parser.c
 *  FILE AUTHOR: TODO
 */

/***SYSTEM FILES***/
#include <stdio.h>

/***LOCAL FILES***/
#include "scanner.h"

/*
 * @input_code file with source code to be compiled
 *
 * Return: intermediate code
 */
char *Parser(FILE *input_code) {
    TokenPtr token = NULL;

    Scanner(input_code);

    do token = ScannerGetToken();
    while (!token);

    return NULL;
}

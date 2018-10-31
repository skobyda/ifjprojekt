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
 *  FILE AUTHOR: Simon Kobyda
 */

/***SYSTEM FILES***/
#include <stdio.h>

/***LOCAL FILES***/
#include "scanner.h"
#include "parser.h"
#include "parser-semantic.h"
#include "generator.h"

/*
 * @input_code file with source code to be compiled
 *
 * Return: intermediate code
 */
ParTreePtr
Parser() {
    TokenPtr token = NULL;

    /* Asks for tokens from scanner */
    do {
        token = ScannerGetToken();
    } while (token);

    /* Creates Derivation tree from tokens */
    ParTreePtr tree = NULL;
    //TODO

    return tree;
}

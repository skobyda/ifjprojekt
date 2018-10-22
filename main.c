/* IFJ Projekt
 * Implementantion of compiler for IFJ18
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: main.c
 *  FILE AUTHOR: Simon Kobyda (xkobyd00)
 */

/***SYSTEM FILES***/
#include <stdio.h>
#include <stdlib.h>

/***LOCAL FILES***/
#include "parser.h"
#include "parser-semantic.h"
#include "scanner.h"
#include "generator.h"

int main(int argc, char **argv) {
    (void)(argc);
    (void)(argv);
    FILE *inputCode;
    char* intermediateCode;

    inputCode = fopen("code.ifj18", "r");
    if (!inputCode)
        goto error;

    /* Pokes Scanner*/
    Scanner(inputCode);

    ParTreePtr tree = Parser();

    /* Runs semantic analysis over symbolic table */
    //if (!ParserSemantic(tree))
        //TODO

    /* Converts symbol table to intermediate code */
    intermediateCode = Generator(tree);
    //if (!intermediateCode)
        //TODO

    printf("INTERMEDIATE CODE IS:\n %s", intermediateCode);

 error:
    //TODO
    fprintf(stderr, "THIS COMPILER DOESN'T WORK YET :)\n");
    return 0;
}

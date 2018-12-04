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

int retval = 0;

void PrintError(int val, unsigned int line, const char *message) {
    if (!message)
        return;

    if (!retval)
        retval = val;

    if (line)
        fprintf(stderr, "ERROR: Line %u: %s\n", line, message);
    else
        fprintf(stderr, "ERROR: %s\n", message);
}

int main(int argc, char **argv) {
    (void)(argc);
    (void)(argv);
    FILE *inputCode;
    FILE *outputCode;

    inputCode = fopen("code.ifj18", "r");
    outputCode = fopen("outputcode.IFJcode18", "wr");

    if (!inputCode || !outputCode)
        goto error;

    /* Pokes Scanner*/
    Scanner(inputCode);

    /* Pokes generator */
    Generator(outputCode);

    ParTreePtr tree = Parser();
    (void)(tree);

    /* Runs semantic analysis over symbolic table */
    //if (!ParserSemantic(tree))
        //TODO

    fclose(inputCode);

 error:
    //TODO
    fprintf(stderr, "THIS COMPILER DOESN'T WORK YET :)\n");
    return retval;
}

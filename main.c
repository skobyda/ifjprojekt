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

int main(int argc, char **argv) {
    (void)(argc);
    (void)(argv);
    FILE *input_code;
    char *intermediate_code;

    input_code = fopen("code.ifj18", "r");
    if (!input_code)
        goto error;

    //Kontrola prekladaca je predana parseru
    intermediate_code = Parser(input_code);
    if (!intermediate_code)
        goto error;

    return 0;
 error:
    //TODO
    fprintf(stderr, "ERROR happened!\n");
    return 0;
}

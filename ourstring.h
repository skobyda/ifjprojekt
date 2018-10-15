/* IFJ Project 2018/19
 * Implementation of Lexical analyser
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: ourstring.h
 *  FILE AUTHOR:
 */

#ifndef SCANNER_H
#define SCANNER_H

/***SYSTEM FILES***/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*Structure for work with strings*/

typedef struct {
	char *sPtr;        // pointer to some string with /0 at the end
	int length;        // number of letters in string
	int Size;    // size of memory allocated for string
} ourstring;

int GetLenght(*ourstring)
{
return ourstring->lenght;
}
#endif

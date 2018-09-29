/* IFJ Project 2018/19
 * Implementation of Semantic Analyser
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: parser-semantic.h
 *  FILE AUTHOR:
 */

#ifndef PARSERSEMANTIC_H
#define PARSERSEMANTIC_H

/***SYSTEM FILES***/
#include <stdbool.h>

/***LOCAL FILES***/
#include "symtable.h"

/* Runs Semantic analysis over symbol table. Edits table, or
 * throws some error if semantic bug was detected.
 * @table Pointer to symbol table. This table may be edited.
 * Returns: true in case of success, false in case of error
 */
bool ParserSemantic(SymTablePtr table);

#endif

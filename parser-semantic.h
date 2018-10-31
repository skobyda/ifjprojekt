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
#include "parser.h"

/* Runs Semantic analysis over derivation tree.
 * throws some error if semantic bug was detected.
 * @tree Pointer to derivation tree.
 * Returns: true in case of success, false in case of error
 */
bool ParserSemantic(ParTreePtr tree);

#endif

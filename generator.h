/* IFJ Project 2018/19
 * Implementation of Intermediate code Generator
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: generator.h
 *  FILE AUTHOR:
 */

#ifndef GENERATOR_H
#define GENERATOR_H

/***LOCAL FILES***/
#include "symtable.h"

/* Generates code from symbol table into intermediate code
 * @table symbol table
 * Returns: Intermediate code as a string
 */
char *Generator(SymTablePtr table);

#endif

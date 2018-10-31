/* IFJ Project 2018/19
 * Implementation of Lexical analyser
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: parser.h
 *  FILE AUTHOR: Simon Kobyda
 */

#ifndef PARSER_H
#define PARSER_H

/***LOCAL FILES***/
#include "scanner.h"

//TODO define operation integers

typedef struct ParTree *ParTreePtr;

struct ParTree {
    TokenPtr token;
    ParTreePtr left;
    ParTreePtr right;
    int operation;
};

typedef struct ParTree ParTree;

/* Runs Syntax analysis over code.
 */
ParTreePtr Parser();

#endif

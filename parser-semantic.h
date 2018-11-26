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

typedef enum {
    WHILEST, //left node -> condition, right node -> block of statements
    IFST, // left node -> condition, right node -> block of statements
    //ENDWHILE,
    //ENDIF,
    BLOCKIF, // left node -> then statements, right node -> else statements
    COMPARATOR, // left node -> left operand, right node -> right operand
    FUNCTIONDEF, // left node -> argument(s), right node -> block of statements
    //ENDFUNCTION,
    FUNCTIONCALL, // left node -> argument(s)
    ARGUMENT, // left node -> next argument
    VARIABLE,
    CONSTANT,
    OPER, // left node -> left operand, right node -> right operand
    STATEMENT, // left node -> current statement, right node -> next statement
    ASSIGNMENT, // left node -> right hand side , right node -> land hand side of expression   
} nodeType;

typedef enum {
    EQU, //==
    LT,  //<
    GT,  //>
    LOE, //<=
    GOE, //>=
    NEQE,//!=
} compType;

typedef enum {
    OADD, //+
    OSUB, //-
    OMUL, //*
    ODIV, // /
} opType;

typedef union {
    bool bVal;
    int iVal;
    float fVal;
    char *sVal;
} Value;
      
typedef struct AbsTreeNode {
    nodeType nType;
    compType cmpType;
    opType oType;
    char *name; //contains name of variable or function, otherwise NULL
    bool declared; //if variable already exists -> TRUE
    Value val; //if constant contains its value
    struct AbsTreeNode *left;
    struct AbsTreeNode *right;
    struct AbsTreeNode *parent;                                  
} *ATreeNodePtr;

// Initializes abstract tree before first usage
void SemanticTreeInit (ATreeNodePtr *RootPtr);

/* Runs Semantic analysis over derivation tree.
 * throws some error if semantic bug was detected.
 * @tree Pointer to derivation tree.
 * Returns: true in case of success, false in case of error
 */
bool ParserSemantic(ParTreePtr tree);

#endif

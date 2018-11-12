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
#include <stdbool.h>

/***LOCAL FILES***/
#include "scanner.h"
#include "parser.h"
#include "parser-semantic.h"
#include "generator.h"

/* Always point to current token */
TokenPtr token = NULL;

//static bool ParserStatList();
static bool ParserStatement();
static bool ParserDeclaration();
static bool ParserIfStatement();
static bool ParserExpression();

/****** REMOVE THIS LATER ******/
int tokenCounter = 0;
#define TOKENNUM 18
Token DummyToken[TOKENNUM] = {
    {ID, 1, "x"},
    {ADDITION, 1, NULL},
    {ID, 1, "1"},
    {EOL, 1, NULL},
    {IF, 2, NULL},
    {ID, 2, "x"},
    {THEN, 2, NULL},
    {EOL, 2, NULL},
    {ID, 3, "x"},
    {ADDITION, 3, NULL},
    {ID, 3, "2"},
    {EOL, 3, NULL},
    {END, 4, NULL},
    {EOL, 4, NULL},
    {ID, 5, "y"},
    {ADDITION, 5, NULL},
    {ID, 5, "A"},
    {EOFile, 5, NULL},
};
static TokenPtr DummyGetToken() {
    if (tokenCounter == TOKENNUM)
        return NULL;
    else
        return &DummyToken[tokenCounter++];
}
/****** REMOVE THIS LATER ******/

#define FUNCTIONCALL(function) do { \
    if (!function()) { \
        printf("ERROR\n"); \
    } \
} while(0)

//TODO remove DummyGetToken()
#define NEXTTOKEN do { \
    token = DummyGetToken(); \
    if (!token) \
        return false; \
} while(0)

/* Rule of LL gramar for list of statements.
 */
/* static bool ParserStatList() {
    printf("StatList\n");

    NEXTTOKEN;

    switch (token->lexem) {
        case EOFile:
            return true;
            break;
        default:
            FUNCTIONCALL(ParserStatement);
            break;
    }

    return true;
}*/

/* Rule of LL gramar for statement.
 */
static bool ParserStatement() {
    printf("Statement\n");

    NEXTTOKEN;

    switch (token->lexem) {
        case ID:
            FUNCTIONCALL(ParserDeclaration);
            break;
        case IF:
            FUNCTIONCALL(ParserIfStatement);
            break;
        case EOL:
            FUNCTIONCALL(ParserStatement);
            break;
        case EOFile:
        case END:
            break;
        default:
            return false;
    }

    return true;
}

/* Rule of LL gramar for If statement.
 */
static bool ParserIfStatement() {
    printf("If\n");

    FUNCTIONCALL(ParserExpression);
    if (token->lexem != THEN)
        printf("ERROR\n"); \

    NEXTTOKEN;
    if (token->lexem != EOL)
        printf("ERROR\n"); \

    FUNCTIONCALL(ParserStatement);

    if (token->lexem != END)
        printf("ERROR\n"); \

    FUNCTIONCALL(ParserStatement);

    return true;
}

/* Rule of LL gramar for declaration and definiton of
 * variables or functions
 */
static bool ParserDeclaration() {
    printf("Declaration\n");

    NEXTTOKEN;

    switch (token->lexem) {
        case ADDITION: // It's declaration of variable
            FUNCTIONCALL(ParserExpression);
            break;
        //case LEFTBRACKET: // It's declaration of function
        //    FUNCTIONCALL(ParserArguments)
        default:
            return false;
    }

    return true;
}

/* Rule of LL gramar for expression.
 * This expression will handled by precedence parsing.
 */
static bool ParserExpression() {
    printf("Expression\n");

    NEXTTOKEN;

    if (token->lexem != ID) {
       printf("ERROR\n"); //TODO remove this condition after precedence p. will be implemented
       return false;
    }
    //TODO parser control for this expression will be handled to precedence parser

    while (token->lexem != EOL && token->lexem != THEN && token->lexem != EOFile)
        NEXTTOKEN;

    switch (token->lexem) {
        case EOL:
            FUNCTIONCALL(ParserStatement);
            break;
        case THEN:
            break;
        case EOFile:
            break;
        default:
            return false;
    }

    return true;
}

/*
 * @input_code file with source code to be compiled
 *
 * Return: intermediate code
 */
ParTreePtr
Parser() {

    /* First token should always be prog rule, so we call it */
    FUNCTIONCALL(ParserStatement);

    //} while (token);

    /* Creates Derivation tree from tokens */
    ParTreePtr tree = NULL;
    //TODO

    return tree;
}

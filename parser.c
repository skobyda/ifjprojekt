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
static bool ParserFunctionDeclaration();
static bool ParserWhile();
static bool ParserExpression();

/****** REMOVE THIS LATER ******/
int tokenCounter = 0;
#define TOKENNUM 30
Token DummyToken[TOKENNUM] = {
    {IDENT, 1, "x"},
    {ADDITION, 1, NULL},
    {IDENT, 1, "1"},
    {EOL, 1, NULL},

    {IF, 2, NULL},
    {IDENT, 2, "x"},
    {THEN, 2, NULL},
    {EOL, 2, NULL},

    {IDENT, 3, "x"},
    {ADDITION, 3, NULL},
    {IDENT, 3, "2"},
    {EOL, 3, NULL},

    {ELSE, 4, NULL},
    {EOL, 4, NULL},

    {WHILE, 5, NULL},
    {IDENT, 5, "x"},
    {DO, 5, NULL},
    {EOL, 5, NULL},

    {IDENT, 6, "x"},
    {ADDITION, 6, NULL},
    {IDENT, 6, "2"},
    {EOL, 6, NULL},

    {END, 7, NULL},
    {EOL, 7, NULL},

    {END, 8, NULL},
    {EOL, 8, NULL},

    {IDENT, 9, "y"},
    {ADDITION, 9, NULL},
    {IDENT, 9, "A"},
    {EOFILE, 9, NULL},
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
        case EOFILE:
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
        case DEF:
            FUNCTIONCALL(ParserFunctionDeclaration);
            break;
        case IDENT:
            FUNCTIONCALL(ParserDeclaration);
            break;
        case IF:
            FUNCTIONCALL(ParserIfStatement);
            break;
        case WHILE:
            FUNCTIONCALL(ParserWhile);
            break;
        case EOL:
            FUNCTIONCALL(ParserStatement);
            break;
        case EOFILE:
        case ELSE:
        case END:
            break;
        default:
            return false;
    }

    return true;
}

/* Rule of LL gramar for If statement.
 */
static bool ParserFunctionDeclaration() {
    printf("Function\n");

    NEXTTOKEN;
    if (token->lexem != IDENT)
        printf("ERROR\n");

    NEXTTOKEN;
    if (token->lexem != LEFT_B)
        printf("ERROR\n");

    NEXTTOKEN;
    while (token->lexem != RIGHT_B) {
        if (token->lexem != IDENT)
            printf("ERROR\n");
        NEXTTOKEN;
    }

    NEXTTOKEN;
    if (token->lexem != RIGHT_B)
        printf("ERROR\n");

    NEXTTOKEN;
    if (token->lexem != EOL)
        printf("ERROR\n");

    FUNCTIONCALL(ParserStatement);

    return true;
}

/* Rule of LL gramar for If statement.
 */
static bool ParserIfStatement() {
    printf("If\n");

    FUNCTIONCALL(ParserExpression);
    if (token->lexem != THEN)
        printf("ERROR\n");

    NEXTTOKEN;
    if (token->lexem != EOL)
        printf("ERROR\n");

    FUNCTIONCALL(ParserStatement);

    if (token->lexem == ELSE)
        FUNCTIONCALL(ParserStatement);

    if (token->lexem != END)
        printf("ERROR\n");

    FUNCTIONCALL(ParserStatement);

    return true;
}

/* Rule of LL gramar for If statement.
 */
static bool ParserWhile() {
    printf("While\n");

    FUNCTIONCALL(ParserExpression);
    if (token->lexem != DO)
        printf("ERROR\n");

    NEXTTOKEN;
    if (token->lexem != EOL)
        printf("ERROR\n");

    FUNCTIONCALL(ParserStatement);

    if (token->lexem != END)
        printf("ERROR\n");

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

    if (token->lexem != IDENT) {
       printf("ERROR\n"); //TODO remove this condition after precedence p. will be implemented
       return false;
    }
    //TODO parser control for this expression will be handled to precedence parser

    while (token->lexem != EOL && token->lexem != DO && token->lexem != THEN && token->lexem != EOFILE)
        NEXTTOKEN;

    switch (token->lexem) {
        case EOL:
            FUNCTIONCALL(ParserStatement);
            break;
        case THEN:
        case DO:
            break;
        case EOFILE:
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

    // In case parser got out of parsing because of error,
    // we want to parse rest of the program anyway
    NEXTTOKEN;
    while (token->lexem != EOFILE) {
        FUNCTIONCALL(ParserStatement);
    }

    //} while (token);

    /* Creates Derivation tree from tokens */
    ParTreePtr tree = NULL;
    //TODO

    return tree;
}

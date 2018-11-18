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
static bool ParserArguments();
static bool ParserStatement();
static bool ParserDeclaration();
static bool ParserIfStatement();
static bool ParserFunctionDeclaration();
static bool ParserWhile();
static bool ParserExpression();

/****** REMOVE THIS LATER ******/
/*
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
*/
/****** REMOVE THIS LATER ******/

#define FUNCTIONCALL(function) do { \
    if (!function()) { \
        printf("ERROR\n"); \
    } \
} while(0)

#define NEXTTOKEN do { \
    if (token) \
        free (token->name); \
    free(token); \
    token = ScannerGetToken(); \
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

/* Rule of LL gramar for Arguments of function calling.
 */
static bool ParserArguments() {
    printf("Functioncall arguments\n");
    bool flag = true; // set to true if expects Identificator, false if expects comma

    /* Asks for tokens (arguments), until it sees right bracket */
    NEXTTOKEN;
    while (token->lexem != RIGHT_B) {
        if (flag && token->lexem != IDENT) {
            printf("ERROR\n");
            goto end;
        } else if (!flag && token->lexem != COMA) {
            printf("ERROR\n");
            goto end;
        }

        // to secure switching of argument/comma
        flag = !flag;

        NEXTTOKEN;
    }

    /* Expects end of line after right comma */
    NEXTTOKEN;
    if (token->lexem != EOL) {
        printf("ERROR\n");
        goto end;
    }

 end:
    while (token->lexem != EOL)
        NEXTTOKEN;
    
    /* Statement on another line */
    FUNCTIONCALL(ParserStatement);

    return true;
}

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
            printf("Else\n");
            break;
        case END:
            printf("End\n");
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
    bool flag = true; // set to true if expects identificator, false if expects comma

    /* Expects name of function */
    NEXTTOKEN;
    if (token->lexem != IDENT) {
        printf("ERROR\n");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* Expects left bracked after function name */
    NEXTTOKEN;
    if (token->lexem != LEFT_B) {
        printf("ERROR\n");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* Reads parameters of function until it sees right bracket */
    NEXTTOKEN;
    while (token->lexem != RIGHT_B) {
        if (flag && token->lexem != IDENT) {
            printf("ERROR\n");
            while (token->lexem != EOL)
                NEXTTOKEN;
        } else if (!flag && token->lexem != COMA) {
            printf("ERROR\n");
            while (token->lexem != EOL)
                NEXTTOKEN;
        }

        // to secure switching of argument/comma
        flag = !flag;
        NEXTTOKEN;
    }

    /* Expects end of line after right bracket */
    NEXTTOKEN;
    if (token->lexem != EOL) {
        printf("ERROR\n");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* Statement(s) inside a function (aka body of function) */
    FUNCTIONCALL(ParserStatement);

    /* Body of function should end with 'END' */
    if (token->lexem != END)
        printf("ERROR\n");

    /* Expects end of line after 'END' */
    NEXTTOKEN;
    if (token->lexem != EOL) {
        printf("ERROR\n");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* Statements after function (rest of the program) */
    FUNCTIONCALL(ParserStatement);

    return true;
}

/* Rule of LL gramar for If statement.
 */
static bool ParserIfStatement() {
    printf("If\n");

    /* Function's condition can be parsed as expression */
    FUNCTIONCALL(ParserExpression);

    /* Expects 'end' after function's condition */
    if (token->lexem != THEN)
        printf("ERROR\n");

    /* Expects end of line after 'end' */
    NEXTTOKEN;
    if (token->lexem != EOL) {
        printf("ERROR\n");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* Function's block of code */
    FUNCTIONCALL(ParserStatement);

    /* ELSE (optional) */
    if (token->lexem == ELSE) {
        /* Expects end of line after 'else' */
        NEXTTOKEN;
        if (token->lexem != EOL) {
            printf("ERROR\n");
            while (token->lexem != EOL)
                NEXTTOKEN;
        }

        /* Else's block of code */
        FUNCTIONCALL(ParserStatement);
    }

    /* Expects 'end' at the end of if/else statement  */
    if (token->lexem != END)
        printf("ERROR\n");

    /* Expects end of line after 'end' */
    NEXTTOKEN;
    if (token->lexem != EOL) {
        printf("ERROR\n");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* Rest of the program */
    FUNCTIONCALL(ParserStatement);

    return true;
}

/* Rule of LL gramar for If statement.
 */
static bool ParserWhile() {
    printf("While\n");

    /* While's condition, can be parsed as Expression */
    FUNCTIONCALL(ParserExpression);

    /* Expects 'do' after while's condition */
    if (token->lexem != DO)
        printf("ERROR\n");

    /* Expects end of line after 'do' */
    NEXTTOKEN;
    if (token->lexem != EOL) {
        printf("ERROR\n");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* While's block of code */
    FUNCTIONCALL(ParserStatement);

    /* Expects 'end' at the end of while statement */
    if (token->lexem != END)
        printf("ERROR\n");

    NEXTTOKEN;
    /* Expects end of line after 'end' */
    if (token->lexem != EOL) {
        printf("ERROR\n");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* Rest of the program */
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
        case LEFT_B: // It's declaration of function
            FUNCTIONCALL(ParserArguments);
            break;
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
    if (token->lexem != IDENT &&
        token->lexem != STR &&
        token->lexem != INT &&
        token->lexem != FLOAT &&
        token->lexem != NIL) {
       printf("ERROR\n"); //TODO remove this condition after precedence p. will be implemented
       return false;
    }
    //TODO parser control for this expression will be handled to precedence parser

    while (token->lexem != EOL &&
           token->lexem != DO &&
           token->lexem != THEN &&
           token->lexem != EOFILE)
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

    if (token) \
        free (token->name); \
    free(token);

    return tree;
}

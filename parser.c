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
#include "generator.h"
#include "parser.h"
#include "parser-semantic.h"
#include "scanner.h"
#include "stack.h"
#include "symtable.h"

/* Always point to current token */
TokenPtr token = NULL;
/* Level of depth of code */
int codeDepth = 0;
SymTablePtr currentTable;

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

static void AuxPrintToken(TokenPtr token) {
    char *lexem;
    char *name;

    switch (token->lexem) {
        case EOL: lexem = "EOL"; break;
        case EOFILE: lexem = "EOFILE"; break;
        case IDENT: lexem = "IDENT"; break;
        case STR: lexem = "STR"; break;
        case INT: lexem = "INT"; break;
        case FLOAT: lexem = "FLOAT"; break;
        case NIL: lexem = "NIL"; break;
        case LEFT_B: lexem = "LEFT_B"; break;
        case RIGHT_B: lexem = "RIGHT_B"; break;
        case PLUS: lexem = "PLUS"; break;
        case MINUS: lexem = "MINUS"; break;
        case MULTIPLY: lexem = "MULTIPLY"; break;
        case DIVISION: lexem = "DIVISION"; break;
        case LESS: lexem = "LESS"; break;
        case MORE: lexem = "MORE"; break;
        case LESSEQ: lexem = "LESSEQ"; break;
        case MOREEQ: lexem = "MOREEQ"; break;
        case EQ: lexem = "EQ"; break;
        case ADDITION: lexem = "ADDITION"; break;
        case NOTEQ: lexem = "NOTEQ"; break;
        case DEF: lexem = "DEF"; break;
        case DO: lexem = "DO"; break;
        case ELSE: lexem = "ELSE"; break;
        case END: lexem = "END"; break;
        case IF: lexem = "IF"; break;
        case NOT: lexem = "NOT"; break;
        case THEN: lexem = "THEN"; break;
        case WHILE: lexem = "WHILE"; break;
        case COMA: lexem = "COMA"; break;
        case NEXT: lexem = "NEXT"; break;
        default: lexem = malloc(sizeof(char) * 3);
                 sprintf(lexem, "%d", token->lexem); 
                 break;

    }

    if(token->name)
        name = token->name;
    else
        name = "NULL";

    printf("Lexem: %s, Name: %s", lexem, name);
}

/* Rule of LL gramar for Arguments of function calling.
 */
static bool ParserArguments() {
    //printf("Functioncall arguments\n");
    bool flag = true; // set to true if expects Identificator, false if expects comma

    /* Asks for tokens (arguments), until it sees right bracket */
    NEXTTOKEN;
    while (token->lexem != RIGHT_B) {
        if (flag && ( // argument
            token->lexem != IDENT &&
            token->lexem != STR &&
            token->lexem != INT &&
            token->lexem != NIL &&
            token->lexem != FLOAT )
            ) {
            printf("ERROR\n");
            goto end;
        } else if (!flag && token->lexem != COMA) { // comma
            printf("ERROR\n");
            goto end;
        }

        /* Semantic Action */
        if (flag) {
            printf("SEMCALL: Function call parameter:");
            AuxPrintToken(token);
            printf("\n");
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
    printf("SEMCALL: Statement\n");
    //TODO SEMCALL

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
            break;
        case ELSE:
            //printf("Else\n");
            break;
        case END:
            //printf("End\n");
            break;
        default:
            return false;
    }

    return true;
}

/* Rule of LL gramar for If statement.
 */
static bool ParserFunctionDeclaration() {
    //printf("Function\n");
    bool flag = true; // set to true if expects identificator, false if expects comma

    /* Expects name of function */
    NEXTTOKEN;
    if (token->lexem != IDENT) {
        printf("ERROR\n");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    char *name = malloc(sizeof(char) * (strlen(token->name) + 1));
    strcpy(name, token->name);

    // Add declaration to symtable
    if (!SymTableFind(currentTable, name)) {
        SymbolPtr symbol = malloc(sizeof(struct Symbol));
        if (!symbol) {
            printf("ERROR: malloc of symbol\n");
            return false;
        }
        symbol->name = name;
        symbol->iType = FUNCTION;
        SymTableAdd(currentTable, symbol);
    } else {
        free(name);
    }

    printf("SEMCALL: Function definition, name: %s\n", token->name);

    /* Expects left bracked after function name */
    NEXTTOKEN;
    if (token->lexem != LEFT_B) {
        printf("ERROR\n");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }


    /* Change Symbol Table for function's block of code and its parameters */
    currentTable = SymTableInit(globalTable);
    if (!currentTable) {
        printf("ERROR\n");
        return NULL;
    }
    codeDepth++;

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

        char *name = malloc(sizeof(char) * (strlen(token->name) + 1));
        strcpy(name, token->name);

        // Add declaration to symtable
        if (!SymTableFind(currentTable, name)) {
            SymbolPtr symbol = malloc(sizeof(struct Symbol));
            if (!symbol)
                printf("ERROR: malloc of symbol\n");
            symbol->name = name;
            symbol->iType = VARIABLE;
            SymTableAdd(currentTable, symbol);
        } else {
            free(name);
        }

        /* Semantic Action */
        if (flag) {
            printf("SEMCALL: Function definition parameter:");
            AuxPrintToken(token);
            printf("\n");
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

    SymTableDestroy(currentTable);
    currentTable = globalTable;
    codeDepth--;

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
    //printf("If\n");

    /* Semantic Action */
    printf("SEMCALL: IF\n");
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

        /* Semantic Action */
        printf("SEMCALL: ELSE\n");

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
    //printf("While\n");

    /* Semantic Action */
    printf("SEMCALL: WHILE\n");
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
    //printf("Declaration\n");

    char *name = malloc(sizeof(char) * (strlen(token->name) + 1));
    strcpy(name, token->name);

    NEXTTOKEN;
    switch (token->lexem) {
        case ADDITION: // It's declaration of variable
            /* Semantic Action */
            printf("SEMCALL: Variable Assignment, variable name: %s\n", name);
            // Add declaration to symtable
            if (!SymTableFind(currentTable, name)) {
                SymbolPtr symbol = malloc(sizeof(struct Symbol));
                if (!symbol) {
                    printf("ERROR: malloc of symbol\n");
                    return false;
                }
                symbol->name = name;
                symbol->iType = VARIABLE;
                SymTableAdd(currentTable, symbol);
            } else {
                free(name);
            }
            FUNCTIONCALL(ParserExpression);
            break;
        case LEFT_B: // It's function call
            /* Semantic Action */
            printf("SEMCALL: Function call, function name: %s\n", name);
            free(name);
            FUNCTIONCALL(ParserArguments);
            break;
        default:
            return false;
    }

    return true;
}

static bool ParserExpressionCheckError(bool flag) {
        if (flag &&
            token->lexem != LEFT_B && // can expect left bracket instead of operand
            token->lexem != IDENT &&
            token->lexem != STR &&
            token->lexem != INT &&
            token->lexem != FLOAT &&
            token->lexem != NIL) {

            printf("ERROR\n");
            while (token->lexem != EOL &&
                   token->lexem != DO &&
                   token->lexem != THEN &&
                   token->lexem != EOFILE)
                NEXTTOKEN;

            return false;
        } else if (!flag &&
                    token->lexem != RIGHT_B && // can expect left bracket instead of operand
                    token->lexem != PLUS &&
                    token->lexem != MINUS &&
                    token->lexem != MULTIPLY &&
                    token->lexem != DIVISION &&
                    token->lexem != LESS &&
                    token->lexem != MORE &&
                    token->lexem != LESSEQ &&
                    token->lexem != MOREEQ &&
                    token->lexem != EQ &&
                    token->lexem != NOTEQ) {

            printf("ERROR\n");
            while (token->lexem != EOL &&
                   token->lexem != DO &&
                   token->lexem != THEN &&
                   token->lexem != EOFILE)
                NEXTTOKEN;

            return false;
        }

        return true;
}

static bool ParserExpressionIsOperator(TokenPtr token) {
    if (token->lexem == PLUS ||
        token->lexem == MINUS ||
        token->lexem == MULTIPLY ||
        token->lexem == DIVISION ||
        token->lexem == LESS ||
        token->lexem == MORE ||
        token->lexem == LESSEQ ||
        token->lexem == MOREEQ ||
        token->lexem == EQ ||
        token->lexem == NOTEQ)
        return true;
    else
        return false;
}

static bool ParserExpressionIsGreater(TokenPtr token1, TokenPtr token2) {
    if ((token1->lexem == MULTIPLY || token1->lexem == DIVISION) &&
        (token2->lexem != MULTIPLY || token2->lexem != DIVISION))
        return true;
    else
        return false;
}

static StackPtr ParserExpressionRevInfixToPrefix(StackPtr RevInfixStack) {
    StackPtr stackTmp = StackCreate();
    StackPtr prefixStack = StackCreate();

    TokenPtr token;

    while ((token = StackPop(RevInfixStack)) != NULL) {
        if (token->lexem == RIGHT_B) {
            StackPush(stackTmp, token);
        } else if (token->lexem == LEFT_B) {
            TokenPtr tokenTmp = StackPop(stackTmp);
            while (tokenTmp->lexem != RIGHT_B) {
                StackPush(prefixStack, tokenTmp);
                tokenTmp = StackPop(stackTmp);
            }
        } else if (ParserExpressionIsOperator(token)) {
            TokenPtr tokenTmp = StackPop(stackTmp);
            while (tokenTmp) {
                if (ParserExpressionIsGreater(tokenTmp, token)) {
                    StackPush(prefixStack, tokenTmp);
                    tokenTmp = StackPop(stackTmp);
                } else {
                    StackPush(stackTmp, tokenTmp);
                    break;
                }
            }
            StackPush(stackTmp, token);
        } else {
            StackPush(prefixStack, token);
        }
    }

    while ((token = StackPop(stackTmp)) != NULL)
        StackPush(prefixStack, token);

    return prefixStack;
}

/* Rule of LL gramar for expression.
 * This expression will handled by precedence parsing.
 */
static bool ParserExpression() {
    //printf("Expression\n");

    bool flag = true;

    StackPtr stack = StackCreate();

    /* Reads operators and operadns of expression */
    NEXTTOKEN;
    while (token->lexem != EOL &&
           token->lexem != DO &&
           token->lexem != THEN &&
           token->lexem != EOFILE) {
        if (!ParserExpressionCheckError(flag))
            return false;

        // push it to stack, so we get reversed expression
        if(!StackPush(stack, token))
           printf("ERROR: StackPush() Failed\n");

        // to secure switching of operator and operand
        if (token->lexem != LEFT_B && token->lexem != RIGHT_B)
            flag = !flag;
        NEXTTOKEN;
    }

    // last token in expression is operator
    if (flag == true)
        printf("ERROR\n");
    else {
        StackPtr infixStack = ParserExpressionRevInfixToPrefix(stack);
        TokenPtr tokenToPrint;

        while ((tokenToPrint = StackPop(infixStack)) != NULL) {
            /* Semantic Action */
            printf("SEMCALL: Expression token:");
            AuxPrintToken(tokenToPrint);
            printf("\n");
        }

    }

    free(stack);

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
    currentTable = SymTableInit(NULL);
    if (!currentTable) {
        printf("ERROR\n");
        return NULL;
    }
    globalTable = currentTable;

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
    SymTableDestroy(currentTable);

    return tree;
}

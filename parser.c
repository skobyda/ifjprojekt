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
#include "main.h"
#include "parser.h"
#include "parser-semantic.h"
#include "scanner.h"
#include "stack.h"
#include "symtable.h"

typedef struct {
    int expressionType; // 0 - NOTHING, 1 - statement, 2 - condition
    int blockOfCodeType; // 0 - global, 1 - definition of function
    int blockOfStatements; // 0 - global, 1 - definition of function
} ParserInfo;


ParserInfo pinfo;
/* Always point to current token */
TokenPtr token = NULL;
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
    function(); \
} while(0)

#define NEXTTOKEN do { \
    if (token) \
        free (token->name); \
    free(token); \
    token = ScannerGetToken(); \
    if (!token) \
        return false; \
} while(0)

static TokenPtr CopyToken(TokenPtr token) {
    TokenPtr newtoken = malloc(sizeof(Token));
    if (!newtoken)
        return NULL;

    newtoken->lexem = token->lexem;
    newtoken->line = token->line;
    if (token->name) {
        newtoken->name = malloc(sizeof(char) * (strlen(token->name) + 1));
        strcpy(newtoken->name, token->name);
    } else {
        token->name = NULL;
    }

    return newtoken;
}

/* Rule of LL gramar for Arguments of function calling.
 */
static bool ParserArguments() {
    bool flag = true; // set to true if expects Identificator, false if expects comma

    /* Generator Order of Argunemt in Function */
    int order = 0;

    /* Asks for tokens (arguments), until it sees right bracket */
    while (token->lexem != RIGHT_B && token->lexem != EOL) {
        if (flag && ( // argument
            token->lexem != IDENT &&
            token->lexem != STR &&
            token->lexem != INT &&
            token->lexem != NIL &&
            token->lexem != FLOAT )
            ) {
            PrintError(2 , token->line, "Expected identifier or constant");
            goto end;
        } else if (!flag && token->lexem != COMA) { // comma
            PrintError(2 , token->line, "Expected comma");
            goto end;
        }

        /* Semantic Action */
        if (flag)
            SemanticFunParamControl(currentTable, token);


        /* Generator Function Parameter Call */
	if (token->lexem == STR ||
            token->lexem == FLOAT ||
            token->lexem == INT ||
            token->lexem == IDENT){
	    GeneratorParameterOut(order, token->name, token->lexem);
	    order++;
        }
        // to secure switching of argument/comma
        flag = !flag;

        NEXTTOKEN;
    }

    /* Generator Function Call */
    GeneratorStackPrint(StackG);
    char *code = PopStack(StackAssign);
    if (code){
        GeneratorRetValAssign(code);
	free(code);
    }   

    if (token->lexem == RIGHT_B) {
        /* If arguments of function call are inside brackets, expects end of line after right bracket */
        NEXTTOKEN;
        if (token->lexem != EOL) {
            PrintError(2 , token->line, "Expected right bracket");
            goto end;
        }
    }

 end:
    while (token->lexem != EOL)
        NEXTTOKEN;

    SemanticNoMoreParam(token);
    /* Statement on another line */
    FUNCTIONCALL(ParserStatement);

    return true;
}

/* Rule of LL gramar for statement.
 */
static bool ParserStatement() {
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
            break;
        case END:
            break;
        default:
            PrintError(2 , token->line, "Unknown statement");
            while (token->lexem != EOL)
                NEXTTOKEN;
            FUNCTIONCALL(ParserStatement);
    }

    return true;
}

/* Rule of LL gramar for If statement.
 */
static bool ParserFunctionDeclaration() {
    bool flag = true; // set to true if expects identificator, false if expects comma
    bool alreadyDefined = false;

    if (pinfo.blockOfCodeType != 0 || pinfo.blockOfStatements != 0)
        PrintError(2 , token->line, "Cannot use a nested function definition (definition inside function or if/while)");

    /* Expects name of function */
    NEXTTOKEN;
    if (token->lexem != IDENT) {
        PrintError(2 , token->line, "Expected identifier (name of function)");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    char *name = malloc(sizeof(char) * (strlen(token->name) + 1));
    strcpy(name, token->name);

    bool test = SemanticFunNameDefControl(token, name);

    SymbolPtr functionSymbol;
    // Add declaration to symtable
    if (test) {
        functionSymbol = malloc(sizeof(struct Symbol));
        if (!functionSymbol) {
            PrintError(99 , 0, "Could not malloc");
            return false;
        }
        functionSymbol->name = name;
        functionSymbol->nextSymbol = NULL;
        functionSymbol->numOfParameters = 0;
        functionSymbol->iType = FUNCTION;
        SymTableAdd(currentTable, functionSymbol);
    } else {
        free(name);
        alreadyDefined = true;
    }
    
    /* Generator Function Definition */
    GeneratorFunctionDefinition(token->name);

    /* Change Symbol Table for function's block of code and its parameters */
    currentTable = SymTableInit(globalTable);
    if (!currentTable) {
        PrintError(99 , 0, "Could not malloc");
        return NULL;
    }

    //TODO FUNCTIONS DOESNT NEED TO HAVE ANY ARGUMENTS
    /* Expects left bracked after function name */
    NEXTTOKEN;
    if (token->lexem != LEFT_B) {
        PrintError(2 , token->line, "Expected left bracked after function name");
        while (token->lexem != EOL)
            NEXTTOKEN;
    } else {

        /* Generator - Parameter for function GeneratorParameterIn - parameter order */
        int order = 0;

        /* Reads parameters of function until it sees right bracket */
        NEXTTOKEN;
        while (token->lexem != RIGHT_B) {
            if (flag && token->lexem != IDENT) {
                PrintError(2 , token->line, "Expected function parameter (identifier)");
                while (token->lexem != EOL)
                    NEXTTOKEN;
            } else if (!flag && token->lexem != COMA) {
                PrintError(2 , token->line, "Expected comma");
                while (token->lexem != EOL)
                    NEXTTOKEN;
            }

            char *name = malloc(sizeof(char) * (strlen(token->name) + 1));
            strcpy(name, token->name);

            // Add declaration to symtable
            if (!SymTableFind(currentTable, name)) {
                SymbolPtr symbol = malloc(sizeof(struct Symbol));
                if (!symbol)
                    PrintError(99 , 0, "Could not malloc");
                symbol->name = name;
                symbol->nextSymbol = NULL;
                symbol->iType = VARIABLE;
                symbol->dType = typeUnknown;
                SymTableAdd(currentTable, symbol);
            } else {
                free(name);
            }
    
            if (!alreadyDefined && flag)
                functionSymbol->numOfParameters++;

            if (flag) {
                /* Generator Parameter In Function */
                GeneratorParameterIn(order, token->name);
                order++;
            }

            // to secure switching of argument/comma
            flag = !flag;
            NEXTTOKEN;
        }


        if(token->lexem == INT ||
           token->lexem == STR ||
           token->lexem == FLOAT ||
           token->lexem == NIL ||
           token->lexem == IDENT ){
            GeneratorParameterIn(order, token->name);
            order++;
        }

        NEXTTOKEN;
        /* Expects end of line after right bracket */
        if (token->lexem != EOL) {
            PrintError(2 , token->line, "Expected end of line after right bracket");
            while (token->lexem != EOL)
                NEXTTOKEN;
        }
    }

    /* Statement(s) inside a function (aka body of function) */
    pinfo.blockOfCodeType = 1;
    FUNCTIONCALL(ParserStatement);
    pinfo.blockOfCodeType = 0;

    SymTableDestroy(currentTable);
    currentTable = globalTable;

    /* Body of function should end with 'END' */
    if (token->lexem != END)
        PrintError(2 , token->line, "Expected 'end' after function's definition");

    /* Generator Action Function End */
    GeneratorFunctionEnd();

    /* Expects end of line after 'END' */
    NEXTTOKEN;
    if (token->lexem != EOL) {
        PrintError(2 , token->line, "Expected end of line after 'end'");
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
    /* Generator If Statement start */
    GeneratorIf();

    /* Function's condition can be parsed as expression */
    pinfo.expressionType = 2;
    FUNCTIONCALL(ParserExpression);
    pinfo.expressionType = 0;

    /* Generator Condition Evaluation */
    GeneratorStackPrint(StackIf);
    
    /* Expects 'end' after function's condition */
    if (token->lexem != THEN)
        PrintError(2 , token->line, "Expected 'then' after condition");

    /* Expects end of line after 'end' */
    NEXTTOKEN;
    if (token->lexem != EOL) {
        PrintError(2 , token->line, "Expected end of line after 'end'");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* Function's block of code */
    pinfo.blockOfStatements = 1;
    FUNCTIONCALL(ParserStatement);
    pinfo.blockOfStatements = 0;

    /* ELSE (optional) */
    if (token->lexem == ELSE) {
        /* Expects end of line after 'else' */
        NEXTTOKEN;
   

        if (token->lexem != EOL) {
            PrintError(2 , token->line, "Expected end of line after 'else'");
            while (token->lexem != EOL)
                NEXTTOKEN;
        }

        /* Generator Else Statement */
        GeneratorStackPrint(StackIf);
        GeneratorStackPrint(StackIf);

        /* Else's block of code */
        pinfo.blockOfStatements = 1;
        FUNCTIONCALL(ParserStatement);
        pinfo.blockOfStatements = 0;
    } else {
        GeneratorStackPrint(StackIf);
        GeneratorStackPrint(StackIf);	
    }

    /* Expects 'end' at the end of if/else statement  */
    if (token->lexem != END)
        PrintError(2 , token->line, "Expected 'end' after if/else's block of code");

    /* Generator End If Label */
    GeneratorStackPrint(StackIf);    

    /* Expects end of line after 'end' */
    NEXTTOKEN;
    if (token->lexem != EOL) {
        PrintError(2 , token->line, "Expected end of line after 'end'");
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
    /* Generator While Start */
    WhileEnd++;
    GeneratorWhile();
    
    /* While's condition, can be parsed as Expression */
    pinfo.expressionType = 1;
    FUNCTIONCALL(ParserExpression);
    pinfo.expressionType = 0;

    /* Generator While Condition Evaluation */
    GeneratorStackPrint(StackWhile);

    /* Expects 'do' after while's condition */
    if (token->lexem != DO)
        PrintError(2 , token->line, "Expected 'do' after condition");

    /* Expects end of line after 'do' */
    NEXTTOKEN;
    if (token->lexem != EOL) {
        PrintError(2 , token->line, "Expected end ofline after 'do'");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* While's block of code */
    pinfo.blockOfStatements = 2;
    FUNCTIONCALL(ParserStatement);
    pinfo.blockOfStatements = 0;

    /* Expects 'end' at the end of while statement */
    if (token->lexem != END)
        PrintError(2 , token->line, "Expected 'end'");

    /* Generator While End */
    GeneratorStackPrint(StackWhile);
    GeneratorStackPrint(StackWhile);
    WhileEnd--;
    if (WhileEnd == 0){
       while (!GenEmptyStack(StackWhileAll)){
            char *code = NULL;
            code = FrontStack(StackWhileAll);
            fprintf(stdout,"%s\n",code);
            free(code);         
        }
    }
    NEXTTOKEN;
    /* Expects end of line after 'end' */
    if (token->lexem != EOL) {
        PrintError(2 , token->line, "Expected end of line 'end'");
        while (token->lexem != EOL)
            NEXTTOKEN;
    }

    /* Rest of the program */
    FUNCTIONCALL(ParserStatement);

    return true;
}

/* Rule of LL gramar for declaration and definiton of
 * variables or function call
 */
static bool ParserDeclaration() {
    SymbolPtr symbol;
    bool defined, test;
    char *name = malloc(sizeof(char) * (strlen(token->name) + 1));
    strcpy(name, token->name);

    NEXTTOKEN;
    switch (token->lexem) {
        case ADDITION: // It's declaration of variable
            test = SemanticVarNameAssignControl(token, name);
            // Add declaration to symtable
            if (!SymTableFind(currentTable, name) && !SymTableFind(globalTable, name) && test) {
                // not yet defined GENERATOR
                
                /* Generator Assignment */
                defined = false;
                GeneratorAssign(name, defined);
                
                SymbolPtr symbol = malloc(sizeof(struct Symbol));
                if (!symbol) {
                    PrintError(99 , token->line, "Can't malloc");
                    return false;
                }
                symbol->name = name;
                symbol->nextSymbol = NULL;
                symbol->iType = VARIABLE;
                symbol->dType = typeNil;
                SymTableAdd(currentTable, symbol);
            } else {
                // defined GENERATOR
                
                /* Generator Assignment */
                defined = true;
                GeneratorAssign(name, defined);
                
                free(name);
            }


            pinfo.expressionType = 1;
            printf("HELLLO\n");
            FUNCTIONCALL(ParserExpression);
            pinfo.expressionType = 0;
            
            /* Generator Assignment */
            GeneratorAssignPrint();
 
            break;
        case LEFT_B: // It's function call
        case IDENT:
        case STR:
        case INT:
        case FLOAT:
        case NIL:
            if (token->lexem == LEFT_B)
                NEXTTOKEN;
            SemanticFunNameCallControl(currentTable, token, name,pinfo.blockOfCodeType);

            /* Generator Action Function Call */
            GeneratorFunctionCall(name);
            free(name);

            FUNCTIONCALL(ParserArguments);
            break;
        case EOL:
            symbol = SymTableFind(currentTable, name);
            if (!symbol) {
                SemanticFunNameCallControl(currentTable, token, name,pinfo.blockOfCodeType);
		
                //PrintError(2 , token->line, "Expected end of line 'end'");
            } else {
                if (symbol->iType == FUNCTION) { //fuction call with no parameters
                    SemanticFunNameCallControl(currentTable, token, name,pinfo.blockOfCodeType);
                    SemanticNoParamControl(token);
                    /* Generator Function without arguments CALL */
                    GeneratorFunctionCall(name);
                    GeneratorStackPrint(StackG);
                } else {
                    GeneratorRetValInFunction(name);
                }
                //could also be variable assigned nowhere. In that case, does nothing
            }
            FUNCTIONCALL(ParserStatement);
            free(name);
            break;
        default:
            PrintError(2 , token->line, "Unknown token");
            while (token->lexem != EOL)
                NEXTTOKEN;
            FUNCTIONCALL(ParserStatement);
            free(name);
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

            PrintError(2 , token->line, "Expected operand or left bracket in the expression");
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

            PrintError(2 , token->line, "Expected operator or right bracket int the expression");
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
            if (tokenTmp) {
                free(tokenTmp->name);
                free(tokenTmp);
            }
            free(token->name);
            free(token);
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

    StackDestroy(stackTmp);

    return prefixStack;
}

/* Rule of LL gramar for expression.
 * This expression will handled by precedence parsing.
 */
static bool ParserExpression() {
    bool flag = true;

    /* Reads operators and operadns of expression */
    NEXTTOKEN;

    SymbolPtr symbol = SymTableFind(currentTable, token->name);
    if (symbol && symbol->iType == FUNCTION) {
            SemanticFunNameCallControl(currentTable, token,token->name,pinfo.blockOfCodeType);
            /* Generator function call */
            GeneratorFunctionCall(token->name);
            NEXTTOKEN;
            switch (token->lexem) {
                case LEFT_B: // It's function call
                case IDENT:
                case STR:
                case INT:
                case FLOAT:
                case NIL:
                    if (token->lexem == LEFT_B)
                        NEXTTOKEN;
                    FUNCTIONCALL(ParserArguments);
                    break;
                case EOL:
                    SemanticNoParamControl(token);
                    //could also be variable assigned nowhere. In that case, does nothing
                    break;
                default:
                    break; //TODO ERROR
            }

            goto next;
    }

    StackPtr stack = StackCreate();
    while (token->lexem != EOL &&
           token->lexem != DO &&
           token->lexem != THEN &&
           token->lexem != EOFILE) {
        if (!ParserExpressionCheckError(flag)) {
            return false;
        }

        TokenPtr newToken = CopyToken(token);

        // push it to stack, so we get reversed expression
        if(!StackPush(stack, newToken))
            PrintError(99, 0, "Internal error: Stack Push failed");

        // to secure switching of operator and operand
        if (token->lexem != LEFT_B && token->lexem != RIGHT_B)
            flag = !flag;
        NEXTTOKEN;
    }

    // last token in expression is operator
    if (flag == true)
        PrintError(2, token->line, "Operator cannot be last token in epxression");
    else {
        StackPtr infixStack = ParserExpressionRevInfixToPrefix(stack);
        TokenPtr tokenToPrint;
       
	ExL Ex = malloc(sizeof(struct ExprS)); 
        Ex->Last = NULL;
        Ex->First = NULL;
	Ex->count = 0;
	bool intOccur = false;
	bool floatOccur = false;
        while ((tokenToPrint = StackPop(infixStack)) != NULL) {
            if(pinfo.expressionType == 2) 
                SemanticFullCondControl(currentTable, tokenToPrint);
            else
                SemanticExprAssignCotrol (currentTable, tokenToPrint);

            /* Generator Action TODO */
            if (tokenToPrint->lexem == FLOAT) floatOccur = true;
            if (tokenToPrint->lexem == INT) intOccur = true;
            GeneratorAddExpression(Ex, tokenToPrint->name, tokenToPrint->lexem);        
           
            free(tokenToPrint->name);
            free(tokenToPrint);
        }
        SemanticExpAssignReset();
        StackDestroy(infixStack);
        /* Generate Expression in IFJcode2018 */
        GeneratorExpression(Ex, floatOccur, intOccur);
    }

    StackDestroy(stack);

    /* Generator Assignment */
    GeneratorAssignPrint();    

 next:
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
bool
Parser() {
    pinfo.expressionType = 0;
    pinfo.blockOfCodeType = 0;
    pinfo.blockOfStatements = 0;

    currentTable = SymTableInit(NULL);
    if (!currentTable) {
        PrintError(99, 0, "Operator cannot be last token in epxression");
        return true;
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
    Semantic2ndDefControl();

    //} while (token);

    if (token) \
        free (token->name); \
    free(token);
    SymTableDestroy(currentTable);

    return false;
}

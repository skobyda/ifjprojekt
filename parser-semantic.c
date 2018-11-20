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
 *  FILE AUTHOR: Denis Filo(xfilod00)
 */

/***SYSTEM FILES***/
#include <stdbool.h>
#include <stdlib.h>

/***LOCAL FILES***/
#include "scanner.h"
#include "parser.h"
#include "parser-semantic.h"
#include "generator.h"
#include "symtable.h"

/* Runs Semantic analysis over derivation tree.
 * throws some error if semantic bug was detected.
 * @tree Pointer to derivation tree.
 * Returns: true in case of success, false in case of error
 */

/*bool SemanticDefinedControl(SymTablePtr currTable, char *name){
    
    bool defined = FALSE;
    
    SymbolPtr symbol = SymTableFind(currTable, name);
*/
void SemanticTreeInit (ATreeNodePtr *RootPtr) {
    *RootPtr = NULL;
}

bool ParserSemantic(ParTreePtr tree) {
    (void)(tree); //DELETE THIS LINE

    //TODO

    return NULL;
}

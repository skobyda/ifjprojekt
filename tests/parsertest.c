/* IFJ Project 2018/19
 * Test of Parser
 *
 * COMPILE:
 * gcc -pedantic -Wall -Wextra -parsertest.c -o parsertest
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: parsertest.h
 *  FILE AUTHOR: Simon Kobyda
 */

/***SYSTEM FILES***/
#include <stdio.h>

/***LOCAL FILES***/
#include "../scanner.h"
#include "../parser.h"

/********** EXPECTED INPUT for TEST CASE 1: **********/

/* Had to declare it as global variables, so each time Parser() calls DummyScanner()
 * expected input variables are already initialized. */
#define EXPECTEDINPUT1COUNT 5
Token expectedInput1[5] = {
    {ID, 1, "x"},
    {ADDITION, 1, NULL},
    {ID, 1, "a"},
    {PLUS, 1, NULL},
    {ID, 1, "b"},
};

/* Keeps track of which lexem is next */
int nextLexem = 0;

/* Keeps track of which Test Case # is run */
int currentTestCase;


/********** EXPECTED OUTPUT for TEST CASE 1: **********/
ParTreePtr
ParserTestCase1GetOutput() {

    /* Parser (derivation) tree */
    ParTreePtr expectedOutput = malloc(sizeof(ParTree));

    expectedOutput->token = NULL;
    expectedOutput->left = malloc(sizeof(ParTree));
    expectedOutput->right = malloc(sizeof(ParTree));
    expectedOutput->operation = ADDITION;

    expectedOutput->left->token = &expectedInput1[0];

    expectedOutput->right->token = NULL;
    expectedOutput->right->left = malloc(sizeof(ParTree));
    expectedOutput->right->right = malloc(sizeof(ParTree));
    expectedOutput->operation = PLUS;

    expectedOutput->right->left->token = &expectedInput1[2];
    expectedOutput->right->right->token = &expectedInput1[4];

    return expectedOutput;
}

/* until scanner part of compiler is not finished, parser will use dummyscanner()
 * which provides parser with hard-coded artificial input */
TokenPtr DummyScannerGetToken() {
    return &expectedInput1[nextLexem++];
}

bool
TestsCompareParTrees(ParTreePtr actualOutput,
                     ParTreePtr expectedOutput)
{
    bool ret = true;

    if (!actualOutput) {
            printf("actualOutput is null!\n");
            return false;
    }
    if (!expectedOutput) {
            printf("expectedOutput is null!\n");
            return false;
    }

    if (actualOutput->token || expectedOutput->token) {
        if (actualOutput->token) {
            printf("ACTTOKEN - lexem:%d,name:%s",
                   actualOutput->token->lexem, actualOutput->token->name );
        } else {
            printf("ACTTOKEN - NULL");
            ret = false;
        }

        if (expectedOutput->token) {
            printf("EXPTOKEN - lexem:%d,name:%s",
                   expectedOutput->token->lexem, expectedOutput->token->name );
        } else {
            printf("EXPTOKEN - NULL");
            ret = false;
        }
    } else {
        if (!actualOutput->left) {
            printf("Left Node of ActualOutput Tree is missing!\n");
            return false;
        }
        if (!actualOutput->right) {
            printf("Right Node of ActualOutput Tree is missing!\n");
            return false;
        }
        if (!expectedOutput->left) {
            printf("Left Node of ExpectedOutput Tree is missing!\n");
            return false;
        }
        if (!expectedOutput->left) {
            printf("Right Node of ExpectedOutput Tree is missing!\n");
            return false;
        }

        ret = TestsCompareParTrees(actualOutput->left, expectedOutput->left);
        ret = TestsCompareParTrees(actualOutput->right, expectedOutput->right);
    }

    return ret;
}

bool
ParserTestCase1() {
    /* Expected output for this test is Parser (derivation) tree */
    ParTreePtr expectedOutput = ParserTestCase1GetOutput();

    /* Actual output returned by our implementation of Parser */
    ParTreePtr actualOutput = Parser();

    return TestsCompareParTrees(actualOutput, expectedOutput);
}

int main() {
    int failTestCount = 0;

    currentTestCase = 1;
    if (!ParserTestCase1())
        failTestCount++;

    printf("%d TEST(S) FAILED!\n", failTestCount);
}

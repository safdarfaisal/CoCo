#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper/definitions.h"

// Define maximum production length and count
#define MAXPRODLEN 8
#define MAXPRODCOUNT 100

// Parse tree node structure
typedef struct ParseTreeNode {
    boolean isTerminal;
    Symbols symbol;
    char lexeme[100];
    struct ParseTreeNode **children;
    int childCount;
} ParseTreeNode;

 
// Forward declarations
ParseTreeNode* program();
ParseTreeNode* mainFunction();
ParseTreeNode* otherFunctions();
ParseTreeNode* function();
ParseTreeNode* input_par();
ParseTreeNode* output_par();
ParseTreeNode* parameter_list();
ParseTreeNode* dataType();
ParseTreeNode* primitiveDatatype();
ParseTreeNode* constructedDatatype();
ParseTreeNode* remaining_list();
ParseTreeNode* stmts();
ParseTreeNode* typeDefinitions();
ParseTreeNode* isRedefined();
ParseTreeNode* typeDefinition();
ParseTreeNode* fieldDefinitions();
ParseTreeNode* fieldDefinition();
ParseTreeNode* moreFields();
ParseTreeNode* declarations();
ParseTreeNode* declaration();
ParseTreeNode* global_or_not();
ParseTreeNode* otherstmts();
ParseTreeNode* stmt();
ParseTreeNode* assignmentStmt();
ParseTreeNode* singleOrRecId();
ParseTreeNode* singleLeft();
ParseTreeNode* oneExpansion();
ParseTreeNode* moreExpansions();
ParseTreeNode* funCallStmt();
ParseTreeNode* outputParameters();
ParseTreeNode* inputParameters();
ParseTreeNode* iterativeStmt();
ParseTreeNode* conditionalStmt();
ParseTreeNode* elseStmt();
ParseTreeNode* ioStmt();
ParseTreeNode* arithmeticExpression();
ParseTreeNode* expPrime();
ParseTreeNode* term();
ParseTreeNode* termPrime();
ParseTreeNode* var();
ParseTreeNode* factor();
ParseTreeNode* highPrecedenceOps();
ParseTreeNode* lowPrecedenceOps();
ParseTreeNode* booleanExpression();
ParseTreeNode* logicalOp();
ParseTreeNode* relationalOp();
ParseTreeNode* returnStmt();
ParseTreeNode* optionalReturn();
ParseTreeNode* idList();
ParseTreeNode* moreIds();
ParseTreeNode* definetypestmt();
ParseTreeNode* a();



#endif
#ifndef ASTGEN_H
#define ASTGEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper/definitions.h"

// Define maximum production length and count
#define MAXPRODLEN 8
#define MAXPRODCOUNT 100

// Token structure
// typedef enum exprDtype{
//     TYPE_UNDEFINED,
//     TYPE_REAL,
//     TYPE_INT
// } exprDtype;



// Parse tree node structure
typedef struct AstTreeNode {
    Symbols symbol;
    char lexeme[100];
    boolean isTerminal;
    struct AstTreeNode *parent;
    struct AstTreeNode **children;
    int childCount;
} AstTreeNode;


// Forward declarations
AstTreeNode* program();
AstTreeNode* mainFunction();
AstTreeNode* otherFunctions(AstTreeNode *node);
AstTreeNode* function();
AstTreeNode* input_par();
AstTreeNode* output_par(AstTreeNode *node);
AstTreeNode* parameter_list(AstTreeNode *node);
AstTreeNode* dataType(AstTreeNode *node);
AstTreeNode* primitiveDatatype();
AstTreeNode* constructedDatatype(AstTreeNode *node);
AstTreeNode* remaining_list(AstTreeNode *node);
AstTreeNode* stmts();
AstTreeNode* typeDefinitions();
AstTreeNode* isRedefined();
AstTreeNode* typeDefinition();
AstTreeNode* fieldDefinitions();
AstTreeNode* fieldDefinition();
AstTreeNode* moreFields(AstTreeNode *node);
AstTreeNode* declarations(AstTreeNode *node);
AstTreeNode* declaration();
AstTreeNode* global_or_not();
AstTreeNode* otherstmts(AstTreeNode *node);
AstTreeNode* stmt(AstTreeNode *node);
AstTreeNode* assignmentStmt();
AstTreeNode* singleOrRecId();
AstTreeNode* singleLeft(AstTreeNode *node);
AstTreeNode* oneExpansion(AstTreeNode *node);
AstTreeNode* moreExpansions(AstTreeNode *node);
AstTreeNode* funCallStmt();
AstTreeNode* outputParameters();
AstTreeNode* inputParameters();
AstTreeNode* iterativeStmt();
AstTreeNode* conditionalStmt();
AstTreeNode* elseStmt();
AstTreeNode* ioStmt();
AstTreeNode* arithmeticExpression();
AstTreeNode* expPrime();
AstTreeNode* term();
AstTreeNode* termPrime();
AstTreeNode* var();
AstTreeNode* factor();
AstTreeNode* highPrecedenceOps();
AstTreeNode* lowPrecedenceOps();
AstTreeNode* booleanExpression();
AstTreeNode* logicalOp();
AstTreeNode* relationalOp();
AstTreeNode* returnStmt();
AstTreeNode* optionalReturn();
AstTreeNode* idList(AstTreeNode *node);
AstTreeNode* moreIds(AstTreeNode *node);
AstTreeNode* definetypestmt();
AstTreeNode* a();


AstTreeNode *astgen_run();


#endif
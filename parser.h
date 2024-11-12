#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper/definitions.h"
// #include "lexer.h"

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

const KeywordPair lex_keywords[27] = {
    {.keyword = "with", .enumVal= {.t = TK_WITH }},
    {.keyword =  "while", .enumVal= {.t = TK_WHILE }},
    {.keyword =  "parameters", .enumVal= {.t = TK_PARAMETERS }},
    {.keyword =  "end", .enumVal= {.t = TK_END }},
    {.keyword =  "union", .enumVal= {.t = TK_UNION }},
    {.keyword =  "endunion", .enumVal= {.t = TK_ENDUNION }},
    {.keyword =  "definetype", .enumVal= {.t = TK_DEFINETYPE }},
    {.keyword =  "as", .enumVal= {.t = TK_AS }},
    {.keyword =  "type", .enumVal= {.t = TK_TYPE }},
    {.keyword =  "global", .enumVal= {.t = TK_GLOBAL }},
    {.keyword =  "parameter", .enumVal= {.t = TK_PARAMETER }},
    {.keyword =  "list", .enumVal= {.t = TK_LIST }},
    {.keyword =  "input", .enumVal= {.t = TK_INPUT }},
    {.keyword =  "output", .enumVal= {.t = TK_OUTPUT }},
    {.keyword =  "int", .enumVal= {.t = TK_INT }},
    {.keyword =  "real", .enumVal= {.t = TK_REAL }},
    {.keyword =  "endwhile", .enumVal= {.t = TK_ENDWHILE }},
    {.keyword =  "if", .enumVal= {.t = TK_IF }},
    {.keyword =  "then", .enumVal= {.t = TK_THEN }},
    {.keyword =  "endif", .enumVal= {.t = TK_ENDIF }},
    {.keyword =  "read", .enumVal= {.t = TK_READ }},
    {.keyword =  "write", .enumVal= {.t = TK_WRITE }},
    {.keyword =  "return", .enumVal= {.t = TK_RETURN }},
    {.keyword =  "call", .enumVal= {.t = TK_CALL }},
    {.keyword =  "record", .enumVal= {.t = TK_RECORD }},
    {.keyword =  "endrecord", .enumVal= {.t = TK_ENDRECORD }},
    {.keyword =  "else", .enumVal= {.t = TK_ELSE }}
};
 
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
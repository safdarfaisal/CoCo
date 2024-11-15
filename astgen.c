#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper/definitions.h"

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

// Define maximum production length and count
#define MAXPRODLEN 8
#define MAXPRODCOUNT 100

// Token structure
typedef enum exprDtype{
    TYPE_UNDEFINED,
    TYPE_REAL,
    TYPE_INT
} exprDtype;

// Parse tree node structure
typedef struct AstTreeNode {
    Symbols symbol;
    char lexeme[100];
    boolean isTerminal;
    exprDtype type;
    struct AstTreeNode *parent;
    struct AstTreeNode **children;
    int childCount;
} AstTreeNode;

// Function to create a new parse tree node
AstTreeNode* createNode(Symbols symbol, const char *lexeme, int isTerminal) {
    AstTreeNode* node = (AstTreeNode*)malloc(sizeof(AstTreeNode));
    node->symbol = symbol;
    node->parent=NULL;
    strcpy(node->lexeme, lexeme);
    node->type = TYPE_UNDEFINED;
    node->children = NULL;
    node->childCount = 0;
    return node;
}

// Function to add a child to a parse tree node
void addChild(AstTreeNode *parent, AstTreeNode *child) {
    parent->children = (AstTreeNode**)realloc(parent->children, 
        (parent->childCount + 1) * sizeof(AstTreeNode*));
    parent->children[parent->childCount] = child;
    parent->childCount++;
    if(!child){
        return;
    }
    child->parent = parent;  
}

void traverse_and_apply(AstTreeNode *node, void (* apply)(AstTreeNode *)){
    if(!node){
        return;
    } 
    for (int i = 0; i < node->childCount; i++){
        traverse_and_apply(node->children[i], apply);
    }

    
}

AstTreeNode *getParent(AstTreeNode *node){
    if(!node){
        printf("NULL node passed\n");
        return NULL;
    }
    if(!(node->parent)){
        printf("Root node passed, returning root node");
        return node;
    }
    return node->parent;
}

// AstTreeNode *getNextChild(AstTreeNode *node){
//     if(!node){
//         return NULL;
//     }
//     if(node->currChild == node->childCount){
//         printf("All children have been met.");
//         return NULL;
//     }
//     return node->children[(node->currChild)++];
// }

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

// Token stream and current token index
Token lexedTokens[5000];
int listSize = 0;
int currentTokenIndex = 0;
AstTreeNode *parseTree;

Token initToken(){
    Token token = (Token )malloc(sizeof(struct Token));
    memset(token, 0, sizeof(struct Token));
    return token;
}

void setValue(Token token, char *value, int size){
    memset(token->value, 0, IDLENGTH*sizeof(char));
    memcpy(token->value, value, size*sizeof(char));
}

void setLineNum(Token token, int lineno){
    token -> lineno = lineno;
}

void setTerminalType(Token token, Terminal term){
    token->terminal = term;
}

Terminal getKeywordTerminal(char *string){
    // lex_keywords is an array of Keyword - enum mappings
    for(int i = 0; i < 25; i++){
        if(!strcmp(lex_keywords[i].keyword, string)){
            return lex_keywords[i].enumVal.t;
        }
    }
    return TK_ERROR;
}

Token createToken(char *value, int lineno, Terminal term){
    Token token = initToken();
    setValue(token, value, strlen(value));
    setLineNum(token, lineno);
    setTerminalType(token, term);
    return token;
}


void loadTokensFromFile(char *filePath){
    // CSV format -> value, line #, terminal enum
    const int maxlen = 100;
    char row[maxlen];
    FILE *tokenfp = fopen(filePath, "r");
    if(!tokenfp){
        printf("File open failed in load token\n");
        return;
    }
    while(fgets(row, maxlen, tokenfp)){
        //read row
        printf("%s", row);
        // tokenize row with , as delimiter
        char delimiter[] = ",";
        // char *val = strtok(row, delimiter);
        // if(val){}
        char *tokenValue = strtok(row, delimiter);
        if(!tokenValue){
            printf("Error Parsing token value\n");
            break;
        }
        // printf("A %s\n", tokenValue);
        char *lineStr = strtok(NULL, delimiter);
        // int lineNo;
        if(!lineStr){
            printf("Error Parsing line number\n");
            break;
        }
        // printf("B %s\n", lineStr);
        int lineNo = atoi(lineStr);
        char* terminalStr =  strtok(NULL, delimiter);
        // printf("C %s\n", terminalStr);
        if(!terminalStr){
            printf("Error Parsing terminal number\n");
            break;
        }
        int terminal = atoi(terminalStr);
        lexedTokens[listSize++] = createToken(tokenValue, lineNo, terminal);
        if(terminal == TK_EOF){
            printf("Reached EOF\n");
        }
        // printf("A %s, B %d, C %d\n", tokenValue, lineNo, terminal);
    }
    fclose(tokenfp);
}


Token getCurrentToken() {
    return lexedTokens[currentTokenIndex];
}

void advanceToken() {
    if (currentTokenIndex < listSize) {
        currentTokenIndex++;
    }
}

AstTreeNode* match(Terminal expected, boolean necessary) {
    Token currentToken = getCurrentToken();
    AstTreeNode* node;
    if (currentToken->terminal == expected) {
        if(necessary){ 
            Symbols symbol;
            symbol.t = expected;
            node = createNode(symbol, currentToken->value, 1);
            advanceToken();
        } else {
            node = NULL;
            advanceToken();
        }
        // advanceToken();
        return node;
    } else {
        printf("Syntax error: Expected %d but found %d\n", expected, currentToken->terminal);
        exit(1);
    }
}

void match_f(Terminal expected){
    match(expected, FALSE);
}

// Function implementations
AstTreeNode* program() {
    Symbols symbol;
    symbol.nt = nt_program;
    AstTreeNode *programNode = createNode(symbol, "program", 1);
    otherFunctions(programNode);
    addChild(programNode, mainFunction());
    return programNode;
}

AstTreeNode* mainFunction() {
    Symbols symbol;
    symbol.nt = nt_mainFunction;
    AstTreeNode *mainFuncNode = createNode(symbol, "mainFunction", 1);
    match_f(TK_MAIN);
    addChild(mainFuncNode, stmts());
    match_f(TK_END);
    return mainFuncNode;
}

AstTreeNode* otherFunctions(AstTreeNode* programNode) {
    Symbols symbol;
    symbol.nt = nt_otherFunctions;
    AstTreeNode *node = createNode(symbol, "otherFunctions", 0);

    if (getCurrentToken()->terminal == TK_FUNID) {
        addChild(programNode, function());
        otherFunctions(programNode);
    } else {
        addChild(programNode, NULL);
    }
    return programNode;
}

AstTreeNode* function() {
    Symbols symbol;
    symbol.nt = nt_function;
    AstTreeNode *functionNode = createNode(symbol, "function", 1);
    addChild(functionNode, match(TK_FUNID, TRUE));
    addChild(functionNode, input_par());
    addChild(functionNode, output_par(functionNode));
    match(TK_SEM, FALSE);
    addChild(functionNode, stmts());
    match(TK_END, FALSE);
    return functionNode;
}

AstTreeNode* input_par() {
    Symbols symbol;
    symbol.nt = nt_input_par;
    AstTreeNode *inputParNode = createNode(symbol, "input parameters", 0);
    match(TK_INPUT, FALSE);
    match(TK_PARAMETER, FALSE);
    match(TK_LIST, FALSE);
    match(TK_SQL, FALSE);
    parameter_list(inputParNode);
    match(TK_SQR, FALSE);
    return inputParNode;
}

AstTreeNode* output_par(AstTreeNode *node) {
    Symbols symbol;
    symbol.nt = nt_output_par;
    AstTreeNode *outputParNode = createNode(symbol, "output parameters", 0);

    if (getCurrentToken()->terminal == TK_OUTPUT) {
        match(TK_OUTPUT, FALSE);
        match(TK_PARAMETER, FALSE);
        match(TK_LIST, FALSE);
        match(TK_SQL, FALSE);
        parameter_list(outputParNode);
        match(TK_SQR, FALSE);
        return outputParNode;
    } else {
        return NULL;
    }
}

AstTreeNode* parameter_list(AstTreeNode *parNode) {
    Symbols symbol;
    symbol.nt = nt_parameter_list;
    AstTreeNode *var = createNode(symbol, "variable", 0);
    addChild(var, dataType(var));
    addChild(var, match(TK_ID, TRUE));
    addChild(parNode, var);
    remaining_list(parNode);
    return parNode;
}

AstTreeNode* dataType(AstTreeNode *varNode) {
    Symbols symbol;
    symbol.nt = nt_dataType;
    AstTreeNode *dataTypeNode = createNode(symbol, "dataType", 0);

    if (getCurrentToken()->terminal == TK_INT || getCurrentToken()->terminal == TK_REAL) {
        return primitiveDatatype();
    } else if (getCurrentToken()->terminal == TK_RECORD || getCurrentToken()->terminal == TK_UNION || getCurrentToken()->terminal == TK_RUID) {
        return constructedDatatype(varNode);
    } else {
        printf("Syntax error in dataType\n");
        exit(1);
    }
}

AstTreeNode* primitiveDatatype() {
    Symbols symbol;
    symbol.nt = nt_primitiveDatatype;
    AstTreeNode *node = createNode(symbol, "primitiveDatatype", 0);

    if (getCurrentToken()->terminal == TK_INT) {
        return match(TK_INT, TRUE);
    } else if (getCurrentToken()->terminal == TK_REAL) {
        return match(TK_REAL, TRUE);
    } else {
        printf("Syntax error in primitiveDatatype\n");
        exit(1);
    }
}

AstTreeNode* constructedDatatype(AstTreeNode *varNode) {
    Symbols symbol;
    symbol.nt = nt_constructedDatatype;
    AstTreeNode *node = createNode(symbol, "constructedDatatype", 0);

    if (getCurrentToken()->terminal == TK_RECORD) {
        addChild(varNode, match(TK_RECORD, TRUE));
        addChild(varNode, match(TK_RUID, TRUE));
    } else if (getCurrentToken()->terminal == TK_UNION) {
        addChild(varNode, match(TK_UNION, TRUE));
        addChild(varNode, match(TK_RUID, TRUE));
    } else if (getCurrentToken()->terminal == TK_RUID) {
        addChild(varNode, match(TK_RUID, TRUE));
    } else {
        printf("Syntax error in constructedDatatype\n");
        exit(1);
    }
    return NULL;
}

AstTreeNode* remaining_list(AstTreeNode *parNode) {
    Symbols symbol;
    symbol.nt = nt_remaining_list;
    AstTreeNode *node = createNode(symbol, "remaining_list", 0);

    if (getCurrentToken()->terminal == TK_COMMA) {
        match_f(TK_COMMA);
        return parameter_list(parNode);
    } else {
        return NULL;
    }
}

AstTreeNode* stmts() {
    Symbols symbol;
    symbol.nt = nt_stmts;
    AstTreeNode *stmtNode = createNode(symbol, "stmts", 0);
    addChild(stmtNode, typeDefinitions());
    addChild(stmtNode, declarations(NULL));
    addChild(stmtNode, otherstmts(NULL));
    addChild(stmtNode, returnStmt());
    return stmtNode;
}

AstTreeNode* typeDefinitions() {
    Symbols symbol;
    symbol.nt = nt_typeDefinitions;
    AstTreeNode *typedefStmtNode = createNode(symbol, "typeDefinitions", 0);

    if (getCurrentToken()->terminal == TK_RECORD || getCurrentToken()->terminal == TK_UNION) {
        addChild(typedefStmtNode, isRedefined());
        addChild(typedefStmtNode, typeDefinition());
    } else {
        return NULL;
    }
    return typedefStmtNode;
}

AstTreeNode* isRedefined() {
    Symbols symbol;
    symbol.nt = nt_isRedefined;
    AstTreeNode *node = createNode(symbol, "isRedefined", 0);

    if (getCurrentToken()->terminal == TK_RECORD || getCurrentToken()->terminal == TK_UNION) {
        return typeDefinition();
    } else {
        return definetypestmt();
    }
}

AstTreeNode* typeDefinition() {
    Symbols symbol;
    symbol.nt = nt_typeDefinition;
    AstTreeNode *node = createNode(symbol, "typeDefinition", 0);

    if (getCurrentToken()->terminal == TK_RECORD) {
        addChild(node, match(TK_RECORD, TRUE));
        addChild(node, match(TK_RUID, TRUE));
        addChild(node, fieldDefinitions());
        match_f(TK_ENDRECORD);
    } else if (getCurrentToken()->terminal == TK_UNION) {
        addChild(node, match(TK_UNION, TRUE));
        addChild(node, match(TK_RUID, TRUE));
        addChild(node, fieldDefinitions());
        match_f(TK_ENDUNION);
    } else {
        printf("Syntax error in typeDefinition\n");
        exit(1);
    }
    return node;
}

AstTreeNode* fieldDefinitions() {
    Symbols symbol;
    symbol.nt = nt_fieldDefinitions;
    AstTreeNode *node = createNode(symbol, "fieldDefinitions", 0);
    addChild(node, fieldDefinition());
    moreFields(node);
    return node;
}

AstTreeNode* fieldDefinition() {
    Symbols symbol;
    symbol.nt = nt_fieldDefinition;
    AstTreeNode *node = createNode(symbol, "fieldDefinition", 0);
    match_f(TK_TYPE);
    addChild(node, dataType(node));
    match_f(TK_COLON);
    addChild(node, match(TK_FIELDID, TRUE));
    match_f(TK_SEM);
    return node;
}

AstTreeNode* moreFields(AstTreeNode *node) {
    Symbols symbol;
    symbol.nt = nt_moreFields;
    AstTreeNode *moreFieldsNode = createNode(symbol, "moreFields", 0);

    if (getCurrentToken()->terminal == TK_TYPE) {
        addChild(node, fieldDefinition());
        moreFields(node);
        return node;
    } else {
        return NULL;
    }
}

AstTreeNode* declarations(AstTreeNode *node) {
    Symbols symbol;
    symbol.nt = nt_declarations;
    if (!node) {
        node = createNode(symbol, "declarations", 0);
    }

    if (getCurrentToken()->terminal == TK_TYPE) {
        addChild(node, declaration());
        declarations(node);
    } else {
        addChild(node, NULL);
        return node;
    }
    return node;
}

AstTreeNode* declaration() {
    Symbols symbol;
    symbol.nt = nt_declaration;
    AstTreeNode *node = createNode(symbol, "declaration", 0);
    match_f(TK_TYPE);
    addChild(node, dataType(node));
    match_f(TK_COLON);
    addChild(node, match(TK_ID, TRUE));
    addChild(node, global_or_not());
    match_f(TK_SEM);
    return node;
}

AstTreeNode* global_or_not() {
    Symbols symbol;
    symbol.nt = nt_global_or_not;
    AstTreeNode *node = createNode(symbol, "global_or_not", 0);

    if (getCurrentToken()->terminal == TK_COLON) {
        match_f(TK_COLON);
        AstTreeNode *is_global = match(TK_GLOBAL, TRUE);
        if (!is_global) {
            return NULL;
        } else {
            return node;
        }
    } else {
        return NULL;
    }
}

AstTreeNode* otherstmts(AstTreeNode *node) {
    Symbols symbol;
    symbol.nt = nt_otherstmts;
    if (!node) {
        node = createNode(symbol, "otherstmts", 0);
    }

    if (getCurrentToken()->terminal == TK_ID || getCurrentToken()->terminal == TK_WHILE ||
        getCurrentToken()->terminal == TK_IF || getCurrentToken()->terminal == TK_READ ||
        getCurrentToken()->terminal == TK_WRITE || getCurrentToken()->terminal == TK_CALL) {

        stmt(node);
        addChild(node, otherstmts(node));
    } else {
        return NULL;
    }
    return node;
}


AstTreeNode* stmt(AstTreeNode *node) {
    Symbols symbol;
    symbol.nt = nt_stmt;
    AstTreeNode *stmtNode = createNode(symbol, "stmt", 0);

    if (getCurrentToken()->terminal == TK_ID) {
        addChild(node, assignmentStmt());
    } else if (getCurrentToken()->terminal == TK_WHILE) {
        addChild(node, iterativeStmt());
    } else if (getCurrentToken()->terminal == TK_IF) {
        addChild(node, conditionalStmt());
    } else if (getCurrentToken()->terminal == TK_READ || getCurrentToken()->terminal == TK_WRITE) {
        addChild(node, ioStmt());
    } else if (getCurrentToken()->terminal == TK_CALL) {
        addChild(node, funCallStmt());
    } else {
        printf("Syntax error in stmt\n");
        exit(1);
    }
    return stmtNode;
}

AstTreeNode* assignmentStmt() {
    Symbols symbol;
    symbol.nt = nt_assignmentStmt;
    AstTreeNode *node = createNode(symbol, "assignmentStmt", 0);
    addChild(node, singleOrRecId());
    addChild(node, match(TK_ASSIGNOP, TRUE));
    addChild(node, arithmeticExpression());
    match_f(TK_SEM);
    return node;
}

AstTreeNode* singleOrRecId() {
    Symbols symbol;
    symbol.nt = nt_singleOrRecId;
    AstTreeNode *node = createNode(symbol, "singleOrRecId", 0);
    AstTreeNode *variableNode = match(TK_ID, TRUE);
    addChild(node, variableNode);
    singleLeft(variableNode);
    return variableNode;
}

AstTreeNode* singleLeft(AstTreeNode *node) {
    Symbols symbol;
    symbol.nt = nt_singleLeft;
    AstTreeNode *singleLeftNode = createNode(symbol, "singleLeft", 0);

    if (getCurrentToken()->terminal == TK_DOT) {
        match_f(TK_DOT);
        AstTreeNode *fieldNode = oneExpansion(node);
        moreExpansions(fieldNode);
    } else {
        return NULL;
    }
    return node;
}

AstTreeNode* oneExpansion(AstTreeNode *node) {
    Symbols symbol;
    symbol.nt = nt_oneExpansion;
    AstTreeNode *oneExpansionNode = createNode(symbol, "oneExpansion", 0);

    match_f(TK_DOT);
    addChild(node, match(TK_FIELDID, TRUE));
    return node;
}

AstTreeNode* moreExpansions(AstTreeNode *node) {
    Symbols symbol;
    symbol.nt = nt_moreExpansions;
    AstTreeNode *moreExpansionsNode = createNode(symbol, "moreExpansions", 0);

    if (getCurrentToken()->terminal == TK_DOT) {
        AstTreeNode *fieldNode = oneExpansion(node);
        moreExpansions(fieldNode);
    } else {
        return NULL;
    }
    return node;
}

AstTreeNode* funCallStmt() {
    Symbols symbol;
    symbol.nt = nt_funCallStmt;
    AstTreeNode *node = createNode(symbol, "funCallStmt", 0);
    addChild(node, outputParameters());
    match_f(TK_CALL);
    addChild(node, match(TK_FUNID, TRUE));
    match_f(TK_WITH);
    match_f(TK_PARAMETERS);
    addChild(node, inputParameters());
    match_f(TK_SEM);
    return node;
}

AstTreeNode* outputParameters() {
    Symbols symbol;
    symbol.nt = nt_outputParameters;
    AstTreeNode *node = createNode(symbol, "outputParameters", 0);

    if (getCurrentToken()->terminal == TK_SQL) {
        match_f(TK_SQL);
        idList(node);
        match_f(TK_SQR);
        match(TK_ASSIGNOP, TRUE);
    } else {
        free(node);
        return NULL;
    }
    return node;
}

AstTreeNode* inputParameters() {
    Symbols symbol;
    symbol.nt = nt_inputParameters;
    AstTreeNode *node = createNode(symbol, "inputParameters", 0);

    match_f(TK_SQL);
    idList(node);
    match_f(TK_SQR);
    return node;
}

AstTreeNode* iterativeStmt() {
    Symbols symbol;
    symbol.nt = nt_iterativeStmt;
    AstTreeNode *node = createNode(symbol, "iterativeStmt", 0);

    match_f(TK_WHILE);
    match_f(TK_OP);
    addChild(node, booleanExpression());
    match_f(TK_CL);
    addChild(node, stmt(node));
    addChild(node, otherstmts(NULL));
    match_f(TK_ENDWHILE);
    return node;
}

AstTreeNode* conditionalStmt() {
    Symbols symbol;
    symbol.nt = nt_conditionalStmt;
    AstTreeNode *node = createNode(symbol, "conditionalStmt", 0);

    match_f(TK_IF);
    match_f(TK_OP);
    addChild(node, booleanExpression());
    match_f(TK_CL);
    match_f(TK_THEN);
    addChild(node, stmt(node));
    addChild(node, otherstmts(NULL));
    addChild(node, elseStmt());
    return node;
}

AstTreeNode* elseStmt() {
    Symbols symbol;
    symbol.nt = nt_elseStmt;
    AstTreeNode *node = createNode(symbol, "elseStmt", 0);

    if (getCurrentToken()->terminal == TK_ELSE) {
        match_f(TK_ELSE);
        addChild(node, otherstmts(NULL));
        match_f(TK_ENDIF);
    } else if (getCurrentToken()->terminal == TK_ENDIF) {
        match_f(TK_ENDIF);
    } else {
        printf("Syntax error in elseStmt\n");
        exit(1);
    }
    return node;
}

AstTreeNode* var() {
    Symbols symbol;
    symbol.nt = nt_var;
    AstTreeNode *node = createNode(symbol, "var", 0);

    if (getCurrentToken()->terminal == TK_ID) {
        addChild(node, singleOrRecId());
    } else if (getCurrentToken()->terminal == TK_NUM) {
        addChild(node, match(TK_NUM, TRUE));
    } else if (getCurrentToken()->terminal == TK_RNUM) {
        addChild(node, match(TK_RNUM, TRUE));
    }
    return node;
}

AstTreeNode* ioStmt() {
    Symbols symbol;
    symbol.nt = nt_ioStmt;
    AstTreeNode *node = createNode(symbol, "ioStmt", 0);

    if (getCurrentToken()->terminal == TK_READ) {
        addChild(node, match(TK_READ, TRUE));
        match_f(TK_OP);
        addChild(node, var());
        match_f(TK_CL);
        match_f(TK_SEM);
    } else if (getCurrentToken()->terminal == TK_WRITE) {
        addChild(node, match(TK_WRITE, TRUE));
        match_f(TK_OP);
        addChild(node, var());
        match_f(TK_CL);
        match_f(TK_SEM);
    } else {
        printf("Syntax error in ioStmt\n");
        exit(1);
    }
    return node;
}

AstTreeNode* arithmeticExpression() {
    Symbols symbol;
    symbol.nt = nt_arithmeticExpression;
    AstTreeNode *node = createNode(symbol, "arithmeticExpression", 0);

    AstTreeNode *termNode = term();
    AstTreeNode *operatorNode = expPrime();
    if (!operatorNode) {
        return termNode;
    } else {
        addChild(operatorNode, termNode);
        return operatorNode;
    }
    return node;
}

AstTreeNode* expPrime() {
    Symbols symbol;
    symbol.nt = nt_expPrime;
    AstTreeNode *node = createNode(symbol, "expPrime", 0);

    if (getCurrentToken()->terminal == TK_PLUS || getCurrentToken()->terminal == TK_MINUS) {
        AstTreeNode *operatorNode = lowPrecedenceOps();
        addChild(operatorNode, term());
        addChild(operatorNode, expPrime());
        return operatorNode;
    } else {
        return NULL;
    }
}

AstTreeNode* term() {
    Symbols symbol;
    symbol.nt = nt_term;
    AstTreeNode *node = createNode(symbol, "term", 0);

    AstTreeNode *factorNode = factor();
    AstTreeNode *expNode = termPrime();
    if (!expNode) {
        return factorNode;
    } else {
        addChild(expNode, factorNode);
        return expNode;
    }
    return node;
}

AstTreeNode* termPrime() {
    Symbols symbol;
    symbol.nt = nt_termPrime;
    AstTreeNode *node = createNode(symbol, "termPrime", 0);

    if (getCurrentToken()->terminal == TK_MUL || getCurrentToken()->terminal == TK_DIV) {
        AstTreeNode *operatorNode = highPrecedenceOps();
        addChild(operatorNode, factor());
        addChild(operatorNode, termPrime());
        return operatorNode;
    } else {
        return NULL;
    }
}

AstTreeNode* factor() {
    Symbols symbol;
    symbol.nt = nt_factor;
    AstTreeNode *node = createNode(symbol, "factor", 0);

    if (getCurrentToken()->terminal == TK_OP) {
        match_f(TK_OP);
        AstTreeNode *expressionNode = arithmeticExpression();
        match_f(TK_CL);
        return expressionNode;
    } else if (getCurrentToken()->terminal == TK_ID || getCurrentToken()->terminal == TK_NUM || getCurrentToken()->terminal == TK_RNUM) {
        return var();
    } else {
        printf("Syntax error in factor\n");
        exit(1);
    }
}

AstTreeNode* highPrecedenceOps() {
    Symbols symbol;
    symbol.nt = nt_highPrecedenceOps;
    AstTreeNode *node = createNode(symbol, "highPrecedenceOps", 0);

    if (getCurrentToken()->terminal == TK_MUL) {
        return match(TK_MUL, TRUE);
    } else if (getCurrentToken()->terminal == TK_DIV) {
        return match(TK_DIV, TRUE);
    } else {
        printf("Syntax error in highPrecedenceOps\n");
        exit(1);
    }
}

AstTreeNode* lowPrecedenceOps() {
    Symbols symbol;
    symbol.nt = nt_lowPrecedenceOps;
    AstTreeNode *node = createNode(symbol, "lowPrecedenceOps", 0);

    if (getCurrentToken()->terminal == TK_PLUS) {
        return match(TK_PLUS, TRUE);
    } else if (getCurrentToken()->terminal == TK_MINUS) {
        return match(TK_MINUS, TRUE);
    } else {
        printf("Syntax error in lowPrecedenceOps\n");
        exit(1);
    }
}

AstTreeNode* booleanExpression() {
    Symbols symbol;
    symbol.nt = nt_booleanExpression;
    AstTreeNode *node = createNode(symbol, "booleanExpression", 0);

    if (getCurrentToken()->terminal == TK_OP) {
        match_f(TK_OP);
        AstTreeNode *firstNode = booleanExpression();
        match_f(TK_CL);
        AstTreeNode *operatorNode = logicalOp();
        match_f(TK_OP);
        addChild(operatorNode, firstNode);
        addChild(operatorNode, booleanExpression());
        match_f(TK_CL);
        match_f(TK_SEM);
        return operatorNode;
    } else if (getCurrentToken()->terminal == TK_ID || getCurrentToken()->terminal == TK_NUM || getCurrentToken()->terminal == TK_RNUM) {
        AstTreeNode *firstVal = var();
        AstTreeNode *operatorNode = relationalOp();
        addChild(operatorNode, var());
        addChild(operatorNode, firstVal);
        return operatorNode;
    } else if (getCurrentToken()->terminal == TK_NOT) {
        AstTreeNode *operatorNode = match(TK_NOT, TRUE);
        match_f(TK_OP);
        addChild(operatorNode, booleanExpression());
        match_f(TK_CL);
        return operatorNode;
    } else {
        printf("Syntax error in booleanExpression\n");
        exit(1);
    }
}

AstTreeNode* logicalOp() {
    Symbols symbol;
    symbol.nt = nt_logicalOp;
    AstTreeNode *node = createNode(symbol, "logicalOp", 0);

    if (getCurrentToken()->terminal == TK_AND) {
        return match(TK_AND, TRUE);
    } else if (getCurrentToken()->terminal == TK_OR) {
        return match(TK_OR, TRUE);
    } else {
        printf("Syntax error in logicalOp\n");
        exit(1);
    }
}

AstTreeNode* relationalOp() {
    Symbols symbol;
    symbol.nt = nt_relationalOp;
    AstTreeNode *node = createNode(symbol, "relationalOp", 0);

    if (getCurrentToken()->terminal == TK_LT) {
        return match(TK_LT, TRUE);
    } else if (getCurrentToken()->terminal == TK_LE) {
        return match(TK_LE, TRUE);
    } else if (getCurrentToken()->terminal == TK_EQ) {
        return match(TK_EQ, TRUE);
    } else if (getCurrentToken()->terminal == TK_GT) {
        return match(TK_GT, TRUE);
    } else if (getCurrentToken()->terminal == TK_GE) {
        return match(TK_GE, TRUE);
    } else if (getCurrentToken()->terminal == TK_NE) {
        return match(TK_NE, TRUE);
    } else {
        printf("Syntax error in relationalOp\n");
        exit(1);
    }
}

AstTreeNode* returnStmt() {
    Symbols symbol;
    symbol.nt = nt_returnStmt;
    AstTreeNode *node = createNode(symbol, "returnStmt", 0);

    addChild(node, match(TK_RETURN, TRUE));
    addChild(node, optionalReturn());
    match_f(TK_SEM);
    return node;
}

AstTreeNode* optionalReturn() {
    Symbols symbol;
    symbol.nt = nt_optionalReturn;
    AstTreeNode *node = createNode(symbol, "optionalReturn", 0);

    if (getCurrentToken()->terminal == TK_SQL) {
        match_f(TK_SQL);
        AstTreeNode *idListNode = idList(NULL);
        match_f(TK_SQR);
        return idListNode;
    } else {
        return NULL;
    }
}

AstTreeNode* idList(AstTreeNode *node) {
    Symbols symbol;
    symbol.nt = nt_idList;
    if (!node) {
        node = createNode(symbol, "id_list", 0);
    }

    addChild(node, match(TK_ID, TRUE));
    addChild(node, moreIds(node));
    return node;
}

AstTreeNode* moreIds(AstTreeNode *node) {
    Symbols symbol;
    symbol.nt = nt_moreIds;
    AstTreeNode *moreIdsNode = createNode(symbol, "moreIds", 0);

    if (getCurrentToken()->terminal == TK_COMMA) {
        match_f(TK_COMMA);
        addChild(node, idList(node));
        return node;
    } else {
        return NULL;
    }
}

AstTreeNode* definetypestmt() {
    Symbols symbol;
    symbol.nt = nt_definetypestmt;
    AstTreeNode *node = createNode(symbol, "definetypestmt", 0);

    match_f(TK_DEFINETYPE);
    addChild(node, a());
    addChild(node, match(TK_RUID, TRUE));
    match_f(TK_AS);
    addChild(node, match(TK_RUID, TRUE));
    return node;
}

AstTreeNode* a() {
    Symbols symbol;
    symbol.nt = nt_a;
    AstTreeNode *node = createNode(symbol, "a", 0);

    if (getCurrentToken()->terminal == TK_RECORD) {
        addChild(node, match(TK_RECORD, TRUE));
    } else if (getCurrentToken()->terminal == TK_UNION) {
        addChild(node, match(TK_UNION, TRUE));
    } else {
        printf("Syntax error in a\n");
        exit(1);
    }
    return node;
}

void printParseTree(AstTreeNode* node, int level) {
    if(level > 10) return ;
    if (node == NULL) return;
    for (int i = 0; i < level; ++i) printf("  ");
    printf("%s\n", node->lexeme);
    for (int i = 0; i < node->childCount; ++i) {
        printParseTree(node->children[i], level + 1);
    }
}

int main(){
    loadTokensFromFile("tokens1.tkn");
    printf("length = %d\n", listSize);
    for(int i = 0; i < listSize; i++){
        Token currentToken = lexedTokens[i];
        printf("Value: %s, line no: %d, terminal_enum: %d\n", 
            currentToken->value, currentToken->lineno, currentToken->terminal);
    }
    parseTree = program();
    printParseTree(parseTree, 1);
    printf("Parsing completed succesfully\n");
    return 0;
}


// int main() {
//     // Example token stream
//     // Token declarationEx[] = {
//     //     {TK_MAIN, "_main"}, {TK_TYPE, "type"}, {TK_INT, "int"}, {TK_COLON, ":"},
//     //     {TK_ID, "x"},{TK_COLON, ':'},{TK_GLOBAL, "global"} ,{TK_SEM, ";"},
//     //     {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
//     // };

//     Token exampleTokens[] = {
//         {TK_MAIN, "_main"}, {TK_ID, "c3d2"}, {TK_ASSIGNOP, "<---"}, {TK_ID, "c3"},
//         {TK_MUL, "*"},{TK_OP, "("},{TK_ID, "c3"},{TK_PLUS, "+"}, {TK_ID, "c4"},{TK_CL,")"},{TK_SEM, ";"},
//         {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
//     };

//     // Token exampleTokens[] = {
//     //     {TK_MAIN, "_main"}, 
//     //     {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
//     // };
//     // Token *exampleTokens = assignVarEx;
//     tokens = exampleTokens;
//     tokenCount = sizeof(exampleTokens) / sizeof(exampleTokens[0]);

//     AstTreeNode *parseTree = program();
//     printParseTree(parseTree, 0);

//     printf("Parsing completed successfully.\n");
//     return 0;
// }   

#endif

// c3d2 <--- c3 * c3;
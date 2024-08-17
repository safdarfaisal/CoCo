#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper/definitions.h"

// Define maximum production length and count
#define MAXPRODLEN 8
#define MAXPRODCOUNT 100

// Token structure
typedef struct Token {
    Symbols type;
    char lexeme[100];
} Token;

// Parse tree node structure
typedef struct ParseTreeNode {
    Symbols symbol;
    char lexeme[100];
    struct ParseTreeNode *parent;
    struct ParseTreeNode **children;
    int currChild;
    int childCount;
} ParseTreeNode;

// Function to create a new parse tree node
ParseTreeNode* createNode(Symbols symbol, const char *lexeme) {
    ParseTreeNode* node = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
    node->symbol = symbol;
    node->parent=NULL;
    strcpy(node->lexeme, lexeme);
    node->children = NULL;
    node->childCount = 0;
    node->currChild = 0;
    return node;
}

// Function to add a child to a parse tree node
void addChild(ParseTreeNode *parent, ParseTreeNode *child) {
    parent->children = (ParseTreeNode**)realloc(parent->children, 
        (parent->childCount + 1) * sizeof(ParseTreeNode*));
    parent->children[parent->childCount] = child;
    parent->childCount++;
    if(!child){
        return;
    }
    child->parent = parent;  
}

void traverse_and_apply(ParseTreeNode *node, void (* apply)(ParseTreeNode *)){
    if(!node){
        return;
    } 
    for (int i = 0; i < node->childCount; i++){
        traverse_and_apply(node->children[i], apply);
    }

    
}

ParseTreeNode *getParent(ParseTreeNode *node){
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

ParseTreeNode *getNextChild(ParseTreeNode *node){
    if(!node){
        return NULL;
    }
    if(node->currChild == node->childCount){
        printf("All children have been met.");
        return NULL;
    }
    return node->children[(node->currChild)++];
}

// Forward declarations
ParseTreeNode* program();
ParseTreeNode* mainFunction();
ParseTreeNode* otherFunctions(ParseTreeNode *node);
ParseTreeNode* function();
ParseTreeNode* input_par();
ParseTreeNode* output_par(ParseTreeNode *node);
ParseTreeNode* parameter_list(ParseTreeNode *node);
ParseTreeNode* dataType(ParseTreeNode *node);
ParseTreeNode* primitiveDatatype();
ParseTreeNode* constructedDatatype(ParseTreeNode *node);
ParseTreeNode* remaining_list(ParseTreeNode *node);
ParseTreeNode* stmts();
ParseTreeNode* typeDefinitions();
ParseTreeNode* isRedefined();
ParseTreeNode* typeDefinition();
ParseTreeNode* fieldDefinitions();
ParseTreeNode* fieldDefinition();
ParseTreeNode* moreFields(ParseTreeNode *node);
ParseTreeNode* declarations(ParseTreeNode *node);
ParseTreeNode* declaration();
ParseTreeNode* global_or_not();
ParseTreeNode* otherstmts(ParseTreeNode *node);
ParseTreeNode* stmt(ParseTreeNode *node);
ParseTreeNode* assignmentStmt();
ParseTreeNode* singleOrRecId();
ParseTreeNode* singleLeft(ParseTreeNode *node);
ParseTreeNode* oneExpansion(ParseTreeNode *node);
ParseTreeNode* moreExpansions(ParseTreeNode *node);
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
ParseTreeNode* idList(ParseTreeNode *node);
ParseTreeNode* moreIds(ParseTreeNode *node);
ParseTreeNode* definetypestmt();
ParseTreeNode* a();

// Token stream and current token index
Token *tokens;
int currentTokenIndex = 0;
int tokenCount;

Token getCurrentToken() {
    return tokens[currentTokenIndex];
}

void advanceToken() {
    if (currentTokenIndex < tokenCount) {
        currentTokenIndex++;
    }
}

ParseTreeNode* match(Symbols expected, boolean necessary) {
    Token currentToken = getCurrentToken();
    ParseTreeNode* node;
    if (currentToken.type == expected) {
        if(necessary){ 
            node = createNode(currentToken.type, currentToken.lexeme);
            advanceToken();
        } else {
            node = NULL;
            advanceToken();
        }
        // advanceToken();
        return node;
    } else {
        printf("Syntax error: Expected %d but found %d\n", expected, currentToken.type);
        exit(1);
    }
}

void match_f(Symbols expected){
    match(expected, FALSE);
}

// Function implementations

ParseTreeNode* program() {
    ParseTreeNode *programNode = createNode(nt_program, "program");
    //program -> otherFunctions mainFunction
    otherFunctions(programNode);
    addChild(programNode, mainFunction());
    return programNode;
}

ParseTreeNode* mainFunction() {
    ParseTreeNode *mainFuncNode = createNode(nt_mainFunction, "mainFunction");
    //mainFunction -> TK_MAIN stmts TK_END
    match_f(TK_MAIN);
    addChild(mainFuncNode, stmts());
    match_f(TK_END);
    return mainFuncNode;
}

ParseTreeNode* otherFunctions(ParseTreeNode* programNode) {
    if (getCurrentToken().type == TK_FUNID) {
        // otherFunctions -> function otherFunctions
        addChild(programNode, function());
        otherFunctions(programNode);
    } else {
        // otherFunctions -> EPSILON
        addChild(programNode, NULL); 
    }
    return programNode;
}

ParseTreeNode* function() {
    ParseTreeNode *functionNode = createNode(nt_function, "function");
    // function -> TK_FUNID input_par output_par TK_SEM stmts TK_END
    addChild(functionNode, match(TK_FUNID, TRUE));
    addChild(functionNode, input_par());
    addChild(functionNode, output_par(functionNode));
    match(TK_SEM, FALSE);
    addChild(functionNode, stmts());
    match(TK_END, FALSE);
    return functionNode;
}

ParseTreeNode* input_par() {
    ParseTreeNode *inputParNode = createNode(nt_input_par, "input parameters");
    // input_par -> TK_INPUT TK_PARAMETER TK_LIST TK_SQL parameter_list TK_SQR
    match(TK_INPUT, FALSE);
    match(TK_PARAMETER, FALSE);
    match(TK_LIST, FALSE);
    match(TK_SQL, FALSE);
    parameter_list(inputParNode);
    match(TK_SQR, FALSE);
    return inputParNode;
}

ParseTreeNode* output_par(ParseTreeNode *node) {
    ParseTreeNode *outputParNode = createNode(nt_output_par, "output parameters");
    if (getCurrentToken().type == TK_OUTPUT) {
//      output_par -> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL parameter_list TK_SQR
        match(TK_OUTPUT, FALSE);
        match(TK_PARAMETER, FALSE);
        match(TK_LIST, FALSE);
        match(TK_SQL, FALSE);
        parameter_list(outputParNode);
        match(TK_SQR, FALSE);
        return outputParNode;
    } else {
        //output_par -> EPSILON
        return NULL;
    }
}

ParseTreeNode* parameter_list(ParseTreeNode *parNode) {
    ParseTreeNode *var = createNode(nt_parameter_list, "variable");
    addChild(var, dataType(var));
    addChild(var, match(TK_ID, TRUE)); // TODO: Fix TK_ID
    addChild(parNode, var);
    remaining_list(parNode);
    return parNode;
}


ParseTreeNode* dataType(ParseTreeNode *varNode) {
    if (getCurrentToken().type == TK_INT || getCurrentToken().type == TK_REAL) {
        return primitiveDatatype();
    } else if (getCurrentToken().type == TK_RECORD || getCurrentToken().type == TK_UNION || getCurrentToken().type == TK_RUID) {
        return constructedDatatype(varNode);
    } else {
        printf("Syntax error in dataType\n");
        exit(1);
    }
}

ParseTreeNode* primitiveDatatype() {
    if (getCurrentToken().type == TK_INT) {
        return match(TK_INT, TRUE);
    } else if (getCurrentToken().type == TK_REAL) {
        return match(TK_REAL, TRUE);
    } else {
        printf("Syntax error in primitiveDatatype\n");
        exit(1);
    }
}

ParseTreeNode* constructedDatatype(ParseTreeNode *varNode) {
    if (getCurrentToken().type == TK_RECORD) {
        // constructedDatatype -> TK_RECORD TK_RUID
        addChild(varNode, match(TK_RECORD, TRUE));
        addChild(varNode, match(TK_RUID, TRUE));
        // return TK_RUID lexeme
    } else if (getCurrentToken().type == TK_UNION) {
        // constructedDatatype -> TK_UNION TK_RUID
        addChild(varNode, match(TK_UNION, TRUE));
        addChild(varNode, match(TK_RUID, TRUE));
        // return TK_RUID lexeme
    } else if (getCurrentToken().type == TK_RUID) {
        addChild(varNode, match(TK_RUID, TRUE));
    } else {
        printf("Syntax error in constructedDatatype\n");
        exit(1);
    }
    return NULL;
}




ParseTreeNode* remaining_list(ParseTreeNode *parNode) {
    if (getCurrentToken().type == TK_COMMA) {
        // remaining_list -> TK_COMMA parameter_list
        match_f(TK_COMMA);
        return parameter_list(parNode);
    } else {
        return NULL;
    }
    return parNode;
}

ParseTreeNode* stmts() {
    ParseTreeNode *stmtNode = createNode(nt_stmts, "stmts");
    addChild(stmtNode, typeDefinitions());
    addChild(stmtNode, declarations(NULL));
    addChild(stmtNode, otherstmts(NULL));
    addChild(stmtNode, returnStmt());
    return stmtNode;
}

ParseTreeNode* typeDefinitions() {
    ParseTreeNode *typedefStmtNode = createNode(nt_typeDefinitions, "typeDefinitions");
    if (getCurrentToken().type == TK_RECORD || getCurrentToken().type == TK_UNION) {
        addChild(typedefStmtNode, isRedefined());
        addChild(typedefStmtNode, typeDefinition());
    } else {
        return NULL;
    }
    return typedefStmtNode;
}

ParseTreeNode* isRedefined() {
    if (getCurrentToken().type == TK_RECORD || getCurrentToken().type == TK_UNION) {
        return typeDefinition();
    } else {
        return definetypestmt();
    }
}

ParseTreeNode* typeDefinition() {
    ParseTreeNode *node = createNode(nt_typeDefinition, "typeDefinition");
    if (getCurrentToken().type == TK_RECORD) {
        addChild(node, match(TK_RECORD, TRUE));
        addChild(node, match(TK_RUID, TRUE));
        addChild(node, fieldDefinitions());
        match_f(TK_ENDRECORD);
    } else if (getCurrentToken().type == TK_UNION) {
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

ParseTreeNode* fieldDefinitions() {
    ParseTreeNode *node = createNode(nt_fieldDefinitions, "fieldDefinitions");
    addChild(node, fieldDefinition());
    moreFields(node);
    return node;
}

ParseTreeNode* fieldDefinition() {
    ParseTreeNode *node = createNode(nt_fieldDefinition, "fieldDefinition");
    match_f(TK_TYPE);
    addChild(node, dataType(node));
    match_f(TK_COLON);
    addChild(node, match(TK_FIELDID, TRUE));
    match_f(TK_SEM);
    return node;
}

ParseTreeNode* moreFields(ParseTreeNode *node) {
    if (getCurrentToken().type == TK_TYPE) {
        addChild(node, fieldDefinition());
        moreFields(node);
        return node;
    } else {
        return NULL;
    }
}

ParseTreeNode* declarations(ParseTreeNode *node) {
    if(!node){
        node = createNode(nt_declarations, "declarations");
    }
    if (getCurrentToken().type == TK_TYPE) {
        addChild(node, declaration());
        declarations(node);
    } else {
        addChild(node, NULL);
        return node;
    }
    return node;
}

ParseTreeNode* declaration() {
    ParseTreeNode *node = createNode(nt_declaration, "declaration");
    match_f(TK_TYPE);
    addChild(node, dataType(node));
    match_f(TK_COLON);
    addChild(node, match(TK_ID, TRUE));
    addChild(node, global_or_not());
    match_f(TK_SEM);
    return node;
}

ParseTreeNode* global_or_not() {
    ParseTreeNode *node = createNode(nt_global_or_not, "global_or_not");
    if (getCurrentToken().type == TK_COLON) {
        match_f(TK_COLON);
        ParseTreeNode *is_global = match(TK_GLOBAL, TRUE);
        if(!is_global){
            return NULL;
        } else {
            return node;
        }
    } else {
        return NULL;
    }
    return node;
}

ParseTreeNode* otherstmts(ParseTreeNode *node) {
    if(!node){
        node = createNode(nt_otherstmts, "otherstmts");
    } 
    if (getCurrentToken().type == TK_ID || getCurrentToken().type == TK_WHILE ||
        getCurrentToken().type == TK_IF || getCurrentToken().type == TK_READ ||
        getCurrentToken().type == TK_WRITE || getCurrentToken().type == TK_CALL) {
        
        stmt(node);
        addChild(node, otherstmts(node));
    } else {
        return NULL;
    }
    return node;
}

ParseTreeNode* stmt(ParseTreeNode *node) {
    if (getCurrentToken().type == TK_ID) {
        addChild(node, assignmentStmt());
    } else if (getCurrentToken().type == TK_WHILE) {
        addChild(node, iterativeStmt());
    } else if (getCurrentToken().type == TK_IF) {
        addChild(node, conditionalStmt());
    } else if (getCurrentToken().type == TK_READ || getCurrentToken().type == TK_WRITE) {
        addChild(node, ioStmt());
    } else if (getCurrentToken().type == TK_CALL) {
        addChild(node, funCallStmt());
    } else {
        printf("Syntax error in stmt\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* assignmentStmt() {
    ParseTreeNode *node = createNode(nt_assignmentStmt, "assignmentStmt");
    addChild(node, singleOrRecId());
    addChild(node, match(TK_ASSIGNOP, TRUE));
    addChild(node, arithmeticExpression());
    match_f(TK_SEM);
    return node;
}

ParseTreeNode* singleOrRecId() {
    ParseTreeNode *node = createNode(nt_singleOrRecId, "singleOrRecId");
    ParseTreeNode *variableNode = match(TK_ID, TRUE);
    addChild(node, variableNode);
    singleLeft(variableNode);
    return variableNode;
}

ParseTreeNode* singleLeft(ParseTreeNode *node) {
    if (getCurrentToken().type == TK_DOT) {
        match_f(TK_DOT);
        ParseTreeNode *fieldNode = oneExpansion(node);
        moreExpansions(fieldNode);   
    } else {
        return NULL;
    }
    return node;
}

ParseTreeNode* oneExpansion(ParseTreeNode *node) {
    match_f(TK_DOT);
    addChild(node, match(TK_FIELDID, TRUE));
    return node;
}

ParseTreeNode* moreExpansions(ParseTreeNode *node) {
    if (getCurrentToken().type == TK_DOT) {
        ParseTreeNode *fieldNode = oneExpansion(node);
        moreExpansions(fieldNode);
    } else {
        return NULL;
    }
    return node;
}

ParseTreeNode* funCallStmt() {
    ParseTreeNode *node = createNode(nt_funCallStmt, "funCallStmt");
    addChild(node, outputParameters());
    match_f(TK_CALL);
    addChild(node, match(TK_FUNID, TRUE));
    match_f(TK_WITH);
    match_f(TK_PARAMETERS);
    addChild(node, inputParameters());
    match_f(TK_SEM);
    return node;
}

ParseTreeNode* outputParameters() {
    ParseTreeNode *node = createNode(nt_outputParameters, "outputParameters");
    if (getCurrentToken().type == TK_SQL) {
        match_f(TK_SQL);
        idList(node); // might need to remove this node
        match_f(TK_SQR);
        match(TK_ASSIGNOP, TRUE);
    } else {
        free(node);
        return NULL;
    }
    return node;
}

ParseTreeNode* inputParameters() {
    ParseTreeNode *node = createNode(nt_inputParameters, "inputParameters");
    match_f(TK_SQL);
    idList(node); // might need to remove this node
    match_f(TK_SQR);
    return node;
}

ParseTreeNode* iterativeStmt() {
    ParseTreeNode *node = createNode(nt_iterativeStmt, "iterativeStmt");
    match_f(TK_WHILE);
    match_f(TK_OP);
    addChild(node, booleanExpression());
    match_f(TK_CL);
    addChild(node, stmt(node));
    addChild(node, otherstmts(NULL));
    match_f(TK_ENDWHILE);
    return node;
}

ParseTreeNode* conditionalStmt() {
    ParseTreeNode *node = createNode(nt_conditionalStmt, "conditionalStmt");
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

ParseTreeNode* elseStmt() {
    ParseTreeNode *node = createNode(nt_elseStmt, "elseStmt");
    if (getCurrentToken().type == TK_ELSE) {
        match_f(TK_ELSE);
        addChild(node, otherstmts(NULL));
        match_f(TK_ENDIF);
    } else if (getCurrentToken().type == TK_ENDIF) {
        match_f(TK_ENDIF);
    } else {
        printf("Syntax error in elseStmt\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* var() {
    ParseTreeNode *node = createNode(nt_var, "var");
    if (getCurrentToken().type == TK_ID) {
        addChild(node, singleOrRecId());
    } else if (getCurrentToken().type == TK_NUM) {
        addChild(node, match(TK_NUM, TRUE));
    } else if (getCurrentToken().type == TK_RNUM) {
        addChild(node, match(TK_RNUM, TRUE));
    }
    return node;
}

ParseTreeNode* ioStmt() {
    ParseTreeNode *node = createNode(nt_ioStmt, "ioStmt");
    if (getCurrentToken().type == TK_READ) {
        addChild(node, match(TK_READ, TRUE));
        match_f(TK_OP);
        addChild(node, var());
        match_f(TK_CL);
        match_f(TK_SEM);
    } else if (getCurrentToken().type == TK_WRITE) {
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


/* TODO:

                            |---- |---| \  /
                            |----   |    \/
                            |       |    /\
                            |     |---| /  \
*/


ParseTreeNode* arithmeticExpression() {
    ParseTreeNode *node = createNode(nt_arithmeticExpression, "arithmeticExpression");
    ParseTreeNode *termNode = term();
    ParseTreeNode *operatorNode = expPrime();
    if(!operatorNode){
        return termNode;
    } else {
        addChild(operatorNode, termNode);
        return operatorNode;
    }
    return node;
}

ParseTreeNode* expPrime() {
    if (getCurrentToken().type == TK_PLUS || getCurrentToken().type == TK_MINUS) {
        ParseTreeNode *operatorNode = lowPrecedenceOps();
        addChild(operatorNode, term());
        addChild(operatorNode, expPrime());
        return operatorNode;
    } else {
        return NULL;
    }
}

ParseTreeNode* term() {
    ParseTreeNode *node = createNode(nt_term, "term");
    ParseTreeNode *factorNode = factor();
    ParseTreeNode *expNode = termPrime();
    if(!expNode){
        return factorNode;
    } else {
        addChild(expNode, factorNode);
        return expNode;
    }
    return node;
}

ParseTreeNode* termPrime() {

    if (getCurrentToken().type == TK_MUL || getCurrentToken().type == TK_DIV) {
        ParseTreeNode *operatorNode = highPrecedenceOps();
        addChild(operatorNode, factor());
        addChild(operatorNode, termPrime());
        return operatorNode;
    } else {
        return NULL;
    }
}

ParseTreeNode* factor() {

    if (getCurrentToken().type == TK_OP) {
        match_f(TK_OP);
        ParseTreeNode *node = arithmeticExpression();
        match_f(TK_CL);
        return node;
    } else if (getCurrentToken().type == TK_ID || getCurrentToken().type == TK_NUM || getCurrentToken().type == TK_RNUM) {
        return var();
    } else {
        printf("Syntax error in factor\n");
        exit(1);
    }
}

/*

                                -----  |\    |  |\   
                                |      | \   |  | \  
                                ----   |  \  |  |  \ 
                                |      |   \ |  |  /
                                -----  |    \|  | /

TODO: 
*/

ParseTreeNode* highPrecedenceOps() {
    ParseTreeNode *node = createNode(nt_highPrecedenceOps, "highPrecedenceOps");
    if (getCurrentToken().type == TK_MUL) {
        return match(TK_MUL, TRUE);
    } else if (getCurrentToken().type == TK_DIV) {
        return match(TK_DIV,TRUE);
    } else {
        printf("Syntax error in highPrecedenceOps\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* lowPrecedenceOps() {
    ParseTreeNode *node = createNode(nt_lowPrecedenceOps, "lowPrecedenceOps");
    if (getCurrentToken().type == TK_PLUS) {
        return match(TK_PLUS, TRUE);
    } else if (getCurrentToken().type == TK_MINUS) {
        return match(TK_MINUS, TRUE);
    } else {
        printf("Syntax error in lowPrecedenceOps\n");
        exit(1);
    }
    return node;
}



ParseTreeNode* booleanExpression() {
    if (getCurrentToken().type == TK_OP) {
        match_f(TK_OP);
        ParseTreeNode *firstNode = booleanExpression();
        match_f(TK_CL);
        ParseTreeNode *operatorNode = logicalOp();
        match_f(TK_OP);
        addChild(operatorNode, firstNode);
        addChild(operatorNode, booleanExpression());
        match_f(TK_CL);
        match_f(TK_SEM);
        return operatorNode;
    } else if (getCurrentToken().type == TK_ID || getCurrentToken().type == TK_NUM || getCurrentToken().type == TK_RNUM) {
        ParseTreeNode *firstVal = var();
        ParseTreeNode *operatorNode = relationalOp();
        addChild(operatorNode, var());
        addChild(operatorNode, firstVal);
        return operatorNode;
    } else if (getCurrentToken().type == TK_NOT) {
        ParseTreeNode *operatorNode = match(TK_NOT, TRUE);
        match_f(TK_OP);
        addChild(operatorNode, booleanExpression());
        match_f(TK_CL);
        return operatorNode;
    } else {
        printf("Syntax error in booleanExpression\n");
        exit(1);
    }
}

ParseTreeNode* logicalOp() {
    if (getCurrentToken().type == TK_AND) {
        return match(TK_AND, TRUE); // handle operators
    } else if (getCurrentToken().type == TK_OR) {
        return match(TK_OR, TRUE);
    } else {
        printf("Syntax error in logicalOp\n");
        exit(1);
    }
}

ParseTreeNode* relationalOp() {
    ParseTreeNode *node = createNode(nt_relationalOp, "relationalOp");
    if (getCurrentToken().type == TK_LT) {
        return match(TK_LT, TRUE);
    } else if (getCurrentToken().type == TK_LE) {
        return match(TK_LE, TRUE);
    } else if (getCurrentToken().type == TK_EQ) {
        return match(TK_EQ,TRUE);
    } else if (getCurrentToken().type == TK_GT) {
        return match(TK_GT, TRUE);
    } else if (getCurrentToken().type == TK_GE) {
        return match(TK_GE, TRUE);
    } else if (getCurrentToken().type == TK_NE) {
        return match(TK_NE, TRUE);
    } else {
        printf("Syntax error in relationalOp\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* returnStmt() {
    ParseTreeNode *node = createNode(nt_returnStmt, "returnStmt");
    addChild(node, match(TK_RETURN, TRUE));
    addChild(node, optionalReturn());
    match_f(TK_SEM);
    return node;
}

ParseTreeNode* optionalReturn() {
    if (getCurrentToken().type == TK_SQL) {
        match_f(TK_SQL);
        ParseTreeNode *idListNode = idList(NULL);
        match_f(TK_SQR);
        return idListNode;
    } else {
        return NULL;
    }
}

ParseTreeNode* idList(ParseTreeNode *node) {
    if(!node){
        node = createNode(TK_PARAMETERS, "id_list");
    }
    addChild(node, match(TK_ID, TRUE));
    addChild(node, moreIds(node));
    return node;
}

ParseTreeNode* moreIds(ParseTreeNode *node) {
    if (getCurrentToken().type == TK_COMMA) {
        match_f(TK_COMMA);
        addChild(node, idList(node));
        return node;
    } else {
        return NULL;
    }
}

ParseTreeNode* definetypestmt() {
    ParseTreeNode *node = createNode(nt_definetypestmt, "definetypestmt");
    match_f(TK_DEFINETYPE);
    addChild(node, a());
    addChild(node, match(TK_RUID, TRUE));
    match_f(TK_AS);
    addChild(node, match(TK_RUID, TRUE));
    return node;
}

ParseTreeNode* a() {
    ParseTreeNode *node = createNode(nt_a, "a");
    if (getCurrentToken().type == TK_RECORD) {
        addChild(node, match(TK_RECORD, TRUE));
    } else if (getCurrentToken().type == TK_UNION) {
        addChild(node, match(TK_UNION, TRUE));
    } else {
        printf("Syntax error in a\n");
        exit(1);
    }
    return node;
}

void printParseTree(ParseTreeNode* node, int level) {
    if(level > 10) return ;
    if (node == NULL) return;
    for (int i = 0; i < level; ++i) printf("  ");
    printf("%s\n", node->lexeme);
    for (int i = 0; i < node->childCount; ++i) {
        printParseTree(node->children[i], level + 1);
    }
}

int main() {
    // Example token stream
    // Token declarationEx[] = {
    //     {TK_MAIN, "_main"}, {TK_TYPE, "type"}, {TK_INT, "int"}, {TK_COLON, ":"},
    //     {TK_ID, "x"},{TK_COLON, ':'},{TK_GLOBAL, "global"} ,{TK_SEM, ";"},
    //     {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
    // };

    Token exampleTokens[] = {
        {TK_MAIN, "_main"}, {TK_ID, "c3d2"}, {TK_ASSIGNOP, "<---"}, {TK_ID, "c3"},
        {TK_MUL, "*"},{TK_OP, "("},{TK_ID, "c3"},{TK_PLUS, "+"}, {TK_ID, "c4"},{TK_CL,")"},{TK_SEM, ";"},
        {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
    };

    // Token exampleTokens[] = {
    //     {TK_MAIN, "_main"}, 
    //     {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
    // };
    // Token *exampleTokens = assignVarEx;
    tokens = exampleTokens;
    tokenCount = sizeof(exampleTokens) / sizeof(exampleTokens[0]);

    ParseTreeNode *parseTree = program();
    printParseTree(parseTree, 0);

    printf("Parsing completed successfully.\n");
    return 0;
}   

#endif

// c3d2 <--- c3 * c3;
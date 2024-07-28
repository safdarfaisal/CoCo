#include "parser.h"

// Function to create a new parse tree node
ParseTreeNode* createNode(Symbols symbol, const char *lexeme) {
    ParseTreeNode* node = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
    node->symbol = symbol;
    strcpy(node->lexeme, lexeme);
    node->children = NULL;
    node->childCount = 0;
    return node;
}

// Function to add a child to a parse tree node
void addChild(ParseTreeNode *parent, ParseTreeNode *child) {
    parent->children = (ParseTreeNode**)realloc(parent->children, (parent->childCount + 1) * sizeof(ParseTreeNode*));
    parent->children[parent->childCount] = child;
    parent->childCount++;
}



// Token stream and current token index

Token getCurrentToken() {
    return tokens[currentTokenIndex];
}

void advanceToken() {
    if (currentTokenIndex < tokenCount) {
        currentTokenIndex++;
    }
}

ParseTreeNode* match(Symbols expected) {
    Token currentToken = getCurrentToken();
    if (currentToken.type == expected) {
        ParseTreeNode* node = createNode(currentToken.type, currentToken.lexeme);
        advanceToken();
        return node;
    } else {
        printf("Syntax error: Expected %d but found %d\n", expected, currentToken.type);
        exit(1);
    }
}

// Function implementations

ParseTreeNode* program() {
    ParseTreeNode *node = createNode(nt_program, "program");
    addChild(node, otherFunctions());
    addChild(node, mainFunction());
    return node;
}

ParseTreeNode* mainFunction() {
    ParseTreeNode *node = createNode(nt_mainFunction, "mainFunction");
    addChild(node, match(TK_MAIN));
    addChild(node, stmts());
    addChild(node, match(TK_END));
    return node;
}

ParseTreeNode* otherFunctions() {
    ParseTreeNode *node = createNode(nt_otherFunctions, "otherFunctions");
    if (getCurrentToken().type == TK_FUNID) {
        addChild(node, function());
        addChild(node, otherFunctions());
    }
    return node;
}

ParseTreeNode* function() {
    ParseTreeNode *node = createNode(nt_function, "function");
    addChild(node, match(TK_FUNID));
    addChild(node, input_par());
    addChild(node, output_par());
    addChild(node, match(TK_SEM));
    addChild(node, stmts());
    addChild(node, match(TK_END));
    return node;
}

ParseTreeNode* input_par() {
    ParseTreeNode *node = createNode(nt_input_par, "input_par");
    addChild(node, match(TK_INPUT));
    addChild(node, match(TK_PARAMETER));
    addChild(node, match(TK_LIST));
    addChild(node, match(TK_SQL));
    addChild(node, parameter_list());
    addChild(node, match(TK_SQR));
    return node;
}

ParseTreeNode* output_par() {
    ParseTreeNode *node = createNode(nt_output_par, "output_par");
    if (getCurrentToken().type == TK_OUTPUT) {
        addChild(node, match(TK_OUTPUT));
        addChild(node, match(TK_PARAMETER));
        addChild(node, match(TK_LIST));
        addChild(node, match(TK_SQL));
        addChild(node, parameter_list());
        addChild(node, match(TK_SQR));
    }
    return node;
}

ParseTreeNode* parameter_list() {
    ParseTreeNode *node = createNode(nt_parameter_list, "parameter_list");
    addChild(node, dataType());
    addChild(node, match(TK_ID));
    addChild(node, remaining_list());
    return node;
}

ParseTreeNode* dataType() {
    ParseTreeNode *node = createNode(nt_dataType, "dataType");
    if (getCurrentToken().type == TK_INT || getCurrentToken().type == TK_REAL) {
        addChild(node, primitiveDatatype());
    } else if (getCurrentToken().type == TK_RECORD || getCurrentToken().type == TK_UNION || getCurrentToken().type == TK_RUID) {
        addChild(node, constructedDatatype());
    } else {
        printf("Syntax error in dataType\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* primitiveDatatype() {
    ParseTreeNode *node = createNode(nt_primitiveDatatype, "primitiveDatatype");
    if (getCurrentToken().type == TK_INT) {
        addChild(node, match(TK_INT));
    } else if (getCurrentToken().type == TK_REAL) {
        addChild(node, match(TK_REAL));
    } else {
        printf("Syntax error in primitiveDatatype\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* constructedDatatype() {
    ParseTreeNode *node = createNode(nt_constructedDatatype, "constructedDatatype");
    if (getCurrentToken().type == TK_RECORD) {
        addChild(node, match(TK_RECORD));
        addChild(node, match(TK_RUID));
    } else if (getCurrentToken().type == TK_UNION) {
        addChild(node, match(TK_UNION));
        addChild(node, match(TK_RUID));
    } else if (getCurrentToken().type == TK_RUID) {
        addChild(node, match(TK_RUID));
    } else {
        printf("Syntax error in constructedDatatype\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* remaining_list() {
    ParseTreeNode *node = createNode(nt_remaining_list, "remaining_list");
    if (getCurrentToken().type == TK_COMMA) {
        addChild(node, match(TK_COMMA));
        addChild(node, parameter_list());
    }
    return node;
}

ParseTreeNode* stmts() {
    ParseTreeNode *node = createNode(nt_stmts, "stmts");
    addChild(node, typeDefinitions());
    addChild(node, declarations());
    addChild(node, otherstmts());
    addChild(node, returnStmt());
    return node;
}

ParseTreeNode* typeDefinitions() {
    ParseTreeNode *node = createNode(nt_typeDefinitions, "typeDefinitions");
    if (getCurrentToken().type == TK_RECORD || getCurrentToken().type == TK_UNION) {
        addChild(node, isRedefined());
        addChild(node, typeDefinition());
    }
    return node;
}

ParseTreeNode* isRedefined() {
    ParseTreeNode *node = createNode(nt_isRedefined, "isRedefined");
    if (getCurrentToken().type == TK_RECORD || getCurrentToken().type == TK_UNION) {
        addChild(node, typeDefinition());
    } else {
        addChild(node, definetypestmt());
    }
    return node;
}

ParseTreeNode* typeDefinition() {
    ParseTreeNode *node = createNode(nt_typeDefinition, "typeDefinition");
    if (getCurrentToken().type == TK_RECORD) {
        addChild(node, match(TK_RECORD));
        addChild(node, match(TK_RUID));
        addChild(node, fieldDefinitions());
        addChild(node, match(TK_ENDRECORD));
    } else if (getCurrentToken().type == TK_UNION) {
        addChild(node, match(TK_UNION));
        addChild(node, match(TK_RUID));
        addChild(node, fieldDefinitions());
        addChild(node, match(TK_ENDUNION));
    } else {
        printf("Syntax error in typeDefinition\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* fieldDefinitions() {
    ParseTreeNode *node = createNode(nt_fieldDefinitions, "fieldDefinitions");
    addChild(node, fieldDefinition());
    addChild(node, moreFields());
    return node;
}

ParseTreeNode* fieldDefinition() {
    ParseTreeNode *node = createNode(nt_fieldDefinition, "fieldDefinition");
    addChild(node, match(TK_TYPE));
    addChild(node, dataType());
    addChild(node, match(TK_COLON));
    addChild(node, match(TK_FIELDID));
    addChild(node, match(TK_SEM));
    return node;
}

ParseTreeNode* moreFields() {
    ParseTreeNode *node = createNode(nt_moreFields, "moreFields");
    if (getCurrentToken().type == TK_TYPE) {
        addChild(node, fieldDefinition());
        addChild(node, moreFields());
    }
    return node;
}

ParseTreeNode* declarations() {
    ParseTreeNode *node = createNode(nt_declarations, "declarations");
    if (getCurrentToken().type == TK_TYPE) {
        addChild(node, declaration());
        addChild(node, declarations());
    }
    return node;
}

ParseTreeNode* declaration() {
    ParseTreeNode *node = createNode(nt_declaration, "declaration");
    addChild(node, match(TK_TYPE));
    addChild(node, dataType());
    addChild(node, match(TK_COLON));
    addChild(node, match(TK_ID));
    addChild(node, global_or_not());
    addChild(node, match(TK_SEM));
    return node;
}

ParseTreeNode* global_or_not() {
    ParseTreeNode *node = createNode(nt_global_or_not, "global_or_not");
    if (getCurrentToken().type == TK_COLON) {
        addChild(node, match(TK_COLON));
        addChild(node, match(TK_GLOBAL));
    }
    return node;
}

ParseTreeNode* otherstmts() {
    ParseTreeNode *node = createNode(nt_otherstmts, "otherstmts");
    if (getCurrentToken().type == TK_ID || getCurrentToken().type == TK_WHILE || getCurrentToken().type == TK_IF || getCurrentToken().type == TK_READ || getCurrentToken().type == TK_WRITE || getCurrentToken().type == TK_CALL) {
        addChild(node, stmt());
        addChild(node, otherstmts());
    }
    return node;
}

ParseTreeNode* stmt() {
    ParseTreeNode *node = createNode(nt_stmt, "stmt");
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
    addChild(node, match(TK_ASSIGNOP));
    addChild(node, arithmeticExpression());
    addChild(node, match(TK_SEM));
    return node;
}

ParseTreeNode* singleOrRecId() {
    ParseTreeNode *node = createNode(nt_singleOrRecId, "singleOrRecId");
    addChild(node, match(TK_ID));
    addChild(node, singleLeft());
    return node;
}

ParseTreeNode* singleLeft() {
    ParseTreeNode *node = createNode(nt_singleLeft, "singleLeft");
    if (getCurrentToken().type == TK_ID) {
        addChild(node, match(TK_ID));
        addChild(node, oneExpansion());
        addChild(node, moreExpansions());
    }
    return node;
}

ParseTreeNode* oneExpansion() {
    ParseTreeNode *node = createNode(nt_oneExpansion, "oneExpansion");
    addChild(node, match(TK_DOT));
    addChild(node, match(TK_FIELDID));
    return node;
}

ParseTreeNode* moreExpansions() {
    ParseTreeNode *node = createNode(nt_moreExpansions, "moreExpansions");
    if (getCurrentToken().type == TK_DOT) {
        addChild(node, oneExpansion());
        addChild(node, moreExpansions());
    }
    return node;
}

ParseTreeNode* funCallStmt() {
    ParseTreeNode *node = createNode(nt_funCallStmt, "funCallStmt");
    addChild(node, outputParameters());
    addChild(node, match(TK_CALL));
    addChild(node, match(TK_FUNID));
    addChild(node, match(TK_WITH));
    addChild(node, match(TK_PARAMETERS));
    addChild(node, inputParameters());
    addChild(node, match(TK_SEM));
    return node;
}

ParseTreeNode* outputParameters() {
    ParseTreeNode *node = createNode(nt_outputParameters, "outputParameters");
    if (getCurrentToken().type == TK_SQL) {
        addChild(node, match(TK_SQL));
        addChild(node, idList());
        addChild(node, match(TK_SQR));
        addChild(node, match(TK_ASSIGNOP));
    }
    return node;
}

ParseTreeNode* inputParameters() {
    ParseTreeNode *node = createNode(nt_inputParameters, "inputParameters");
    addChild(node, match(TK_SQL));
    addChild(node, idList());
    addChild(node, match(TK_SQR));
    return node;
}

ParseTreeNode* iterativeStmt() {
    ParseTreeNode *node = createNode(nt_iterativeStmt, "iterativeStmt");
    addChild(node, match(TK_WHILE));
    addChild(node, match(TK_OP));
    addChild(node, booleanExpression());
    addChild(node, match(TK_CL));
    addChild(node, stmt());
    addChild(node, otherstmts());
    addChild(node, match(TK_ENDWHILE));
    return node;
}

ParseTreeNode* conditionalStmt() {
    ParseTreeNode *node = createNode(nt_conditionalStmt, "conditionalStmt");
    addChild(node, match(TK_IF));
    addChild(node, match(TK_OP));
    addChild(node, booleanExpression());
    addChild(node, match(TK_CL));
    addChild(node, match(TK_THEN));
    addChild(node, stmt());
    addChild(node, otherstmts());
    addChild(node, elseStmt());
    return node;
}

ParseTreeNode* elseStmt() {
    ParseTreeNode *node = createNode(nt_elseStmt, "elseStmt");
    if (getCurrentToken().type == TK_ELSE) {
        addChild(node, match(TK_ELSE));
        addChild(node, otherstmts());
        addChild(node, match(TK_ENDIF));
    } else if (getCurrentToken().type == TK_ENDIF) {
        addChild(node, match(TK_ENDIF));
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
        addChild(node, match(TK_NUM));
    } else if (getCurrentToken().type == TK_RNUM) {
        addChild(node, match(TK_RNUM));
    }
    return node;
}

ParseTreeNode* ioStmt() {
    ParseTreeNode *node = createNode(nt_ioStmt, "ioStmt");
    if (getCurrentToken().type == TK_READ) {
        addChild(node, match(TK_READ));
        addChild(node, match(TK_OP));
        addChild(node, var());
        addChild(node, match(TK_CL));
        addChild(node, match(TK_SEM));
    } else if (getCurrentToken().type == TK_WRITE) {
        addChild(node, match(TK_WRITE));
        addChild(node, match(TK_OP));
        addChild(node, var());
        addChild(node, match(TK_CL));
        addChild(node, match(TK_SEM));
    } else {
        printf("Syntax error in ioStmt\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* arithmeticExpression() {
    ParseTreeNode *node = createNode(nt_arithmeticExpression, "arithmeticExpression");
    addChild(node, term());
    addChild(node, expPrime());
    return node;
}

ParseTreeNode* expPrime() {
    ParseTreeNode *node = createNode(nt_expPrime, "expPrime");
    if (getCurrentToken().type == TK_PLUS || getCurrentToken().type == TK_MINUS) {
        addChild(node, lowPrecedenceOps());
        addChild(node, term());
        addChild(node, expPrime());
    }
    return node;
}

ParseTreeNode* term() {
    ParseTreeNode *node = createNode(nt_term, "term");
    addChild(node, factor());
    addChild(node, termPrime());
    return node;
}

ParseTreeNode* termPrime() {
    ParseTreeNode *node = createNode(nt_termPrime, "termPrime");
    if (getCurrentToken().type == TK_MUL || getCurrentToken().type == TK_DIV) {
        addChild(node, highPrecedenceOps());
        addChild(node, factor());
        addChild(node, termPrime());
    }
    return node;
}

ParseTreeNode* factor() {
    ParseTreeNode *node = createNode(nt_factor, "factor");
    if (getCurrentToken().type == TK_OP) {
        addChild(node, match(TK_OP));
        addChild(node, arithmeticExpression());
        addChild(node, match(TK_CL));
    } else if (getCurrentToken().type == TK_ID || getCurrentToken().type == TK_NUM || getCurrentToken().type == TK_RNUM) {
        addChild(node, var());
    } else {
        printf("Syntax error in factor\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* highPrecedenceOps() {
    ParseTreeNode *node = createNode(nt_highPrecedenceOps, "highPrecedenceOps");
    if (getCurrentToken().type == TK_MUL) {
        addChild(node, match(TK_MUL));
    } else if (getCurrentToken().type == TK_DIV) {
        addChild(node, match(TK_DIV));
    } else {
        printf("Syntax error in highPrecedenceOps\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* lowPrecedenceOps() {
    ParseTreeNode *node = createNode(nt_lowPrecedenceOps, "lowPrecedenceOps");
    if (getCurrentToken().type == TK_PLUS) {
        addChild(node, match(TK_PLUS));
    } else if (getCurrentToken().type == TK_MINUS) {
        addChild(node, match(TK_MINUS));
    } else {
        printf("Syntax error in lowPrecedenceOps\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* booleanExpression() {
    ParseTreeNode *node = createNode(nt_booleanExpression, "booleanExpression");
    if (getCurrentToken().type == TK_OP) {
        addChild(node, match(TK_OP));
        addChild(node, booleanExpression());
        addChild(node, match(TK_CL));
        addChild(node, logicalOp());
        addChild(node, match(TK_OP));
        addChild(node, booleanExpression());
        addChild(node, match(TK_CL));
        addChild(node, match(TK_SEM));
    } else if (getCurrentToken().type == TK_ID || getCurrentToken().type == TK_NUM || getCurrentToken().type == TK_RNUM) {
        addChild(node, var());
        addChild(node, relationalOp());
        addChild(node, var());
    } else if (getCurrentToken().type == TK_NOT) {
        addChild(node, match(TK_NOT));
        addChild(node, match(TK_OP));
        addChild(node, booleanExpression());
        addChild(node, match(TK_CL));
    } else {
        printf("Syntax error in booleanExpression\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* logicalOp() {
    ParseTreeNode *node = createNode(nt_logicalOp, "logicalOp");
    if (getCurrentToken().type == TK_AND) {
        addChild(node, match(TK_AND));
    } else if (getCurrentToken().type == TK_OR) {
        addChild(node, match(TK_OR));
    } else {
        printf("Syntax error in logicalOp\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* relationalOp() {
    ParseTreeNode *node = createNode(nt_relationalOp, "relationalOp");
    if (getCurrentToken().type == TK_LT) {
        addChild(node, match(TK_LT));
    } else if (getCurrentToken().type == TK_LE) {
        addChild(node, match(TK_LE));
    } else if (getCurrentToken().type == TK_EQ) {
        addChild(node, match(TK_EQ));
    } else if (getCurrentToken().type == TK_GT) {
        addChild(node, match(TK_GT));
    } else if (getCurrentToken().type == TK_GE) {
        addChild(node, match(TK_GE));
    } else if (getCurrentToken().type == TK_NE) {
        addChild(node, match(TK_NE));
    } else {
        printf("Syntax error in relationalOp\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* returnStmt() {
    ParseTreeNode *node = createNode(nt_returnStmt, "returnStmt");
    addChild(node, match(TK_RETURN));
    addChild(node, optionalReturn());
    addChild(node, match(TK_SEM));
    return node;
}

ParseTreeNode* optionalReturn() {
    ParseTreeNode *node = createNode(nt_optionalReturn, "optionalReturn");
    if (getCurrentToken().type == TK_SQL) {
        addChild(node, match(TK_SQL));
        addChild(node, idList());
        addChild(node, match(TK_SQR));
    }
    return node;
}

ParseTreeNode* idList() {
    ParseTreeNode *node = createNode(nt_idList, "idList");
    addChild(node, match(TK_ID));
    addChild(node, moreIds());
    return node;
}

ParseTreeNode* moreIds() {
    ParseTreeNode *node = createNode(nt_moreIds, "moreIds");
    if (getCurrentToken().type == TK_COMMA) {
        addChild(node, match(TK_COMMA));
        addChild(node, idList());
    }
    return node;
}

ParseTreeNode* definetypestmt() {
    ParseTreeNode *node = createNode(nt_definetypestmt, "definetypestmt");
    addChild(node, match(TK_DEFINETYPE));
    addChild(node, a());
    addChild(node, match(TK_RUID));
    addChild(node, match(TK_AS));
    addChild(node, match(TK_RUID));
    return node;
}

ParseTreeNode* a() {
    ParseTreeNode *node = createNode(nt_a, "a");
    if (getCurrentToken().type == TK_RECORD) {
        addChild(node, match(TK_RECORD));
    } else if (getCurrentToken().type == TK_UNION) {
        addChild(node, match(TK_UNION));
    } else {
        printf("Syntax error in a\n");
        exit(1);
    }
    return node;
}

void printParseTree(ParseTreeNode* node, int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; ++i) printf("  ");
    printf("%s\n", node->lexeme);
    for (int i = 0; i < node->childCount; ++i) {
        printParseTree(node->children[i], level + 1);
    }
}

int main() {
    // Example token stream
    Token exampleTokens[] = {
        {TK_MAIN, "_main"}, {TK_TYPE, "type"}, {TK_INT, "int"}, {TK_COLON, ":"},
        {TK_ID, "x"}, {TK_SEM, ";"},
        {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
    };
    // Token exampleTokens[] = {
    //     {TK_MAIN, "_main"}, 
    //     {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
    // };

    tokens = exampleTokens;
    tokenCount = sizeof(exampleTokens) / sizeof(exampleTokens[0]);

    ParseTreeNode *parseTree = program();
    printParseTree(parseTree, 0);

    printf("Parsing completed successfully.\n");
    return 0;
}


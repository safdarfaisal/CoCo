#include "parser.h"
// #include "lexer.h"

Token lexedTokens[5000];
int listSize = 0;
int currentTokenIndex = 0;
ParseTreeNode *parseTree;

Token initToken(){
    Token token = (Token)malloc(sizeof(struct Token));
    memset(token, 0, sizeof(struct Token));
    return token;
}

void setValue(Token token, char *value, int size){
    memset(token->value, 0, IDLENGTH*sizeof(char));
    memcpy(token->value, value, size*sizeof(char));
}

void setLineNum(Token token, int lineno){
    token->lineno = lineno;
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

void printParseTree(ParseTreeNode* node, int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; ++i) printf("  ");
    printf("%s\n", node->lexeme);
    for (int i = 0; i < node->childCount; ++i) {
        printParseTree(node->children[i], level + 1);
    }
}

// Function to create a new parse tree node
ParseTreeNode* createNode(Symbols symbol, const char *lexeme, int isTerminal) {
    ParseTreeNode* node = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
    node->symbol = symbol;
    node->isTerminal = isTerminal;
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



// Read the token file being passed to get the list of terminals

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
        char delimiter[] = "~";
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



// Token stream and current token index

Token getCurrentToken() {
    return lexedTokens[currentTokenIndex];
}

void advanceToken() {
    if (currentTokenIndex < listSize) {
        currentTokenIndex++;
    }
}

ParseTreeNode* match(Terminal expected) {
    Token currentToken = getCurrentToken();
    if (currentToken->terminal == expected) {
        Symbols symbolVal;
        symbolVal.t = currentToken->terminal;
        ParseTreeNode* node = createNode(symbolVal, currentToken->value, 1);
        advanceToken();
        return node;
    } else {
        printf("Syntax error: Expected %d but found %d\n", expected, currentToken->terminal);
        exit(1);
    }
}

// Function implementations

ParseTreeNode* program() {
    Symbols symbolVal;
    symbolVal.nt = nt_program;
    ParseTreeNode *node = createNode(symbolVal, "program", 0);
    addChild(node, otherFunctions());
    addChild(node, mainFunction());
    return node;
}

ParseTreeNode* mainFunction() {
    Symbols symbolVal;
    symbolVal.nt = nt_mainFunction;
    ParseTreeNode *node = createNode(symbolVal, "mainFunction", 0);
    addChild(node, match(TK_MAIN));
    addChild(node, stmts());
    addChild(node, match(TK_END));
    return node;
}

ParseTreeNode* otherFunctions() {
    Symbols symbolVal;
    symbolVal.nt = nt_otherFunctions;
    ParseTreeNode *node = createNode(symbolVal, "otherFunctions", 0);
    if (getCurrentToken()->terminal == TK_FUNID) {
        addChild(node, function());
        addChild(node, otherFunctions());
    }
    return node;
}

ParseTreeNode* function() {
    Symbols symbolVal;
    symbolVal.nt = nt_function;
    ParseTreeNode *node = createNode(symbolVal, "function", 0);
    addChild(node, match(TK_FUNID));
    addChild(node, input_par());
    addChild(node, output_par());
    addChild(node, match(TK_SEM));
    addChild(node, stmts());
    addChild(node, match(TK_END));
    return node;
}

ParseTreeNode* input_par() {
    Symbols symbolVal;
    symbolVal.nt = nt_input_par;
    ParseTreeNode *node = createNode(symbolVal, "input_par", 0);
    addChild(node, match(TK_INPUT));
    addChild(node, match(TK_PARAMETER));
    addChild(node, match(TK_LIST));
    addChild(node, match(TK_SQL));
    addChild(node, parameter_list());
    addChild(node, match(TK_SQR));
    return node;
}

ParseTreeNode* output_par() {
    Symbols symbolVal;
    symbolVal.nt = nt_output_par;
    ParseTreeNode *node = createNode(symbolVal, "output_par", 0);
    if (getCurrentToken()->terminal == TK_OUTPUT) {
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
    Symbols symbolVal;
    symbolVal.nt = nt_parameter_list;
    ParseTreeNode *node = createNode(symbolVal, "parameter_list", 0);
    addChild(node, dataType());
    addChild(node, match(TK_ID));
    addChild(node, remaining_list());
    return node;
}

ParseTreeNode* dataType() {
    Symbols symbolVal;
    symbolVal.nt = nt_dataType;
    ParseTreeNode *node = createNode(symbolVal, "dataType", 0);
    if (getCurrentToken()->terminal == TK_INT || getCurrentToken()->terminal == TK_REAL) {
        addChild(node, primitiveDatatype());
    } else if (getCurrentToken()->terminal == TK_RECORD || getCurrentToken()->terminal == TK_UNION || getCurrentToken()->terminal == TK_RUID) {
        addChild(node, constructedDatatype());
    } else {
        printf("Syntax error in dataType\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* primitiveDatatype() {
    Symbols symbolVal;
    symbolVal.nt = nt_primitiveDatatype;
    ParseTreeNode *node = createNode(symbolVal, "primitiveDatatype", 0);
    if (getCurrentToken()->terminal == TK_INT) {
        addChild(node, match(TK_INT));
    } else if (getCurrentToken()->terminal == TK_REAL) {
        addChild(node, match(TK_REAL));
    } else {
        printf("Syntax error in primitiveDatatype\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* constructedDatatype() {
    Symbols symbolVal;
    symbolVal.nt = nt_constructedDatatype;
    ParseTreeNode *node = createNode(symbolVal, "constructedDatatype", 0);
    if (getCurrentToken()->terminal == TK_RECORD) {
        addChild(node, match(TK_RECORD));
        addChild(node, match(TK_RUID));
    } else if (getCurrentToken()->terminal == TK_UNION) {
        addChild(node, match(TK_UNION));
        addChild(node, match(TK_RUID));
    } else if (getCurrentToken()->terminal == TK_RUID) {
        addChild(node, match(TK_RUID));
    } else {
        printf("Syntax error in constructedDatatype\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* remaining_list() {
    Symbols symbolVal;
    symbolVal.nt = nt_remaining_list;
    ParseTreeNode *node = createNode(symbolVal, "remaining_list", 0);
    if (getCurrentToken()->terminal == TK_COMMA) {
        addChild(node, match(TK_COMMA));
        addChild(node, parameter_list());
    }
    return node;
}

ParseTreeNode* stmts() {
    Symbols symbolVal;
    symbolVal.nt = nt_stmts;
    ParseTreeNode *node = createNode(symbolVal, "stmts", 0);
    addChild(node, typeDefinitions());
    addChild(node, declarations());
    addChild(node, otherstmts());
    addChild(node, returnStmt());
    return node;
}

ParseTreeNode* typeDefinitions() {
    Symbols symbolVal;
    symbolVal.nt = nt_typeDefinitions;
    ParseTreeNode *node = createNode(symbolVal, "typeDefinitions", 0);
    if (getCurrentToken()->terminal == TK_RECORD || getCurrentToken()->terminal == TK_UNION) {
        addChild(node, isRedefined());
        addChild(node, typeDefinition());
    }
    return node;
}

ParseTreeNode* isRedefined() {
    Symbols symbolVal;
    symbolVal.nt = nt_isRedefined;
    ParseTreeNode *node = createNode(symbolVal, "isRedefined", 0);
    if (getCurrentToken()->terminal == TK_RECORD || getCurrentToken()->terminal == TK_UNION) {
        addChild(node, typeDefinition());
    } else {
        addChild(node, definetypestmt());
    }
    return node;
}

ParseTreeNode* typeDefinition() {
    Symbols symbolVal;
    symbolVal.nt = nt_typeDefinition;
    ParseTreeNode *node = createNode(symbolVal, "typeDefinition", 0);
    if (getCurrentToken()->terminal == TK_RECORD) {
        addChild(node, match(TK_RECORD));
        addChild(node, match(TK_RUID));
        addChild(node, fieldDefinitions());
        addChild(node, match(TK_ENDRECORD));
        addChild(node, match(TK_SEM));
    } else if (getCurrentToken()->terminal == TK_UNION) {
        addChild(node, match(TK_UNION));
        addChild(node, match(TK_RUID));
        addChild(node, fieldDefinitions());
        addChild(node, match(TK_ENDUNION));
        addChild(node, match(TK_SEM));
    }
    return NULL;
}

ParseTreeNode* fieldDefinitions() {
    Symbols symbolVal;
    symbolVal.nt = nt_fieldDefinitions;
    ParseTreeNode *node = createNode(symbolVal, "fieldDefinitions", 0);
    addChild(node, fieldDefinition());
    addChild(node, moreFields());
    return node;
}

ParseTreeNode* fieldDefinition() {
    Symbols symbolVal;
    symbolVal.nt = nt_fieldDefinition;
    ParseTreeNode *node = createNode(symbolVal, "fieldDefinition", 0);
    addChild(node, match(TK_TYPE));
    addChild(node, dataType());
    addChild(node, match(TK_COLON));
    addChild(node, match(TK_FIELDID));
    addChild(node, match(TK_SEM));
    return node;
}

ParseTreeNode* moreFields() {
    Symbols symbolVal;
    symbolVal.nt = nt_moreFields;
    ParseTreeNode *node = createNode(symbolVal, "moreFields", 0);
    if (getCurrentToken()->terminal == TK_TYPE) {
        addChild(node, fieldDefinition());
        addChild(node, moreFields());
    }
    return node;
}

ParseTreeNode* declarations() {
    Symbols symbolVal;
    symbolVal.nt = nt_declarations;
    ParseTreeNode *node = createNode(symbolVal, "declarations", 0);
    if (getCurrentToken()->terminal == TK_TYPE) {
        addChild(node, declaration());
        addChild(node, declarations());
    }
    return node;
}

ParseTreeNode* declaration() {
    Symbols symbolVal;
    symbolVal.nt = nt_declaration;
    ParseTreeNode *node = createNode(symbolVal, "declaration", 0);
    addChild(node, match(TK_TYPE));
    addChild(node, dataType());
    addChild(node, match(TK_COLON));
    addChild(node, match(TK_ID));
    addChild(node, global_or_not());
    addChild(node, match(TK_SEM));
    return node;
}

ParseTreeNode* global_or_not() {
    Symbols symbolVal;
    symbolVal.nt = nt_global_or_not;
    ParseTreeNode *node = createNode(symbolVal, "global_or_not", 0);
    if (getCurrentToken()->terminal == TK_COLON) {
        addChild(node, match(TK_COLON));
        addChild(node, match(TK_GLOBAL));
    }
    return node;
}

ParseTreeNode* otherstmts() {
    Symbols symbolVal;
    symbolVal.nt = nt_otherstmts;
    ParseTreeNode *node = createNode(symbolVal, "otherstmts", 0);
    if (getCurrentToken()->terminal == TK_ID || getCurrentToken()->terminal == TK_WHILE || getCurrentToken()->terminal == TK_IF || getCurrentToken()->terminal == TK_READ || getCurrentToken()->terminal == TK_WRITE || getCurrentToken()->terminal == TK_CALL ||getCurrentToken()->terminal == TK_SQL) {
        addChild(node, stmt());
        addChild(node, otherstmts());
    }
    return node;
}

ParseTreeNode* stmt() {
    Symbols symbolVal;
    symbolVal.nt = nt_stmt;
    ParseTreeNode *node = createNode(symbolVal, "stmt", 0);
    if (getCurrentToken()->terminal == TK_ID) {
        addChild(node, assignmentStmt());
    } else if (getCurrentToken()->terminal == TK_WHILE) {
        addChild(node, iterativeStmt());
    } else if (getCurrentToken()->terminal == TK_IF) {
        addChild(node, conditionalStmt());
    } else if (getCurrentToken()->terminal == TK_READ || getCurrentToken()->terminal == TK_WRITE) {
        addChild(node, ioStmt());
    } else if (getCurrentToken()->terminal == TK_SQL || getCurrentToken()->terminal == TK_CALL) {
        addChild(node, funCallStmt());
    } else {
        printf("Syntax error in stmt\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* assignmentStmt() {
    Symbols symbolVal;
    symbolVal.nt = nt_assignmentStmt;
    ParseTreeNode *node = createNode(symbolVal, "assignmentStmt", 0);
    addChild(node, singleOrRecId());
    addChild(node, match(TK_ASSIGNOP));
    addChild(node, arithmeticExpression());
    addChild(node, match(TK_SEM));
    return node;
}

ParseTreeNode* singleOrRecId() {
    Symbols symbolVal;
    symbolVal.nt = nt_singleOrRecId;
    ParseTreeNode *node = createNode(symbolVal, "singleOrRecId", 0);
    addChild(node, match(TK_ID));
    addChild(node, singleLeft());
    return node;
}

ParseTreeNode* singleLeft() {
    Symbols symbolVal;
    symbolVal.nt = nt_singleLeft;
    ParseTreeNode *node = createNode(symbolVal, "singleLeft", 0);
    if (getCurrentToken()->terminal == TK_DOT) {
        addChild(node, oneExpansion());
        addChild(node, moreExpansions());
    }
    return node;
}

ParseTreeNode* oneExpansion() {
    Symbols symbolVal;
    symbolVal.nt = nt_oneExpansion;
    ParseTreeNode *node = createNode(symbolVal, "oneExpansion", 0);
    addChild(node, match(TK_DOT));
    addChild(node, match(TK_FIELDID));
    return node;
}

ParseTreeNode* moreExpansions() {
    Symbols symbolVal;
    symbolVal.nt = nt_moreExpansions;
    ParseTreeNode *node = createNode(symbolVal, "moreExpansions", 0);
    if (getCurrentToken()->terminal == TK_DOT) {
        addChild(node, oneExpansion());
        addChild(node, moreExpansions());
    }
    return node;
}

ParseTreeNode* funCallStmt() {
    Symbols symbolVal;
    symbolVal.nt = nt_funCallStmt;
    ParseTreeNode *node = createNode(symbolVal, "funCallStmt", 0);
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
    Symbols symbolVal;
    symbolVal.nt = nt_outputParameters;
    ParseTreeNode *node = createNode(symbolVal, "outputParameters", 0);
    if (getCurrentToken()->terminal == TK_SQL) {
        addChild(node, match(TK_SQL));
        addChild(node, idList());
        addChild(node, match(TK_SQR));
        addChild(node, match(TK_ASSIGNOP));
    }
    return node;
}

ParseTreeNode* inputParameters() {
    Symbols symbolVal;
    symbolVal.nt = nt_inputParameters;
    ParseTreeNode *node = createNode(symbolVal, "inputParameters", 0);
    addChild(node, match(TK_SQL));
    addChild(node, idList());
    addChild(node, match(TK_SQR));
    return node;
}

ParseTreeNode* iterativeStmt() {
    Symbols symbolVal;
    symbolVal.nt = nt_iterativeStmt;
    ParseTreeNode *node = createNode(symbolVal, "iterativeStmt", 0);
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
    Symbols symbolVal;
    symbolVal.nt = nt_conditionalStmt;
    ParseTreeNode *node = createNode(symbolVal, "conditionalStmt", 0);
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
    Symbols symbolVal;
    symbolVal.nt = nt_elseStmt;
    ParseTreeNode *node = createNode(symbolVal, "elseStmt", 0);
    if (getCurrentToken()->terminal == TK_ELSE) {
        addChild(node, match(TK_ELSE));
        addChild(node, otherstmts());
        addChild(node, match(TK_ENDIF));
    } else if (getCurrentToken()->terminal == TK_ENDIF) {
        addChild(node, match(TK_ENDIF));
    } else {
        printf("Syntax error in elseStmt\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* var() {
    Symbols symbolVal;
    symbolVal.nt = nt_var;
    ParseTreeNode *node = createNode(symbolVal, "var", 0);
    if (getCurrentToken()->terminal == TK_ID) {
        addChild(node, singleOrRecId());
    } else if (getCurrentToken()->terminal == TK_NUM) {
        addChild(node, match(TK_NUM));
    } else if (getCurrentToken()->terminal == TK_RNUM) {
        addChild(node, match(TK_RNUM));
    }
    return node;
}

ParseTreeNode* ioStmt() {
    Symbols symbolVal;
    symbolVal.nt = nt_ioStmt;
    ParseTreeNode *node = createNode(symbolVal, "ioStmt", 0);
    if (getCurrentToken()->terminal == TK_READ) {
        addChild(node, match(TK_READ));
        addChild(node, match(TK_OP));
        addChild(node, var());
        addChild(node, match(TK_CL));
        addChild(node, match(TK_SEM));
    } else if (getCurrentToken()->terminal == TK_WRITE) {
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
    Symbols symbolVal;
    symbolVal.nt = nt_arithmeticExpression;
    ParseTreeNode *node = createNode(symbolVal, "arithmeticExpression", 0);
    addChild(node, term());
    addChild(node, expPrime());
    return node;
}

ParseTreeNode* expPrime() {
    Symbols symbolVal;
    symbolVal.nt = nt_expPrime;
    ParseTreeNode *node = createNode(symbolVal, "expPrime", 0);
    if (getCurrentToken()->terminal == TK_PLUS || getCurrentToken()->terminal == TK_MINUS) {
        addChild(node, lowPrecedenceOps());
        addChild(node, term());
        addChild(node, expPrime());
    }
    return node;
}

ParseTreeNode* term() {
    Symbols symbolVal;
    symbolVal.nt = nt_term;
    ParseTreeNode *node = createNode(symbolVal, "term", 0);
    addChild(node, factor());
    addChild(node, termPrime());
    return node;
}

ParseTreeNode* termPrime() {
    Symbols symbolVal;
    symbolVal.nt = nt_termPrime;
    ParseTreeNode *node = createNode(symbolVal, "termPrime", 0);
    if (getCurrentToken()->terminal == TK_MUL || getCurrentToken()->terminal == TK_DIV) {
        addChild(node, highPrecedenceOps());
        addChild(node, factor());
        addChild(node, termPrime());
    }
    return node;
}

ParseTreeNode* factor() {
    Symbols symbolVal;
    symbolVal.nt = nt_factor;
    ParseTreeNode *node = createNode(symbolVal, "factor", 0);
    if (getCurrentToken()->terminal == TK_OP) {
        addChild(node, match(TK_OP));
        addChild(node, arithmeticExpression());
        addChild(node, match(TK_CL));
    } else if (getCurrentToken()->terminal == TK_ID || getCurrentToken()->terminal == TK_NUM || getCurrentToken()->terminal == TK_RNUM) {
        addChild(node, var());
    } else {
        printf("Syntax error in factor on line no: %d\n", getCurrentToken()->terminal);
        printParseTree(parseTree, 0);
        exit(1);
    }
    return node;
}

ParseTreeNode* highPrecedenceOps() {
    Symbols symbolVal;
    symbolVal.nt = nt_highPrecedenceOps;
    ParseTreeNode *node = createNode(symbolVal, "highPrecedenceOps", 0);
    if (getCurrentToken()->terminal == TK_MUL) {
        addChild(node, match(TK_MUL));
    } else if (getCurrentToken()->terminal == TK_DIV) {
        addChild(node, match(TK_DIV));
    } else {
        printf("Syntax error in highPrecedenceOps\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* lowPrecedenceOps() {
    Symbols symbolVal;
    symbolVal.nt = nt_lowPrecedenceOps;
    ParseTreeNode *node = createNode(symbolVal, "lowPrecedenceOps", 0);
    if (getCurrentToken()->terminal == TK_PLUS) {
        addChild(node, match(TK_PLUS));
    } else if (getCurrentToken()->terminal == TK_MINUS) {
        addChild(node, match(TK_MINUS));
    } else {
        printf("Syntax error in lowPrecedenceOps\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* booleanExpression() {
    Symbols symbolVal;
    symbolVal.nt = nt_booleanExpression;
    ParseTreeNode *node = createNode(symbolVal, "booleanExpression", 0);
    if (getCurrentToken()->terminal == TK_OP) {
        addChild(node, match(TK_OP));
        addChild(node, booleanExpression());
        addChild(node, match(TK_CL));
        addChild(node, logicalOp());
        addChild(node, match(TK_OP));
        addChild(node, booleanExpression());
        addChild(node, match(TK_CL));
        addChild(node, match(TK_SEM));
    } else if (getCurrentToken()->terminal == TK_ID || getCurrentToken()->terminal == TK_NUM || getCurrentToken()->terminal == TK_RNUM) {
        addChild(node, var());
        addChild(node, relationalOp());
        addChild(node, var());
    } else if (getCurrentToken()->terminal == TK_NOT) {
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
    Symbols symbolVal;
    symbolVal.nt = nt_logicalOp;
    ParseTreeNode *node = createNode(symbolVal, "logicalOp", 0);
    if (getCurrentToken()->terminal == TK_AND) {
        addChild(node, match(TK_AND));
    } else if (getCurrentToken()->terminal == TK_OR) {
        addChild(node, match(TK_OR));
    } else {
        printf("Syntax error in logicalOp\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* relationalOp() {
    Symbols symbolVal;
    symbolVal.nt = nt_relationalOp;
    ParseTreeNode *node = createNode(symbolVal, "relationalOp", 0);
    if (getCurrentToken()->terminal == TK_LT) {
        addChild(node, match(TK_LT));
    } else if (getCurrentToken()->terminal == TK_LE) {
        addChild(node, match(TK_LE));
    } else if (getCurrentToken()->terminal == TK_EQ) {
        addChild(node, match(TK_EQ));
    } else if (getCurrentToken()->terminal == TK_GT) {
        addChild(node, match(TK_GT));
    } else if (getCurrentToken()->terminal == TK_GE) {
        addChild(node, match(TK_GE));
    } else if (getCurrentToken()->terminal == TK_NE) {
        addChild(node, match(TK_NE));
    } else {
        printf("Syntax error in relationalOp\n");
        exit(1);
    }
    return node;
}

ParseTreeNode* returnStmt() {
    Symbols symbolVal;
    symbolVal.nt = nt_returnStmt;
    ParseTreeNode *node = createNode(symbolVal, "returnStmt", 0);
    addChild(node, match(TK_RETURN));
    addChild(node, optionalReturn());
    addChild(node, match(TK_SEM));
    return node;
}

ParseTreeNode* optionalReturn() {
    Symbols symbolVal;
    symbolVal.nt = nt_optionalReturn;
    ParseTreeNode *node = createNode(symbolVal, "optionalReturn", 0);
    if (getCurrentToken()->terminal == TK_SQL) {
        addChild(node, match(TK_SQL));
        addChild(node, idList());
        addChild(node, match(TK_SQR));
    }
    return node;
}

ParseTreeNode* idList() {
    Symbols symbolVal;
    symbolVal.nt = nt_idList;
    ParseTreeNode *node = createNode(symbolVal, "idList", 0);
    addChild(node, match(TK_ID));
    addChild(node, moreIds());
    return node;
}

ParseTreeNode* moreIds() {
    Symbols symbolVal;
    symbolVal.nt = nt_moreIds;
    ParseTreeNode *node = createNode(symbolVal, "moreIds", 0);
    if (getCurrentToken()->terminal == TK_COMMA) {
        addChild(node, match(TK_COMMA));
        addChild(node, idList());
    }
    return node;
}

ParseTreeNode* definetypestmt() {
    Symbols symbolVal;
    symbolVal.nt = nt_definetypestmt;
    ParseTreeNode *node = createNode(symbolVal, "definetypestmt", 0);
    addChild(node, match(TK_DEFINETYPE));
    addChild(node, a());
    addChild(node, match(TK_RUID));
    addChild(node, match(TK_AS));
    addChild(node, match(TK_RUID));
    return node;
}

ParseTreeNode* a() {
    Symbols symbolVal;
    symbolVal.nt = nt_a;
    ParseTreeNode *node = createNode(symbolVal, "a", 0);
    if (getCurrentToken()->terminal == TK_RECORD) {
        addChild(node, match(TK_RECORD));
    } else if (getCurrentToken()->terminal == TK_UNION) {
        addChild(node, match(TK_UNION));
    } else {
        printf("Syntax error in a\n");
        exit(1);
    }
    return node;
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
    printParseTree(parseTree, 0);
    printf("Parsing completed succesfully\n");
    return 0;
}


// ParseTreeNode* program() {
//     Symbols symbolVal;
//     symbolVal.nt = nt_program;
//     ParseTreeNode *node = createNode(symbolVal, "program", 0);
//     addChild(node, otherFunctions());
//     addChild(node, mainFunction());
//     return node;
// }

// ParseTreeNode* mainFunction() {
//     ParseTreeNode *node = createNode(nt_mainFunction, "mainFunction");
//     addChild(node, match(TK_MAIN));
//     addChild(node, stmts());
//     addChild(node, match(TK_END));
//     return node;
// }

// ParseTreeNode* otherFunctions() {
//     ParseTreeNode *node = createNode(nt_otherFunctions, "otherFunctions");
//     if (getCurrentToken()->terminal == TK_FUNID) {
//         addChild(node, function());
//         addChild(node, otherFunctions());
//     }
//     return node;
// }

// ParseTreeNode* function() {
//     ParseTreeNode *node = createNode(nt_function, "function");
//     addChild(node, match(TK_FUNID));
//     addChild(node, input_par());
//     addChild(node, output_par());
//     addChild(node, match(TK_SEM));
//     addChild(node, stmts());
//     addChild(node, match(TK_END));
//     return node;
// }

// ParseTreeNode* input_par() {
//     ParseTreeNode *node = createNode(nt_input_par, "input_par");
//     addChild(node, match(TK_INPUT));
//     addChild(node, match(TK_PARAMETER));
//     addChild(node, match(TK_LIST));
//     addChild(node, match(TK_SQL));
//     addChild(node, parameter_list());
//     addChild(node, match(TK_SQR));
//     return node;
// }

// ParseTreeNode* output_par() {
//     ParseTreeNode *node = createNode(nt_output_par, "output_par");
//     if (getCurrentToken()->terminal == TK_OUTPUT) {
//         addChild(node, match(TK_OUTPUT));
//         addChild(node, match(TK_PARAMETER));
//         addChild(node, match(TK_LIST));
//         addChild(node, match(TK_SQL));
//         addChild(node, parameter_list());
//         addChild(node, match(TK_SQR));
//     }
//     return node;
// }

// ParseTreeNode* parameter_list() {
//     ParseTreeNode *node = createNode(nt_parameter_list, "parameter_list");
//     addChild(node, dataType());
//     addChild(node, match(TK_ID));
//     addChild(node, remaining_list());
//     return node;
// }

// ParseTreeNode* dataType() {
//     ParseTreeNode *node = createNode(nt_dataType, "dataType");
//     if (getCurrentToken()->terminal == TK_INT || getCurrentToken()->terminal == TK_REAL) {
//         addChild(node, primitiveDatatype());
//     } else if (getCurrentToken()->terminal == TK_RECORD || getCurrentToken()->terminal == TK_UNION || getCurrentToken()->terminal == TK_RUID) {
//         addChild(node, constructedDatatype());
//     } else {
//         printf("Syntax error in dataType\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* primitiveDatatype() {
//     ParseTreeNode *node = createNode(nt_primitiveDatatype, "primitiveDatatype");
//     if (getCurrentToken()->terminal == TK_INT) {
//         addChild(node, match(TK_INT));
//     } else if (getCurrentToken()->terminal == TK_REAL) {
//         addChild(node, match(TK_REAL));
//     } else {
//         printf("Syntax error in primitiveDatatype\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* constructedDatatype() {
//     ParseTreeNode *node = createNode(nt_constructedDatatype, "constructedDatatype");
//     if (getCurrentToken()->terminal == TK_RECORD) {
//         addChild(node, match(TK_RECORD));
//         addChild(node, match(TK_RUID));
//     } else if (getCurrentToken()->terminal == TK_UNION) {
//         addChild(node, match(TK_UNION));
//         addChild(node, match(TK_RUID));
//     } else if (getCurrentToken()->terminal == TK_RUID) {
//         addChild(node, match(TK_RUID));
//     } else {
//         printf("Syntax error in constructedDatatype\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* remaining_list() {
//     ParseTreeNode *node = createNode(nt_remaining_list, "remaining_list");
//     if (getCurrentToken()->terminal == TK_COMMA) {
//         addChild(node, match(TK_COMMA));
//         addChild(node, parameter_list());
//     }
//     return node;
// }

// ParseTreeNode* stmts() {
//     ParseTreeNode *node = createNode(nt_stmts, "stmts");
//     addChild(node, typeDefinitions());
//     addChild(node, declarations());
//     addChild(node, otherstmts());
//     addChild(node, returnStmt());
//     return node;
// }

// ParseTreeNode* typeDefinitions() {
//     ParseTreeNode *node = createNode(nt_typeDefinitions, "typeDefinitions");
//     if (getCurrentToken()->terminal == TK_RECORD || getCurrentToken()->terminal == TK_UNION) {
//         addChild(node, isRedefined());
//         addChild(node, typeDefinition());
//     }
//     return node;
// }

// ParseTreeNode* isRedefined() {
//     ParseTreeNode *node = createNode(nt_isRedefined, "isRedefined");
//     if (getCurrentToken()->terminal == TK_RECORD || getCurrentToken()->terminal == TK_UNION) {
//         addChild(node, typeDefinition());
//     } else {
//         addChild(node, definetypestmt());
//     }
//     return node;
// }

// ParseTreeNode* typeDefinition() {
//     ParseTreeNode *node = createNode(nt_typeDefinition, "typeDefinition");
//     if (getCurrentToken()->terminal == TK_RECORD) {
//         addChild(node, match(TK_RECORD));
//         addChild(node, match(TK_RUID));
//         addChild(node, fieldDefinitions());
//         addChild(node, match(TK_ENDRECORD));
//     } else if (getCurrentToken()->terminal == TK_UNION) {
//         addChild(node, match(TK_UNION));
//         addChild(node, match(TK_RUID));
//         addChild(node, fieldDefinitions());
//         addChild(node, match(TK_ENDUNION));
//     } else {
//         printf("Syntax error in typeDefinition\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* fieldDefinitions() {
//     ParseTreeNode *node = createNode(nt_fieldDefinitions, "fieldDefinitions");
//     addChild(node, fieldDefinition());
//     addChild(node, moreFields());
//     return node;
// }

// ParseTreeNode* fieldDefinition() {
//     ParseTreeNode *node = createNode(nt_fieldDefinition, "fieldDefinition");
//     addChild(node, match(TK_TYPE));
//     addChild(node, dataType());
//     addChild(node, match(TK_COLON));
//     addChild(node, match(TK_FIELDID));
//     addChild(node, match(TK_SEM));
//     return node;
// }

// ParseTreeNode* moreFields() {
//     ParseTreeNode *node = createNode(nt_moreFields, "moreFields");
//     if (getCurrentToken()->terminal == TK_TYPE) {
//         addChild(node, fieldDefinition());
//         addChild(node, moreFields());
//     }
//     return node;
// }

// ParseTreeNode* declarations() {
//     ParseTreeNode *node = createNode(nt_declarations, "declarations");
//     if (getCurrentToken()->terminal == TK_TYPE) {
//         addChild(node, declaration());
//         addChild(node, declarations());
//     }
//     return node;
// }

// ParseTreeNode* declaration() {
//     ParseTreeNode *node = createNode(nt_declaration, "declaration");
//     addChild(node, match(TK_TYPE));
//     addChild(node, dataType());
//     addChild(node, match(TK_COLON));
//     addChild(node, match(TK_ID));
//     addChild(node, global_or_not());
//     addChild(node, match(TK_SEM));
//     return node;
// }

// ParseTreeNode* global_or_not() {
//     ParseTreeNode *node = createNode(nt_global_or_not, "global_or_not");
//     if (getCurrentToken()->terminal == TK_COLON) {
//         addChild(node, match(TK_COLON));
//         addChild(node, match(TK_GLOBAL));
//     }
//     return node;
// }

// ParseTreeNode* otherstmts() {
//     ParseTreeNode *node = createNode(nt_otherstmts, "otherstmts");
//     if (getCurrentToken()->terminal == TK_ID || getCurrentToken()->terminal == TK_WHILE || getCurrentToken()->terminal == TK_IF || getCurrentToken()->terminal == TK_READ || getCurrentToken()->terminal == TK_WRITE || getCurrentToken()->terminal == TK_CALL) {
//         addChild(node, stmt());
//         addChild(node, otherstmts());
//     }
//     return node;
// }

// ParseTreeNode* stmt() {
//     ParseTreeNode *node = createNode(nt_stmt, "stmt");
//     if (getCurrentToken()->terminal == TK_ID) {
//         addChild(node, assignmentStmt());
//     } else if (getCurrentToken()->terminal == TK_WHILE) {
//         addChild(node, iterativeStmt());
//     } else if (getCurrentToken()->terminal == TK_IF) {
//         addChild(node, conditionalStmt());
//     } else if (getCurrentToken()->terminal == TK_READ || getCurrentToken()->terminal == TK_WRITE) {
//         addChild(node, ioStmt());
//     } else if (getCurrentToken()->terminal == TK_CALL) {
//         addChild(node, funCallStmt());
//     } else {
//         printf("Syntax error in stmt\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* assignmentStmt() {
//     ParseTreeNode *node = createNode(nt_assignmentStmt, "assignmentStmt");
//     addChild(node, singleOrRecId());
//     addChild(node, match(TK_ASSIGNOP));
//     addChild(node, arithmeticExpression());
//     addChild(node, match(TK_SEM));
//     return node;
// }

// ParseTreeNode* singleOrRecId() {
//     ParseTreeNode *node = createNode(nt_singleOrRecId, "singleOrRecId");
//     addChild(node, match(TK_ID));
//     addChild(node, singleLeft());
//     return node;
// }

// ParseTreeNode* singleLeft() {
//     ParseTreeNode *node = createNode(nt_singleLeft, "singleLeft");
//     if (getCurrentToken()->terminal == TK_ID) {
//         addChild(node, match(TK_ID));
//         addChild(node, oneExpansion());
//         addChild(node, moreExpansions());
//     }
//     return node;
// }

// ParseTreeNode* oneExpansion() {
//     ParseTreeNode *node = createNode(nt_oneExpansion, "oneExpansion");
//     addChild(node, match(TK_DOT));
//     addChild(node, match(TK_FIELDID));
//     return node;
// }

// ParseTreeNode* moreExpansions() {
//     ParseTreeNode *node = createNode(nt_moreExpansions, "moreExpansions");
//     if (getCurrentToken()->terminal == TK_DOT) {
//         addChild(node, oneExpansion());
//         addChild(node, moreExpansions());
//     }
//     return node;
// }

// ParseTreeNode* funCallStmt() {
//     ParseTreeNode *node = createNode(nt_funCallStmt, "funCallStmt");
//     addChild(node, outputParameters());
//     addChild(node, match(TK_CALL));
//     addChild(node, match(TK_FUNID));
//     addChild(node, match(TK_WITH));
//     addChild(node, match(TK_PARAMETERS));
//     addChild(node, inputParameters());
//     addChild(node, match(TK_SEM));
//     return node;
// }

// ParseTreeNode* outputParameters() {
//     ParseTreeNode *node = createNode(nt_outputParameters, "outputParameters");
//     if (getCurrentToken()->terminal == TK_SQL) {
//         addChild(node, match(TK_SQL));
//         addChild(node, idList());
//         addChild(node, match(TK_SQR));
//         addChild(node, match(TK_ASSIGNOP));
//     }
//     return node;
// }

// ParseTreeNode* inputParameters() {
//     ParseTreeNode *node = createNode(nt_inputParameters, "inputParameters");
//     addChild(node, match(TK_SQL));
//     addChild(node, idList());
//     addChild(node, match(TK_SQR));
//     return node;
// }

// ParseTreeNode* iterativeStmt() {
//     ParseTreeNode *node = createNode(nt_iterativeStmt, "iterativeStmt");
//     addChild(node, match(TK_WHILE));
//     addChild(node, match(TK_OP));
//     addChild(node, booleanExpression());
//     addChild(node, match(TK_CL));
//     addChild(node, stmt());
//     addChild(node, otherstmts());
//     addChild(node, match(TK_ENDWHILE));
//     return node;
// }

// ParseTreeNode* conditionalStmt() {
//     ParseTreeNode *node = createNode(nt_conditionalStmt, "conditionalStmt");
//     addChild(node, match(TK_IF));
//     addChild(node, match(TK_OP));
//     addChild(node, booleanExpression());
//     addChild(node, match(TK_CL));
//     addChild(node, match(TK_THEN));
//     addChild(node, stmt());
//     addChild(node, otherstmts());
//     addChild(node, elseStmt());
//     return node;
// }

// ParseTreeNode* elseStmt() {
//     ParseTreeNode *node = createNode(nt_elseStmt, "elseStmt");
//     if (getCurrentToken()->terminal == TK_ELSE) {
//         addChild(node, match(TK_ELSE));
//         addChild(node, otherstmts());
//         addChild(node, match(TK_ENDIF));
//     } else if (getCurrentToken()->terminal == TK_ENDIF) {
//         addChild(node, match(TK_ENDIF));
//     } else {
//         printf("Syntax error in elseStmt\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* var() {
//     ParseTreeNode *node = createNode(nt_var, "var");
//     if (getCurrentToken()->terminal == TK_ID) {
//         addChild(node, singleOrRecId());
//     } else if (getCurrentToken()->terminal == TK_NUM) {
//         addChild(node, match(TK_NUM));
//     } else if (getCurrentToken()->terminal == TK_RNUM) {
//         addChild(node, match(TK_RNUM));
//     }
//     return node;
// }

// ParseTreeNode* ioStmt() {
//     ParseTreeNode *node = createNode(nt_ioStmt, "ioStmt");
//     if (getCurrentToken()->terminal == TK_READ) {
//         addChild(node, match(TK_READ));
//         addChild(node, match(TK_OP));
//         addChild(node, var());
//         addChild(node, match(TK_CL));
//         addChild(node, match(TK_SEM));
//     } else if (getCurrentToken()->terminal == TK_WRITE) {
//         addChild(node, match(TK_WRITE));
//         addChild(node, match(TK_OP));
//         addChild(node, var());
//         addChild(node, match(TK_CL));
//         addChild(node, match(TK_SEM));
//     } else {
//         printf("Syntax error in ioStmt\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* arithmeticExpression() {
//     ParseTreeNode *node = createNode(nt_arithmeticExpression, "arithmeticExpression");
//     addChild(node, term());
//     addChild(node, expPrime());
//     return node;
// }

// ParseTreeNode* expPrime() {
//     ParseTreeNode *node = createNode(nt_expPrime, "expPrime");
//     if (getCurrentToken()->terminal == TK_PLUS || getCurrentToken()->terminal == TK_MINUS) {
//         addChild(node, lowPrecedenceOps());
//         addChild(node, term());
//         addChild(node, expPrime());
//     }
//     return node;
// }

// ParseTreeNode* term() {
//     ParseTreeNode *node = createNode(nt_term, "term");
//     addChild(node, factor());
//     addChild(node, termPrime());
//     return node;
// }

// ParseTreeNode* termPrime() {
//     ParseTreeNode *node = createNode(nt_termPrime, "termPrime");
//     if (getCurrentToken()->terminal == TK_MUL || getCurrentToken()->terminal == TK_DIV) {
//         addChild(node, highPrecedenceOps());
//         addChild(node, factor());
//         addChild(node, termPrime());
//     }
//     return node;
// }

// ParseTreeNode* factor() {
//     ParseTreeNode *node = createNode(nt_factor, "factor");
//     if (getCurrentToken()->terminal == TK_OP) {
//         addChild(node, match(TK_OP));
//         addChild(node, arithmeticExpression());
//         addChild(node, match(TK_CL));
//     } else if (getCurrentToken()->terminal == TK_ID || getCurrentToken()->terminal == TK_NUM || getCurrentToken()->terminal == TK_RNUM) {
//         addChild(node, var());
//     } else {
//         printf("Syntax error in factor\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* highPrecedenceOps() {
//     ParseTreeNode *node = createNode(nt_highPrecedenceOps, "highPrecedenceOps");
//     if (getCurrentToken()->terminal == TK_MUL) {
//         addChild(node, match(TK_MUL));
//     } else if (getCurrentToken()->terminal == TK_DIV) {
//         addChild(node, match(TK_DIV));
//     } else {
//         printf("Syntax error in highPrecedenceOps\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* lowPrecedenceOps() {
//     ParseTreeNode *node = createNode(nt_lowPrecedenceOps, "lowPrecedenceOps");
//     if (getCurrentToken()->terminal == TK_PLUS) {
//         addChild(node, match(TK_PLUS));
//     } else if (getCurrentToken()->terminal == TK_MINUS) {
//         addChild(node, match(TK_MINUS));
//     } else {
//         printf("Syntax error in lowPrecedenceOps\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* booleanExpression() {
//     ParseTreeNode *node = createNode(nt_booleanExpression, "booleanExpression");
//     if (getCurrentToken()->terminal == TK_OP) {
//         addChild(node, match(TK_OP));
//         addChild(node, booleanExpression());
//         addChild(node, match(TK_CL));
//         addChild(node, logicalOp());
//         addChild(node, match(TK_OP));
//         addChild(node, booleanExpression());
//         addChild(node, match(TK_CL));
//         addChild(node, match(TK_SEM));
//     } else if (getCurrentToken()->terminal == TK_ID || getCurrentToken()->terminal == TK_NUM || getCurrentToken()->terminal == TK_RNUM) {
//         addChild(node, var());
//         addChild(node, relationalOp());
//         addChild(node, var());
//     } else if (getCurrentToken()->terminal == TK_NOT) {
//         addChild(node, match(TK_NOT));
//         addChild(node, match(TK_OP));
//         addChild(node, booleanExpression());
//         addChild(node, match(TK_CL));
//     } else {
//         printf("Syntax error in booleanExpression\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* logicalOp() {
//     ParseTreeNode *node = createNode(nt_logicalOp, "logicalOp");
//     if (getCurrentToken()->terminal == TK_AND) {
//         addChild(node, match(TK_AND));
//     } else if (getCurrentToken()->terminal == TK_OR) {
//         addChild(node, match(TK_OR));
//     } else {
//         printf("Syntax error in logicalOp\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* relationalOp() {
//     ParseTreeNode *node = createNode(nt_relationalOp, "relationalOp");
//     if (getCurrentToken()->terminal == TK_LT) {
//         addChild(node, match(TK_LT));
//     } else if (getCurrentToken()->terminal == TK_LE) {
//         addChild(node, match(TK_LE));
//     } else if (getCurrentToken()->terminal == TK_EQ) {
//         addChild(node, match(TK_EQ));
//     } else if (getCurrentToken()->terminal == TK_GT) {
//         addChild(node, match(TK_GT));
//     } else if (getCurrentToken()->terminal == TK_GE) {
//         addChild(node, match(TK_GE));
//     } else if (getCurrentToken()->terminal == TK_NE) {
//         addChild(node, match(TK_NE));
//     } else {
//         printf("Syntax error in relationalOp\n");
//         exit(1);
//     }
//     return node;
// }

// ParseTreeNode* returnStmt() {
//     ParseTreeNode *node = createNode(nt_returnStmt, "returnStmt");
//     addChild(node, match(TK_RETURN));
//     addChild(node, optionalReturn());
//     addChild(node, match(TK_SEM));
//     return node;
// }

// ParseTreeNode* optionalReturn() {
//     ParseTreeNode *node = createNode(nt_optionalReturn, "optionalReturn");
//     if (getCurrentToken()->terminal == TK_SQL) {
//         addChild(node, match(TK_SQL));
//         addChild(node, idList());
//         addChild(node, match(TK_SQR));
//     }
//     return node;
// }

// ParseTreeNode* idList() {
//     ParseTreeNode *node = createNode(nt_idList, "idList");
//     addChild(node, match(TK_ID));
//     addChild(node, moreIds());
//     return node;
// }

// ParseTreeNode* moreIds() {
//     ParseTreeNode *node = createNode(nt_moreIds, "moreIds");
//     if (getCurrentToken()->terminal == TK_COMMA) {
//         addChild(node, match(TK_COMMA));
//         addChild(node, idList());
//     }
//     return node;
// }

// ParseTreeNode* definetypestmt() {
//     ParseTreeNode *node = createNode(nt_definetypestmt, "definetypestmt");
//     addChild(node, match(TK_DEFINETYPE));
//     addChild(node, a());
//     addChild(node, match(TK_RUID));
//     addChild(node, match(TK_AS));
//     addChild(node, match(TK_RUID));
//     return node;
// }

// ParseTreeNode* a() {
//     ParseTreeNode *node = createNode(nt_a, "a");
//     if (getCurrentToken()->terminal == TK_RECORD) {
//         addChild(node, match(TK_RECORD));
//     } else if (getCurrentToken()->terminal == TK_UNION) {
//         addChild(node, match(TK_UNION));
//     } else {
//         printf("Syntax error in a\n");
//         exit(1);
//     }
//     return node;
// }



// int main() {
//     // Example token stream
//     Token exampleTokens[] = {
//         {TK_MAIN, "_main"}, {TK_TYPE, "type"}, {TK_INT, "int"}, {TK_COLON, ":"},
//         {TK_ID, "x"}, {TK_SEM, ";"},
//         {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
//     };
//     // Token exampleTokens[] = {
//     //     {TK_MAIN, "_main"}, 
//     //     {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
//     // };

//     tokens = exampleTokens;
//     tokenCount = sizeof(exampleTokens) / sizeof(exampleTokens[0]);

//     ParseTreeNode *parseTree = program();
//     printParseTree(parseTree, 0);

//     printf("Parsing completed successfully.\n");
//     return 0;
// }


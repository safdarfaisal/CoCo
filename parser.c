#ifndef PARSER_H  
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper/definitions.h"
// #include "datastructures/tree.h"
// #include "lexer.h"

#define MAXPRODLEN 8 
#define MAXPRODCOUNT 100
// #define MAXUNIQUE 6

// Token structure
typedef struct Token{
    Symbols type;
    char lexeme[100];
} Token;

// typedef struct ProdRules{
//     Symbols LHS;
//     Symbols RHS[MAXPRODLEN];
//     int RHScount;
//     int numOfChildren;
// } ProdRules;


// Symbols stringToSymbols(char string[]){
//     for(int i = 0; i < 111; i++){
//         if(!strcmp(string, StringToSymbols[i].string)){
//             return StringToSymbols[i].symbol;
//         }
//     }
//     return 0;
// }

// const char *symbolToString(Symbols symbol){
//     for (int i = 0; i < 111; i++){
//         if(StringToSymbols[i].symbol == symbol){
//             return StringToSymbols[i].string;
//         }
//     }
//     return "";
// }

// ProdRules *getProductions(){
//     FILE *fp = fopen("grammar.txt", "r");
//     ProdRules *rules = (ProdRules *)(malloc(sizeof(ProdRules)*91));
//     memset(rules, 0, sizeof(ProdRules)*91);
//     char string[30];
//     char c;
//     int i = 0;
//     while(!feof(fp)){
//         fscanf(fp, "%29s", string);
//         rules[i].LHS = stringToSymbols(string);
//         // printf("%s ", string);
//         c = fgetc(fp);
//         int j = 0;
//         while(c != '\n'){
//             fscanf(fp, "%29s", string);
//             rules[i].RHS[j++] = stringToSymbols(string);
//             // printf("%s ", string);
//             if(feof(fp)){
//                 break;
//             }
//             c = fgetc(fp);
//         }
//         rules[i].RHScount = j;
//         i++;
//         // printf("\n");
//     }
//     // return 0;
//     // for(int x = 0; x < 91; x++){
//     //     printf("%s ", symbolToString(rules[x].LHS));
//     //     for(int j = 0; j < rules[x].RHScount; j++){
//     //         printf("%s ", symbolToString(rules[x].RHS[j]));
//     //     }
//     //     printf("\n");
//     // }
//     return rules;
// }

// Token *getNextToken(int lexPointer){
//     return lexedData[lexPointer];
// }

// typedef struct TokenNode{
//     Symbols *symbol;
//     int lineno;
// } TokenNode;

// TokenNode currentToken;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




// Forward declarations
void program();
void mainFunction();
void otherFunctions();
void function();
void input_par();
void output_par();
void parameter_list();
void dataType();
void primitiveDatatype();
void constructedDatatype();
void remaining_list();
void stmts();
void typeDefinitions();
void isRedefined();
void typeDefinition();
void fieldDefinitions();
void fieldDefinition();
void moreFields();
void declarations();
void declaration();
void global_or_not();
void otherstmts();
void stmt();
void assignmentStmt();
void singleOrRecId();
void singleLeft();
void oneExpansion();
void moreExpansions();
void funCallStmt();
void outputParameters();
void inputParameters();
void iterativeStmt();
void conditionalStmt();
void elseStmt();
void ioStmt();
void arithmeticExpression();
void expPrime();
void term();
void termPrime();
void var();
void factor();
void highPrecedenceOps();
void lowPrecedenceOps();
void booleanExpression();
void logicalOp();
void relationalOp();
void returnStmt();
void optionalReturn();
void idList();
void moreIds();
void definetypestmt();
void a();

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

void match(Symbols expected) {
    if (getCurrentToken().type == expected) {
        advanceToken();
    } else {
        printf("Syntax error: Expected %d but found %d\n", expected, getCurrentToken().type);
        exit(1);
    }
}

// Function implementations

void program() {
    otherFunctions();
    mainFunction();
}

void mainFunction() {
    match(TK_MAIN);
    stmts();
    match(TK_END);
}

void otherFunctions() {
    if (getCurrentToken().type == TK_FUNID) {
        function();
        otherFunctions();
    } else {
        // EPSILON production
    }
}

void function() {
    match(TK_FUNID);
    input_par();
    output_par();
    match(TK_SEM);
    stmts();
    match(TK_END);
}

void input_par() {
    match(TK_INPUT);
    match(TK_PARAMETER);
    match(TK_LIST);
    match(TK_SQL);
    parameter_list();
    match(TK_SQR);
}

void output_par() {
    if (getCurrentToken().type == TK_OUTPUT) {
        match(TK_OUTPUT);
        match(TK_PARAMETER);
        match(TK_LIST);
        match(TK_SQL);
        parameter_list();
        match(TK_SQR);
    } else {
        // EPSILON production
    }
}

void parameter_list() {
    dataType();
    match(TK_ID);
    remaining_list();
}

void dataType() {
    if (getCurrentToken().type == TK_INT || getCurrentToken().type == TK_REAL) {
        primitiveDatatype();
    } else if (getCurrentToken().type == TK_RECORD || getCurrentToken().type == TK_UNION || getCurrentToken().type == TK_RUID) {
        constructedDatatype();
    } else {
        printf("Syntax error in dataType\n");
        exit(1);
    }
}

void primitiveDatatype() {
    if (getCurrentToken().type == TK_INT) {
        match(TK_INT);
    } else if (getCurrentToken().type == TK_REAL) {
        match(TK_REAL);
    } else {
        printf("Syntax error in primitiveDatatype\n");
        exit(1);
    }
}

void constructedDatatype() {
    if (getCurrentToken().type == TK_RECORD) {
        match(TK_RECORD);
        match(TK_RUID);
    } else if (getCurrentToken().type == TK_UNION) {
        match(TK_UNION);
        match(TK_RUID);
    } else if (getCurrentToken().type == TK_RUID) {
        match(TK_RUID);
    } else {
        printf("Syntax error in constructedDatatype\n");
        exit(1);
    }
}

void remaining_list() {
    if (getCurrentToken().type == TK_COMMA) {
        match(TK_COMMA);
        parameter_list();
    } else {
        // EPSILON production
    }
}

void stmts() {
    typeDefinitions();
    declarations();
    otherstmts();
    returnStmt();
}

void typeDefinitions() {
    if (getCurrentToken().type == TK_RECORD || getCurrentToken().type == TK_UNION) {
        isRedefined();
        typeDefinition();
    } else {
        // EPSILON production
    }
}

void isRedefined() {
    if (getCurrentToken().type == TK_RECORD || getCurrentToken().type == TK_UNION) {
        typeDefinition();
    } else {
        definetypestmt();
    }
}

void typeDefinition() {
    if (getCurrentToken().type == TK_RECORD) {
        match(TK_RECORD);
        match(TK_RUID);
        fieldDefinitions();
        match(TK_ENDRECORD);
    } else if (getCurrentToken().type == TK_UNION) {
        match(TK_UNION);
        match(TK_RUID);
        fieldDefinitions();
        match(TK_ENDUNION);
    } else {
        printf("Syntax error in typeDefinition\n");
        exit(1);
    }
}

void fieldDefinitions() {
    fieldDefinition();
    moreFields();
}

void fieldDefinition() {
    match(TK_TYPE);
    dataType();
    match(TK_COLON);
    match(TK_FIELDID);
    match(TK_SEM);
}

void moreFields() {
    if (getCurrentToken().type == TK_TYPE) {
        fieldDefinition();
        moreFields();
    } else {
        // EPSILON production
    }
}

void declarations() {
    if (getCurrentToken().type == TK_TYPE) {
        declaration();
        declarations();
    } else {
        // EPSILON production
    }
}

void declaration() {
    match(TK_TYPE);
    dataType();
    match(TK_COLON);
    match(TK_ID);
    global_or_not();
    match(TK_SEM);
}

void global_or_not() {
    if (getCurrentToken().type == TK_COLON) {
        match(TK_COLON);
        match(TK_GLOBAL);
    } else {
        // EPSILON production
    }
}

void otherstmts() {
    if (getCurrentToken().type == TK_ID || getCurrentToken().type == TK_WHILE || getCurrentToken().type == TK_IF || getCurrentToken().type == TK_READ || getCurrentToken().type == TK_WRITE || getCurrentToken().type == TK_CALL) {
        stmt();
        otherstmts();
    } else {
        // EPSILON production
    }
}

void stmt() {
    if (getCurrentToken().type == TK_ID) {
        assignmentStmt();
    } else if (getCurrentToken().type == TK_WHILE) {
        iterativeStmt();
    } else if (getCurrentToken().type == TK_IF) {
        conditionalStmt();
    } else if (getCurrentToken().type == TK_READ || getCurrentToken().type == TK_WRITE) {
        ioStmt();
    } else if (getCurrentToken().type == TK_CALL) {
        funCallStmt();
    } else {
        printf("Syntax error in stmt\n");
        exit(1);
    }
}

void assignmentStmt() {
    singleOrRecId();
    match(TK_ASSIGNOP);
    arithmeticExpression();
    match(TK_SEM);
}

void singleOrRecId() {
    match(TK_ID);
    singleLeft();
}

void singleLeft() {
    if (getCurrentToken().type == TK_ID) {
        match(TK_ID);
        oneExpansion();
        moreExpansions();
    } else {
        // EPSILON production
    }
}

void oneExpansion() {
    match(TK_DOT);
    match(TK_FIELDID);
}

void moreExpansions() {
    if (getCurrentToken().type == TK_DOT) {
        oneExpansion();
        moreExpansions();
    } else {
        // EPSILON production
    }
}

void funCallStmt() {
    outputParameters();
    match(TK_CALL);
    match(TK_FUNID);
    match(TK_WITH);
    match(TK_PARAMETERS);
    inputParameters();
    match(TK_SEM);
}

void outputParameters() {
    if (getCurrentToken().type == TK_SQL) {
        match(TK_SQL);
        idList();
        match(TK_SQR);
        match(TK_ASSIGNOP);
    } else {
        // EPSILON production
    }
}

void inputParameters() {
    match(TK_SQL);
    idList();
    match(TK_SQR);
}

void iterativeStmt() {
    match(TK_WHILE);
    match(TK_OP);
    booleanExpression();
    match(TK_CL);
    stmt();
    otherstmts();
    match(TK_ENDWHILE);
}

void conditionalStmt() {
    match(TK_IF);
    match(TK_OP);
    booleanExpression();
    match(TK_CL);
    match(TK_THEN);
    stmt();
    otherstmts();
    elseStmt();
}

void elseStmt() {
    if (getCurrentToken().type == TK_ELSE) {
        match(TK_ELSE);
        otherstmts();
        match(TK_ENDIF);
    } else if (getCurrentToken().type == TK_ENDIF) {
        match(TK_ENDIF);
    } else {
        printf("Syntax error in elseStmt\n");
        exit(1);
    }
}

void var(){
    if(getCurrentToken().type == TK_ID){
        singleOrRecId();
    } else if (getCurrentToken().type == TK_NUM){
        match(TK_NUM);
    } else if (getCurrentToken().type == TK_RNUM){
        match(TK_RNUM);
    }
}

void ioStmt() {
    if (getCurrentToken().type == TK_READ) {
        match(TK_READ);
        match(TK_OP);
        var();
        match(TK_CL);
        match(TK_SEM);
    } else if (getCurrentToken().type == TK_WRITE) {
        match(TK_WRITE);
        match(TK_OP);
        var();
        match(TK_CL);
        match(TK_SEM);
    } else {
        printf("Syntax error in ioStmt\n");
        exit(1);
    }
}

void arithmeticExpression() {
    term();
    expPrime();
}

void expPrime() {
    if (getCurrentToken().type == TK_PLUS || getCurrentToken().type == TK_MINUS) {
        lowPrecedenceOps();
        term();
        expPrime();
    } else {
        // EPSILON production
    }
}

void term() {
    factor();
    termPrime();
}

void termPrime() {
    if (getCurrentToken().type == TK_MUL || getCurrentToken().type == TK_DIV) {
        highPrecedenceOps();
        factor();
        termPrime();
    } else {
        // EPSILON production
    }
}

void factor() {
    if (getCurrentToken().type == TK_OP) {
        match(TK_OP);
        arithmeticExpression();
        match(TK_CL);
    } else if (getCurrentToken().type == TK_ID || getCurrentToken().type == TK_NUM || getCurrentToken().type == TK_RNUM) {
        var();
    } else {
        printf("Syntax error in factor\n");
        exit(1);
    }
}

void highPrecedenceOps() {
    if (getCurrentToken().type == TK_MUL) {
        match(TK_MUL);
    } else if (getCurrentToken().type == TK_DIV) {
        match(TK_DIV);
    } else {
        printf("Syntax error in highPrecedenceOps\n");
        exit(1);
    }
}

void lowPrecedenceOps() {
    if (getCurrentToken().type == TK_PLUS) {
        match(TK_PLUS);
    } else if (getCurrentToken().type == TK_MINUS) {
        match(TK_MINUS);
    } else {
        printf("Syntax error in lowPrecedenceOps\n");
        exit(1);
    }
}

void booleanExpression() {
    if (getCurrentToken().type == TK_OP) {
        match(TK_OP);
        booleanExpression();
        match(TK_CL);
        logicalOp();
        match(TK_OP);
        booleanExpression();
        match(TK_CL);
        match(TK_SEM);
    } else if (getCurrentToken().type == TK_ID || getCurrentToken().type == TK_NUM || getCurrentToken().type == TK_RNUM) {
        var();
        relationalOp();
        var();
    } else if (getCurrentToken().type == TK_NOT) {
        match(TK_NOT);
        match(TK_OP);
        booleanExpression();
        match(TK_CL);
    } else {
        printf("Syntax error in booleanExpression\n");
        exit(1);
    }
}

void logicalOp() {
    if (getCurrentToken().type == TK_AND) {
        match(TK_AND);
    } else if (getCurrentToken().type == TK_OR) {
        match(TK_OR);
    } else {
        printf("Syntax error in logicalOp\n");
        exit(1);
    }
}

void relationalOp() {
    if (getCurrentToken().type == TK_LT) {
        match(TK_LT);
    } else if (getCurrentToken().type == TK_LE) {
        match(TK_LE);
    } else if (getCurrentToken().type == TK_EQ) {
        match(TK_EQ);
    } else if (getCurrentToken().type == TK_GT) {
        match(TK_GT);
    } else if (getCurrentToken().type == TK_GE) {
        match(TK_GE);
    } else if (getCurrentToken().type == TK_NE) {
        match(TK_NE);
    } else {
        printf("Syntax error in relationalOp\n");
        exit(1);
    }
}

void returnStmt() {
    match(TK_RETURN);
    optionalReturn();
    match(TK_SEM);
}

void optionalReturn() {
    if (getCurrentToken().type == TK_SQL) {
        match(TK_SQL);
        idList();
        match(TK_SQR);
    } else {
        // EPSILON production
    }
}

void idList() {
    match(TK_ID);
    moreIds();
}

void moreIds() {
    if (getCurrentToken().type == TK_COMMA) {
        match(TK_COMMA);
        idList();
    } else {
        // EPSILON production
    }
}

void definetypestmt() {
    match(TK_DEFINETYPE);
    a();
    match(TK_RUID);
    match(TK_AS);
    match(TK_RUID);
}

void a() {
    if (getCurrentToken().type == TK_RECORD) {
        match(TK_RECORD);
    } else if (getCurrentToken().type == TK_UNION) {
        match(TK_UNION);
    } else {
        printf("Syntax error in a\n");
        exit(1);
    }
}

int main() {
    // Example token stream
    Token exampleTokens[] = {
        {TK_MAIN, "_main"},  {TK_TYPE, "type"}, {TK_INT, "int"}, {TK_COLON, ":"}, 
        {TK_ID, "x"}, {TK_SEM, ";"},
        {TK_RETURN, "return"}, {TK_SEM,";"},{TK_END, "end"}, {EPSILON, ""}
    };
    
    tokens = exampleTokens;
    tokenCount = sizeof(exampleTokens) / sizeof(exampleTokens[0]);
    
    program();
    
    printf("Parsing completed successfully.\n");
    return 0;
}

#endif

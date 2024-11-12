#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MIN(x,y) x < y ? x : y
#define MAX(x,y) x > y ? x : y
#define EOB '$'
#define KILL(x) perror(x); exit(EXIT_FAILURE);
#define NONTERMINALS  
#define TERMINALS 57
#define IDLENGTH 30
#define MAXUNIQUE 8

typedef struct Token *Token;

typedef enum {
    nt_program,
    nt_mainFunction,
    nt_otherFunctions,
    nt_function,
    nt_input_par,
    nt_output_par,
    nt_parameter_list,
    nt_dataType,
    nt_primitiveDatatype,
    nt_constructedDatatype,
    nt_remaining_list,
    nt_stmts,
    nt_typeDefinitions,
    nt_isRedefined,
    nt_typeDefinition,
    nt_fieldDefinitions,
    nt_fieldDefinition,
    nt_moreFields,
    nt_declarations,
    nt_declaration,
    nt_global_or_not,
    nt_otherstmts,
    nt_stmt,
    nt_assignmentStmt,
    nt_singleLeft,
    nt_oneExpansion,
    nt_moreExpansions,
    nt_funCallStmt,
    nt_outputParameters,
    nt_inputParameters,
    nt_iterativeStmt,
    nt_conditionalStmt,
    nt_elseStmt,
    nt_ioStmt,
    nt_arithmeticExpression,
    nt_expPrime,
    nt_term,
    nt_termPrime,
    nt_factor,
    nt_highPrecedenceOps,
    nt_lowPrecedenceOps,
    nt_booleanExpression,
    nt_singleOrRecId,
    nt_var,
    nt_logicalOp,
    nt_relationalOp,
    nt_returnStmt,
    nt_optionalReturn,
    nt_idList,
    nt_moreIds,
    nt_definetypestmt,
    nt_a
} NonTerminal;

// A list of all the possible terminals from source code
typedef enum {
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RUID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_ERROR,
    TK_EOF
} Terminal;


// Using union makes this a lot easier to think about
typedef union {
    NonTerminal nt;
    Terminal t;
} Symbols;

struct Token{ 
    Terminal terminal;
    char value[IDLENGTH];
    int lineno;
};

typedef struct KeywordPair{
    char keyword[IDLENGTH];
    Symbols enumVal;
} KeywordPair; 



typedef struct SymbolPair {
    char string[30];
    Symbols symbol;
} SymbolPair;


// Not using this as of now, meant for table parsing
// extern const _NonTerminal NonTerminalProd[57] = {
//     {nt_program, {0}, 1},
//     {nt_mainFunction, {1}, 1},
//     {nt_otherFunctions, {2, 3}, 2},
//     {nt_function, {4}, 1},
//     {nt_input_par, {5}, 1},
//     {nt_output_par, {6, 7}, 2},
//     {nt_parameter_list, {8}, 1},
//     {nt_dataType, {9, 10}, 2},
//     {nt_primitiveDatatype, {11, 12}, 2},
//     {nt_constructedDatatype, {13, 14, 15}, 3},
//     {nt_remaining_list, {16, 17}, 2},
//     {nt_stmts, {18}, 1},
//     {nt_typeDefinitions, {19, 20}, 2},
//     {nt_isRedefined, {21, 22}, 2},
//     {nt_typeDefinition, {23, 24}, 2},
//     {nt_fieldDefinitions, {25}, 1},
//     {nt_fieldDefinition, {26}, 1},
//     {nt_moreFields, {27, 28}, 2},
//     {nt_declarations, {29}, 1},
//     {nt_declaration, {30}, 1},
//     {nt_global_or_not, {31, 32}, 2},
//     {nt_otherstmts, {33, 34}, 2},
//     {nt_stmt, {35, 36, 37, 38, 39}, 5},
//     {nt_assignmentStmt, {40}, 1},
//     {nt_singleOrRecId, {41}, 1},
//     {nt_singleLeft, {42, 43}, 2},
//     {nt_oneExpansion, {44}, 1},
//     {nt_moreExpansions, {45, 46}, 2},
//     {nt_funCallStmt, {47}, 1},
//     {nt_outputParameters, {48, 49}, 2},
//     {nt_inputParameters, {50}, 1},
//     {nt_iterativeStmt, {51}, 1},
//     {nt_conditionalStmt, {52}, 1},
//     {nt_elseStmt, {53, 54}, 2},
//     {nt_ioStmt, {55, 56}, 2},
//     {nt_arithmeticExpression, {57}, 1},
//     {nt_expPrime, {58, 59}, 2},
//     {nt_term, {60}, 1},
//     {nt_termPrime, {61, 62}, 2},
//     {nt_factor, {63, 64}, 2},
//     {nt_highPrecedenceOps, {65, 66}, 2},
//     {nt_lowPrecedenceOps, {67, 68}, 2},
//     {nt_booleanExpression, {69, 70, 71}, 3},
//     {nt_var, {72, 73}, 2},
//     {nt_logicalOp, {74, 75}, 2},
//     {nt_relationalOp, {76, 77, 78, 79, 80, 81}, 6},
//     {nt_returnStmt, {82}, 1},
//     {nt_optionalReturn, {83, 84}, 2},
//     {nt_idList, {85}, 1},
//     {nt_moreIds, {86, 87}, 2},
//     {nt_definetypestmt, {88}, 1},
//     {nt_a, {89, 90}, 2}
// };


// extern const SymbolPair StringToSymbols[111]
/* = {
    {"program" , nt_program},
    {"mainFunction" , nt_mainFunction},
    {"otherFunctions" , nt_otherFunctions},
    {"function" , nt_function},
    {"input_par" , nt_input_par},
    {"output_par" , nt_output_par},
    {"parameter_list" , nt_parameter_list},
    {"dataType" , nt_dataType},
    {"primitiveDatatype" , nt_primitiveDatatype},
    {"constructedDatatype" , nt_constructedDatatype},
    {"remaining_list" , nt_remaining_list},
    {"stmts" , nt_stmts},
    {"typeDefinitions" , nt_typeDefinitions},
    {"isRedefined" , nt_isRedefined},
    {"typeDefinition" , nt_typeDefinition},
    {"fieldDefinitions" , nt_fieldDefinitions},
    {"fieldDefinition" , nt_fieldDefinition},
    {"moreFields" , nt_moreFields},
    {"declarations" , nt_declarations},
    {"declaration" , nt_declaration},
    {"global_or_not" , nt_global_or_not},
    {"otherstmts" , nt_otherstmts},
    {"stmt" , nt_stmt},
    {"assignmentStmt" , nt_assignmentStmt},
    {"singleOrRecId" , nt_singleOrRecId},
    {"singleLeft" , nt_singleLeft},
    {"oneExpansion" , nt_oneExpansion},
    {"moreExpansions" , nt_moreExpansions},
    {"funCallStmt" , nt_funCallStmt},
    {"outputParameters" , nt_outputParameters},
    {"inputParameters" , nt_inputParameters},
    {"iterativeStmt" , nt_iterativeStmt},
    {"conditionalStmt" , nt_conditionalStmt},
    {"elseStmt" , nt_elseStmt},
    {"ioStmt" , nt_ioStmt},
    {"arithmeticExpression" , nt_arithmeticExpression},
    {"expPrime" , nt_expPrime},
    {"term" , nt_term},
    {"termPrime" , nt_termPrime},
    {"factor" , nt_factor},
    {"highPrecedenceOps" , nt_highPrecedenceOps},
    {"lowPrecedenceOps" , nt_lowPrecedenceOps},
    {"booleanExpression" , nt_booleanExpression},
    {"singleOrRecId" , nt_singleOrRecId},
    {"var" , nt_var},
    {"logicalOp" , nt_logicalOp},
    {"relationalOp" , nt_relationalOp},
    {"returnStmt" , nt_returnStmt},
    {"optionalReturn" , nt_optionalReturn},
    {"idList" , nt_idList},
    {"moreIds" , nt_moreIds},
    {"definetypestmt" , nt_definetypestmt},
    {"a" , nt_a},
    {"TK_ASSIGNOP", TK_ASSIGNOP},
    {"TK_COMMENT", TK_COMMENT},
    {"TK_FIELDID", TK_FIELDID},
    {"TK_ID", TK_ID},
    {"TK_NUM", TK_NUM},
    {"TK_RNUM", TK_RNUM},
    {"TK_FUNID", TK_FUNID},
    {"TK_RUID", TK_RUID},
    {"TK_WITH", TK_WITH},
    {"TK_PARAMETERS", TK_PARAMETERS},
    {"TK_END", TK_END},
    {"TK_WHILE", TK_WHILE},
    {"TK_UNION", TK_UNION},
    {"TK_ENDUNION", TK_ENDUNION},
    {"TK_DEFINETYPE", TK_DEFINETYPE},
    {"TK_AS", TK_AS},
    {"TK_TYPE", TK_TYPE},
    {"TK_MAIN", TK_MAIN},
    {"TK_GLOBAL", TK_GLOBAL},
    {"TK_PARAMETER", TK_PARAMETER},
    {"TK_LIST", TK_LIST},
    {"TK_SQL", TK_SQL},
    {"TK_SQR", TK_SQR},
    {"TK_INPUT", TK_INPUT},
    {"TK_OUTPUT", TK_OUTPUT},
    {"TK_INT", TK_INT},
    {"TK_REAL", TK_REAL},
    {"TK_COMMA", TK_COMMA},
    {"TK_SEM", TK_SEM},
    {"TK_COLON", TK_COLON},
    {"TK_DOT", TK_DOT},
    {"TK_ENDWHILE", TK_ENDWHILE},
    {"TK_OP", TK_OP},
    {"TK_CL", TK_CL},
    {"TK_IF", TK_IF},
    {"TK_THEN", TK_THEN},
    {"TK_ENDIF", TK_ENDIF},
    {"TK_READ", TK_READ},
    {"TK_WRITE", TK_WRITE},
    {"TK_RETURN", TK_RETURN},
    {"TK_PLUS", TK_PLUS},
    {"TK_MINUS", TK_MINUS},
    {"TK_MUL", TK_MUL},
    {"TK_DIV", TK_DIV},
    {"TK_CALL", TK_CALL},
    {"TK_RECORD", TK_RECORD},
    {"TK_ENDRECORD", TK_ENDRECORD},
    {"TK_ELSE", TK_ELSE},
    {"TK_AND", TK_AND},
    {"TK_OR", TK_OR},
    {"TK_NOT", TK_NOT},
    {"TK_LT", TK_LT},
    {"TK_LE", TK_LE},
    {"TK_GT", TK_GT},
    {"TK_GE", TK_GE},
    {"TK_EQ", TK_EQ},
    {"TK_NE", TK_NE},
    {"EPSILON", EPSILON},
};
*/

typedef enum {
    FALSE,
    TRUE
} boolean;



#endif
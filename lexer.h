#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "helper/definitions.h"

//TODO: ## There is a line number discrepancy (not sure from where) ##

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

// Token createToken(char *value, int lineno, Terminal term);

#endif

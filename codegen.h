#ifndef CODEGEN_H
#define CODEGEN_H

#include <string.h>
#include <stdlib.h>
#include "astgen.h"
#include "symboltable.h"

#define LINELENGTH 100


typedef struct code_line {
    char value[LINELENGTH];
    struct code_line* next;
} code_line;

typedef struct code_block {
    code_line *top;
    code_line *bottom;
} code_block;

void codegen_run(char *opfilepath, SymbolTable *table, AstTreeNode *program);

#endif
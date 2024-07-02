#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include "tree.c"
#include "../lexer.h"

#endif

typedef struct ASTnode {
    Token *tokenVal;
} ASTNode;
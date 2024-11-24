#ifndef ASTGEN_H
#define ASTGEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper/definitions.h"

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

#endif
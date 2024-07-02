#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "datastructures/buffer.h"
#include "helper/definitions.h"


typedef struct Token{ 
    Symbols terminal;
    char value[IDLENGTH];
    int lineno;
} Token;

static int currLine = 0;

#endif
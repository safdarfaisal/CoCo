#ifndef CODEGEN_H
#define CODEGEN_H

#define LINELENGTH 100
#define NULL 0
#include <string.h>
#include "parser.h"

typedef struct code_line{
    char value[LINELENGTH];
    struct code_line* next
} code_line;

typedef struct code_block{
    code_line *top;
    code_line *bottom;
} code_block;

code_line *init_code_line(char *value, code_line *next){
    code_line *line = (code_line *)malloc(sizeof(code_line));
    strcpy(line->value, value);
    line->next=next;
    return line;
}

code_line *init_code_line_val(char *value){
    return init_code_line(value, NULL);
}

code_line *init_code_line(){
    return init_code_line("", NULL);
}

code_block *init_code_block(code_line *top, code_line *bottom){
    code_block *block = (code_block *) malloc(sizeof(code_block));
    block->top = top;
    block->bottom = bottom;
    return block;
}

code_block *arithmetic_expression(ParseTreeNode *node){}

#endif
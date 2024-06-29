#ifndef STACK_H
#define STACK_H

#include<stdio.h>
#include<stdlib.h>

typedef struct StackNode{
    void* value;
    struct StackNode *prev;
} StackNode;

typedef struct Stack{
    StackNode *top;
    int length;
} Stack;

Stack* initStack();
void push(Stack *s, void* val);
void pop(Stack *s);
void *peek(Stack *s);
int isEmpty(Stack *s);
int getLength(Stack *s);

//DEBUG ONLY
void printStackVals(Stack *s);

#endif
#include "stack.h"

Stack* initStack(){
    Stack* newStack = (Stack *)malloc(sizeof(Stack));
    newStack->top = NULL;
    newStack->length = 0;
    return newStack;
}

void push (Stack *s, void* val){
    StackNode *node = (StackNode *) malloc(sizeof(StackNode));
    node->value = val;
    node->prev = s->top;
    s->top = node;
    if (!s->top){
        s->length = 1;
    } else {
        s->length++;
    }
}

void pop(Stack *s){
    if(!s){
        printf("Stack not found, returning");
        return ;
    }
    if(!s->top){
        printf("Stack empty, returning");
        return ;
    }
    StackNode *top = s->top;
    s->top = top->prev;
    s->length--;
}

void *peek(Stack *s){
    if (!s){
        printf("Stack not found, returning");
        return NULL;
    }
    return s->top->value;
}

int getLength(Stack *s){
    return !s ? 0 : s->length; 
}

int isEmpty(Stack *s){
    return s->length ? 0 : 1;
}

// For debug purposes only

// void printStackVals(Stack *s){
//     StackNode *currNode = s->top;
//     while(currNode){
//         printf("%x", currNode->value);
//         currNode = currNode->prev;
//     }
//     fflush(stdout);
// }

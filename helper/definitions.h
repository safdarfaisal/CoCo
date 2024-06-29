#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MIN(x,y) x < y ? x : y
#define MAX(x,y) x > y ? x : y
#define BUFFERSIZE 2000
#define EOB '$'

typedef enum boolean {
    FALSE = 0,
    TRUE = 1
} boolean;

#endif
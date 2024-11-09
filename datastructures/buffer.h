#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../helper/definitions.h"

#define BUFFERSIZE 2000
#define EOB '$'

typedef struct Buffer *Buffer;
typedef struct TwinBuffer *TwinBuffer;

struct Buffer {
    char buffer[BUFFERSIZE];
    int forward;
};

struct TwinBuffer {
    Buffer firstBuffer;
    Buffer secondBuffer;
    boolean isFirst;
    int forward; // current position of character read w.r.t current buffer
    /* currWordStart: for backtracking, value ranges between 0 and 2*BUFFERSIZE,
     -1 if not in middle of reading */
    int currWordStart; 
    FILE *fp;
};

Buffer _initBuffer();
size_t _loadBuffer(Buffer buffer, FILE *fp);
TwinBuffer initTwinBuffer();

FILE *createFilePointer(char *path);
void setFilePointer(char *path, TwinBuffer fullBuffer);
void toggleIsFirst(TwinBuffer fullBuffer);
void loadFileIntoBuffer(TwinBuffer fullBuffer);
char getCharacterFromBuffer(Buffer buffer, int *forward);
char getCharacter(TwinBuffer fullBuffer);
void _toggleIsReading(TwinBuffer fullBuffer);
void _freeBuffer(Buffer buffer);
void freeTwinBuffer(TwinBuffer buffer);
void goBackOne(TwinBuffer fullBuffer);

#endif
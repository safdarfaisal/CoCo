#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../helper/definitions.h"

typedef struct Buffer {
    char buffer[BUFFERSIZE];
    int forward;
} Buffer;

typedef struct TwinBuffer {
    Buffer *firstBuffer;
    Buffer *secondBuffer;
    boolean isFirst;
    int forward;
    FILE *fp;
} TwinBuffer;

Buffer *initBuffer();
size_t loadBuffer(Buffer *buffer, FILE *fp);
TwinBuffer *initTwinBuffer();
FILE *createFilePointer(char *path);
void setFilePointer(char *path, TwinBuffer *buffer);
void toggleIsFirst(TwinBuffer *buffer);
void loadFileIntoBuffer(TwinBuffer *buffer);
char getCharacterFromBuffer(Buffer *buffer, int *forward);
char getCharacter(TwinBuffer *buffer);

void freeBuffer(Buffer *buffer);
void freeTwinBuffer(TwinBuffer *buffer);


#endif
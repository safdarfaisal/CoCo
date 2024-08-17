#include "buffer.h"

Buffer *initBuffer(){
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    memset(buffer, 0, sizeof(*buffer));
    buffer->buffer[BUFFERSIZE-1] = EOB; //This has to remain constant 
    return buffer;
}

size_t loadBuffer(Buffer *buffer, FILE *fp){
    size_t charactersRead = fread(buffer->buffer, sizeof(char), 
        BUFFERSIZE - 1, fp);
    if (charactersRead < BUFFERSIZE - 1){
        buffer->buffer[charactersRead/sizeof(char)] = '\0';
    }
    buffer->forward = 0;
    return charactersRead;
}

void freeBuffer(Buffer *buffer){
    free(buffer);
}

void freeTwinBuffer(TwinBuffer *buffer){
    freeBuffer(buffer->firstBuffer);
    freeBuffer(buffer->secondBuffer);

    if(buffer->fp){
        if(!fclose(buffer->fp)){
            perror("Failed to close file during destructor");
            exit(EXIT_FAILURE);
        }
    }
    free(buffer);
}

TwinBuffer *initTwinBuffer(){
    TwinBuffer *twinBuffer = (TwinBuffer *)malloc(sizeof(TwinBuffer));
    memset(twinBuffer, 0, sizeof(*twinBuffer));
    twinBuffer->firstBuffer = initBuffer();
    twinBuffer->secondBuffer = initBuffer();
    twinBuffer->isFirst = TRUE;
    return twinBuffer;
}

FILE *createFilePointer(char *path){
    FILE *fp = fopen(path, "r");
    if(!fp){
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    return fp;
}

void setFilePointer(char *path, TwinBuffer *buffer){
    FILE *fp = createFilePointer(path);
    if (!buffer->fp){
        buffer->fp = fp;
    } else {
        if(!fclose(buffer->fp)){
            perror("Unable to close existing file");
            exit(EXIT_FAILURE);
        }
        buffer->fp = fp;
    }
}

void toggleIsFirst(TwinBuffer *buffer){
    buffer->isFirst = buffer->isFirst ? FALSE : TRUE;
}

void loadFileIntoBuffer(TwinBuffer *twinBuffer){
    if(!twinBuffer->fp){
        perror("No input file found");
        exit(EXIT_FAILURE);
    }
    toggleIsFirst(twinBuffer);
    Buffer *buffer = twinBuffer->isFirst ? twinBuffer->firstBuffer : twinBuffer->secondBuffer;
    loadBuffer(buffer, twinBuffer->fp);
    
}

char getCharacterFromBuffer(Buffer *buffer, int *forward){
    if(!buffer){
        perror("Buffer not initialized");
        exit(EXIT_FAILURE);
    }
    char character = buffer->buffer[(*forward)++];
    return character;
}

char getCharacter(TwinBuffer *buffer){
    if(!buffer){
        perror("Twin Buffer not initialized");
        exit(EXIT_FAILURE);
    }
    Buffer *currentBuffer = buffer->isFirst ? buffer->firstBuffer : buffer->secondBuffer;
    Buffer *otherBuffer = !buffer->isFirst ? buffer->firstBuffer : buffer->secondBuffer;
    char character = getCharacterFromBuffer(currentBuffer, &(buffer->forward));
    switch (character){
        case '\0':
            // EOF hit
            printf("\nAll information has been read\n");
        break;
        case '$':
            // EOB hit
            toggleIsFirst(buffer);
            loadBuffer(otherBuffer, buffer->fp);
            character = buffer->isFirst ? currentBuffer->buffer[0] : otherBuffer->buffer[0];
            buffer->forward = 1;
        break;
    }
    return character;
}

void ungetcBuffer(TwinBuffer *buffer, char *c){
    if(!buffer)    KILL("Twinbuffer not initialized")
    Buffer *currBuffer = buffer->isFirst ? buffer->firstBuffer : buffer-> secondBuffer;
    currBuffer->forward--;
}

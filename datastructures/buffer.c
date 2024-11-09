#include "buffer.h"
#include <ctype.h>


// create base buffer structure
Buffer _initBuffer(){
    Buffer buffer = (Buffer)malloc(sizeof(struct Buffer));
    memset(buffer, 0, sizeof(struct Buffer));
    buffer->buffer[BUFFERSIZE-1] = EOB; //This has to remain constant 
    return buffer;
}

// load the buffer with data from the file 
size_t _loadBuffer(Buffer buffer, FILE *fp){
    size_t charactersRead = fread(buffer->buffer, sizeof(char), 
        BUFFERSIZE - 1, fp);
    if (charactersRead < BUFFERSIZE - 1){
        buffer->buffer[charactersRead/sizeof(char)] = '\0';
    }
    buffer->forward = 0;
    return charactersRead;
}

void _freeBuffer(Buffer buffer){
    free(buffer);
}



TwinBuffer initTwinBuffer(){
    TwinBuffer twinBuffer = (TwinBuffer)malloc(sizeof(struct TwinBuffer));
    memset(twinBuffer, 0, sizeof(*twinBuffer));
    twinBuffer->firstBuffer = _initBuffer();
    twinBuffer->secondBuffer = _initBuffer();
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

void freeTwinBuffer(TwinBuffer buffer){
    _freeBuffer(buffer->firstBuffer);
    _freeBuffer(buffer->secondBuffer);
    if(buffer->fp){
        if(!fclose(buffer->fp)){
            perror("Failed to close file during destructor");
            exit(EXIT_FAILURE);
        }
    }
    free(buffer);
}

void setFilePointer(char *path, TwinBuffer fullBuffer){
    FILE *fp = createFilePointer(path);
    if (!fullBuffer->fp){
        fullBuffer->fp = fp;
    } else {
        if(!fclose(fullBuffer->fp)){
            perror("Unable to close existing file");
            exit(EXIT_FAILURE);
        }
        fullBuffer->fp = fp;
    }
}

void toggleIsFirst(TwinBuffer fullBuffer){
    fullBuffer->isFirst = fullBuffer->isFirst ? FALSE : TRUE;
}

void loadFileIntoBuffer(TwinBuffer twinBuffer){
    if(!twinBuffer->fp){
        perror("No input file found");
        exit(EXIT_FAILURE);
    }
    toggleIsFirst(twinBuffer);
    Buffer buffer = twinBuffer->isFirst ? twinBuffer->firstBuffer : twinBuffer->secondBuffer;
    _loadBuffer(buffer, twinBuffer->fp);
    
}

char getCharacterFromBuffer(Buffer buffer, int *forward){
    if(!buffer){
        perror("Buffer not initialized");
        exit(EXIT_FAILURE);
    }
    char character = buffer->buffer[(*forward)++];
    return character;
}


// Really don't think i'll be using this, but fun to keep ig?? 
void _toggleIsReading(TwinBuffer fullBuffer){
    if(fullBuffer->currWordStart != -1){
        fullBuffer->currWordStart = -1;
    } else {
        fullBuffer->currWordStart = fullBuffer->forward + fullBuffer->isFirst * 1999;
    }
    
}

char getCharacter(TwinBuffer fullBuffer){
    if(!fullBuffer){
        perror("Twin Buffer not initialized");
        exit(EXIT_FAILURE);
    }
    Buffer currentBuffer = fullBuffer->isFirst ? fullBuffer->firstBuffer : fullBuffer->secondBuffer;
    Buffer otherBuffer = !fullBuffer->isFirst ? fullBuffer->firstBuffer : fullBuffer->secondBuffer;
    char character = getCharacterFromBuffer(currentBuffer, &(fullBuffer->forward)); // Character has been taken
    switch (character){
        case '\0':
            // EOF hit
            printf("All information has been read\n");
        break;
        case '$':
            // EOB hit
            printf("Switching Buffers\n");
            toggleIsFirst(fullBuffer);
            _loadBuffer(otherBuffer, fullBuffer->fp);
            character = fullBuffer->isFirst ? currentBuffer->buffer[0] : otherBuffer->buffer[0];
            fullBuffer->forward = 1;
        break;
    }
    if(fullBuffer->currWordStart == -1 && character != ' '){
        _toggleIsReading(fullBuffer);
    } else if(fullBuffer->currWordStart != -1 && isspace(character)){
        _toggleIsReading(fullBuffer);
    }
    return character;
}


// You'll only be loading the next buffer at the end of this one right?
// so you coukd just make it a bound of 0 to 3999 right? 

// we know 1999 and 3999 are fixed to EOB character.. :shrugs:
// so we know that if 2000, go to 1998, and if at 0, go to 3998
// Only conditionals right?
// If buffer->forward at 0, set forward to 1998 always and toggle isFirst
// else just decrement forward. Will I have a case where forward is 0 tho?
// since if it reads 1999, it goes to next after reading 0
// Will I never have that conditional? Still gonna add it to be safe.

void goBackOne(TwinBuffer fullBuffer){
    if(!fullBuffer){
        KILL("Twinbuffer not initalized");
    }
    if(fullBuffer->forward == 0){
        toggleIsFirst(fullBuffer);
        fullBuffer->forward = BUFFERSIZE - 2;
    } else {
        fullBuffer->forward--;
    }
}
// void ungetcBuffer(TwinBuffer buffer, char *c){
//     if(!buffer)    KILL("Twinbuffer not initialized")
//     Buffer currBuffer = buffer->isFirst ? buffer->firstBuffer : buffer-> secondBuffer;
//     currBuffer->forward--;
// }

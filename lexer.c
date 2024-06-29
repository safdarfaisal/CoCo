#include "lexer.h"

Token *initToken(){
    Token *token = (Token *)malloc(sizeof(Token));
    memset(token, 0, sizeof(Token));
    return token;
}

void setValue(Token *token, char *value, int size){
    memset(token->value, 0, IDLENGTH*sizeof(char));
    memcpy(token->value, value, size*sizeof(char));
}

void setLineNum(Token *token, int lineno){
    token->lineno = lineno;
}

void setTerminalType(Token *token, Terminals term){
    token->terminal = term;
}

Terminals getKeywordTerminal(char *string){
    for(int i = 0; i < 25; i++){
        if(!strcmp(keywords[i].keyword, string)){
            return keywords[i].enumVal;
        }
    }
    return TK_FIELDID;
}

Token *createToken(char *value, int lineno, Terminals term){
    Token *token = initToken();
    setValue(token, value);
    setLineNum(token, lineno);
    setTerminalType(token, term);
    return token;
}

Token *createKeywordToken(char *value, int lineno){
    
}

void lex(char *path){
    TwinBuffer *buffer = initTwinBuffer();
    setFilePointer(path, buffer);
    loadFileIntoBuffer(buffer);
    while(1){
        char character = getCharacter(buffer); 
        if
        
        if(character =='\0'){
            printf("\n");
            return;
        }
        printf("%c", character);
    }
    
}

int main(int argc, char const *argv[])
{   
    lex("help.pls");

    return 0;
}

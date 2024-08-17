#include "lexer.h"

extern Token *lexedData[5000];
static int lexPointer = 0;

void death(const char string[]){
    perror(string);
    exit(EXIT_FAILURE);
}

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

void setTerminalType(Token *token, Symbols term){
    token->terminal = term;
}

Symbols getKeywordTerminal(char *string){
    for(int i = 0; i < 25; i++){
        if(!strcmp(keywords[i].keyword, string)){
            return keywords[i].enumVal;
        }
    }
    return TK_FIELDID;
}

Token *createToken(char *value, int lineno, Symbols term){
    Token *token = initToken();
    setValue(token, value, strlen(value));
    setLineNum(token, lineno);
    setTerminalType(token, term);
    return token;
}

Token *createKeywordToken(char *value, int lineno){
    Symbols term = getKeywordTerminal(value);
    return createToken(value, lineno, term);
}

// TODO: Need to be able to detect multiple lexing errors 

void lex(char *path){
    TwinBuffer *buffer = initTwinBuffer();
    setFilePointer(path, buffer);
    loadFileIntoBuffer(buffer);
    while(1){
        int lineno = 0;
        char c = getCharacter(buffer);
        char val[IDLENGTH];
        memset(val, 0, sizeof(char)*IDLENGTH);
        int valPointer = 0;
        //Ignores comments
        if(c == '%'){
            while(1){
                char nextChar = getCharacter(buffer);
                if(nextChar == '\n'){
                    lineno++;
                    break;
                }
            }
        }
        else {
            if(c == '='){
                char nextChar = getCharacter(buffer);
                if(nextChar == '='){
                    lexedData[lexPointer++] = createToken("==",lineno,TK_EQ);
                }
            } else if (c == '<') {
                char nextChar = getCharacter(buffer);
                if(nextChar == '-'){ 
                    if(getCharacter(buffer) == '-'){
                        if(getCharacter(buffer) == '-'){
                            lexedData[lexPointer++] = createToken("<---", lineno, TK_ASSIGNOP);
                        }
                    }
                } else if (nextChar == '='){
                    lexedData[lexPointer++] = createToken("<=", lineno, TK_LE);
                } else {
                    
                }
            } else if (c == '>'){

            }



            // printf("%c", c);
        }
        if(c =='\0'){
            printf("\n");
            return;
        }
    }
    
}

int main(int argc, char const *argv[])
{   
    lex("help.pls");

    return 0;
}

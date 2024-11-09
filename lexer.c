
#include <ctype.h>
#include "lexer.h"
#include "datastructures/buffer.h"

extern Token *lexedData[5000];
static int lexPointer = 0;

void death(const char string[]){
    perror(string);
    exit(EXIT_FAILURE);
}

Token initToken(){
    Token token = (Token)malloc(sizeof(struct Token));
    memset(token, 0, sizeof(struct Token));
    return token;
}

void setValue(Token token, char *value, int size){
    memset(token->value, 0, IDLENGTH*sizeof(char));
    memcpy(token->value, value, size*sizeof(char));
}

void setLineNum(Token token, int lineno){
    token->lineno = lineno;
}

void setTerminalType(Token token, Terminal term){
    token->terminal = term;
}

Terminal getKeywordTerminal(char *string){
    // lex_keywords is an array of Keyword - enum mappings
    for(int i = 0; i < 25; i++){
        if(!strcmp(lex_keywords[i].keyword, string)){
            return lex_keywords[i].enumVal.t;
        }
    }
    return TK_ERROR;
}

Token createToken(char *value, int lineno, Terminal term){
    Token token = initToken();
    setValue(token, value, strlen(value));
    setLineNum(token, lineno);
    setTerminalType(token, term);
    return token;
}

Token createKeywordToken(char *value, int lineno){
    Terminal term = getKeywordTerminal(value);
    return createToken(value, lineno, term);
}

// TODO: Need to be able to detect multiple lexing errors 

int lineno = 0;

// lex one word at a time buffer should be modified automatically.
Token lex(TwinBuffer fullBuffer){
    // while might not be needed not here atleast will see TODO
    while(1){
        char c = getCharacter(fullBuffer);
        // store the word to put into token
        char val[IDLENGTH];
        memset(val, 0, sizeof(char)*IDLENGTH);
        int valPointer = 0; // valpointer denotes position of last char 
                            // of val
        // Comments are ignored
        if(c == '%'){
            while(1){
                char c = getCharacter(fullBuffer);
                if(c == '\n'){
                    lineno++;
                    return NULL;
                }
                if(c == '\0'){
                    //EOF encountered, create and leave 
                    return createToken("\0", lineno, TK_EOF);
                    // exits lex :D
                }
            }
        }

        if(isalpha(c)){
            // get word from source :thumbsup:
            val[valPointer++] = c;
            c = getCharacter(fullBuffer);
            while(isalnum(c)) {
                val[valPointer++] = c;
                c = getCharacter(fullBuffer);
            }
            val[valPointer] = '\0';
            goBackOne(fullBuffer);
            // check if word is a keyword
            Terminal keywordTerm = getKeywordTerminal(val);
            /* if terminal is TK_ERROR, check if it is TK_ID or
               TK_FIELDID */
            if(keywordTerm == TK_ERROR){
                /* 
                    Will implement diferentiation.
                    TK_ID implies [b-d][2-7][b-d]*[2-7]*
                    TK_FIELDID implies [a-z][a-z]*
                    How do I differentiate?
                    Walk through the string? 
                */
                // first check if it is a TK_ID 
                int counter = 2;
                // first 2 characters are bound.
                if(val[0] >= 'b' && val[0] <= 'd'){
                    if(val[1] >= '2' && val[1] <= '7'){
                        int letterMatch = 1;
                        while(counter < valPointer){
                            //check [b-d]*
                            if(letterMatch && !(val[counter] >= 'b' && val[counter] <= 'd')){
                                letterMatch = 0;
                            }
                            // if past [b-d]*, look for [2-7]*
                            if(!letterMatch && !(val[counter] >= '2' && val[counter] <= '7')){
                                    break;
                            }
                            counter++;
                        }
                    }   
                }
                if(counter == valPointer){
                        return createToken(val, lineno, TK_ID);
                }
                // Now check if it is TK_FIELDID
                counter = 1;
                //first character has to be a-z
                if(val[0] >= 'a' && val[0] <= 'z'){
                    while(counter < valPointer){
                        if(!(val[counter] >= 'a' && val[counter] <= 'z')){
                            break;
                        }
                        counter++;
                    }
                }
                if(counter == valPointer){
                    return createToken(val, lineno, TK_FIELDID);
                }
                // If it isn"t TK_ID or TK_FIELDID, it"s an error 
                // Go fix it. :D
                return createToken(val, lineno, TK_ERROR);
            } else {
                return createToken(val, lineno, keywordTerm);
            }
        }
        // if it starts with a "_", it can only be TK_FUNID or TK_MAIN
        if(c == '_'){
            // read the keyword.
            val[valPointer++] = c;
            c = getCharacter(fullBuffer);
            while(isalnum(c)){
                val[valPointer++] = c;
                c = getCharacter(fullBuffer);
            }
            val[valPointer] = '\0';
            goBackOne(fullBuffer);
            // Now we have two options. 
            // _main or _[alpha][alpha]*[digit]*
            // check if _main -> TK_MAIN
            if(!strcmp(val, "_main")){
                return createToken(val, lineno, TK_MAIN);
            }
            // Not TK_MAIN. check if TK_FUNID
            int counter = 1;
            if(isalpha(val[counter])){
                counter++; // counter = 2 works too :P
                while(counter < valPointer){
                    int letterMatch = 1;
                    // check if not a letter until it occurs once 
                    if(letterMatch && !isalpha(val[counter])){
                        letterMatch = 0;    
                    }
                    // check if not a digit only once it a letter doesn't occur
                    if(!letterMatch && !isdigit(val[counter])){
                        break;
                    }
                    counter++;
                }
            }
            if(counter == valPointer){
                return createToken(val, lineno, TK_FUNID);
            }
            // Not following standards is error 
            // (probably can externalise this)
            return createToken(val, lineno, TK_ERROR);
        }
        // Now we check numbers cuz why not?
        if(isdigit(c)){
            // two options TK_RNUM or TK_NUM
            // TK_NUM [digit][digit]* (not hard)
            // get string representation of the number
            val[valPointer++] = c;
            c = getCharacter(fullBuffer);
            while(!isspace(c)){
                val[valPointer++] = c;
                c = getCharacter(fullBuffer);
            }
            goBackOne(fullBuffer);
            // I need to add an go back since buffer is now at space.
            // will skip new lines. kinda cringe will fix 
            // (same for all other ones)
            // Now, we have the next value (ends at space), Check for 
            // TK_NUM [digit][digit]* 

            // I know we're kinda double checking. 
            // If it works it works. :shrugs:
            int counter = 1;
            // we know first value is a digit. 4head 
            while(counter < valPointer){
                if(!isdigit(val[counter])){
                    break;
                }
                counter++;
            }
            if(counter == valPointer){
                return createToken(val, lineno, TK_NUM);
            }
            // Now onto TK_RNUM 
            //  [digit][digit]*[.][digit][digit][E][+|-|][0-9][0-9]
            int isTKRNUM = 1;
            counter = 1;
            //[digit]*
            while(counter < valPointer){
                if(!isdigit(val[counter])){
                    break;
                }
                counter++;
            }
            // [.]
            if(!(val[counter++]=='.')){
                return createToken(val, lineno, TK_ERROR);
            }
            // [digit]
            if(!isdigit(val[counter++])){
                return createToken(val, lineno, TK_ERROR);
            }
            // [digit]
            if(!isdigit(val[counter++])){
                return createToken(val, lineno, TK_ERROR);
            }
            // [E]
            if(!(val[counter++] == 'E')){
                return createToken(val, lineno, TK_ERROR);
            }
            if(!(
                val[counter] == '+' ||
                val[counter] == '-' || 
                isdigit(val[counter])
            )){
                return createToken(val, lineno, TK_ERROR);
            } else if((val[counter] == '+' || val[counter] == '-')){
                counter++;
            }
            // [digit]
            if(!isdigit(val[counter++])){
                return createToken(val, lineno, TK_ERROR);
            }
            // [digit]
            if(!isdigit(val[counter++])){
                return createToken(val, lineno, TK_ERROR);
            }
            // I probably should make this so that it cuts off here
            while(counter < valPointer){
                val[counter++] = '\0'; // this works?
            }
            return createToken(val, lineno, TK_RNUM);
        }
        // TK_FIELDID #[a-z][a-z]*
        if(c == '#'){
            val[valPointer++] = c;
            c = getCharacter(fullBuffer);
            while(!isspace(c)){
                val[valPointer++] = c;
                c = getCharacter(fullBuffer);
            }
            goBackOne(fullBuffer);
            if(islower(val[0])){
                int counter = 1;
                while(counter < valPointer){
                    if(!islower(val[counter])){
                        break;
                    }
                    counter++;
                }
                if(counter == valPointer){
                    return createToken(val, lineno, TK_RUID);
                }
            }
            return createToken(val, lineno, TK_ERROR);
        }
        // multicharacter fixed tokens
        Terminal singleTerm;
        int isSingleMatch = 0; 
        switch (c){
            case '(': singleTerm=TK_OP; isSingleMatch = 1; break;
            case ')': singleTerm=TK_CL; isSingleMatch = 1; break;
            case '[': singleTerm=TK_SQL; isSingleMatch = 1; break;
            case ']': singleTerm=TK_SQR; isSingleMatch = 1; break;
            case '+': singleTerm=TK_PLUS; isSingleMatch = 1; break;
            case '-': singleTerm=TK_MINUS; isSingleMatch = 1; break;
            case '*': singleTerm=TK_MUL; isSingleMatch = 1; break;
            case '/': singleTerm=TK_DIV; isSingleMatch = 1; break;
            case ',': singleTerm=TK_COMMA; isSingleMatch = 1; break;
            case ';': singleTerm=TK_SEM; isSingleMatch = 1; break;
            case ':': singleTerm=TK_COLON; isSingleMatch = 1; break;
            case '.': singleTerm=TK_DOT; isSingleMatch = 1; break;
            case '~': singleTerm=TK_NOT; isSingleMatch = 1; break;
        }
        if(isSingleMatch){
            val[0] = c;
            val[1] = '\0';
            return createToken(val, lineno, singleTerm);
        }
        // Now multicharacter tokens or combinations
        // <---, &&&, @@@, <, <=, ==. >=, >, !=
        // first let's do < and derivatives
        if(c == '<'){
            char c1 = getCharacter(fullBuffer);
            // <--- TK_ASSIGNOP
            if(c1 == '-'){
                char c2 = getCharacter(fullBuffer);
                if(c2 == '-'){
                    char c3 = getCharacter(fullBuffer);
                    if(c3 == '-'){
                        return createToken("<---", lineno, TK_ASSIGNOP);
                    }
                    goBackOne(fullBuffer);
                }
                goBackOne(fullBuffer);
            }
            // <= TK_LE
            if(c1 == '='){
                return createToken("<=", lineno, TK_LE);
            }
            // < TK_LT
            goBackOne(fullBuffer);
            return createToken("<", lineno, TK_LT);
        }
        // > and its derivatives
        if(c == '>'){
            char c1 = getCharacter(fullBuffer);
            // >= TK_GE
            if(c1 == '='){
                return createToken(">=", lineno, TK_GE);
            }
            // < TK_GT
            goBackOne(fullBuffer);
            return createToken(">", lineno, TK_GT);
        }
        // &&&
        if(c == '&'){
            char c1 = getCharacter(fullBuffer);
            if(c1 == '&'){
                char c2 = getCharacter(fullBuffer);
                if(c2 == '&'){
                    return createToken("&&&", lineno, TK_AND);
                }
                goBackOne(fullBuffer);
            }
            goBackOne(fullBuffer);
            return createToken("&", lineno, TK_ERROR);
        }
        // @@@
        if(c == '@'){
            char c1 = getCharacter(fullBuffer);
            if(c1 == '@'){
                char c2 = getCharacter(fullBuffer);
                if(c2 == '@'){
                    return createToken("@@@", lineno, TK_OR);
                }
                goBackOne(fullBuffer);
            }
            goBackOne(fullBuffer);
            return createToken("@", lineno, TK_ERROR);
        }
        // == 
        if(c == '='){
            char c1 = getCharacter(fullBuffer);
            if(c1 == '='){
                return createToken("==", lineno, TK_EQ);
            }
            goBackOne(fullBuffer);
            return createToken("=", lineno, TK_ERROR);
        }
        // !=
        if(c == '!'){
            char c1 = getCharacter(fullBuffer);
            if(c1 == '='){
                return createToken("!=", lineno, TK_NE);
            }
            goBackOne(fullBuffer);
            return createToken("!", lineno, TK_ERROR);
        }
        if(c == '\n'){
            lineno++;
        }
        if(c == '\0'){
            return createToken("\0", lineno, TK_EOF);
        }
        if(isspace(c)){
            return NULL;
        }
    }        
}


// we have a lexer that can read words now. I hope :P
// Now let's read a file.
void lexFile(char *inpPath, char *opPath){
    // setup the starting conditions
    lineno = 1;
    TwinBuffer fullBuffer = initTwinBuffer();
    setFilePointer(inpPath, fullBuffer);
    loadFileIntoBuffer(fullBuffer);
    FILE *opFile = fopen(opPath, "w+");
    while(1){
        Token newToken = lex(fullBuffer);
        if(!newToken){
            continue;
        }
        // Write into file. one every line works
        fprintf(opFile,
            "%s, %d, %d\n",
             newToken->value, newToken->lineno, newToken->terminal);
        if(newToken->terminal == TK_EOF){
            printf("File succesfully lexed\n");
            break;
        }
    }
    fclose(opFile);          
}

// Now we have a lexer for the whole file.. :thumbsup:

int main(int argc, char const *argv[])
{   
    lexFile("help.pls", "tokens.tkn");
    return 0;

}
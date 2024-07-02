#ifndef PARSER_H  
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper/definitions.h"

#define MAXPRODLEN 8 
#define MAXPRODCOUNT 100


typedef struct ProdRules{
    Symbols LHS;
    Symbols RHS[MAXPRODLEN];
    int RHScount;
} ProdRules;

Symbols stringToSymbols(char string[]){
    for(int i = 0; i < 111; i++){
        if(!strcmp(string, StringToSymbols[i].string)){
            return StringToSymbols[i].symbol;
        }
    }
    return 0;
}

const char *symbolToString(Symbols symbol){
    for (int i = 0; i < 111; i++){
        if(StringToSymbols[i].symbol == symbol){
            return StringToSymbols[i].string;
        }
    }
    return "";
}

int main(){
    FILE *fp = fopen("grammar.txt", "r");
    ProdRules *rules = (ProdRules *)(malloc(sizeof(ProdRules)*91));
    memset(rules, 0, sizeof(ProdRules)*91);
    char string[30];
    char c;
    int i = 0;
    while(!feof(fp)){
        fscanf(fp, "%29s", string);
        rules[i].LHS = stringToSymbols(string);
        // printf("%s ", string);
        c = fgetc(fp);
        int j = 0;
        while(c != '\n'){
            fscanf(fp, "%29s", string);
            rules[i].RHS[j++] = stringToSymbols(string);
            // printf("%s ", string);
            if(feof(fp)){
                break;
            }
            c = fgetc(fp);
        }
        rules[i].RHScount = j;
        i++;
        // printf("\n");
    }
    // return 0;
    for(int x = 0; i < 91; i++){
        printf("%s ", symbolToString(rules[x].LHS));
        for(int j = 0; j < rules[x].RHScount; j++){
            printf("%s ", symbolToString(rules[x].RHS[j]));
        }
        printf("\n");
    }
}
#endif

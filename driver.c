#include "lexer.h"
#include <stdio.h>


/*

    The main driver function - use this to run the program.
        1. Provides the tokenized file as <filename>.lexed
        else just exits. (No error handling yet)
*/
int main(int argc, char const *argv[]){
    int choice = -1;
    printf("1. Generate the tokenized file from the lexer",'\n');
    scanf(choice);
    switch (choice)
    {
    case 1: 
        break;
    
    default:
        printf("Invalid Choice\n");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

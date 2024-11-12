#ifndef CODEGEN_H
#define CODEGEN_H

#define LINELENGTH 100
#define NULL 0
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "cJSON.h"

typedef struct code_line {
    char value[LINELENGTH];
    struct code_line* next;
} code_line;

typedef struct code_block {
    code_line *top;
    code_line *bottom;
} code_block;


// Forward declaration for parseJSONToTree function
ParseTreeNode *parseJSONToTree(cJSON *json, ParseTreeNode *parent);

// Function to map a string to the Symbols enum
Symbols mapStringToSymbol(const char *str, boolean *isTerminal) {
    Symbols symbol;
    if (strcmp(str, "nt_program") == 0) {
        symbol.nt = nt_program;
        *isTerminal = FALSE;
    } else if (strcmp(str, "nt_mainFunction") == 0) {
        symbol.nt = nt_mainFunction;
        *isTerminal = FALSE;
    } else if (strcmp(str, "TK_PLUS") == 0) {
        symbol.t = TK_PLUS;
        *isTerminal = TRUE;
    } else if (strcmp(str, "TK_MINUS") == 0) {
        symbol.t = TK_MINUS;
        *isTerminal = TRUE;
    } else {
        // Set default for unknown symbols
        symbol.t = TK_ERROR;
        *isTerminal = TRUE;
    }
    return symbol;
}

// Function to read the parse tree from JSON and convert it into ParseTreeNode structure
ParseTreeNode *readTreefromJSON(char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = (char *)malloc(length + 1);
    if (!data) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(data);
    free(data);

    if (!json) {
        fprintf(stderr, "Error before: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    // Print the JSON structure for verification
    char *printedJSON = cJSON_Print(json);
    if (printedJSON) {
        printf("Parsed JSON:\n%s\n", printedJSON);
        free(printedJSON);
    }

    ParseTreeNode *root = parseJSONToTree(json, NULL);
    cJSON_Delete(json);
    return root;
}

// Function to parse cJSON object into a ParseTreeNode structure
ParseTreeNode *parseJSONToTree(cJSON *json, ParseTreeNode *parent) {
    if (!json) return NULL;

    cJSON *nodeItem = cJSON_GetObjectItem(json, "node");
    cJSON *childrenItem = cJSON_GetObjectItem(json, "child");

    if (!nodeItem || !cJSON_IsString(nodeItem)) {
        return NULL;
    }

    ParseTreeNode *newNode = (ParseTreeNode *)malloc(sizeof(ParseTreeNode));
    newNode->isTerminal = 0;  // Assume non-terminal by default; set accordingly in logic
    newNode->symbol = mapStringToSymbol(nodeItem->valuestring, &(newNode->isTerminal)); // Assuming mapStringToSymbol is defined in parser.h
    strncpy(newNode->lexeme, nodeItem->valuestring, 99);
    newNode->lexeme[99] = '\0'; // Ensure null-termination
    newNode->childCount = cJSON_IsArray(childrenItem) ? cJSON_GetArraySize(childrenItem) : 0;
    newNode->children = (newNode->childCount > 0) ? (ParseTreeNode **)malloc(newNode->childCount * sizeof(ParseTreeNode *)) : NULL;

    if (childrenItem && cJSON_IsArray(childrenItem)) {
        int i = 0;
        cJSON *child;
        cJSON_ArrayForEach(child, childrenItem) {
            newNode->children[i++] = parseJSONToTree(child, newNode);
        }
    }

    return newNode;
}

code_line *init_code_line(char *value, code_line *next) {
    code_line *line = (code_line *)malloc(sizeof(code_line));
    strcpy(line->value, value);
    line->next = next;
    return line;
}

code_line *init_code_line_val(char *value) {
    return init_code_line(value, NULL);
}

// code_line *init_code_line() {
//     return init_code_line("", NULL);
// }

code_block *init_code_block(code_line *top, code_line *bottom) {
    code_block *block = (code_block *)malloc(sizeof(code_block));
    block->top = top;
    block->bottom = bottom;
    return block;
}

code_block *arithmetic_expression(ParseTreeNode *node) {
    // Implementation for generating code from an arithmetic expression parse tree
    if (!node) return NULL;

    code_block *block = init_code_block(NULL, NULL);
    if (node->isTerminal) {
        block->top = init_code_line_val(node->lexeme);
        block->bottom = block->top;
    } else {
        for (int i = 0; i < node->childCount; i++) {
            code_block *childBlock = arithmetic_expression(node->children[i]);
            if (childBlock) {
                if (!block->top) {
                    block->top = childBlock->top;
                    block->bottom = childBlock->bottom;
                } else {
                    block->bottom->next = childBlock->top;
                    block->bottom = childBlock->bottom;
                }
            }
        }
    }

    return block;
}

// Function to print code lines from a code block
void print_code_block(code_block *block) {
    if (!block || !block->top) {
        printf("Empty code block\n");
        return;
    }

    code_line *current = block->top;
    while (current) {
        printf("%s\n", current->value);
        current = current->next;
    }
}
void printParseTree(ParseTreeNode* node, int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; ++i) printf("  ");
    printf("%s\n", node->lexeme);
    for (int i = 0; i < node->childCount; ++i) {
        printParseTree(node->children[i], level + 1);
    }
}


int main(){
    ParseTreeNode *node = readTreefromJSON("./testfiles/codegen/arithExpr.ast");
    printParseTree(node, 10);
    return 0;
}

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "astgen.c" // Assuming parser.h includes necessary definitions
#include "helper/definitions.h"

// Create 3 symbol tables, one for functions, one for symbols, one for 
// records/unions

// Define a simple enum for type information
typedef enum {
    TYPE_INT,
    TYPE_REAL,
    TYPE_RECORD,
    TYPE_UNION
} DataType;



// Symbol table entry structure
typedef struct SymbolTableEntry {
    char name[100];
    char fnRef[100];
    DataType dataType;
    int scope;
    int size;
    struct SymbolTableEntry* next;
} SymbolTableEntry;

typedef struct Field {
    char fieldId[30];
    DataType fieldType;
    char consName[30];
    struct Field *next;
} Field;

typedef struct RecordTableEntry {
    int isUnion;
    char recordID[100];
    Field **fields;
    int fieldCount;
    struct RecordTableEntry *next;
} RecordTableEntry;

// Symbol table structure (using chaining for collision resolution)
typedef struct SymbolTable {
    SymbolTableEntry** table;
    int size;
} SymbolTable;

typedef struct RecordTable {
    RecordTableEntry** table;
    int size;
} RecordTable;

// Function to create a new symbol table entry
SymbolTableEntry* createSymbolTableEntry(char* name, char *fnRef, DataType dataType, int scope) {
    SymbolTableEntry* entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
    strcpy(entry->name, name);
    entry->scope = scope; // 1 if function par, else 0 
    if(scope)
        strcpy(entry->fnRef,fnRef); // g
    entry->dataType = dataType; // typing for symbol, is it int
    entry->size = 4; // size in bytes (currently same for int and real)
    entry->next = NULL; // for chaining (linked list)
    return entry;
}

RecordTableEntry *initRecordTableEntry(char *recordID, int isUnion){
    RecordTableEntry *newRecord = (RecordTableEntry *)malloc(sizeof(RecordTableEntry));
    newRecord->isUnion = isUnion;
    newRecord->fieldCount=0;
    newRecord->fields = NULL;
    newRecord->next = NULL;
    return newRecord;
}


// Function to create a symbol table
SymbolTable* createSymbolTable(int size, int isSymbolTable) {
    SymbolTable* symTable = (SymbolTable*)malloc(sizeof(SymbolTable));
    symTable->table = (SymbolTableEntry**)malloc(size * sizeof(SymbolTableEntry*));
    for (int i = 0; i < size; i++) {
        symTable->table[i] = NULL;
    }
    symTable->size = size;
    return symTable;
}

// Hash function
unsigned int hash(const char* str, int size) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % size;
}

Field *initField(){
    Field *newField = (Field *)malloc(sizeof(Field));
    memset(newField, 0, sizeof(Field));
    return newField;
}
createRecordTableEntry(){}

// Function to insert a symbol into the symbol table
void insertSymbolTableEntry(SymbolTable* table, SymbolTableEntry *entry) {
    unsigned int index = hash(entry->name, table->size);
    entry->next = table->table[index];
    table->table[index] = entry;
}
void insertRecordTableEntry(RecordTable *table, RecordTableEntry *entry){
    unsigned int index = hash(entry->recordID, table->size);
    entry->next = table->table[index];
    table->table[index] = entry;
}

// Function to lookup a symbol in the symbol table
SymbolTableEntry* lookupSymbol(SymbolTable* symTable, const char* name) {
    unsigned int index = hash(name, symTable->size);
    SymbolTableEntry* entry = symTable->table[index];
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

// Function to print the symbol table
void printSymbolTable(SymbolTable* symTable) {

    for (int i = 0; i < symTable->size; i++) {
        SymbolTableEntry* entry = symTable->table[i];
        if (entry) {
            printf("Index %d:\n", i);
            while (entry) {
                printf("  Name: %s, Type: %d, DataType: %d, Scope: %d\n", entry->name, entry->type, entry->dataType, entry->scope);
                entry = entry->next;
            }
        }
    }
}

// Define scope and the symbol table
int currentScope = 0;
SymbolTable* symTable;


// Function to determine the data type of a node
DataType getDataType(AstTreeNode* node) {
    
    if (node->symbol.t == TK_INT) return TYPE_INT;
    if (node->symbol.t == TK_REAL) return TYPE_REAL;
    // Handle constructed datatypes too, needs to complete type def for that
    return TYPE_UNDEFINED;
}

// Function to check if two data types are compatible
int areTypesCompatible(DataType type1, DataType type2) {
    // For simplicity, assuming only INT and REAL types
    if (type1 == type2) return 1;
    if (type1 == TYPE_INT && type2 == TYPE_REAL) return 1; // Implicit conversion
    if (type1 == TYPE_REAL && type2 == TYPE_INT) return 1; // Implicit conversion
    return 0;
}

// Function to traverse the parse tree and perform semantic analysis and type checking
void semanticAnalysis(SymbolTable *symbolTable, RecordTable *recordTable, AstTreeNode* node, char *fnRef) {
    if (!node) return;
    int scope = 0;
    if (fnRef){
        scope = 1;
    }
    // Handle different types o bgvfcf nodes (I don't handle function as of now)
    if(!node->isTerminal){
        switch (node->symbol.nt) {
            case nt_typeDefinitions:
                for(int i = 0; i < node->childCount; i++){
                    AstTreeNode *definitionNode = node->children[i];
                    int isUnion = !(strcmp(definitionNode->children[0]->lexeme, "union"));
                    char *consName = definitionNode->children[1]->lexeme; 
                    AstTreeNode *fieldNodes = node->children[2];
                    Field **fieldList = NULL;
                    for(int i = 0; i < fieldNodes->childCount; i++){
                        AstTreeNode *fieldDefNode = fieldNodes->children[i];
                        Field *field = initField();
                        strcpy(field->fieldId,fieldDefNode->children[fieldDefNode->childCount - 1]->lexeme);
                        /*
                        One way to do this is by adding a column on the symbol table that indicates whether the type 
                        is defined, for each field definition. Then, after populating the table, go through all the
                        table rows that have this value set to False, check if the type records for these types are 
                        available, and then set to True. If any row remains with the value set to False, throw an 
                        error.
                         */
                        if(!strcmp(fieldDefNode->children[0]->lexeme, "int")){
                            field->fieldType = TYPE_INT;
                        } else if (!strcmp(fieldDefNode->children[0]->lexeme, "real")){
                            field->fieldType = TYPE_REAL;
                        } else if (!strcmp(fieldDefNode->children[0]->lexeme, "record")){
                            field->fieldType = TYPE_RECORD;
                            strcpy(field->consName, fieldDefNode->children[fieldDefNode->childCount - 2]->lexeme);
                        } else if (!strcmp(fieldDefNode->children[0]->lexeme, "union")){
                            field->fieldType = TYPE_UNION;
                            strcpy(field->consName, fieldDefNode->children[fieldDefNode->childCount - 2]->lexeme);
                        }
                        field->next = *fieldList;
                        fieldList = &field;
                    } 
                    RecordTableEntry *entry = initRecordTableEntry(consName, isUnion);
                    entry->fieldCount = fieldNodes->childCount;
                    entry->fields = fieldList;
                    insertRecordTableEntry(recordTable, entry);
                }

                return;
            case nt_declarations: {
                // Go through all the children of nt_declarations 
                // child order is nt_declaration: type, varname
                for(int i = 0; i < node->childCount; i++){
                    // Now at declarations node
                    AstTreeNode *varNode = node->children[i];
                    DataType type = getDataType(varNode);
                    char *name = varNode->lexeme;
                    if(type != TYPE_RECORD || type != TYPE_UNION){
                        // primitive datatype, can be added directly
                        SymbolTableEntry *entry = createSymbolTableEntry(name, fnRef, type, scope);
                        // insert the entry into the symbol table
                        insertEntry(symbolTable, entry);
                    } else {
                        // lookup record  
                    }
                }
                // Check if the variable is already declared in the current scope
                // ParseTreeNode* idNode = node->children[1];
                // DataType dataType = getDataType(node->children[0]);
                // if (lookupSymbol(symTable, idNode->lexeme)) {
                //     printf("Semantic error: Variable '%s' is already declared.\n", idNode->lexeme);
                //     exit(1);
                // }
                // // Insert the variable into the symbol table
                // insertSymbol(symTable, idNode->lexeme, idNode->symbol, dataType, currentScope);
                // break;
                return ;
            }
            case nt_assignmentStmt: {
                // Check if the variable is declared before assignment
                ParseTreeNode* idNode = node->children[0]->children[0]; // Assuming singleOrRecId -> ID
                SymbolTableEntry* entry = lookupSymbol(symTable, idNode->lexeme);
                if (!entry) {
                    printf("Semantic error: Variable '%s' is not declared.\n", idNode->lexeme);
                    exit(1);
                }
                // Check if the assigned expression is compatible with the variable's type
                DataType exprType = getDataType(node->children[2]);
                if (!areTypesCompatible(entry->dataType, exprType)) {
                    printf("Type error: Incompatible types for variable '%s'.\n", idNode->lexeme);
                    exit(1);
                }
                break;
            }
            case nt_arithmeticExpression: {
                // Check type consistency in arithmetic expressions
                DataType leftType = getDataType(node->children[0]);
                DataType rightType = getDataType(node->children[1]);
                if (!areTypesCompatible(leftType, rightType)) {
                    printf("Type error: Incompatible types in arithmetic expression.\n");
                    exit(1);
                }
                break;
            }
            // Add more cases for other semantic checks and type checks as needed
            default:
                break;
        }
    }
    // Recursively analyze children
    for (int i = 0; i < node->childCount; i++) {
        semanticAnalysis(node->children[i], fnRef);
    }
}

// Main function
int main() {
    // Initialize the symbol table
    symTable = createSymbolTable(100);

    // Example token stream (should be replaced with actual tokens)
    Token exampleTokens[] = {
        {TK_MAIN, "_main"}, {TK_TYPE, "type"}, {TK_INT, "int"}, {TK_COLON, ":"},
        {TK_ID, "x"}, {TK_SEM, ";"}, {TK_ID, "x"}, {TK_ASSIGNOP, "<---"}, {TK_NUM, "10"}, {TK_SEM, ";"},
        {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
    };

    tokens = exampleTokens;
    tokenCount = sizeof(exampleTokens) / sizeof(exampleTokens[0]);

    // Parse the tokens and generate the parse tree
    ParseTreeNode* parseTree = program();

    // Perform semantic analysis and type checking
    semanticAnalysis(parseTree);

    // Print the symbol table
    printSymbolTable(symTable);

    printf("Parsing, semantic analysis, and type checking completed successfully.\n");
    return 0;
}

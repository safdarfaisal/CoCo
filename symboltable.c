#include "symboltable.h"

// Create 3 symbol tables, one for functions, one for symbols, one for 
// records/unions

// Define a simple enum for type information

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
SymbolTable* createSymbolTable(int size) {
    SymbolTable* symTable = (SymbolTable*)malloc(sizeof(SymbolTable));
    symTable->table = (SymbolTableEntry**)malloc(size * sizeof(SymbolTableEntry*));
    for (int i = 0; i < size; i++) {
        symTable->table[i] = NULL;
    }
    symTable->size = size;
    return symTable;
}

RecordTable *createRecordTable(int size){
    RecordTable *recTable = (RecordTable *)malloc(sizeof(RecordTable));
    recTable->table = (RecordTableEntry **)malloc(size * sizeof(RecordTableEntry *));
    memset(recTable,0, sizeof(recTable));
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
                printf("  Name: %s, Type: %d, DataType: %d, Scope: %d\n", entry->name, entry->fnRef, entry->dataType, entry->scope);
                entry = entry->next;
            }
        }
    }
}


// Function to determine the data type of a node
DataType getDataType(AstTreeNode* node) {
    
    if (!strcmp(node->children[0]->lexeme,"int")) return TYPE_INT;
    if (!strcmp(node->children[0]->lexeme,"real")) return TYPE_REAL;
    if (!strcmp(node->children[0]->lexeme,"record")) return TYPE_RECORD;
    if (!strcmp(node->children[0]->lexeme,"union")) return TYPE_UNION;
    // Handle constructed datatypes too, needs to complete type def for that
    return TYPE_UNDEFINED;
}

// // Function to check if two data types are compatible
// int areTypesCompatible(DataType type1, DataType type2) {
//     // For simplicity, assuming only INT and REAL types
//     if (type1 == type2) return 1;
//     if (type1 == TYPE_INT && type2 == TYPE_REAL) return 1; // Implicit conversion
//     if (type1 == TYPE_REAL && type2 == TYPE_INT) return 1; // Implicit conversion
//     return 0;
// }

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
                    if(type == TYPE_INT || type == TYPE_REAL){
                        // primitive datatype, can be added directly
                        SymbolTableEntry *entry = createSymbolTableEntry(name, fnRef, type, scope);
                        // insert the entry into the symbol table
                        insertSymbolTableEntry(symbolTable, entry);
                    } else {
                        // lookup record  TODO:
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
            break;
            case nt_function: {
                // enter nt_function field. Need to add vars to ip/op pars
                char *funcName = node->children[0]->lexeme;
                // get input parameters 
                AstTreeNode *inputParams = node->children[1];
                for(int i = 0; i<inputParams->childCount; i++){
                    AstTreeNode *varNode = inputParams->children[i];
                    DataType varType = getDataType(varNode);
                    char *varName = varNode->children[varNode->childCount - 1];
                    varName = strcat(".", varName);
                    varName = strcat(funcName, varName);
                    // var name is now func.var
                    if(varType == TK_RECORD || varType == TK_UNION){
                        // constructed
                        char *consName = "";
                        strcpy(consName, varNode->children[1]);
                        SymbolTableEntry *funcentry = createSymbolTableEntry(varName, funcName, varType, 1);
                        strcpy(funcentry->consName, consName);
                        insertSymbolTableEntry(symbolTable, funcentry);
                    } else {
                        // primitives
                        SymbolTableEntry *funcentry = createSymbolTableEntry(varName, funcName, varType, 1);
                        insertSymbolTableEntry(symbolTable, funcentry);
                    }
                }
                // get output parameters
                AstTreeNode *outputParams = node->children[2];
                for(int i = 0; i<outputParams->childCount; i++){
                    AstTreeNode *varNode = outputParams->children[i];
                    DataType varType = getDataType(varNode);
                    char *varName = varNode->children[varNode->childCount - 1];
                    varName = strcat(".", varName);
                    varName = strcat(funcName, varName);
                    // var name is now func.var
                    if(varType == TK_RECORD || varType == TK_UNION){
                        // constructed
                        char *consName = "";
                        strcpy(consName, varNode->children[1]);
                        SymbolTableEntry *funcentry = createSymbolTableEntry(varName, funcName, varType, 1);
                        strcpy(funcentry->consName, consName);
                        insertSymbolTableEntry(symbolTable, funcentry);
                    } else {
                        // primitives
                        SymbolTableEntry *funcentry = createSymbolTableEntry(varName, funcName, varType, 1);
                        insertSymbolTableEntry(symbolTable, funcentry);
                    }
                }
                // read statemtns from this point.
                semanticAnalysis(symbolTable, recordTable, node->children[3], funcName);
            }
            break;
            // case nt_assignmentStmt: {
            //     // Check if the variable is declared before assignment
            //     ParseTreeNode* idNode = node->children[0]->children[0]; // Assuming singleOrRecId -> ID
            //     SymbolTableEntry* entry = lookupSymbol(symTable, idNode->lexeme);
            //     if (!entry) {
            //         printf("Semantic error: Variable '%s' is not declared.\n", idNode->lexeme);
            //         exit(1);
            //     }
            //     // Check if the assigned expression is compatible with the variable's type
            //     DataType exprType = getDataType(node->children[2]);
            //     if (!areTypesCompatible(entry->dataType, exprType)) {
            //         printf("Type error: Incompatible types for variable '%s'.\n", idNode->lexeme);
            //         exit(1);
            //     }
            //     break;
            // }
            // case nt_arithmeticExpression: {
            //     // Check type consistency in arithmetic expressions
            //     DataType leftType = getDataType(node->children[0]);
            //     DataType rightType = getDataType(node->children[1]);
            //     if (!areTypesCompatible(leftType, rightType)) {
            //         printf("Type error: Incompatible types in arithmetic expression.\n");
            //         exit(1);
            //     }
            //     break;
            // }
            // Add more cases for other semantic checks and type checks as needed
            default:
                break;
        }
    }
    // Recursively analyze children
    for (int i = 0; i < node->childCount; i++) {
        semanticAnalysis(symbolTable, recordTable, node->children[i], fnRef);
    }
}

void run(AstTreeNode *node, SymbolTable *symtable, RecordTable *rectable){
    semanticAnalysis(symtable, rectable, node, NULL);
}


// Main function
// int main() {
//     // Initialize the symbol table
//     symTable = createSymbolTable(100);

//     // Example token stream (should be replaced with actual tokens)
//     Token exampleTokens[] = {
//         {TK_MAIN, "_main"}, {TK_TYPE, "type"}, {TK_INT, "int"}, {TK_COLON, ":"},
//         {TK_ID, "x"}, {TK_SEM, ";"}, {TK_ID, "x"}, {TK_ASSIGNOP, "<---"}, {TK_NUM, "10"}, {TK_SEM, ";"},
//         {TK_RETURN, "return"}, {TK_SEM, ";"}, {TK_END, "end"}, {EPSILON, ""}
//     };

//     tokens = exampleTokens;
//     tokenCount = sizeof(exampleTokens) / sizeof(exampleTokens[0]);

//     // Parse the tokens and generate the parse tree
//     ParseTreeNode* parseTree = program();

//     // Perform semantic analysis and type checking
//     semanticAnalysis(parseTree);

//     // Print the symbol table
//     printSymbolTable(symTable);

//     printf("Parsing, semantic analysis, and type checking completed successfully.\n");
//     return 0;
// }

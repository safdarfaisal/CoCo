#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "astgen.h" // Assuming astgen.h includes necessary definitions
#include "helper/definitions.h"


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
    char consName[30];
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

#endif
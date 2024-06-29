#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define TABLESIZE 40

typedef struct HashNode {
    int key;
    void *value;
} HashNode;

typedef struct HashTable{
    Llist *value[TABLESIZE]; // Linked list of hashNodes 
    int (*hash)(char *);    
} HashTable;

HashNode *initHashNode(int key, void *value);
void setHashFunc(HashTable *table, int (*hash)(char *));
HashTable *createHashTable(int (*hash)(char *));
void insert(HashTable *table, void *value, char *key);
void *getHashValue(HashTable *table, char *key, int (*cmp)(void *, void *));
void deleteValue(HashTable *table, char *key, int (*cmp)(void *, void *));

void freeHashNode(HashNode *node, void (*freeVal)(void *));
void freeTable(HashTable *table);

#endif
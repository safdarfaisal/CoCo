#include "hashtable.h"

HashNode *initHashNode(int key, void *value){
    HashNode *node = (HashNode *)(malloc(sizeof(HashNode)));
    node->key = key;
    node->value = value;
    return node;
}

HashTable *initHashTable(){
    HashTable *table = (HashTable *)(malloc(sizeof(table)));
    for(int i = 0; i < TABLESIZE; i++){
        (table->value)[i] = createLinkedList();
    }
    return table;
}

void setHashFunc(HashTable *table, int (*hash)(char *)){
    table->hash = hash;
}

HashTable *createHashTable(int (*hash)(char *)){
    HashTable *table = initHashTable();
    setHashFunc(table, hash);
    return table;
}

void insert(HashTable *table, void *value, char *key){
    int hash = (table->hash)(key);
    insertAtEnd((table->value[hash]), createNode(initHashNode(hash, value)));
}


// Comparator is externalised
void *getHashValue(HashTable *table, char *key, int (*cmp)(void *, void *)){
    int hash = (table->hash)(key);
    Llist *list = table->value[hash];
    void *value = ((HashNode *) searchForValue(list, key, cmp))->value;
    return value;
}

void deleteValue(HashTable *table, char *key, int (*cmp)(void *, void *)){
    int hash = (table->hash)(key);
    Llist *list = table->value[hash];
    int pos = getPos(list, key, cmp);
    removeNode(list, pos);
}

void freeHashNode(HashNode *node, void (*freeVal)(void *)){
    freeVal(node->value);
    free(node);
}

void freeTable(HashTable *table){
    for(int i = 0; i < TABLESIZE; i++){
        deleteList(table->value[i]);
    }
    free(table);
}



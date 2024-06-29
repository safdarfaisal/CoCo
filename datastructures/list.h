#ifndef LLIST_H
#define LLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct ListNode ListNode;

struct ListNode{
    void *data;
    ListNode *next;
};

typedef struct Llist{
    ListNode *head;
    int count;  
} Llist;


ListNode *createEmptyNode();
void populateNode(ListNode *node, void *val);
ListNode *createNode(void *val); 
Llist *createLinkedList();

/*TODO: Eventually externalise this to include destructors for the data
        inside the node. Currently has memory leak on data. */
void deleteNode(ListNode *node);
void deleteList(Llist *list);

void insertNode(Llist *list, ListNode *node, int position);
void insertAtStart(Llist *list, ListNode *node);
void insertAtEnd(Llist *list, ListNode *node);


void removeNode(Llist *list, int position);

void *getValue(Llist *list, int position);
void *getStartValue(Llist *list);
void *getEndValue(Llist *list);
void *searchForValue(Llist *list, void *value, int (*cmp)(void *, void *));
int getPos(Llist *list, void *value, int (*cmp)(void *, void *));



#endif
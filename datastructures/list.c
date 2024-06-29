#include "list.h" 



//Constructors
ListNode *createEmptyNode(){
    ListNode *newNode = (ListNode *) malloc(sizeof(ListNode));
    newNode->data = NULL;
    newNode->next = NULL;
    return newNode;
}

void populateNode(ListNode *node, void *val){
    node->data = val;
}

ListNode *createNode(void *val){
    ListNode *newNode = createEmptyNode();
    populateNode(newNode, val);
    return newNode;
}

Llist *createLinkedList(){
    Llist *newList = (Llist *)malloc(sizeof(Llist));
    memset(newList, 0, sizeof(Llist));
    return newList;
}


//Destructors

void deleteNode(ListNode *node){
    free(node);
}

void deleteList(Llist *list){
    if(!list){
        printf("List not found\n");
        return;
    }
    if(!list->head){
        free(list);
        return;
    }
    ListNode *currNode = list->head;
    ListNode *nextNode = currNode->next;

    while(list->count > 0){
        deleteNode(currNode);
        list->count--;
        currNode = nextNode;
        if(currNode->next != NULL){
            nextNode = currNode->next;
        }
    }
    free(list);
}

//Insert

// 0-Indexed position for list notation
void insertNode(Llist *list, ListNode *node, int position){
    assert(!list);
    assert(!node);
    assert(list->count >= position && position >= 0);

    if(list->count == 0){
        list->head = node;
    }

    if(position == 0){
        node->next = list->head;
        list->head = node;
    } else {
        ListNode *currNode = list->head;
        for(int i = 1; i < position; i++){
            currNode = currNode->next;
        }
        node->next = currNode->next;
        currNode->next = node;
    }
    list->count++;
}

void insertAtStart(Llist *list, ListNode *node){
    insertNode(list, node, 0);
}

void insertAtEnd(Llist *list, ListNode *node){
    insertNode(list, node, list->count);
}

//Remove
void removeNode(Llist *list, int position){
    if(!list){
        printf("No list exists\n");
        return ;
    }
    if (position < 0 || position >= list->count){
        return ;
    } else {
        ListNode *prevNode = list->head;
        ListNode *node = list->head;
        
        for(int i = 0; i < position - 1; i++){
            node->next = node->next;
        }

    }

}

//Find
void *getValue(Llist *list, int position){
    if (!list){
        printf("No list exists\n");
        return NULL;
    }
    if(position < 0 || position >= list->count){
        return NULL;
    }
    ListNode *node = list->head;
    for(int i = 0; i < position; i++){
        node = node->next;
    }

    return node->data;
}

void *getStartValue(Llist *list){
    return getValue(list, 0);
}

void *getEndValue(Llist *list){
    return getValue(list, list->count-1);
}

void *searchForValue(Llist *list, void *value, int (*cmp)(void *, void *)){
    if(!list){
        perror("No list found");
        exit(EXIT_FAILURE);
    }
    ListNode *node = list->head;
    while(node){
        if(cmp(node, value) != 0){
            return node->data;
        }
        node = node->next;
    }
    return NULL;
}

int getPos(Llist *list, void *value, int (*cmp)(void *, void *)){
    if(!list){
        perror("No list found");
        exit(EXIT_FAILURE);
    }
    ListNode *node = list->head;
    int i = 0;
    while(node){
        if(cmp(node, value) != 0){
            return i;
        }
        node = node->next;
        i++;
    }
    return -1;
}
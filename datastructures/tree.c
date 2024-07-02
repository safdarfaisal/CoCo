#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>

#define MAXCHILDCOUNT 8

#endif

typedef struct TreeNode{
    void *data;
    TreeNode *children[MAXCHILDCOUNT];
    int numOfChildren;
} TreeNode;

TreeNode *initNode(int numOfChildren){
    TreeNode *node = (TreeNode *)(malloc(sizeof(TreeNode)));
    node->children = (void **)(malloc(sizeof(void *)*MAXCHILDCOUNT));
    node->numOfChildren = numOfChildren;
    return node;
}

void addChild(TreeNode *node, TreeNode *child){

}
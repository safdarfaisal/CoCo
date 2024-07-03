#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define MAXCHILDCOUNT 8

typedef struct TreeNode{
    void *data;
    TreeNode *children[MAXCHILDCOUNT];
    int numOfChildren;
} TreeNode;

TreeNode *initNode();
void populateNode(TreeNode *node, void *data);
void addChild(TreeNode *, TreeNode *);
void deleteNode(TreeNode*);

#endif

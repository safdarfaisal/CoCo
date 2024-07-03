#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define MAXCHILDCOUNT 8

#endif


TreeNode *initNode(){
    TreeNode *node = (TreeNode *)(malloc(sizeof(TreeNode)));
    return node;
}

void populateNode(TreeNode *node, void *data){
    node->data = data;
}

void addChild(TreeNode *node, TreeNode *child){
    node->children[node->numOfChildren++] = child;
}

void deleteNode(TreeNode *node){
    free(node);
}
#include "tree.h"


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
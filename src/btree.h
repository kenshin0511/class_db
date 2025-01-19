#ifndef BTREE_H
#define BTREE_H

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct BinaryTree {
    Node* root;
} BinaryTree;

BinaryTree* create_tree();
void insert(BinaryTree* tree, int value);
void select(BinaryTree* tree, int value);
void free_tree(BinaryTree* tree);

#endif

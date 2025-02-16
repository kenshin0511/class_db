#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "btree.h"

// Insertion dans le B-Tree
void btree_insert(BTreeNode** root, const char* key) {
    if (*root == NULL) {
        *root = btree_create_node(key);
        return;
    }

    int cmp = strcmp(key, (*root)->key);
    if (cmp < 0) {
        btree_insert(&(*root)->left, key);
    } else if (cmp > 0) {
        btree_insert(&(*root)->right, key);
    }
}

// Recherche dans le B-Tree
int btree_search(const BTreeNode* root, const char* key) {
    if (root == NULL) {
        return 0;  // Clé non trouvée
    }

    int cmp = strcmp(key, root->key);
    if (cmp == 0) {
        return 1;  // Clé trouvée
    } else if (cmp < 0) {
        return btree_search(root->left, key);
    } else {
        return btree_search(root->right, key);
    }
}

// Affichage in-order (croissant)
void btree_print_inorder(const BTreeNode* root) {
    if (root == NULL) {
        return;
    }

    btree_print_inorder(root->left);
    printf("%s\n", root->key);
    btree_print_inorder(root->right);
}

// Création d'un nouveau nœud du B-Tree
BTreeNode* btree_create_node(const char* key) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    if (node == NULL) {
        perror("Erreur d'allocation mémoire pour BTreeNode");
        exit(EXIT_FAILURE);
    }
    strncpy(node->key, key, MAX_KEY_SIZE - 1);
    node->key[MAX_KEY_SIZE - 1] = '\0'; // Assurez-vous que la chaîne est terminée
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Libération de la mémoire allouée pour le B-Tree
void btree_free(BTreeNode* root) {
    if (root == NULL) {
        return;
    }

    // Libère récursivement les sous-arbres gauche et droit
    btree_free(root->left);
    btree_free(root->right);

    // Libère le nœud courant
    free(root);
}

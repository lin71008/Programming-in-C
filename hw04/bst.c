#include <stdlib.h>

#include "bst.h"

sBSTNode* sBSTNode_create(void *data)
{
    sBSTNode *newnode = (sBSTNode*) malloc(sizeof(sBSTNode));
    if (newnode == NULL) { return NULL; }
    newnode->data = data;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

void sBSTNode_delete(sBSTNode **root, void (*data_delete)(void*))
{
    if (root == NULL) { return; }
    if ((*root) == NULL) { return; }
    if ((*root)->left != NULL) { sBSTNode_delete(&((*root)->left), data_delete); }
    if ((*root)->right != NULL) { sBSTNode_delete(&((*root)->right), data_delete); }
    if ((*root)->data != NULL) { data_delete(&((*root)->data)); }
    free((*root));
    (*root) = NULL;
}

int32_t sBST_add(sBSTNode **root, void *data, int (*data_cmp)(const void*, const void*))
{
    if (root == NULL) { return 1; }
    if ((*root) == NULL)
    {
        sBSTNode *newnode = sBSTNode_create(data);
        if (newnode == NULL) { return 2; }
        (*root) = newnode;
        return 0;
    }
    else if (data_cmp(data, (*root)->data) ==  1) { return sBST_add(&((*root)->right), data, data_cmp); }
    else if (data_cmp(data, (*root)->data) == -1) { return sBST_add(&((*root)->left ), data, data_cmp); }
    else { return 3; }
}

int32_t sBST_remove_root_node(sBSTNode **root, void (*data_delete)(void*))
{
    if (root == NULL) { return 1; }
    if ((*root) == NULL) { return 0; }
    if ((*root)->left != NULL)
    {
        sBSTNode **newroot = sBST_rightmost(&((*root)->left));
        (*newroot)->left = (*root)->left;
        (*newroot)->right = (*root)->right;
        (*root)->left = NULL;
        (*root)->right = NULL;
        sBSTNode_delete(root, data_delete);
        (*root) = (*newroot);
        (*newroot) = NULL;
        return 0;
    }
    else
    {
        sBSTNode *newroot = (*root)->right;
        (*root)->right = NULL;
        sBSTNode_delete(root, data_delete);
        (*root) = newroot;
        return 0;
    }
}

int32_t sBST_remove_by_key(sBSTNode **root, const void *key, int (*key_cmp)(const void*, const sBSTNode*), void (*data_delete)(void*))
{
    if (root == NULL) { return 1; }
    if ((*root) == NULL) { return 2; }
    sBSTNode **target = sBST_find_by_key(root, key, key_cmp);
    if (target != NULL)
    {
        sBST_remove_root_node(target, data_delete);
        return 0;
    }
    return 3;
}

sBSTNode** sBST_leftmost(sBSTNode **root)
{
    if (root == NULL) return NULL;
    if ((*root) == NULL) return root;
    while ((*root)->left != NULL) { root = &((*root)->left); }
    return root;
}

sBSTNode** sBST_rightmost(sBSTNode **root)
{
    if (root == NULL) return NULL;
    if ((*root) == NULL) return root;
    while ((*root)->right != NULL) { root = &((*root)->right); }
    return root;
}

sBSTNode** sBST_find_by_key(sBSTNode **root, const void *key, int (*key_cmp)(const void*, const sBSTNode*))
{
    if (root == NULL) return NULL;
    while ((*root) != NULL)
    {
        int32_t cmp_result = 0;
        cmp_result = key_cmp(key, (*root)->data);
        if (cmp_result == -1) { root = &((*root)->left); }
        if (cmp_result == 1) { root = &((*root)->right); }
        else { break; }
    }
    if ((*root) != NULL) { return root; }
    else { return NULL; }
}

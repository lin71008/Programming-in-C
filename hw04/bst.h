#ifndef __BST_H__
#define __BST_H__

#include <stdint.h>

#pragma pack(push)
#pragma pack(1)
typedef struct _sBSTNode
{
    void *data;
    struct _sBSTNode *left;
    struct _sBSTNode *right;
} sBSTNode;
#pragma pack(pop)

extern sBSTNode* sBSTNode_create(void *data);
extern void sBSTNode_delete(sBSTNode **root, void (*data_delete)(void*));

// if success return zero, otherwise return non zero.
extern int32_t sBST_add(sBSTNode **root, void *data, int (*data_cmp)(const void*, const void*));
extern int32_t sBST_remove_root_node(sBSTNode **root, void (*data_delete)(void*));
extern int32_t sBST_remove_by_key(sBSTNode **root, const void *key, int (*key_cmp)(const void*, const sBSTNode*), void (*data_delete)(void*));

// if not exist return NULL
extern sBSTNode** sBST_leftmost(sBSTNode **root);
extern sBSTNode** sBST_rightmost(sBSTNode **root);
extern sBSTNode** sBST_find_by_key(sBSTNode **root, const void *key, int (*key_cmp)(const void*, const sBSTNode*));

// return amount
extern int32_t sBST_find_by_filter(sBSTNode **dest, const sBSTNode **root, int (*filter)(const void*));

#endif

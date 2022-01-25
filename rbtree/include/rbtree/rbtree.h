//
// Created by Anish Sinha on 1/18/22.
//

#ifndef REDBLACKTREE_RBTREE_H
#define REDBLACKTREE_RBTREE_H

typedef struct RBTree RBTree;
typedef struct RBNode RBNode;

RBTree *rbtree(void);
void insert(RBTree *, double);
void delete(RBTree *, double);
void inorder(RBTree *);
void preorder(RBTree *);
void postorder(RBTree *);
RBNode *root(RBTree *);
void test(RBTree *);
void test1(RBTree *, double);


#endif //REDBLACKTREE_RBTREE_H

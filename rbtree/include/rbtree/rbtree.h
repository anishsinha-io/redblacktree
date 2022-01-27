//
// Created by Anish Sinha on 1/18/22.
//

#ifndef REDBLACKTREE_RBTREE_H
#define REDBLACKTREE_RBTREE_H

#include <stdbool.h>

typedef struct RBTree RBTree;
typedef struct RBNode RBNode;

RBTree *rbtree(void);
void insert(RBTree *, double);
void delete(RBTree *, double);
void inorder(RBTree *);
void preorder(RBTree *);
void postorder(RBTree *);
RBNode *root(RBTree *);
bool test_suite_one(void);
bool test_suite_two(void);
bool test_suite_three(void);
bool test_suite_four(void);
bool test_suite_five(void);
bool test_suite_six(void);
void test1(RBTree *, double);


#endif //REDBLACKTREE_RBTREE_H

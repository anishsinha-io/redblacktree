#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definitions

typedef enum COLOR {
    black = 0, red = 1
} COLOR;

typedef struct RBNode {
    double val;
    struct RBNode *par;
    struct RBNode *left;
    struct RBNode *right;
    COLOR col;
} RBNode;

typedef struct RBTree {
    RBNode *root;
} RBTree;

// Constructors

RBNode *rbnode(double val, COLOR col) {
    RBNode *node = malloc(sizeof(RBNode));
    node->val = val;
    node->col = col;
    node->par = node->left = node->right = NULL;
    return node;
}

RBTree *rbtree() {
    RBTree *tree = malloc(sizeof(RBTree));
    tree->root = NULL;
    return tree;
}

// Auxiliary functions

static RBNode *rotate_left(RBNode *gr) {
    RBNode *par = gr->right;
    RBNode *ch = par->left;
    par->par = (gr->par) ? gr->par : NULL;
    par->left = gr;
    gr->par = par;
    ch && (ch->par = gr);
    gr->right = ch;
    return par;
}

static RBNode *rotate_right(RBNode *gr) {
    RBNode *par = gr->left;
    RBNode *ch = par->right;
    par->par = (gr->par) ? gr->par : NULL;
    par->right = gr;
    gr->par = par;
    ch && (ch->par = gr);
    gr->left = ch;
    return par;
}

static RBNode *bst_insert(RBNode **root, double val) {
    if (*root == NULL) {
        *root = rbnode(val, black);
        return *root;
    }
    RBNode *it = *root;
    RBNode *t;
    while (it != NULL) {
        t = it;
        if (it->val < val) it = it->right;
        else if (it->val > val) it = it->left;
    }
    RBNode *new_node = rbnode(val, red);
    if (val > t->val) t->right = new_node;
    else if (val < t->val) t->left = new_node;
    new_node->par = t;
    return new_node;
}

static RBNode *inorder_successor(RBNode *node) {
    RBNode *it = node->right;
    while (it && it->left != NULL) it = it->left;
    return it;
}

static RBNode *inorder_predecessor(RBNode *node) {
    RBNode *it = node->left;
    while (it && it->right != NULL) it = it->right;
    return it;
}

static void repair(RBNode **root, RBNode **new_node) {
    if (*root == *new_node) return;
    if ((*new_node)->par && (*new_node)->par->col == black) return;
    if (!(*new_node)->par || !(*new_node)->par->par) return;
    RBNode *par = (*new_node)->par;
    RBNode *gr = par->par;
    RBNode *unc, *great_gr = gr->par;
    if (gr->val < par->val) {
        unc = gr->left;
        if (unc && unc->col == red) {
            gr->col = (gr == *root) ? black : red;
            par->col = unc->col = black;
            repair(root, &gr);
        } else {
            if ((*new_node) == par->left) {
                RBNode *intermediate_par = rotate_right(par);
                gr->right = intermediate_par;
            }
            RBNode *new_par = rotate_left(gr);
            !new_par->par && (*root = new_par);
            gr->col = red;
            new_par->col = black;
            great_gr && (great_gr->right = new_par);
        }
    } else {
        unc = gr->right;
        if (unc && unc->col == red) {
            gr->col = (gr == *root) ? black : red;
            par->col = unc->col = black;
            repair(root, &gr);
        } else {
            if ((*new_node) == par->right) {
                RBNode *intermediate_par = rotate_left(par);
                gr->left = intermediate_par;
            }
            RBNode *new_par = rotate_right(gr);
            !new_par->par && (*root = new_par);
            gr->col = red;
            new_par->col = black;
            great_gr && (great_gr->left = new_par);
        }
    }
}

// Public api -> search/insert/delete

void insert(RBTree *tree, double val) {
    RBNode *new_node = bst_insert(&tree->root, val);
    repair(&(tree->root), &new_node);
}

// Public api -> traversal

void inorder_helper(RBNode *root) {
    if (root != NULL) {
        inorder_helper(root->left);
        printf("%f\t", root->val);
        inorder_helper(root->right);
    }
}

void inorder(RBTree *tree) {
    inorder_helper(tree->root);
}

void preorder_helper(RBNode *root) {
    if (root != NULL) {
        printf("%f\t", root->val);
        preorder_helper(root->left);
        preorder_helper(root->right);
    }
}

void preorder(RBTree *tree) {
    preorder_helper(tree->root);
}

void postorder_helper(RBNode *root) {
    if (root != NULL) {
        printf("%f\t", root->val);
        postorder_helper(root->left);
        postorder_helper(root->right);
    }
}

void postorder(RBTree *tree) {
    postorder_helper(tree->root);
}

// Test -> to check the validity of the test itself go to https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
// to visualize the tree in a prettier way than is present in this program

void test(RBTree *tree) {
    RBNode *root = tree->root;
    printf("%f | %d\n", root->val, root->col);
    printf("%f | %d\n", root->left->val, root->left->col);
    printf("%f | %d\n", root->left->left->val, root->left->left->col);
    printf("%f | %d\n", root->left->right->val, root->left->right->col);
    printf("%f | %d\n", root->left->left->left->val, root->left->left->left->col);
    printf("%f | %d\n", root->left->left->right->val, root->left->left->right->col);
    printf("\n");
    printf("%f | %d\n", root->right->val, root->right->col);
    printf("%f | %d\n", root->right->left->val, root->right->left->col);
    printf("%f | %d\n", root->right->right->val, root->right->right->col);
    printf("%f | %d\n", root->right->right->right->val, root->right->right->right->col);
    printf("\n");
}
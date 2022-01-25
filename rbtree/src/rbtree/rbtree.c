#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definitions

typedef enum COLOR {
    doubleblack = -1, black = 0, red = 1
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

RBNode *root(RBTree *tree) {
    return tree->root;
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

static bool is_left_child(RBNode *node) {
    if (!node->par) return false;
    if (node->par->val > node->val) return true;
    return false;
}

static bool is_leaf(RBNode *node) {
    return !(node->left || node->right);
}

static RBNode *non_null_child(RBNode *node) {
    if (node->left) return node->left;
    return node->right;
}

static void splice(RBNode *node) {
    if (is_left_child(node)) {
        node->par->left = NULL;
    } else node->par->right = NULL;
}

static bool has_red_child(RBNode *n) {
    if (is_leaf(n)) return false;
    if ((n->left && n->left->col == red) || (n->right && n->right->col == red)) return true;
    return false;
}

static void repair(RBNode **root, RBNode **new_node) {
    if (*root == *new_node) return;
    if ((*new_node)->par && (*new_node)->par->col == black) return;
    if (!(*new_node)->par || !(*new_node)->par->par) return;
    RBNode *par = (*new_node)->par;
    RBNode *gr = par->par;
    RBNode *unc, *great_gr = gr->par;
    if (!is_left_child(par)) {
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
            if (great_gr) {
                new_par->par = great_gr;
                if (new_par->val > great_gr->val) great_gr->right = new_par;
                else great_gr->left = new_par;
            }
            repair(root, &gr);
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
            if (great_gr) {
                new_par->par = great_gr;
                if (new_par->val > great_gr->val) great_gr->right = new_par;
                else great_gr->left = new_par;
            }
            repair(root, &new_par);
        }
    }
}

RBNode *search(RBNode *root, double val) {
    if (!root) return NULL;
    if (root->val == val) return root;
    if (root->val < val) return search(root->right, val);
    if (root->val > val) return search(root->left, val);
    return root;
}

// Public api -> search/insert/v_delete

void insert(RBTree *tree, double val) {
    RBNode *new_node = bst_insert(&tree->root, val);
    repair(&(tree->root), &new_node);
}

static RBNode *bst_splice(RBNode **root, double val) {
    RBNode *node = search(*root, val);
    if (!node) return NULL;
    if (is_leaf(node)) {
        splice(node);
        return node;
    } else if ((!node->left || !node->right) && !(node->left && node->right)) {
        RBNode *repl;
        if (node->left) repl = node->left;
        else repl = node->right;
        RBNode *spliced_node = bst_splice(root, repl->val);
        node->val = repl->val;
        return spliced_node;
    } else {
        RBNode *repl = inorder_predecessor(node);
        RBNode *spliced_node = bst_splice(root, repl->val);
        node->val = repl->val;
        return spliced_node;
    }
}

static void resolve_double_black(RBNode **root, RBNode *double_black_node) {
    if (*root == double_black_node) return;
    RBNode *sibling, *par = double_black_node->par, *gr = par->par;
    bool left_child = is_left_child(double_black_node);
    if (left_child) sibling = par->right;
    else sibling = par->left;
    if ((sibling && sibling->col == black) || !sibling) {
        RBNode *intermediate_par, *new_par;
        bool sib_left_child = is_left_child(sibling);
        if (is_leaf(sibling)) {
            if (par->col == red) {
                par->col = black;
                sibling && (sibling->col = red);
            } else {
                sibling && (sibling->col = red);
                resolve_double_black(root, par);
            }
            return;
        }
        if (is_left_child(double_black_node) && ((sibling && sibling->left) && sibling->left->col == red) &&
            !sibling->right) {
            intermediate_par = rotate_right(sibling);
            if (sib_left_child) par->left = intermediate_par;
            else par->right = intermediate_par;
            intermediate_par->col = black;
            intermediate_par->right->col = red;
        }
        if (!is_left_child(double_black_node) && ((sibling && sibling->right) && sibling->right->col == red) &&
            !sibling->left) {
            intermediate_par = rotate_left(sibling);
            if (sib_left_child) par->left = intermediate_par;
            else par->right = intermediate_par;
            intermediate_par->col = black;
            intermediate_par->left->col = red;
        }
        if (left_child) new_par = rotate_left(par);
        else new_par = rotate_right(par);
        new_par->col = red;
        if (new_par->left) new_par->left->col = black;
        if (new_par->right) new_par->right->col = black;
        if (!gr) {
            *root = new_par;
            new_par->col = black;
        } else {
            new_par->par = gr;
            if (new_par->val < gr->val) gr->left = new_par;
            else gr->right = new_par;
        }
    } else if (sibling->col == red && par->col == black) {
        RBNode *new_par;
        par->col = red;
        if (left_child) new_par = rotate_left(par);
        else new_par = rotate_right(par);
        if (!gr) {
            *root = new_par;
            new_par->col = black;
        } else {
            new_par->par = gr;
            if (new_par->val < gr->val) gr->left = new_par;
            else gr->right = new_par;
        }
        new_par->col = black;
        resolve_double_black(root, double_black_node);
    }
}


static void rb_delete(RBNode **root, double val) {
    RBNode *node = bst_splice(root, val);
    if (!node) return;
    if (node->col == red) return;
    if (node->col == black && *root == node) {
        *root = NULL;
        return;
    } else {
        resolve_double_black(root, node);
    }

}

void delete(RBTree *tree, double val) {
    rb_delete(&(tree->root), val);
}

// Public api -> traversal

void inorder_helper(RBNode *root) {
    if (root != NULL) {
        inorder_helper(root->left);
        printf("%f|%d\n", root->val, root->col);
        inorder_helper(root->right);
    }
}

void inorder(RBTree *tree) {
    inorder_helper(tree->root);
}

void preorder_helper(RBNode *root) {
    if (root != NULL) {
        printf("%f|%d\n", root->val, root->col);
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
    printf("%f | %d\n", root->right->val, root->right->col);
    printf("%f | %d\n", root->right->left->val, root->right->left->col);
    printf("%f | %d\n", root->right->right->val, root->right->right->col);
    printf("%f | %d\n", root->right->right->left->val, root->right->right->left->col);
    printf("%f | %d\n", root->right->left->right->val, root->right->left->right->col);
}

void test1(RBTree *tree, double val) {
    RBNode *n = search(tree->root, val);
    if (n) printf("%f\n", n->val);
    else printf("val %f not found", val);
}
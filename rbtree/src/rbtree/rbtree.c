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

RBNode *search(RBNode *root, double val) {
    if (!root) return NULL;
    if (root->val == val) return root;
    if (root->val < val) return search(root->right, val);
    if (root->val > val) return search(root->left, val);
    return root;
}

static RBNode *bst_insert(RBNode **root, double val) {
    RBNode *n = search(*root, val);
    if (n) return NULL;
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

static void splice(RBNode *node) {
    if (is_left_child(node)) {
        node->par->left = NULL;
    } else node->par->right = NULL;
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



// Public api -> search/insert/v_delete

void insert(RBTree *tree, double val) {
    RBNode *new_node = bst_insert(&tree->root, val);
    if (new_node) repair(&(tree->root), &new_node);
}

static RBNode *bst_splice(RBNode **root, double val) {
    if ((*root)->val == val) return *root;
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

static RBNode *sibling(RBNode *node) {
    if (!node || !node->par) return NULL;
    if (is_left_child(node)) return node->par->right;
    return node->par->left;
}

static bool has_red_child(RBNode *node) {
    if ((node->left && node->left->col == red) || (node->right && node->right->col == red))
        return true;
    return false;
}

static bool red_left_child(RBNode *node) {
    if (!node || is_leaf(node)) return false;
    return (node->left && node->left->col == red);
}

static bool connect_grandparent(RBNode *gr, RBNode *node) {
    if (!gr) return false;
    if (node->val > gr->val) gr->right = node;
    else gr->left = node;
    node->par = gr;
    return true;
}

static void recolor_children(RBNode *node, COLOR col) {
    if (node->left) node->left->col = col;
    if (node->right) node->right->col = col;
}

static void resolve_double_black(RBNode **root, RBNode *double_black_node) {
    RBNode *sib = sibling(double_black_node), *par = double_black_node->par;
    if (double_black_node == *root) return;
    // sibling will always be defined here because a black node cannot have a nil sibling so just
    // exit program if that somehow happens:
    if (!sib) return;
    // the next line will not segfault because if the sibling is defined, the parent is also defined and can safely
    // be dereferenced
    RBNode *gr = par->par;
    // cases 1 through 4 --> sibling is black and has at least one red child
    if (sib->col == black && has_red_child(sib)) {
        // cases 1 and 2 --> node is a left child
        COLOR par_initial_col = par->col;
        if (is_left_child(double_black_node)) {
            if (red_left_child(sib)) {
                // if we're here, we need to rotate right about the sibling first, recolor, then left about the parent
                RBNode *intermediate_par = rotate_right(sib);
                par->right = intermediate_par;
                intermediate_par->col = black;
                intermediate_par->right->col = red;
            }
            RBNode *new_par = rotate_left(par);
            if (!connect_grandparent(gr, new_par)) {
                *root = new_par;
                (*root)->col = black;
                new_par->par = NULL;
            } else {
                new_par->col = par_initial_col;
                recolor_children(new_par, black);
            }
        } else {
            // cases 2 and 4 --> node is a right child
            if (!is_left_child(double_black_node)) {
                if (!red_left_child(sib)) {
                    // if we're here, we need to rotate left about the sibling first, recolor, then right about the parent
                    RBNode *intermediate_par = rotate_left(sib);
                    par->left = intermediate_par;
                    intermediate_par->col = black;
                    intermediate_par->left->col = red;
                }
                RBNode *new_par = rotate_right(par);
                if (!connect_grandparent(gr, new_par)) {
                    *root = new_par;
                    (*root)->col = black;
                    new_par->par = NULL;
                } else {
                    new_par->col = par_initial_col;
                    recolor_children(new_par, black);
                }
            }
        }
    } else if (sib->col == black && !has_red_child(sib)) {
        // case 5 --> if the parent is red, just recolor parent and sibling, otherwise recurse up the tree until we
        // hit a case we can handle, or we reach the root, in which case just return
        sib->col = red;
        if (par->col == red) par->col = black;
        else resolve_double_black(root, par);
    } else {
        // case 6 --> if we are here, the sibling is red. rotate and recurse
        RBNode *new_par;
        if (is_left_child(double_black_node)) new_par = rotate_left(par);
        else new_par = rotate_right(par);
        if (!connect_grandparent(gr, new_par)) {
            *root = new_par;
            (*root)->col = black;
            new_par->par = NULL;
            if (is_left_child(double_black_node)) new_par->left->col = red;
            else new_par->right->col = red;
            resolve_double_black(root, double_black_node);
        } else {
            new_par->col = black;
            if (is_left_child(double_black_node)) new_par->left->col = red;
            else new_par->right->col = red;
            resolve_double_black(root, double_black_node);
        }
    }
}


static void rb_delete(RBNode **root, double val) {
    RBNode *node = bst_splice(root, val);
    if (!node) return;
    if (node->col == red) return;
    if (node->col == black && *root == node) {
        *root = NULL;
        return;
    }
    resolve_double_black(root, node);
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

// Tests -> to check the validity of the tests, visit https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
// to visualize the tree yourself.

static RBTree *create_dummy_tree() {
    RBTree *tree1 = rbtree();
    insert(tree1, 20);
    insert(tree1, 1);
    insert(tree1, 19);
    insert(tree1, 2);
    insert(tree1, 5);
    insert(tree1, 8);
    insert(tree1, 33);
    insert(tree1, 3);
    insert(tree1, 22);
    insert(tree1, 17);
    insert(tree1, 4);
    insert(tree1, 23);
    insert(tree1, 16);
    insert(tree1, 27);
    insert(tree1, 11);
    insert(tree1, 6);
    insert(tree1, 7);
    insert(tree1, 25);
    insert(tree1, 14);
    insert(tree1, 9);
    return tree1;
}

bool test_suite_one() {
    RBTree *tree1 = create_dummy_tree();
    delete(tree1, 33);
    return (
            tree1->root->val == 5 && tree1->root->col == black &&
            tree1->root->right->val == 19 && tree1->root->right->col == red &&
            tree1->root->right->right->val == 22 && tree1->root->right->right->col == black &&
            tree1->root->right->right->left->val == 20 &&
            tree1->root->right->right->left->col == black &&
            tree1->root->right->right->right->val == 25 &&
            tree1->root->right->right->right->col == red &&
            tree1->root->right->right->right->left->val == 23 &&
            tree1->root->right->right->right->left->col == black &&
            tree1->root->right->right->right->right->val == 27 &&
            tree1->root->right->right->right->right->col == black
    );
}


bool test_suite_two() {
    RBTree *tree1 = create_dummy_tree();
    delete(tree1, 1);
    return (
            tree1->root->val == 5 && tree1->root->col == black &&
            tree1->root->left->val == 3 && tree1->root->left->col == black &&
            tree1->root->left->left->val == 2 && tree1->root->left->left->col == black &&
            tree1->root->left->right->val == 4 && tree1->root->left->right->col == black
    );
}

bool test_suite_three() {
    RBTree *tree1 = create_dummy_tree();
    delete(tree1, 4);
    delete(tree1, 3);
    return (
            tree1->root->val == 19 && tree1->root->col == black &&
            tree1->root->left->val == 8 && tree1->root->left->col == red &&
            tree1->root->left->left->val == 5 && tree1->root->left->left->col == black &&
            tree1->root->left->right->val == 16 && tree1->root->left->right->col == black &&
            tree1->root->left->left->left->val == 2 &&
            tree1->root->left->left->left->col == black &&
            tree1->root->left->left->right->val == 6 &&
            tree1->root->left->left->right->col == black &&
            tree1->root->left->right->left->val == 11 &&
            tree1->root->left->right->left->col == black &&
            tree1->root->left->right->right->val == 17 &&
            tree1->root->left->right->right->col == black &&
            tree1->root->left->left->left->left->val == 1 &&
            tree1->root->left->left->left->left->col == red &&
            tree1->root->left->left->right->right->val == 7 &&
            tree1->root->left->left->right->right->col == red
    );
}

bool test_suite_four() {
    RBTree *tree1 = create_dummy_tree();
    delete(tree1, 17);
    return (
            tree1->root->val == 5 && tree1->root->col == black &&
            tree1->root->right->val == 19 && tree1->root->right->col == red &&
            tree1->root->right->left->val == 8 && tree1->root->right->left->col == black &&
            tree1->root->right->right->val == 22 && tree1->root->right->right->col == black &&
            tree1->root->right->left->left->val == 6 &&
            tree1->root->right->left->left->col == black &&
            tree1->root->right->left->right->val == 11 &&
            tree1->root->right->left->right->col == red &&
            tree1->root->right->right->left->val == 20 &&
            tree1->root->right->right->left->col == black &&
            tree1->root->right->right->right->val == 27 &&
            tree1->root->right->right->right->col == red &&
            tree1->root->right->left->left->right->val == 7 &&
            tree1->root->right->left->left->right->col == red &&
            tree1->root->right->left->right->left->val == 9 &&
            tree1->root->right->left->right->left->col == black &&
            tree1->root->right->left->right->right->val == 16 &&
            tree1->root->right->left->right->right->col == black &&
            tree1->root->right->left->right->right->left->val == 14 &&
            tree1->root->right->left->right->right->left->col == red &&
            tree1->root->right->right->right->left->val == 23 &&
            tree1->root->right->right->right->left->col == black &&
            tree1->root->right->right->right->right->val == 33 &&
            tree1->root->right->right->right->right->col == black &&
            tree1->root->right->right->right->left->right->val == 25 &&
            tree1->root->right->right->right->left->right->col == red
    );
}

bool test_suite_five() {
    RBTree *tree1 = create_dummy_tree();
    delete(tree1, 20);
    return (
            tree1->root->val == 5 && tree1->root->col == black &&
            tree1->root->right->val == 19 && tree1->root->right->col == red &&
            tree1->root->right->left->val == 16 && tree1->root->right->left->col == black &&
            tree1->root->right->right->val == 27 && tree1->root->right->right->col == black &&
            tree1->root->right->left->left->val == 8 && tree1->root->right->left->left->col == red &&
            tree1->root->right->left->right->val == 17 && tree1->root->right->left->right->col == black &&
            tree1->root->right->left->left->left->val == 6 &&
            tree1->root->right->left->left->left->col == black &&
            tree1->root->right->left->left->right->val == 11 &&
            tree1->root->right->left->left->right->col == black &&
            tree1->root->right->left->left->left->right->val == 7 &&
            tree1->root->right->left->left->left->right->col == red &&
            tree1->root->right->left->left->right->left->val == 9 &&
            tree1->root->right->left->left->right->left->col == red &&
            tree1->root->right->left->left->right->right->val == 14 &&
            tree1->root->right->left->left->right->right->col == red &&
            tree1->root->right->right->left->val == 23 && tree1->root->right->right->left->col == red &&
            tree1->root->right->right->right->val == 33 && tree1->root->right->right->right->col == black &&
            tree1->root->right->right->left->left->val == 22 && tree1->root->right->right->left->left->col == black &&
            tree1->root->right->right->left->right->val == 25 && tree1->root->right->right->left->right->col == black
    );
}

bool test_suite_six() {
    RBTree *tree1 = create_dummy_tree();
    delete(tree1, 11);
    return (
            tree1->root->val == 5 && tree1->root->col == black &&
            tree1->root->right->left->val == 16 && tree1->root->right->left->col == black &&
            tree1->root->right->left->left->val == 8 && tree1->root->right->left->left->col == red &&
            tree1->root->right->left->right->val == 17 && tree1->root->right->left->right->col == black &&
            tree1->root->right->left->left->left->val == 6 && tree1->root->right->left->left->left->col == black &&
            tree1->root->right->left->left->right->val == 9 && tree1->root->right->left->left->right->col == black &&
            tree1->root->right->left->left->left->right->val == 7 &&
            tree1->root->right->left->left->left->right->col == red &&
            tree1->root->right->left->left->right->right->val == 14 &&
            tree1->root->right->left->left->right->right->col == red
    );
}


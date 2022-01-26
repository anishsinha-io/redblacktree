#include <stdio.h>
#include "rbtree/rbtree.h"

int main() {
    RBTree *tree1 = rbtree();
    insert(tree1, 1);
    insert(tree1, 2);
    insert(tree1, 3);
    insert(tree1, 4);
    insert(tree1, 5);
    insert(tree1, 6);
    delete(tree1, 1);
    test_suite_one(tree1);
    return 0;
}

// insert(tree1, 20);
// insert(tree1, 1);
// insert(tree1, 19);
// insert(tree1, 2);
// insert(tree1, 5);
// insert(tree1, 8);
// insert(tree1, 33);
// insert(tree1, 3);
// insert(tree1, 22);
// insert(tree1, 17);
// insert(tree1, 4);
// insert(tree1, 23);
// insert(tree1, 16);
// insert(tree1, 27);
// insert(tree1, 11);
// insert(tree1, 6);
// insert(tree1, 7);
// insert(tree1, 25);
// insert(tree1, 14);
// insert(tree1, 9);
// inorder(tree1);
// test_suite_one(tree1);
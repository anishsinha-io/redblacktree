#include <stdio.h>
#include "rbtree/rbtree.h"

int main() {
    RBTree *tree1 = rbtree();
    insert(tree1, 1);
    insert(tree1, 10);
    insert(tree1, 2);
    insert(tree1, 9);
    insert(tree1, 3);
    insert(tree1, 8);
    insert(tree1, 4);
    insert(tree1, 7);
    insert(tree1, 5);
    insert(tree1, 6);
    delete(tree1, 10);
    inorder(tree1);
    test(tree1);
    return 0;
}

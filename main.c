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
    insert(tree1, 7);
    insert(tree1, 8);
    insert(tree1, 9);
    insert(tree1, 10);
    insert(tree1, 11);
    insert(tree1, 12);
    insert(tree1, 13);
    insert(tree1, 14);
    insert(tree1, 15);
    insert(tree1, 16);
    insert(tree1, 17);
    inorder(tree1);
    return 0;
}

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
    delete(tree1, 5);
    inorder(tree1);
    return 0;
}

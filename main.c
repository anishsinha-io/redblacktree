#include <stdio.h>
#include "rbtree/rbtree.h"

int main() {
    RBTree *tree1 = rbtree();
    insert(tree1, 10);
    insert(tree1, 20);
    insert(tree1, 30);
    insert(tree1, 50);
    insert(tree1, 40);
    insert(tree1, 60);
    insert(tree1, 70);
    insert(tree1, 80);
    insert(tree1, 4);
    insert(tree1, 9);
    inorder(tree1);
    printf("\n");
    test(tree1);
    return 0;
}

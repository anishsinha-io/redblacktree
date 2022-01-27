#include <stdio.h>
#include "rbtree/rbtree.h"

int main() {
    bool t1 = test_suite_one(), t2 = test_suite_two(), t3 = test_suite_three(), t4 = test_suite_four(),
            t5 = test_suite_five(), t6 = test_suite_six();
    if (t1 && t2 && t3 && t4 && t5 && t6) {
        printf("\033[0;32m");
        printf("all tests passed!");
    } else {
        printf("\033[0;31m");
        if (!t1) printf("test one failed");
        if (!t2) printf("test two failed");
        if (!t3) printf("test three failed");
        if (!t4) printf("test four failed");
        if (!t5) printf("test five failed");
        if (!t6) printf("test six failed");
    }
    return 0;
}


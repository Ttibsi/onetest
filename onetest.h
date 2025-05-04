#ifndef ONETEST_H
#define ONETEST_H

#include <stdio.h> 
extern int onetest_test_counter; 

#define call(func) { \
    onetest_test_counter++; \
    int ret = func(); \
    if (ret) { \
        printf("\x1b[31m[%d] %s\x1b[0m\n", onetest_test_counter, #func); \
    } else { \
        printf("\x1b[32m[%d] %s\x1b[0m\n", onetest_test_counter, #func); \
    } \
} 

#define assert_eq(x, y) if (x != y) { return 1; } 
#define assert_ne(x, y) if (x == y) { return 1; } 
 
#ifdef ONETEST_IMPLEMENTATION

int onetest_test_counter = 0; 

#endif // ONETEST_IMPLEMENTATION
#endif // ONETEST_H 

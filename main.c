#define ONETEST_IMPLEMENTATION
#include "onetest.h"

int test_foo(void) {
    onetest_assert(1 == 1);
    onetest_assert(1 != 4);
    return 0;
}

int test_bar(void) { 
    onetest_assert(0.3f == 1.1f);
    return 0; 
} 

int test_baz(void) {
    onetest_assert(1 == 2);
    return 0;
} 
 
int main(void) {
    onetest_test_t tests[] = {
        ONETEST_TEST(test_foo),
        ONETEST_TEST(test_bar),
        ONETEST_SKIP(test_baz),     
    };
    
    return onetest_exec(tests, sizeof(tests) / sizeof(tests[0]));
}

#define ONETEST_IMPLEMENTATION
#include "onetest.h"

int test_foo(void) {
    onetest_assert_eq(1, 1);
    onetest_assert_ne(1, 4);
    return 0; 
}

int test_bar(void) { 
    onetest_assert_eq(0.3f, 1.1f);
    return 0; 
} 
 
int main(void) {
    onetest_test_t tests[] = {
        ONETEST_TEST(test_foo),
        ONETEST_TEST(test_bar)
    };
    
    return onetest_exec(tests, sizeof(tests) / sizeof(tests[0]));
}

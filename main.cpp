#define ONETEST_IMPLEMENTATION
#include "onetest.h"

int test_foo() {
    assert_eq<int>(1, 1);
    assert_ne<int>(1, 4);
    return 0; 
}

int test_bar() { 
    assert_eq<float>(0.3, 1.1);
    return 0; 
} 
 
int main() {
    return onetest_exec({{
        call(test_foo),
        call(test_bar)
    }});
} 

#define ONETEST_IMPLEMENTATION
#include "onetest.h" 

int test_foo() {
    assert_eq(5, 5);
    assert_eq(4,5); 
    return 0; 
} 

int test_bar() {
    assert_ne(4, 5);
    return 0;
} 

int main() {
   call(test_foo);
   call(test_bar);
} 

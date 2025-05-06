#define ONETEST_IMPLEMENTATION
#include "onetest.h" 

#include "other.h" 

int test_foo(void) {
    assert_num_eq(5, 5);
    assert_num_eq(4,5); 
    return 0; 
} 

int test_bar(void) {
    assert_num_ne(4, 5);
    return 0;
} 

int main() {
    onetest_init(); 
    call(test_foo, "test_foo");
    call(test_bar, "test_bar");
    call(test_other, "test_other"); 
    onetest_exec(); 
} 

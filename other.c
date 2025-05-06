#include "onetest.h"
#include "other.h" 
 
int test_other(void) {
    assert_str_eq("Hello hello", "Hello world"); 
    return 0; 
} 

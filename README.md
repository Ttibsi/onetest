# onetest
A unit testing library in C99 in only one file

![Image](https://github.com/user-attachments/assets/1b2d0699-ca24-47ae-88a7-9cfdf62521b1)

# How to use
This is an STB-style library, so you'll only need to define `ONETEST_IMPLEMENTATION`
the once. You'll need a separate `main()` function. Pass the `call` function a
pointer to your test functions, and a name to display with it. the `onetest_init`
and `onetest_exec` functions need to be called for setup and teardown.


```c
#define ONETEST_IMPLEMENTATION
#include "onetest.h"

int add(int x, int y) { return x + y; }

int test_add(void) {
    assert_num_eq(add(2, 2), 4);
    assert_num_ne(add(1, 1), 3);
    return 0;
}

int main() {
    onetest_init(); 
    call(test_add, "add two nums"); 
    onetest_exec(); 
}
```

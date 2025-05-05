#ifndef ONETEST_H
#define ONETEST_H

#include <stdlib.h>

#define call(func, name) { \
    test_append(func, name); \
    e = new_errors(); \
}
#define ONETEST_STR_LEN 48 

#define assert_eq(x, y) if (x != y) { \
    error_append(x, "does not equal", y); \
    return 1;\
} 

#define assert_ne(x, y) if (x == y) { \
    error_append(x, "is equal to", y); \
    return 1;\
} 

typedef int (*func_ptr)(void); 
typedef struct {
    size_t size;
    size_t cap;
    func_ptr* items;    
    char** names; 
} test_array;

typedef struct {
    size_t size;
    size_t cap;
    char** items;
} Errors; 
extern Errors e; 

void onetest_init(void);
void onetest_exec(void);
 
#ifdef ONETEST_IMPLEMENTATION
#include <stdio.h> 
#include <string.h> 
#include <sys/ioctl.h>

test_array tests; 
Errors e;

void onetest_init(void) {
    tests.size = 0; 
    tests.cap = 8; 
    tests.items = malloc(sizeof(func_ptr) * 8);
    tests.names = malloc(sizeof(char) * ONETEST_STR_LEN * 8); 
} 

void test_append(func_ptr f, char* name) {
    if (tests.size == tests.cap) {
        tests.items = realloc(tests.items, tests.cap * 2 * sizeof(func_ptr));
        tests.names = realloc(tests.names, tests.cap * 2 * sizeof(char) * ONETEST_STR_LEN); 
        tests.cap = tests.cap * 2;
    }

    tests.items[tests.size] = f;
    tests.names[tests.size] = name; 
    tests.size++; 
} 

Errors new_errors(void) {
    if (e.size) { free(e.items); }
    e.size = 0;
    e.cap = 8;
    e.items = malloc(sizeof(char) * ONETEST_STR_LEN * 8); 
    return e; 
} 

void error_append() {
}

size_t get_term_width(void) {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_col;
} 

void onetest_exec(void) {
    const char* fail = "\x1b[41mFailed\x1b[0m";
    const char* succeed = "\x1b[42mPassed\x1b[0m";

    for (size_t i = 0; i < tests.size; i++) {
        int ret = tests.items[i]();

        // +9 because that's the size of the ansi escape codes above
        // +1 for \0 for printing 
        size_t term_width = get_term_width() + 9;
        char* out = malloc(sizeof(char) * term_width); 
        out = memset(out, '.', term_width); 
        out = strncpy(out, tests.names[i], strlen(tests.names[i])); 
        int dest = term_width - 15; 

        if (ret) {
            strcpy(out + dest, fail); 
        } else {
            strcpy(out + dest, succeed); 
        } 

        out[term_width] = '\0'; 
        printf("%s\n", out);
        free(out); 
    } 

    free(tests.items);
    free(tests.names); 
    tests.items = NULL;
    tests.names = NULL; 
    tests.size = 0;
}

#endif // ONETEST_IMPLEMENTATION
#endif // ONETEST_H 

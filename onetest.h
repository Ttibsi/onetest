#ifndef ONETEST_H
#define ONETEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ONETEST_MAX_ERRORS 32
#define ONETEST_MAX_ERROR_LEN 256
#define ONETEST_MAX_TESTS 128

#define ONETEST_TEST(func) {func, #func, 0}
#define ONETEST_SKIP(func) {func, #func, 1}

typedef int (*onetest_func_t)(void);

typedef struct {
    onetest_func_t func;
    const char* name;
    int skip;  // 0 for no, 1 for yes
} onetest_test_t;

extern char onetest_errors[ONETEST_MAX_ERRORS][ONETEST_MAX_ERROR_LEN];
extern int onetest_error_count;

int onetest_exec(const onetest_test_t* tests, int test_count);

#define onetest_assert(x) \
    do { \
        if (!(x)) { \
            snprintf(onetest_errors[onetest_error_count], ONETEST_MAX_ERROR_LEN, \
                    "%s:%d - assertion failed: %s", __FUNCTION__, __LINE__, #x); \
            onetest_error_count++; \
            if (onetest_error_count >= ONETEST_MAX_ERRORS) onetest_error_count = ONETEST_MAX_ERRORS - 1; \
        } \
    } while(0)

#ifdef ONETEST_IMPLEMENTATION

char onetest_errors[ONETEST_MAX_ERRORS][ONETEST_MAX_ERROR_LEN];
int onetest_error_count = 0;

typedef struct {
    int pass;
    int skip;
    int fail;
} Counters;

static int onetest_is_ci(void) { return getenv("CI") != NULL; }

static double onetest_get_time_ms(void) {
    clock_t c = clock();
    return ((double)c / CLOCKS_PER_SEC) * 1000.0;
}

static inline const char* status_color(int ci, const char* tty_color) {
    return ci ? "" : tty_color;
}

static inline void print_dots(int count) {
    for (int i = 0; i < count; ++i) { putchar('.'); }
}

static inline void print_status(const char* name, int padding, const char* label, const char* color) {
    printf("%s", name);
    print_dots(padding);
    printf("%s%s\x1b[0m\n", color, label);
}

static inline void print_errors(int ci) {
    for (int j = 0; j < onetest_error_count; ++j) {
        if (ci)
            printf("\t%s\n", onetest_errors[j]);
        else
            printf("\t\x1b[31m%s\x1b[0m\n", onetest_errors[j]);
    }
}

static inline void summary_msg(Counters counters, double ms, char* out, size_t outsz) {
    if (counters.fail == 0) {
        if (counters.skip)
            snprintf(out, outsz, " %d tests passed (%d skipped) in %.2fms ", counters.pass, counters.skip, ms);
        else
            snprintf(out, outsz, " %d tests passed in %.2fms ", counters.pass, ms);
    } else if (counters.fail == 1) {
        if (counters.skip)
            snprintf(out, outsz, " 1 test failed (%d skipped) in %.2fms ", counters.skip, ms);
        else
            snprintf(out, outsz, " 1 test failed in %.2fms ", ms);
    } else {
        if (counters.skip)
            snprintf(out, outsz, " %d tests failed (%d skipped) in %.2fms ", counters.fail, counters.skip, ms);
        else
            snprintf(out, outsz, " %d tests failed in %.2fms ", counters.fail, ms);
    }
}

int onetest_exec(const onetest_test_t* tests, int test_count) {
    int ci            = onetest_is_ci();
    Counters counters = {0, 0, 0};
    double start_time = onetest_get_time_ms();

    for (int i = 0; i < test_count; ++i) {
        onetest_error_count = 0;
        memset(onetest_errors, 0, sizeof(onetest_errors));

        int retcode = 0;
        if (tests[i].skip) {
            ++counters.skip;
        } else {
            retcode = tests[i].func();
            if (retcode) {
                snprintf(onetest_errors[onetest_error_count],
                         ONETEST_MAX_ERROR_LEN,
                         "Return code: %i", retcode);
                ++onetest_error_count;
            }
        }

        int name_len = strlen(tests[i].name);
        int padding  = 79 - name_len - (tests[i].skip ? 7 : 6);
        if (padding < 0) { padding = 0; }

        if (tests[i].skip) {
            const char *col = status_color(ci, "\x1b[43m");
            print_status(tests[i].name, padding, "Skipped", col);
        } else if (onetest_error_count > 0) {
            const char *col = status_color(ci, "\x1b[41m");
            print_status(tests[i].name, padding, "Failed", col);
        } else {
            const char *col = status_color(ci, "\x1b[42m");
            print_status(tests[i].name, padding, "Passed", col);
            ++counters.pass;
        }

        if (!tests[i].skip) { print_errors(ci); }
        if (onetest_error_count > 0 || retcode > 0) { ++counters.fail; }
    }

    double end_time = onetest_get_time_ms();
    double duration = end_time - start_time;

    char msg[128];
    summary_msg(counters, duration, msg, 128);

    int msg_len   = strlen(msg);
    int line_size = (79 - msg_len) / 2;
    const char *color = status_color(ci, counters.fail ? "\x1b[31m" : "\x1b[32m");

    printf("%s", color);
    for (int i = 0; i < line_size + (msg_len % 2 ? 0 : 1); ++i) { putchar('='); }
    printf("%s", msg);
    for (int i = 0; i < line_size; ++i) { putchar('='); }
    if (!ci) { printf("\x1b[0m"); }
    putchar('\n');

    return counters.fail;
}

#endif /* ONETEST_IMPLEMENTATION */
#endif /* ONETEST_H */

#ifndef ONETEST_H
#define ONETEST_H
#include <functional>
#include <source_location>
#include <span>
#include <string> 
#include <vector> 

#define call(f) {f, #f}
 
using ssl_t = std::source_location;
using test_t = std::function<int(void)>;
struct Test {
    test_t ptr; 
    std::string name;

    Test(test_t t, std::string n): ptr(t), name(n) {}
}; 

extern std::vector<std::string> errors; 
void onetest_exec(std::span<const Test>);

template <typename T>
int assert_eq(T x, T y, ssl_t loc = std::source_location::current());

template <typename T>
int assert_ne(T x, T y, ssl_t loc = std::source_location::current());

#ifdef ONETEST_IMPLEMENTATION
#include <chrono> 
#include <iostream>
#include <format> 

std::vector<std::string> errors;
 
template <typename T>
int assert_eq(T x, T y, ssl_t loc) {
    if (x != y) {
        errors.push_back(std::format("{}:{} - {} does not equal {}", loc.function_name(), loc.line(), x, y));
        return 1;
    } 
    return 0;
} 

template <typename T>
int assert_ne(T x, T y, ssl_t loc) {
    if (x == y) {
        errors.push_back(std::format("{}:{} - {} is equal to {}", loc.function_name(), loc.line(), x, y)); 
        return 1; 
    } 
    return 0; 
}

void onetest_exec(std::span<const Test> tests) {
    int fail_counter = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto&& test: tests) {
        errors.clear();
        errors.reserve(8); 
 
        test.ptr();
 
        const int intermediary = 79 - 6 - test.name.size();
        const std::string result = errors.size() ? "\x1b[41mFailed\x1b[0m" : "\x1b[42mPassed\x1b[0m";
        std::cout << test.name << std::string(intermediary, '.') << result << "\n";

        for (auto&& e: errors) {
            std::cout << "\t\x1b[31m" << e << "\x1b[0m\n";
        }

        if (errors.size()) { fail_counter++; } 
    }
 
    const auto runtime = std::chrono::steady_clock::now() - start;
    const auto duration = std::chrono::duration<double, std::milli>(runtime).count();

    std::string msg;
    switch (fail_counter) {
        case 0:
            msg = std::format(" All tests passed in {:.2f}ms ", duration); 
            break;
        case 1:
            msg = std::format(" 1 test failed in {:.2f}ms ", duration);
            break; 
        default: 
            msg = std::format(" {} tests failed in {:.2f}ms ", fail_counter, duration); 
    }

    const int line_size = (79 - msg.size()) / 2;
    const std::string color = fail_counter ? "\x1b[31m" : "\x1b[32m"; 

    std::cout << color << std::string(line_size + !(msg.size() % 2), '=') << msg << std::string(line_size, '=') << "\x1b[0m\n";
    return; 
} 

#endif // ONETEST_IMPLEMENTATION
#endif // ONETEST_H 

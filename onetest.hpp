#ifndef ONETEST_H
#define ONETEST_H
#include <format>
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

    Test(test_t t, std::string n) : ptr(t), name(n) {}
};

extern std::vector<std::string> errors;
int onetest_exec(std::span<const Test>);

template <typename T>
inline int assert_eq(T x, T y, ssl_t loc = std::source_location::current()) {
    if (x != y) {
        errors.push_back(
            std::format("{}:{} - {} does not equal {}", loc.function_name(), loc.line(), x, y));
        return 1;
    }
    return 0;
}

template <typename T>
inline int assert_ne(T x, T y, ssl_t loc = std::source_location::current()) {
    if (x == y) {
        errors.push_back(
            std::format("{}:{} - {} is equal to {}", loc.function_name(), loc.line(), x, y));
        return 1;
    }
    return 0;
}

#ifdef ONETEST_IMPLEMENTATION
#include <chrono>
#include <cstdlib>
#include <iostream>

std::vector<std::string> errors;

int onetest_exec(std::span<const Test> tests) {
    bool ci = (std::getenv("CI") != nullptr);
    int fail_counter = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto&& test : tests) {
        errors.clear();
        errors.reserve(8);

        test.ptr();

        const int intermediary = 79 - 6 - test.name.size();
        std::string result;
        if (ci) {
            result = (errors.size() ? "Failed" : "Passed");
        } else {
            result = (errors.size() ? "\x1b[41mFailed\x1b[0m" : "\x1b[42mPassed\x1b[0m");
        }
        std::cout << test.name << std::string(intermediary, '.') << result << "\n";

        for (auto&& e : errors) {
            if (ci) {
                std::cout << "\t\x1b[31m" << e << "\x1b[0m\n";
            } else {
                std::cout << "\t" << e << "\n";
            }
        }

        if (errors.size()) {
            fail_counter++;
        }
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
    std::string color = fail_counter ? "\x1b[31m" : "\x1b[32m";
    std::string reset = "\x1b[0m";
    if (ci) {
        color = "";
    }
    if (ci) {
        reset = "";
    }

    std::cout << color << std::string(line_size + !(msg.size() % 2), '=') << msg
              << std::string(line_size, '=') << reset << "\n";
    return fail_counter;
}

#endif  // ONETEST_IMPLEMENTATION
#endif  // ONETEST_H

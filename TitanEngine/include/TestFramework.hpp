#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <iomanip>

namespace Titan::Test {

class TestCase {
public:
    std::string name;
    std::function<void()> test;
    bool passed{false};
    std::string message;

    TestCase(const std::string& n, std::function<void()> t) : name(n), test(t) {}
};

class TestSuite {
private:
    static std::vector<TestCase>& GetTests() {
        static std::vector<TestCase> tests;
        return tests;
    }

public:
    static void Register(const std::string& name, std::function<void()> test) {
        GetTests().emplace_back(name, test);
    }

    static int RunAll() {
        auto& tests = GetTests();
        int passed = 0, failed = 0;

        std::cout << "\n" << std::string(70, '=') << "\n";
        std::cout << "Running " << tests.size() << " tests...\n";
        std::cout << std::string(70, '=') << "\n";

        for (auto& tc : tests) {
            try {
                tc.test();
                tc.passed = true;
                passed++;
                std::cout << "[PASS] " << std::setw(50) << std::left << tc.name << "\n";
            } catch (const std::exception& e) {
                tc.passed = false;
                tc.message = e.what();
                failed++;
                std::cout << "[FAIL] " << std::setw(50) << std::left << tc.name
                          << " - " << e.what() << "\n";
            }
        }

        std::cout << std::string(70, '=') << "\n";
        std::cout << "Results: " << passed << " passed, " << failed << " failed\n";
        std::cout << std::string(70, '=') << "\n";

        return failed == 0 ? 0 : 1;
    }
};

class Assertion {
public:
    static void Assert(bool condition, const std::string& message) {
        if (!condition) {
            throw std::runtime_error(message);
        }
    }

    static void AssertEqual(int a, int b, const std::string& message = "") {
        if (a != b) {
            throw std::runtime_error(message.empty() ? 
                std::to_string(a) + " != " + std::to_string(b) : message);
        }
    }

    static void AssertEqual(float a, float b, float epsilon = 0.0001f, const std::string& message = "") {
        if (std::abs(a - b) > epsilon) {
            throw std::runtime_error(message.empty() ? 
                std::to_string(a) + " != " + std::to_string(b) : message);
        }
    }

    static void AssertEqual(const std::string& a, const std::string& b, const std::string& message = "") {
        if (a != b) {
            throw std::runtime_error(message.empty() ? 
                "\"" + a + "\" != \"" + b + "\"" : message);
        }
    }

    static void AssertNotNull(const void* ptr, const std::string& message = "Pointer is null") {
        if (ptr == nullptr) {
            throw std::runtime_error(message);
        }
    }

    static void AssertNull(const void* ptr, const std::string& message = "Pointer is not null") {
        if (ptr != nullptr) {
            throw std::runtime_error(message);
        }
    }
};


} // namespace Titan::Test

// Macro for registering tests
#define REGISTER_TEST(name) \
    void Test_##name##_func(); \
    namespace { \
        struct Test_##name##_Reg { \
            Test_##name##_Reg() { \
                Titan::Test::TestSuite::Register(#name, &Test_##name##_func); \
            } \
        } test_##name##_instance; \
    } \
    void Test_##name##_func()

#define ASSERT(cond) Titan::Test::Assertion::Assert((cond), #cond)
#define ASSERT_EQ(a, b) Titan::Test::Assertion::AssertEqual((a), (b))
#define ASSERT_FLOAT_EQ(a, b) Titan::Test::Assertion::AssertEqual((a), (b))
#define ASSERT_STR_EQ(a, b) Titan::Test::Assertion::AssertEqual((a), (b))
#define ASSERT_NOT_NULL(ptr) Titan::Test::Assertion::AssertNotNull((ptr))
#define ASSERT_NULL(ptr) Titan::Test::Assertion::AssertNull((ptr))

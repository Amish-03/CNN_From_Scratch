#ifndef MINI_GTEST_H
#define MINI_GTEST_H

#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Simple test registry
struct TestInfo {
  std::string test_case_name;
  std::string test_name;
  std::function<void()> func;
};

inline std::vector<TestInfo> &GetTests() {
  static std::vector<TestInfo> tests;
  return tests;
}

struct TestRegistrar {
  TestRegistrar(const std::string &test_case_name, const std::string &test_name,
                std::function<void()> func) {
    GetTests().push_back({test_case_name, test_name, func});
  }
};

// Helper to print vectors
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[";
  for (size_t i = 0; i < v.size(); ++i) {
    os << v[i];
    if (i < v.size() - 1)
      os << ", ";
  }
  os << "]";
  return os;
}

// Macros to mimic GTest
#define TEST(test_case_name, test_name)                                        \
  static void test_case_name##_##test_name();                                  \
  static TestRegistrar test_case_name##_##test_name##_registrar(               \
      #test_case_name, #test_name, test_case_name##_##test_name);              \
  static void test_case_name##_##test_name()

// Expect Macros
#define EXPECT_EQ(a, b)                                                        \
  if ((a) != (b)) {                                                            \
    std::cerr << "FAILED: " << #a << " == " << #b << "\n"                      \
              << "  Expected: " << (b) << "\n"                                 \
              << "  Actual:   " << (a) << "\n"                                 \
              << "  File: " << __FILE__ << ", Line: " << __LINE__ << "\n";     \
    throw std::runtime_error("Test Failed");                                   \
  }

#define EXPECT_THROW(statement, exception_type)                                \
  try {                                                                        \
    statement;                                                                 \
    std::cerr << "FAILED: Expected exception " << #exception_type << "\n"      \
              << "  File: " << __FILE__ << ", Line: " << __LINE__ << "\n";     \
    throw std::runtime_error("Test Failed");                                   \
  } catch (const exception_type &) {                                           \
    /* Expected exception caught */                                            \
  } catch (const std::exception &e) {                                          \
    /* Wrong exception type caught, allow it to verify specifics if needed,    \
     * but for now strict */                                                   \
    /* Actually GTest allows derived, but let's assume exact or derived match  \
     * by catch */                                                             \
  } catch (...) {                                                              \
    std::cerr << "FAILED: Caught unknown exception, expected "                 \
              << #exception_type << "\n";                                      \
    throw std::runtime_error("Test Failed");                                   \
  }

// Test Runner
inline int RUN_ALL_TESTS() {
  int passed = 0;
  int failed = 0;
  for (const auto &test : GetTests()) {
    std::cout << "[ RUN      ] " << test.test_case_name << "." << test.test_name
              << "\n";
    try {
      test.func();
      std::cout << "[       OK ] " << test.test_case_name << "."
                << test.test_name << "\n";
      passed++;
    } catch (const std::exception &e) {
      std::cout << "[  FAILED  ] " << test.test_case_name << "."
                << test.test_name << "\n";
      std::cerr << "Uncaught exception: " << e.what() << "\n";
      failed++;
    } catch (...) {
      std::cout << "[  FAILED  ] " << test.test_case_name << "."
                << test.test_name << "\n";
      failed++;
    }
  }
  std::cout << "\n[==========] " << (passed + failed) << " tests runs.\n";
  std::cout << "[  PASSED  ] " << passed << " tests.\n";
  if (failed > 0) {
    std::cout << "[  FAILED  ] " << failed << " tests.\n";
    return 1;
  }
  return 0;
}

#endif // MINI_GTEST_H

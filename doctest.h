// doctest.h - minimal single-header testing framework
// Simplified for C++ Programming 1 (no pointers, vectors, sstream, functional, etc.)

#ifndef DOCTEST_LIBRARY_INCLUDED
#define DOCTEST_LIBRARY_INCLUDED

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

namespace doctest {

// Simple approximation for double comparison
struct Approx {
    double m_value;
    double m_epsilon;
    
    Approx(double value) {
        m_value = value;
        m_epsilon = 0.0001;
    }
    
    bool compare(double lhs) const {
        double diff = lhs - m_value;
        if (diff < 0) diff = -diff; 
        return diff <= m_epsilon;
    }
};

bool operator==(double lhs, const Approx& rhs) {
    return rhs.compare(lhs);
}

// Simple test case structure (no pointers)
struct TestCase {
    string name;
    void (*func)();
    bool isActive;
};

// Global test registry using array (no pointers or linked list)
class TestRegistry {
private:
    static const int MAX_TESTS = 50;
    TestCase tests[MAX_TESTS];
    int count;
    
public:
    TestRegistry() : count(0) {
        for (int i = 0; i < MAX_TESTS; i++) {
            tests[i].isActive = false;
        }
    }
    
    static TestRegistry& instance() {
        static TestRegistry registry;
        return registry;
    }
    
    void addTest(const string& name, void (*func)()) {
        if (count < MAX_TESTS) {
            tests[count].name = name;
            tests[count].func = func;
            tests[count].isActive = true;
            count++;
        }
    }
    
    int getCount() const { return count; }
    
    TestCase getTest(int index) const {
        return tests[index];
    }
};

// Simple exception for test failures
class AssertionException {
private:
    string message;
    
public:
    AssertionException(const string& msg) : message(msg) {}
    string getMessage() const { return message; }
};

// Context to run tests (no pointers)
class Context {
public:
    Context() {}
    
    int run() {
        cout << "\n===============================================================================\n";
        cout << "Running doctest unit tests\n";
        cout << "===============================================================================\n\n";
        
        int passed = 0;
        int failed = 0;
        
        TestRegistry& registry = TestRegistry::instance();
        int totalTests = registry.getCount();
        
        for (int i = 0; i < totalTests; i++) {
            TestCase test = registry.getTest(i);
            if (test.isActive) {
                cout << "TEST CASE: " << test.name << "\n";
                try {
                    test.func();
                    cout << "  PASSED\n\n";
                    passed++;
                }
                catch (const AssertionException& e) {
                    cout << "  FAILED\n";
                    cout << "  " << e.getMessage() << "\n\n";
                    failed++;
                }
                catch (...) {
                    cout << "  FAILED (unknown exception)\n\n";
                    failed++;
                }
            }
        }
        
        cout << "===============================================================================\n";
        cout << "Test summary:\n";
        cout << "  " << passed << " test case(s) passed\n";
        cout << "  " << failed << " test case(s) failed\n";
        cout << "===============================================================================\n\n";
        
        return failed;
    }
    
    bool shouldExit() const { return false; }
};

} // namespace doctest

#define DOCTEST_CONCAT_IMPL(x, y) x##y
#define DOCTEST_CONCAT(x, y) DOCTEST_CONCAT_IMPL(x, y)

#define TEST_CASE(name) \
    static void DOCTEST_CONCAT(test_func_, __LINE__)(); \
    static struct DOCTEST_CONCAT(test_reg_, __LINE__) { \
        DOCTEST_CONCAT(test_reg_, __LINE__)() { \
            doctest::TestRegistry::instance().addTest(name, DOCTEST_CONCAT(test_func_, __LINE__)); \
        } \
    } DOCTEST_CONCAT(test_reg_instance_, __LINE__); \
    static void DOCTEST_CONCAT(test_func_, __LINE__)()

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            string msg = "CHECK FAILED: "; \
            msg += #expr; \
            throw doctest::AssertionException(msg); \
        } \
    } while(false)

// Auto-generate main() when requested
#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
int main() {
    doctest::Context context;
    int res = context.run();
    return res;
}
#endif

#endif // DOCTEST_LIBRARY_INCLUDED

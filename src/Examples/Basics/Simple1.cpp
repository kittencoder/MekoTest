#include "Meko.h"

using namespace Meko::TestSuite;

// every test-instance is a class
class TestInstance : public Test {
   public:
    // some basic data for the test
    TestInstance() : Test("example test", "example topic", true) {}

    // the test itself
    bool ExecuteTest() override {
        return true;
    }
};

int main(int argc, char** argv) {
    try {
        // get a test-suite
        TestSuite ts;

        // add tests we want to run
        ts.addTest(new TestInstance());
        ts.addTest(new TestInstance());
        ts.addTest(new TestInstance());

        // run tests and render a result
        std::cout << ts.run().render() << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

#include "Meko.h"

using namespace Meko::TestSuite;

class TestInstance : public Test {
   public:
    TestInstance() : Test("example test", "example topic", true) {}

    bool ExecuteTest() override {
        throw std::runtime_error("details describing the failure");
        return false;
    }
};
class ExampleTestB : public Test {
   public:
    ExampleTestB() : Test("example test 2", "example topic", true) {}

    bool ExecuteTest() override {
        throw TestFailed("unexpected error", "details describing the failure");
        return false;
    }
};
class ExampleTestC : public Test {
   public:
    ExampleTestC() : Test("example test 3", "example topic", false) {}

    bool ExecuteTest() override {
        throw TestFailed("expected error", "details describing the failure");
        return false;
    }
};

int main(int argc, char** argv) {
    try {
        TestSuite ts;

        // fails due to exception
        ts.addTest(new TestInstance());
        ts.addTest(new ExampleTestB());
        // should fail and does due to exception
        ts.addTest(new ExampleTestC());

        std::cout << ts.run().render() << std::endl;

    } catch (Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

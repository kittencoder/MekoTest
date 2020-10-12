#include "Meko.h"

using namespace Meko::TestSuite;
using Meko::raw_str_arg;

class TestInstance : public Test {
   public:
    TestInstance(raw_str_arg topic, bool shouldSucceed, bool willSucceed) : Test("example test", topic, shouldSucceed), willSucceed(willSucceed) {}

    bool ExecuteTest() override {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        setErrorReason("the reason");
        setErrorDetails("some details describing why and how the test failed");
        return willSucceed;
    }
    bool willSucceed;
};

int main(int argc, char** argv) {
    try {
        TestSuite ts;

        // add some tests that will fail
        ts.addTest(new TestInstance("topic A", true, false));
        ts.addTest(new TestInstance("topic C", false, true));
        ts.addTest(new TestInstance("topic C", false, true));

        ts.addTest(new TestInstance("topic C", true, true));
        ts.addTest(new TestInstance("topic C", true, true));
        // add a handful of positive and negative tests that will succeed for 2 topics
        for (bool s : {true, false}) {
            for (size_t i = 0; i < 20; ++i) {
                ts.addTest(new TestInstance("topic A", s, s));
            }
        }
        for (bool s : {true, false, false}) {
            for (size_t i = 0; i < 20; ++i) {
                ts.addTest(new TestInstance("topic B", s, s));
            }
        }

        std::cout << ts.run().render() << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

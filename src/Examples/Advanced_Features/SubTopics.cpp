#include "Meko.h"

using namespace Meko::TestSuite;

class TestInstance : public Test {
   public:
    TestInstance(Meko::raw_str_arg topic) : Test("example test", topic, true) {}

    bool ExecuteTest() override {
        return true;
    }
};

int main(int argc, char** argv) {
    try {
        TestSuite ts;

        // see how th result changes when adding a test for "Generic 2"
        ts.addTest(new TestInstance("Generic 1"));

        auto result = ts.run();

        result.setSubTopics({
            {"Generic 1", {"Special 1 A", "Special 1 B"}},
            {"Generic 2", {"Special 2 A", "Special 2 B"}},
        });

        std::cout
            << result.render() << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

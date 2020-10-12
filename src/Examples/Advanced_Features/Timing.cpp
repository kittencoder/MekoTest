#include "Meko.h"

using namespace Meko::TestSuite;

class TestInstance : public Test {
   public:
    TestInstance() : Test("example test", "start-stop timer", true) {}

    bool ExecuteTest() override {
        startTimer();
        // see how the result change, when doing more/less "work" here
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        stopTimer();
        return true;
    }
};
class ExampleTestB : public Test {
   public:
    ExampleTestB() : Test("example test", "manual time-setting", true) {}

    bool ExecuteTest() override {
        // see how the result changes when changing the time-value here
        addTime(std::chrono::seconds(2));
        return true;
    }
};

int main(int argc, char** argv) {
    try {
        TestSuite ts;

        ts.addTest(new TestInstance());
        ts.addTest(new ExampleTestB());

        std::cout << ts.run().render() << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

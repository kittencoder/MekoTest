#include "Meko.h"

using namespace Meko::TestSuite;

class TestInstance : public Test {
   public:
    TestInstance() : Test("example test", "example topic", true) {}

    bool ExecuteTest() override {
        if (++runs > 3) {
            setErrorReason("ran to often");
            setErrorDetails("ran " + to_raw_str(runs) + " times");
            return false;
        }
        return true;
    }

   protected:
    int runs = 0;
};

int main(int argc, char** argv) {
    try {
        TestSuite ts;

        ts.addTest(new TestInstance());

        // increase this number to see the test above fail due to the condition
        std::cout << ts.run(3).render() << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

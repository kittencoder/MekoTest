#include "Meko.h"

using namespace Meko::TestSuite;

class TestInstance : public Test {
   public:
    TestInstance() : Test("example test", "example topic", true) {}

    Meko::raw_str data;
    /**
     * prepare testing-data
     */
    bool prepare() override {
        data = "some data";
        return true;
    }

    bool ExecuteTest() override {
        return data == "some data";
    }
    /**
     * cleanup testing data
     */
    void cleanup() override {
        data = "";
    }
};

int main(int argc, char** argv) {
    try {
        TestSuite ts;

        ts.addTest(new TestInstance());

        std::cout << ts.run().render() << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

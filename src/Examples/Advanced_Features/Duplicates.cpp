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

        // see how E and F both get hidden from the missing-list when adding one of them
        for (Meko::raw_str_arg t : {"Topic A", "Topic B", "Topic D"}) {
            ts.addTest(new TestInstance(t));
        }

        auto result = ts.run();
        result.setDuplicates({
            {"Topic A", "Topic C"},
            {"Topic E", "Topic F"},
        });

        std::cout << result.render() << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

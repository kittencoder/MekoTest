#include "Meko.h"

using namespace Meko::TestSuite;

class TestInstance : public Test {
   public:
    TestInstance(Meko::raw_str_arg topic) : Test("example test", topic, true) {}

    bool ExecuteTest() override {
        size_t processingTime = getTopic().compare("Topic A") * 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(processingTime));
        return true;
    }
};

int main(int argc, char** argv) {
    try {
        TestSuite ts(false);

        for (Meko::raw_str_arg t : {"Topic A", "Topic B", "Topic D"}) {
            ts.addTest(new TestInstance(t));
        }

        auto result = ts.run();
        result.setDuplicates({
            {"Topic C", "Duplicate of C"},
            {"Topic E", "Duplicate of E"},
        });
        result.setSimpleTopics({"Topic F"});
        result.setSubTopics({
            {"Topic A", {"Topic G", "Topic H"}},
        });

        // turn this on to show every available section
        bool fullReport = true;
        if (fullReport) {
            std::cout << result.render(Report::ReportDisplayConfig().approximateRuns(1000).showDuplicates().showSimpleTopics()) << std::endl;
        } else {
            std::cout << result.render(Report::ReportDisplayConfig().disableGraph().disableColor()) << std::endl;
        }

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

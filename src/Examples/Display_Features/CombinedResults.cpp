#include "Meko.h"

using namespace Meko::TestSuite;

class TestInstance : public Test {
   public:
    TestInstance(Meko::raw_str_arg topic) : Test("example test", topic, true) {}

    bool ExecuteTest() override {
        size_t processingTime = getTopic().compare("Topic A") * 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(processingTime));
        return getTopic().compare("Topic B") != 0;
    }
};

Report runSomeTests(const std::set<Meko::raw_str>& topics, size_t numberOfRuns = 1) {
    TestSuite ts;

    for (Meko::raw_str_arg t : topics) {
        ts.addTest(new TestInstance(t));
    }

    return ts.run(numberOfRuns);
}

CombinedReport runMultiple() {
    std::unordered_map<Meko::raw_str, Report> reports;
    reports["Sub-1"] = runSomeTests({"SubTopic 1"}, 10);
    reports["Sub-2"] = runSomeTests({"SubTopic 2", "SubTopic 3"}, 15);
    return CombinedReport(reports);
}

int main(int argc, char** argv) {
    try {
        std::unordered_map<Meko::raw_str, Report> reports;
        std::unordered_map<Meko::raw_str, Meko::raw_str> humanReadableNames;

        reports["Plugin 1"] = runSomeTests({"Topic A", "Topic B", "Topic C"});
        humanReadableNames["Plugin 1"] = "First Plugin";

        reports["Plugin 2"] = runSomeTests({"Topic A", "Topic B", "Topic D"}, 20);
        humanReadableNames["Plugin 2"] = "Second Plugin";

        reports["Plugin 3"] = runSomeTests({"Topic E", "Topic F", "Topic G"});
        // when not providing a human-readable name, the key will be used

        reports["Container"] = runMultiple();

        auto result = CombinedReport(reports, humanReadableNames);

        std::cout << result.render() << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

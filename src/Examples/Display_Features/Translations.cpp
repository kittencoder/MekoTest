#include "Meko.h"

using namespace Meko::TestSuite;

class TestInstance : public Test {
   public:
    TestInstance(Meko::raw_str_arg topic) : Test("example test", topic, true) {}

    bool ExecuteTest() override {
        return true;
    }
};

Report::Translations makeCustomTranslations() {
    Report::Translations customTranslations;

    // HEADINGS and descriptions:
    customTranslations.h_Graph = "Graphical Overview";
    customTranslations.d_graph = "All the data combined";
    customTranslations.h_missingTopics = "Missing";
    customTranslations.d_missingTopics = "this is not tested";
    customTranslations.h_simpleTopics = "Simple";
    customTranslations.d_simpleTopics = "to easy to report missing";
    customTranslations.h_subTopics = "Nested";
    customTranslations.d_subTopics = "Might be covered by other tests";
    customTranslations.h_Duplicates = "Duplicates";
    customTranslations.d_Duplicates = "exactly as the others";
    customTranslations.h_Details = "ERRORS";
    customTranslations.d_Details = "What went wrong?";
    customTranslations.h_Statistics = "Stats";
    customTranslations.d_Statistics = "Combined data";

    // GRAPH:
    customTranslations.topic = "Collections";
    customTranslations.graph = "";
    customTranslations.positives = "+ Tests";
    customTranslations.negatives = "- Tests";
    customTranslations.positiveAvg = "+ Avg.";
    customTranslations.negativeAvg = "- Avg.";

    // you don't have to set all values
    // unset values will remain default

    // Details:
    // customTranslations.det_failedTopic = "Failed test for Topic: ";
    // customTranslations.det_testname = "Failed Test: ";
    // customTranslations.det_reason = "Reason and details: ";

    // // Statistics:
    // customTranslations.sum_totalTime = "Total time testing";
    // customTranslations.sum_totalTimeSuccess = "Total time of successful tests";
    // customTranslations.sum_totalTests = "Tests run";
    // customTranslations.sum_totalTestsWithMultiplyer = "Tests run (including reruns)";
    // customTranslations.sum_success = "Successful tests";
    // customTranslations.sum_failed = "Failed tests";
    // customTranslations.sum_topics = "Topics tested";
    // customTranslations.sum_topicsFullyTested = "Topics fully tested (including simple)";
    // customTranslations.sum_topicsPartiallyTested = "Topics partially tested (including simple)";
    // customTranslations.sum_topicsUntested = "Topics untested (including simple)";
    // customTranslations.sum_topicsKnown = "Topics known";

    // UNITS:
    customTranslations.millionSuffix = "million";
    customTranslations.thousandSuffix = "thousand";
    return customTranslations;
}

Report::ReportDisplayConfig makeDisplayConf() {
    return Report::ReportDisplayConfig()
        .approximateRuns(1000000)
        .showDuplicates()
        .showSimpleTopics();
}

int main(int argc, char** argv) {
    try {
        TestSuite ts;

        for (Meko::raw_str_arg t : {"Topic A", "Topic B", "Topic D"}) {
            ts.addTest(new TestInstance(t));
        }

        auto result = ts.run();

        std::cout << result.render(makeDisplayConf(), makeCustomTranslations()) << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

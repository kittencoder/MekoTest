#ifndef MEKO_TESTSUITE_REPORT_H
#define MEKO_TESTSUITE_REPORT_H

#include "../Meko.h"

namespace Meko::TestSuite {
using namespace Color;
class CombinedReport;
/**
 * a report renderer for tests
 * 
 * todo: expose some informational methods
 * 
 * todo: list tests that were unable to run (due to fork/pipe fail... !*test)
 */
class Report {
   public:
    /**
     * a report might live longer than the TestSuite-instance (so don't store and rely on this list)
     * tests must all be concluded and red/collected
     * 
     * runtimes in tests are not yet divided my the multiplyer
     */
    Report(std::set<Test*> tests = {}, size_t multiplier = 1);
    //Report& operator=(Report const& cpyfrom);
    /**
     * renders the report using default settings
     */
    operator raw_str();
    /**
     * parameter-object with the option to be constructed when invoking render
     */
    class ReportDisplayConfig {
       public:
        ReportDisplayConfig() {}
        ReportDisplayConfig& disableColor() {
            color = false;
            return *this;
        }
        ReportDisplayConfig& disableGraph() {
            renderGraph = false;
            return *this;
        }
        ReportDisplayConfig& skipMissingReport() {
            showMissingTopics = false;
            return *this;
        }
        ReportDisplayConfig& showSimpleTopics() {
            displayMissingSimpleTopics = true;
            return *this;
        }
        ReportDisplayConfig& hideSubTopics() {
            showSubTopics = false;
            return *this;
        }
        ReportDisplayConfig& showDuplicates() {
            listDuplicates = true;
            return *this;
        }
        ReportDisplayConfig& hideFailDetails() {
            listFailDetails = false;
            return *this;
        }
        ReportDisplayConfig& setGraphWidth(double w) {
            graphWidth = w;
            return *this;
        }
        ReportDisplayConfig& approximateRuns(size_t num) {
            timeScale = num;
            return *this;
        }
        ReportDisplayConfig& hideStatistics() {
            statistics = false;
            return *this;
        }
        ReportDisplayConfig& hideSimpleInGraph() {
            hideSimpleGraph = true;
            return *this;
        }

        bool color = true;
        bool renderGraph = true;
        bool showMissingTopics = true;
        bool displayMissingSimpleTopics = false;
        bool showSubTopics = true;
        bool listDuplicates = false;
        bool listFailDetails = true;
        bool statistics = true;
        bool hideSimpleGraph = false;
        double graphWidth = 30;
        size_t timeScale = 1;
        friend Report;
    };
    class Translations {
       public:
        Translations(){};
        // HEADINGS and descriptions:
        raw_str h_Graph = "Test-Graph";
        raw_str d_graph = "Accumulated data per Topic";
        raw_str h_missingTopics = "Missing Topics";
        raw_str d_missingTopics = "100\% untested Topics";
        raw_str h_simpleTopics = "Simple Topics";
        raw_str d_simpleTopics = "simple untested Topics that might not be worth testing";
        raw_str h_subTopics = "Sub-Topics";
        raw_str d_subTopics = "possibly tested indirectly bo other Topics";
        raw_str h_Duplicates = "Duplicate Topics";
        raw_str d_Duplicates = "Topics that are exactly like others (or reference just those)";
        raw_str h_Details = "Error List";
        raw_str d_Details = "Detailed description of errors for failed tests";
        raw_str h_Statistics = "Statistics";
        raw_str d_Statistics = "Generic data about the tests";
        // GRAPH:
        raw_str topic = "Topic";
        raw_str graph = "Test Graph";
        raw_str positives = "Positive Tests";
        raw_str negatives = "Negative Tests";
        raw_str positiveAvg = "Positive Avg.";
        raw_str negativeAvg = "Negative Avg.";
        // Details:
        raw_str det_failedTopic = "Failed test for Topic: ";
        raw_str det_testname = "Failed Test: ";
        raw_str det_reason = "Reason and details: ";
        // Statistics:
        raw_str sum_totalTime = "Total time testing";
        raw_str sum_totalTimeSuccess = "Total time of successful tests";
        raw_str sum_totalTests = "Tests run";
        raw_str sum_totalTestsWithMultiplyer = "Tests run (including reruns*tests)";
        raw_str sum_success = "Successful tests";
        raw_str sum_failed = "Failed tests";
        raw_str sum_topics = "Topics tested";
        raw_str sum_topicsFullyTested = "Topics fully tested (including simple)";
        raw_str sum_topicsPartiallyTested = "Topics partially tested (including simple)";
        raw_str sum_topicsUntested = "Topics untested (including simple)";
        raw_str sum_topicsKnown = "Topics known";
        // UNITS:
        raw_str millionSuffix = "m";
        raw_str thousandSuffix = "k";
    };
    raw_str render(const ReportDisplayConfig& config = ReportDisplayConfig(), const Translations& translations = Translations());

   public:  // add additional metadata
    /**
     * show the report all available topics
     * 
     * when test's did not cover a topic it will be shown below the graph
     * => showMissingTopics will hide the report of those
     */
    void setAllTopics(const std::set<raw_str>& topics);
    /**
     * topics which are not shown in the missing-list if they are missing
     * => displayMissingSimpleTopics will show those anyway
     */
    void setSimpleTopics(const std::set<raw_str>& topics);

    /**
     * maybe some topics are sub-topics of other topics
     * 
     * eg, when testing A also covers B and C, put {A:{B,C}} in here
     * => showSubTopics will report those
     */
    void setSubTopics(const std::unordered_map<raw_str, std::set<raw_str>>& references);
    /**
     * when some topics are just "Copies"/"References" of other topics, tell it here
     * 
     * eg. when A is a direct reference to B (Testing A invokes B, or testing B covers the whole content ob A)
     * only one is required to be tested, to exclude both from the missing-list
     * => listDuplicates will show those
     */
    void setDuplicates(const std::unordered_map<raw_str, raw_str>& references);

   protected:  // rendering
    static O& o;
    const static char positiveCountChar;
    const static char negativeCountChar;
    const static Style sHeaderDecoration;
    const static Style sHeader;
    const static Style sHeaderDescription;
    const static Style sTopicOk;
    const static Style sTopicWarn;
    const static Style sTopicFailed;
    const static Style sGPS;
    const static Style sGPF;
    const static Style sGNS;
    const static Style sGNF;
    const static Style sRatioOk;
    const static Style sRatioPartial;
    const static Style sRatioNone;
    const static Style sTime;
    const static Style sFailedTestName;
    const static Style sFailedTestReason;
    const static Style sDuplicate;
    const static Style sDuplicateTarget;

   protected:  // global data [source in brackets]
    /**
     * [config] approximare x-times test execution
     */
    size_t timeScale = 1;
    /**
     * [graph] how much to compress the +- list
     */
    double stretchFactor = 1;
    /**
     * [config] how wide should the +- list be stretched to?
     */
    double graphWidth = 30;
    /**
     * [config] simple topics are hidden in the graph (use when to cluttered...)
     * ONLY EFFECTIVE WHEN TOPIC IS 100% SUCCESSFUL
     */
    bool hideSimpleGraph = false;
    /**
     * [render-param] what to "tell" when outputting static texts
     */
    Translations translations;

   protected:  // test-result-data (data extracted from test-results)
    class TopicData {
       public:
        TopicData() {}
        // name stored as map-key
        time_hr totalTimePositive{0};
        time_hr totalTimeNegative{0};
        time_hr totalExecuteTime{0};
        size_t positiveTests = 0;
        size_t negativeTests = 0;
        size_t positiveSuccessful = 0;
        size_t negativeSuccessful = 0;
        size_t multiplyer = 1;
        size_t totalTests() const { return positiveTests + negativeTests; }
        bool success() const { return positiveTests == positiveSuccessful && negativeTests == negativeSuccessful; }
        class FailedDetails {
           public:
            raw_str testname = "";
            raw_str reason = "";
            raw_str details = "";
        };
        std::vector<FailedDetails> failed;
    };
    std::unordered_map<raw_str, TopicData> testedTopics;
    void readTestResults(Test* t, size_t multiplier);

   protected:  // env-data (data set afterwards)
    std::set<raw_str> allTopics;
    std::set<raw_str> simpleTopics;
    std::unordered_map<raw_str, std::set<raw_str>> subTopics;
    std::set<raw_str> indirectTested;
    std::set<raw_str> indirectSimple;
    std::set<raw_str> dupIndirect;
    std::set<raw_str> dupDirect;
    std::unordered_map<raw_str, raw_str> duplicates;
    // reruns in multiplyer=>runs
    std::map<size_t, size_t> reruns;

   protected:  // global render methods
    raw_str GR_graph();
    raw_str GR_MissingTopics(bool simple = false);
    raw_str GR_SubTopics();
    raw_str GR_Duplicates();
    raw_str GR_Details();
    raw_str GR_Statistics();

   protected:  // partial render methods (belonging to a specific part)
    // Graph
    Support::TablePrinter::rowType buildGraphRow(raw_str_arg topicName, const TopicData& data);
    raw_str renderPMRow(raw_str_arg posLS, raw_str_arg negLS, raw_str_arg posLF, raw_str_arg negLF);
    raw_str renderRatio(size_t good, size_t bad);
    double calcStretchFactor(const TopicData& e);
    // Missing + simple-missing
    /**
     * simple: when true
     *              return a list of untested simple rules
     *         when false
     *              return a list of untested non-simple rules
     */
    std::set<raw_str> getUntestedTopics(bool simple);
    // // // Sub-logic to getUntestedTopics
    bool isSimple(raw_str_arg topic);
    bool isTested(raw_str_arg topic);
    bool hasTestedRef(raw_str_arg topic);
    // // //
    // Summary
    raw_str renderTotalTime();
    std::set<raw_str> listFullyTested();
    std::set<raw_str> listPartiallyTested();
    // kinda like getUntestedTopics, but not filtering for simple tests
    std::set<raw_str> listUntested();
    bool hasReruns();
    raw_str printReruns();

    // global precalc
    void updateIndirects();

   protected:  // helper render methods (belonging nowhere specific...)
    raw_str nameNumber(size_t number);
    raw_str renderHeadline(raw_str_arg header, raw_str_arg description);
    friend CombinedReport;
};
}  // namespace Meko::TestSuite
#endif
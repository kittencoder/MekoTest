
#include "../Meko.h"

namespace Meko::TestSuite {
using namespace Color;
using namespace Support;
const char Report::positiveCountChar = '+';
const char Report::negativeCountChar = '-';
O& Report::o(O::getInstance());
const Style Report::sHeaderDecoration(FG::Yellow);
const Style Report::sHeader(FG::Light_Magenta);
const Style Report::sHeaderDescription(FG::Blue);
const Style Report::sTopicOk(FG::Light_Green);
const Style Report::sTopicWarn(FG::Yellow);
const Style Report::sTopicFailed(FG::Light_Red);
const Style Report::sGPS(FG::Light_Green);
const Style Report::sGPF(FG::Light_Red);
const Style Report::sGNS(FG::Light_Green);
const Style Report::sGNF(FG::Light_Red);
const Style Report::sRatioOk(FG::Light_Green);
const Style Report::sRatioPartial(FG::Yellow);
const Style Report::sRatioNone(FG::Light_Red);
const Style Report::sTime(FG::Cyan);
const Style Report::sFailedTestName(FG::Light_Red, BG::Black);
const Style Report::sFailedTestReason(FG::Light_Red);
const Style Report::sDuplicate(FG::Yellow);
const Style Report::sDuplicateTarget(FG::Light_Yellow);
Report::Report(std::set<Test*> tests, size_t multiplyer) {
    for (Test* t : tests) {
        readTestResults(t, multiplyer);
    }
    reruns[multiplyer] = tests.size();
}

void Report::setAllTopics(const std::set<raw_str>& topics) {
    allTopics = topics;
}
void Report::setSimpleTopics(const std::set<raw_str>& topics) {
    simpleTopics = topics;
}
void Report::setSubTopics(const std::unordered_map<raw_str, std::set<raw_str>>& references) {
    subTopics = references;
}
void Report::setDuplicates(const std::unordered_map<raw_str, raw_str>& references) {
    duplicates = references;
}

void Report::readTestResults(Test* t, size_t multiplyer) {
    bool posTest = t->shouldBeSuccessful();
    bool worked = t->fulfilled();
    auto& e = testedTopics[t->getTopic()];
    e.positiveTests += posTest;
    e.negativeTests += !posTest;
    e.positiveSuccessful += posTest * worked;
    e.negativeSuccessful += (!posTest) * worked;
    e.totalExecuteTime += t->getExecTime();
    e.multiplyer = multiplyer;
    if (worked) {
        // only account time of successful tests
        e.totalTimePositive += t->getAverageExecTime() * posTest;
        e.totalTimeNegative += t->getAverageExecTime() * (!posTest);
    } else {
        // only store reasons and details for failed tests
        e.failed.push_back({t->getTestName(), t->getErrorReason(), t->getDetails()});
    }
}
Report::operator raw_str() {
    return render();
}
raw_str Report::render(const ReportDisplayConfig& config, const Translations& pTranslations) {
    // setup
    std::vector<raw_str> parts;
    bool colorState = o.getColorState();
    o.enableColors(config.color);
    graphWidth = config.graphWidth;
    timeScale = config.timeScale;
    hideSimpleGraph = config.hideSimpleGraph;
    translations = pTranslations;
    updateIndirects();
    // gather all known topics from all sources
    for (const auto& [topicName, topicData] : testedTopics) {
        allTopics.insert(topicName);
    }
    for (const auto& [topicNameA, topicNameB] : duplicates) {
        allTopics.insert(topicNameA);
        allTopics.insert(topicNameB);
    }
    for (const auto& [referencingTopic, subTopicList] : subTopics) {
        allTopics.insert(referencingTopic);
        for (const auto& topicName : subTopicList) {
            allTopics.insert(topicName);
        }
    }
    for (const auto& topicName : simpleTopics) {
        allTopics.insert(topicName);
    }
    // render parts
    if (config.renderGraph) {
        parts.push_back(renderHeadline(translations.h_Graph, translations.d_graph));
        parts.push_back(GR_graph());
    }
    if (config.showMissingTopics) {
        parts.push_back(renderHeadline(translations.h_missingTopics, translations.d_missingTopics));
        parts.push_back(GR_MissingTopics(false));
        if (config.displayMissingSimpleTopics) {
            parts.push_back(renderHeadline(translations.h_simpleTopics, translations.d_simpleTopics));
            parts.push_back(GR_MissingTopics(true));
        }
        if (config.showSubTopics) {
            parts.push_back(renderHeadline(translations.h_subTopics, translations.d_subTopics));
            parts.push_back(GR_SubTopics());
        }
    }
    if (config.listDuplicates) {
        parts.push_back(renderHeadline(translations.h_Duplicates, translations.d_Duplicates));
        parts.push_back(GR_Duplicates());
    }
    if (config.listFailDetails) {
        parts.push_back(renderHeadline(translations.h_Details, translations.d_Details));
        parts.push_back(GR_Details());
    }
    if (config.statistics) {
        parts.push_back(renderHeadline(translations.h_Statistics, translations.d_Statistics));
        parts.push_back(GR_Statistics());
    }
    // cleanup + collection
    o.enableColors(colorState);
    return StringUtilities::implode("\n", parts);
}
raw_str Report::GR_graph() {
    TablePrinter::sheetType graph;
    raw_str scaleTxt = timeScale == 1 ? "" : ("/" + nameNumber(timeScale));
    graph.push_back({translations.topic, translations.graph, translations.positives, translations.negatives, translations.positiveAvg + scaleTxt, translations.negativeAvg + scaleTxt});
    auto topicsSorted = extractKeys(testedTopics);
    std::sort(topicsSorted.begin(), topicsSorted.end(), [this](const auto& a, const auto& b) {
        return testedTopics[a].totalTests() > testedTopics[b].totalTests();
    });
    if (!testedTopics.empty()) {
        stretchFactor = calcStretchFactor(testedTopics[topicsSorted[0]]);
    }
    for (const auto& topicName : topicsSorted) {
        if (hideSimpleGraph && isSimple(topicName)) {
            if (testedTopics[topicName].success()) {
                continue;
            }
        }
        graph.push_back(buildGraphRow(topicName, testedTopics[topicName]));
    }
    return TablePrinter::getInstance().render(graph);
}
double Report::calcStretchFactor(const TopicData& e) {
    return std::min(1., graphWidth / (double(e.totalTests())));
}
TablePrinter::rowType Report::buildGraphRow(raw_str_arg topicName, const TopicData& data) {
    TablePrinter::rowType rt;
    size_t positiveTestsSuccess = data.positiveSuccessful;
    size_t negativeTestsSuccess = data.negativeSuccessful;
    size_t positiveTestsFailed = data.positiveTests - data.positiveSuccessful;
    size_t negativeTestsFailed = data.negativeTests - data.negativeSuccessful;
    raw_str posLS = StringUtilities::pad("", ((double)positiveTestsSuccess) * stretchFactor, PadDirection::PAD_RIGHT, positiveCountChar);
    raw_str negLS = StringUtilities::pad("", ((double)negativeTestsSuccess) * stretchFactor, PadDirection::PAD_RIGHT, negativeCountChar);
    raw_str posLF = StringUtilities::pad("", ((double)positiveTestsFailed) * stretchFactor, PadDirection::PAD_RIGHT, positiveCountChar);
    raw_str negLF = StringUtilities::pad("", ((double)negativeTestsFailed) * stretchFactor, PadDirection::PAD_RIGHT, negativeCountChar);
    if (positiveTestsSuccess > 0 && posLS.length() == 0) {
        posLS += positiveCountChar;
    }
    if (negativeTestsSuccess > 0 && negLS.length() == 0) {
        negLS += negativeCountChar;
    }
    if (positiveTestsFailed > 0 && posLF.length() == 0) {
        posLF += positiveCountChar;
    }
    if (negativeTestsFailed > 0 && negLF.length() == 0) {
        negLF += negativeCountChar;
    }

    auto TTimePos = data.totalTimePositive;
    if (data.positiveSuccessful != 0) {
        TTimePos /= data.positiveSuccessful;
    }
    auto TTimeNeg = data.totalTimeNegative;
    if (data.negativeSuccessful != 0) {
        TTimeNeg /= data.negativeSuccessful;
    }

    raw_str timePos = TimeUtilities::renderSimple(TTimePos * timeScale, true);
    raw_str timeNeg = TimeUtilities::renderSimple(TTimeNeg * timeScale, true);
    if (TTimePos.count() == 0) {
        timePos = "---";
    }
    if (TTimeNeg.count() == 0) {
        timeNeg = "---";
    }
    return {
        o.p(topicName, data.success() ? sTopicOk : sTopicFailed),
        renderPMRow(posLS, negLS, posLF, negLF),
        renderRatio(positiveTestsSuccess, positiveTestsFailed),
        renderRatio(negativeTestsSuccess, negativeTestsFailed),
        o.p(timePos, sTime),
        o.p(timeNeg, sTime),
    };
    return rt;
}
raw_str Report::renderPMRow(raw_str_arg posLS, raw_str_arg negLS, raw_str_arg posLF, raw_str_arg negLF) {
    return o.p(posLS, sGPS) + o.p(posLF, sGPF) + o.p(negLS, sGNS) + o.p(negLF, sGNF);
}
raw_str Report::renderRatio(size_t good, size_t bad) {
    raw_str s = to_raw_str(good) + "/" + to_raw_str(good + bad);
    if (good == 0) {
        return o.p(s, sRatioNone);
    }
    if (bad == 0) {
        return o.p(s, sRatioOk);
    }
    return o.p(s, sRatioPartial);
}
raw_str Report::GR_MissingTopics(bool simple) {
    auto topics = getUntestedTopics(simple);
    std::set<raw_str> transformed;
    for (auto& topic : topics) {
        transformed.insert(o.p(topic, simple ? sTopicWarn : sTopicFailed));
    }
    return StringUtilities::implode(", ", transformed);
}
raw_str Report::GR_SubTopics() {
    std::vector<raw_str> topics(indirectTested.begin(), indirectTested.end());
    for (auto& topic : topics) {
        topic = o.p(topic, sTopicWarn);
    }
    return StringUtilities::implode(", ", topics);
}
raw_str Report::GR_Duplicates() {
    std::vector<raw_str> entries;
    AutoPadder pTopic(' ', PadDirection::PAD_LEFT);
    for (const auto& [topic, references] : duplicates) {
        pTopic.show(topic.length());
    }
    for (const auto& [topic, references] : duplicates) {
        entries.push_back(pTopic(o.p(topic, sDuplicate)) + " -> " + o.p(references, sDuplicateTarget));
    }
    return StringUtilities::implode("\n", entries);
}
raw_str Report::GR_Details() {
    raw_str r;
    for (const auto& [topicName, topicData] : testedTopics) {
        if (topicData.failed.size() != 0) {
            r += o.p(translations.det_failedTopic, sHeaderDecoration) + o.p(topicName, sTopicFailed);
            for (const auto& fail : topicData.failed) {
                r += "\n" + translations.det_testname + o.p(fail.testname, sFailedTestName) + "\n" + translations.det_reason + o.p(fail.reason, sFailedTestReason) + "\n" + fail.details + "\n";
            }
        }
    }
    return r;
}
raw_str Report::GR_Statistics() {
    TablePrinter::sheetType table;
    time_hr totalTime{0};
    time_hr totalSuccessTime{0};
    size_t testsRun = 0;
    size_t successful = 0;
    for (const auto& [topicName, topicData] : testedTopics) {
        totalTime += topicData.totalExecuteTime;
        totalSuccessTime += topicData.totalTimeNegative + topicData.totalTimePositive;
        testsRun += topicData.positiveTests + topicData.negativeTests;
        successful += topicData.positiveSuccessful + topicData.negativeSuccessful;
    }
    size_t failed = testsRun - successful;

    table.push_back({translations.sum_totalTime, o.p(TimeUtilities::renderSimple(totalTime, true), sTime)});
    table.push_back({translations.sum_totalTimeSuccess, o.p(TimeUtilities::renderSimple(totalSuccessTime, true), sTime)});
    if (hasReruns()) {
        table.push_back({translations.sum_totalTestsWithMultiplyer, printReruns()});
    } else {
        table.push_back({translations.sum_totalTests, printReruns()});
    }
    table.push_back({translations.sum_failed, o.p(to_raw_str(failed), failed == 0 ? sTopicOk : sTopicFailed)});
    table.push_back({translations.sum_success, o.p(to_raw_str(successful), successful != 0 ? sTopicOk : sTopicFailed)});
    table.push_back({translations.sum_topics, to_raw_str(testedTopics.size())});
    table.push_back({translations.sum_topicsKnown, to_raw_str(allTopics.size())});

    size_t fullyTested = listFullyTested().size();
    size_t partiallyTested = listPartiallyTested().size();
    size_t untested = listUntested().size();
    table.push_back({translations.sum_topicsFullyTested, o.p(to_raw_str(fullyTested), fullyTested == 0 ? sTopicFailed : sTopicOk)});
    table.push_back({translations.sum_topicsPartiallyTested, o.p(to_raw_str(partiallyTested), partiallyTested == 0 ? sTopicOk : sTopicWarn)});
    table.push_back({translations.sum_topicsUntested, o.p(to_raw_str(untested), untested == 0 ? sTopicOk : sTopicFailed)});

    return TablePrinter::getInstance().render(table, new TablePrinterStyle(true, true));
}

raw_str Report::renderHeadline(raw_str_arg header, raw_str_arg description) {
    return o.p("### ", sHeaderDecoration) + o.p(header, sHeader) + "\n" + o.p(description + ":", sHeaderDescription);
}
raw_str Report::nameNumber(size_t number) {
    if (number >= 1000000) {
        return to_raw_str(number / 1000000) + translations.millionSuffix;
    }
    if (number >= 1000) {
        return to_raw_str(number / 1000) + translations.thousandSuffix;
    }
    return to_raw_str(number);
}
std::set<raw_str> Report::getUntestedTopics(bool simple) {
    std::set<raw_str> r;
    for (const auto& topic : allTopics) {
        if (isSimple(topic) != simple || isTested(topic) || hasTestedRef(topic)) {
            continue;
        }
        r.insert(topic);
    }
    return r;
}
bool Report::isSimple(raw_str_arg topic) {
    return std::find(simpleTopics.begin(), simpleTopics.end(), topic) != simpleTopics.end();
}
bool Report::isTested(raw_str_arg topic) {
    return testedTopics.find(topic) != testedTopics.end() || dupDirect.contains(topic);
}
bool Report::hasTestedRef(raw_str_arg topic) {
    return indirectTested.find(topic) != indirectTested.end() || indirectSimple.contains(topic) || dupIndirect.contains(topic);
}
void Report::updateIndirects() {
    indirectTested = {};
    indirectSimple = {};
    dupIndirect = {};
    dupDirect = {};
    size_t oldCnt;
    do {
        oldCnt = indirectTested.size() + indirectSimple.size();
        for (const auto& [root, referenced] : subTopics) {
            if (isTested(root) || hasTestedRef(root)) {
                for (const auto& ref : referenced) {
                    if (!isSimple(ref)) {
                        indirectTested.insert(ref);
                    } else {
                        indirectSimple.insert(ref);
                    }
                }
            }
        }
    } while (oldCnt != indirectTested.size() + indirectSimple.size());
    for (const auto& [dup1, dup2] : duplicates) {
        if (isTested(dup1)) {
            dupDirect.insert(dup2);
        } else if (isTested(dup2)) {
            dupDirect.insert(dup1);
        } else if (hasTestedRef(dup1)) {
            dupIndirect.insert(dup2);
        } else if (hasTestedRef(dup2)) {
            dupIndirect.insert(dup1);
        }
    }
}
std::set<raw_str> Report::listFullyTested() {
    std::set<raw_str> r;
    for (const auto& t : allTopics) {
        if (isTested(t)) {
            r.insert(t);
        }
    }
    return r;
}
std::set<raw_str> Report::listPartiallyTested() {
    std::set<raw_str> r;
    for (const auto& t : allTopics) {
        if (isTested(t)) {
            continue;
        }
        if (hasTestedRef(t)) {
            r.insert(t);
        }
    }
    return r;
}
std::set<raw_str> Report::listUntested() {
    std::set<raw_str> r;
    for (const auto& t : allTopics) {
        if (isTested(t) || hasTestedRef(t)) {
            continue;
        }
        r.insert(t);
    }
    return r;
}
bool Report::hasReruns() {
    for (const auto& [mul, cnt] : reruns) {
        if (mul != 1) {
            return true;
        }
    }
    return false;
}
raw_str Report::printReruns() {
    std::vector<raw_str> segments;
    size_t sum = 0;
    for (const auto& [mul, cnt] : reruns) {
        segments.push_back(to_raw_str(mul) + "*" + to_raw_str(cnt));
        sum += mul * cnt;
    }
    if (!hasReruns()) {
        return to_raw_str(sum);
    }
    return to_raw_str(sum) + " = ( " + Support::StringUtilities::implode(" + ", segments) + " )";
}
}  // namespace Meko::TestSuite

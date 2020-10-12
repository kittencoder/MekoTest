#include "../Meko.h"

namespace Meko::TestSuite {
const Style CombinedReport::sSuitename(FG::Black, BG::White);
std::unordered_map<raw_str, Report> CombinedReport::getIndividualReports() {
    return individualReports;
}
CombinedReport::CombinedReport(const std::unordered_map<raw_str, Report>& reports, const std::unordered_map<raw_str, raw_str>& humanReadableNames) {
    if (reports.empty()) {
        return;
    }
    for (const auto& [suiteName, report] : reports) {
        raw_str fsname = suiteName;
        if (humanReadableNames.find(fsname) != humanReadableNames.end()) {
            fsname = humanReadableNames.at(fsname);
        }
        mergeInto(" : " + o.p(fsname, sSuitename), report);
        for (const auto& [rerunMul, rerunCnt] : report.reruns) {
            reruns[rerunMul] += rerunCnt;
        }
    }
}
void CombinedReport::mergeInto(raw_str_arg suiteName, const Report& report) {
    for (const auto& [topicName, topicData] : report.testedTopics) {
        testedTopics[topicName + suiteName] = topicData;
    }
    suffix(suiteName, report.allTopics, allTopics);
    suffix(suiteName, report.simpleTopics, simpleTopics);
    for (const auto& [k, v] : report.subTopics) {
        std::set<raw_str> r;
        suffix(suiteName, v, r);
        subTopics[k + suiteName] = r;
    }
    suffix(suiteName, report.indirectTested, indirectTested);
    suffix(suiteName, report.indirectSimple, indirectSimple);
    suffix(suiteName, report.dupIndirect, dupIndirect);
    suffix(suiteName, report.dupDirect, dupDirect);
    for (const auto& [a, b] : report.duplicates) {
        duplicates[a + suiteName] = b + suiteName;
    }
}
void CombinedReport::suffix(raw_str_arg suffix, const std::set<raw_str>& data, std::set<raw_str>& target) {
    for (const auto& e : data) {
        target.insert(e + suffix);
    }
}

}  // namespace Meko::TestSuite

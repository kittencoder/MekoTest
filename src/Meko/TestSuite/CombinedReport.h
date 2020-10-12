#ifndef MEKO_TESTSUITE_COMBINEDREPORT_H
#define MEKO_TESTSUITE_COMBINEDREPORT_H

#include "../Meko.h"

namespace Meko::TestSuite {
using namespace Color;
/**
 * combines multiple report-instances into a single large report!
 */
class CombinedReport : public Report {
   public:
    /**
     * pass reports as a list of [suite-name => report]
     * 
     * topics in the combined report will be suffixed by their suite-name
     */
    CombinedReport(const std::unordered_map<raw_str, Report>& reports, const std::unordered_map<raw_str, raw_str>& humanReadableNames = {});
    std::unordered_map<raw_str, Report> getIndividualReports();

   protected:
    std::unordered_map<raw_str, Report> individualReports;
    /**
     * suite-name already suffixed by ":"
     */
    void mergeInto(raw_str_arg suiteName, const Report& report);
    void suffix(raw_str_arg suffix, const std::set<raw_str>& data, std::set<raw_str>& target);
    const static Style sSuitename;
};
}  // namespace Meko::TestSuite
#endif
#ifndef MEKO_TESTSUITE_TESTFAILED_H
#define MEKO_TESTSUITE_TESTFAILED_H

#include "../Meko.h"

namespace Meko::TestSuite {
/**
 * throw when a test failed
 */
class TestFailed : public std::exception {
   public:
    TestFailed(raw_str_arg reason, raw_str_arg details);
    raw_str getReason() const;
    raw_str getDetails() const;

   protected:
    raw_str reason;
    raw_str details;
};
}  // namespace Meko::TestSuite
#endif
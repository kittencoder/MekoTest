#ifndef MEKO_TESTSUITE_TESTSUITE_H
#define MEKO_TESTSUITE_TESTSUITE_H

#include "../Meko.h"

namespace Meko::TestSuite {
using namespace Color;
using Support::AutoPadder;
/**
 * a test instance
 */
class TestSuite {
   public:
    /**
     * printProgress when enabled print the status of started/done tests regularly
     * 
     * maxConcurrency is the maximum number of concurrently running tests
     * special cases:
     *  0  : exact number of cpu-cores !!! system-usage will influence timing
     * -1  : autodetect cpu-cores minus 1 (so when using 4 cores, run tests on 3. when using 8 cores, run tests on 7)
     * -2  : half the number of detected cpu-cores
     * any other negative value : default to -1
     * 
     * 
     * be aware that cpu/resource usage of the system might always influence timing
     * 
     * idleTime is the time to let pass if all slots are filled
     * rampUp is the time to wait after starting tests
     */
    TestSuite(bool printProgress = true, ssize_t maxConcurrency = -1, time_hr rampUp = time_mr(0), time_hr idleTime = time_mr(0));
    ~TestSuite();
    /**
     * add a test and the suite takes ownership of that test-instance
     * so you don't have to manage deleting/keeping test-cases
     */
    void addTest(Test* test);

    /**
     * run provided test-cases, returns true if all tests succeed, false if any failed
     */
    Report run(size_t multiplyer = 1);

   protected:
    /**
     * run a maximum of num tests in parallel
     */
    void runConc(size_t num, size_t multiplyer);
    /**
     * calculate the maximum number of concurrently running tests
     */
    size_t maxConcurrent();

   protected:  // progress-printer
    void doPrintProgress(size_t allowed, size_t started, size_t successful, size_t failed);
    const static Style sRunning;
    const static Style sDone;
    const static Style sSuccessful;
    const static Style sFailed;
    const static Style sRuntime;
    const static Style sLeft;
    AutoPadder pRunning;
    AutoPadder pDone;
    AutoPadder pSuccessful;
    AutoPadder pFailed;
    AutoPadder pTime;
    size_t oldPrintInfoLength = 0;
    Support::ETASimple eta;

   protected:
    std::set<Test*> tests;
    bool printProgress;
    ssize_t maxConcurrency;
    time_hr rampUp;
    time_hr idleTime;
};
}  // namespace Meko::TestSuite
#endif
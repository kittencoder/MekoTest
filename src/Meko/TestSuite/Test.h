#ifndef MEKO_TESTSUITE_TEST_H
#define MEKO_TESTSUITE_TEST_H

#include "../Meko.h"

namespace Meko::TestSuite {
class TestSuite;
/**
 * a test instance
 */
class Test : public Exec::Run {
   public:
    Test(raw_str_arg testname, raw_str_arg topic, bool shouldSucceed);

   protected:
    friend Meko::TestSuite::TestSuite;
    void EXEC() override;
    // called by the suite to deactivate cross-process communication...
    void runsInSync();
    bool runsUnsync = true;
    void POSTEXEC(bool normalExit) override;
    /**
     * automatically called by this class in the client
     */
    void publishResults();
    /**
     * automatically called by this class in the host after execution is done
     */
    void collectResults();
    /**
     * implement this
     * 
     * it might get called multiple times (if requested, so an average runtime may be calculated)
     * 
     * use `startTimer`&`stopTimer` to capture execution time
     * or `addTime` to add it manually
     * 
     * use `setErrorReason` to add a reason for failure
     * use `setErrorDetails` to add details to the error-report
     */
    virtual bool ExecuteTest() = MUST_BE_IMPLEMENTED;
    /**
     * executed once before running the test (possibly multiple times)
     * inside the test-process.
     * 
     * Initialize larger test values here instead of the constructor, to avoid copying and using memory from the main-process.
     * 
     * when returning false, the test won't be executed and be considered to have failed.
     */
    virtual bool prepare();
    /**
     * in case anything from `prepare` needs cleanup, do it here
     * 
     * won't be called when "prepare" returns false
     */
    virtual void cleanup();

   protected:  // interfacing with the test-suite
    /**
     * start the internal timer
     */
    void startTimer();
    /**
     * stop the internal timer and save the difference from starting it
     */
    void stopTimer();
    /**
     * as an alternative to the timer a self-calculated time can be passed
     */
    void addTime(const time_hr& executionTime);
    void setErrorReason(raw_str_arg reason);
    void setErrorDetails(raw_str_arg details);

   public:  // static values
    /**
     * tests run as expected will be grouped by topics
     */
    raw_str getTopic() const;
    /**
     * will be shown when the test failed
     */
    raw_str getTestName() const;
    /**
     * some tests are designed to fail
     */
    bool shouldBeSuccessful() const;

   public:  // test-result-values
    /**
     * was the test meeting the expectation
     */
    bool wasSuccessful() const;
    /**
     * how long did the test ran? (provided by the test itself, so overhead-time can be removed)
     */
    time_hr getExecTime() const;
    /**
     * a short string telling what went wrong
     * can(/should) be color-coded
     */
    raw_str getErrorReason() const;
    /**
     * some details (eg. source-code, line-numbers, etc)
     * can(/should) be color-coded
     */
    raw_str getDetails() const;

   public:  // calculated values
    time_hr getAverageExecTime() const;
    bool fulfilled() const;

   protected:  // constant values
    raw_str testname;
    raw_str topic;
    bool shouldSucceed;
    size_t multiplyer = 1;

   public:
    /**
     * how often should the test be run?
     */
    void setMultiplyer(size_t multiplyer);

   protected:  // time
    bool timerStarted = false;
    bool timerStopped = false;
    clock::time_point timerStart;
    time_hr executionTime{0};

   protected:  // values
    bool success = false;
    raw_str reason = "";
    raw_str details = "";
};
}  // namespace Meko::TestSuite
#endif

#include "../Meko.h"

typedef Meko::Exec::CommPipe::PipeType pt;

namespace Meko::TestSuite {
Test::Test(raw_str_arg testname, raw_str_arg topic, bool shouldSucceed) : Run({
                                                                              {"execTime", pt::CLIENT_TO_HOST},
                                                                              {"success", pt::CLIENT_TO_HOST},
                                                                              {"errorReason", pt::CLIENT_TO_HOST},
                                                                              {"errorDetails", pt::CLIENT_TO_HOST},
                                                                          }),
                                                                          testname(testname),
                                                                          topic(topic),
                                                                          shouldSucceed(shouldSucceed) {
}

void Test::EXEC() {
    if (!prepare()) {
        setErrorReason("preparation failed");
        success = false;
        publishResults();
        return;
    }
    auto globalStart = clock::now();
    for (size_t iteration = 0; iteration < multiplyer; ++iteration) {
        try {
            success = ExecuteTest();
        } catch (const TestFailed& testfail) {
            setErrorReason(testfail.getReason());
            setErrorDetails(testfail.getDetails());
            success = false;
        } catch (const std::exception& genericException) {
            setErrorReason("due to exception");
            setErrorDetails(genericException.what());
            success = false;
        }
        if (!fulfilled()) {
            // only successful tests are being rerun
            // failing test-times aren't used in the report-calculation
            break;
        }
    }
    if (!timerStopped) {
        if (!timerStarted) {
            executionTime = clock::now() - globalStart;
        } else {
            stopTimer();
        }
    }
    cleanup();
    publishResults();
}
bool Test::prepare() {
    return true;
}
void Test::cleanup() {
}
void Test::POSTEXEC(bool normalExit) {
    NOT_USED(normalExit);
    collectResults();
}
void Test::publishResults() {
    getWritePipe("execTime") << executionTime.count() << std::flush;
    getWritePipe("success") << success << std::flush;
    getWritePipe("errorReason") << reason << std::flush;
    getWritePipe("errorDetails") << details << std::flush;
}
void Test::collectResults() {
    time_hr::rep t{0};
    getReadPipe("execTime") >> t;
    executionTime = time_hr(t);
    getReadPipe("success") >> success;
    reason = Support::StringUtilities::dumpStream(getReadPipe("errorReason"));
    details = Support::StringUtilities::dumpStream(getReadPipe("errorDetails"));
}
void Test::startTimer() {
    if (timerStarted) {
        return;  // don't double-start, catch the whole set
    }
    timerStarted = true;
    timerStart = clock::now();
}
void Test::stopTimer() {
    if (!timerStarted) {
        return;
    }
    executionTime += clock::now() - timerStart;
    timerStarted = false;
    timerStopped = true;
}
void Test::addTime(const time_hr& et) {
    // used timing-feature
    timerStopped = true;
    executionTime += et;
}
void Test::setErrorReason(raw_str_arg r) {
    reason = r;
}
void Test::setErrorDetails(raw_str_arg d) {
    details = d;
}
raw_str Test::getTopic() const {
    return topic;
}
raw_str Test::getTestName() const {
    return testname;
}
bool Test::shouldBeSuccessful() const {
    return shouldSucceed;
}

bool Test::wasSuccessful() const {
    return success;
}
time_hr Test::getExecTime() const {
    return executionTime;
}
raw_str Test::getErrorReason() const {
    return reason;
}
raw_str Test::getDetails() const {
    return details;
}
void Test::setMultiplyer(size_t m) {
    multiplyer = m;
}

time_hr Test::getAverageExecTime() const {
    return getExecTime() / multiplyer;
}
bool Test::fulfilled() const {
    return shouldBeSuccessful() == wasSuccessful();
}
}  // namespace Meko::TestSuite
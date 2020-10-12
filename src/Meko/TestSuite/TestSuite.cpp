
#include "../Meko.h"

namespace Meko::TestSuite {
using namespace Color;
using Support::AutoPadder;
const Style TestSuite::sRunning(FG::Light_Cyan);
const Style TestSuite::sDone(FG::Light_Magenta);
const Style TestSuite::sSuccessful(FG::Green);
const Style TestSuite::sFailed(FG::Light_Red);
const Style TestSuite::sRuntime(FG::Light_Magenta);
const Style TestSuite::sLeft(FG::Yellow);
TestSuite::TestSuite(bool printProgress, ssize_t maxConcurrency, time_hr rampUp, time_hr idleTime) : printProgress(printProgress),
                                                                                                     maxConcurrency(maxConcurrency),
                                                                                                     rampUp(rampUp),
                                                                                                     idleTime(idleTime) {
}
TestSuite::~TestSuite() {
    for (Test* t : tests) {
        delete t;
    }
}
void TestSuite::addTest(Test* test) {
    tests.insert(test);
}
Report TestSuite::run(size_t multiplyer) {
    if (multiplyer < 1) {
        multiplyer = 1;
    }
    if (tests.empty()) {
        return Report({}, multiplyer);
    }
    runConc(maxConcurrent(), multiplyer);
    if (printProgress) {
        std::cout << std::endl;
    }
    return Report(tests, multiplyer);
}
void TestSuite::runConc(size_t num, size_t multiplyer) {
    size_t started = 0;
    size_t successful = 0;
    size_t failed = 0;
    auto currentStart = tests.begin();
    auto currentCollect = tests.begin();
    size_t runningTests = 0;
    eta.setTotal(tests.size()).start();
    /**
     * 1. run tests until "num" tests have started
     * or tests.end() is reached
     * 
     * 2. check all running tests for exists
     * 3.a if some have exited, collect as many as possible
     * 3.b if none have exited, wait for one
     */
    while (currentCollect != tests.end()) {
        // 1.
        while (runningTests < num && currentStart != tests.end()) {
            (*currentStart)->setMultiplyer(multiplyer);
            if (!(*currentStart)->execute()) {
                std::cout << std::endl
                          << "EXECUTION FAILED FOR" << (*currentStart)->getTestName() << std::endl;
            }
            doPrintProgress(num, ++started, successful, failed);
            if (rampUp.count() > 0) {
                std::this_thread::sleep_for(rampUp);
            }
            ++runningTests;
            ++currentStart;
        }
        bool stopped = false;
        // 2.
        for (auto it = currentCollect; it != currentStart; ++it) {
            if (it == tests.end()) {
                break;
            }
            if ((*it)->hasStopped()) {
                stopped = true;
                --runningTests;
                bool fulfilled = (*it)->fulfilled();
                successful += fulfilled;
                failed += !fulfilled;
                doPrintProgress(num, started, successful, failed);
            }
        }
        // 3
        if (stopped) {
            // 3.a
            while (currentCollect != currentStart && (!(*currentCollect)->isRunning())) {
                // update output here too in case it has changed meanwhile here...
                if ((*currentCollect)->hasStopped()) {
                    --runningTests;
                    bool fulfilled = (*currentCollect)->fulfilled();
                    successful += fulfilled;
                    failed += !fulfilled;
                    doPrintProgress(num, started, successful, failed);
                }
                ++currentCollect;
            }
        } else {
            // 3.b
            if (idleTime.count() > 0) {
                std::this_thread::sleep_for(idleTime);
            } else {
                (*currentCollect)->wait();
            }
        }
    }
}
size_t TestSuite::maxConcurrent() {
    if (maxConcurrency > 0) {
        return maxConcurrency;
    }
    size_t cores = std::thread::hardware_concurrency();
    size_t v = 0;
    switch (maxConcurrency) {
        case 0:
            v = cores;
            break;
        case -2:
            v = cores / 2;
            break;

        default:  //and -1
            v = cores - 1;
            break;
    }
    return std::max((size_t)1, v);
}
void TestSuite::doPrintProgress(size_t allowed, size_t started, size_t successful, size_t failed) {
    if (!printProgress) {
        return;
    }
    size_t done = successful + failed;
    size_t running = started - done;
    auto& o = O::getInstance();
    std::stringstream ss;
    ss << "\rDone: " << o.p(pDone(to_raw_str(done)), sDone) << "/" << tests.size()
       << ", Running: " << o.p(pRunning(to_raw_str(running)), sRunning) << "/" << allowed
       << ", Ok/Fail: " << o.p(pSuccessful(to_raw_str(successful)), sSuccessful) << " "
       << o.p(pFailed(to_raw_str(failed)), sFailed);

    eta.setCurrent(done);

    size_t pil = ss.str().length();
    if (pil != oldPrintInfoLength || eta.shouldPrint()) {
        oldPrintInfoLength = pil;
        ss << pTime(raw_str(" ") + o.p(eta.renderTimePassed(), sRuntime) + ", eta: " + o.p(eta.renderTimeLeft(), sLeft));
    }
    std::cout << ss.str() << std::flush;
}
}  // namespace Meko::TestSuite
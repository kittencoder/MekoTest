#include "Meko.h"

using namespace Meko::Support;
using namespace Meko::Color;

void printTimeTable(Meko::raw_str_arg text, bool precise) {
    auto& o = O::getInstance();
    // check the Tables.md and Tables.cpp files for table-examples
    TablePrinter::sheetType sheet;
    std::cout << text << std::endl;
    sheet.push_back({"raw nanoseconds", "rendered value"});
    std::chrono::nanoseconds tv(1);
    for (size_t i = 0; i < 15; ++i) {
        // print raw nanosecond-count and "simple-rendered" time values
        sheet.push_back({o.p(to_raw_str(tv.count()), FG::Magenta), o.p(TimeUtilities::renderSimple(tv, precise), FG::Cyan)});

        tv = tv * 11;
    }
    std::cout << TablePrinter::getInstance().render(sheet);
}

int main(int argc, char** argv) {
    try {
        auto& o = O::getInstance();

        printTimeTable("non precise printing", false);
        printTimeTable("precise printing", true);

        std::cout << "ETA calculator:" << std::endl;
        size_t total = 700;
        // the eta-calculator
        ETASimple eta;
        // you can optinally specify update intervals
        eta.setPrintInterval(std::chrono::milliseconds(600));
        // tell it the total value to aim for
        eta.setTotal(total);
        for (size_t i = 1; i <= total; i += 3) {
            // update the current position
            eta.setCurrent(i);
            // eta has a timer that triggers this to be true when a certain interval has been reached, so you don't have to redraw on every item
            if (eta.shouldPrint()) {
                // print several progress infos
                std::cout << "Currently at " << o.p(to_raw_str(i), FG::Green) << "/" << o.p(to_raw_str(total), FG::Yellow) << " time passed: " << o.p(eta.renderTimePassed(), FG::Magenta) << ", time left: " << o.p(eta.renderTimeLeft(), FG::Cyan) << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(75));
        }

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}
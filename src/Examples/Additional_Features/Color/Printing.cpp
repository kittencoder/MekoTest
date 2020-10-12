#include "Meko.h"

using namespace Meko::Color;

void printColor() {
    auto& o = O::getInstance();
    // print some colorized text by passing colors to the printer
    std::cout << o.p("Green text", FG::Green) << " and " << o.p("red text with blue background", FG::Red, BG::Blue) << std::endl;
}

int main(int argc, char** argv) {
    try {
        // get the colorizer instance
        auto& o = O::getInstance();

        printColor();

        // you can globally disable color output (eg. when reading output by another script/application)
        o.disableColors();

        printColor();

        // and reenable colors again
        o.enableColors();

        printColor();
    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}
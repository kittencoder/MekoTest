#include "Meko.h"

using namespace Meko::Color;

void printColor(const Style& s1, const Style& s2) {
    auto& o = O::getInstance();
    // print some colorized text by passing colors to the printer
    std::cout << o.p("First part", s1) << " and " << o.p("second part", s2) << std::endl;
}
void printSelection(const Style& s) {
    auto& o = O::getInstance();
    // print some colorized text by passing colors to the printer
    std::cout << o.p("Some text with a ", s) << o.p("selected", s.flip()) << o.p(" part", s);
}

int main(int argc, char** argv) {
    try {
        Style s1(FG::Green);
        Style s2(FG::Blue, BG::Light_Red);
        Style s3(FG::Light_Red, BG::Yellow);
        printColor(s1, s2);
        printColor(s2, s3);
        printColor(s3, s1);
        printSelection(Style(FG::Yellow, BG::Magenta));
    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}
#include "Meko.h"

using namespace Meko::Support;

int main(int argc, char** argv) {
    try {
        // basically a vector of vector of strings
        TablePrinter::sheetType sheet;
        sheet.push_back({"Headline 1", "Headline 2", "Headline 3", "Headline 4"});
        sheet.push_back({"Data 1A", "Data 1B", "Data 1C", "Data 1D"});
        sheet.push_back({"Data 2A", "Data 2B", "Data 2C", "Data 2D"});
        sheet.push_back({"Data 3A", "Data 3B", "Data 3C", "Data 3D"});

        // print the table with default styling
        std::cout << "Default style:" << std::endl;
        std::cout << TablePrinter::getInstance().render(sheet) << std::endl;

        // pass a style configuration (the table-printer consumes/deletes this style by default, to prevent this, pass "false" as the next argument)
        std::cout << "Without headline:" << std::endl;
        std::cout << TablePrinter::getInstance().render(sheet, new TablePrinterStyle(false)) << std::endl;

        // render a complete grid between all cells
        std::cout << "with grid:" << std::endl;
        std::cout << TablePrinter::getInstance().render(sheet, new TablePrinterStyle(false, true)) << std::endl;

        // pass different frame-presets
        std::cout << "different presets:" << std::endl;
        std::cout << TablePrinter::getInstance().render(sheet, new TablePrinterStyle(true, false, TablePrinterStyle::simple)) << std::endl;
        std::cout << TablePrinter::getInstance().render(sheet, new TablePrinterStyle(false, false, TablePrinterStyle::thick)) << std::endl;

        std::cout << "custom styling:" << std::endl;
        // unset parts will stay empty
        std::cout << TablePrinter::getInstance().render(sheet, new TablePrinterStyle(false, false, {
                                                                                                       {"TopLeft", "/"},
                                                                                                       {"Top", "-"},
                                                                                                       {"Bottom", "_"},
                                                                                                       {"Left", "["},
                                                                                                       {"Right", "]"},
                                                                                                   }))
                  << std::endl;

        // even colors and different-length-rows in tables are supported
        using namespace Meko::Color;
        auto& o = O::getInstance();
        sheet.push_back({o.p("Data 1", FG::Green), o.p("Data ", FG::Cyan) + o.p(" two", FG::Blue, BG::Yellow), "Data 3"});
        std::cout << TablePrinter::getInstance().render(sheet, new TablePrinterStyle(false, false, TablePrinterStyle::thick)) << std::endl;
        sheet.push_back({o.p("Data 1", FG::Green), o.p("Data ", FG::Cyan) + o.p(" two", FG::Blue, BG::Yellow), "Data 3", "Data 4", o.p("one to many", FG::Magenta)});
        std::cout << TablePrinter::getInstance().render(sheet, new TablePrinterStyle(false, false, TablePrinterStyle::thick)) << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}

#include "../Meko.h"

namespace Meko::Color {

Style::Style(FG::Color fg, BG::Color bg) : fg(fg), bg(bg) {
}
Style Style::flip() const {
    return Style(static_cast<FG::Color>(bg - 10), static_cast<BG::Color>(fg + 10));
}
O::O() {
}
void O::disableColors() {
    enableColors(false);
}
void O::enableColors(bool on) {
    colorsEnabled = on;
}
bool O::getColorState() const {
    return colorsEnabled;
}
void O::disableDebugMode() {
    pre = "\e[";
    post = "m";
}
void O::enableDebugMode() {
    pre = "[\e[36m";
    post = "\e[0m]";
}
// print withg color
raw_str O::p(raw_str t, bool newline) const {
    return p(t, FG::Default_Color, newline);
}
raw_str O::p(raw_str t, FG::Color fg, bool newline) const {
    return p(t, fg, BG::Default_Color, newline);
}
raw_str O::p(raw_str t, Style s, bool newline) const {
    return p(t, s.fg, s.bg, newline);
}
raw_str O::p(raw_str t, FG::Color fg, BG::Color bg, bool newline) const {
    // print foreground if not FG::Default_Color
    // print background if not BG::Default_Color
    // print text
    // print reset-style if anything is set
    // print endline if requested
    std::stringstream oc;
    if (colorsEnabled) {
        oc << (fg != FG::Default_Color ? (raw_str(pre) + to_raw_str(fg) + post) : "")
           << (bg != BG::Default_Color ? (raw_str(pre) + to_raw_str(bg) + post) : "")
           << t
           << ((fg != FG::Default_Color || bg != BG::Default_Color) ? (pre + "0" + post) : "");
    } else {
        oc << t;
    }
    if (newline) {
        oc << std::endl;
    }
    return oc.str();
}

}  // namespace Meko::Color

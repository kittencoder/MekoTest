#ifndef COLOR_STYLE_H
#define COLOR_STYLE_H

#include "../Meko.h"

namespace Meko::Color {

namespace FG {
enum Color : int {
    Default_Color = 39,
    Black = 30,
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
    Magenta = 35,
    Cyan = 36,
    Light_gray = 37,
    Dark_Gray = 90,
    Light_Red = 91,
    Light_Green = 92,
    Light_Yellow = 93,
    Light_Blue = 94,
    Light_Magenta = 95,
    Light_Cyan = 96,
    White = 97,
};
}
namespace BG {
enum Color : int {
    Default_Color = 49,
    Black = 40,
    Red = 41,
    Green = 42,
    Yellow = 43,
    Blue = 44,
    Magenta = 45,
    Cyan = 46,
    Light_Gray = 47,
    Dark_Gray = 100,
    Light_Red = 101,
    Light_Green = 102,
    Light_Yellow = 103,
    Light_Blue = 104,
    Light_Magenta = 105,
    Light_Cyan = 106,
    White = 107,
};
}

class Style {
   public:
    Style(FG::Color fg = FG::Default_Color, BG::Color bg = BG::Default_Color);
    //Foreground and Background colors
    FG::Color fg;
    BG::Color bg;
    Style flip() const;
};
class O : public Support::Singleton<O> {
   public:
    O();
    /**
     * use when no colors should be produced
     */
    void disableColors();
    /**
     * use to re-enable color output
     */
    void enableColors(bool on = true);
    /**
     * check if colors are enabled/disabled
     */
    bool getColorState() const;
    /**
     * use to re-enable normal color output
     */
    void disableDebugMode();
    /**
     * output [color-code] instead of the actual escape-code for debug purposes
     */
    void enableDebugMode();
    // print withg color
    raw_str p(raw_str t, bool newline = false) const;
    raw_str p(raw_str t, FG::Color fg, bool newline = false) const;
    raw_str p(raw_str t, FG::Color fg, BG::Color bg, bool newline = false) const;
    raw_str p(raw_str t, Style s, bool newline = false) const;

   protected:
    raw_str pre = "\e[";
    raw_str post = "m";
    bool colorsEnabled = true;
};

}  // namespace Meko::Color

#endif
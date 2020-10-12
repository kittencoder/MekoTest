#ifndef SUPPORT_STRINGUTILITIES_H
#define SUPPORT_STRINGUTILITIES_H

#include "../Meko.h"

namespace Meko::Support {

// where to pad the string to
enum class PadDirection {
    // insert padding on the left
    PAD_LEFT,
    // insert padding on the right
    PAD_RIGHT,
    // insert padding on both sides
    PAD_BOTH
};

class StringUtilities {
   public:
    /**
     * pattern to find terminal escape-sequences used for color-output (to not include them in the width when padding a string)
     */
    const static raw_str escapeFinderPattern;
    static raw_str replace(raw_str_arg input, raw_str from, raw_str to);
    static raw_str replace(raw_str_arg input, std::unordered_map<raw_str, raw_str> fromTo);

    // render all escape sequences (like "\n" to a newline) https://en.cppreference.com/w/cpp/language/escape
    static raw_str execEscapes(raw_str_arg input);
    // inverse of execEscapes
    static raw_str unexecEscapes(raw_str_arg input);
    /**
    * c++ equivalent to the PHP-explode function
    */
    static std::vector<raw_str> explode(raw_str const& s, char const& delim);
    static raw_str implode(raw_str const& s, const std::vector<raw_str>& arr);
    static raw_str implode(raw_str const& s, const std::set<raw_str>& arr);

    static bool contains(raw_str const& s, char const& c);
    static bool contains(std::vector<raw_str> const& set, raw_str const& s);
    static raw_str pad(raw_str const& src, int width, PadDirection direction, char pad = ' ');

    // trim from start (in place)
    static void ltrim(raw_str& s);

    // trim from end (in place)
    static void rtrim(raw_str& s);

    // trim from both ends (in place)
    static void trim(raw_str& s);

    // trim from start (copying)
    static raw_str ltrim_copy(raw_str s);

    // trim from end (copying)
    static raw_str rtrim_copy(raw_str s);

    // trim from both ends (copying)
    static raw_str trim_copy(raw_str s);
    static raw_str uppercase(raw_str s);
    static raw_str lowercase(raw_str s);
    // uppercase everything expect the first 2 letters (eg. the 0x)
    static raw_str uppercase_hex(raw_str s);
    static raw_str uppercase_hex(int v);
    static raw_str getPointer(const void* ptr);
    template <class T>
    static raw_str getHex(T num) {
        std::stringstream ss;
        ss << std::hex << num;
        return StringUtilities::uppercase(ss.str());
    }
    template <class T>
    static raw_str getHexWithPrefix(T num) {
        return raw_str("0x") + getHex(num);
    }

    // returns a string only containing letters from a-z
    static raw_str az(raw_str s);
    // returns a string only containing letters from a-z and A-Z
    static raw_str azAZ(raw_str s);
    // returns a string only containing letters from a-z and 0-9
    static raw_str az09(raw_str s);
    // returns a string only containing letters from a-z, A-Z and 0-9
    static raw_str azAZ09(raw_str s);
    static raw_str filter(raw_str s, std::vector<raw_str> allow);
    static raw_str dumpStream(std::istream& is);
};

class AutoPadder {
   public:
    AutoPadder(char padChar = ' ', PadDirection direction = PadDirection::PAD_RIGHT);
    /**
      * set padding mode
      */
    void setDirection(PadDirection newDirection);
    raw_str pad(raw_str const& s);
    raw_str operator()(raw_str const& s);
    /**
     * show that string to this padder so it might adjust its width
     */
    void show(raw_str const& s);
    /**
     * show several strings
     */
    void show(const std::vector<raw_str>& s);
    void show(const std::set<raw_str>& s);
    /**
     * show that width to this padder so it might adjust its width
     */
    void show(size_t w);
    void setPadChar(char newC);
    size_t getWidth() const;

   protected:
    char c;
    PadDirection direction;
    size_t maxwidth = 0;
};

}  // namespace Meko::Support

#endif
#include "../Meko.h"

/**
 * does not need implementation
 * yet this source file is for consistent structuring
 */
namespace Meko::Support {
const raw_str StringUtilities::escapeFinderPattern = "[[:cntrl:]]\\[[0-9]{1,3}m";
raw_str StringUtilities::execEscapes(raw_str_arg input) {
    static std::unordered_map<raw_str, raw_str> escapes = {
        {"\\\\", "\\"},
        {"\\\"", "\""},
        {"\\'", "\'"},
        {"\\?", "\?"},
        {"\\a", "\a"},
        {"\\b", "\b"},
        {"\\f", "\f"},
        {"\\n", "\n"},
        {"\\r", "\r"},
        {"\\t", "\t"},
        {"\\v", "\v"},
        {"\\0", "\0"},  // todo: support octal, hexadecimal and unicode escape codes
    };
    return replace(input, escapes);
}
raw_str StringUtilities::unexecEscapes(raw_str_arg input) {
    static std::unordered_map<raw_str, raw_str> escapes = {
        {"\"", "\\\""},
        {"\'", "\\'"},
        {"\?", "\\?"},
        {"\a", "\\a"},
        {"\b", "\\b"},
        {"\f", "\\f"},
        {"\n", "\\n"},
        {"\r", "\\r"},
        {"\t", "\\t"},
        {"\v", "\\v"},
        {"\\", "\\\\"},
        // {"\0", "\\0"}, // breaks the replace-function and creates an infinite loop
    };
    return replace(input, escapes);
}
raw_str StringUtilities::replace(raw_str_arg input, raw_str from, raw_str to) {
    raw_str str = input;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != raw_str::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();  // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
raw_str StringUtilities::replace(raw_str_arg input, std::unordered_map<raw_str, raw_str> fromTo) {
    raw_str t = input;
    for (auto& i : fromTo) {
        t = replace(t, i.first, i.second);
    }
    return t;
}

std::vector<raw_str> StringUtilities::explode(raw_str const& s, char const& delim) {
    std::vector<raw_str> result;
    std::istringstream iss(s);

    for (raw_str token; std::getline(iss, token, delim);) {
        result.push_back(std::move(token));
    }

    return result;
}
raw_str StringUtilities::implode(raw_str const& s, const std::vector<raw_str>& arr) {
    std::stringstream result;
    bool first = true;
    for (auto i : arr) {
        result << ((!first) ? s : "") << i;
        first = false;
    }
    return result.str();
}
raw_str StringUtilities::implode(raw_str const& s, const std::set<raw_str>& arr) {
    std::stringstream result;
    bool first = true;
    for (auto i : arr) {
        result << ((!first) ? s : "") << i;
        first = false;
    }
    return result.str();
}
bool StringUtilities::contains(raw_str const& s, char const& c) {
    return s.find(c) != raw_str::npos;
}
bool StringUtilities::contains(std::vector<raw_str> const& set, raw_str const& s) {
    return std::find(set.begin(), set.end(), s) != set.end();
}

raw_str StringUtilities::pad(raw_str const& src, int width, PadDirection direction, char pad) {
    static std::regex escapeFinder(escapeFinderPattern);
    raw_str filtered;
    std::regex_replace(std::back_inserter(filtered), src.begin(), src.end(), escapeFinder, "");
    int toPad = width - filtered.size();
    if (toPad <= 0) {
        return src;
    }
    switch (direction) {
        case PadDirection::PAD_BOTH: {
            int toPadRight = toPad / 2;
            toPad -= toPadRight;
            return raw_str(toPad, pad) + src + raw_str(toPadRight, pad);
        }
        case PadDirection::PAD_LEFT: {
            return raw_str(toPad, pad) + src;
        }
        case PadDirection::PAD_RIGHT: {
            return src + raw_str(toPad, pad);
        }
    }
    // will never be reached, yet this return statement is there to calm the compiler down
    return src;
}

void StringUtilities::ltrim(raw_str& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void StringUtilities::rtrim(raw_str& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace)))
                .base(),
            s.end());
}

void StringUtilities::trim(raw_str& s) {
    ltrim(s);
    rtrim(s);
}

raw_str StringUtilities::ltrim_copy(raw_str s) {
    ltrim(s);
    return s;
}

raw_str StringUtilities::rtrim_copy(raw_str s) {
    rtrim(s);
    return s;
}

raw_str StringUtilities::trim_copy(raw_str s) {
    trim(s);
    return s;
}
raw_str StringUtilities::uppercase(raw_str s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}
raw_str StringUtilities::lowercase(raw_str s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
raw_str StringUtilities::uppercase_hex(raw_str s) {
    std::transform(s.begin() + 2, s.end(), s.begin() + 2, ::toupper);
    return s;
}
raw_str StringUtilities::uppercase_hex(int v) {
    std::stringstream stream;
    stream << "0x" << std::hex << v;
    raw_str s(stream.str());
    std::transform(s.begin() + 2, s.end(), s.begin() + 2, ::toupper);
    return s;
}

raw_str StringUtilities::getPointer(const void* ptr) {
    const void* address = static_cast<const void*>(ptr);
    std::stringstream ss;
    ss << address;
    return StringUtilities::uppercase_hex(ss.str());
}

raw_str StringUtilities::az(raw_str s) {
    static raw_str preset = "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z";
    static auto lst = explode(preset, ',');
    return filter(s, lst);
}
raw_str StringUtilities::azAZ(raw_str s) {
    static raw_str preset1 = "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z";
    static raw_str preset2 = preset1 + "," + uppercase(preset1);
    static auto lst = explode(preset2, ',');
    return filter(s, lst);
}
raw_str StringUtilities::az09(raw_str s) {
    static raw_str preset = "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,0,1,2,3,4,5,6,7,8,9";
    static auto lst = explode(preset, ',');
    return filter(s, lst);
}
raw_str StringUtilities::azAZ09(raw_str s) {
    static raw_str preset1 = "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z";
    static raw_str preset2 = preset1 + "," + uppercase(preset1) + ",0,1,2,3,4,5,6,7,8,9";
    static auto lst = explode(preset2, ',');
    return filter(s, lst);
}
raw_str StringUtilities::filter(raw_str s, std::vector<raw_str> allow) {
    std::stringstream ss;
    for (auto i : s) {
        if (std::find(allow.begin(), allow.end(), raw_str({i})) != allow.end()) {
            ss << i;
        }
    }
    return ss.str();
}
raw_str StringUtilities::dumpStream(std::istream& is) {
    return raw_str((std::istreambuf_iterator<char>(is)),
                   (std::istreambuf_iterator<char>()));
}

AutoPadder::AutoPadder(char padChar, PadDirection direction) {
    setDirection(direction);
    setPadChar(padChar);
}
void AutoPadder::setDirection(PadDirection newDirection) {
    direction = newDirection;
}
raw_str AutoPadder::pad(raw_str const& s) {
    show(s);
    return StringUtilities::pad(s, maxwidth, direction, c);
}
raw_str AutoPadder::operator()(raw_str const& s) {
    return pad(s);
}
void AutoPadder::show(const std::vector<raw_str>& s) {
    for (const auto& i : s) {
        show(i);
    }
}
void AutoPadder::show(const std::set<raw_str>& s) {
    for (const auto& i : s) {
        show(i);
    }
}
void AutoPadder::show(raw_str const& s) {
    static std::regex escapeFinder(StringUtilities::escapeFinderPattern);
    raw_str filtered;
    std::regex_replace(std::back_inserter(filtered), s.begin(), s.end(), escapeFinder, "");
    show(filtered.size());
}

size_t AutoPadder::getWidth() const {
    return maxwidth;
}
void AutoPadder::show(size_t w) {
    maxwidth = std::max(maxwidth, w);
}
void AutoPadder::setPadChar(char newC) {
    c = newC;
}

}  // namespace Meko::Support

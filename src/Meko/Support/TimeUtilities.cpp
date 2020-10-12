
#include "../Meko.h"

namespace Meko::Support {

typedef std::chrono::nanoseconds tns;
typedef std::chrono::milliseconds tms;
typedef std::chrono::seconds ts;
typedef std::chrono::minutes tm;
typedef std::chrono::hours th;

raw_str TimeUtilities::renderSimple(time_hr time, bool precise) {
    auto ns = std::chrono::duration_cast<tns>(time);
    auto ms = std::chrono::duration_cast<tms>(ns);
    ns -= ms;
    auto s = std::chrono::duration_cast<ts>(ms);
    ms -= s;
    auto m = std::chrono::duration_cast<tm>(s);
    s -= m;
    auto h = std::chrono::duration_cast<th>(m);
    m -= h;
    raw_str r;

    bool doPrint = false;
    if (h.count() != 0) {
        doPrint = true;
    }
    if (doPrint) {
        r += to_raw_str(h.count()) + "h ";
    }
    if (m.count() != 0) {
        doPrint = true;
    }
    if (doPrint) {
        r += to_raw_str(m.count()) + "m ";
    }
    if (s.count() != 0) {
        doPrint = true;
    }
    bool hadSeconds = false;
    if (doPrint) {
        r += to_raw_str(s.count()) + "s";
        hadSeconds = true;
        if (!precise) {
            return r;
        }
    }
    if (ms.count() != 0) {
        doPrint = true;
    }
    if (doPrint) {
        if (hadSeconds) {
            r += " ";
        }
        r += to_raw_str(ms.count());
        if (precise && ns.count() != 0) {
            std::string afterComma = to_raw_str(ns.count());
            afterComma.erase(std::remove_if(afterComma.rbegin(), afterComma.rend(),
                                            [](char c) {
                                                return c != '0';
                                            })
                                 .base(),
                             afterComma.end());
            r += "." + afterComma;
        }
        r += "ms";
    } else {
        // even without "precise" we want to output...
        r += to_raw_str(ns.count()) + "ns";
    }
    return r;
}
ETASimple::ETASimple(const time_hr& iv) : interval(iv) {
    start();
}
raw_str
ETASimple::renderTimePassed(bool precise) {
    return TimeUtilities::renderSimple(getTimePassed(), precise);
}
raw_str ETASimple::renderTimeLeft(bool precise) {
    return TimeUtilities::renderSimple(getTimeLeft(), precise);
}
time_hr ETASimple::getTimeLeft() {
    auto passed = DC_HR(getTimePassed());
    lastUpdate = clock::now();
    auto left = total - current;
    if (current == 0 || left == 0) {
        return time_hr::zero();
    }
    passed *= left;
    passed /= current;
    return passed;
}
time_hr ETASimple::getTimePassed() {
    return clock::now() - startedAt;
}
bool ETASimple::shouldPrint() {
    if (current == total) {
        return true;
    }
    return interval < (clock::now() - lastUpdate);
}
ETASimple& ETASimple::setPrintInterval(const time_hr& pi) {
    interval = pi;
    return *this;
}
ETASimple& ETASimple::setTotal(size_t t) {
    total = t;
    return *this;
}
ETASimple& ETASimple::setCurrent(size_t c) {
    current = c;
    return *this;
}
ETASimple& ETASimple::start() {
    lastUpdate = startedAt = clock::now();
    return *this;
}

}  // namespace Meko::Support
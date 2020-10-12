#ifndef SUPPORT_TIMEGUTILITIES_H
#define SUPPORT_TIMEGUTILITIES_H

#include "../Meko.h"

namespace Meko::Support {

class TimeUtilities {
   public:
    static raw_str renderSimple(time_hr time, bool precise = false);
};
/**
 * calculate an ETA (time left until current reaches total if progression remains linear)
 */
class ETASimple {
   public:
    ETASimple(const time_hr& interval = std::chrono::seconds(1));
    /**
     * set the starting time point to "now"
     */
    ETASimple& start();
    /**
     * update the goal
     */
    ETASimple& setTotal(size_t total);
    /**
     * update the current state
     */
    ETASimple& setCurrent(size_t current);
    /**
     * how often to report "print now"
     */
    ETASimple& setPrintInterval(const time_hr& interval = std::chrono::seconds(1));
    /**
     * interval over?
     */
    bool shouldPrint();
    /**
     * calculate how much time is left (and update reference for "shouldPrint")
     */
    time_hr getTimeLeft();
    raw_str renderTimeLeft(bool precise = false);
    /**
     * get the current execution time
     */
    time_hr getTimePassed();
    raw_str renderTimePassed(bool precise = false);

   protected:  // eta calculation
    size_t total;
    size_t current;
    time_hr interval;
    clock::time_point startedAt;
    clock::time_point lastUpdate;
};
}  // namespace Meko::Support
#endif
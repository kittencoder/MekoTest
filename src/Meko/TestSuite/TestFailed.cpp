
#include "../Meko.h"

namespace Meko::TestSuite {

TestFailed::TestFailed(raw_str_arg reason, raw_str_arg details) : reason(reason),
                                                                  details(details) {
}
raw_str TestFailed::getReason() const {
    return reason;
}
raw_str TestFailed::getDetails() const {
    return details;
}
}  // namespace Meko::TestSuite
#ifndef SUPPORT_NONCOPYABLE_H
#define SUPPORT_NONCOPYABLE_H

#include "../Meko.h"

namespace Meko::Support {

class non_copyable {
   protected:
    non_copyable(non_copyable &&) = default;
    non_copyable() = default;
    ~non_copyable() = default;

    non_copyable(non_copyable const &) = delete;
    void operator=(non_copyable const &) = delete;
};
}  // namespace Meko::Support
#endif
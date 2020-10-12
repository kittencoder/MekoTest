#ifndef MEKO_EXEC_EXITCLEANUP_H
#define MEKO_EXEC_EXITCLEANUP_H

#include "../Meko.h"

namespace Meko::Exec {
/**
 * thrown by Meko::Exec::Run
 * to terminate the application in a way that allows descruction and free's to happen
 * 
 * a try-catch-block may be added around the main-function to trigger RAII destructors along the way
 */
class ExitCleanup : public std::exception {
   public:
};
}  // namespace Meko::Exec
#endif
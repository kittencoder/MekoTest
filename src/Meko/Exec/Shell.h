#ifndef MEKO_EXEC_SHELL_H
#define MEKO_EXEC_SHELL_H

#include "../Meko.h"

namespace Meko::Exec {
/**
 * run something in a shell (usually `sh -c`)
 */
class Shell : public RunProcess {
   public:
    Shell(raw_str_arg command, raw_str_arg shell = DEFAULT_RUN_SHELL, raw_str_arg shellExecArg = DEFAULT_RUN_SHELL_ARGS);
};
}  // namespace Meko::Exec
#endif
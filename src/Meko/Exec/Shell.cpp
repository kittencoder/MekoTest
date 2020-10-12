
#include "../Meko.h"

namespace Meko::Exec {
Shell::Shell(raw_str_arg command, raw_str_arg shell, raw_str_arg shellExecArg) : RunProcess(shell, {shellExecArg, command}) {
}
}
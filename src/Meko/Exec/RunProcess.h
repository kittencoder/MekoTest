#ifndef MEKO_EXEC_RUNPROCESS_H
#define MEKO_EXEC_RUNPROCESS_H

#include "../Meko.h"

namespace Meko::Exec {
/**
 * run a process using fork and exec
 */
class RunProcess : public Run {
   public:
    RunProcess(raw_str_arg command, const std::vector<raw_str>& arguments = {});
    ~RunProcess();

   protected:
    raw_str command;
    std::vector<raw_str> arguments;
    char** rawArguments = nullptr;
    void EXEC() override;
    char** buildArguments();
};
}  // namespace Meko::Exec
#endif
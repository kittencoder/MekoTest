
#include "../Meko.h"

namespace Meko::Exec {
RunProcess::RunProcess(raw_str_arg command, const std::vector<raw_str>& arguments) : command(command),
                                                                                     arguments(arguments) {
}
void RunProcess::EXEC() {
    char** argv_list = buildArguments();
    int rv = execvp(argv_list[0], argv_list);
    if (rv == -1) {
        implOk = false;
    }
}

RunProcess::~RunProcess() {
    if (rawArguments != nullptr) {
        // rawArguments is 1 bigger than arguments, yet the last element is a nullptr
        for (size_t i = 0; i < arguments.size(); ++i) {
            delete[] rawArguments[i];
        }
        delete[] rawArguments;
    }
}

char** RunProcess::buildArguments() {
    if (rawArguments == nullptr) {
        arguments.insert(arguments.begin(), command);

        // account for null-byte-entry
        rawArguments = new char*[arguments.size() + 1];
        for (size_t i = 0; i < arguments.size(); ++i) {
            size_t argLen = arguments[i].length();
            // account for null-byte
            rawArguments[i] = new char[argLen + 1];
            strcpy(rawArguments[i], arguments[i].c_str());
        }
        rawArguments[arguments.size()] = nullptr;
    }
    return rawArguments;
}
}  // namespace Meko::Exec
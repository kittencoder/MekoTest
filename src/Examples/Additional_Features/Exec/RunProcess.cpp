#include "Meko.h"

using namespace Meko;
using namespace Meko::Exec;

int main(int argc, char** argv) {
    try {
        // run an application and pass arguments
        RunProcess rp("ls", {"-l", "-a", "-h", "--color"});
        std::cout << "result of RunProcess 'ls -l -a -h':" << std::endl
                  << (raw_str)rp << std::endl;
    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}